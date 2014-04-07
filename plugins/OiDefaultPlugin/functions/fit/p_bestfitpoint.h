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
#include "console.h"
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
    PluginMetaData* getMetaData();
    bool exec(Point&);
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();
    QStringList getResultProtocol();

private:
    void setUpPointResult(Point&);
    bool checkObservationCount();

};

#endif // P_BESTFITPOINT_H
