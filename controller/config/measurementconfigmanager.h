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

using namespace oi;

/*!
 * \brief The MeasurementConfigManager class
 * Used to save and manage all available measurement configs
 */
class MeasurementConfigManager : public QObject
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

    bool hasSavedMeasurementConfig(const QString &name);
    bool hasProjectMeasurementConfig(const QString &name);

    bool hasSavedMeasurementConfig(const MeasurementConfig &mConfig);
    bool hasProjectMeasurementConfig(const MeasurementConfig &mConfig);

    MeasurementConfig getSavedMeasurementConfig(const QString &name) const;
    MeasurementConfig getProjectMeasurementConfig(const QString &name) const;

    QList<MeasurementConfig> getSavedMeasurementConfigs() const;
    QList<MeasurementConfig> getProjectMeasurementConfigs() const;

    MeasurementConfig getActiveMeasurementConfig(const GeometryTypes &type) const;

    void addMeasurementConfig(const MeasurementConfig &mConfig);
    void addProjectMeasurementConfig(const MeasurementConfig &mConfig);

    void removeMeasurementConfig(const QString &name);
    void removeProjectMeasurementConfig(const QString &name);
    void removeAllProjectMeasurementConfigs();

    void replaceMeasurementConfig(const QString &name, const MeasurementConfig &mConfig);

    void loadFromConfigFolder();

signals:

    //##################################
    //inform about sensor config changes
    //##################################

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

    QList<MeasurementConfig> savedMeasurementConfigList;
    QList<MeasurementConfig> projectMeasurementConfigList;

    QMap<GeometryTypes, MeasurementConfig> activeMeasurementConfigs;

    //#################
    //helper attributes
    //#################

    QPointer<OiJob> currentJob;

};

#endif // MEASUREMENTCONFIGURATIONMANAGER_H
