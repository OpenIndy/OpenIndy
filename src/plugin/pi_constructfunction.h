#ifndef PI_CONSTRUCTFUNCTION_H
#define PI_CONSTRUCTFUNCTION_H

#include "function.h"
#include <QString>

/*!
 * \brief The ConstructFunction class
 * Interface for implementing construct function plugins.
 */
class ConstructFunction : public Function
{

public:
    virtual ~ConstructFunction(){}

    virtual QList<InputParams> getNeededElements() = 0;
    virtual QList<Configuration::FeatureTypes> applicableFor() = 0;
    virtual PluginMetaData* getMetaData() = 0;
};

#define ConstructFunction_iidd "de.openIndy.Plugin.Function.ConstructFunction.v001"


#endif // PI_CONSTRUCTFUNCTION_H

/*
 * bei construct coordinate system muss im plugin die nominals liste für das gebaute coord sys gefüllt werden
 */
