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

using namespace oi;

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

    //check presence of a config
    bool hasSavedSensorConfig(const QString &name);
    bool hasProjectSensorConfig(const QString &name);
    bool hasSavedSensorConfig(const SensorConfiguration &sConfig);
    bool hasProjectSensorConfig(const SensorConfiguration &sConfig);

    //get configs
    SensorConfiguration getSavedSensorConfig(const QString &name) const;
    SensorConfiguration getProjectSensorConfig(const QString &name) const;
    const QList<SensorConfiguration> &getSavedSensorConfigs() const;
    const QList<SensorConfiguration> &getProjectSensorConfigs() const;

    //active config
    const SensorConfiguration &getActiveSensorConfig() const;
    void setActiveSensorConfig(const SensorConfiguration &sConfig);

    //add or remove configs
    void addSavedSensorConfig(const SensorConfiguration &sConfig, bool save = true);
    void addProjectSensorConfig(const SensorConfiguration &sConfig);
    void removeSavedSensorConfig(const QString &name, bool save = true);
    void removeProjectSensorConfig(const QString &name);
    void removeAllSavedSensorConfigs(bool save = true);
    void removeAllProjectSensorConfigs();

    //replace a config
    void replaceSensorConfig(const QString &name, const SensorConfiguration &sConfig, bool save = true);

    //load configs from xml
    void loadFromConfigFolder();

    //synchronize config manager
    void synchronize(const SensorConfigurationManager &other);

signals:

    //##################################
    //inform about sensor config changes
    //##################################

    void sensorConfigurationsChanged();
    void sensorConfigurationReplaced(const SensorConfiguration &oldSConfig, const SensorConfiguration &newSConfig);
    void activeSensorConfigurationChanged();

    //############
    //log messages
    //############

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

private:

    //##############
    //helper methods
    //##############

    //save or remove configs
    void saveSensorConfig(const SensorConfiguration &sConfig, bool save);
    void deleteSensorConfig(const QString &name, bool save);

    //compare two configs
    bool equals(const SensorConfiguration &sConfigA, const SensorConfiguration &sConfigB);

    //###################
    //save sensor configs
    //###################

    QMap<QString, SensorConfiguration> savedSensorConfigsMap;
    QMap<QString, SensorConfiguration> projectSensorConfigsMap;

    QList<SensorConfiguration> savedSensorConfigsList;
    QList<SensorConfiguration> projectSensorConfigsList;

    SensorConfiguration activeSensorConfig;

};

Q_DECLARE_METATYPE( SensorConfigurationManager )
Q_DECLARE_METATYPE( SensorConfigurationManager* )

#endif // SENSORCONFIGURATIONMANAGER_H
