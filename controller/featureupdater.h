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

class FeatureUpdater : public QObject
{
    Q_OBJECT
public:
    explicit FeatureUpdater(QObject *parent = 0);
    
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

    bool deleteFeature(FeatureWrapper *myFeature, QList<FeatureWrapper *> featureSet);

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

};

#endif // FEATUREUPDATER_H
