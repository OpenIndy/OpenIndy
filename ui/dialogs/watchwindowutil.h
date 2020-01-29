#ifndef WATCHWINDOWUTIL_H
#define WATCHWINDOWUTIL_H

#include "featurewrapper.h"
#include "feature.h"
#include "oivec.h"
#include "position.h"
#include "radius.h"
#include "geometry.h"
namespace oi {
class WatchWindowUtil
{

public:
    WatchWindowUtil();

    QPair<Position, Radius> getPosition(QPointer<FeatureWrapper> feature, OiVec trackerXYZ);
};
}
#endif // WATCHWINDOWUTIL_H
