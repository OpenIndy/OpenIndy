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

    PluginMetaData* getMetaData();
    bool exec(Circle &circle);
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

private:
    int getObservationCount();
};

#endif // P_BESTFITCIRCLE_H
