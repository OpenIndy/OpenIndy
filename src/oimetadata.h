#ifndef OIMETADATA_H
#define OIMETADATA_H

#include <QString>
#include <QJsonArray>
#include "oiplugin.h"
#include "sensor.h"
#include "lasertracker.h"
#include "totalstation.h"
#include "constructfunction.h"
#include "fitfunction.h"
#include "generatefeaturefunction.h"
#include "geodeticfunction.h"
#include "networkadjustment.h"
#include "objecttransformation.h"
#include "systemtransformation.h"
#include "oiexchangeinterface.h"
#include "oiexchangesimpleascii.h"
#include "oiexchangedefinedformat.h"

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
    static QString iid_GenerateFeatureFunction;
    static QString iid_GeodeticFunction;
    static QString iid_NetworkAdjustment;
    static QString iid_ObjectTransformation;
    static QString iid_SystemTransformation;
    static QString iid_Sensor;
    static QString iid_Simulation;
    static QString iid_OiTool;
    static QString iid_OiExchange;
    static QString iid_OiExchangeSimpleAscii;
    static QString iid_OiExchangeDefinedFormat;

    static QString getVersion();
    static QString getCompiler();
    static QString getOperatingSys();
    static bool findIID(QString iid);
    //static int generateId();


};

#endif // OIMETADATA_H
