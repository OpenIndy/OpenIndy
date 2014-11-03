#ifndef P_BESTFITSPHERERAULS_H
#define P_BESTFITSPHERERAULS_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include "pi_fitfunction.h"
#include "point.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

class BestFitSphereRauls : public FitFunction
{
public:
    PluginMetaData *getMetaData() const;
    bool exec(Sphere&);
    QList<InputParams> getNeededElements() const;
    QList<Configuration::FeatureTypes> applicableFor() const;

private:
    QList<Observation *> myValidObservations;

    int setUpObservations();
    bool approximate(Sphere&);
    bool fit(Sphere&);
};

#endif // P_BESTFITSPHERERAULS_H
