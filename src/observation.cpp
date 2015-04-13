#include "observation.h"

/*
#include "geometry.h"
#include "station.h"
#include "reading.h"
#include "function.h"

Observation::Observation() :
    myReading(NULL), myStation(NULL), myXyz(4), myOriginalXyz(4), sigmaXyz(4)
{
    //this->id = Configuration::generateID();

    //initialize matrices
    myStatistic.qxx = OiMat(4,4);
    myOriginalStatistic.qxx = OiMat(4,4);
}

Observation::~Observation(){

    //delete corresponding reading
    if(this->myReading != NULL){
        delete this->myReading;
    }

    //delete this observation from target geometries
    foreach(Geometry *myGeom, this->myTargetGeometries){
        if(myGeom != NULL){
            myGeom->removeObservation(this);
        }
    }

}

const OiVec &Observation::getXYZ() const{
    return this->myXyz;
}

const OiVec &Observation::getOriginalXYZ() const{
    return this->myOriginalXyz;
}

const Statistic &Observation::getStatistic() const{
    return this->myStatistic;
}

const Statistic &Observation::getOriginalStatistic() const{
    return this->myOriginalStatistic;
}

const OiVec &Observation::getSigmaXYZ() const{
    return this->sigmaXyz;
}

const QPointer<Station> &Observation::getStation() const{
    return this->myStation;
}

void Observation::setStation(const QPointer<Station> &station){
    if(!station.isNull()){
        this->myStation = station;
    }
}

const QList<QPointer<Geometry> > &Observation::getTargetGeometries() const{
    return this->myTargetGeometries;
}

void Observation::addTargetGeometry(const QPointer<Geometry> &targetGeometry){
    if(!targetGeometry.isNull()){
        this->myTargetGeometries.append(targetGeometry);
    }
}

void Observation::removeTargetGeometry(const QPointer<Geometry> &targetGeometry){

    if(targetGeometry.isNull()){
        return;
    }

    if(this->myTargetGeometries.contains(targetGeometry)){
        this->myTargetGeometries.removeOne(targetGeometry);
    }

}

void Observation::removeTargetGeometry(const int &geomId){

    foreach(const QPointer<Geometry> &geom, this->myTargetGeometries){
        if(!geom.isNull() && geom->getId() == geomId){
            this->myTargetGeometries.removeOne(geom);
        }
    }

}

const QPointer<Reading> &Observation::getReading() const{
    return this->myReading;
}

void Observation::setReading(const QPointer<Reading> &reading){
    if(!reading.isNull()){
        this->myReading = reading;
    }
}

QDomElement Observation::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement observation = Element::toOpenIndyXML(xmlDoc);

    if(observation.isNull()){
        return observation;
    }

    observation.setTagName("observation");

    //add observation attributes
    if(this->myXyz.getSize() >= 3){
        observation.setAttribute("x", this->myXyz.getAt(0));
        observation.setAttribute("y", this->myXyz.getAt(1));
        observation.setAttribute("z", this->myXyz.getAt(2));
    }else{
        observation.setAttribute("x", this->myXyz.getAt(0));
        observation.setAttribute("y", this->myXyz.getAt(1));
        observation.setAttribute("z", this->myXyz.getAt(2));
    }
    if(this->sigmaXyz.getSize() >= 3){
        observation.setAttribute("sigmaX", this->sigmaXyz.getAt(0));
        observation.setAttribute("sigmaY", this->sigmaXyz.getAt(1));
        observation.setAttribute("sigmaZ", this->sigmaXyz.getAt(2));
    }else{
        observation.setAttribute("sigmaX", this->sigmaXyz.getAt(0));
        observation.setAttribute("sigmaY", this->sigmaXyz.getAt(1));
        observation.setAttribute("sigmaZ", this->sigmaXyz.getAt(2));
    }
    observation.setAttribute("isValid", this->isValid);
    observation.setAttribute("isSolved", this->isSolved);
    observation.setAttribute("isUsed", this->isUsed);

    //add station
    if(this->myStation != NULL){
        QDomElement station = xmlDoc.createElement("station");
        station.setAttribute("ref", this->myStation->getId());
        observation.appendChild(station);
    }

    //add reading
    if(this->myReading != NULL){
        QDomElement reading = this->myReading->toOpenIndyXML(xmlDoc);
        if(!reading.isNull()){
            observation.appendChild(reading);
        }
    }

    return observation;

}

bool Observation::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Element::fromOpenIndyXML(xmlElem);

    if(result){

        //set observation attributes
        if(!xmlElem.hasAttribute("isValid") || !xmlElem.hasAttribute("x") || !xmlElem.hasAttribute("y")
                || !xmlElem.hasAttribute("z") || !xmlElem.hasAttribute("sigmaX") || !xmlElem.hasAttribute("sigmaY")
                || !xmlElem.hasAttribute("sigmaZ")){
            return false;
        }
        this->isValid = xmlElem.attribute("isValid").toInt();
        this->isSolved = xmlElem.attribute("isSolved").toInt();
        this->isUsed = xmlElem.attribute("isUsed").toInt();
        this->myXyz.setAt(0, xmlElem.attribute("x").toDouble());
        this->myXyz.setAt(1, xmlElem.attribute("y").toDouble());
        this->myXyz.setAt(2, xmlElem.attribute("z").toDouble());
        this->myXyz.setAt(3, 1.0);
        this->sigmaXyz.setAt(0, xmlElem.attribute("sigmaX").toDouble());
        this->sigmaXyz.setAt(1, xmlElem.attribute("sigmaY").toDouble());
        this->sigmaXyz.setAt(2, xmlElem.attribute("sigmaZ").toDouble());

        //set reading
        QDomElement reading = xmlElem.firstChildElement("reading");
        if(reading.isNull()){
            return false;
        }
        QPointer<Reading> myReading = new Reading();
        this->myReading = myReading;
        myReading->setObservation(this);
        result = myReading->fromOpenIndyXML(reading);

        //get original xyz from reading
        this->myReading->toCartesian();
        this->myOriginalXyz.setAt(0, this->myReading->getCartesianReading().xyz.getAt(0));
        this->myOriginalXyz.setAt(1, this->myReading->getCartesianReading().xyz.getAt(1));
        this->myOriginalXyz.setAt(2, this->myReading->getCartesianReading().xyz.getAt(2));
        this->myOriginalXyz.setAt(3, 1.0);

    }

    return result;

}

void Observation::calcFromReading(){

    if(this->myReading.isNull()){
        return;
    }

    switch (this->myReading->getTypeOfReading()) {
    case ePolarReading:
        //calc obs xyz
        this->myOriginalXyz = Reading::toCartesian(this->myReading->getPolarReading().azimuth,this->myReading->getPolarReading().zenith,this->myReading->getPolarReading().distance);
        this->myXyz = Reading::toCartesian(this->myReading->getPolarReading().azimuth,this->myReading->getPolarReading().zenith,this->myReading->getPolarReading().distance);

        //calc sigma xyz
        //this->sigmaXyz = r->errorPropagationPolarToCartesian();

        //created from reading polar
        this->setIsValid(true);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    case eCartesianReading:
        //calc xyz
        this->myOriginalXyz.setAt(0,this->myReading->getCartesianReading().xyz.getAt(0));
        this->myOriginalXyz.setAt(1,this->myReading->getCartesianReading().xyz.getAt(1));
        this->myOriginalXyz.setAt(2,this->myReading->getCartesianReading().xyz.getAt(2));
        this->myOriginalXyz.setAt(3,1.0);
        this->myXyz.setAt(0,this->myReading->getCartesianReading().xyz.getAt(0));
        this->myXyz.setAt(1,this->myReading->getCartesianReading().xyz.getAt(1));
        this->myXyz.setAt(2,this->myReading->getCartesianReading().xyz.getAt(2));
        this->myXyz.setAt(3,1.0);

        //created from reading cartesian
        this->setIsValid(true);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    case eDirectionReading:
        //created from reading direction
        this->setIsValid(false);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    case eDistanceReading:
        //created from reading distance
        this->setIsValid(false);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    case eTemperatureReading:
        //created from reading temperature
        this->setIsValid(false);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    case eLevelReading:
        //created from reading level
        this->setIsValid(false);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    case eUndefinedReading:
        //created from reading undefined
        this->setIsValid(false);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    default:
        this->setIsValid(false);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    }
}

void Observation::setIsValid(const bool &isValid)
{
    this->isValid = isValid;
}

const bool &Observation::getIsValid() const
{
    return this->isValid;
}

void Observation::setIsSolved(const bool &isSolved)
{
    this->isSolved = isSolved;
}

bool Observation::getIsSolved() const
{
    return this->isSolved;
}

void Observation::setIsUsed(const bool &use)
{
    this->isUsed = use;
}

bool Observation::getIsUsed() const
{
    return this->isUsed;
}

bool Observation::getUseState()
{
    if(this->isSolved && this->isValid && this->isUsed){
        return true;
    }
    return false;
}
*/

/*!
 * \brief Observation::Observation
 * \param parent
 */
Observation::Observation(QObject *parent) : Element(parent), xyz(4), originalXyz(4){

}

/*!
 * \brief Observation::Observation
 * \param copy
 * \param parent
 */
Observation::Observation(const Observation &copy, QObject *parent) : Element(copy, parent){

    //copy attributes
    this->isValid = copy.isValid;
    this->isSolved = copy.isSolved;
    this->isActiveCoordSys = copy.isActiveCoordSys;
    this->xyz = copy.xyz;
    this->originalXyz = copy.originalXyz;
    this->statistic = copy.statistic;
    this->originalStatistic = copy.originalStatistic;

    //copy reading and station
    this->reading = copy.reading;
    this->station = copy.station;

}

/*!
 * \brief Observation::operator =
 * \param copy
 * \return
 */
Observation &Observation::operator=(const Observation &copy){

    //copy attributes
    this->isValid = copy.isValid;
    this->isSolved = copy.isSolved;
    this->isActiveCoordSys = copy.isActiveCoordSys;
    this->xyz = copy.xyz;
    this->originalXyz = copy.originalXyz;
    this->statistic = copy.statistic;
    this->originalStatistic = copy.originalStatistic;

    //copy reading and station
    this->reading = copy.reading;
    this->station = copy.station;

    return *this;

}

/*!
 * \brief Observation::~Observation
 */
Observation::~Observation(){

}

/*!
 * \brief Observation::getXYZ
 * \return
 */
const OiVec &Observation::getXYZ() const{
    return this->xyz;
}

/*!
 * \brief Observation::getOriginalXYZ
 * \return
 */
const OiVec &Observation::getOriginalXYZ() const{
    return this->originalXyz;
}

/*!
 * \brief Observation::getStatistic
 * \return
 */
const Statistic &Observation::getStatistic() const{
    return this->statistic;
}

/*!
 * \brief Observation::getOriginalStatistic
 * \return
 */
const Statistic &Observation::getOriginalStatistic() const{
    return this->originalStatistic;
}

/*!
 * \brief Observation::getStation
 * \return
 */
const QPointer<Station> &Observation::getStation() const{
    return this->station;
}

/*!
 * \brief Observation::setStation
 * \param station
 */
void Observation::setStation(const QPointer<Station> &station){
    if(!station.isNull()){
        this->station = station;
    }
}

/*!
 * \brief Observation::getTargetGeometries
 * \return
 */
const QList<QPointer<Geometry> > &Observation::getTargetGeometries() const{
    return this->targetGeometries;
}

/*!
 * \brief Observation::addTargetGeometry
 * \param targetGeometry
 */
void Observation::addTargetGeometry(const QPointer<Geometry> &targetGeometry){
    if(!targetGeometry.isNull()){
        this->targetGeometriesList.append(targetGeometry);
        this->targetGeometriesMap.insert(targetGeometry->getId(), targetGeometry);
    }
}

/*!
 * \brief Observation::removeTargetGeometry
 * \param targetGeometry
 */
void Observation::removeTargetGeometry(const QPointer<Geometry> &targetGeometry){
    if(!targetGeometry.isNull()){
        this->targetGeometriesList.removeOne(targetGeometry);
        this->targetGeometriesMap.remove(targetGeometry->getId());
    }
}

/*!
 * \brief Observation::removeTargetGeometry
 * \param geomId
 */
void Observation::removeTargetGeometry(const int &geomId){
    QPointer<Geometry> targetGeometry = this->targetGeometriesMap.value(geomId, QPointer<Geometry>());
    this->removeTargetGeometry(targetGeometry);
}

/*!
 * \brief Observation::getReading
 * \return
 */
const QPointer<Reading> &Observation::getReading() const{
    return this->reading;
}

/*!
 * \brief Observation::setReading
 * \param reading
 */
void Observation::setReading(const QPointer<Reading> &reading){
    if(!reading.isNull()){
        this->reading = reading;
    }
}

/*!
 * \brief Observation::setIsValid
 * \param isValid
 */
void Observation::setIsValid(const bool &isValid){
    this->isValid = isValid;
}

/*!
 * \brief Observation::getIsValid
 * \return
 */
const bool &Observation::getIsValid() const{
    return this->isValid;
}

/*!
 * \brief Observation::setIsSolved
 * \param isSolved
 */
void Observation::setIsSolved(const bool &isSolved){
    this->isSolved = isSolved;
}

/*!
 * \brief Observation::getIsSolved
 * \return
 */
const bool &Observation::getIsSolved() const{
    return this->isSolved;
}

/*!
 * \brief Observation::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Observation::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement observation = Element::toOpenIndyXML(xmlDoc);
/*
    if(observation.isNull()){
        return observation;
    }

    observation.setTagName("observation");

    //add observation attributes
    if(this->myXyz.getSize() >= 3){
        observation.setAttribute("x", this->myXyz.getAt(0));
        observation.setAttribute("y", this->myXyz.getAt(1));
        observation.setAttribute("z", this->myXyz.getAt(2));
    }else{
        observation.setAttribute("x", this->myXyz.getAt(0));
        observation.setAttribute("y", this->myXyz.getAt(1));
        observation.setAttribute("z", this->myXyz.getAt(2));
    }
    if(this->sigmaXyz.getSize() >= 3){
        observation.setAttribute("sigmaX", this->sigmaXyz.getAt(0));
        observation.setAttribute("sigmaY", this->sigmaXyz.getAt(1));
        observation.setAttribute("sigmaZ", this->sigmaXyz.getAt(2));
    }else{
        observation.setAttribute("sigmaX", this->sigmaXyz.getAt(0));
        observation.setAttribute("sigmaY", this->sigmaXyz.getAt(1));
        observation.setAttribute("sigmaZ", this->sigmaXyz.getAt(2));
    }
    observation.setAttribute("isValid", this->isValid);
    observation.setAttribute("isSolved", this->isSolved);
    observation.setAttribute("isUsed", this->isUsed);

    //add station
    if(this->myStation != NULL){
        QDomElement station = xmlDoc.createElement("station");
        station.setAttribute("ref", this->myStation->getId());
        observation.appendChild(station);
    }

    //add reading
    if(this->myReading != NULL){
        QDomElement reading = this->myReading->toOpenIndyXML(xmlDoc);
        if(!reading.isNull()){
            observation.appendChild(reading);
        }
    }
*/
    return observation;

}

/*!
 * \brief Observation::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Observation::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Element::fromOpenIndyXML(xmlElem);

    if(result){
/*
        //set observation attributes
        if(!xmlElem.hasAttribute("isValid") || !xmlElem.hasAttribute("x") || !xmlElem.hasAttribute("y")
                || !xmlElem.hasAttribute("z") || !xmlElem.hasAttribute("sigmaX") || !xmlElem.hasAttribute("sigmaY")
                || !xmlElem.hasAttribute("sigmaZ")){
            return false;
        }
        this->isValid = xmlElem.attribute("isValid").toInt();
        this->isSolved = xmlElem.attribute("isSolved").toInt();
        this->isUsed = xmlElem.attribute("isUsed").toInt();
        this->myXyz.setAt(0, xmlElem.attribute("x").toDouble());
        this->myXyz.setAt(1, xmlElem.attribute("y").toDouble());
        this->myXyz.setAt(2, xmlElem.attribute("z").toDouble());
        this->myXyz.setAt(3, 1.0);
        this->sigmaXyz.setAt(0, xmlElem.attribute("sigmaX").toDouble());
        this->sigmaXyz.setAt(1, xmlElem.attribute("sigmaY").toDouble());
        this->sigmaXyz.setAt(2, xmlElem.attribute("sigmaZ").toDouble());

        //set reading
        QDomElement reading = xmlElem.firstChildElement("reading");
        if(reading.isNull()){
            return false;
        }
        QPointer<Reading> myReading = new Reading();
        this->myReading = myReading;
        myReading->setObservation(this);
        result = myReading->fromOpenIndyXML(reading);

        //get original xyz from reading
        this->myReading->toCartesian();
        this->myOriginalXyz.setAt(0, this->myReading->getCartesianReading().xyz.getAt(0));
        this->myOriginalXyz.setAt(1, this->myReading->getCartesianReading().xyz.getAt(1));
        this->myOriginalXyz.setAt(2, this->myReading->getCartesianReading().xyz.getAt(2));
        this->myOriginalXyz.setAt(3, 1.0);
*/
    }

    return result;

}
