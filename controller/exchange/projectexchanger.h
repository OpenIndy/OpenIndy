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
#include <QCryptographicHash>

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
class OI_CORE_EXPORT ProjectExchanger
{
public:
    ProjectExchanger();
    ~ProjectExchanger();

    //##############################
    //save or load OpenIndy projects
    //##############################

    QDomDocument saveProject(const QPointer<OiJob> &job);
    const QPointer<OiJob> loadProject(const QDomDocument &project);

    //#####################################
    //get or set measurement config manager
    //#####################################

    QPointer<MeasurementConfigManager> &getMeasurementConfigManager();
    void setMeasurementConfigManager(const QPointer<MeasurementConfigManager> &mConfigManager);

private:

    //#############################
    //helper methods and attributes
    //#############################

    //helper methods for loading different types of elements & configs
    bool loadObservations(const QDomDocument &project);
    bool loadStations(const QDomDocument &project);
    bool loadCoordinateSystems(const QDomDocument &project);
    bool loadTransformationParameters(const QDomDocument &project);
    bool loadGeometries(const QDomDocument &project);
    bool loadConfigs(const QDomDocument &project);

    //helper methods for restoring the dependencies between the loaded elements
    bool restoreStationDependencies(const QDomDocument &project);
    bool restoreCoordinateSystemDependencies(const QDomDocument &project);
    bool restoreTrafoParamDependencies(const QDomDocument &project);
    bool restoreGeometryDependencies(const QDomDocument &project);
    bool restoreObservationDependencies(const QDomDocument &project);

    QList<QPointer<Function> > restoreFunctionDependencies(const QDomElement &functions);
    QPointer<BundleAdjustment> restoreBundleDependencies(QDomElement &bundle);

    //helper maps for loading an OpenIndy project (key is the id of an element)
    QMap<int, QPointer<Observation> > myObservations;
    QMap<int, QPointer<Reading> > myReadings;
    QMap<int, QPointer<FeatureWrapper> > myStations;
    QMap<int, QPointer<FeatureWrapper> > myCoordinateSystems;
    QMap<int, QPointer<FeatureWrapper> > myTransformationParameters;
    QMap<int, QPointer<FeatureWrapper> > myGeometries;

    // all loaded MeasurementConfigs from project file
    QMap<QString, MeasurementConfig> measurementConfigs;
    QMap<QString, SensorConfiguration> mySConfigs;

    void clearHelperMaps(bool deleteOnClear);

    QList<int> stationPoints;

    QPointer<MeasurementConfigManager> mConfigManager;

};

#endif // OIPROJECTEXCHANGER_H
