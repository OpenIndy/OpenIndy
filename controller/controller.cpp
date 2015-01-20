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
    c = new Console(0);
    Console::c = c;

    //get pointer to state objects
    this->myFeatureState = OiFeatureState::getInstance();
    this->myConfigState = OiConfigState::getInstance();

    this->myDeleteFeaturesCallback = new DeleteFeaturesFunctor();
    this->myDeleteFeaturesCallback->c = this;

    lastmConfig;
    //this->defaultLastmConfig();

    //set up models
    this->initModels();
    this->connectModels();

    this->createDefaultProject();

    this->lastRequestId = -1;


    this->myModelManager = OiModelManager::getInstance();


    //set up filter mechanism for available elements treeview
    connect(this, SIGNAL(sendAvailableElementsFilter(Configuration::ElementTypes,bool)), this->availableElementsModel, SLOT(setFilter(Configuration::ElementTypes,bool)));


    connect(PluginLoader::myMetaInfo,SIGNAL(sendMe(PluginMetaData*)),this,SLOT(savePluginData(PluginMetaData*)));


    //connect(OiFeatureState::getActiveStation(),SIGNAL(actionFinished(bool)),this,SLOT(showResults(bool)));
    //connect(&OiFeatureState::getActiveStation()->sensorPad->getOiEmitter(),SIGNAL(sendString(QString)),this,SLOT(printToConsole(QString)));
    //connect(&this->activeStation->sensorPad->instrument->myEmitter,SIGNAL(sendString(QString)),this,SLOT(printToConsole(QString)));
    //connect(this,SIGNAL(refreshGUI()),this->tblModel,SLOT(updateModel()));

    //emit refreshGUI();

    openIndyServer = new OiServer();
    openIndyServer->startServer();
    openIndyWebSocketServer = new OiWebSocketServer();
    openIndyWebSocketServer->startServer();
    //connect(openIndyServer, SIGNAL(getProject(OiProjectData*)), this, SLOT(handleRemoteCommand(OiProjectData*)));


}

/*!
 * \brief Controller::initModels
 * Create and initialize models
 */
void Controller::initModels(){
    try{

        //models for tableviews
        this->tblModel = new TableModel();
        this->featureOverviewModel = new FeatureOverviewProxyModel();
        this->featureOverviewModel->setSourceModel(this->tblModel);
        this->featureOverviewModel->setDynamicSortFilter(true);
        this->trafoParamModel = new TrafoParamProxyModel();
        this->trafoParamModel->setSourceModel(this->tblModel);

        //models for function plugin dialog
        this->functionTreeViewModel = new QStandardItemModel();
        this->functionTreeViewModel->setHorizontalHeaderItem(0, new QStandardItem("functions"));
        this->neededElementsModel = new QSqlQueryModel();
        this->usedElementsModel = new UsedElementsModel();

        this->pluginsModel = new QSqlQueryModel();

        //model for plugin overview
        this->myPluginTreeViewModel = new PluginTreeViewModel();
        this->myPluginTreeViewModel->refreshModel();

        //feature treeview models
        this->featureTreeViewModel = new FeatureTreeViewModel();
        this->featureTreeViewModel->refreshModel();
        this->availableElementsModel = new AvailableElementsTreeViewProxyModel();
        this->availableElementsModel->setHeader("available elements");
        this->availableElementsModel->setSourceModel(this->featureTreeViewModel);
        this->featureGraphicsModel = new FeatureGraphicsTreeViewProxyModel();
        this->featureGraphicsModel->setHeader("feature overview");
        this->featureGraphicsModel->setSourceModel(this->featureTreeViewModel);

        //feature groups model
        this->myFeatureGroupsModel = new QStringListModel();
        this->setUpFeatureGroupsModel();

        //coordinate systems model
        this->myCoordinateSystemsModel = new QStringListModel();

        //point feature model
        this->myPointFeatureModel = new PointFeatureModel();
        this->myPointFeatureProxyModel = new PointFeatureFilterModel();
        this->myPointFeatureProxyModel->setSourceModel(this->myPointFeatureModel);

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief Controller::connectModels
 * Update models when a corresponding signal is emittet from OiFeatureState
 */
void Controller::connectModels(){
    try{

        //update table model when one or more features change
        connect(this->myFeatureState, SIGNAL(featureSetChanged()), this->tblModel, SLOT(updateModel()));
        connect(this->myFeatureState, SIGNAL(activeFeatureChanged()), this->tblModel, SLOT(updateModel()));
        connect(this->myFeatureState, SIGNAL(activeStationChanged()), this->tblModel, SLOT(updateModel()));
        connect(this->myFeatureState, SIGNAL(geometryObservationsChanged()), this, SLOT(recalcActiveFeature()), Qt::DirectConnection);
        connect(this->myFeatureState, SIGNAL(systemObservationsAdded()), this, SLOT(recalcActiveFeature()), Qt::DirectConnection);
        connect(this->myFeatureState, SIGNAL(activeCoordinateSystemChanged()), this->tblModel, SLOT(updateModel()));

        //update feature groups model when a group is added or removed
        connect(this->myFeatureState, SIGNAL(availableGroupsChanged()), this, SLOT(setUpFeatureGroupsModel()));

        //update feature tree view model which is used in function plugin loader
        connect(this->myFeatureState, SIGNAL(featureSetChanged()), this->featureTreeViewModel, SLOT(refreshModel()));

        //update coordinate systems model
        connect(this->myFeatureState, SIGNAL(featureSetChanged()), this, SLOT(setUpCoordinateSystemsModel()));
        connect(this->myFeatureState, SIGNAL(activeCoordinateSystemChanged()), this, SLOT(setUpCoordinateSystemsModel()));

        //update function treeview model for active feature
        connect(this->myFeatureState, SIGNAL(featureFunctionsChanged()), this, SLOT(changeFunctionTreeViewModel()));
        connect(this->myFeatureState, SIGNAL(activeFeatureChanged()), this, SLOT(changeFunctionTreeViewModel()));

        //send save or load project task to OiRequestHandler & listen to his answers
        connect(this, SIGNAL(sendXmlRequest(OiRequestResponse*)), OiRequestHandler::getInstance(), SLOT(receiveRequest(OiRequestResponse*)));
        connect(OiRequestHandler::getInstance(), SIGNAL(sendResponse(OiRequestResponse*)), this, SLOT(receiveRequestResult(OiRequestResponse*)));

        //save project each time when observations were added
        connect(OiFeatureState::getInstance(), SIGNAL(systemObservationsAdded()), this, SLOT(saveProject()));



        connect(this, SIGNAL(refreshGUI()), this->tblModel, SLOT(updateModel()));

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief Controller::createDefaultFeatures
 * Create a station and the PART system as default features when starting OpenIndy
 */
bool Controller::createDefaultProject(){

    if(OiFeatureState::getFeatureCount() == 0){

        OiProjectData::setActiveProject("OpenIndyTest");

        //create PART and STATION01 as default
        FeatureAttributesExchange partAttributes, stationAttributes;
        partAttributes.featureType = Configuration::eCoordinateSystemFeature;
        partAttributes.name = "PART";
        stationAttributes.featureType = Configuration::eStationFeature;
        stationAttributes.name = "STATION01";
        FeatureWrapper *part = OiFeatureState::addFeature(partAttributes);
        FeatureWrapper *station01 = OiFeatureState::addFeature(stationAttributes);

        //set position parameter for STATION01
        station01->getStation()->position->setCommonState(false);

        //set STATION01's station system as active station
        station01->getStation()->setActiveStationState(true);

        //set PART as active coordinate system
        part->getCoordinateSystem()->setActiveCoordinateSystemState(true);

    }else{
        return false;
    }

    return true;

}

/*!
 * \brief Controller::addFeature
 * Creates new features with given parameters. After creating, the features are set to the feature wrapper and added
 * to the feature list in the controller.
 * \param count
 * \param featureType
 * \param name
 * \param actualNominal
 * \param isCommonPoint
 */
void Controller::addFeature(FeatureAttributesExchange fae){

    //get default measurement config depending on the feature type of the feature to be created
    MeasurementConfig mConfig;
    switch(fae.featureType){
    case Configuration::eCircleFeature:
        mConfig = Circle::defaultMeasurementConfig;
        break;
    case Configuration::eConeFeature:
        mConfig = Cone::defaultMeasurementConfig;
        break;
    case Configuration::eCylinderFeature:
        mConfig = Cylinder::defaultMeasurementConfig;
        break;
    case Configuration::eEllipsoidFeature:
        mConfig = Ellipsoid::defaultMeasurementConfig;
        break;
    case Configuration::eHyperboloidFeature:
        mConfig = Hyperboloid::defaultMeasurementConfig;
        break;
    case Configuration::eLineFeature:
        mConfig = Line::defaultMeasurementConfig;
        break;
    case Configuration::eNurbsFeature:
        mConfig = Nurbs::defaultMeasurementConfig;
        break;
    case Configuration::eParaboloidFeature:
        mConfig = Paraboloid::defaultMeasurementConfig;
        break;
    case Configuration::ePlaneFeature:
        mConfig = Plane::defaultMeasurementConfig;
        break;
    case Configuration::ePointFeature:
        mConfig = Point::defaultMeasurementConfig;
        break;
    case Configuration::ePointCloudFeature:
        mConfig = PointCloud::defaultMeasurementConfig;
        break;
    case Configuration::eScalarEntityAngleFeature:
        mConfig = ScalarEntityAngle::defaultMeasurementConfig;
        break;
    case Configuration::eScalarEntityDistanceFeature:
        mConfig = ScalarEntityDistance::defaultMeasurementConfig;
        break;
    case Configuration::eScalarEntityMeasurementSeriesFeature:
        mConfig = ScalarEntityMeasurementSeries::defaultMeasurementConfig;
        break;
    case Configuration::eScalarEntityTemperatureFeature:
        mConfig = ScalarEntityTemperature::defaultMeasurementConfig;
        break;
    case Configuration::eSphereFeature:
        mConfig = Sphere::defaultMeasurementConfig;
        break;
    }

    int fType = FeatureUpdater::addFeature(fae, mConfig);
    if(fType == Configuration::eStationFeature && fType == Configuration::eCoordinateSystemFeature){
        emit CoordSystemsModelChanged();
    }

    //refresh feature tree view models
    //this->featureTreeViewModel->refreshModel();


    //emit refreshGUI();
    //emit featureAdded();

    //refresh feature tree view models
    //this->featureTreeViewModel->refreshModel();
}

/*!
 * \brief Controller::startMeasurement
 * After checking some conditions, it calls the measure function of the active sensor.
 */
void Controller::startMeasurement(){

    if(OiFeatureState::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }else if(OiFeatureState::getActiveFeature() == NULL){
        Console::addLine("no active feature");
        return;
    }

    bool checkActiveCoordSys = false;

    if (OiFeatureState::getActiveStation()->coordSys->getIsActiveCoordinateSystem()){
        checkActiveCoordSys = true;
    }

    if(checkSensorValid() && checkFeatureValid()){

        if(OiFeatureState::getActiveFeature()->getGeometry()->getIsNominal()){
            if(!this->generateActualForNominal(OiFeatureState::getActiveFeature())){
                Console::addLine("can not create actual for nominal feature");
                return;
            }
        }
        switch (OiFeatureState::getActiveFeature()->getTypeOfFeature()) {
        case Configuration::ePlaneFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(Plane::defaultMeasurementConfig);
            break;
        case Configuration::ePointFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(Point::defaultMeasurementConfig);
            break;
        case Configuration::eLineFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(Line::defaultMeasurementConfig);
            break;
        case Configuration::eSphereFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(Sphere::defaultMeasurementConfig);
            break;
        case Configuration::eScalarEntityAngleFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(ScalarEntityAngle::defaultMeasurementConfig);
            break;
        case Configuration::eScalarEntityDistanceFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(ScalarEntityDistance::defaultMeasurementConfig);
            break;
        case Configuration::eCircleFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(Circle::defaultMeasurementConfig);
            break;
        case Configuration::eConeFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(Cone::defaultMeasurementConfig);
            break;
        case Configuration::eCylinderFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(Cylinder::defaultMeasurementConfig);
            break;
        case Configuration::eEllipsoidFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(Ellipsoid::defaultMeasurementConfig);
            break;
        case Configuration::eHyperboloidFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(Hyperboloid::defaultMeasurementConfig);
            break;
        case Configuration::eParaboloidFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(Paraboloid::defaultMeasurementConfig);
            break;
        case Configuration::eNurbsFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(Nurbs::defaultMeasurementConfig);
            break;
        case Configuration::ePointCloudFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(PointCloud::defaultMeasurementConfig);
            break;
        case Configuration::eScalarEntityTemperatureFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(ScalarEntityTemperature::defaultMeasurementConfig);
            break;
        case Configuration::eScalarEntityMeasurementSeriesFeature:
            OiFeatureState::getActiveFeature()->getGeometry()->setMeasurementConfig(ScalarEntityMeasurementSeries::defaultMeasurementConfig);
            break;
        }
        OiFeatureState::getActiveStation()->emitStartMeasure(OiFeatureState::getActiveFeature()->getGeometry(), checkActiveCoordSys);

        emit sensorWorks("measuring...");
    }
}

/*!
 * \brief Controller::addMeasurement
 * Add the last reading of current sensor to the active feature as an observations
 */
void Controller::addMeasurement(){

    if(OiFeatureState::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }else if(OiFeatureState::getActiveFeature() == NULL || OiFeatureState::getActiveFeature()->getGeometry() == NULL){
        Console::addLine("no active feature");
        return;
    }

    bool checkActiveCoordSys = false;

    if (OiFeatureState::getActiveStation()->coordSys->getIsActiveCoordinateSystem()){
        checkActiveCoordSys = true;
    }

    if(checkSensorValid() && checkFeatureValid()){

        if(OiFeatureState::getActiveFeature()->getGeometry()->getIsNominal()){
            if(!this->generateActualForNominal(OiFeatureState::getActiveFeature())){
                Console::addLine("can not create actual for nominal feature");
                return;
            }
        }

        QPair<Configuration::ReadingTypes, Reading*> lastReading = OiFeatureState::getActiveStation()->sensorPad->instrument->getLastReading();

        Reading *r = new Reading();
        *r = *lastReading.second;
        OiFeatureState::getActiveStation()->sensorPad->addReading(r, OiFeatureState::getActiveFeature()->getGeometry(), checkActiveCoordSys);

    }

}

/*!
 * \brief Controller::startMove
 * After checking some conditions, it calls the move function of the active sensor.
 */
void Controller::startMove(Reading *parameter){

    if(OiFeatureState::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    //TODO check function
    if (parameter->typeofReading == Configuration::ePolar){
        OiFeatureState::getActiveStation()->emitStartMove(parameter->rPolar.azimuth,parameter->rPolar.zenith,parameter->rPolar.distance,false);
        emit sensorWorks("moving...");
    }else if (parameter->typeofReading == Configuration::eCartesian){
        OiFeatureState::getActiveStation()->emitStartMove(parameter->rCartesian.xyz.getAt(0),parameter->rCartesian.xyz.getAt(1),parameter->rCartesian.xyz.getAt(2));
        emit sensorWorks("moving...");
    }

}


void Controller::startAim(){

    if(OiFeatureState::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }else if(OiFeatureState::getActiveFeature() == NULL){
        Console::addLine("no active feature");
        return;
    }

    if(OiFeatureState::getActiveFeature()->getGeometry() != NULL && !OiFeatureState::getActiveFeature()->getGeometry()->getIsSolved()){
        Console::addLine("Cannot aim a unsolved feature.");
        return;
    }
    if(checkFeatureValid() && checkSensorValid()){

        OiVec xyz = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ();
        if(xyz.getSize() < 3){
            return;
        }
        OiVec polarElements = Reading::toPolar(xyz.getAt(0),xyz.getAt(1),xyz.getAt(2));
        if(OiFeatureState::getActiveStation()->coordSys != OiFeatureState::getActiveCoordinateSystem()){

            //get homogeneous matrix from "from- coordsys" to active coord system
            OiMat t = FeatureUpdater::trafoControl.getTransformationMatrix(OiFeatureState::getActiveStation()->coordSys);
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

        }

        OiFeatureState::getActiveStation()->emitStartMove(polarElements.getAt(0),polarElements.getAt(1),polarElements.getAt(2),false);
        emit sensorWorks("moving...");
    }

}

/*!
 * \brief Controller::startConnect
 * After checking some conditions, it calls the connect function of the active sensor.
 */
void Controller::startConnect(){

    if(OiFeatureState::getActiveStation() != NULL){
        if(OiFeatureState::getActiveStation()->sensorPad->instrument != NULL){
            OiFeatureState::getActiveStation()->emitStartConnect(OiFeatureState::getActiveStation()->getInstrumentConfig().connConfig);
            emit sensorWorks("connecting...");
            OiSensorEmitter *s = OiFeatureState::getActiveStation()->getActiveSensorEmitter();
            connect(s,SIGNAL(sendConnectionStat(bool)),this,SLOT(sendIsConnected(bool)));
            connect(s,SIGNAL(sendIsReadyForMeasurement(int,QString)),this,SLOT(sendSensorState(int,QString)));
        }else{
            Console::addLine("sensor not connected");
        }
    }else{
        Console::addLine("no active station");
    }
}
/*!
 * \brief Controller::startDisconnect
 * After checking some conditions, it calls the disconnect function of the active sensor.
 */
void Controller::startDisconnect(){

    if(OiFeatureState::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    if(checkSensorValid()){
        OiSensorEmitter *s = OiFeatureState::getActiveStation()->getActiveSensorEmitter();
        disconnect(s,SIGNAL(sendConnectionStat(bool)),this,SLOT(sendIsConnected(bool)));
        disconnect(s,SIGNAL(sendIsReadyForMeasurement(int,QString)),this,SLOT(sendSensorState(int,QString)));
        OiFeatureState::getActiveStation()->emitStartDisconnect();
        emit sensorWorks("disconnecting...");
        emit sensorDisconnected();
    }
}

/*!
 * \brief Controller::startToggleSight
 * After checking some conditions, it calls the toggle sight function of the active sensor.
 */
void Controller::startToggleSight(){

    if(OiFeatureState::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    if(checkSensorValid()){
        OiFeatureState::getActiveStation()->emitStartToggleSight();
        emit sensorWorks("toggle sight...");
    }
}

/*!
 * \brief Controller::startInitialize
 * After checking some conditions, it calls the initialize function of the active sensor.
 */
void Controller::startInitialize(){

    if(OiFeatureState::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    if(checkSensorValid()){
        OiFeatureState::getActiveStation()->emitStartInitialize();
        emit sensorWorks("initialize...");
    }
}

/*!
 * \brief Controller::startHome
 * After checking some conditions, it calls the home function of the active sensor.
 */
void Controller::startHome(){

    if(OiFeatureState::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    if(checkSensorValid()){
        OiFeatureState::getActiveStation()->emitStartHome();
        emit sensorWorks("home...");
    }
}

/*!
 * \brief Controller::startCompensation
 * After checking some conditions, it calls the compensation function of the active sensor.
 */
void Controller::startCompensation(){

    if(OiFeatureState::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    if(checkSensorValid()){
        OiFeatureState::getActiveStation()->emitStartCompensation();
        emit sensorWorks("compensation...");
    }
}

/*!
 * \brief Controller::startChangeMotorState
 * After checking some conditions, it calls the change motor state function of the active sensor.
 */
void Controller::startChangeMotorState(){

    if(OiFeatureState::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    if(checkSensorValid()){
        OiFeatureState::getActiveStation()->emitStartMotorState();
        emit sensorWorks("change motor state...");
    }
}

/*!
 * \brief Controller::startCustomAction calls the custom action of the sensor.
 * \param s
 */
void Controller::startCustomAction(QString s)
{
    if(OiFeatureState::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    emit sensorWorks("custom action: " + s);
    OiFeatureState::getActiveStation()->emitSelfDefinedAction(s);
}

void Controller::emitShowWatchWindow()
{
    emit this->showWatchWindow();
}

void Controller::emitCloseWatchWindow()
{
    emit this->closeWatchWindow();
}

void Controller::recalcAll()
{
   myFeatureUpdater.recalcAll();
   emit refreshGUI();
}

/*!
 * \brief Controller::recalcActiveFeature
 * Call recalcFeature function for active feature
 */
void Controller::recalcActiveFeature(){
    if(OiFeatureState::getActiveFeature() != NULL && OiFeatureState::getActiveFeature()->getTrafoParam() != NULL){
        this->recalcTrafoParam(OiFeatureState::getActiveFeature()->getTrafoParam());
    }else if(OiFeatureState::getActiveFeature() != NULL && OiFeatureState::getActiveFeature()->getFeature() != NULL){
        this->recalcFeature(OiFeatureState::getActiveFeature()->getFeature());
    }
    emit this->refreshGUI();
}

/*!
 * \brief Controller::recalcFeature
 * Recalcs the given feature
 * \param f
 */
void Controller::recalcFeature(Feature *f){
    //start recalcing
    this->myFeatureUpdater.recalcFeature(f);
    //refresh feature tree view models
    this->featureTreeViewModel->refreshModel();

    emit this->refreshGUI();

}

/*!
 * \brief Controller::recalcTrafoParam
 * Recalcs the given trafo param
 * \param tp
 */
void Controller::recalcTrafoParam(TrafoParam *tp){
    //start recalcing
    this->myFeatureUpdater.recalcTrafoParam(tp);
    //refresh feature tree view models
    this->featureTreeViewModel->refreshModel();
}

/*!
 * \brief Controller::changeActiveStation
 * this function changes the active station to the given one and set the sensor object to the new one.
 * \param setSensor
 */
void Controller::changeActiveStation(bool setSensor){

    if(OiFeatureState::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }else if(OiFeatureState::getActiveFeature() == NULL){
        Console::addLine("no active feature");
        return;
    }

    if(OiFeatureState::getActiveFeature()->getStation() != NULL){

        /*if(OiFeatureState::getActiveStation()->sensorPad->instrument != NULL){
            this->startDisconnect();
        }*/

        if(setSensor){
            OiFeatureState::getActiveStation()->sensorPad->copyMe(OiFeatureState::getActiveFeature()->getStation()->sensorPad);
            //OiFeatureState::getActiveFeature()->getStation()->sensorPad = OiFeatureState::getActiveStation()->sensorPad;
            //OiFeatureState::getActiveFeature()->getStation()->setInstrumentConfig(OiFeatureState::getActiveStation()->getInstrumentConfig());
            OiFeatureState::getActiveFeature()->getStation()->setActiveStationState(true);
            //this->startConnect();
        }else{
            OiFeatureState::getActiveFeature()->getStation()->setActiveStationState(true);
        }

    connect(OiFeatureState::getActiveStation(),SIGNAL(actionFinished(bool)),this,SLOT(showResults(bool)));
    emit changedStation();
    emit refreshGUI();
    }
}

/*!
 * \brief Controller::showResults
 * After a sensor action finish, this funtion is called, to show the result in the console.
 * \param b
 */
void Controller::showResults(bool b){
    if(b){
        Console::addLine("sensor action completed");
        Console::addLine("count observation: ", OiFeatureState::getActiveStation()->coordSys->getObservations().size());
    }
}

/*!
 * \brief Controller::defaultLastmConfig
 * Setting up a default constellation of the last measurement configuration.
 * It can be changed at runtime.
 */
/*void Controller::defaultLastmConfig(){
    lastmConfig.name = "default configuration";
    lastmConfig.count = 1;
    lastmConfig.iterations = 1;
    lastmConfig.measureTwoSides = false;
    if(OiFeatureState::getActiveStation() != NULL && OiFeatureState::getActiveStation()->sensorPad->instrument != NULL){
        QList<Configuration::ReadingTypes> *suppRTypes = OiFeatureState::getActiveStation()->sensorPad->instrument->getSupportedReadingTypes();
        if(suppRTypes != NULL && suppRTypes->contains(Configuration::ePolar)){
            lastmConfig.typeOfReading = Configuration::ePolar;
        }else{
            lastmConfig.typeOfReading = OiFeatureState::getActiveStation()->sensorPad->instrument->getSupportedReadingTypes()->at(0);
        }
    }else{
        lastmConfig.typeOfReading = Configuration::ePolar;
    }
    lastmConfig.timeDependent = false;
    lastmConfig.distanceDependent = false;
    lastmConfig.timeInterval = 0.0;
    lastmConfig.distanceInterval = 0.0;
}*/

/*!
 * \brief Controller::savePluginData
 * This function saves the plugin data and meta data in the database of openIndy.
 * So the plugin can be chosen in runtime out of all available plugins in the database.
 * \param metaInfo
 */
void Controller::savePluginData(PluginMetaData* metaInfo){

    if (!metaInfo->alreadyExists){

        QList<Sensor*> sensorList = PluginLoader::loadSensorPlugins(metaInfo->path);
        QList<Function*> functionList = PluginLoader::loadFunctionPlugins(metaInfo->path);
        QList<NetworkAdjustment*> networkAdjustmentList = PluginLoader::loadNetworkAdjustmentPlugins(metaInfo->path);
        QList<SimulationModel*> simulationList = PluginLoader::loadSimulationPlugins(metaInfo->path);
        QList<OiTool*> toolList = PluginLoader::loadOiToolPlugins(metaInfo->path);
        QList<OiExchangeSimpleAscii*> simpleAsciiList = PluginLoader::loadOiExchangeSimpleAsciiPlugins(metaInfo->path);
        QList<OiExchangeDefinedFormat*> definedFormatList = PluginLoader::loadOiExchangeDefinedFormatPlugins(metaInfo->path);

        SystemDbManager::savePlugin(metaInfo, functionList, sensorList, networkAdjustmentList,simulationList,toolList, simpleAsciiList, definedFormatList);

    }
}

/*!
 * \brief Controller::setSensorModel
 * Loads all available plugins for the chosen sensor. You can chose the sensor at runtime in the set instrument menu.
 * \param sT
 */
void Controller::setSensorModel(Configuration::SensorTypes sT){

    switch(sT){
    case Configuration::eLaserTracker:
        SystemDbManager::getLaserTrackerModel(pluginsModel);
        break;
    case Configuration::eTotalStation:
        SystemDbManager::getTotalStationModel(pluginsModel);
        break;
    case Configuration::eUndefinedSensor:
        SystemDbManager::getUndefinedSensorModel(pluginsModel);
        break;
    }
    emit sendSQLModel(pluginsModel);
}

/*!
 * \brief Controller::getSelectedPlugin
 * Loads the selected plugin and sets it up as active station, after an instance was build.
 * \param index
 * \param sT
 */
void Controller::getSelectedPlugin(int index){

    /*if(OiFeatureState::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    Console::addLine("index: ", index);

    QString path = pluginsModel->record(index).value("file_path").toString();
    QString name = pluginsModel->record(index).value("name").toString();
    Console::addLine(path);

    if(path != NULL){

        //this->activeStation->InstrumentConfig = new SensorConfiguration();
        OiFeatureState::getActiveStation()->sensorPad->instrument = PluginLoader::loadSensorPlugin(path, name);
        connect(&OiFeatureState::getActiveStation()->sensorPad->instrument->myEmitter,SIGNAL(sendString(QString)),this,SLOT(printToConsole(QString)));
        //defaultLastmConfig();
        updateFeatureMConfig();
    }*/
}

void Controller::getTempSensor(int index)
{
    QString path = pluginsModel->record(index).value("file_path").toString();
    QString name = pluginsModel->record(index).value("name").toString();

    if(path != NULL){
        Sensor *s = PluginLoader::loadSensorPlugin(path, name);
        emit sendTempSensor(s);
    }

}

/*!
 * \brief Controller::getSelectedFeature
 * Sets up the selected feature from the tableview as active feature.
 * So it can be used for measuring and other actions.
 * \param index
 */
void Controller::setSelectedFeature(int featureIndex){
    try{

        //if a new feature was selected
        if(OiFeatureState::getFeatures().at(featureIndex) != NULL && OiFeatureState::getFeatures().at(featureIndex)->getFeature() != NULL
                && !OiFeatureState::getFeatures().at(featureIndex)->getFeature()->getIsActiveFeature()){
            //this->changeUsedElementsModel(-1, -1);

            //set the selected feature as active feature
            OiFeatureState::getFeatures().at(featureIndex)->getFeature()->setActiveFeatureState(true);

            //send the actual position of the active feature
            if(OiFeatureState::getActiveFeature()->getGeometry()!= NULL){
                double x  = OiFeatureState::getActiveFeature()->getGeometry()->getDisplayX(false).toDouble();
                double y = OiFeatureState::getActiveFeature()->getGeometry()->getDisplayY(false).toDouble();
                double z = OiFeatureState::getActiveFeature()->getGeometry()->getDisplayZ(false).toDouble();

                emit sendPositionOfActiveFeature(x,  y,  z);
            }

        }
        //emit refreshGUI();
        //set up tree view model with all functions of selected feature
        //this->changeFunctionTreeViewModel();

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief Controller::receiveSensorConfiguration
 * \param sc
 */
void Controller::receiveSensorConfiguration(SensorConfiguration sc, bool connect){

    if(OiFeatureState::getActiveStation() == NULL){
        Console::addLine("no active station");
        return;
    }

    OiFeatureState::getActiveStation()->sensorPad->instrument = sc.mySensor;
    QObject::connect(&OiFeatureState::getActiveStation()->sensorPad->instrument->myEmitter,SIGNAL(sendString(QString)),this,SLOT(printToConsole(QString)));
    updateFeatureMConfig();

    if(connect){
        OiFeatureState::getActiveStation()->setInstrumentConfig(sc);
        this->startConnect();
    }else{
        OiFeatureState::getActiveStation()->setInstrumentConfig(sc);
    }

}

/*!
 * \brief Controller::setFunction
 * Opens the menu to set a function for the active feature. The dialog shows all available functions for the selected feature.
 */
void Controller::setFunction(){
    if(OiFeatureState::getActiveFeature() != NULL && OiFeatureState::getActiveFeature()->getFeature() != NULL){
        if(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().size() == 0
                && (OiFeatureState::getActiveFeature()->getGeometry() == NULL || !OiFeatureState::getActiveFeature()->getGeometry()->getIsNominal())){
            SystemDbManager::getCreateFunctionModel(this->pluginsModel,OiFeatureState::getActiveFeature()->getTypeOfFeature());
        }else{
            SystemDbManager::getChangeFunctionModel(this->pluginsModel,OiFeatureState::getActiveFeature()->getTypeOfFeature());
        }
    }else{
        Console::addLine("no feature selected");
    }
}

/*!
 * \brief Controller::receiveFunctionId
 * Receives the id of the selected function for the selected feature and querys the needed elements from database for that function.
 * \param id
 */
void Controller::receiveFunctionId(int id){
    SystemDbManager::getNeededElements(neededElementsModel, id);
}

/*!
 * \brief Controller::createFunction
 * Creates an instance of the selected function and adds it to the function list of the active and selected feature.
 * \param index
 */
void Controller::createFunction(int index){
    qDebug() << "creat func";
    qDebug() << index;
    qDebug() << this->pluginsModel->record(index).value("file_path").toString();
    QString path = pluginsModel->record(index).value("file_path").toString();
    QString name = pluginsModel->record(index).value("name").toString();
    if(OiFeatureState::getActiveFeature() != NULL && OiFeatureState::getActiveFeature()->getFeature() != NULL){
        //if the active feature is not a nominal geometry
        /*if(OiFeatureState::getActiveFeature()->getGeometry() == NULL
                || !OiFeatureState::getActiveFeature()->getGeometry()->getIsNominal()){*/
            //create function and connect OiFunctionEmitter to console
            Function *newFunction = PluginLoader::loadFunctionPlugin(path,name);
            if(newFunction != NULL){
                connect(&newFunction->getOiEmitter(), SIGNAL(sendString(QString)), this, SLOT(printToConsole(QString)));
                OiFeatureState::getActiveFeature()->getFeature()->addFunction(newFunction);
            }
            //set up tree view model with all functions of selected feature
            this->changeFunctionTreeViewModel();
            //set up available functions for the active feature
            this->setFunction();
        /*}else{
            Console::addLine("A nominal geometry cannot have a function!");
        }*/
    }
}

/*!
 * \brief Controller::deleteFunctionFromFeature
 * Delete the function at the specified index from the active feature
 * \param index
 */
void Controller::deleteFunctionFromFeature(int index){
    if(OiFeatureState::getActiveFeature() != NULL && OiFeatureState::getActiveFeature()->getFeature() != NULL){
        Feature *activeFeature = OiFeatureState::getActiveFeature()->getFeature();
        if(activeFeature->getFunctions().size() > index){
            //the first function of a feature can only be deleted if it is the only function
            if(activeFeature->getFunctions().size() > 1 && index == 0){
                Console::addLine("Cannot delete the function of a feature that defines it!");
            }else{
                activeFeature->removeFunction(index);
                //set up tree view model with all functions of selected feature
                this->changeFunctionTreeViewModel();
                //set up available functions for the active feature
                this->setFunction();
            }
        }
    }
}

bool Controller::checkFeatureValid(){
    if(OiFeatureState::getActiveFeature() != NULL && OiFeatureState::getActiveFeature()->getFeature() != NULL){
        if(OiFeatureState::getActiveFeature()->getGeometry() != NULL && OiFeatureState::getActiveFeature()->getGeometry() != OiFeatureState::getActiveStation()->position){
            return true;
        }else{
            Console::addLine("you cannot measure this feature");
            return false;
        }
    }else{
        Console::addLine("no feature selected");
        return false;
    }
}

bool Controller::checkSensorValid(){

    if(OiFeatureState::getActiveStation() != NULL){
        if(OiFeatureState::getActiveStation()->sensorPad->instrument != NULL){
            return true;
        }else{
            Console::addLine("sensor not connected");
            return false;
        }
    }else{
        Console::addLine("no active station");
        return false;
    }
}

void Controller::importFeatures(QList<FeatureWrapper*> f){
    for(int i = 0;i<f.size();i++){

        OiFeatureState::addFeature(f.at(i));

    }

    //refresh feature tree view models
    this->featureTreeViewModel->refreshModel();

    refreshGUI();
}

void Controller::setActiveCoordSystem(QString CoordSysName){

    qDebug() << CoordSysName;
    for(int i=0; i<OiFeatureState::getFeatures().size();i++){

        FeatureWrapper *fw = OiFeatureState::getFeatures().at(i);

        if(OiFeatureState::getFeatures().at(i)->getCoordinateSystem() != NULL && OiFeatureState::getFeatures().at(i)->getCoordinateSystem()->getFeatureName() == CoordSysName){
            OiFeatureState::getFeatures().at(i)->getCoordinateSystem()->setActiveCoordinateSystemState(true);
            break;
        }
        if(OiFeatureState::getFeatures().at(i)->getStation() != NULL &&
                OiFeatureState::getFeatures().at(i)->getStation()->coordSys != NULL &&
                OiFeatureState::getFeatures().at(i)->getStation()->getFeatureName() == CoordSysName){
            OiFeatureState::getFeatures().at(i)->getStation()->coordSys->setActiveCoordinateSystemState(true);
            break;
        }

    }

    if(OiFeatureState::getActiveCoordinateSystem() != NULL){

        //transform observations to current system and recalc all features
        this->myFeatureUpdater.switchCoordinateSystem(OiFeatureState::getActiveCoordinateSystem());

    }

    //update table view for all features
    emit this->refreshGUI();
}

/*!
 * \brief Controller::changeFunctionTreeViewModel
 * Update model for tree view with all functions of selected feature
 */
void Controller::changeFunctionTreeViewModel(){
    this->functionTreeViewModel->clear();
    this->functionTreeViewModel->setHorizontalHeaderItem(0, new QStandardItem("functions"));
    QStandardItem *rootItem = this->functionTreeViewModel->invisibleRootItem();
    if(OiFeatureState::getActiveFeature() != NULL){
        foreach(Function *f, OiFeatureState::getActiveFeature()->getFeature()->getFunctions()){
            if(f != NULL){
                QStandardItem *function = new QStandardItem(f->getMetaData()->name);
                foreach(InputParams param, f->getNeededElements()){
                    if(param.infinite){
                        QStandardItem *element = new QStandardItem(QString("n %1s").arg(Configuration::getElementTypeString(param.typeOfElement)));
                        function->appendRow(element);
                    }else{
                        QStandardItem *element = new QStandardItem(QString("1 %1").arg(Configuration::getElementTypeString(param.typeOfElement)));
                        function->appendRow(element);
                    }
                }
                rootItem->appendRow(function);
            }
        }
    }
}

/*!
 * \brief Controller::changeUsedElementsModel
 * Update model for listview with used elements
 * \param functionIndex
 * \param elementIndex
 */
void Controller::changeUsedElementsModel(int functionIndex, int elementIndex){
    this->usedElementsModel->removeAllElements();

    if(functionIndex >= 0 && elementIndex >= 0 && OiFeatureState::getActiveFeature() != NULL
            && OiFeatureState::getActiveFeature()->getFeature()->getFunctions().size() > functionIndex
            && OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(functionIndex)->getNeededElements().size() > elementIndex){
        //get function at functionIndex of active feature
        Function *func = OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(functionIndex);
        QMap<int, QList<InputFeature> > featureOrder = func->getFeatureOrder();
        QMap<int, QList<InputFeature> >::iterator idx = featureOrder.find(elementIndex);
        if(idx != featureOrder.end() && idx.value().size() > 0){ //if the list with elements is not empty
            QList<InputFeature> featurePosition = idx.value(); //elements of function at elementIndex
            switch(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement){
                case Configuration::ePointElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Point *p = func->getPoint(featurePosition.at(i).id);
                        if(p != NULL){
                            FeatureWrapper *pointWrapper = new FeatureWrapper();
                            pointWrapper->setPoint(p);
                            FeatureTreeItem *point = new FeatureTreeItem(p->getFeatureName());
                            point->setFeature(pointWrapper);
                            this->usedElementsModel->addElement(point);
                        }
                    }
                    break;
                case Configuration::eLineElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Line *l = func->getLine(featurePosition.at(i).id);
                        if(l != NULL){
                            FeatureWrapper *lineWrapper = new FeatureWrapper();
                            lineWrapper->setLine(l);
                            FeatureTreeItem *line = new FeatureTreeItem(l->getFeatureName());
                            line->setFeature(lineWrapper);
                            this->usedElementsModel->addElement(line);
                        }
                    }
                    break;
                case Configuration::ePlaneElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Plane *p = func->getPlane(featurePosition.at(i).id);
                        if(p != NULL){
                            FeatureWrapper *planeWrapper = new FeatureWrapper();
                            planeWrapper->setPlane(p);
                            FeatureTreeItem *plane = new FeatureTreeItem(p->getFeatureName());
                            plane->setFeature(planeWrapper);
                            this->usedElementsModel->addElement(plane);
                        }
                    }
                    break;
                case Configuration::eSphereElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Sphere *s = func->getSphere(featurePosition.at(i).id);
                        if(s != NULL){
                            FeatureWrapper *sphereWrapper = new FeatureWrapper();
                            sphereWrapper->setSphere(s);
                            FeatureTreeItem *sphere = new FeatureTreeItem(s->getFeatureName());
                            sphere->setFeature(sphereWrapper);
                            this->usedElementsModel->addElement(sphere);
                        }
                    }
                    break;
                case Configuration::eScalarEntityAngleElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        ScalarEntityAngle *s = func->getScalarEntityAngle(featurePosition.at(i).id);
                        if(s != NULL){
                            FeatureWrapper *angleWrapper = new FeatureWrapper();
                            angleWrapper->setScalarEntityAngle(s);
                            FeatureTreeItem *angle = new FeatureTreeItem(s->getFeatureName());
                            angle->setFeature(angleWrapper);
                            this->usedElementsModel->addElement(angle);
                        }
                    }
                    break;
                case Configuration::eScalarEntityDistanceElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        ScalarEntityDistance *s = func->getScalarEntityDistance(featurePosition.at(i).id);
                        if(s != NULL){
                            FeatureWrapper *distanceWrapper = new FeatureWrapper();
                            distanceWrapper->setScalarEntityDistance(s);
                            FeatureTreeItem *distance = new FeatureTreeItem(s->getFeatureName());
                            distance->setFeature(distanceWrapper);
                            this->usedElementsModel->addElement(distance);
                        }
                    }
                    break;
                case Configuration::eTrafoParamElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        TrafoParam *tp = func->getTrafoParam(featurePosition.at(i).id);
                        if(tp != NULL){
                            FeatureWrapper *trafoParamWrapper = new FeatureWrapper();
                            trafoParamWrapper->setTrafoParam(tp);
                            FeatureTreeItem *trafoParam = new FeatureTreeItem(tp->getFeatureName());
                            trafoParam->setFeature(trafoParamWrapper);
                            this->usedElementsModel->addElement(trafoParam);
                        }
                    }
                    break;
                case Configuration::eCoordinateSystemElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        CoordinateSystem *cs = func->getCoordinateSystem(featurePosition.at(i).id);
                        if(cs != NULL){
                            FeatureWrapper *coordinateSystemWrapper = new FeatureWrapper();
                            coordinateSystemWrapper->setCoordinateSystem(cs);
                            FeatureTreeItem *coordinateSystem = new FeatureTreeItem(cs->getFeatureName());
                            coordinateSystem->setFeature(coordinateSystemWrapper);
                            this->usedElementsModel->addElement(coordinateSystem);
                        }
                    }
                    break;
                case Configuration::eStationElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Station *s = func->getStation(featurePosition.at(i).id);
                        if(s != NULL){
                            FeatureWrapper *stationWrapper = new FeatureWrapper();
                            stationWrapper->setStation(s);
                            FeatureTreeItem *station = new FeatureTreeItem(s->getFeatureName());
                            station->setFeature(stationWrapper);
                            this->usedElementsModel->addElement(station);
                        }
                    }
                    break;
                case Configuration::eObservationElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Observation *o = func->getObservation(featurePosition.at(i).id);
                        if(o != NULL && o->myReading != NULL){
                            FeatureTreeItem *observation = new FeatureTreeItem(QString::number(o->myReading->id));
                            observation->setObservation(o);
                            this->usedElementsModel->addElement(observation);
                        }
                    }
                    break;
                case Configuration::eReadingCartesianElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Reading *r = func->getReadingCartesian(featurePosition.at(i).id);
                        if(r != NULL){
                            FeatureTreeItem *reading = new FeatureTreeItem(r->measuredAt.toString());
                            reading->setReading(r);
                            this->usedElementsModel->addElement(reading);
                        }
                    }
                    break;
                case Configuration::eReadingDirectionElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Reading *r = func->getReadingDirection(featurePosition.at(i).id);
                        if(r != NULL){
                            FeatureTreeItem *reading = new FeatureTreeItem(r->measuredAt.toString());
                            reading->setReading(r);
                            this->usedElementsModel->addElement(reading);
                        }
                    }
                    break;
                case Configuration::eReadingDistanceElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Reading *r = func->getReadingDistance(featurePosition.at(i).id);
                        if(r != NULL){
                            FeatureTreeItem *reading = new FeatureTreeItem(r->measuredAt.toString());
                            reading->setReading(r);
                            this->usedElementsModel->addElement(reading);
                        }
                    }
                    break;
                case Configuration::eReadingPolarElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Reading *r = func->getReadingPolar(featurePosition.at(i).id);
                        if(r != NULL){
                            FeatureTreeItem *reading = new FeatureTreeItem(r->measuredAt.toString());
                            reading->setReading(r);
                            this->usedElementsModel->addElement(reading);
                        }
                    }
                    break;
                case Configuration::eScalarEntityTemperatureElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        ScalarEntityTemperature *s = func->getScalarEntityTemperature(featurePosition.at(i).id);
                        if(s != NULL){
                            FeatureWrapper *temperatureWrapper = new FeatureWrapper();
                            temperatureWrapper->setScalarEntityTemperature(s);
                            FeatureTreeItem *temperature = new FeatureTreeItem(s->getFeatureName());
                            temperature->setFeature(temperatureWrapper);
                            this->usedElementsModel->addElement(temperature);
                        }
                    }
                    break;
            }
        }
    }

    this->usedElementsModel->refreshModel();
}

/*!
 * \brief Controller::setSelectedFunction
 * Receive selected function from function plugin loader dialog
 * \param index
 * \param neededElement
 */
void Controller::setSelectedFunction(int functionIndex, int neededElementIndex){
    if(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().size() > functionIndex && functionIndex >= 0){ //if function index is valid
        if(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(functionIndex)->getNeededElements().size() > neededElementIndex
                && neededElementIndex >= 0){ //if needed element index is valid
            emit this->sendFunctionDescription(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(functionIndex)->getNeededElements()
                                               .at(neededElementIndex).description);
            emit this->sendAvailableElementsFilter(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(functionIndex)->getNeededElements()
                                                   .at(neededElementIndex).typeOfElement, false);
        }else{ //if function itself was clicked
            Function *func = OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(functionIndex);
            emit this->sendFunctionDescription(func->getMetaData()->description);
            emit this->sendAvailableElementsFilter(Configuration::eUndefinedElement, true);
            emit this->sendExtraParameterForFunction(func->getIntegerParameter(), func->getDoubleParameter(), func->getStringParameter(), func->getFunctionConfiguration());
        }
    }else{
        emit this->sendAvailableElementsFilter(Configuration::eUndefinedElement, true);
    }
    //re-build used elements model
    this->changeUsedElementsModel(functionIndex, neededElementIndex);
}

/*!
 * \brief Controller::addElement2Function
 * Add element to the active function at index
 * \param element
 * \param functionIndex
 * \param elementIndex
 */
void Controller::addElement2Function(FeatureTreeItem *element, int functionIndex, int elementIndex){
    if(element != NULL && functionIndex >= 0 && elementIndex >= 0 && OiFeatureState::getActiveFeature() != NULL){
        //check circle warning recursively
        if(element->getIsFeature() && element->getFeature() != NULL && element->getFeature()->getFeature() != NULL
                && this->checkCircleWarning(OiFeatureState::getActiveFeature()->getFeature(), element->getFeature()->getFeature())){
            Console::addLine(QString("%1 %2")
                             .arg("You cannot use a feature for the calculation, which itself is directly or indirectly")
                             .arg("dependent on the feature to be calculated."));
            emit this->showMessageBox("circle warning", QString("%1 %2")
                                      .arg("You cannot use a feature for the calculation, which itself is directly or indirectly")
                                      .arg("dependent on the feature to be calculated."));
            return;
        }
        //get pointer to active feature and add used element
        Feature *feature = OiFeatureState::getActiveFeature()->getFeature();
        if(feature->getFunctions().size() > functionIndex && feature->getFunctions().at(functionIndex)->getNeededElements().size() > elementIndex){
            if(element->getIsFeature() && element->getFeature() != NULL && element->getFeature()->getFeature() != NULL){
                switch(element->getFeature()->getTypeOfFeature()){
                    case Configuration::ePointFeature:
                        if(element->getFeature()->getPoint() != NULL
                                && feature->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::ePointElement){
                            feature->getFunctions().at(functionIndex)->addPoint(element->getFeature()->getPoint(), elementIndex);
                        }
                        break;
                    case Configuration::eLineFeature:
                        if(element->getFeature()->getLine() != NULL
                                && feature->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eLineElement){
                            feature->getFunctions().at(functionIndex)->addLine(element->getFeature()->getLine(), elementIndex);
                        }
                        break;
                    case Configuration::ePlaneFeature:
                        if(element->getFeature()->getPlane() != NULL
                                && feature->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::ePlaneElement){
                            feature->getFunctions().at(functionIndex)->addPlane(element->getFeature()->getPlane(), elementIndex);
                        }
                        break;
                    case Configuration::eSphereFeature:
                        if(element->getFeature()->getSphere() != NULL
                                && feature->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eSphereElement){
                            feature->getFunctions().at(functionIndex)->addSphere(element->getFeature()->getSphere(), elementIndex);
                        }
                        break;
                    case Configuration::eStationFeature:
                        if(element->getFeature()->getStation() != NULL
                                && feature->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eStationElement){
                            feature->getFunctions().at(functionIndex)->addStation(element->getFeature()->getStation(), elementIndex);
                        }
                        break;
                    case Configuration::eCoordinateSystemFeature:
                        if(element->getFeature()->getCoordinateSystem() != NULL
                                && feature->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eCoordinateSystemElement){
                            feature->getFunctions().at(functionIndex)->addCoordSystem(element->getFeature()->getCoordinateSystem(), elementIndex);
                        }
                        break;
                    case Configuration::eTrafoParamFeature:
                        if(element->getFeature()->getTrafoParam() != NULL
                                && feature->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eTrafoParamElement){
                            feature->getFunctions().at(functionIndex)->addTrafoParam(element->getFeature()->getTrafoParam(), elementIndex);
                        }
                        break;
                    case Configuration::eScalarEntityAngleFeature:
                        if(element->getFeature()->getScalarEntityAngle() != NULL
                                && feature->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eScalarEntityAngleElement){
                            feature->getFunctions().at(functionIndex)->addScalarEntityAngle(element->getFeature()->getScalarEntityAngle(), elementIndex);
                        }
                        break;
                    case Configuration::eScalarEntityDistanceFeature:
                        if(element->getFeature()->getScalarEntityDistance() != NULL
                                && feature->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eScalarEntityDistanceElement){
                            feature->getFunctions().at(functionIndex)->addScalarEntityDistance(element->getFeature()->getScalarEntityDistance(), elementIndex);
                        }
                        break;
                    case Configuration::eScalarEntityTemperatureFeature:
                        if(element->getFeature()->getScalarEntityTemperature() != NULL
                                && feature->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eScalarEntityTemperatureElement){
                            feature->getFunctions().at(functionIndex)->addScalarEntityTemperature(element->getFeature()->getScalarEntityTemperature(), elementIndex);
                        }
                        break;
                }
                //set usedFor and previouslyNeeded for active feature and used element
                feature->previouslyNeeded.append(element->getFeature());
                element->getFeature()->getFeature()->usedFor.append(OiFeatureState::getActiveFeature());
            }else if(element->getIsReading() && element->getReading() != NULL){
                switch(element->getReading()->typeofReading){
                    case Configuration::eCartesian:
                        if(feature->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eReadingCartesianElement){
                            feature->getFunctions().at(functionIndex)->addReadingCartesian(element->getReading(), elementIndex);
                        }
                        break;
                    case Configuration::ePolar:
                        if(feature->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eReadingPolarElement){
                            feature->getFunctions().at(functionIndex)->addReadingPolar(element->getReading(), elementIndex);
                        }
                        break;
                    case Configuration::eDistance:
                        if(feature->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eReadingDistanceElement){
                            feature->getFunctions().at(functionIndex)->addReadingDistance(element->getReading(), elementIndex);
                        }
                        break;
                    case Configuration::eDirection:
                        if(feature->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eReadingDirectionElement){
                            feature->getFunctions().at(functionIndex)->addReadingDirection(element->getReading(), elementIndex);
                        }
                        break;
                }
            }else if(element->getIsObservation() && element->getObservation() != NULL
                     && feature->getFunctions().at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eObservationElement){
                feature->getFunctions().at(functionIndex)->addObservation(element->getObservation(), elementIndex);

                //if feature is a geometry add the observation to the list of observations in class geometry
                Geometry *geom = OiFeatureState::getActiveFeature()->getGeometry();
                if(geom != NULL){
                    geom->addObservation(element->getObservation());
                    geom->getObservations().last()->myTargetGeometries.append(geom);
                }
            }
        }
        this->changeUsedElementsModel(functionIndex, elementIndex);
    }
}

/*!
 * \brief Controller::removeElementFromFunction
 * Remove element from the active function at elementIndex
 * \param element
 * \param functionIndex
 * \param elementIndex
 */
void Controller::removeElementFromFunction(FeatureTreeItem *element, int functionIndex, int elementIndex){
    if(element != NULL && functionIndex >= 0 && OiFeatureState::getActiveFeature() != NULL){
        Feature *feature = OiFeatureState::getActiveFeature()->getFeature();
        if(feature->getFunctions().size() > functionIndex && feature->getFunctions().at(functionIndex) != NULL){
            if(element->getIsFeature() && element->getFeature() != NULL && element->getFeature()->getFeature() != NULL){
                switch(element->getFeature()->getTypeOfFeature()){
                    case Configuration::ePointFeature:
                        if(element->getFeature()->getPoint() != NULL){
                            feature->getFunctions().at(functionIndex)->removePoint(element->getFeature()->getFeature()->getId());
                        }
                        break;
                    case Configuration::eLineFeature:
                        if(element->getFeature()->getLine() != NULL){
                            feature->getFunctions().at(functionIndex)->removeLine(element->getFeature()->getFeature()->getId());
                        }
                        break;
                    case Configuration::ePlaneFeature:
                        if(element->getFeature()->getPlane() != NULL){
                            feature->getFunctions().at(functionIndex)->removePlane(element->getFeature()->getFeature()->getId());
                        }
                        break;
                    case Configuration::eSphereFeature:
                        if(element->getFeature()->getSphere() != NULL){
                            feature->getFunctions().at(functionIndex)->removeSphere(element->getFeature()->getFeature()->getId());
                        }
                        break;
                    case Configuration::eStationFeature:
                        if(element->getFeature()->getStation() != NULL){
                            feature->getFunctions().at(functionIndex)->removeStation(element->getFeature()->getFeature()->getId());
                        }
                        break;
                    case Configuration::eCoordinateSystemFeature:
                        if(element->getFeature()->getCoordinateSystem() != NULL){
                            feature->getFunctions().at(functionIndex)->removeCoordSystem(element->getFeature()->getFeature()->getId());
                        }
                        break;
                    case Configuration::eTrafoParamFeature:
                        if(element->getFeature()->getTrafoParam() != NULL){
                            feature->getFunctions().at(functionIndex)->removeTrafoParam(element->getFeature()->getFeature()->getId());
                        }
                        break;
                    case Configuration::eScalarEntityAngleFeature:
                        if(element->getFeature()->getScalarEntityAngle() != NULL){
                            feature->getFunctions().at(functionIndex)->removeScalarEntityAngle(element->getFeature()->getFeature()->getId());
                        }
                        break;
                    case Configuration::eScalarEntityDistanceFeature:
                        if(element->getFeature()->getScalarEntityDistance() != NULL){
                            feature->getFunctions().at(functionIndex)->removeScalarEntityDistance(element->getFeature()->getFeature()->getId());
                        }
                        break;
                    case Configuration::eScalarEntityTemperatureFeature:
                        if(element->getFeature()->getScalarEntityTemperature() != NULL){
                            feature->getFunctions().at(functionIndex)->removeScalarEntityTemperature(element->getFeature()->getFeature()->getId());
                        }
                        break;
                }
                //set usedFor and previouslyNeeded for active feature and used element
                feature->previouslyNeeded.removeOne(element->getFeature());
                element->getFeature()->getFeature()->usedFor.removeOne(OiFeatureState::getActiveFeature());
            }else if(element->getIsObservation() && element->getObservation() != NULL){
                feature->getFunctions().at(functionIndex)->removeObservation(element->getObservation()->getId());
            }else if(element->getIsReading() && element->getReading() != NULL){
                switch(element->getReading()->typeofReading){
                    case Configuration::eCartesian:
                        feature->getFunctions().at(functionIndex)->removeReadingCartesian(element->getReading()->id);
                    case Configuration::eDirection:
                        feature->getFunctions().at(functionIndex)->removeReadingDirection(element->getReading()->id);
                    case Configuration::eDistance:
                        feature->getFunctions().at(functionIndex)->removeReadingDistance(element->getReading()->id);
                    case Configuration::ePolar:
                        feature->getFunctions().at(functionIndex)->removeReadingPolar(element->getReading()->id);
                }
            }
            this->changeUsedElementsModel(functionIndex, elementIndex);
        }
    }
}

/*!
 * \brief Controller::checkCircleWarning
 * Checks wether there is a circular reference. If so this function returns true, otherwise false.
 * \param activeFeature
 * \param usedForActiveFeature
 * \return
 */
bool Controller::checkCircleWarning(Feature *activeFeature, Feature *usedForActiveFeature){
    if(activeFeature != NULL && usedForActiveFeature != NULL){
        //if activeFeature and usedForActiveFeature are equal
        if(activeFeature->getId() == usedForActiveFeature->getId()){
            return true;
        }
        //if active feature is in list of previously needed features of usedForActiveFeature
        foreach(FeatureWrapper *f, usedForActiveFeature->previouslyNeeded){
            if(f != NULL && f->getFeature() != NULL){
                if(f->getFeature()->getId() == activeFeature->getId()){
                    return true;
                }else if(f->getFeature()->previouslyNeeded.size() > 0 && this->checkCircleWarning(activeFeature, f->getFeature())){
                    return true;
                }
            }
        }
        return false;
    }
    return true;
}

int Controller::checkActiveFeatureIndex(int current, int index){

    //int tmpIndex = index;
    int trafoParamCount = 0;
    int featureIndex = 0;

    for (current; current <= index; current++){
        if(OiFeatureState::getFeatures().at(current)->getTrafoParam() != NULL){
            trafoParamCount += 1;
        }
    }
    featureIndex += trafoParamCount;
    if( trafoParamCount > 0){
        featureIndex += checkActiveFeatureIndex(index+1, index+trafoParamCount);
    }

    return featureIndex;
}

/*!
 * \brief getNominalValues sets the attributes of the selected feature to the specified values.
 * It uses the exchange object for nominal data
 * \param nominalValue
 */
void Controller::getNominalValues(NominalAttributeExchange nominalValue){

    switch (OiFeatureState::getActiveFeature()->getTypeOfFeature()) {
    case Configuration::ePointFeature:
        OiFeatureState::getActiveFeature()->getPoint()->xyz.setAt(0,nominalValue.nomX);
        OiFeatureState::getActiveFeature()->getPoint()->xyz.setAt(1,nominalValue.nomY);
        OiFeatureState::getActiveFeature()->getPoint()->xyz.setAt(2,nominalValue.nomZ);
        break;
    case Configuration::ePlaneFeature:
        OiFeatureState::getActiveFeature()->getPlane()->xyz.setAt(0,nominalValue.nomX);
        OiFeatureState::getActiveFeature()->getPlane()->xyz.setAt(1,nominalValue.nomY);
        OiFeatureState::getActiveFeature()->getPlane()->xyz.setAt(2,nominalValue.nomZ);
        OiFeatureState::getActiveFeature()->getPlane()->ijk.setAt(0,nominalValue.nomI);
        OiFeatureState::getActiveFeature()->getPlane()->ijk.setAt(1,nominalValue.nomJ);
        OiFeatureState::getActiveFeature()->getPlane()->ijk.setAt(2,nominalValue.nomK);
        break;
    case Configuration::eLineFeature:
        OiFeatureState::getActiveFeature()->getLine()->xyz.setAt(0,nominalValue.nomX);
        OiFeatureState::getActiveFeature()->getLine()->xyz.setAt(1,nominalValue.nomY);
        OiFeatureState::getActiveFeature()->getLine()->xyz.setAt(2,nominalValue.nomZ);
        OiFeatureState::getActiveFeature()->getLine()->ijk.setAt(0,nominalValue.nomI);
        OiFeatureState::getActiveFeature()->getLine()->ijk.setAt(1,nominalValue.nomJ);
        OiFeatureState::getActiveFeature()->getLine()->ijk.setAt(2,nominalValue.nomK);
        break;
    case Configuration::eSphereFeature:
        OiFeatureState::getActiveFeature()->getSphere()->xyz.setAt(0,nominalValue.nomX);
        OiFeatureState::getActiveFeature()->getSphere()->xyz.setAt(1,nominalValue.nomY);
        OiFeatureState::getActiveFeature()->getSphere()->xyz.setAt(2,nominalValue.nomZ);
        OiFeatureState::getActiveFeature()->getSphere()->radius = nominalValue.nomR;
        break;
    case Configuration::eScalarEntityAngleFeature:
        OiFeatureState::getActiveFeature()->getScalarEntityAngle()->setAngle(nominalValue.nomSAE);
        break;
    case Configuration::eScalarEntityDistanceFeature:
        OiFeatureState::getActiveFeature()->getScalarEntityDistance()->setDistance(nominalValue.nomSDE);
        break;
    case Configuration::eScalarEntityTemperatureFeature:
        OiFeatureState::getActiveFeature()->getScalarEntityTemperature()->setTemperature(nominalValue.nomSTE);
        break;
    case Configuration::eScalarEntityMeasurementSeriesFeature:
        OiFeatureState::getActiveFeature()->getScalarEntityMeasurementSeries()->setSeriesValue(nominalValue.nomSMSE);
        break;
    default:
        break;
    }
}

/*!
 * \brief Controller::setFunctionConfiguration
 * Set function configuration for the function at functionIndex of active feature
 * \param functionIndex
 * \param config
 */
void Controller::setFunctionConfiguration(int functionIndex, FunctionConfiguration config){
    if(OiFeatureState::getActiveFeature() != NULL && functionIndex >= 0 &&
            OiFeatureState::getActiveFeature()->getFeature()->getFunctions().size() > functionIndex){
        OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(functionIndex)->setFunctionConfiguration(config);
    }
}

/*!
 * \brief Controller::saveProject
 * Save the current project
 * \param projectData
 * \return
 */
bool Controller::saveProject(){
    try{

        if(OiProjectData::getIsValid()){
            if(OiProjectData::getIsSaved()){

                OiRequestResponse *request;
                request = new OiRequestResponse();
                request->requesterId = Configuration::generateID();
                this->lastRequestId = request->requesterId;

                QDomElement root = request->request.createElement("OiRequest");
                root.setAttribute("id", OiRequestResponse::eGetProject);
                request->request.appendChild(root);

                emit this->sendXmlRequest(request);

            }else{
                Console::addLine("The project has already been saved");
                return false;
            }
        }else{
            Console::addLine("The project has no name or no device is selected");
            return false;
        }

    }catch(exception &e){
        Console::addLine(e.what());
        return false;
    }
}

/*!
 * \brief Controller::loadProject
 * \param projectName
 * \param myDevice
 * \return
 */
bool Controller::loadProject(QString projectName, QIODevice *myDevice){

    //TODO check if an active project is set

    //set active project
    OiProjectData::setActiveProject(projectName, myDevice);

    OiRequestResponse *request;
    request = new OiRequestResponse();
    request->requesterId = Configuration::generateID();
    this->lastRequestId = request->requesterId;

    QDomElement root = request->request.createElement("OiRequest");
    root.setAttribute("id", OiRequestResponse::eSetProject);
    request->request.appendChild(root);

    emit this->sendXmlRequest(request);

    //TODO delete old request objects

    /*

    //delete all features
    OiFeatureState::resetFeatureLists();

    bool result = OiProjectExchanger::loadProject(projectData);

    //recalc all features after loading
    this->recalcAll();

    this->tblModel->updateModel();

    return result;*/

    return true;

}

/*!
 * \brief Controller::startStakeOut
 * \param request
 */
void Controller::startStakeOut(QDomDocument request){
    OiRequestResponse *myRequest;
    myRequest = new OiRequestResponse();
    myRequest->requesterId = Configuration::generateID();
    this->lastRequestId = myRequest->requesterId;

    QDomNode root = myRequest->request.importNode(request.documentElement(), true);
    myRequest->request.appendChild(root);

    qDebug() << "test: " << myRequest->request.toString();

    emit this->sendXmlRequest(myRequest);
}

/*!
 * \brief Controller::nextStakeOutGeometry
 */
void Controller::nextStakeOutGeometry(){
    OiRequestResponse *myRequest;
    myRequest = new OiRequestResponse();
    myRequest->requesterId = Configuration::generateID();
    this->lastRequestId = myRequest->requesterId;

    QDomDocument request;
    QDomElement stakeOutId = request.createElement("stakeOutId");
    //stakeOutId.setAttribute("id", );

    qDebug() << "test: " << myRequest->request.toString();

    emit this->sendXmlRequest(myRequest);
}

/*!
 * \brief Controller::receiveRequestResult
 * Is called whenever a request was finished by OiRequestHandler and the response is available
 * \param request
 * \return
 */
bool Controller::receiveRequestResult(OiRequestResponse *request){
    try{

        if(request != NULL && request->requesterId == this->lastRequestId && !request->response.isNull()){

            switch(request->myRequestType){
            case OiRequestResponse::eGetProject:

                //save xml in file
                if(OiProjectData::getDevice() != NULL){

                    //create new document to remove OiResponse tag
                    QDomDocument project;
                    project.appendChild(project.importNode(request->response.documentElement().firstChildElement("oiProjectData"), true));

                    OiProjectData::getDevice()->open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);

                    QTextStream stream(OiProjectData::getDevice());
                    project.save(stream, 4);

                    OiProjectData::getDevice()->close();

                    //emit this->showMessageBox(OiProjectData::getProjectName(), "OpenIndy project successfully stored.");
                    Console::addLine("OpenIndy project successfully stored.");

                }

                break;
            case OiRequestResponse::eSetProject:
                qDebug() << request->response.toString();

                //recalc all features after loading
                this->recalcAll();

                this->tblModel->updateModel();

                break;
            }

            this->lastRequestId = -1;

            return true;

        }

        return false;

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief Controller::printToConsole
 * Connected to a functions oiemitter a plugin can print a text to the console
 * \param message
 */
void Controller::printToConsole(QString message){
    Console::addLine(message);
}

/*!
 * \brief updateFeatureMConfig checks the previously set measurement configurations for supported reading types
 */
void Controller::updateFeatureMConfig()
{
    if(OiFeatureState::getActiveStation() != NULL && OiFeatureState::getActiveStation()->sensorPad->instrument != NULL &&
            OiFeatureState::getActiveStation()->sensorPad->instrument->getSupportedReadingTypes() != NULL &&
            OiFeatureState::getActiveStation()->sensorPad->instrument->getSupportedReadingTypes()->size() >0){

        QList<Configuration::ReadingTypes> readingTypes = *OiFeatureState::getActiveStation()->sensorPad->instrument->getSupportedReadingTypes();

        //Check and edit lastMConfig
        bool contains = false;
        for(int i=0; i< readingTypes.size();i++){
            if(this->lastmConfig.typeOfReading == readingTypes.at(i)){
                contains = true;
            }
        }
        if(!contains){
            this->lastmConfig.typeOfReading = readingTypes.at(0);
        }

        //check and edit previous mconfigs
        for(int k=0; k<OiFeatureState::getFeatures().size();k++){
            contains = false;
            if(OiFeatureState::getFeatures().at(k)->getGeometry() != NULL){
                for(int m=0;m<readingTypes.size();m++){
                    if(OiFeatureState::getFeatures().at(k)->getGeometry()->getMeasurementConfig().typeOfReading == readingTypes.at(m)){
                        contains = true;
                    }
                }
                if(!contains){
                    MeasurementConfig myConfig = OiFeatureState::getFeatures().at(k)->getGeometry()->getMeasurementConfig();
                    myConfig.typeOfReading = this->lastmConfig.typeOfReading;
                    OiFeatureState::getFeatures().at(k)->getGeometry()->setMeasurementConfig(myConfig);
                }
            }
            if(OiFeatureState::getFeatures().at(k)->getStation() != NULL){
                for(int n=0; n<readingTypes.size();n++){
                    if(OiFeatureState::getFeatures().at(k)->getStation()->position->getMeasurementConfig().typeOfReading == readingTypes.at(n)){
                        contains = true;
                    }
                }
                if(!contains){
                    MeasurementConfig myConfig = OiFeatureState::getFeatures().at(k)->getStation()->position->getMeasurementConfig();
                    myConfig.typeOfReading = this->lastmConfig.typeOfReading;
                    OiFeatureState::getFeatures().at(k)->getStation()->position->setMeasurementConfig(myConfig);
                }
            }
        }
    }
}

void Controller::sendIsConnected(bool b)
{
    emit isConnected(b);
}

void Controller::sendSensorState(int sState, QString msg)
{
    emit setSensorState(sState,msg);
}

QMultiMap<QString, QString> Controller::getOiTools()
{
    return SystemDbManager::getAvailableOiTools();
}

void Controller::loadOiToolWidget(QString pluginName, QString toolName)
{
    QString pluginPath = SystemDbManager::getPluginFilePath(toolName,pluginName);

    OiTool* oiToolWidget = PluginLoader::loadOiToolPlugin(pluginPath,toolName);

    if(oiToolWidget != NULL){

        OiJob *jobState = new OiJobState();

        connect(OiFeatureState::getInstance(),SIGNAL(activeFeatureChanged()),jobState,SLOT(emitActiveFeatureChanged()));
        connect(OiFeatureState::getInstance(),SIGNAL(activeStationChanged()),jobState,SLOT(emitActiveStationChanged()));
        connect(OiFeatureState::getInstance(),SIGNAL(activeCoordinateSystemChanged()),jobState,SLOT(emitActiveCoordinateSystemChanged()));
        connect(OiFeatureState::getInstance(),SIGNAL(featureSetChanged()),jobState,SLOT(emitFeatureSetChanged()));
        connect(OiFeatureState::getInstance(),SIGNAL(featureAttributesChanged()),jobState,SLOT(emitFeatureAttributesChanged()));
        connect(OiFeatureState::getInstance(),SIGNAL(geometryObservationsChanged()),jobState,SLOT(emitGeometryObservationsChanged()));
        connect(OiFeatureState::getInstance(),SIGNAL(featureFunctionsChanged()),jobState,SLOT(emitFeatureFunctionsChanged()));
        connect(OiFeatureState::getInstance(),SIGNAL(coordSystemSetChanged()),jobState,SLOT(emitCoordSystemSetChanged()));

        connect(jobState, SIGNAL(startAim()), this, SLOT(startAim()));
        connect(jobState, SIGNAL(showWatchWindow()), this, SLOT(emitShowWatchWindow()));
        connect(jobState,SIGNAL(closeWatchWindow()), this, SLOT(emitCloseWatchWindow()));

        oiToolWidget->setOiJob(jobState);

        emit openOiToolWidget(oiToolWidget);
    }
}

bool Controller::generateActualForNominal(FeatureWrapper *f)
{
    if(f==NULL || f->getGeometry() == NULL){
        return false;
    }

    if(f->getGeometry()->getMyActual() != NULL){

    }else{
        FeatureAttributesExchange fae;
        MeasurementConfig mConfig;

        fae.actual = true;
        fae.common = false;
        fae.count = 1;
        fae.name = f->getGeometry()->getFeatureName();
        fae.destSystem = NULL;
        fae.featureType = f->getTypeOfFeature();
        fae.function = this->getDefaultFunction(f->getTypeOfFeature());
        fae.group = f->getGeometry()->getGroupName();
        fae.isMovement = false;
        fae.nominal = false;
        fae.nominalSystem = NULL;
        fae.startSystem = NULL;

        int fType = FeatureUpdater::addFeature(fae, mConfig);

        if(f->getTypeOfFeature() !=fType){
            return false;
        }
    }

    f->getGeometry()->getMyActual()->setActiveFeatureState(true);

    return true;

}

/*void Controller::handleRemoteCommand(OiProjectData *d)
{
    OiProjectExchanger::saveProject(*d);
}*/

/*!
 * \brief Controller::deleteFeature
 * Delete the specified feature if possible
 * \param myFeature
 */
void Controller::deleteFeatures(QList<FeatureWrapper*> myFeatures){
    this->featuresToDelete = myFeatures;

    //check if delete task is valid
    int countCoordSys = 0;
    bool countCheck = true; //at least one coordinate system has to exist
    bool activeCheck = true; //the active station cannot be deleted
    bool displayCheck = true; //the display coordinate system cannot be deleted
    foreach(FeatureWrapper *f, this->featuresToDelete){
        if(f->getStation() != NULL && f->getStation() == OiFeatureState::getActiveStation()){ //do not delete active station
            activeCheck = false;
            break;
        }else if(f->getCoordinateSystem() != NULL && f->getCoordinateSystem() == OiFeatureState::getActiveCoordinateSystem()){ //do not delete display coordinate system
            displayCheck = false;
            break;
        }else if(f->getStation() != NULL || f->getCoordinateSystem() != NULL){
            countCoordSys++;
        }
    }
    if(countCoordSys >= (OiFeatureState::getStations().size() + OiFeatureState::getCoordinateSystems().size())){
        countCheck = false;
    }

    if(activeCheck && countCheck && displayCheck){ //if delete task is valid

        if(myFeatures.size() == 1){
            emit this->showMessageBoxForDecision("Delete features", QString("%1 %2 %3").arg("You have selected")
                                                 .arg(myFeatures.size())
                                                 .arg("feature. Do you really want to delete it, including all dependencies?"), this->myDeleteFeaturesCallback);
        }else{
            emit this->showMessageBoxForDecision("Delete features", QString("%1 %2 %3").arg("You have selected")
                                                 .arg(myFeatures.size())
                                                 .arg("features. Do you really want to delete them, including all dependencies?"), this->myDeleteFeaturesCallback);
        }

    }else{ //delete task is not valid

        if(activeCheck == false){
            emit this->showMessageBox("Delete error", "You cannot delete the active station!");
        }else if(displayCheck == false){
            emit this->showMessageBox("Delete error", "You cannot delete the display coordinate system!");
        }else{
            emit this->showMessageBox("Delete error", "At least one coordinate system has to exist!");
        }

    }
}

/*!
 * \brief Controller::deleteFeatureCallback
 * Is called after the user has decided wether to delete a feature with dependencies or not
 * \param command
 */
void Controller::deleteFeaturesCallback(bool command){
    //TODO create a backup file before deleting features

    if(command){ //if user decided to delete the selected features

        foreach(FeatureWrapper *delFeature, this->featuresToDelete){
            if(delFeature != NULL && delFeature->getFeature() != NULL){

                //clear active feature and set it to active station
                if(delFeature->getFeature()->getIsActiveFeature()){
                    foreach(Station *myStation, OiFeatureState::getStations()){
                        if(myStation != NULL && myStation->getIsActiveStation()){
                            myStation->setActiveFeatureState(true);
                            break;
                        }
                    }
                }

                //remove the feature and all dependencies
                OiFeatureState::removeFeature(delFeature);

                //remove feature from lists
                /*OiFeatureState::
                this->features.removeOne(delFeature);
                if(delFeature->getStation() != NULL){
                    this->stations.removeOne(delFeature->getStation());
                    //remove corresponding trafo param set
                    if(delFeature->getStation()->coordSys != NULL){
                        foreach(TrafoParam *myTrafo, delFeature->getStation()->coordSys->trafoParams){
                            if(myTrafo != NULL){
                                int index = -1;
                                for(int i = 0; i < this->features.size(); i++){
                                    if(this->features.at(i) != NULL && this->features.at(i)->getTrafoParam() != NULL
                                            && this->features.at(i)->getTrafoParam()->id == myTrafo->id){
                                        index = i;
                                        break;
                                    }
                                }
                                if(index >= 0){
                                    this->features.removeAt(index);
                                }
                            }
                        }
                    }
                }else if(delFeature->getCoordinateSystem() != NULL){
                    this->coordSys.removeOne(delFeature->getCoordinateSystem());
                    //remove corresponding trafo param set
                    foreach(TrafoParam *myTrafo, delFeature->getCoordinateSystem()->trafoParams){
                        if(myTrafo != NULL){
                            int index = -1;
                            for(int i = 0; i < this->features.size(); i++){
                                if(this->features.at(i) != NULL && this->features.at(i)->getTrafoParam() != NULL
                                        && this->features.at(i)->getTrafoParam()->id == myTrafo->id){
                                    index = i;
                                    break;
                                }
                            }
                            if(index >= 0){
                                this->features.removeAt(index);
                            }
                        }
                    }
                }*/

                //update group name map for combo boxes
                /*QString group = delFeature->getFeature()->group;
                if(this->availableGroups.contains(group)){
                    int count = this->availableGroups.find(group).value();
                    if(count <= 1){
                        this->availableGroups.remove(group);
                    }else{
                        this->availableGroups.insert(group, count-1);
                    }
                }*/

                //delete feature
                //this->myFeatureUpdater.deleteFeature(delFeature);

            }
        }

        //recalc all features because some of the features maybe depent on the deleted features
        this->myFeatureUpdater.recalcFeatureSet();

        //refresh feature tree view models
        //this->featureTreeViewModel->refreshModel();

        //emit this->refreshGUI();
        //emit this->availableGroupsChanged(this->availableGroups);
        //emit this->resetFeatureSelection();

    }
}

/*!
 * \brief Controller::groupNameChanged
 * Group name of one feature was edited in table view
 * \param oldValue
 * \param newValue
 */
/*void Controller::groupNameChanged(QString oldValue, QString newValue){

    if(oldValue.compare(newValue) != 0){
        //count down by 1 the number of occurences of oldValue as group name
        if(oldValue.compare("") != 0 && this->availableGroups.contains(oldValue)){
            int count = this->availableGroups.find(oldValue).value();
            if(count <= 1){
                this->availableGroups.remove(oldValue);
            }else{
                this->availableGroups.insert(oldValue, count-1);
            }
        }

        //count up by 1 the number of occurences of newValue as group name
        if(newValue.compare("") != 0){
            int count = 0;
            if(this->availableGroups.contains(newValue)){
                count += this->availableGroups.find(newValue).value();
            }
            this->availableGroups.insert(newValue, count+1);
        }

        emit this->availableGroupsChanged(this->availableGroups);
    }

}*/

/*!
 * \brief Controller::checkAvailablePlugins
 * For each geometry type check wether there is a corresponding plugin to create it
 */
void Controller::checkAvailablePlugins(){
    //QStringList availableGeometries = SystemDbManager::getSupportedGeometries();
    //emit this->updateGeometryIcons(availableGeometries);
}

/*!
 * \brief Controller::checkPluginAvailability
 * Check wether there are one or more plugins for a specified feature type
 * \param typeOfFeature
 * \return
 */
bool Controller::checkPluginAvailability(Configuration::FeatureTypes typeOfFeature){
    QStringList availableGeometries = SystemDbManager::getSupportedGeometries();
    if(availableGeometries.contains(QString(Configuration::getFeatureTypeString(typeOfFeature)))){
        return true;
    }
    return false;
}

/*!
 * \brief Controller::getAvailableCreateFunctions
 * Returns a list of all available fit and create functions of the specified feature type
 * \param featureType
 * \return
 */
QStringList Controller::getAvailableCreateFunctions(Configuration::FeatureTypes featureType){
    QStringList result;

    //query database for all available fit and construct functions of featureType
    QList<FunctionPlugin> fitFunctions = SystemDbManager::getAvailableFitFunctions(featureType);
    QList<FunctionPlugin> createFunctions = SystemDbManager::getAvailableConstructFunctions(featureType);

    //add the function names to the result list
    result.append("");
    foreach(FunctionPlugin plugin, fitFunctions){
        result.append(QString("%1 [%2]").arg(plugin.name).arg(plugin.pluginName));
    }
    foreach(FunctionPlugin plugin, createFunctions){
        result.append(QString("%1 [%2]").arg(plugin.name).arg(plugin.pluginName));
    }

    return result;
}

/*!
 * \brief Controller::getDefaultFunction
 * Returns the default function of the specified feature type or an empty string if no default function is available
 * \param featureType
 * \return
 */
QString Controller::getDefaultFunction(Configuration::FeatureTypes featureType){
    QString result;

    FunctionPlugin plugin = SystemDbManager::getDefaultFunction(featureType);
    if(plugin.name.compare("") != 0){
        result = QString("%1 [%2]").arg(plugin.name).arg(plugin.pluginName);
    }

    return result;
}

FeatureUpdater *Controller::getFeatureUpdater()
{
    return &myFeatureUpdater;
}

/*!
 * \brief Controller::setUpFeatureGroupsModel
 */
void Controller::setUpFeatureGroupsModel(){
    try{

        QStringList myFeatureGroups;

        QList<QString> myGroups = OiFeatureState::getAvailableGroups().keys();

        myFeatureGroups.append("All Groups");
        foreach(QString group, myGroups){
            myFeatureGroups.append(group);
        }

        this->myFeatureGroupsModel->setStringList(myFeatureGroups);

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief Controller::setUpCoordinateSystemsModel
 */
void Controller::setUpCoordinateSystemsModel(){
    try{

        QStringList myCoordSys;

        //append coordinate system names
        foreach(CoordinateSystem *mySystem, OiFeatureState::getCoordinateSystems()){
            if(mySystem != NULL){
                myCoordSys.append(mySystem->getFeatureName());
            }
        }

        //append station system names
        foreach(Station *myStation, OiFeatureState::getStations()){
            if(myStation != NULL){
                myCoordSys.append(myStation->getFeatureName());
            }
        }

        this->myCoordinateSystemsModel->setStringList(myCoordSys);

        emit this->activeCoordinateSystemChanged();

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief Controller::setActiveGroup
 * \param group
 */
void Controller::setActiveGroup(QString group){
    OiFeatureState::setActiveGroup(group);
}
