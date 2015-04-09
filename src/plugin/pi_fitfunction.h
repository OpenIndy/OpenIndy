#ifndef PI_FITFUNCTION_H
#define PI_FITFUNCTION_H

#include "function.h"
#include <QString>

/*!
 * \brief The FitFunction class
 * Interface for implementing fit function plugins.
 */
class FitFunction : public Function
{

public:
    virtual ~FitFunction(){}

    virtual QList<NeededElement> getNeededElements() const = 0;
    virtual QList<FeatureTypes> applicableFor() const = 0;
    virtual PluginMetaData getMetaData() const = 0;
};

#define FitFunction_iidd "de.openIndy.Plugin.Function.FitFunction.v001"


#endif // PI_FITFUNCTION_H
