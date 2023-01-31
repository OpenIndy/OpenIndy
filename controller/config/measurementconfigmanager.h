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
    bool hasSavedMeasurementConfig(const QString &name);
    bool hasProjectMeasurementConfig(const QString &name);
    bool hasSavedMeasurementConfig(const MeasurementConfig &mConfig);
    bool hasProjectMeasurementConfig(const MeasurementConfig &mConfig);

    //get configs
    MeasurementConfig getSavedMeasurementConfig(const QString &name) const;
    MeasurementConfig getProjectMeasurementConfig(const QString &name) const;
    const QList<MeasurementConfig> getSavedMeasurementConfigs() const;
    const QList<MeasurementConfig> getProjectMeasurementConfigs() const;

    //active config
    MeasurementConfig getActiveMeasurementConfig(const GeometryTypes &type) const;

    //add or remove configs
    void addSavedMeasurementConfig(const MeasurementConfig &mConfig);
    void addProjectMeasurementConfig(const MeasurementConfig &mConfig);
    void removeSavedMeasurementConfig(const QString &name);
    void removeProjectMeasurementConfig(const QString &name);
    void removeAllSavedMeasurementConfigs();
    void removeAllProjectMeasurementConfigs();

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
    void saveMeasurementConfig(const MeasurementConfig &mConfig);
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

    QMap<QString, MeasurementConfig> savedMeasurementConfigMap;
    QMap<QString, MeasurementConfig> projectMeasurementConfigMap;

    QMap<GeometryTypes, MeasurementConfig> activeMeasurementConfigs;

    //#################
    //helper attributes
    //#################

    QPointer<OiJob> currentJob;

};

#endif // MEASUREMENTCONFIGURATIONMANAGER_H
