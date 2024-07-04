#include "featureutil.h"


// adapted from FeatureUpdater::createBundleTransformations
// create intial bundle transformation (Bundle01_1) to STATION01
bool FeatureUtil::createInitialBundleTransformation(QPointer<OiJob> job) {

    FeatureAttributes trafoAttr;
    trafoAttr.typeOfFeature = eTrafoParamFeature;
    trafoAttr.group = "";
    trafoAttr.count = 1;
    trafoAttr.startSystem = "Bundle01";
    trafoAttr.name = QString("%1_1").arg(trafoAttr.startSystem);
    trafoAttr.destinationSystem = "STATION01";

    QList<QPointer<FeatureWrapper> > features = job->addFeatures(trafoAttr);
    if(features.isEmpty()) {
        return false;
    }
    QPointer<FeatureWrapper> trafoParam = features.first();

    //set up transformation parameters
    if(trafoParam.isNull() || trafoParam->getTrafoParam().isNull()) {
        return false;
    }

    QPointer<TrafoParam> trafo = trafoParam->getTrafoParam();

    //set up parameters
    QMap<TrafoParamParameters, double> parameters;
    parameters.insert(eUnknownTX, 0.0);
    parameters.insert(eUnknownTY, 0.0);
    parameters.insert(eUnknownTZ, 0.0);
    parameters.insert(eUnknownRX, 0.0);
    parameters.insert(eUnknownRY, 0.0);
    parameters.insert(eUnknownRZ, 0.0);
    parameters.insert(eUnknownSX, 1.0);
    parameters.insert(eUnknownSY, 1.0);
    parameters.insert(eUnknownSZ, 1.0);
    trafo->setUnknownParameters(parameters);
    trafo->setIsUsed(true);
    trafo->setIsDatumTrafo(true);

    return true;
}
