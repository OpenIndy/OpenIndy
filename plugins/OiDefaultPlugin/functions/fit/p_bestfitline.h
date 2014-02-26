#ifndef P_BESTFTILINE_H
#define P_BESTFTILINE_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <qmath.h>
#include <vector>
#include "pi_fitfunction.h"
#include "console.h"
#include "line.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

using namespace std;

class BestFitLine : public FitFunction
{ 
public:
    PluginMetaData* getMetaData();
    bool exec(Line&);
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

private:
    bool checkObservationCount();
    bool setUpResult(Line&);
    OiMat preCalc(OiVec centroid);

    OiVec referenceDirection;

};

#endif // P_BESTFTILINE_H
