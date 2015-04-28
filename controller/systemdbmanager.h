#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QString>
#include <QList>
#include <QStringList>
#include <QDir>
#include <QMultiMap>
#include <QPointer>

#include "pluginmetadata.h"
#include "oimetadata.h"
#include "console.h"
#include "types.h"
#include "util.h"

//###############################################
//exchange classes to save database query results
//###############################################

namespace sdb{

class Function;
class Simulation;
class Sensor;
class Tool;
class NetworkAdjustment;
class Exchange;

//! represents an OpenIndy plugin
class Plugin{
public:
    Plugin() : id(-1), has_dependencies(false), is_active(false) {}

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

    QList<Function> functions;
    QList<Simulation> simulations;
    QList<Sensor> sensors;
    QList<Tool> tools;
    QList<NetworkAdjustment> networkAdjustments;
    QList<Exchange> exchanges;
};

//! represents a function
class Function{
public:
    Function() : id(-1){}

    int id;
    QString iid;
    QString name;
    QString description;

    Plugin plugin;

    QList<FeatureTypes> applicableFor;
    QList<ElementTypes> neededElements;
};

//! represents a sensor
class Sensor{
public:
    Sensor() : id(-1){}

    int id;
    QString iid;
    QString name;
    QString description;

    Plugin plugin;
};

//! represents a simulation
class Simulation{
public:
    Simulation() : id(-1){}

    int id;
    QString iid;
    QString name;
    QString description;

    Plugin plugin;
};

//! represents a tool
class Tool{
public:
    Tool() : id(-1){}

    int id;
    QString iid;
    QString name;
    QString description;

    Plugin plugin;
};

//! represents a network adjustment
class NetworkAdjustment{
public:
    NetworkAdjustment() : id(-1){}

    int id;
    QString iid;
    QString name;
    QString description;

    Plugin plugin;
};

//! represents an exchange function
class Exchange{
public:
    Exchange() : id(-1){}

    int id;
    QString iid;
    QString name;
    QString description;

    Plugin plugin;
};

}

//##################
//database interface
//##################

/*!
 * \brief The SystemDbManager class
 * System database interface class
 */
class SystemDbManager
{
public:

    //#####################
    //add or remove plugins
    //#####################

    static bool addPlugin(const sdb::Plugin &plugin);
    static bool removePlugin(const sdb::Plugin &plugin);

    //###########
    //get plugins
    //###########

    static QList<sdb::Plugin> getPlugins();

    //###########
    //get sensors
    //###########

    static QList<sdb::Sensor> getSensors();

    static QList<sdb::Sensor> getLaserTrackers();
    static QList<sdb::Sensor> getTotalStations();

    //#############
    //get functions
    //#############

    static QList<sdb::Function> getFunctions();

    static QList<sdb::Function> getCreateFunctions(const FeatureTypes &type);
    static QList<sdb::Function> getChangeFunctions(const FeatureTypes &type);

    //#########
    //get tools
    //#########

    static QList<sdb::Tool> getTools();

    //###############
    //get simulations
    //###############

    static QList<sdb::Simulation> getSimulations();

    //#######################
    //get network adjustments
    //#######################

    static QList<sdb::NetworkAdjustment> getNetworkAdjustments();

    //#############
    //get exchanges
    //#############

    static QList<sdb::Exchange> getExchanges();

    //###################
    //measurement configs
    //###################

    static QStringList getMeasurementConfigs();

    static bool addMeasurementConfig(const QString &name);
    static bool removeMeasurementConfig(const QString &name);

    static QString getDefaultMeasurementConfig(const QString &elementType);
    static bool setDefaultMeasurementConfig(const QString &name, const QString &elementType);

    //##############
    //sensor configs
    //##############

    static QStringList getSensorConfigs();

    static bool addSensorConfig(const QString &name);
    static bool removeSensorConfig(const QString &name);

    static QString getDefaultSensorConfig();
    static bool setDefaultSensorConfig(const QString &name);

private:

    //##########
    //connection
    //##########

    static QSqlDatabase db;
    static bool isInit;

    static void init();
    static bool connect();
    static void disconnect();

    //################
    //helper functions
    //################

    static int savePlugin(const sdb::Plugin &plugin);

    static void saveFunction(const sdb::Function &function, const int &plugin_id);
    static void saveSensor(const sdb::Sensor &sensor, const int &plugin_id);
    static void saveSimulation(const sdb::Simulation &simulation, const int &plugin_id);
    static void saveExchange(const sdb::Exchange &exchange, const int &plugin_id);
    static void saveTool(const sdb::Tool &tool, const int &plugin_id);
    static void saveNetworkAdjustment(const sdb::NetworkAdjustment &networkAdjustment, const int &plugin_id);

    static int getLastId(const QString &table);
    static QList<int> getElementIds(const QStringList &elements);

};

#endif // DATABASE_H
