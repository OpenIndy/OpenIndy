#ifndef OICONFIGSTATE_H
#define OICONFIGSTATE_H

#include <QObject>
#include <exception>
#include <QString>
#include <QStringList>

#include "configuration.h"
#include "measurementconfig.h"
#include "sensorconfiguration.h"
#include "featurewrapper.h"
#include "oifeaturestate.h"

class OiConfigState : public QObject
{
    Q_OBJECT
    friend class OiFeatureState;
private:
    explicit OiConfigState(QObject *parent = 0);

public:
    static OiConfigState *getInstance();

    static const QList<MeasurementConfig> &getSavedMeasurementConfigs();
    static const QList<MeasurementConfig> &getProjectMeasurementConfigs();
    static QList<MeasurementConfig> getAllMeasurementConfigs();

signals:
    void measurementConfigAdded();
    void measurementConfigRemoved();
    void activeMeasurementConfigChanged();

private slots:
    void defaultMeasurementConfigChanged(int featureId);
    void observationAdded(int featureId);

private:
    static OiConfigState *myConfigState;

    static QList<MeasurementConfig> savedMeasurementConfigs; //measurement configs that were saved in configs folder
    static QList<MeasurementConfig> projectMeasurementConfigs; //measurement configs that are only available in the current project
    static QMap<QString, QList<Reading*> > usedMeasurementConfigs; //map with key = measurement config display name and value = list of readings that use the config

    //load configuration files (xml) from config folder when starting OpenIndy
    static void loadSavedMeasurementConfigs();
    static void loadSavedSensorConfigs();

    //load and set default configs from database when starting OpenIndy

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
