#ifndef P_BESTFITPLANE_H
#define P_BESTFITPLANE_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <qmath.h>
#include <vector>
#include "pi_fitfunction.h"
#include "point.h"
#include "plane.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

using namespace std;

class BestFitPlane : public FitFunction
{  
public:
    PluginMetaData* getMetaData();
    bool exec(Plane&);
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

private:
    bool checkObservationCount();
    bool setUpResult(Plane&);
    OiMat preCalc();

    OiVec referenceDirection;

};

#endif // P_BESTFITPLANE_H
