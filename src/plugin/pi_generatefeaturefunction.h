#ifndef PI_GENERATEFEATUREFUNCTION_H
#define PI_GENERATEFEATUREFUNCTION_H

#include <QString>

#include "function.h"
//#include "oifunctionemitter.h"

/*!
 * \brief The FitFunction class
 * Interface for implementing generate-feature-function plugins.
 */
class GenerateFeatureFunction : public Function
{

public:
    virtual ~GenerateFeatureFunction(){}

    virtual QList<InputParams> getNeededElements() const = 0;
    virtual QList<Configuration::FeatureTypes> applicableFor() const = 0;
    virtual PluginMetaData* getMetaData() const = 0;

/*protected:
    void addFeature(FeatureWrapper *myFeature){ //add a feature to OpenIndy
        this->getOiEmitter().addFeature(myFeature);
    }*/

};

#define GenerateFeatureFunction_iidd "de.openIndy.Plugin.Function.GenerateFeatureFunction.v001"

#endif // PI_GENERATEFEATUREFUNCTION_H
