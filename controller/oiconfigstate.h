#ifndef OICONFIGSTATE_H
#define OICONFIGSTATE_H

#include <QObject>
#include <exception>

#include "configuration.h"
#include "measurementconfig.h"
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

    static MeasurementConfig getActiveMeasurementConfig();

signals:
    void activeMeasurementConfigChanged();

public slots:
    void featureSetChanged();

private slots:
    void setMeasurementConfig(int id);

private:
    static OiConfigState *myConfigState;

    MeasurementConfig activeMeasurementConfig;
    QList<MeasurementConfig> savedMeasurementConfigs; //measurement configs that were saved in configs folder
    //QMap< MeasurementConfig, QList<FeatureWrapper*> > usedMeasurementConfigs; //measurement configs that are used by one or more geometries

    enum SignalType{
        eMeasurementConfigAdded
    };

    static void connectFeature(FeatureWrapper *myFeature); //called from OiFeatureState whenever a feature is added
    static void disconnectFeature(FeatureWrapper *myFeature); //called from OiFeatureState whenever a feature is removed

    void emitSignal(SignalType mySignalType);
};

#endif // OICONFIGSTATE_H
