#ifndef OIMETADATA_H
#define OIMETADATA_H

#include <QString>
#include <QJsonArray>
#include "configuration.h"
#include "pi_oiplugin.h"
#include "sensor.h"
#include "pi_lasertracker.h"
#include "pi_totalstation.h"
#include "pi_constructfunction.h"
#include "pi_fitfunction.h"
#include "pi_geodeticfunction.h"
#include "pi_networkadjustment.h"
#include "pi_objecttransformation.h"
#include "pi_systemtransformation.h"




/*!
 * \brief The OiMetaData class
 */
class OiMetaData
{
private:

    static QString version;

 public:

    static QString sensorPlugin;
    static QString functionPlugin;

    static QString iid_OiPlugin;
    static QString iid_LaserTracker;
    static QString iid_TotalStation;
    static QString iid_ConstructFunction;
    static QString iid_FitFunction;
    static QString iid_GeodeticFunction;
    static QString iid_NetworkAdjustment;
    static QString iid_ObjectTransformation;
    static QString iid_SystemTransformation;
    static QString iid_Sensor;


    static QString getVersion();
    static QString getCompiler();
    static QString getOperatingSys();
    static bool findIID(QString iid);
    static QString findFeature(Configuration::FeatureTypes f);
    static QString findElement(Configuration::ElementTypes e);
    static int generateId();

};

#endif // OIMETADATA_H
