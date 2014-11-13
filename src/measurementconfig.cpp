#include "measurementconfig.h"

MeasurementConfig::MeasurementConfig(){

    //set defaults
    this->count = 1000;
    this->iterations = 1;
    this->measureTwoSides = false;
    this->timeDependent = false;
    this->distanceDependent = false;
    this->timeInterval = 0;
    this->distanceInterval = 0.0;
    this->typeOfReading = static_cast<Configuration::ReadingTypes>(-1);
    this->isSaved = false;

}

/*!
 * \brief MeasurementConfig::getName
 * \return
 */
QString MeasurementConfig::getName() const{
    return this->name;
}

/*!
 * \brief MeasurementConfig::getDisplayName
 * \return
 */
QString MeasurementConfig::getDisplayName() const{
    return QString("%1%2").arg(this->name).arg(this->isSaved?"":" (project)");
}

/*!
 * \brief MeasurementConfig::getIsSaved
 * \return
 */
bool MeasurementConfig::getIsSaved() const{
    return this->isSaved;
}

/*!
 * \brief MeasurementConfig::getIsValid
 * \return
 */
bool MeasurementConfig::getIsValid() const{
    if(this->name.compare("") == 0){
        return false;
    }
    return true;
}

/*!
 * \brief MeasurementConfig::setName
 * \param name
 * \return
 */
bool MeasurementConfig::setName(QString name){
    if(this->name.compare("") == 0){
        this->name = name;
        return true;
    }
    return false;
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
    mConfig.setAttribute("timeInterval", this->timeInterval);
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
    this->typeOfReading = (Configuration::ReadingTypes)xmlElem.attribute("typeOfReading").toInt();

    return true;

}

/*!
 * \brief MeasurementConfig::setIsSaved
 * \param isSaved
 */
void MeasurementConfig::setIsSaved(bool isSaved){
    this->isSaved = isSaved;
}
