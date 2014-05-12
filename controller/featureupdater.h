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

class FeatureUpdater : public QObject
{
    Q_OBJECT
public:
    explicit FeatureUpdater(QObject *parent = 0);

    static bool validateFeatureName(QList<FeatureWrapper*> features, QString featureName,FeatureAttributesExchange fae);
    static int addFeature(QList<Station*>&stations,QList<CoordinateSystem*>&coordSys, QList<FeatureWrapper*>&features, FeatureAttributesExchange fae, MeasurementConfig mConfig);
    static QStringList ConstructFeatureName(QString name, int count);
    static void sortFeatures(QList<FeatureWrapper*> &features);
    static void checkForNominals(QList<FeatureWrapper*>&features, FeatureWrapper *fw);
    static void addNominalToActual(QList<FeatureWrapper*>&features, FeatureWrapper *fw);
signals:
    
public slots:
    void recalcFeature(Feature *f);
    void recalcTrafoParam(TrafoParam *tp, QList<FeatureWrapper *> featureSet,
                          QList<CoordinateSystem*> coordinateSystems,
                          QList<Station*> stations, CoordinateSystem *activeCoordSystem);
    void recalcFeatureSet(QList<FeatureWrapper*> featureSet);
    void switchCoordinateSystem(QList<CoordinateSystem*> coordinateSystems,
                                QList<Station*> stations,
                                QList<FeatureWrapper *> featureSet, CoordinateSystem *to);
    TrafoParam* findTrafoParam(CoordinateSystem *searchSystem, QList<TrafoParam *> trafoParams);

    bool deleteFeature(FeatureWrapper *myFeatureWrapper, QList<FeatureWrapper *> featureSet);

private:
    void fillTrafoParamFunctionNN(SystemTransformation *function, TrafoParam *tp);
    void fillTrafoParamFunctionAN(SystemTransformation *function, TrafoParam *tp, QList<FeatureWrapper *> featureSet,
                                  QList<CoordinateSystem *> coordinateSystems, QList<Station *> stations,
                                  CoordinateSystem *activeCoordSystem);
    void fillTrafoParamFunctionAA(SystemTransformation *function, TrafoParam *tp, QList<FeatureWrapper *> featureSet,
                                  QList<CoordinateSystem *> coordinateSystems, QList<Station *> stations,
                                  CoordinateSystem *activeCoordSystem);
    void switchCoordinateSystemWithoutTransformation(QList<CoordinateSystem*> coordinateSystems,
                                QList<Station*> stations,
                                QList<FeatureWrapper *> featureSet, CoordinateSystem *to);

    static void createFeature(QList<Station*>&stations,QList<CoordinateSystem *> &coordSys, QList<FeatureWrapper *> &features,
                       MeasurementConfig mConfig, QString name, bool nominal, FeatureAttributesExchange fae);

    static Function* getFunctionFromComboValue(QString functionValue);

};

#endif // FEATUREUPDATER_H
