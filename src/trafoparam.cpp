#include "trafoparam.h"
/*
#include "coordinatesystem.h"
#include "statistic.h"
#include "function.h"

TrafoParam::TrafoParam(QObject *parent) : Feature(parent), homogenMatrix(4, 4), translation(3), rotation(3), scale(3), quaternion(4)
{
    //this->id = Configuration::generateID();
    this->isUpdated = false;
    this->use = false;
    this->validTime = QDateTime::currentDateTime();
    this->isMovement = false;
    this->isDatumTrafo = false;
    this->scale.setAt(0, 1.0);
    this->scale.setAt(1, 1.0);
    this->scale.setAt(2, 1.0);
    this->myStatistic = new Statistic;
    this->myStatistic->isValid = true;

    OiMat e(4,4);

    for(int i=0; i<4; i++){
        e.setAt(i,i,1.0);
    }
    this->setHomogenMatrix(e,e,e);
}

TrafoParam::~TrafoParam(){

    //delete this trafo set in from system
    if(this->from != NULL){
        this->from->removeTransformationParameter(this);
    }

    //delete this trafo set in to system
    if(this->to != NULL){
        this->to->removeTransformationParameter(this);
    }

}

OiMat TrafoParam::getHomogenMatrix() const{
    return this->homogenMatrix;
}

void TrafoParam::recalc(){
     //isDefined -> becomes true as soon as the first function of a feature has been executed, which defines the feature
     //isSolved -> is true as long as there isn't any function which cannot be successfully executed

    bool isDefined = false;
    foreach(Function *f, this->functionList){
        if(!isDefined){
            this->isSolved = f->exec(*this);
            isDefined = true;
        }else if(this->isSolved){
            this->isSolved = f->exec(*this);
        }
    }
}

//Sets the transformation parameters. Input are 3 4x4 homogeneous matrices
bool TrafoParam::setHomogenMatrix(OiMat rotation, OiMat translation, OiMat scale){

    if(rotation.getColCount() == 4 && rotation.getRowCount() == 4
            && translation.getColCount() == 4 && translation.getRowCount() == 4
            && scale.getColCount() == 4 && scale.getRowCount() == 4){

        OiVec t(3);
        OiVec r(3);
        OiVec s(3);
        for(int i = 0; i < 3; i++){
            t.setAt(i, translation.getAt(i, 3));
            s.setAt(i, scale.getAt(i, i));
        }

        r.setAt(0, qAtan2(-rotation.getAt(2,1), rotation.getAt(2,2))); //alpha
        r.setAt(1, qAsin(rotation.getAt(2,0))); //beta
        r.setAt(2, qAtan2(-rotation.getAt(1,0), rotation.getAt(0,0))); //gamma
        if( qFabs(qCos(r.getAt(1)) * qCos(r.getAt(2))) - qFabs(rotation.getAt(0,0)) > 0.01 ){
            r.setAt(1, 3.141592653589793 - r.getAt(1));
        }

        this->translation = t;
        this->scale = s;
        this->rotation = r;

        this->homogenMatrix = translation * rotation * scale;

    }else{
        return false;
    }

    emit this->transformationParameterChanged(this->id);
    return true;
}

OiVec TrafoParam::getTranslation() const{
    return this->translation;
}

OiVec TrafoParam::getRotation() const{
    return this->rotation;
}

OiVec TrafoParam::getScale() const{
    return this->scale;
}

OiVec TrafoParam::getQuaternion() const{
    return this->quaternion;
}

bool TrafoParam::setQuaternion(OiVec quaternion){
    if(quaternion.getSize() == 4){
        this->quaternion = quaternion;
        return true;
    }
    return false;
}

const QPointer<CoordinateSystem> &TrafoParam::getStartSystem() const{
    return this->from;
}

const QPointer<CoordinateSystem> &TrafoParam::getDestinationSystem() const{
    return this->to;
}

bool TrafoParam::setCoordinateSystems(const QPointer<CoordinateSystem> &from, const QPointer<CoordinateSystem> &to){
    if(!from.isNull() && !to.isNull()){
        this->from = from;
        this->to = to;
        emit this->coordinateSystemsChanged(this->id);
        return true;
    }
    return false;
}

bool TrafoParam::getIsUsed() const{
    return this->use;
}

void TrafoParam::setIsUsed(const bool &isUsed){
    this->use = isUsed;
    emit this->isUsedChanged(this->id);
}

QDateTime TrafoParam::getValidTime(){
    return this->validTime;
}

void TrafoParam::setValidTime(QDateTime validTime){
    this->validTime = validTime;
    emit this->validTimeChanged(this->id);
}

bool TrafoParam::getIsMovement(){
    return this->isMovement;
}

void TrafoParam::setIsMovement(bool isMovement){
    this->isMovement = isMovement;
    emit this->isMovementChanged(this->id);
}

Statistic *TrafoParam::getStatistic() const{
    return this->myStatistic;
}

void TrafoParam::setStatistic(Statistic *myStatistic){
    if(myStatistic != NULL){
        if(this->myStatistic != NULL){
            //delete this->myStatistic;
            this->myStatistic = NULL;
        }
        this->myStatistic = myStatistic;
    }
}

bool TrafoParam::getisDatumTrafo()
{
    return this->isDatumTrafo;
}

void TrafoParam::setisDatumTrafo(bool isDatumTrafo)
{
    this->isDatumTrafo =isDatumTrafo;
}

QDomElement TrafoParam::toOpenIndyXML(QDomDocument &xmlDoc){

    QDomElement trafoParam = Feature::toOpenIndyXML(xmlDoc);

    if(trafoParam.isNull()){
        return trafoParam;
    }

    trafoParam.setTagName("transformationParameter");

    //add trafoParam attributes
    if(this->translation.getSize() >= 3 && this->rotation.getSize() >= 3 && this->getScale().getSize() >= 3){
        trafoParam.setAttribute("tx", this->translation.getAt(0));
        trafoParam.setAttribute("ty", this->translation.getAt(1));
        trafoParam.setAttribute("tz", this->translation.getAt(2));
        trafoParam.setAttribute("rx", this->rotation.getAt(0));
        trafoParam.setAttribute("ry", this->rotation.getAt(1));
        trafoParam.setAttribute("rz", this->rotation.getAt(2));
        trafoParam.setAttribute("mx", this->scale.getAt(0));
        trafoParam.setAttribute("my", this->scale.getAt(1));
        trafoParam.setAttribute("mz", this->scale.getAt(2));
    }
    trafoParam.setAttribute("movement", this->isMovement);
    trafoParam.setAttribute("datumtrafo", this->isDatumTrafo);
    trafoParam.setAttribute("use", this->use);
    trafoParam.setAttribute("time", this->validTime.toString(Qt::ISODate));

    //add from and to coordinate systems
    if(this->from != NULL && this->to != NULL){
        QDomElement from = xmlDoc.createElement("from");
        from.setAttribute("ref", this->from->getId());
        trafoParam.appendChild(from);
        QDomElement to = xmlDoc.createElement("to");
        to.setAttribute("ref", this->to->getId());
        trafoParam.appendChild(to);
    }

    return trafoParam;

}

bool TrafoParam::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Feature::fromOpenIndyXML(xmlElem);

    if(result){

        //set trafoParam attributes
        if(!xmlElem.hasAttribute("tx") || !xmlElem.hasAttribute("ty") || !xmlElem.hasAttribute("tz")
                || !xmlElem.hasAttribute("rx") || !xmlElem.hasAttribute("ry") || !xmlElem.hasAttribute("rz")
                || !xmlElem.hasAttribute("mx") || !xmlElem.hasAttribute("my") || !xmlElem.hasAttribute("mz")
                || !xmlElem.hasAttribute("use") || !xmlElem.hasAttribute("movement") || !xmlElem.hasAttribute("datumtrafo")
                || !xmlElem.hasAttribute("time")){
            return false;
        }
        this->translation.setAt(0, xmlElem.attribute("tx").toDouble());
        this->translation.setAt(1, xmlElem.attribute("ty").toDouble());
        this->translation.setAt(2, xmlElem.attribute("tz").toDouble());
        this->rotation.setAt(0, xmlElem.attribute("rx").toDouble());
        this->rotation.setAt(1, xmlElem.attribute("ry").toDouble());
        this->rotation.setAt(2, xmlElem.attribute("rz").toDouble());
        this->scale.setAt(0, xmlElem.attribute("mx").toDouble());
        this->scale.setAt(1, xmlElem.attribute("my").toDouble());
        this->scale.setAt(2, xmlElem.attribute("mz").toDouble());
        this->use = xmlElem.attribute("use").toInt();
        this->isMovement = xmlElem.attribute("movement").toInt();
        this->isDatumTrafo = xmlElem.attribute("datumtrafo").toInt();
        this->validTime = QDateTime::fromString(xmlElem.attribute("time"), Qt::ISODate);

        //calculate homogeneous matrix
        OiMat tmpTranslation(4,4);
        OiMat tmpRotation(4,4);
        OiMat tmpScale(4,4);

        tmpTranslation.setAt(0,0,1.0);
        tmpTranslation.setAt(0,3,this->translation.getAt(0));
        tmpTranslation.setAt(1,1,1.0);
        tmpTranslation.setAt(1,3,this->translation.getAt(1));
        tmpTranslation.setAt(2,2,1.0);
        tmpTranslation.setAt(2,3,this->translation.getAt(2));
        tmpTranslation.setAt(3,3,1.0);

        tmpScale.setAt(0,0,this->scale.getAt(0));
        tmpScale.setAt(1,1,this->scale.getAt(1));
        tmpScale.setAt(2,2,this->scale.getAt(2));
        tmpScale.setAt(3,3,1.0);

        tmpRotation.setAt(0,0,qCos(this->rotation.getAt(1))*qCos(this->rotation.getAt(2)));
        tmpRotation.setAt(0,1,qCos(this->rotation.getAt(0))*qSin(this->rotation.getAt(2))+qSin(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qCos(this->rotation.getAt(2)));
        tmpRotation.setAt(0,2,qSin(this->rotation.getAt(0))*qSin(this->rotation.getAt(2))-qCos(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qCos(this->rotation.getAt(2)));
        tmpRotation.setAt(1,0,-qCos(this->rotation.getAt(1))*qSin(this->rotation.getAt(2)));
        tmpRotation.setAt(1,1,qCos(this->rotation.getAt(0))*qCos(this->rotation.getAt(2))-qSin(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qSin(this->rotation.getAt(2)));
        tmpRotation.setAt(1,2,qSin(this->rotation.getAt(0))*qCos(this->rotation.getAt(2))+qCos(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qSin(this->rotation.getAt(2)));
        tmpRotation.setAt(2,0,qSin(this->rotation.getAt(1)));
        tmpRotation.setAt(2,1,-qSin(this->rotation.getAt(0))*qCos(this->rotation.getAt(1)));
        tmpRotation.setAt(2,2,qCos(this->rotation.getAt(0))*qCos(this->rotation.getAt(1)));
        tmpRotation.setAt(3,3,1.0);

        this->homogenMatrix = tmpTranslation*tmpScale*tmpRotation;

    }

    return result;

}

QString TrafoParam::getDisplayStartSystem() const{
    return this->from->getFeatureName();
}

QString TrafoParam::getDisplayDestinationSystem() const{
    return this->to->getFeatureName();
}

QString TrafoParam::getDisplayTranslationX() const
{
    double displayValue = this->translation.getAt(0);
    this->convertMetricValue(displayValue);
    return QString::number(displayValue, 'f', this->getMetricDigits());
    //return QString::number(this->translation.getAt(0)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
}

QString TrafoParam::getDisplayTranslationY() const
{
    double displayValue = this->translation.getAt(0);
    this->convertMetricValue(displayValue);
    return QString::number(displayValue, 'f', this->getMetricDigits());
    //return QString::number(this->translation.getAt(1)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
}

QString TrafoParam::getDisplayTranslationZ() const
{
    double displayValue = this->translation.getAt(2);
    this->convertMetricValue(displayValue);
    return QString::number(displayValue, 'f', this->getMetricDigits());
    //return QString::number(this->translation.getAt(2)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
}

QString TrafoParam::getDisplayRotationX() const
{
    double displayValue = this->rotation.getAt(0);
    this->convertAngularValue(displayValue);
    return QString::number(displayValue, 'f', this->getAngularDigits());
    //return QString::number(this->rotation.getAt(0)*OiUnitConverter::getAngleMultiplier(),'f',OiUnitConverter::angleDigits);
}

QString TrafoParam::getDisplayRotationY() const
{
    double displayValue = this->rotation.getAt(1);
    this->convertAngularValue(displayValue);
    return QString::number(displayValue, 'f', this->getAngularDigits());
    //return QString::number(this->rotation.getAt(1)*OiUnitConverter::getAngleMultiplier(),'f',OiUnitConverter::angleDigits);
}

QString TrafoParam::getDisplayRotationZ() const
{
    double displayValue = this->rotation.getAt(2);
    this->convertAngularValue(displayValue);
    return QString::number(displayValue, 'f', this->getAngularDigits());
    //return QString::number(this->rotation.getAt(2)*OiUnitConverter::getAngleMultiplier(),'f',OiUnitConverter::angleDigits);
}

QString TrafoParam::getDisplayScaleX() const
{
    return QString::number(this->scale.getAt(0),'f',6);
}

QString TrafoParam::getDisplayScaleY() const
{
    return QString::number(this->scale.getAt(1),'f',6);
}

QString TrafoParam::getDisplayScaleZ() const
{
    return QString::number(this->scale.getAt(2),'f',6);
}

QString TrafoParam::getDisplayUse() const
{
    return QString(this->use?"true":"false");
}

QString TrafoParam::getDisplayTime() const
{
    return this->validTime.toString(Qt::ISODate);
}

QString TrafoParam::getDisplayStdDev() const
{
    if(this->myStatistic != NULL && this->myStatistic->isValid){
        //return QString::number(this->myStatistic->stdev*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
    }
    return "statistic not valid";
}
*/

#include "featurewrapper.h"

/*!
 * \brief TrafoParam::TrafoParam
 * \param parent
 */
TrafoParam::TrafoParam(QObject *parent) : Feature(parent), scale(3), rotation(3), translation(3), homogenMatrix(4, 4){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setTrafoParam(this);
    }

    //default values for trafo param attributes
    this->isUsed = false;
    this->validTime = QDateTime::currentDateTime();
    this->isMovement = false;
    this->isDatumTrafo = false;

    //default values for transformation parameters
    this->scale.setAt(0, 1.0);
    this->scale.setAt(1, 1.0);
    this->scale.setAt(2, 1.0);
    this->homogenMatrix.setAt(0, 0, 1.0);
    this->homogenMatrix.setAt(1, 1, 1.0);
    this->homogenMatrix.setAt(2, 2, 1.0);
    this->homogenMatrix.setAt(3, 3, 1.0);

}

/*!
 * \brief TrafoParam::TrafoParam
 * \param copy
 * \param parent
 */
TrafoParam::TrafoParam(const TrafoParam &copy, QObject *parent) : Feature(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setTrafoParam(this);
    }

    //copy attributes
    this->from = copy.from;
    this->to = copy.to;
    this->isUsed = copy.isUsed;
    this->validTime = copy.validTime;
    this->isMovement = copy.isMovement;
    this->isDatumTrafo = copy.isDatumTrafo;

    //copy transformation parameters
    this->rotation = copy.rotation;
    this->translation = copy.translation;
    this->scale = copy.scale;
    this->homogenMatrix = copy.homogenMatrix;

}

/*!
 * \brief TrafoParam::operator =
 * \param copy
 * \return
 */
TrafoParam &TrafoParam::operator=(const TrafoParam &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setTrafoParam(this);
    }

    //copy attributes
    this->from = copy.from;
    this->to = copy.to;
    this->isUsed = copy.isUsed;
    this->validTime = copy.validTime;
    this->isMovement = copy.isMovement;
    this->isDatumTrafo = copy.isDatumTrafo;

    //copy transformation parameters
    this->rotation = copy.rotation;
    this->translation = copy.translation;
    this->scale = copy.scale;
    this->homogenMatrix = copy.homogenMatrix;

    return *this;

}

/*!
 * \brief TrafoParam::~TrafoParam
 */
TrafoParam::~TrafoParam(){

}

/*!
 * \brief TrafoParam::getIsUsed
 * \return
 */
const bool &TrafoParam::getIsUsed() const{
    return this->isUsed;
}

/*!
 * \brief TrafoParam::setIsUsed
 * \param isUsed
 */
void TrafoParam::setIsUsed(const bool &isUsed){
    if(this->isUsed != isUsed){
        this->isUsed = isUsed;
        emit this->isUsedChanged(this->id);
    }
}

/*!
 * \brief TrafoParam::getStartSystem
 * \return
 */
const QPointer<CoordinateSystem> &TrafoParam::getStartSystem() const{
    return this->from;
}

/*!
 * \brief TrafoParam::getDestinationSystem
 * \return
 */
const QPointer<CoordinateSystem> &TrafoParam::getDestinationSystem() const{
    return this->to;
}

/*!
 * \brief TrafoParam::setCoordinateSystems
 * \param from
 * \param to
 * \return
 */
bool TrafoParam::setCoordinateSystems(const QPointer<CoordinateSystem> &from, const QPointer<CoordinateSystem> &to){

    if(!from.isNull() && !to.isNull()){

        this->from = from;
        this->to = to;

        emit this->coordinateSystemsChanged(this->id);

        return true;

    }

    return false;
}

/*!
 * \brief TrafoParam::getIsMovement
 * \return
 */
const bool &TrafoParam::getIsMovement() const{
    return this->isMovement;
}

/*!
 * \brief TrafoParam::setIsMovement
 * \param isMovement
 */
void TrafoParam::setIsMovement(const bool &isMovement){
    if(this->isMovement != isMovement){
        this->isMovement = isMovement;
        emit this->isMovementChanged(this->id);
    }
}

/*!
 * \brief TrafoParam::getValidTime
 * \return
 */
const QDateTime &TrafoParam::getValidTime() const{
    return this->validTime;
}

/*!
 * \brief TrafoParam::setValidTime
 * \param validTime
 */
void TrafoParam::setValidTime(const QDateTime &validTime){
    if(this->validTime != validTime){
        this->validTime = validTime;
        emit this->validTimeChanged(this->id);
    }
}

/*!
 * \brief TrafoParam::getStatistic
 * \return
 */
const Statistic &TrafoParam::getStatistic() const{
    return this->statistic;
}

/*!
 * \brief TrafoParam::setStatistic
 * \param statistic
 */
void TrafoParam::setStatistic(const Statistic &statistic){
    this->statistic = statistic;
}

/*!
 * \brief TrafoParam::getIsDatumTrafo
 * \return
 */
const bool &TrafoParam::getIsDatumTrafo(){
    return this->isDatumTrafo;
}

/*!
 * \brief TrafoParam::setIsDatumTrafo
 * \param isDatumTrafo
 */
void TrafoParam::setIsDatumTrafo(const bool &isDatumTrafo){
    if(this->isDatumTrafo != isDatumTrafo){
        this->isDatumTrafo = isDatumTrafo;
    }
}

/*!
 * \brief TrafoParam::getHomogenMatrix
 * \return
 */
const OiMat &TrafoParam::getHomogenMatrix() const{
    return this->homogenMatrix;
}

/*!
 * \brief TrafoParam::getTranslation
 * \return
 */
const OiVec &TrafoParam::getTranslation() const{
    return this->translation;
}

/*!
 * \brief TrafoParam::getRotation
 * \return
 */
const OiVec &TrafoParam::getRotation() const{
    return this->rotation;
}

/*!
 * \brief TrafoParam::getScale
 * \return
 */
const OiVec &TrafoParam::getScale() const{
    return this->scale;
}

/*!
 * \brief TrafoParam::setTransformationParameters
 * Sets the transformation parameters. Input are 3 vectors of size 3.
 * \param rotation
 * \param translation
 * \param scale
 * \return
 */
bool TrafoParam::setTransformationParameters(const OiVec &rotation, const OiVec &translation, const OiVec &scale){
    return false;
}

/*!
 * \brief TrafoParam::setTransformationParameters
 * Sets the transformation parameters. Input are 3 homogeneous matrices of size 4x4.
 * \param rotation
 * \param translation
 * \param scale
 * \return
 */
bool TrafoParam::setTransformationParameters(const OiMat &rotation, const OiMat &translation, const OiMat &scale){

    if(rotation.getColCount() == 4 && rotation.getRowCount() == 4
            && translation.getColCount() == 4 && translation.getRowCount() == 4
            && scale.getColCount() == 4 && scale.getRowCount() == 4){

        OiVec t(3);
        OiVec r(3);
        OiVec s(3);
        for(int i = 0; i < 3; i++){
            t.setAt(i, translation.getAt(i, 3));
            s.setAt(i, scale.getAt(i, i));
        }

        r.setAt(0, qAtan2(-rotation.getAt(2,1), rotation.getAt(2,2))); //alpha
        r.setAt(1, qAsin(rotation.getAt(2,0))); //beta
        r.setAt(2, qAtan2(-rotation.getAt(1,0), rotation.getAt(0,0))); //gamma
        if( qFabs(qCos(r.getAt(1)) * qCos(r.getAt(2))) - qFabs(rotation.getAt(0,0)) > 0.01 ){
            r.setAt(1, 3.141592653589793 - r.getAt(1));
        }

        this->translation = t;
        this->scale = s;
        this->rotation = r;

        this->homogenMatrix = translation * rotation * scale;

        emit this->transformationParameterChanged(this->id);

    }else{
        return false;
    }

    return true;

}

/*!
 * \brief TrafoParam::setTransformationParameters
 * Sets the transformation parameters. Input is one matrix of size 4x4.
 * \param homogenMatrix
 * \return
 */
bool TrafoParam::setTransformationParameters(const OiMat &homogenMatrix){
    return false;
}

/*!
 * \brief TrafoParam::recalc
 */
void TrafoParam::recalc(){
    Feature::recalc();
}

/*!
 * \brief TrafoParam::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement TrafoParam::toOpenIndyXML(QDomDocument &xmlDoc){

    QDomElement trafoParam = Feature::toOpenIndyXML(xmlDoc);
/*
    if(trafoParam.isNull()){
        return trafoParam;
    }

    trafoParam.setTagName("transformationParameter");

    //add trafoParam attributes
    if(this->translation.getSize() >= 3 && this->rotation.getSize() >= 3 && this->getScale().getSize() >= 3){
        trafoParam.setAttribute("tx", this->translation.getAt(0));
        trafoParam.setAttribute("ty", this->translation.getAt(1));
        trafoParam.setAttribute("tz", this->translation.getAt(2));
        trafoParam.setAttribute("rx", this->rotation.getAt(0));
        trafoParam.setAttribute("ry", this->rotation.getAt(1));
        trafoParam.setAttribute("rz", this->rotation.getAt(2));
        trafoParam.setAttribute("mx", this->scale.getAt(0));
        trafoParam.setAttribute("my", this->scale.getAt(1));
        trafoParam.setAttribute("mz", this->scale.getAt(2));
    }
    trafoParam.setAttribute("movement", this->isMovement);
    trafoParam.setAttribute("datumtrafo", this->isDatumTrafo);
    trafoParam.setAttribute("use", this->use);
    trafoParam.setAttribute("time", this->validTime.toString(Qt::ISODate));

    //add from and to coordinate systems
    if(this->from != NULL && this->to != NULL){
        QDomElement from = xmlDoc.createElement("from");
        from.setAttribute("ref", this->from->getId());
        trafoParam.appendChild(from);
        QDomElement to = xmlDoc.createElement("to");
        to.setAttribute("ref", this->to->getId());
        trafoParam.appendChild(to);
    }
*/
    return trafoParam;

}

/*!
 * \brief TrafoParam::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool TrafoParam::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Feature::fromOpenIndyXML(xmlElem);
/*
    if(result){

        //set trafoParam attributes
        if(!xmlElem.hasAttribute("tx") || !xmlElem.hasAttribute("ty") || !xmlElem.hasAttribute("tz")
                || !xmlElem.hasAttribute("rx") || !xmlElem.hasAttribute("ry") || !xmlElem.hasAttribute("rz")
                || !xmlElem.hasAttribute("mx") || !xmlElem.hasAttribute("my") || !xmlElem.hasAttribute("mz")
                || !xmlElem.hasAttribute("use") || !xmlElem.hasAttribute("movement") || !xmlElem.hasAttribute("datumtrafo")
                || !xmlElem.hasAttribute("time")){
            return false;
        }
        this->translation.setAt(0, xmlElem.attribute("tx").toDouble());
        this->translation.setAt(1, xmlElem.attribute("ty").toDouble());
        this->translation.setAt(2, xmlElem.attribute("tz").toDouble());
        this->rotation.setAt(0, xmlElem.attribute("rx").toDouble());
        this->rotation.setAt(1, xmlElem.attribute("ry").toDouble());
        this->rotation.setAt(2, xmlElem.attribute("rz").toDouble());
        this->scale.setAt(0, xmlElem.attribute("mx").toDouble());
        this->scale.setAt(1, xmlElem.attribute("my").toDouble());
        this->scale.setAt(2, xmlElem.attribute("mz").toDouble());
        this->use = xmlElem.attribute("use").toInt();
        this->isMovement = xmlElem.attribute("movement").toInt();
        this->isDatumTrafo = xmlElem.attribute("datumtrafo").toInt();
        this->validTime = QDateTime::fromString(xmlElem.attribute("time"), Qt::ISODate);

        //calculate homogeneous matrix
        OiMat tmpTranslation(4,4);
        OiMat tmpRotation(4,4);
        OiMat tmpScale(4,4);

        tmpTranslation.setAt(0,0,1.0);
        tmpTranslation.setAt(0,3,this->translation.getAt(0));
        tmpTranslation.setAt(1,1,1.0);
        tmpTranslation.setAt(1,3,this->translation.getAt(1));
        tmpTranslation.setAt(2,2,1.0);
        tmpTranslation.setAt(2,3,this->translation.getAt(2));
        tmpTranslation.setAt(3,3,1.0);

        tmpScale.setAt(0,0,this->scale.getAt(0));
        tmpScale.setAt(1,1,this->scale.getAt(1));
        tmpScale.setAt(2,2,this->scale.getAt(2));
        tmpScale.setAt(3,3,1.0);

        tmpRotation.setAt(0,0,qCos(this->rotation.getAt(1))*qCos(this->rotation.getAt(2)));
        tmpRotation.setAt(0,1,qCos(this->rotation.getAt(0))*qSin(this->rotation.getAt(2))+qSin(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qCos(this->rotation.getAt(2)));
        tmpRotation.setAt(0,2,qSin(this->rotation.getAt(0))*qSin(this->rotation.getAt(2))-qCos(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qCos(this->rotation.getAt(2)));
        tmpRotation.setAt(1,0,-qCos(this->rotation.getAt(1))*qSin(this->rotation.getAt(2)));
        tmpRotation.setAt(1,1,qCos(this->rotation.getAt(0))*qCos(this->rotation.getAt(2))-qSin(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qSin(this->rotation.getAt(2)));
        tmpRotation.setAt(1,2,qSin(this->rotation.getAt(0))*qCos(this->rotation.getAt(2))+qCos(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qSin(this->rotation.getAt(2)));
        tmpRotation.setAt(2,0,qSin(this->rotation.getAt(1)));
        tmpRotation.setAt(2,1,-qSin(this->rotation.getAt(0))*qCos(this->rotation.getAt(1)));
        tmpRotation.setAt(2,2,qCos(this->rotation.getAt(0))*qCos(this->rotation.getAt(1)));
        tmpRotation.setAt(3,3,1.0);

        this->homogenMatrix = tmpTranslation*tmpScale*tmpRotation;

    }
*/
    return result;

}
