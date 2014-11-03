#ifndef P_BESTFITPOINT_H
#define P_BESTFITPOINT_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <vector>
#include <QStringList>
#include "pi_fitfunction.h"
#include "point.h"
#include "plane.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

#include <QtMath>

using namespace std;

class BestFitPoint : public FitFunction
{
public:
    PluginMetaData* getMetaData() const;
    bool exec(Point&);
    QList<InputParams> getNeededElements() const;
    QList<Configuration::FeatureTypes> applicableFor() const;
    QStringList getResultProtocol();

private:
    void setUpPointResult(Point&);
    bool checkObservationCount();

};

#endif // P_BESTFITPOINT_H
