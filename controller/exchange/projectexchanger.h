#ifndef OIPROJECTEXCHANGER_H
#define OIPROJECTEXCHANGER_H

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QDateTime>
#include <QtAlgorithms>
#include <QMap>
#include <QtXml>
#include <QPointer>
#include <QFile>

#include "function.h"
#include "systemdbmanager.h"
#include "pluginloader.h"
#include "sensorcontrol.h"
#include "types.h"
#include "util.h"
#include "console.h"
#include "measurementconfigmanager.h"

using namespace oi;

/*!
 * \brief The OiProjectExchanger class
 * This class provides methods to save and load OpenIndy projects and different configs
 */
class ProjectExchanger
{
public:

    //##############################
    //save or load OpenIndy projects
    //##############################

    static QDomDocument saveProject(const QPointer<OiJob> &job);
    static const QPointer<OiJob> &loadProject(const QDomDocument &project);

    //#####################################
    //get or set measurement config manager
    //#####################################

    static QPointer<MeasurementConfigManager> &getMeasurementConfigManager();
    static void setMeasurementConfigManager(const QPointer<MeasurementConfigManager> &mConfigManager);

private:

    //#############################
    //helper methods and attributes
    //#############################

    //helper methods for loading different types of elements & configs
    static bool loadObservations(const QDomDocument &project);
    static bool loadStations(const QDomDocument &project);
    static bool loadCoordinateSystems(const QDomDocument &project);
    static bool loadTransformationParameters(const QDomDocument &project);
    static bool loadGeometries(const QDomDocument &project);
    static bool loadConfigs(const QDomDocument &project);

    //helper methods for restoring the dependencies between the loaded elements
    static bool restoreStationDependencies(const QDomDocument &project);
    static bool restoreCoordinateSystemDependencies(const QDomDocument &project);
    static bool restoreTrafoParamDependencies(const QDomDocument &project);
    static bool restoreGeometryDependencies(const QDomDocument &project);
    static bool restoreObservationDependencies(const QDomDocument &project);

    static QList<QPointer<Function> > restoreFunctionDependencies(const QDomElement &functions);

    //helper maps for loading an OpenIndy project (key is the id of an element)
    static QMap<int, QPointer<Observation> > myObservations;
    static QMap<int, QPointer<Reading> > myReadings;
    static QMap<int, QPointer<FeatureWrapper> > myStations;
    static QMap<int, QPointer<FeatureWrapper> > myCoordinateSystems;
    static QMap<int, QPointer<FeatureWrapper> > myTransformationParameters;
    static QMap<int, QPointer<FeatureWrapper> > myGeometries;

    //helper maps to load configs (QString is the name)
    static QMap<QString, MeasurementConfig> myMConfigs;
    static QMap<QString, SensorConfiguration> mySConfigs;

    static void clearHelperMaps(bool deleteOnClear);

    static QList<int> stationPoints;

    static QPointer<MeasurementConfigManager> mConfigManager;

};

#endif // OIPROJECTEXCHANGER_H
