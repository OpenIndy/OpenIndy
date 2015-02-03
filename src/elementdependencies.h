#ifndef ELEMENTDEPENDENCIES_H
#define ELEMENTDEPENDENCIES_H



#include "configuration.h"
#include <QList>

struct featureIndex{

    int idx;
    int typeOfElement;

};

struct functionInfo{
    QString name;
    QString plugin;
    int executionIndex;
    QMap<int,featureIndex> neededElements;
};

struct sensorInfo{
    QString name;
    QString plugin;
};

//TODO delete this class


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
    bool addActiveSensor(sensorInfo info);

    bool addUsedFor(int id);
    bool addPreviouslyNeeded(int id);

    bool setStationPosition(int id);
    bool setStationCoordSystem(int id);
    int getStationPosition();
    int getStationCoordSystem();

    void setFromSystem(int id);
    void setToSystem(int id);
    int getFromSystem();
    int getToSystem();

    bool hasPluginSensor();
    bool hasPluginFunction();
    bool hasObservation();
    bool hasFeature();

    QList<int> getObservationDependencies();
    QMap< QString,QList<int>* > getfeatureDependencies();

    QList<functionInfo> getNeededFunctions();
    QList<sensorInfo> getNeededSensors();

    sensorInfo getActiveSensor();

    QList<int> getUsedFor();
    QList<int> getPreviouslyNeeded();

private:
    bool hasSensorPlugin;
    bool hasFunctionPlugin;
    bool hasObservations;
    bool hasFeatures;

    QList<int> observations;
    QMap< QString,QList<int>* > features;

    QList<functionInfo> neededFunctions;
    QList<sensorInfo> neededSensors;
    sensorInfo activeSensor;

    int stationCoordSystem;
    int stationPosition;

    QList<int> usedFor;
    QList<int> previouslyNeeded;

    int from, to; //start an dest coordinate systems of a trafoParam

};

#endif // ELEMENTDEPENDENCIES_H
