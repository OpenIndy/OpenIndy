#ifndef OICONFIGSTATE_H
#define OICONFIGSTATE_H

#include <QObject>
#include <exception>
#include <QString>
#include <QStringList>
#include <QStringListModel>

#include "configuration.h"
#include "measurementconfig.h"
#include "sensorconfiguration.h"
#include "featurewrapper.h"
#include "oifeaturestate.h"
#include "systemdbmanager.h"
#include "pluginloader.h"

class OiConfigState : public QObject
{
    Q_OBJECT
    friend class OiFeatureState;
private:
    explicit OiConfigState(QObject *parent = 0);

public:
    static OiConfigState *getInstance();

    //measurement config
    static const QList<MeasurementConfig> &getSavedMeasurementConfigs();
    static const QList<MeasurementConfig> &getProjectMeasurementConfigs();
    static QList<MeasurementConfig> getAllMeasurementConfigs();
    static MeasurementConfig getMeasurementConfig(QString displayName);
    static bool addMeasurementConfig(MeasurementConfig &mConfig);
    static bool setDefaultMeasurementConfig(MeasurementConfig mConfig, Configuration::FeatureTypes typeOfFeature);

    //sensor config
    static SensorConfiguration createConfigFromSensor(QString pluginName, QString sensorName);
    static SensorConfiguration getSensorConfig(QString displayName);

    //get models to represent config data
    static QStringListModel *getMeasurementConfigNames();
    static QStringListModel *getSensorConfigNames();

signals:
    void measurementConfigAdded();
    void measurementConfigRemoved();
    void activeMeasurementConfigChanged();

private slots:
    void geomMeasurementConfigChanged(int featureId);
    void observationAdded(int featureId);

private:
    static OiConfigState *myConfigState;

    static QList<MeasurementConfig> savedMeasurementConfigs; //measurement configs that were saved in configs folder
    static QList<MeasurementConfig> projectMeasurementConfigs; //measurement configs that are only available in the current project
    static QMap<QString, QList<Reading*> > usedMeasurementConfigs; //map with key = measurement config display name and value = list of readings that use the config

    static QList<SensorConfiguration> savedSensorConfigs; //sensor configs that were saved in configs folder
    static QList<SensorConfiguration> projectSensorConfigs; //sensor configs that are only available in the current project

    //models to represent config data
    static QStringListModel *measurementConfigNames; //the names of all available measurement configs
    static QStringListModel *sensorConfigNames; //the names of all available sensor configs

    //update models when configs where added or deleted
    static void updateMeasurementConfigModels();

    //load configuration files (xml) from config folder when starting OpenIndy
    static void loadSavedMeasurementConfigs();
    static void loadSavedSensorConfigs();

    //save configuration files (xml) to config folder
    static void saveMeasurementConfig(const MeasurementConfig &mConfig, bool override = false);

    //load and set default configs from database when starting OpenIndy

    //check a new measurement config wether it exists and wether it is valid
    static void checkMeasurementConfig(MeasurementConfig &mConfig, bool &sameName, bool &sameParameters, bool &isUsed);

    //called from OiFeatureState to do connects to inform OiConfigState when a specific event occurs
    static void connectFeature(FeatureWrapper *myFeature); //called from OiFeatureState whenever a feature is added
    static void disconnectFeature(FeatureWrapper *myFeature); //called from OiFeatureState whenever a feature is removed

    enum SignalType{
        eMeasurementConfigAdded,
        eMeasurementConfigRemoved,
        eActiveMeasurementConfigChanged
    };

    void emitSignal(SignalType mySignalType);
};

#endif // OICONFIGSTATE_H
