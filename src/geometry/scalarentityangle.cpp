#include "scalarentityangle.h"
/*
#include "function.h"

MeasurementConfig ScalarEntityAngle::defaultMeasurementConfig;

ScalarEntityAngle::ScalarEntityAngle(bool isNominal, QObject *parent) : Geometry(isNominal, parent)
{
    //this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = false;
}

ScalarEntityAngle::ScalarEntityAngle(const ScalarEntityAngle &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->angle = copy.getAngle();
    this->isSolved = copy.isSolved;
}

void ScalarEntityAngle::recalc(){

    if(this->functionList.size() > 0){

        bool solved = true;
        foreach(Function *f, this->functionList){

            //execute the function if it exists and if the last function was executed successfully
            if(f != NULL && solved == true){
                solved = f->exec(*this);
            }

        }
        this->setIsSolved(solved);

    }else if(this->isNominal == false){

        this->angle = 0.0;
        this->setIsSolved(false);

    }

}

QDomElement ScalarEntityAngle::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement entityAngle = Geometry::toOpenIndyXML(xmlDoc);

    if(entityAngle.isNull()){
        return entityAngle;
    }

    entityAngle.setAttribute("type", getGeometryTypeName(eScalarEntityAngleGeometry));

    //add angle
    QDomElement angle = xmlDoc.createElement("angle");
    if(this->getIsSolved() || this->getIsNominal()){
        angle.setAttribute("value", this->angle);
    }else{
        angle.setAttribute("value", 0.0);
    }
    entityAngle.appendChild(angle);

    return entityAngle;

}

bool ScalarEntityAngle::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set circle attributes
        QDomElement angle = xmlElem.firstChildElement("angle");

        if(angle.isNull() || !angle.hasAttribute("value")){
            return false;
        }

        this->angle = angle.attribute("value").toDouble();

    }

    return result;

}

bool ScalarEntityAngle::saveSimulationData()
{
    this->mySimulationData.addScalar(this->angle);
    return true;
}

double ScalarEntityAngle::getAngle() const{
    return this->angle;
}

void ScalarEntityAngle::setAngle(double angle){
    this->angle = angle;
}

void ScalarEntityAngle::setAngle(OiVec direction1, OiVec direction2){
    if(direction1.getSize() >= 3 && direction2.getSize() >= 3){
        //copy OiVec's to new OiVec's of size 3
        OiVec a(3);
        OiVec b(3);
        for(int i = 0; i < 3; i++){
            a.setAt(i, direction1.getAt(i));
            b.setAt(i, direction2.getAt(i));
        }
        //normalize vectors
        a.normalize();
        b.normalize();
        //calculate angle between directions
        double ab;
        OiVec::dot(ab, a, b);
        this->angle = qAcos(ab);
    }
}

double ScalarEntityAngle::getScalar() const
{
    return this->angle;
}

QString ScalarEntityAngle::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString ScalarEntityAngle::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}
QString ScalarEntityAngle::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}

QString ScalarEntityAngle::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString ScalarEntityAngle::getDisplayMConfig() const{
    return this->activeMeasurementConfig.getDisplayName();
}

QString ScalarEntityAngle::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        //return QString::number(this->myStatistic.stdev*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
    }
    return "-/-";

}

QString ScalarEntityAngle::getDisplayScalarAngleValue() const{
    return "";
    //return QString::number(this->angle*OiUnitConverter::getAngleMultiplier(),'f',OiUnitConverter::angleDigits);
}
*/


/*!
 * \brief ScalarEntityAngle::ScalarEntityAngle
 * \param isNominal
 * \param parent
 */
ScalarEntityAngle::ScalarEntityAngle(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

}

/*!
 * \brief ScalarEntityAngle::ScalarEntityAngle
 * \param isNominal
 * \param angle
 * \param parent
 */
ScalarEntityAngle::ScalarEntityAngle(const bool &isNominal, const double &angle, QObject *parent) : Geometry(isNominal, parent){
    this->setAngle(angle);
}

/*!
 * \brief ScalarEntityAngle::ScalarEntityAngle
 * \param copy
 * \param parent
 */
ScalarEntityAngle::ScalarEntityAngle(const ScalarEntityAngle &copy, QObject *parent) : Geometry(copy, parent){
    this->angle = copy.angle;
}

/*!
 * \brief ScalarEntityAngle::operator =
 * \param copy
 * \return
 */
ScalarEntityAngle &ScalarEntityAngle::operator=(const ScalarEntityAngle &copy){
    this->angle = copy.angle;
    return *this;
}

/*!
 * \brief ScalarEntityAngle::~ScalarEntityAngle
 */
ScalarEntityAngle::~ScalarEntityAngle(){

}

/*!
 * \brief ScalarEntityAngle::getAngle
 * \return
 */
const double &ScalarEntityAngle::getAngle() const{
    return this->angle;
}

/*!
 * \brief ScalarEntityAngle::setAngle
 * \param angle
 */
void ScalarEntityAngle::setAngle(const double &angle){
    this->angle = angle;
}

/*!
 * \brief ScalarEntityAngle::recalc
 */
void ScalarEntityAngle::recalc(){

    Geometry::recalc();

    //reset angle definition if not solved
    if(!this->isSolved){
        this->angle = 0.0;
    }

}

/*!
 * \brief ScalarEntityAngle::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement ScalarEntityAngle::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement entityAngle = Geometry::toOpenIndyXML(xmlDoc);

    if(entityAngle.isNull()){
        return entityAngle;
    }

    entityAngle.setAttribute("type", getGeometryTypeName(eScalarEntityAngleGeometry));

    //add angle
    QDomElement angle = xmlDoc.createElement("angle");
    if(this->getIsSolved() || this->getIsNominal()){
        angle.setAttribute("value", this->angle);
    }else{
        angle.setAttribute("value", 0.0);
    }
    entityAngle.appendChild(angle);

    return entityAngle;

}

/*!
 * \brief ScalarEntityAngle::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool ScalarEntityAngle::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set circle attributes
        QDomElement angle = xmlElem.firstChildElement("angle");

        if(angle.isNull() || !angle.hasAttribute("value")){
            return false;
        }

        this->angle = angle.attribute("value").toDouble();

    }

    return result;

}
