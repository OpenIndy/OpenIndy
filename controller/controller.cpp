#include "controller.h"

#include "feature.h"
#include "coordinatesystem.h"
#include "station.h"
#include "pi_lasertracker.h"

/*!
 * \brief Controller::Controller
 * \param parent
 */
Controller::Controller(QObject *parent) :
    QObject(parent)
{
    /*c = new Console(0);
    Console::c = c;

    //get pointer to state objects
    this->myConfigState = OiConfigState::getInstance();

    this->myDeleteFeaturesCallback = new DeleteFeaturesFunctor();
    this->myDeleteFeaturesCallback->c = this;

    //set up models
    //this->connectModels();

    this->createDefaultProject();

    this->lastRequestId = -1;


    connect(PluginLoader::myMetaInfo,SIGNAL(sendMe(PluginMetaData*)),this,SLOT(savePluginData(PluginMetaData*)));


    openIndyServer = new OiServer();
    openIndyServer->startServer();

    openIndyWebSocketServer = new OiWebSocketServer();
    openIndyWebSocketServer->startServer();
    //connect(openIndyServer, SIGNAL(getProject(OiProjectData*)), this, SLOT(handleRemoteCommand(OiProjectData*)));
*/
}

/*!
 * \brief Controller::setActiveFeature
 * \param featureId
 */
void Controller::setActiveFeature(const int &featureId){
/*
	//get the selected feature by its id
    FeatureWrapper *selectedFeature = OiJob::getFeature(featureId);
	
	//check if the selected feature exists
    if(selectedFeature == NULL || selectedFeature->getFeature() == NULL){
        Console::addLine("Cannot activate an invalid feature");
        return;
	}

    //activate the feature if it is not activated yet
    if(!selectedFeature->getFeature()->getIsActiveFeature()){
        selectedFeature->getFeature()->setActiveFeatureState(true);
    }
*/
}

void Controller::setActiveStation(const int &featureId)
{

}

/*!
 * \brief Controller::setActiveCoordinateSystem
 * \param featureId
 */
void Controller::setActiveCoordinateSystem(const int &featureId){
    //OiJob::activateCoordinateSystem(featureId);
}

/*!
 * \brief Controller::addFeatures
 * Add one or more features defined by the given attributes
 * \param attributes
 */
void Controller::addFeatures(const FeatureAttributes &attributes){
    //OiJob::addFeatures(attributes);
    //Console::addLine("feature creation failed");
}

void Controller::removeFeature(const int &featureId)
{

}


/*!
 * \brief Controller::startMeasurement
 * After checking some conditions, it calls the measure function of the active sensor.
 */
void Controller::startMeasurement(){
/*
    if(OiJob::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }else if(OiJob::getActiveFeature() == NULL){
        Console::addLine("no active feature");
        return;
    }

    bool checkActiveCoordSys = false;

    if (OiJob::getActiveStation()->coordSys->getIsActiveCoordinateSystem()){
        checkActiveCoordSys = true;
    }

    if(checkSensorValid() && checkFeatureValid()){

        if(OiJob::getActiveFeature()->getGeometry()->getIsNominal()){
            if(!this->generateActualForNominal(OiJob::getActiveFeature())){
                Console::addLine("can not create actual for nominal feature");
                return;
            }
        }
        switch (OiJob::getActiveFeature()->getTypeOfFeature()) {
        case Configuration::ePlaneFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(Plane::defaultMeasurementConfig);
            break;
        case Configuration::ePointFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(Point::defaultMeasurementConfig);
            break;
        case Configuration::eLineFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(Line::defaultMeasurementConfig);
            break;
        case Configuration::eSphereFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(Sphere::defaultMeasurementConfig);
            break;
        case Configuration::eScalarEntityAngleFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(ScalarEntityAngle::defaultMeasurementConfig);
            break;
        case Configuration::eScalarEntityDistanceFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(ScalarEntityDistance::defaultMeasurementConfig);
            break;
        case Configuration::eCircleFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(Circle::defaultMeasurementConfig);
            break;
        case Configuration::eConeFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(Cone::defaultMeasurementConfig);
            break;
        case Configuration::eCylinderFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(Cylinder::defaultMeasurementConfig);
            break;
        case Configuration::eEllipsoidFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(Ellipsoid::defaultMeasurementConfig);
            break;
        case Configuration::eHyperboloidFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(Hyperboloid::defaultMeasurementConfig);
            break;
        case Configuration::eParaboloidFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(Paraboloid::defaultMeasurementConfig);
            break;
        case Configuration::eNurbsFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(Nurbs::defaultMeasurementConfig);
            break;
        case Configuration::ePointCloudFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(PointCloud::defaultMeasurementConfig);
            break;
        case Configuration::eScalarEntityTemperatureFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(ScalarEntityTemperature::defaultMeasurementConfig);
            break;
        case Configuration::eScalarEntityMeasurementSeriesFeature:
            OiJob::getActiveFeature()->getGeometry()->setMeasurementConfig(ScalarEntityMeasurementSeries::defaultMeasurementConfig);
            break;
        }
        OiJob::getActiveStation()->emitStartMeasure(OiJob::getActiveFeature()->getGeometry(), checkActiveCoordSys);

        emit sensorWorks("measuring...");
    }*/
}

/*!
 * \brief Controller::addMeasurement
 * Add the last reading of current sensor to the active feature as an observations
 */
void Controller::addMeasurement(){
/*
    if(OiJob::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }else if(OiJob::getActiveFeature() == NULL || OiJob::getActiveFeature()->getGeometry() == NULL){
        Console::addLine("no active feature");
        return;
    }

    bool checkActiveCoordSys = false;

    if (OiJob::getActiveStation()->coordSys->getIsActiveCoordinateSystem()){
        checkActiveCoordSys = true;
    }

    if(checkSensorValid() && checkFeatureValid()){

        if(OiJob::getActiveFeature()->getGeometry()->getIsNominal()){
            if(!this->generateActualForNominal(OiJob::getActiveFeature())){
                Console::addLine("can not create actual for nominal feature");
                return;
            }
        }

        QPair<Configuration::ReadingTypes, Reading*> lastReading = OiJob::getActiveStation()->sensorPad->instrument->getLastReading();

        Reading *r = new Reading();
        *r = *lastReading.second;
        OiJob::getActiveStation()->sensorPad->addReading(r, OiJob::getActiveFeature()->getGeometry(), checkActiveCoordSys);

    }
*/
}

/*!
 * \brief Controller::startMove
 * After checking some conditions, it calls the move function of the active sensor.
 */
void Controller::startMove(Reading *parameter){
/*
    if(OiJob::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    //TODO check function
    if (parameter->typeofReading == Configuration::ePolar){
        OiJob::getActiveStation()->emitStartMove(parameter->rPolar.azimuth,parameter->rPolar.zenith,parameter->rPolar.distance,false);
        emit sensorWorks("moving...");
    }else if (parameter->typeofReading == Configuration::eCartesian){
        OiJob::getActiveStation()->emitStartMove(parameter->rCartesian.xyz.getAt(0),parameter->rCartesian.xyz.getAt(1),parameter->rCartesian.xyz.getAt(2));
        emit sensorWorks("moving...");
    }
*/
}


void Controller::startAim(){
/*
    if(OiJob::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }else if(OiJob::getActiveFeature() == NULL){
        Console::addLine("no active feature");
        return;
    }

    if(OiJob::getActiveFeature()->getGeometry() != NULL && !OiJob::getActiveFeature()->getGeometry()->getIsSolved()){
        Console::addLine("Cannot aim a unsolved feature.");
        return;
    }
    if(checkFeatureValid() && checkSensorValid()){

        OiVec xyz = OiJob::getActiveFeature()->getGeometry()->getXYZ();
        if(xyz.getSize() < 3){
            return;
        }
        OiVec polarElements = Reading::toPolar(xyz.getAt(0),xyz.getAt(1),xyz.getAt(2));
        if(OiJob::getActiveStation()->coordSys != OiJob::getActiveCoordinateSystem()){

            //get homogeneous matrix from "from- coordsys" to active coord system
            OiMat t = FeatureUpdater::trafoControl.getTransformationMatrix(OiJob::getActiveStation()->coordSys);
            //if matrix is valid
            if(t.getColCount() == 4 && t.getRowCount() == 4){
                OiVec xyz = Reading::toCartesian(polarElements.getAt(0),polarElements.getAt(1),polarElements.getAt(2));
                //inverse because t is from "from" to active system, we need xyz in "from" system, that is the
                //active station coord system
                xyz = t.inv() * xyz;
                polarElements = Reading::toPolar(xyz.getAt(0),xyz.getAt(1),xyz.getAt(2));
            }

            /*QList<TrafoParam*> myTrafoParams = OiFeatureState::getActiveCoordinateSystem()->getTransformationParameters(OiFeatureState::getActiveStation()->coordSys);
            TrafoParam *tp = NULL;
            if(myTrafoParams.size() > 0){
                for(int i=0;i<myTrafoParams.size();i++){
                    if(!myTrafoParams.at(i)->getIsMovement() && myTrafoParams.at(i)->getIsUsed()){
                        tp = myTrafoParams.at(i);
                        break;
                    }
                }
            }
            if(tp != NULL){
                OiMat t;
                if(tp->getDestinationSystem() == OiFeatureState::getActiveStation()->coordSys){
                    t = tp->getHomogenMatrix();
                }else{
                    t = tp->getHomogenMatrix().inv();
                }
                OiVec xyz = Reading::toCartesian(polarElements.getAt(0),polarElements.getAt(1),polarElements.getAt(2));
                xyz = t * xyz;
                polarElements = Reading::toPolar(xyz.getAt(0),xyz.getAt(1),xyz.getAt(2));
            }*/
/*
        }

        OiJob::getActiveStation()->emitStartMove(polarElements.getAt(0),polarElements.getAt(1),polarElements.getAt(2),false);
        emit sensorWorks("moving...");
    }
*/
}

/*!
 * \brief Controller::startConnect
 * After checking some conditions, it calls the connect function of the active sensor.
 */
void Controller::startConnect(){
/*
    if(OiJob::getActiveStation() != NULL){
        if(OiJob::getActiveStation()->sensorPad->instrument != NULL){
            OiJob::getActiveStation()->emitStartConnect(OiJob::getActiveStation()->getInstrumentConfig().connConfig);
            emit sensorWorks("connecting...");
            OiSensorEmitter *s = OiJob::getActiveStation()->getActiveSensorEmitter();
            connect(s,SIGNAL(sendConnectionStat(bool)),this,SLOT(sendIsConnected(bool)));
            connect(s,SIGNAL(sendIsReadyForMeasurement(int,QString)),this,SLOT(sendSensorState(int,QString)));
        }else{
            Console::addLine("sensor not connected");
        }
    }else{
        Console::addLine("no active station");
    }*/
}
/*!
 * \brief Controller::startDisconnect
 * After checking some conditions, it calls the disconnect function of the active sensor.
 */
void Controller::startDisconnect(){
/*
    if(OiJob::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    if(checkSensorValid()){
        OiSensorEmitter *s = OiJob::getActiveStation()->getActiveSensorEmitter();
        disconnect(s,SIGNAL(sendConnectionStat(bool)),this,SLOT(sendIsConnected(bool)));
        disconnect(s,SIGNAL(sendIsReadyForMeasurement(int,QString)),this,SLOT(sendSensorState(int,QString)));
        OiJob::getActiveStation()->emitStartDisconnect();
        emit sensorWorks("disconnecting...");
        emit sensorDisconnected();
    }*/
}

/*!
 * \brief Controller::startToggleSight
 * After checking some conditions, it calls the toggle sight function of the active sensor.
 */
void Controller::startToggleSight(){
/*
    if(OiJob::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    if(checkSensorValid()){
        OiJob::getActiveStation()->emitStartToggleSight();
        emit sensorWorks("toggle sight...");
    }*/
}

/*!
 * \brief Controller::startInitialize
 * After checking some conditions, it calls the initialize function of the active sensor.
 */
void Controller::startInitialize(){
/*
    if(OiJob::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    if(checkSensorValid()){
        OiJob::getActiveStation()->emitStartInitialize();
        emit sensorWorks("initialize...");
    }*/
}

/*!
 * \brief Controller::startHome
 * After checking some conditions, it calls the home function of the active sensor.
 */
void Controller::startHome(){
/*
    if(OiJob::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    if(checkSensorValid()){
        OiJob::getActiveStation()->emitStartHome();
        emit sensorWorks("home...");
    }*/
}

/*!
 * \brief Controller::startCompensation
 * After checking some conditions, it calls the compensation function of the active sensor.
 */
void Controller::startCompensation(){
/*
    if(OiJob::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    if(checkSensorValid()){
        OiJob::getActiveStation()->emitStartCompensation();
        emit sensorWorks("compensation...");
    }*/
}

/*!
 * \brief Controller::startChangeMotorState
 * After checking some conditions, it calls the change motor state function of the active sensor.
 */
void Controller::startChangeMotorState(){
/*
    if(OiJob::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    if(checkSensorValid()){
        OiJob::getActiveStation()->emitStartMotorState();
        emit sensorWorks("change motor state...");
    }*/
}

/*!
 * \brief Controller::startCustomAction calls the custom action of the sensor.
 * \param s
 */
void Controller::startCustomAction(const QString &s)
{/*
    if(OiJob::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    emit sensorWorks(s);
    OiJob::getActiveStation()->emitSelfDefinedAction(s);*/
}
