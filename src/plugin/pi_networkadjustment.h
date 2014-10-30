#ifndef PI_NETWORKADJUSTMENT_H
#define PI_NETWORKADJUSTMENT_H

#include <QString>

class CoordinateSystem;
class Statistic;

/*!
 * \brief The NetworkAdjustment class
 * Interface for implementing network adjustment plugins.
 */
class NetworkAdjustment
{

public:
    virtual ~NetworkAdjustment(){}

    QList<CoordinateSystem*> coordSys;
    Statistic *stats;

    virtual PluginMetaData* getMetaData() const = 0;

    virtual void recalc() = 0;
};
#define NetworkAdjustment_iidd "de.openIndy.Plugin.NetworkAdjustment.v001"


#endif // PI_NETWORKADJUSTMENT_H
