#include "measurementconfig.h"

MeasurementConfig::MeasurementConfig(){
    this->typeOfReading = static_cast<Configuration::ReadingTypes>(-1);
}

/*!
 * \brief MeasurementConfig::toOpenIndyXML
 * \param stream
 * \return
 */
bool MeasurementConfig::toOpenIndyXML(QXmlStreamWriter &stream){

    stream.writeStartElement("MeasurementConfig");

    stream.writeAttribute("name", this->name);
    stream.writeAttribute("internalRef", QString::number(this->internalRef));
    stream.writeAttribute("count", QString::number(this->count));
    stream.writeAttribute("face", QString::number(this->face));
    stream.writeAttribute("iterations", QString::number(this->iterations));
    stream.writeAttribute("measureTwoSides", this->measureTwoSides?"1":"0");
    stream.writeAttribute("timeDependent", this->timeDependent?"1":"0");
    stream.writeAttribute("distanceDependent", this->distanceDependent?"1":"0");
    stream.writeAttribute("timeInterval", QString::number(this->timeInterval));
    stream.writeAttribute("distanceInterval", QString::number(this->distanceInterval));
    stream.writeAttribute("typeOfReading", QString::number(this->typeOfReading));

    stream.writeEndElement();

    return true;

}

/*!
 * \brief MeasurementConfig::fromOpenIndyXML
 * \param xml
 * \return
 */
ElementDependencies MeasurementConfig::fromOpenIndyXML(QXmlStreamReader &xml){

    ElementDependencies dependencies;

    QXmlStreamAttributes attributes = xml.attributes();
    if(attributes.hasAttribute("name")){
        this->name = attributes.value("name").toString();
    }
    if(attributes.hasAttribute("count")){
        this->count = attributes.value("count").toInt();
    }
    if(attributes.hasAttribute("face")){
        this->face = (Configuration::Faces)attributes.value("face").toInt();
    }
    if(attributes.hasAttribute("iterations")){
        this->iterations = attributes.value("iterations").toInt();
    }
    if(attributes.hasAttribute("measureTwoSides")){
        this->measureTwoSides = attributes.value("measureTwoSides").toInt();
    }
    if(attributes.hasAttribute("timeDependent")){
        this->timeDependent = attributes.value("timeDependent").toInt();
    }
    if(attributes.hasAttribute("distanceDependent")){
        this->distanceDependent = attributes.value("distanceDependent").toInt();
    }
    if(attributes.hasAttribute("timeInterval")){
        this->timeInterval = attributes.value("timeInterval").toLong();
    }
    if(attributes.hasAttribute("distanceInterval")){
        this->distanceInterval = attributes.value("distanceInterval").toDouble();
    }
    if(attributes.hasAttribute("typeOfReading")){
        this->typeOfReading = (Configuration::ReadingTypes)attributes.value("typeOfReading").toInt();
    }

    xml.readNext();

    return dependencies;

}
