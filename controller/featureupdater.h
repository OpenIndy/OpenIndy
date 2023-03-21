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
#include "sortlistbyname.h"

using namespace oi;

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

public slots:

    //#####################
    //recalculation methods
    //#####################

    //recalculate features
    void recalcFeatureSet();
    void recalcFeature(const QPointer<Feature> &feature);
    void recalcTrafoParam(const QPointer<TrafoParam> &trafoParam);
    bool recalcBundle(const QPointer<CoordinateSystem> &bundleSystem);

    //change the active coordinate system (transforms all observations to the given coordinate system, if possible)
    void switchCoordinateSystem();

private:

    //##############
    //helper methods
    //##############

    void connectJob();
    void disconnectJob();

    //recursive recalculation
    void recursiveFeatureRecalculation(const QPointer<Feature> &feature);

    //set up input elements for trafo param features
    void setUpTrafoParamActualActual(const QPointer<TrafoParam> &trafoParam, const QPointer<SystemTransformation> &systemTransformation);
    void setUpTrafoParamActualNominal(const QPointer<TrafoParam> &trafoParam, const QPointer<SystemTransformation> &systemTransformation);
    void setUpTrafoParamNominalNominal(const QPointer<TrafoParam> &trafoParam, const QPointer<SystemTransformation> &systemTransformation);
    void setUpTrafoParamBundleNominal(const QPointer<TrafoParam> &trafoParam, const QPointer<SystemTransformation> &systemTransformation);
    void setUpInputElementsForAlignmentTransformations(const QPointer<SystemTransformation> &systemTransformation, const bool &startSystem);
    void setUpInputElementsForNormalTransformations(const QPointer<TrafoParam> &trafoParam, const QPointer<SystemTransformation> &systemTransformation, const bool &startSystem, const bool &bestFitWithLevel);

    //change the active coordinate system (does not transform observations)
    void switchCoordinateSystemWithoutTransformation(const QPointer<CoordinateSystem> &destinationSystem);
    void transformAllObsToDestSystem(const QPointer<CoordinateSystem> &destinationSystem);
    void recalcFeatureSetWithoutTransformation();
    void recalcFeatureWithoutTransformation(const QPointer<Feature> &feature);
    //change with transformation for bundle
    void switchCoordinateSystemWithTransformation(const QPointer<CoordinateSystem> &destinationSystem);

    void transformObsAndNominals(const QPointer<CoordinateSystem> &destinationSystem);

    //copy geometry
    void copyGeometry(InputElement &newElement, const QPointer<FeatureWrapper> &oldElement, const ElementTypes &type);

    //clear or save bundle results
    void clearBundleResults(const QPointer<CoordinateSystem> &bundleSystem);
    void saveBundleResults(const QPointer<CoordinateSystem> &bundleSystem);
    //void createBundleGeometries(const QList<BundleGeometry> &geometries, const QPointer<CoordinateSystem> &bundleSystem);
    void createBundleTransformations(QList<BundleTransformation> &transformations, const QPointer<CoordinateSystem> &bundleSystem);
    //recalc bundle- trafoParam dependencies
    void recalcAllBundleTrafoDependencies(const QPointer<CoordinateSystem> &bundleSystem);

    //##################
    //general attributes
    //##################

    QPointer<OiJob> currentJob;

    TrafoController trafoController; //handles all transformations to transform Observations in the right coordinate system

};

#endif // FEATUREUPDATER_H
