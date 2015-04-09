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

    virtual QList<NeededElement> getNeededElements() const = 0;
    virtual QList<FeatureTypes> applicableFor() const = 0;
    virtual PluginMetaData getMetaData() const = 0;
};

#define ConstructFunction_iidd "de.openIndy.Plugin.Function.ConstructFunction.v001"


#endif // PI_CONSTRUCTFUNCTION_H

/*
 * bei construct coordinate system muss im plugin die nominals liste für das gebaute coord sys gefüllt werden
 */
