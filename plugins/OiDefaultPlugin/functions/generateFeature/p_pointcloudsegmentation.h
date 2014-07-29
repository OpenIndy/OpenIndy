#ifndef P_POINTCLOUDSEGMENTATION_H
#define P_POINTCLOUDSEGMENTATION_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <vector>
#include <QStringList>
#include "pi_generatefeaturefunction.h"
#include "point.h"
#include "plane.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

#include <QtMath>

using namespace std;

class PointCloudSegmentation : public GenerateFeatureFunction
{
public:
    PluginMetaData* getMetaData();

    bool exec(PointCloud&);

    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

    QStringList getResultProtocol();

private:

};

#endif // P_POINTCLOUDSEGMENTATION_H
