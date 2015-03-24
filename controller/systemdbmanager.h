#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QSqlQueryModel>
#include <QString>
#include <QList>
#include <QDir>
#include <QMultiMap>
#include <QPointer>

#include "pluginmetadata.h"
#include "oimetadata.h"
#include "console.h"

//! base class for all plugin types
class SpecialPlugin{
public:
    int id;
    QString iid;
    QString name;
    QString description;
    QString pluginName;
};

//! function plugins
class FunctionPlugin : public SpecialPlugin{
public:
    QList<FeatureTypes> applicableFor;
    QList<ElementTypes> neededElements;
};

//! sensor plugins
class SensorPlugin : public SpecialPlugin{
};

//! simulation plugins
class SimulationPlugin : public SpecialPlugin{
};

//! tool plugins
class OiToolPlugin : public SpecialPlugin{
};

//! exchange plugins
class OiExchangePlugin : public SpecialPlugin{
};

//! class that holds information about an OpenIndy plugin
class Plugin{
public:
    int id;
    QString iid;
    QString name;
    QString description;
    QString version;
    QString author;
    QString compiler;
    QString operating_sys;
    bool has_dependencies;
    QString file_path;
    bool is_active;

    QList<FunctionPlugin> myFunctions;
    QList<SimulationPlugin> mySimulations;
    QList<SensorPlugin> mySensors;
    QList<OiToolPlugin> myTools;
};

//! database interface class
class SystemDbManager
{
public:
    //TODO QPointer const ?

    //##################################
    //public functions to query database
    //##################################

    //add or remove plugins
    static bool savePlugin(const Plugin &plugin);
    static bool deletePlugin(const Plugin &plugin);
    static bool deletePlugin(const QString &iid);

    //get sql models (function)
    static bool getCreateFunctionsModel(QPointer<QSqlQueryModel> &sqlModel, const FeatureTypes &typeOfFeature);
    static bool getChangeFunctionsModel(QPointer<QSqlQueryModel> &sqlModel, const FeatureTypes &typeOfFeature);
    static bool getNeededElementsModel(QPointer<QSqlQueryModel> &sqlModel, const FunctionPlugin &functionPlugin);

    //get sql models (sensor)
    static bool getLaserTrackerModel(QPointer<QSqlQueryModel> &sqlModel);
    static bool getTotalStationModel(QPointer<QSqlQueryModel> &sqlModel);
    static bool getUndefinedSensorModel(QPointer<QSqlQueryModel> &sqlModel);

    //add or remove measurement configs
    static bool addMeasurementConfig(const QString &name);
    static bool removeMeasurementConfig(const QString &name);
    static QString getDefaultMeasurementConfig(const GeometryTypes &typeOfGeometry);
    static bool setDefaultMeasurementConfig(const GeometryTypes &typeOfGeometry, const QString &name);

    //add or remove sensor configs
    static bool addSensorConfig(const QString &name);
    static bool removeSensorConfig(const QString &name);
    static QString getDefaultSensorConfig();
    static bool setDefaultSensorConfig(const QString &name);

    //get the file path of a plugin
    static bool getSensorPluginFilePath(QString &filePath, const QString &pluginName, const QString &sensorName);
    static bool getFunctionPluginFilePath(QString &filePath, const QString &pluginName, const QString &functionName);
    //bool getPluginFilePath(QString &filePath, const QString &pluginName, const QString &sensorName);
    //bool getPluginFilePath(QString &filePath, const QString &pluginName, const QString &sensorName);


/*
    static QString getPluginFilePath(QString name, QString plugin);

    static QStringList getSupportedGeometries();

    static QList<Plugin> getAvailablePlugins();
    static QStringList getAvailablePluginNames();

    static FunctionPlugin getDefaultFunction(FeatureTypes featureType);
    static QList<FunctionPlugin> getAvailableFitFunctions(FeatureTypes featureType);
    static QList<FunctionPlugin> getAvailableConstructFunctions(FeatureTypes featureType);

    static QList<SimulationPlugin> getAvailableSimulationPlugins();

    static void saveDefaultFunction(FeatureTypes featureType, QString function, QString plugin);

    static QMultiMap<QString,QString> getAvailableOiTools();

    static QMultiMap<QString,QString> getAvailableSimpleAsciiExchangePlugins();
    static QMultiMap<QString,QString> getAvailableDefinedFormatExchangePlugins();
*/

private:
    static QSqlDatabase db;
    static bool isInit;

    static void init();
    static bool connect();
    static void disconnect();

    static int getElementId(QString element);
    static QList<int> getElementIds(QList<QString> elements);
    static int getLastId(QString table);
    static int savePluginHelper(PluginMetaData *metaInfo);
    static void saveFunctionPlugin(int pluginId, Function* f);
    static void saveSensorPlugin(int pluginId, Sensor* s);
    static void saveSimulationPlugin(int pluginId, SimulationModel* s);
    static void saveNetworkAdjustmentPlugin(int pluginId, NetworkAdjustment* n);
    static void saveOiToolPlugin(int pluginId, OiTool* t);
    static void saveOiExchangeSimpleAsciiPlugin(int pluginId, OiExchangeSimpleAscii* sa);
    static void saveOiExchangeDefinedFormatPlugin(int pluginId, OiExchangeDefinedFormat* df);

};

#endif // DATABASE_H
