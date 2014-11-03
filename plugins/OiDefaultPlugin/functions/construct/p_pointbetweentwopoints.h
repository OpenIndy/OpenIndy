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
    PluginMetaData* getMetaData() const;
    bool exec(Point&);
    QList<InputParams> getNeededElements() const;
    QList<Configuration::FeatureTypes> applicableFor() const;

private:
    Point* startPoint;
    Point* destPoint;
    bool getTwoPoints();
    bool calcMidPoint(Point&);
    bool variancePropagation(Point&);
};

#endif // P_POINTBETWEENTWOPOINTS_H
