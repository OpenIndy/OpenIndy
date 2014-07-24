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

#include "projectrestorer.h"
#include <QThread>

/*!
 * \brief The oiProjectExchanger class
 * static class for generating a openindyXML of the actual project or to restore
 * a project from a openindyXML.
 */

class OiProjectExchanger
{
public:
    OiProjectExchanger();

    static bool saveProject(OiProjectData &data);
    static bool loadProject(OiProjectData &data);

    static bool saveSensorConfig(QString name);
    static SensorConfiguration* loadSensorConfig(QString name);

    static bool saveMeasurementConfig(QString name);
    static MeasurementConfig* loadMeasurementConfig(QString name);

    static bool saveSettings(QString name);
    //TODO create a settings object

private:

    static ProjectRestorer *restorer;
    QThread workingThread;

    static QList<Observation*> observations;
    static QList<FeatureWrapper*> geometries;
    static QList<FeatureWrapper*> stations;
    static QList<FeatureWrapper*> coordSystems;
    static QList<FeatureWrapper*> trafoParams;
    static QList<FeatureWrapper*> features;
    static QList<ElementDependencies> dependencies;

    static QList<int> stationElements;

    static bool regenerateRelations(OiProjectData &data);
    static bool regenerateFeature(Feature* f);

    static Observation* findObservation(int id);
    static Geometry* findGeometrie(int id);
    static CoordinateSystem* findCoordSys(int id);
    static TrafoParam* findTrafoParam(int id);
    static FeatureWrapper* findFeature(int id);
    static Station* findStation(int id);


};

#endif // OIPROJECTEXCHANGER_H
