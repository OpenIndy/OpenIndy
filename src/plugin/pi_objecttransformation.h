#ifndef PI_OBJECTTRANSFORMATION_H
#define PI_OBJECTTRANSFORMATION_H

#include "function.h"
#include <QString>
#include "oivec.h"

/*!
 * \brief The ObjectTransformation class
 * Interface for implementing object transformation function plugins.
 */
class ObjectTransformation : public Function
{

public:
    virtual ~ObjectTransformation(){}

    OiVec translate;
    double rotate;
    double offset;

    virtual QList<NeededElement> getNeededElements() const = 0;
    virtual QList<FeatureTypes> applicableFor() const = 0;

    virtual PluginMetaData getMetaData() const = 0;

};

#define ObjectTransformation_iidd "de.openIndy.Plugin.Function.ObjectTransformation.v001"

#endif // PI_OBJECTTRANSFORMATION_H
