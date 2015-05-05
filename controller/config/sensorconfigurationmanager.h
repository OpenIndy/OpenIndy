#ifndef SENSORCONFIGURATIONMANAGER_H
#define SENSORCONFIGURATIONMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QApplication>

#include "types.h"
#include "systemdbmanager.h"
#include "sensorconfiguration.h"
#include "console.h"

/*!
 * \brief The SensorConfigurationManager class
 * Used to save and manage all available sensor configs
 */
class SensorConfigurationManager : public QObject
{
    Q_OBJECT

public:
    explicit SensorConfigurationManager(QObject *parent = 0);

    SensorConfigurationManager(const SensorConfigurationManager &copy, QObject *parent = 0);

    SensorConfigurationManager &operator=(const SensorConfigurationManager &copy);

    //#########################
    //get or set sensor configs
    //#########################

    SensorConfiguration getSavedSensorConfig(const QString &name) const;
    QList<SensorConfiguration> getSavedSensorConfigs() const;
    QList<SensorConfiguration> getProjectSensorConfigs() const;
    const SensorConfiguration &getActiveSensorConfig(const QString &name) const;

    void addSensorConfig(const SensorConfiguration &sConfig);
    void removeSensorConfig(const QString &name);

    void replaceSensorConfig(const QString &name, const SensorConfiguration &sConfig);

    void loadFromConfigFolder();

signals:

    //##################################
    //inform about sensor config changes
    //##################################

    void sensorConfigurationsChanged();
    void activeSensorConfigurationChanged();

private:

    //###################
    //save sensor configs
    //###################

    QMap<QString, SensorConfiguration> savedSensorConfigs;
    QMap<QString, SensorConfiguration> projectSensorConfigs;

    SensorConfiguration activeSensorConfig;

    //##############
    //helper methods
    //##############

    void saveSensorConfig(const SensorConfiguration &sConfig);
    void deleteSensorConfig(const QString &name);

};

#endif // SENSORCONFIGURATIONMANAGER_H
