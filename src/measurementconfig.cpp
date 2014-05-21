#include "measurementconfig.h"

MeasurementConfig::MeasurementConfig()
{
    typeOfReading = static_cast<Configuration::ReadingTypes>(-1);
}

bool MeasurementConfig::toOpenIndyXML(QXmlStreamWriter &stream)
{

    stream.writeStartElement("MeasurementConfig");

    stream.writeAttribute("name", this->name);

    stream.writeTextElement("count", QString::number(this->count));
    stream.writeTextElement("face", QString::number(this->face));
    stream.writeTextElement("iterations",QString::number(this->iterations));
    stream.writeTextElement("measureTwoSides",QString::number(this->measureTwoSides));
    stream.writeTextElement("timeDependent",QString::number(this->timeDependent));
    stream.writeTextElement("distanceDependent", QString::number(this->distanceDependent));
    stream.writeTextElement("timeInterval", QString::number(this->timeInterval));
    stream.writeTextElement("distanceInterval", QString::number(this->distanceInterval));
    stream.writeTextElement("typeOfReading", QString::number(this->typeOfReading));

    stream.writeEndElement();

   return true;
}

ElementDependencies MeasurementConfig::fromOpenIndyXML(QXmlStreamReader &xml)
{
    ElementDependencies e;


    return e;
}
