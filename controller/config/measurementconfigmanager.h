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

    //check presence of a config
    bool isStandardConfig(const QString &name);
    bool isUserConfig(const QString &name);
    bool isProjectConfig(const QString &name);
    bool isUserConfig(const MeasurementConfig &mConfig);
    bool isProjectConfig(const MeasurementConfig &mConfig);

    //get configs
    MeasurementConfig getUserConfig(const QString &name) const;
    MeasurementConfig getProjectConfig(const QString &name) const;
    const QList<MeasurementConfig> getUserConfigs() const;
    const QList<MeasurementConfig> getProjectConfigs() const;

    //active config
    MeasurementConfig getActiveConfig(const GeometryTypes &type) const;

    //add or remove configs
    void addUserConfig(const MeasurementConfig &mConfig);
    void addProjectConfig(const MeasurementConfig &mConfig);
    void removeUserConfig(const QString &name);
    void removeProjectConfig(const QString &name);
    void removeAllUserConfigs();
    void removeAllProjectConfigs();

    //replace a config
    void replaceMeasurementConfig(const QString &name, const MeasurementConfig &mConfig);

    //load configs from xml
    void loadFromConfigFolder();

    //synchronize config manager
    void synchronize(const MeasurementConfigManager &other);

signals:

    //#######################################
    //inform about measurement config changes
    //#######################################

    void measurementConfigurationsChanged();
    void measurementConfigurationReplaced(const MeasurementConfig &oldMConfig, const MeasurementConfig &newMConfig);
    void activeMeasurementConfigurationChanged(const GeometryTypes &type);

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

    //update geometries when measurement configs change
    void updateGeometries();
    void updateGeometries(const MeasurementConfig &oldMConfig, const MeasurementConfig &newMConfig);

    //job connects
    void connectJob();
    void disconnectJob();

    //########################
    //save measurement configs
    //########################

    QMap<QString, MeasurementConfig> userConfigs;
    QMap<QString, MeasurementConfig> projectConfigs;

    QMap<GeometryTypes, MeasurementConfig> activeMeasurementConfigs;

    //#################
    //helper attributes
    //#################

    QPointer<OiJob> currentJob;

};

#endif // MEASUREMENTCONFIGURATIONMANAGER_H
