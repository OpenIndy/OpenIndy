#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QDir>
#include <QPluginLoader>
#include <QApplication>
#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include "pi_oiplugin.h"
#include "pi_lasertracker.h"
#include "pi_totalstation.h"
#include "pi_fitfunction.h"
#include "sensor.h"
#include "simulationmodel.h"
#include "sensorcontrol.h"
#include "station.h"
#include "coordinatesystem.h"
#include "console.h"
#include "oimetadata.h"
#include "pluginmetadata.h"
#include "plugincopier.h"
#include "pi_oitool.h"


/*!
 * \brief The PluginLoader class
 */
class PluginLoader : public QObject
{
    Q_OBJECT
public:
    explicit PluginLoader(QObject *parent = 0);

    static PluginMetaData *myMetaInfo;

signals:
    
public slots:

//integrating an openIndy plugin
    //! try to find an oiPlugin
    static QString findOiPlugin(QString dirPath);

    //! get meta data
    static PluginMetaData* getPluginMetaData(QString path);

    //! copy the oiPlugin to the oi main application
    static bool copyPlugin(QString filename);

//loading stored openIndy plugins
    static Sensor* loadSensorPlugin(QString path, QString name);
    static Function* loadFunctionPlugin(QString path, QString name);
    static SimulationModel* loadSimulationPlugin(QString path, QString name);
    static NetworkAdjustment* loadNetworkAdjustmentPlugin(QString path, QString name);
    static OiTool* loadOiToolPlugin(QString path, QString name);
    static OiExchangeSimpleAscii *loadOiExchangeSimpleAsciiPlugin(QString path, QString name);
    static OiExchangeDefinedFormat *loadOiExchangeDefinedFormatPlugin(QString path, QString name);

    static QList<Sensor*> loadSensorPlugins(QString path);
    static QList<Function*> loadFunctionPlugins(QString path);
    static QList<SimulationModel*> loadSimulationPlugins(QString path);
    static QList<NetworkAdjustment*> loadNetworkAdjustmentPlugins(QString path);
    static QList<OiTool*> loadOiToolPlugins(QString path);
    static QList<OiExchangeSimpleAscii *> loadOiExchangeSimpleAsciiPlugins(QString path);
    static QList<OiExchangeDefinedFormat *> loadOiExchangeDefinedFormatPlugins(QString path);

    static PluginCopier* getCopier();
    static bool deletePlugin(PluginMetaData* metaData);

private:

    //! gets the meta Data of the oiPlugin
    static bool getMetaData(QString path);

    //! check whether the oiPlugin is valid
    static bool checkPlugin();


    static PluginCopier *pCopier;

};

#endif // PLUGINLOADER_H
