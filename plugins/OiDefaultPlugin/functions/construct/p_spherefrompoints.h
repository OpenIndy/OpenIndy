#ifndef P_SPHEREFROMPOINTS_H
#define P_SPHEREFROMPOINTS_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include "pi_constructfunction.h"
#include "point.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "cfitting_sphere.h"
#include "pluginmetadata.h"

class SphereFromPoints : public ConstructFunction
{ 
public:
    PluginMetaData* getMetaData();
    bool exec(Sphere&);
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

private:
    void setUpResult(Sphere &s, double *x, double *y, double *z, int count, double *xm, double r, double *qxx);
    int getPointCount();

};

#endif // P_SPHEREFROMPOINTS_H
