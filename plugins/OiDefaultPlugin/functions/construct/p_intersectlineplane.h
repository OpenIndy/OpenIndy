#ifndef P_INTERSECTLINEPLANE_H
#define P_INTERSECTLINEPLANE_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <qmath.h>
#include <vector>
#include "pi_constructfunction.h"
#include "point.h"
#include "line.h"
#include "plane.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

using namespace std;

class IntersectLinePlane : public ConstructFunction
{  
public:
    PluginMetaData* getMetaData();
    bool exec(Point&);
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

private:
    bool setUpResult(Point&);
    bool checkElementCount();
    bool isIntersection(Line*, Plane*);

};

#endif // P_INTERSECTLINEPLANE_H
