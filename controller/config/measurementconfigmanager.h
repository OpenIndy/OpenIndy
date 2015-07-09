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

    //##############################
    //get or set measurement configs
    //##############################

    MeasurementConfig getSavedMeasurementConfig(const QString &name) const;
    QList<MeasurementConfig> getSavedMeasurementConfigs() const;
    QList<MeasurementConfig> getProjectMeasurementConfigs() const;
    const MeasurementConfig &getActiveMeasurementConfig(const GeometryTypes &type) const;

    void addMeasurementConfig(const MeasurementConfig &mConfig);
    void removeMeasurementConfig(const QString &name);

    void replaceMeasurementConfig(const QString &name, const MeasurementConfig &mConfig);

    void loadFromConfigFolder();

signals:

    //##################################
    //inform about sensor config changes
    //##################################

    void measurementConfigurationsChanged();
    void activeMeasurementConfigurationChanged(const GeometryTypes &type);

    //############
    //log messages
    //############

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

private:

    //########################
    //save measurement configs
    //########################

    QMap<QString, MeasurementConfig> savedMeasurementConfigs;
    QMap<QString, MeasurementConfig> projectMeasurementConfigs;

    QMap<GeometryTypes, MeasurementConfig> activeMeasurementConfigs;

    //##############
    //helper methods
    //##############

    void saveMeasurementConfig(const MeasurementConfig &mConfig);
    void deleteMeasurementConfig(const QString &name);

};

#endif // MEASUREMENTCONFIGURATIONMANAGER_H
