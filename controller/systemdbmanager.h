#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QSqlQueryModel>
#include <QString>
#include <QList>
#include <QDir>

#include "pluginmetadata.h"
#include "oimetadata.h"
#include "console.h"
#include "configuration.h"

struct FunctionPlugin{
    int id;
    QString iid;
    QString name;
    QString description;
    QString pluginName;
    QList<Configuration::FeatureTypes> applicableFor;
    QList<Configuration::ElementTypes> neededElements;
};

struct SensorPlugin{
    int id;
    QString iid;
    QString name;
    QString description;
    QString pluginName;
};

struct SimulationPlugin{
    int id;
    QString iid;
    QString name;
    QString description;
    QString pluginName;
};

struct Plugin{
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
};

class SystemDbManager
{
public:
    static int savePlugin(PluginMetaData *metaInfo, QList<Function*> functions, QList<Sensor*> sensors, QList<NetworkAdjustment*> networkAdjustments,QList<SimulationModel*> simulationList );
    static bool deletePlugin(int id);

    static bool getCreateFunctionModel(QSqlQueryModel *sqlModel, Configuration::FeatureTypes ft);
    static bool getChangeFunctionModel(QSqlQueryModel *sqlModel, Configuration::FeatureTypes ft);
    static bool getNeededElements(QSqlQueryModel *sqlModel, int id);

    static bool getLaserTrackerModel(QSqlQueryModel *sqlModel);
    static bool getTotalStationModel(QSqlQueryModel *sqlModel);
    static bool getUndefinedSensorModel(QSqlQueryModel *sqlModel);

    static QString getPluginFilePath(QString name, QString plugin);

    static QStringList getSupportedGeometries();

    static QList<Plugin> getAvailablePlugins();
    static FunctionPlugin getDefaultFunction(Configuration::FeatureTypes featureType);
    static QList<FunctionPlugin> getAvailableFitFunctions(Configuration::FeatureTypes featureType);
    static QList<FunctionPlugin> getAvailableConstructFunctions(Configuration::FeatureTypes featureType);

    static void saveDefaultFunction(Configuration::FeatureTypes featureType, QString function, QString plugin);

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

};

#endif // DATABASE_H
