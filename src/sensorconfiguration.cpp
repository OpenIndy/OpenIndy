#include "sensorconfiguration.h"

SensorConfiguration::SensorConfiguration()
{
    connConfig = new ConnectionConfig();
}

bool SensorConfiguration::toOpenIndyXML(QXmlStreamWriter &stream)
{

    return true;
}

ElementDependencies SensorConfiguration::fromOpenIndyXML(QXmlStreamReader &xml)
{
    ElementDependencies e;


    return e;
}
