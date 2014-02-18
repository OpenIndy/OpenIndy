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

    virtual QList<InputParams> getNeededElements() = 0;
    virtual QList<Configuration::FeatureTypes> applicableFor() = 0;
    virtual PluginMetaData* getMetaData() = 0;
};

#define FitFunction_iidd "de.openIndy.Plugin.Function.FitFunction.v001"


#endif // PI_FITFUNCTION_H
