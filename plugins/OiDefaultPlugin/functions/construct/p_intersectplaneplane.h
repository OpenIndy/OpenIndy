#ifndef P_INTERSECTPLANEPLANE_H
#define P_INTERSECTPLANEPLANE_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <qmath.h>
#include <vector>
#include "pi_constructfunction.h"
#include "point.h"
#include "plane.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

using namespace std;

class IntersectPlanePlane : public ConstructFunction
{  
public:
    PluginMetaData* getMetaData();
    bool exec(Line&);
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

private:
    bool setUpResult(Line&);
    bool checkElementCount();
    bool isIntersection(Plane*, Plane*);
    OiVec calcDirectionVector(OiVec, OiVec);
    OiVec calcPointOnLine(OiVec, OiVec, OiVec, OiVec);

};

#endif // P_INTERSECTPLANEPLANE_H
