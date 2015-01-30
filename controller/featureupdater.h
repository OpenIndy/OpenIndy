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

private:
    explicit FeatureUpdater(QObject *parent = 0);

public:
    static FeatureUpdater *getInstance();

    static TrafoController trafoControl;

signals:
    void featuresRecalculated();
    void featureRecalculated(int featureId);
    void trafoParamRecalculated(int featureId);
    
public slots:
    void recalcAll();
    void recalcFeature(Feature *f);
    void recalcTrafoParam(TrafoParam *tp);
    void recalcFeatureSet();
    void switchCoordinateSystem(CoordinateSystem *to);
    TrafoParam* findTrafoParam(CoordinateSystem *searchSystem, QList<TrafoParam *> trafoParams);

private:
    void fillTrafoParamFunctionNN(SystemTransformation *function, TrafoParam *tp);
    void fillTrafoParamFunctionAN(SystemTransformation *function, TrafoParam *tp);
    void fillTrafoParamFunctionAA(SystemTransformation *function, TrafoParam *tp);
    void fillTrafoParamFunctionMovement(SystemTransformation *function, TrafoParam *tp);

    void switchCoordinateSystemWithoutTransformation(CoordinateSystem *to);
    void switchCoordinateSystemWithoutMovement(CoordinateSystem *to);

    static FeatureUpdater *myFeatureUpdater;

};

#endif // FEATUREUPDATER_H
