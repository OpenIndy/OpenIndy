#include "observation.h"

#include "station.h"
#include "coordinatesystem.h"
#include "function.h"

/*!
 * \brief Observation::Observation
 * \param parent
 */
Observation::Observation(QObject *parent) : Element(parent), xyz(4), originalXyz(4), isValid(false), isSolved(false){

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
    //this->isActiveCoordSys = copy.isActiveCoordSys;
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
    //this->isActiveCoordSys = copy.isActiveCoordSys;
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

    //check the corresponding reading
    if(this->getReading().isNull()){
        return;
    }

    QList<QPointer<Geometry> > targetGeometries;

    //remove observation and reading from its target geometries
    foreach(const QPointer<Geometry> &geom, this->targetGeometriesList){

        //check target geometry
        if(geom.isNull()){
            continue;
        }
        targetGeometries.append(geom);

        //remove observation and reading from functions of the targte geometry
        foreach(const QPointer<Function> &function, geom->getFunctions()){

            //check function and remove elements
            if(!function.isNull()){
                function->blockSignals(true);
                function->removeInputElement(this->getId());
                function->removeInputElement(this->getReading()->getId());
                function->blockSignals(false);
            }

        }

    }
    foreach(const QPointer<Geometry> &geom, targetGeometries){
        geom->blockSignals(true);
        this->removeTargetGeometry(geom->getId());
        geom->blockSignals(false);
    }

    //remove observation and reading from the station and station system
    if(!this->getStation().isNull() && !this->getStation()->getCoordinateSystem().isNull()){
        this->getStation()->blockSignals(true);
        this->getStation()->getCoordinateSystem()->blockSignals(true);
        this->getStation()->removeReading(this->getReading());
        this->getStation()->getCoordinateSystem()->removeObservation(this);
        this->getStation()->blockSignals(false);
        this->getStation()->getCoordinateSystem()->blockSignals(false);
    }

    //delete the corresponding reading
    delete this->getReading();

}

/*!
 * \brief Observation::getXYZ
 * \return
 */
const OiVec &Observation::getXYZ() const{
    return this->xyz;
}

/*!
 * \brief Observation::setXYZ
 * \param xyz
 */
void Observation::setXYZ(const OiVec &xyz){

    //check vector
    if(xyz.getSize() == this->xyz.getSize()){
        this->xyz = xyz;
    }

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
    if(this->station.isNull() && !station.isNull()){
        this->station = station;
    }
}

/*!
 * \brief Observation::getTargetGeometries
 * \return
 */
const QList<QPointer<Geometry> > &Observation::getTargetGeometries() const{
    return this->targetGeometriesList;
}

/*!
 * \brief Observation::addTargetGeometry
 * \param targetGeometry
 */
void Observation::addTargetGeometry(const QPointer<Geometry> &targetGeometry){

    if(!targetGeometry.isNull()){

        //check if target geometry is already included
        if(this->targetGeometriesList.contains(targetGeometry)){
            return;
        }

        this->targetGeometriesList.append(targetGeometry);
        this->targetGeometriesMap.insert(targetGeometry->getId(), targetGeometry);

        targetGeometry->addObservation(this);

    }

}

/*!
 * \brief Observation::removeTargetGeometry
 * \param targetGeometry
 */
void Observation::removeTargetGeometry(const QPointer<Geometry> &targetGeometry){

    if(!targetGeometry.isNull()){

        //check if target geometry is included
        if(!this->targetGeometriesList.contains(targetGeometry)){
            return;
        }

        this->targetGeometriesList.removeOne(targetGeometry);
        this->targetGeometriesMap.remove(targetGeometry->getId());

        targetGeometry->removeObservation(this);

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
/*void Observation::setReading(const QPointer<Reading> &reading){
    if(!reading.isNull()){
        this->reading = reading;
    }
}*/

/*!
 * \brief Observation::setIsValid
 * \param isValid
 */
/*void Observation::setIsValid(const bool &isValid){
    this->isValid = isValid;
}*/

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
 * \brief Observation::getDisplayId
 * \return
 */
QString Observation::getDisplayId() const{
    return QString::number(this->id);
}

/*!
 * \brief Observation::getDisplayStation
 * \return
 */
QString Observation::getDisplayStation() const{

    if(this->station.isNull()){
        return "";
    }
    return this->station->getFeatureName();

}

/*!
 * \brief Observation::getDisplayTargetGeometries
 * \return
 */
QString Observation::getDisplayTargetGeometries() const{

    QString result = "";
    foreach(const QPointer<Geometry> &geometry, this->targetGeometriesList){

        if(geometry.isNull()){
            continue;
        }

        if(result.compare("") == 0){
            result.append(geometry->getFeatureName());
        }else{
            result.append(QString(", %1").arg(geometry->getFeatureName()));
        }

    }
    return result;

}

/*!
 * \brief Observation::getDisplayX
 * \param type
 * \param digits
 * \return
 */
QString Observation::getDisplayX(const UnitType &type, const int &digits) const{
    if(this->isValid && this->isSolved){
        return QString::number(convertFromDefault(this->xyz.getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Observation::getDisplayY
 * \param type
 * \param digits
 * \return
 */
QString Observation::getDisplayY(const UnitType &type, const int &digits) const{
    if(this->isValid && this->isSolved){
        return QString::number(convertFromDefault(this->xyz.getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Observation::getDisplayZ
 * \param type
 * \param digits
 * \return
 */
QString Observation::getDisplayZ(const UnitType &type, const int &digits) const{
    if(this->isValid && this->isSolved){
        return QString::number(convertFromDefault(this->xyz.getAt(2), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Observation::getDisplaySigmaX
 * \param type
 * \param digits
 * \return
 */
QString Observation::getDisplaySigmaX(const UnitType &type, const int &digits) const{
    /*if(this->isValid && this->isSolved){
        return QString::number(convertFromDefault(this->xyz.getAt(0), type), 'f', digits);
    }*/
    return QString("");
}

/*!
 * \brief Observation::getDisplaySigmaY
 * \param type
 * \param digits
 * \return
 */
QString Observation::getDisplaySigmaY(const UnitType &type, const int &digits) const{
    /*if(this->isValid && this->isSolved){
        return QString::number(convertFromDefault(this->xyz.getAt(0), type), 'f', digits);
    }*/
    return QString("");
}

/*!
 * \brief Observation::getDisplaySigmaZ
 * \param type
 * \param digits
 * \return
 */
QString Observation::getDisplaySigmaZ(const UnitType &type, const int &digits) const{
    /*if(this->isValid && this->isSolved){
        return QString::number(convertFromDefault(this->xyz.getAt(0), type), 'f', digits);
    }*/
    return QString("");
}

/*!
 * \brief Observation::getDisplayIsValid
 * \return
 */
QString Observation::getDisplayIsValid() const{
    return this->isValid?"true":"false";
}

/*!
 * \brief Observation::getDisplayIsSolved
 * \return
 */
QString Observation::getDisplayIsSolved() const{
    return this->isSolved?"true":"false";
}

/*!
 * \brief Observation::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Observation::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement observation = Element::toOpenIndyXML(xmlDoc);

    if(observation.isNull()){
        return observation;
    }

    observation.setTagName("observation");

    //add observation attributes
    if(this->xyz.getSize() >= 3){
        observation.setAttribute("x", this->xyz.getAt(0));
        observation.setAttribute("y", this->xyz.getAt(1));
        observation.setAttribute("z", this->xyz.getAt(2));
    }else{
        observation.setAttribute("x", 0.0);
        observation.setAttribute("y", 0.0);
        observation.setAttribute("z", 0.0);
    }
    /*if(this->sigmaXyz.getSize() >= 3){
        observation.setAttribute("sigmaX", this->sigmaXyz.getAt(0));
        observation.setAttribute("sigmaY", this->sigmaXyz.getAt(1));
        observation.setAttribute("sigmaZ", this->sigmaXyz.getAt(2));
    }else{
        observation.setAttribute("sigmaX", 0.0);
        observation.setAttribute("sigmaY", 0.0);
        observation.setAttribute("sigmaZ", 0.0);
    }*/
    observation.setAttribute("sigmaX", 0.0);
    observation.setAttribute("sigmaY", 0.0);
    observation.setAttribute("sigmaZ", 0.0);

    observation.setAttribute("isValid", this->isValid);
    observation.setAttribute("isSolved", this->isSolved);

    //add station
    if(!this->station.isNull()){
        QDomElement station = xmlDoc.createElement("station");
        station.setAttribute("ref", this->station->getId());
        observation.appendChild(station);
    }

    //add reading
    if(!this->reading.isNull()){
        QDomElement reading = this->reading->toOpenIndyXML(xmlDoc);
        if(!reading.isNull()){
            observation.appendChild(reading);
        }
    }

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

        //set observation attributes
        if(!xmlElem.hasAttribute("isValid") || !xmlElem.hasAttribute("isSolved") || !xmlElem.hasAttribute("x") || !xmlElem.hasAttribute("y")
                || !xmlElem.hasAttribute("z") || !xmlElem.hasAttribute("sigmaX") || !xmlElem.hasAttribute("sigmaY")
                || !xmlElem.hasAttribute("sigmaZ")){
            return false;
        }
        this->isValid = xmlElem.attribute("isValid").toInt();
        this->isSolved = xmlElem.attribute("isSolved").toInt();
        this->xyz.setAt(0, xmlElem.attribute("x").toDouble());
        this->xyz.setAt(1, xmlElem.attribute("y").toDouble());
        this->xyz.setAt(2, xmlElem.attribute("z").toDouble());
        this->xyz.setAt(3, 1.0);
        /*this->sigmaXyz.setAt(0, xmlElem.attribute("sigmaX").toDouble());
        this->sigmaXyz.setAt(1, xmlElem.attribute("sigmaY").toDouble());
        this->sigmaXyz.setAt(2, xmlElem.attribute("sigmaZ").toDouble());*/

        //set reading
        QDomElement reading = xmlElem.firstChildElement("reading");
        if(reading.isNull()){
            return false;
        }
        this->reading = new Reading();
        result = this->reading->fromOpenIndyXML(reading);
        this->reading->setObservation(this);

    }

    return result;

}
