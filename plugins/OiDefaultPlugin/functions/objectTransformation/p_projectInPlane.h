#ifndef P_PROJECTINPLANE_H
#define P_PROJECTINPLANE_H

#include "pi_objecttransformation.h"
#include "point.h"
#include "plane.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

class ProjectInPlane : public ObjectTransformation
{
public:
    PluginMetaData* getMetaData() const;
    bool exec(Point&);
    QList<InputParams> getNeededElements() const;
    QList<Configuration::FeatureTypes> applicableFor() const;

private:
    void projectPoint(Point&, Plane*);

    Plane* getPlane();
};

#endif // P_PROJECTINPLANE_H
