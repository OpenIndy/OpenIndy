#ifndef ELEMENTDEPENDENCIES_H
#define ELEMENTDEPENDENCIES_H



#include"configuration.h"

struct functionInfo{
    QString name;
    QString plugin;
    int executionIndex;
    QMap<int,int> neededElements;
};

struct sensorInfo{
    QString name;
    QString plugin;
};



class ElementDependencies
{
public:
    ElementDependencies();

    int elementID;
    Configuration::ElementTypes typeOfElement;

    bool addFeatureID(int id,QString featureGroup);
    bool addObservationID(int id);
    bool addFunctionInfo(functionInfo info);
    bool addSensorInfo(sensorInfo info);

private:
    bool hasSensorPlugin;
    bool hasFunctionPlugin;
    bool hasObservations;
    bool hasFeatures;

    QList<int> observations;
    QMap< QString,QList<int>* > features;

    QList<functionInfo> neededFunctions;
    QList<sensorInfo> neededSensors;




};

#endif // ELEMENTDEPENDENCIES_H
