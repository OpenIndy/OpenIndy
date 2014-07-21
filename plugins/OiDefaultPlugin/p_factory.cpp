#include "p_factory.h"

int    gDebugLevel=0;
TReal  gConvergence=1e-9;

/*!
 * \brief OiTemplatePlugin::OiTemplatePlugin
 * \param parent
 */
OiTemplatePlugin::OiTemplatePlugin(QObject *parent):
    QObject(parent){

}

/*!
 * \brief OiTemplatePlugin::createSensors
 * Returns a list with all sensors
 * \return
 */
QList<Sensor*> OiTemplatePlugin::createSensors(){
    QList<Sensor*> resultSet;
    Sensor *pTracker = new PseudoTracker();
    Sensor *LeicaTachy = new LeicaTachymeter();
    Sensor *pSensor = new PseudoSensor();
    resultSet.append(pTracker);
    resultSet.append(LeicaTachy);
    resultSet.append(pSensor);
    return resultSet;
}

/*!
 * \brief OiTemplatePlugin::createFunctions
 * Returns a list with all functions
 * \return
 */
QList<Function*> OiTemplatePlugin::createFunctions(){
    QList<Function*> resultSet;
    resultSet.append(new BestFitPoint());
    resultSet.append(new BestFitLine());
    resultSet.append(new BestFitPlane());
    resultSet.append(new BestFitPlane2());
    resultSet.append(new BestFitSphere());
    resultSet.append(new PointFromPoints());
    resultSet.append(new LineFromPoints());
    resultSet.append(new PlaneFromPoints());
    resultSet.append(new IntersectLinePlane());
    resultSet.append(new IntersectPlanePlane());
    resultSet.append(new SphereFromPoints());
    resultSet.append(new ChangeRadius());
    resultSet.append(new TranslateByLine());
    resultSet.append(new TranslateByPlane());
    resultSet.append(new ProjectInPlane());
    resultSet.append(new Helmert7Param());
    resultSet.append(new ShiftPlane());
    resultSet.append(new BestFitCircle());
    resultSet.append(new SimpleTemperatureCompensation());
    resultSet.append(new ExtendedTemperatureCompensation());
    resultSet.append(new Helmert6Param());
    resultSet.append(new BestFitSphereRauls());

    return resultSet;
}

/*!
 * \brief OiTemplatePlugin::createNetworkAdjustments
 * Returns a list with all network adjustments
 * \return
 */
QList<NetworkAdjustment*> OiTemplatePlugin::createNetworkAdjustments(){
    QList<NetworkAdjustment*> resultSet;
    return resultSet;
}

/*!
 * \brief OiTemplatePlugin::createSimulations
 * Returns a list with all simulations
 * \return
 */
QList<SimulationModel *> OiTemplatePlugin::createSimulations(){
    QList<SimulationModel*> resultSet;
    resultSet.append(new SimplePolarMeasurement());
    return resultSet;
}

/*!
 * \brief OiTemplatePlugin::createSensor
 * Returns the sensor with the specified name
 * \param name
 * \return
 */
Sensor* OiTemplatePlugin::createSensor(QString name){
    Sensor *result = NULL;
    if(name.compare("PseudoTracker") == 0){
        result = new PseudoTracker();
    }else if(name.compare("LeicaTachymeter") == 0){
        result = new LeicaTachymeter();
    }else if(name.compare("PseudoSensor") == 0){
        result = new PseudoSensor();
    }
    return result;
}

/*!
 * \brief OiTemplatePlugin::createFunction
 * Returns the function with the specified name
 * \param name
 * \return
 */
Function* OiTemplatePlugin::createFunction(QString name){
    Function *result = NULL;
    if(name.compare("BestFitPoint") == 0){
        result = new BestFitPoint();
    }else if(name.compare("BestFitLine") == 0){
        result = new BestFitLine();
    }else if(name.compare("BestFitPlane") == 0){
        result = new BestFitPlane();
    }else if(name.compare("BestFitPlane2") == 0){
        result = new BestFitPlane2();
    }else if(name.compare("BestFitSphere") == 0){
        result = new BestFitSphere();
    }else if(name.compare("PointFromPoints") == 0){
        result = new PointFromPoints();
    }else if(name.compare("LineFromPoints") == 0){
        result = new LineFromPoints();
    }else if(name.compare("PlaneFromPoints") == 0){
        result = new PlaneFromPoints();
    }else if(name.compare("IntersectLinePlane") == 0){
        result = new IntersectLinePlane();
    }else if(name.compare("IntersectPlanePlane") == 0){
        result = new IntersectPlanePlane();
    }else if(name.compare("SphereFromPoints") == 0){
        result = new SphereFromPoints();
    }else if(name.compare("ChangeRadius") == 0){
        result = new ChangeRadius();
    }else if(name.compare("TranslateByLine") == 0){
        result = new TranslateByLine();
    }else if(name.compare("TranslateByPlane") == 0){
        result = new TranslateByPlane();
    }else if(name.compare("ProjectInPlane") == 0){
        result = new ProjectInPlane();
    }else if(name.compare("HelmertTransformation") == 0){
        result = new Helmert7Param();
    }else if(name.compare("ShiftPlane") == 0){
        result = new ShiftPlane();
    }else if(name.compare("BestFitCircle") == 0){
        result = new BestFitCircle();
    }else if(name.compare("StandardTempComp") == 0){
        result = new SimpleTemperatureCompensation();
    }else if(name.compare("9ParameterHelmertTransformation") == 0){
        result = new ExtendedTemperatureCompensation();
    }else if(name.compare("6ParameterHelmertTransformation") == 0){
        result = new Helmert6Param();
    }else if(name.compare("BestFitSphereRauls") == 0){
        result = new BestFitSphereRauls();
    }
    return result;
}

/*!
 * \brief OiTemplatePlugin::createNetworkAdjustment
 * Returns the network adjustment with the specified name
 * \param name
 * \return
 */
NetworkAdjustment* OiTemplatePlugin::createNetworkAdjustment(QString name){
    NetworkAdjustment *result = NULL;

    return result;
}

/*!
 * \brief OiTemplatePlugin::createSimulation
 * Returns the simulation with the specified name
 * \param name
 * \return
 */
SimulationModel *OiTemplatePlugin::createSimulation(QString name){
    SimulationModel *result = NULL;

    if(name.compare("SimplePolarMeasurement") == 0){
        result = new SimplePolarMeasurement();
    }

    return result;
}
