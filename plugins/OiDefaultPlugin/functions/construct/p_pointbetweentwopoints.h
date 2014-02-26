#ifndef P_POINTBETWEENTWOPOINTS_H
#define P_POINTBETWEENTWOPOINTS_H

#include <QObject>
#include "pi_constructfunction.h"
#include "point.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

class PointBetweenTwoPoints : public ConstructFunction
{
public:
    PluginMetaData* getMetaData();
    bool exec(Point&);
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

private:
    Point* startPoint;
    Point* destPoint;
    bool getTwoPoints();
    bool calcMidPoint(Point&);
    bool variancePropagation(Point&);
};

#endif // P_POINTBETWEENTWOPOINTS_H
