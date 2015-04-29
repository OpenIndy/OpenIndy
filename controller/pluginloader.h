#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QPointer>
#include <QList>
#include <QString>
#include <QDir>
#include <QPluginLoader>
#include <QApplication>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

#include "console.h"
#include "pluginmetadata.h"
#include "plugin.h"
#include "sensor.h"
#include "function.h"
#include "simulationmodel.h"
#include "tool.h"
#include "networkadjustment.h"
#include "exchangeinterface.h"
#include "exchangesimpleascii.h"
#include "exchangedefinedformat.h"
#include "plugincopier.h"

/*!
 * \brief The PluginLoader class
 * Loads OpenIndy plugins
 */
class PluginLoader : public QObject
{
    Q_OBJECT

private:
    explicit PluginLoader(QObject *parent = 0);

public:

    //##########################
    //get plugin loader instance
    //##########################

    static QPointer<PluginLoader> getInstance();

    //#################################
    //get plugin meta data as a preview
    //#################################

    static PluginMetaData getPluginMetaData(const QString &path);

    //########################
    //import or remove plugins
    //########################

    static void importPlugin(const QString &path);
    static void deletePlugin(const QString &name);

    //#####################
    //load OpenIndy plugins
    //#####################

    static QPointer<Sensor> loadSensorPlugin(const QString &path, const QString &name);
    static QPointer<Function> loadFunctionPlugin(const QString &path, const QString &name);
    static QPointer<SimulationModel> loadSimulationPlugin(const QString &path, const QString &name);
    static QPointer<NetworkAdjustment> loadNetworkAdjustmentPlugin(const QString &path, const QString &name);
    static QPointer<Tool> loadToolPlugin(const QString &path, const QString &name);
    static QPointer<ExchangeSimpleAscii> loadExchangeSimpleAsciiPlugin(const QString &path, const QString &name);
    static QPointer<ExchangeDefinedFormat> loadExchangeDefinedFormatPlugin(const QString &path, const QString &name);

    static QList<QPointer<Sensor> > loadSensorPlugins(const QString &path);
    static QList<QPointer<Function> > loadFunctionPlugins(const QString &path);
    static QList<QPointer<SimulationModel> > loadSimulationPlugins(const QString &path);
    static QList<QPointer<NetworkAdjustment> > loadNetworkAdjustmentPlugins(const QString &path);
    static QList<QPointer<Tool> > loadToolPlugins(const QString &path);
    static QList<QPointer<ExchangeSimpleAscii> > loadExchangeSimpleAsciiPlugins(const QString &path);
    static QList<QPointer<ExchangeDefinedFormat> > loadExchangeDefinedFormatPlugins(const QString &path);

signals:

    //#####################
    //inform about progress
    //#####################

    void updateProgress(const int &progress);
    void sendMessage(const QString &msg);
    void sendError(const QString &error);
    void importFinished(const bool &success);
    void deletionFinished(const bool &success);

    //############
    //start import
    //############

    void startImport(const QString &path);
    void startDeletion(const QString &name);

private slots:

    //###############
    //clean up copier
    //###############

    void taskFinished();

private:

    //#############################
    //single plugin loader instance
    //#############################

    static QPointer<PluginLoader> myInstance;

    //########################
    //plugin copier and thread
    //########################

    static PluginCopier pluginCopier;
    static QThread copierThread;
    
    /*
public slots:

//integrating an openIndy plugin
    //! try to find an oiPlugin
    static QString findOiPlugin(QString dirPath);

    //! get meta data
    static PluginMetaData* getPluginMetaData(QString path);

    //! copy the oiPlugin to the oi main application
    static bool copyPlugin(QString filename);

//loading stored openIndy plugins


    //static PluginCopier* getCopier();
    static bool deletePlugin(PluginMetaData* metaData);

private:

    //! gets the meta Data of the oiPlugin
    static bool getMetaData(QString path);

    //! check whether the oiPlugin is valid
    static bool checkPlugin();


    //static PluginCopier *pCopier;*/

};

#endif // PLUGINLOADER_H
