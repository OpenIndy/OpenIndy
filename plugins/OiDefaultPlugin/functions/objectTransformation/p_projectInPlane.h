#ifndef P_PROJECTINPLANE_H
#define P_PROJECTINPLANE_H

#include "pi_objecttransformation.h"
#include "console.h"
#include "point.h"
#include "plane.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

class ProjectInPlane : public ObjectTransformation
{
public:
    PluginMetaData* getMetaData();
    bool exec(Point&);
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

private:
    void projectPoint(Point&, Plane*);

    Plane* getPlane();
};

#endif // P_PROJECTINPLANE_H
