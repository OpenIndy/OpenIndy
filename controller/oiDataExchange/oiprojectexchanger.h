#ifndef OIPROJECTEXCHANGER_H
#define OIPROJECTEXCHANGER_H

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QDateTime>
#include <QtAlgorithms>
#include "oiprojectdata.h"
#include "function.h"
#include "systemdbmanager.h"
#include "pluginloader.h"

class oiProjectExchanger
{
public:
    oiProjectExchanger();

    static bool saveProject(oiProjectData &data);
    static bool loadProject(oiProjectData &data);

    static bool saveSensorConfig(QString name);
    static SensorConfiguration* loadSensorConfig(QString name);

    static bool saveMeasurementConfig(QString name);
    static MeasurementConfig* loadMeasurementConfig(QString name);

    static bool saveSettings(QString name);
    //TODO create a settings object

private:

    static QList<Observation*> observations;
    static QList<FeatureWrapper*> geometries;
    static QList<FeatureWrapper*> stations;
    static QList<FeatureWrapper*> coordSystems;
    static QList<FeatureWrapper*> trafoParams;
    static QList<FeatureWrapper*> features;

    static QList<int> stationElements;

    //export functions
    static bool parseCommonFeatureToXML(QXmlStreamWriter& stream, Feature *f);
    static bool saveProxyObservation(QXmlStreamWriter& stream, QList<Observation*> o);
    static bool parseNominalGeometry(QXmlStreamWriter& stream, QList<Geometry*> g);
    static bool parseStationsToXML(QXmlStreamWriter& stream);
    static bool parseCoordSysToXML(QXmlStreamWriter& stream);
    static bool parseTrafoParamToXML(QXmlStreamWriter& stream);
    static bool parseGeometryToXML(QXmlStreamWriter& stream);
    static bool parseObservationToXML(QXmlStreamWriter& stream);

    //import functions
    static Observation* parseObservation(QXmlStreamReader& xml);
    static Reading* parseReading(QXmlStreamReader& xml);
    static FeatureWrapper* parseStation(QXmlStreamReader& xml);
    static FeatureWrapper* parseGeometry(QXmlStreamReader& xml);
    static FeatureWrapper* parseCoordinatesystem(QXmlStreamReader& xml);
    static FeatureWrapper* parseTrafoPara(QXmlStreamReader& xml);
    static void parseFunction(QXmlStreamReader& xml, FeatureWrapper *feature);
    static bool regenerateRelations(oiProjectData &data);
    static bool regenerateFeature(Feature* f);

    static Observation* findObservation(int id);
    static Geometry* findGeometrie(int id);
    static CoordinateSystem* findCoordSys(int id);
    static TrafoParam* findTrafoParam(int id);
    static FeatureWrapper* findFeature(int id);
    static Station* findStation(int id);


};

#endif // OIPROJECTEXCHANGER_H
