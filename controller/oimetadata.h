#ifndef OIMETADATA_H
#define OIMETADATA_H

#include <QString>
#include <QJsonArray>

#include "types.h"
#include "plugin.h"
#include "sensor.h"
#include "lasertracker.h"
#include "totalstation.h"
#include "constructfunction.h"
#include "specialfunction.h"
#include "fitfunction.h"
#include "generatefeaturefunction.h"
#include "geodeticfunction.h"
#include "bundleadjustment.h"
#include "objecttransformation.h"
#include "systemtransformation.h"
#include "exchangeinterface.h"
#include "exchangesimpleascii.h"
#include "exchangedefinedformat.h"

/*!
 * \brief The OiMetaData class
 * Holds version information about OpenIndy
 */
class OiMetaData
{

public:

    //#########################
    //plugin interface versions
    //#########################

    static QString iid_Plugin;
    static QString iid_LaserTracker;
    static QString iid_TotalStation;
    static QString iid_ConstructFunction;
    static QString iid_FitFunction;
    static QString iid_SpecialFunction;
    static QString iid_GeodeticFunction;
    static QString iid_BundleAdjustment;
    static QString iid_ObjectTransformation;
    static QString iid_SystemTransformation;
    static QString iid_Sensor;
    static QString iid_Simulation;
    static QString iid_Tool;
    static QString iid_Exchange;
    static QString iid_ExchangeSimpleAscii;
    static QString iid_ExchangeDefinedFormat;

    static bool findIID(const QString &iid);

    static SensorTypes getSensorTypeEnum(const QString &iid);

    //###############################
    //get plugin interface type names
    //###############################

    static QStringList getSensorTypeNames();
    static QString getSensorTypeName(const QString &iid);

    static QStringList getFunctionTypeNames();
    static QString getFunctionTypeName(const QString &iid);

    static QStringList getExchangeTypeNames();
    static QString getExchangeTypeName(const QString &iid);

    //#################################
    //version of the main program (src)
    //#################################

    static QString version;

    //#################################
    //meta data about this installation
    //#################################

    static QString getCompiler();
    static QString getOperatingSys();

};

#endif // OIMETADATA_H
