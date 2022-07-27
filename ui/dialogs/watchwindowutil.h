#ifndef WATCHWINDOWUTIL_H
#define WATCHWINDOWUTIL_H

#include "featurewrapper.h"
#include "feature.h"
#include "oivec.h"
#include "position.h"
#include "radius.h"
#include "geometry.h"


#if defined(OI_MAIN_LIB)
#  define OI_MAIN_EXPORT Q_DECL_EXPORT
#else
#  define OI_MAIN_EXPORT Q_DECL_IMPORT
#endif


namespace oi {

struct Result {
  Position position;
  Radius radius;
  OiVec delta;
  double d3D;
};

class OI_MAIN_EXPORT WatchWindowUtil
{

public:
    WatchWindowUtil();

    Result getPosition(QPointer<FeatureWrapper> feature, OiVec trackerXYZ);
};
}
#endif // WATCHWINDOWUTIL_H
