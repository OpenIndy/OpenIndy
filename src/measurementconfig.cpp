#include "measurementconfig.h"

MeasurementConfig::MeasurementConfig(){
    this->typeOfReading = static_cast<Configuration::ReadingTypes>(-1);
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
    mConfig.setAttribute("face", this->face);
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
            || !xmlElem.hasAttribute("face") || !xmlElem.hasAttribute("measureTwoSides") || !xmlElem.hasAttribute("timeDependent")
            || !xmlElem.hasAttribute("distanceDependent") || !xmlElem.hasAttribute("timeInterval")
            || !xmlElem.hasAttribute("distanceInterval") || !xmlElem.hasAttribute("typeOfReading")){
        return false;
    }

    this->name = xmlElem.attribute("name");
    this->count = xmlElem.attribute("count").toInt();
    this->iterations = xmlElem.attribute("iterations").toInt();
    this->face = (Configuration::Faces)xmlElem.attribute("face").toInt();
    this->measureTwoSides = xmlElem.attribute("measureTwoSides").toInt();
    this->timeDependent = xmlElem.attribute("timeDependent").toInt();
    this->distanceDependent = xmlElem.attribute("distanceDependent").toInt();
    this->timeInterval = xmlElem.attribute("timeInterval").toLong();
    this->distanceInterval = xmlElem.attribute("distanceInterval").toDouble();
    this->typeOfReading = (Configuration::ReadingTypes)xmlElem.attribute("typeOfReading").toInt();

    return true;

}
