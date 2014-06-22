#include "simplepolarmeasurement.h"

SimplePolarMeasurement::SimplePolarMeasurement()
{
}

PluginMetaData *SimplePolarMeasurement::getMetaData()
{
    PluginMetaData* metaData = new PluginMetaData();

    metaData->name = "SimplePolarMeasurement_Simulation";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "mlux";
    metaData->description = "simulation of a simple polar measurement system";
    metaData->iid = "de.openIndy.Plugin.Simulation.v001";
    //...

    return metaData;
}

QMap<QString, UncertaintyComponent> SimplePolarMeasurement::getSensorUncertainties()
{
    QMap<QString, UncertaintyComponent> sensorUncertainties;

    //lambda
    UncertaintyComponent lambda;
    lambda.value = 0.0;
    lambda.uncertainty = 0.000403;
    lambda.errorUnit = UnitConverter::eMILLIMETER;
    lambda.errorDimension = UnitConverter::eMetric;

    /*trackerErrors->insert("lambda [mm]",0.000403);
    trackerErrors->insert("mu",0.000005);
    trackerErrors->insert("ex [mm]",0.0000122);
    trackerErrors->insert("by [mm]",0.0000654);
    trackerErrors->insert("bz [mm]",0.0000974);
    trackerErrors->insert("alpha [arc sec]",0.128);
    trackerErrors->insert("gamma [arc sec]",0.079);
    trackerErrors->insert("Aa1 [arc sec]",0.064);
    trackerErrors->insert("Ba1 [arc sec]",0.080);
    trackerErrors->insert("Aa2 [arc sec]",0.073);
    trackerErrors->insert("Ba2 [arc sec]",0.090);
    trackerErrors->insert("Ae0 [arc sec]",0.223);
    trackerErrors->insert("Ae1 [arc sec]",0.152);
    trackerErrors->insert("Be1 [arc sec]",0.183);
    trackerErrors->insert("Ae2 [arc sec]",0.214);
    trackerErrors->insert("Be2 [arc sec]",0.179);*/


    return sensorUncertainties;
}

QMap<QString, UncertaintyComponent> SimplePolarMeasurement::getObjectUncertainties()
{
    QMap<QString, UncertaintyComponent> objectUncertainties;


    return objectUncertainties;
}

QMap<QString, UncertaintyComponent> SimplePolarMeasurement::getEnviromentUncertainties()
{
    QMap<QString, UncertaintyComponent> enviromentUncertainties;


    return enviromentUncertainties;
}

QMap<QString, UncertaintyComponent> SimplePolarMeasurement::getHumanInfluence()
{
    QMap<QString, UncertaintyComponent> humanInfluence;


    return humanInfluence;
}

QMap<QString, int> *SimplePolarMeasurement::getIntegerParameter()
{
    return NULL;
}

QMap<QString, double> *SimplePolarMeasurement::getDoubleParameter()
{
    return NULL;
}

QMap<QString, QStringList> *SimplePolarMeasurement::getStringParameter()
{
    return NULL;
}

bool SimplePolarMeasurement::distort(Reading *r)
{
    return false;
}
