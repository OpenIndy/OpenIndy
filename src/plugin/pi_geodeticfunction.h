#ifndef PI_GEODETICFUNCTION_H
#define PI_GEODETICFUNCTION_H

#include "function.h"
#include <QString>

/*!
 * \brief The GeodeticFunction class
 * Interface for implementing geodetic function plugins.
 */
class GeodeticFunction : public Function
{

public:
    virtual ~GeodeticFunction(){}

    virtual QList<InputParams> getNeededFeatures() = 0;
    virtual QList<Configuration::FeatureTypes> applicableFor() = 0;
    virtual PluginMetaData* getMetaData() = 0;
};

#define GeodeticFunction_iidd "de.openIndy.Plugin.Function.GeodeticFunction.v001"


#endif // PI_GEODETICFUNCTION_H
