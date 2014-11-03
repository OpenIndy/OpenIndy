#ifndef P_BESTFITCIRCLE_H
#define P_BESTFITCIRCLE_H

#include "pi_fitfunction.h"
#include "oivec.h"
#include "oimat.h"
#include "cfitting_plane.h"
#include "cfitting_sphere.h"

class BestFitCircle : public FitFunction
{
public:
    BestFitCircle();

    PluginMetaData* getMetaData() const;
    bool exec(Circle &circle);
    QList<InputParams> getNeededElements() const;
    QList<Configuration::FeatureTypes> applicableFor() const;

private:
    int getObservationCount();
    OiVec getApproximatedPlane(int nop, double x[], double y[], double z[], double cx, double cy, double cz);
};

#endif // P_BESTFITCIRCLE_H
