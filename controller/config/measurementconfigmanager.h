#ifndef MEASUREMENTCONFIGURATIONMANAGER_H
#define MEASUREMENTCONFIGURATIONMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QApplication>

#include "types.h"
#include "systemdbmanager.h"
#include "measurementconfig.h"
#include "console.h"

#if defined(OI_MAIN_LIB)
#  define OI_MAIN_EXPORT Q_DECL_EXPORT
#else
#  define OI_MAIN_EXPORT Q_DECL_IMPORT
#endif

using namespace oi;

/*!
 * \brief The MeasurementConfigManager class
 * Used to save and manage all available measurement configs
 */
class OI_MAIN_EXPORT MeasurementConfigManager : public QObject
{
    Q_OBJECT

public:
    explicit MeasurementConfigManager(QObject *parent = 0);

    MeasurementConfigManager(const MeasurementConfigManager &copy, QObject *parent = 0);

    MeasurementConfigManager &operator=(const MeasurementConfigManager &copy);

    //###################################
    //get or set the current OpenIndy job
    //###################################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    //##############################
    //get or set measurement configs
    //##############################

    //get configs
    MeasurementConfig getConfig(const MeasurementConfigKey &key) const;
    MeasurementConfig getUserConfig(const QString &name) const;
    MeasurementConfig getProjectConfig(const QString &name) const;
    MeasurementConfig getStandardConfig(const QString &name) const;
    MeasurementConfig findConfig(const QString &name) const;
    const QList<MeasurementConfig> getConfigs() const;
    const QList<MeasurementConfig> getUserConfigs() const;
    const QList<MeasurementConfig> getProjectConfigs() const;
    const QList<QString> getUserConfigNames() const;
    const QList<QString> getProjectConfigNames() const;

    //active config
    MeasurementConfig getActiveConfig(const GeometryTypes &type) const;

    //add or remove configs
    void saveUserConfig(const MeasurementConfig &mConfig);
    void addProjectConfig(const MeasurementConfig &mConfig);
    void removeUserConfig(const QString &name);
    void removeProjectConfig(const QString &name);

    //replace a config
    void replaceMeasurementConfig(const MeasurementConfigKey &oldKey, const MeasurementConfig &mConfig);

    void saveToConfigFolder(const QString &name, const MeasurementConfig &mConfig);
    //load configs from xml
    void loadFromConfigFolder();

    //synchronize config manager
    void synchronize(const MeasurementConfigManager &other);

signals:

    //#######################################
    //inform about measurement config changes
    //#######################################

    void measurementConfigurationsChanged();

    //############
    //log messages
    //############

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

private:

    //##############
    //helper methods
    //##############

    //save or remove configs
    void saveConfig(const MeasurementConfig &mConfig);
    void deleteMeasurementConfig(const QString &name);

    //job connects
    void connectJob();
    void disconnectJob();

    //########################
    //save measurement configs
    //########################

    QMap<MeasurementConfigKey, MeasurementConfig> configs;

    QMap<GeometryTypes, MeasurementConfig> activeMeasurementConfigs;

    //#################
    //helper attributes
    //#################

    QPointer<OiJob> currentJob;

};

#endif // MEASUREMENTCONFIGURATIONMANAGER_H
