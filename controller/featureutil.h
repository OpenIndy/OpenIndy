#ifndef FEATUREUTIL_H
#define FEATUREUTIL_H

#include <QString>
#include <QPointer>
#include <QList>

#include "oijob.h"

using namespace oi;

class FeatureUtil {

public:

    // adapted from FeatureUpdater::createBundleTransformations
    // create intial bundle transformation (Bundle01_1) to STATION01
    bool createInitialBundleTransformation(QPointer<OiJob> job);
};

#endif // FEATUREUTIL_H
