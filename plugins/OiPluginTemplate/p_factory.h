#ifndef P_FACTORY_H
#define P_FACTORY_H

#include <QtGlobal>
#include <QObject>

#include "pi_oiplugin.h"

#include "functionconfiguration.h"
#include "sensorconfiguration.h"

#include "sensor.h"
#include "function.h"

#include "pi_networkadjustment.h"
#include "pi_lasertracker.h"
#include "pi_totalstation.h"
#include "pi_constructfunction.h"
#include "pi_fitfunction.h"
#include "pi_geodeticfunction.h"
#include "pi_objecttransformation.h"
#include "pi_systemtransformation.h"

class OiTemplatePlugin : public QObject, OiPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "de.openIndy.Plugin.v001" FILE "metaInfo.json")
    Q_INTERFACES(OiPlugin)
    
public:
    OiTemplatePlugin(QObject *parent = 0);

    QList<Sensor*> createSensors();
    QList<Function*> createFunctions();
    QList<NetworkAdjustment*> createNetworkAdjustments();
    Sensor* createSensor(QString name);
    Function* createFunction(QString name);
    NetworkAdjustment* createNetworkAdjustment(QString name);

};

#endif // P_FACTORY_H
