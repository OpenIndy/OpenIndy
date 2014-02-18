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
#include "sensorcontrol.h"
#include "station.h"
#include "coordinatesystem.h"
#include "console.h"
#include "oimetadata.h"
#include "pluginmetadata.h"


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
    //! load a laser tracker plugin
    static Sensor* loadSensorPlugin(QString path, QString name);
    static Function* loadFunctionPlugin(QString path, QString name);
    static NetworkAdjustment* loadNetworkAdjustmentPlugin(QString path, QString name);

    static QList<Sensor*> loadSensorPlugins(QString path);
    static QList<Function*> loadFunctionPlugins(QString path);
    static QList<NetworkAdjustment*> loadNetworkAdjustmentPlugins(QString path);


private:

    //! gets the meta Data of the oiPlugin
    static bool getMetaData(QString path);

    //! check whether the oiPlugin is valid
    static bool checkPlugin();

    static bool copyDependencies(QString dirPath);
    static bool copyDir(QString sourcePath, QString destinationPath);

};

#endif // PLUGINLOADER_H
