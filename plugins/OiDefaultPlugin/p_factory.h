#ifndef P_FACTORY_H
#define P_FACTORY_H

#include <QtGlobal>
#include <QObject>
#include <QString>

#include "plugin.h"

#include "sensorconfiguration.h"

#include "sensor.h"
#include "function.h"

#include "networkadjustment.h"
#include "lasertracker.h"
#include "totalstation.h"
#include "constructfunction.h"
#include "fitfunction.h"
#include "geodeticfunction.h"
#include "objecttransformation.h"
#include "systemtransformation.h"

#include "p_bestfitpoint.h"
#include "p_bestfitline.h"
#include "p_bestfitplane.h"
#include "p_bestfitplane2.h"
#include "p_bestfitsphere.h"
#include "p_bestfitcircle.h"
#include "p_bestfitsphererauls.h"

#include "p_pointfrompoints.h"
#include "p_linefrompoints.h"
#include "p_planefrompoints.h"
#include "p_intersectlineplane.h"
#include "p_intersectplaneplane.h"
#include "p_spherefrompoints.h"

#include "p_changeradius.h"
#include "p_register.h"
#include "p_translatebyvalue.h"

#include "p_helmert7Param.h"
#include "p_simpletemperaturecompensation.h"
#include "p_extendedtemperaturecompensation.h"
#include "p_helmert6param.h"

#include "p_pseudotracker.h"
#include "p_leicatachymeter.h"

#include "simulationmodel.h"

#include "simplepolarmeasurement.h"

#include "p_oiexchangeascii.h"

class OiTemplatePlugin : public QObject, Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "de.openIndy.plugin.v001" FILE "metaInfo.json")
    Q_INTERFACES(Plugin)
    
public:
    OiTemplatePlugin(QObject *parent = 0);

    //##########################################################
    //create instances of all available plugin types in a plugin
    //##########################################################

    QList<QPointer<Sensor> > createSensors();
    QList<QPointer<Function> > createFunctions();
    QList<QPointer<NetworkAdjustment> > createNetworkAdjustments();
    QList<QPointer<SimulationModel> > createSimulations();
    QList<QPointer<Tool> > createTools();
    QList<QPointer<ExchangeSimpleAscii> > createSimpleAsciiExchanges();
    QList<QPointer<ExchangeDefinedFormat> > createDefinedFormatExchanges();

    //###################################################
    //create instances of a plugin type with a given name
    //###################################################

    QPointer<Sensor> createSensor(const QString &name);
    QPointer<Function> createFunction(const QString &name);
    QPointer<NetworkAdjustment> createNetworkAdjustment(const QString &name);
    QPointer<SimulationModel> createSimulation(const QString &name);
    QPointer<Tool> createTool(const QString &name);
    QPointer<ExchangeSimpleAscii> createSimpleAsciiExchange(const QString &name);
    QPointer<ExchangeDefinedFormat> createDefinedFormatExchange(const QString &name);

};

#endif // P_FACTORY_H
