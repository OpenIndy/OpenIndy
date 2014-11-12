#ifndef OICONFIGSTATE_H
#define OICONFIGSTATE_H

#include <QObject>
#include <exception>

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

    static const MeasurementConfig &getActiveMeasurementConfig();
    static const QList<MeasurementConfig> &getSavedMeasurementConfigs();
    static const QList<MeasurementConfig> &getProjectMeasurementConfigs();
    static QList<MeasurementConfig> getAllMeasurementConfigs();

signals:
    void measurementConfigAdded();
    void measurementConfigRemoved();
    void activeMeasurementConfigChanged();

public slots:
    void featureSetChanged();

private slots:
    void setMeasurementConfig(int id);

private:
    static OiConfigState *myConfigState;

    MeasurementConfig activeMeasurementConfig;
    QList<MeasurementConfig> savedMeasurementConfigs; //measurement configs that were saved in configs folder
    QList<MeasurementConfig> projectMeasurementConfigs; //measurement configs that are only available in the current project
    QMap<int, QList<Observation*> > usedMeasurementConfigs; //map with key = measurement config id and value = list of observations that use the config

    enum SignalType{
        eMeasurementConfigAdded,
        eMeasurementConfigRemoved,
        eActiveMeasurementConfigChanged
    };

    static void connectFeature(FeatureWrapper *myFeature); //called from OiFeatureState whenever a feature is added
    static void disconnectFeature(FeatureWrapper *myFeature); //called from OiFeatureState whenever a feature is removed

    void emitSignal(SignalType mySignalType);
};

#endif // OICONFIGSTATE_H
