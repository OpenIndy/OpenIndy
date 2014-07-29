#ifndef P_POINTCLOUDSEGMENTATION_H
#define P_POINTCLOUDSEGMENTATION_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QString>
#include <QMap>
#include <QStringList>

#include "pi_generatefeaturefunction.h"

#include "plane.h"
#include "sphere.h"
#include "cylinder.h"

#include "pluginmetadata.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"

class PointCloudSegmentation : public GenerateFeatureFunction
{
public:
    PluginMetaData* getMetaData();

    bool exec(PointCloud&);

    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

    QMap<QString, int> getIntegerParameter();
    QMap<QString, double> getDoubleParameter();
    QMap<QString, QStringList> getStringParameter();

    QStringList getResultProtocol();

private:

};

#endif // P_POINTCLOUDSEGMENTATION_H
