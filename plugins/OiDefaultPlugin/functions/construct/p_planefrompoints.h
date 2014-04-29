#ifndef P_PLANEFROMPOINTS_H
#define P_PLANEFROMPOINTS_H

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

class PlaneFromPoints : public ConstructFunction
{
public:
    PluginMetaData* getMetaData();
    bool exec(Plane&);
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

private:
    bool checkPointCount();
    bool setUpResult(Plane&);
    OiMat preCalc();

    OiVec referenceDirection;

};

#endif // P_PLANEFROMPOINTS_H
