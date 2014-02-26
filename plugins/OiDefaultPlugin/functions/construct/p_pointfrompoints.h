#ifndef P_POINTFROMPOINTS_H
#define P_POINTFROMPOINTS_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <vector>
#include "pi_constructfunction.h"
#include "console.h"
#include "point.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

using namespace std;

class PointFromPoints : public ConstructFunction
{
public:
    PluginMetaData* getMetaData();
    bool exec(Point&);
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

private:
    void setUpPointResult(Point&);
    bool checkPointCount();

};

#endif // P_POINTFROMPOINTS_H
