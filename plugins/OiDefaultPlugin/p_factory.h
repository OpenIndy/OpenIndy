#ifndef P_FACTORY_H
#define P_FACTORY_H

#include <QtGlobal>
#include <QObject>
#include <QString>

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

#include "p_bestfitpoint.h"
#include "p_bestfitline.h"
#include "p_bestfitplane.h"
#include "p_bestfitplane2.h"
#include "p_bestfitsphere.h"
#include "p_bestfitcircle.h"

#include "p_pointfrompoints.h"
#include "p_linefrompoints.h"
#include "p_planefrompoints.h"
#include "p_intersectlineplane.h"
#include "p_intersectplaneplane.h"
#include "p_spherefrompoints.h"

#include "p_changeradius.h"
#include "p_translateByLine.h"
#include "p_translateByPlane.h"
#include "p_projectInPlane.h"
#include "p_shiftplane.h"

#include "p_helmert7Param.h"
#include "p_simpletemperaturecompensation.h"
#include "p_extendedtemperaturecompensation.h"
#include "p_helmert6param.h"

#include "p_pseudotracker.h"
#include "p_leicatachymeter.h"
#include "p_pseudosensor.h"

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
