#ifndef FEATUREUPDATER_H
#define FEATUREUPDATER_H

#include <QObject>

#include "featurewrapper.h"
#include "feature.h"
#include "coordinatesystem.h"
#include "trafoparam.h"
#include "function.h"
#include "pi_systemtransformation.h"
#include "sortlistbyname.h"
#include "featureattributesexchange.h"
#include "console.h"
#include "systemdbmanager.h"
#include "pluginloader.h"
#include "oifeaturestate.h"
#include "trafocontroller.h"

class FeatureUpdater : public QObject
{
    Q_OBJECT
public:
    explicit FeatureUpdater(QObject *parent = 0);

    static TrafoController trafoControl;

    static bool validateFeatureName(QString featureName,FeatureAttributesExchange fae);
    static int addFeature(FeatureAttributesExchange fae, MeasurementConfig mConfig);
    static QStringList ConstructFeatureName(QString name, int count);
    static void sortFeatures();
    static void checkForNominals(FeatureWrapper *fw);
    static void addNominalToActual(FeatureWrapper *fw);
signals:
    
public slots:
    void recalcFeature(Feature *f);
    void recalcTrafoParam(TrafoParam *tp);
    void recalcFeatureSet();
    void switchCoordinateSystem(CoordinateSystem *to);
    TrafoParam* findTrafoParam(CoordinateSystem *searchSystem, QList<TrafoParam *> trafoParams);

    //bool deleteFeature(FeatureWrapper *myFeatureWrapper);

private:
    void fillTrafoParamFunctionNN(SystemTransformation *function, TrafoParam *tp);
    void fillTrafoParamFunctionAN(SystemTransformation *function, TrafoParam *tp);
    void fillTrafoParamFunctionAA(SystemTransformation *function, TrafoParam *tp);
    void switchCoordinateSystemWithoutTransformation(CoordinateSystem *to);

    static void createFeature(MeasurementConfig mConfig, QString name, bool nominal, FeatureAttributesExchange fae);

    static Function* getFunctionFromComboValue(QString functionValue, QString &functionName, QString &pluginName);

};

#endif // FEATUREUPDATER_H
