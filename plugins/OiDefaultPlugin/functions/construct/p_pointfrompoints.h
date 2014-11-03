#ifndef P_POINTFROMPOINTS_H
#define P_POINTFROMPOINTS_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <vector>
#include "pi_constructfunction.h"
#include "point.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

using namespace std;

class PointFromPoints : public ConstructFunction
{
public:
    PluginMetaData* getMetaData() const;
    bool exec(Point&);
    QList<InputParams> getNeededElements() const;
    QList<Configuration::FeatureTypes> applicableFor() const;

private:
    void setUpPointResult(Point&);
    bool checkPointCount();

};

#endif // P_POINTFROMPOINTS_H
