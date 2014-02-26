#ifndef P_BESTFITSPHERE_H
#define P_BESTFITSPHERE_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include "pi_fitfunction.h"
#include "console.h"
#include "point.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "cfitting_sphere.h"
#include "pluginmetadata.h"

class BestFitSphere : public FitFunction
{
public:
    PluginMetaData* getMetaData();
    bool exec(Sphere&);
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

private:
    void setUpResult(Sphere &s, double *x, double *y, double *z, int count, double *xm, double r, double *qxx);
    int getObservationCount();

};

#endif // P_BESTFITSPHERE_H
