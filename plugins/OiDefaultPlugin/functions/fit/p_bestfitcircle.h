#ifndef P_BESTFITCIRCLE_H
#define P_BESTFITCIRCLE_H

#include "pi_fitfunction.h"

class BestFitCircle : public FitFunction
{
public:
    BestFitCircle();

    PluginMetaData* getMetaData();
    bool exec(Circle&);
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

private:

};

#endif // P_BESTFITCIRCLE_H
