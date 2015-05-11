#include "measurementconfig.h"
/*
MeasurementConfig::MeasurementConfig(){

    //set defaults
    this->count = 1000;
    this->iterations = 1;
    this->measureTwoSides = false;
    this->timeDependent = false;
    this->distanceDependent = false;
    this->timeInterval = 0;
    this->distanceInterval = 0.0;
    //this->typeOfReading = static_cast<Configuration::ReadingTypes>(-1);
    this->typeOfReading = ePolarReading;
    this->isSaved = false;

}

QString MeasurementConfig::getName() const{
    return this->name;
}

QString MeasurementConfig::getDisplayName() const{
    if(this->getIsValid()){
        return QString("%1%2").arg(this->name).arg(this->isSaved?"":" (project)");
    }
    return "";
}

bool MeasurementConfig::getIsSaved() const{
    return this->isSaved;
}

bool MeasurementConfig::getIsValid() const{
    if(this->name.compare("") == 0){
        return false;
    }
    return true;
}

bool MeasurementConfig::setName(QString name){
    if(this->name.compare("") == 0){
        this->name = name;
        return true;
    }
    return false;
}

QDomElement MeasurementConfig::toOpenIndyXML(QDomDocument &xmlDoc) const{

    if(xmlDoc.isNull()){
        return QDomElement();
    }

    QDomElement mConfig = xmlDoc.createElement("measurementConfig");

    //set measurement config attributes
    mConfig.setAttribute("name", this->name);
    mConfig.setAttribute("count", this->count);
    mConfig.setAttribute("iterations", this->iterations);
    mConfig.setAttribute("measureTwoSides", this->measureTwoSides);
    mConfig.setAttribute("timeDependent", this->timeDependent);
    mConfig.setAttribute("distanceDependent", this->distanceDependent);
    mConfig.setAttribute("timeInterval", QString::number(this->timeInterval));
    mConfig.setAttribute("distanceInterval", this->distanceInterval);
    mConfig.setAttribute("typeOfReading", this->typeOfReading);

    return mConfig;

}

bool MeasurementConfig::fromOpenIndyXML(QDomElement &xmlElem){

    if(xmlElem.isNull()){
        return false;
    }

    if(!xmlElem.hasAttribute("name") || !xmlElem.hasAttribute("count") || !xmlElem.hasAttribute("iterations")
            || !xmlElem.hasAttribute("measureTwoSides") || !xmlElem.hasAttribute("timeDependent")
            || !xmlElem.hasAttribute("distanceDependent") || !xmlElem.hasAttribute("timeInterval")
            || !xmlElem.hasAttribute("distanceInterval") || !xmlElem.hasAttribute("typeOfReading")){
        return false;
    }

    this->name = xmlElem.attribute("name");
    this->count = xmlElem.attribute("count").toInt();
    this->iterations = xmlElem.attribute("iterations").toInt();
    this->measureTwoSides = xmlElem.attribute("measureTwoSides").toInt();
    this->timeDependent = xmlElem.attribute("timeDependent").toInt();
    this->distanceDependent = xmlElem.attribute("distanceDependent").toInt();
    this->timeInterval = xmlElem.attribute("timeInterval").toLong();
    this->distanceInterval = xmlElem.attribute("distanceInterval").toDouble();
    this->typeOfReading = (ReadingTypes)xmlElem.attribute("typeOfReading").toInt();

    return true;

}

void MeasurementConfig::setIsSaved(bool isSaved){
    this->isSaved = isSaved;
}
*/

/*!
 * \brief MeasurementConfig::MeasurementConfig
 */
MeasurementConfig::MeasurementConfig() : isSaved(false){

    //set defaults
    this->count = 500;
    this->iterations = 1;
    this->measureTwoSides = false;
    this->timeDependent = false;
    this->distanceDependent = false;
    this->timeInterval = 0;
    this->distanceInterval = 0.0;
    this->typeOfReading = ePolarReading;

}

/*!
 * \brief MeasurementConfig::MeasurementConfig
 * \param copy
 */
MeasurementConfig::MeasurementConfig(const MeasurementConfig &copy){

    //copy measurement config attributes
    this->name = copy.name;
    this->isSaved = copy.isSaved;
    this->count = copy.count;
    this->iterations = copy.iterations;
    this->measureTwoSides = copy.measureTwoSides;
    this->timeDependent = copy.timeDependent;
    this->distanceDependent = copy.distanceDependent;
    this->timeInterval = copy.timeInterval;
    this->distanceInterval = copy.distanceInterval;
    this->typeOfReading = copy.typeOfReading;

}

/*!
 * \brief MeasurementConfig::operator =
 * \param copy
 * \return
 */
MeasurementConfig &MeasurementConfig::operator=(const MeasurementConfig &copy){

    //copy measurement config attributes
    this->name = copy.name;
    this->isSaved = copy.isSaved;
    this->count = copy.count;
    this->iterations = copy.iterations;
    this->measureTwoSides = copy.measureTwoSides;
    this->timeDependent = copy.timeDependent;
    this->distanceDependent = copy.distanceDependent;
    this->timeInterval = copy.timeInterval;
    this->distanceInterval = copy.distanceInterval;
    this->typeOfReading = copy.typeOfReading;

    return *this;

}

/*!
 * \brief MeasurementConfig::getName
 * \return
 */
const QString &MeasurementConfig::getName() const{
    return this->name;
}

/*!
 * \brief MeasurementConfig::setName
 * \param name
 */
void MeasurementConfig::setName(const QString &name){
    this->name = name;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getIsSaved
 * \return
 */
const bool &MeasurementConfig::getIsSaved() const{
    return this->isSaved;
}

/*!
 * \brief MeasurementConfig::setIsSaved
 * \param isSaved
 */
void MeasurementConfig::setIsSaved(const bool &isSaved){
    this->isSaved = isSaved;
}

/*!
 * \brief MeasurementConfig::getIsValid
 * \return
 */
bool MeasurementConfig::getIsValid() const{
    if(this->name.compare("") != 0){
        return true;
    }
    return false;
}

/*!
 * \brief MeasurementConfig::getCount
 * \return
 */
const int &MeasurementConfig::getCount() const{
    return this->count;
}

/*!
 * \brief MeasurementConfig::setCount
 * \param count
 */
void MeasurementConfig::setCount(const int &count){
    this->count = count;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getIterations
 * \return
 */
const int &MeasurementConfig::getIterations() const{
    return this->iterations;
}

/*!
 * \brief MeasurementConfig::setIterations
 * \param iterations
 */
void MeasurementConfig::setIterations(const int &iterations){
    this->iterations = iterations;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getMeasureTwoSides
 * \return
 */
const bool &MeasurementConfig::getMeasureTwoSides() const{
    return this->measureTwoSides;
}

/*!
 * \brief MeasurementConfig::setMeasureTwoSides
 * \param twoSides
 */
void MeasurementConfig::setMeasureTwoSides(const bool &twoSides){
    this->measureTwoSides = measureTwoSides;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getTimeDependent
 * \return
 */
const bool &MeasurementConfig::getTimeDependent() const{
    return this->timeDependent;
}

/*!
 * \brief MeasurementConfig::setTimeDependent
 * \param timeDependent
 */
void MeasurementConfig::setTimeDependent(const bool &timeDependent){
    this->timeDependent = timeDependent;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getDistanceDependent
 * \return
 */
const bool &MeasurementConfig::getDistanceDependent() const{
    return this->distanceDependent;
}

/*!
 * \brief MeasurementConfig::setDistanceDependent
 * \param distanceDependent
 */
void MeasurementConfig::setDistanceDependent(const bool &distanceDependent){
    this->distanceDependent = distanceDependent;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getTimeInterval
 * \return
 */
const long &MeasurementConfig::getTimeInterval() const{
    return this->timeInterval;
}

/*!
 * \brief MeasurementConfig::setTimeInterval
 * \param interval
 */
void MeasurementConfig::setTimeInterval(const long &interval){
    this->timeInterval = timeInterval;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getDistanceInterval
 * \return
 */
const double &MeasurementConfig::getDistanceInterval() const{
    return this->distanceInterval;
}

/*!
 * \brief MeasurementConfig::setDistanceInterval
 * \param interval
 */
void MeasurementConfig::setDistanceInterval(const double &interval){
    this->distanceInterval = interval;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getTypeOfReading
 * \return
 */
const ReadingTypes &MeasurementConfig::getTypeOfReading() const{
    return this->typeOfReading;
}

/*!
 * \brief MeasurementConfig::setTypeOfReading
 * \param type
 */
void MeasurementConfig::setTypeOfReading(const ReadingTypes &type){
    this->typeOfReading = type;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement MeasurementConfig::toOpenIndyXML(QDomDocument &xmlDoc) const{

    if(xmlDoc.isNull()){
        return QDomElement();
    }

    QDomElement mConfig = xmlDoc.createElement("measurementConfig");

    //set measurement config attributes
    mConfig.setAttribute("name", this->name);
    mConfig.setAttribute("count", this->count);
    mConfig.setAttribute("iterations", this->iterations);
    mConfig.setAttribute("measureTwoSides", this->measureTwoSides);
    mConfig.setAttribute("timeDependent", this->timeDependent);
    mConfig.setAttribute("distanceDependent", this->distanceDependent);
    mConfig.setAttribute("timeInterval", QString::number(this->timeInterval));
    mConfig.setAttribute("distanceInterval", this->distanceInterval);
    mConfig.setAttribute("typeOfReading", this->typeOfReading);

    return mConfig;

}

/*!
 * \brief MeasurementConfig::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool MeasurementConfig::fromOpenIndyXML(QDomElement &xmlElem){

    if(xmlElem.isNull()){
        return false;
    }

    if(!xmlElem.hasAttribute("name") || !xmlElem.hasAttribute("count") || !xmlElem.hasAttribute("iterations")
            || !xmlElem.hasAttribute("measureTwoSides") || !xmlElem.hasAttribute("timeDependent")
            || !xmlElem.hasAttribute("distanceDependent") || !xmlElem.hasAttribute("timeInterval")
            || !xmlElem.hasAttribute("distanceInterval") || !xmlElem.hasAttribute("typeOfReading")){
        return false;
    }

    this->name = xmlElem.attribute("name");
    this->count = xmlElem.attribute("count").toInt();
    this->iterations = xmlElem.attribute("iterations").toInt();
    this->measureTwoSides = xmlElem.attribute("measureTwoSides").toInt();
    this->timeDependent = xmlElem.attribute("timeDependent").toInt();
    this->distanceDependent = xmlElem.attribute("distanceDependent").toInt();
    this->timeInterval = xmlElem.attribute("timeInterval").toLong();
    this->distanceInterval = xmlElem.attribute("distanceInterval").toDouble();
    this->typeOfReading = (ReadingTypes)xmlElem.attribute("typeOfReading").toInt();

    return true;

}
