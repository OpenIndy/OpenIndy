#ifndef WATCHWINDOWUTIL_H
#define WATCHWINDOWUTIL_H

#include "featurewrapper.h"
#include "feature.h"
#include "oivec.h"
#include "position.h"
#include "radius.h"
#include "geometry.h"
namespace oi {

struct Result {
  Position position;
  Radius radius;
  OiVec delta;
  double d3D;
};

class WatchWindowUtil
{

public:
    WatchWindowUtil();

    Result getPosition(QPointer<FeatureWrapper> feature, OiVec trackerXYZ);
};
}
#endif // WATCHWINDOWUTIL_H
