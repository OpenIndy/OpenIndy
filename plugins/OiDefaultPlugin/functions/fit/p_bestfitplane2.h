#ifndef P_BESTFITPLANE2_H
#define P_BESTFITPLANE2_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <qmath.h>
#include <QList>
#include "pi_fitfunction.h"
#include "plane.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "cfitting_plane.h"
#include "pluginmetadata.h"

class BestFitPlane2 : public FitFunction
{
public:
    PluginMetaData* getMetaData() const;
    bool exec(Plane&);
    QList<InputParams> getNeededElements() const;
    QList<Configuration::FeatureTypes> applicableFor() const;

private:
    void setUpResult(Plane &p, double *x, double *y, double *z, int count, double *n, double d, double *qxx);
    int getObservationCount();

    OiVec referenceDirection;

};

#endif // P_KERNFITPLANE_H
