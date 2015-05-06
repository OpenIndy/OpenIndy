#ifndef FEATUREUPDATER_H
#define FEATUREUPDATER_H

#include <QObject>
#include <QPointer>

#include "featurewrapper.h"
#include "feature.h"
#include "coordinatesystem.h"
#include "trafoparam.h"
#include "function.h"
#include "featureattributes.h"
#include "console.h"
#include "systemdbmanager.h"
#include "pluginloader.h"
#include "trafocontroller.h"
#include "oijob.h"

/*!
 * \brief The FeatureUpdater class
 * Handles all recalculation tasks of features of an OpenIndy job
 */
class FeatureUpdater : public QObject
{
    Q_OBJECT

public:
    explicit FeatureUpdater(QObject *parent = 0);

signals:

    //#####################################################
    //inform when a recalculation process has been finished
    //#####################################################

    void featuresRecalculated();
    void featureRecalculated(const int &featureId);
    void trafoParamRecalculated(const int &featureId);
    
public:

    //###################################
    //get or set the current OpenIndy job
    //###################################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    //################
    //add measurements
    //################

    void addMeasurementResults(const int &geomId, const QList<QPointer<Reading> > &readings);

    //#####################
    //recalculation methods
    //#####################

    //recalculate features
    void recalcAll();
    void recalcFeatureSet();
    void recalcFeature(const QPointer<Feature> &feature);
    void recalcTrafoParam(const QPointer<TrafoParam> &trafoParam);

    //change the active coordinate system
    void switchCoordinateSystem(const QPointer<CoordinateSystem> &to);

private:

    //##############
    //helper methods
    //##############

    void recursiveFeatureRecalculation(const QPointer<Feature> &feature);

    //##################
    //general attributes
    //##################

    QPointer<OiJob> currentJob;

/*
    TrafoParam* findTrafoParam(CoordinateSystem *searchSystem, QList<TrafoParam *> trafoParams);

private:
    void fillTrafoParamFunctionNN(SystemTransformation *function, TrafoParam *tp);
    void fillTrafoParamFunctionAN(SystemTransformation *function, TrafoParam *tp);
    void fillTrafoParamFunctionAA(SystemTransformation *function, TrafoParam *tp);
    void fillTrafoParamFunctionMovement(SystemTransformation *function, TrafoParam *tp);

    void switchCoordinateSystemWithoutTransformation(CoordinateSystem *to);
    void switchCoordinateSystemWithoutMovement(CoordinateSystem *to);

    static FeatureUpdater *myFeatureUpdater;*/

};

#endif // FEATUREUPDATER_H
