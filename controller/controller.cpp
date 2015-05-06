#include "controller.h"

/*!
 * \brief Controller::Controller
 * \param parent
 */
Controller::Controller(QObject *parent) : QObject(parent){

    //register meta types
    this->registerMetaTypes();

    //initialize model manager
    ModelManager::init();

    //initialize display configs
    this->initDisplayConfigs();

    //initialize config manager
    this->initConfigManager();

    //connect helper objects
    this->connectDataExchanger();
    this->connectFeatureUpdater();

}

/*!
 * \brief Controller::addFeatures
 * \param attributes
 */
void Controller::addFeatures(const FeatureAttributes &attributes){

    //check job
    if(this->job.isNull()){
        Console::getInstance()->addLine("No active job");
        return;
    }

    this->job->addFeatures(attributes);

}

/*!
 * \brief Controller::sensorConfigurationChanged
 * \param name
 * \param connectSensor
 */
void Controller::sensorConfigurationChanged(const QString &name, const bool &connectSensor){

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //get and check the specified sensor config
    SensorConfiguration sConfig = this->sensorConfigManager->getSavedSensorConfig(name);
    if(!sConfig.getIsValid()){
        Console::getInstance()->addLine(QString("No sensor configuration available with the name %1").arg(name));
        return;
    }

    //get and check plugin information
    sdb::Plugin plugin = SystemDbManager::getPlugin(sConfig.getPluginName());
    if(plugin.id == -1){
        Console::getInstance()->addLine(QString("No plugin available with the name %1").arg(sConfig.getPluginName()));
        return;
    }

    //create sensor instance and assign it to the active station
    QPointer<Sensor> sensor = PluginLoader::loadSensorPlugin(plugin.file_path, sConfig.getSensorName());
    if(sensor.isNull()){
        Console::getInstance()->addLine(QString("No sensor available with the name %1").arg(sConfig.getSensorName()));
        return;
    }

    //set active station's sensor
    sensor->setSensorConfiguration(sConfig);
    activeStation->setSensor(sensor);

    Console::getInstance()->addLine("Active sensor changed");

}

/*!
 * \brief Controller::setActiveFeature
 * \param featureId
 */
/*void Controller::setActiveFeature(const int &featureId){

    //check job
    if(this->job.isNull()){
        Console::getInstance()->addLine("No active job");
        return;
    }

    //set active feature
    QPointer<FeatureWrapper> feature = this->job->getFeatureById(featureId);
    if(!feature.isNull() && !feature->getFeature().isNull()){
        feature->getFeature()->setActiveFeatureState(true);
    }else{
        Console::getInstance()->addLine(QString("No is no feature with the id %1 that could be activated").arg(featureId));
    }

}*/

/*!
 * \brief Controller::setActiveStation
 * \param featureId
 */
void Controller::setActiveStation(const int &featureId){

}

/*!
 * \brief Controller::setActiveCoordinateSystem
 * \param featureId
 */
void Controller::setActiveCoordinateSystem(const int &featureId){

}

/*!
 * \brief Controller::importNominals
 * \param params
 */
void Controller::importNominals(const ExchangeParams &params){

    //try to start import nominal task
    if(!this->exchanger.importData(params)){
        return;
    }

    //show loading dialog during import
    emit this->nominalImportStarted();

}

/*!
 * \brief Controller::setFeatureTableColumnConfig
 * \param config
 */
void Controller::setFeatureTableColumnConfig(const FeatureTableColumnConfig &config){
    ModelManager::setFeatureTableColumnConfig(config);
}

/*!
 * \brief Controller::setTrafoParamColumnConfig
 * \param config
 */
void Controller::setTrafoParamColumnConfig(const TrafoParamTableColumnConfig &config){
    ModelManager::setTrafoParamColumnConfig(config);
}

/*!
 * \brief Controller::setParameterDisplayConfig
 * \param config
 */
void Controller::setParameterDisplayConfig(const ParameterDisplayConfig &config){
    ModelManager::setParameterDisplayConfig(config);
}

/*!
 * \brief Controller::createDefaultJob
 */
void Controller::createDefaultJob(){

    //create job with a station and a nominal system
    QPointer<OiJob> job = new OiJob();
    this->setJob(job);

    QPointer<FeatureWrapper> stationFeature = new FeatureWrapper();
    QPointer<Station> station = new Station();
    station->setFeatureName("STATION01");
    stationFeature->setStation(station);

    QPointer<FeatureWrapper> systemFeature = new FeatureWrapper();
    QPointer<CoordinateSystem> system = new CoordinateSystem();
    system->setFeatureName("PART");
    systemFeature->setCoordinateSystem(system);

    job->addFeature(stationFeature);
    job->addFeature(systemFeature);

    //activate features
    station->setActiveStationState(true);
    station->getCoordinateSystem()->setActiveCoordinateSystemState(true);

}

/*!
 * \brief Controller::startConnect
 */
void Controller::startConnect(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("connecting sensor...");

    //connect sensor
    activeStation->connectSensor();

}

/*!
 * \brief Controller::startDisconnect
 */
void Controller::startDisconnect(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("disconnecting sensor...");

    //disconnect sensor
    activeStation->disconnectSensor();

}

/*!
 * \brief Controller::startMeasurement
 */
void Controller::startMeasurement(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> activeFeature = this->job->getActiveFeature();
    if(activeFeature.isNull() || activeFeature->getGeometry().isNull()){
        Console::getInstance()->addLine("No active feature");
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("performing measurement...");

    //perform measurement
    int id = activeFeature->getGeometry()->getId();
    MeasurementConfig mConfig;
    activeStation->measure(id, mConfig);

}

/*!
 * \brief Controller::startMove
 * \param reading
 */
void Controller::startMove(const Reading &reading){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> activeFeature = this->job->getActiveFeature();
    if(activeFeature.isNull() || activeFeature->getGeometry().isNull()){

    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("moving sensor...");

    //move sensor
    if(reading.getTypeOfReading() == eCartesianReading){
        activeStation->move(reading.getCartesianReading().xyz.getAt(0),
                            reading.getCartesianReading().xyz.getAt(1),
                            reading.getCartesianReading().xyz.getAt(2));
        return;
    }else if(reading.getTypeOfReading() == ePolarReading){
        activeStation->move(reading.getPolarReading().azimuth,
                            reading.getPolarReading().zenith,
                            reading.getPolarReading().distance, false);
        return;
    }

    emit this->sensorActionFinished(false, "Selected reading type is not supported");

}

/*!
 * \brief Controller::startAim
 */
void Controller::startAim(){

    //TODO implement transformation here

}

/*!
 * \brief Controller::startToggleSight
 */
void Controller::startToggleSight(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("toggling sight orientation...");

    //toggle sight
    activeStation->toggleSight();

}

/*!
 * \brief Controller::startInitialize
 */
void Controller::startInitialize(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("initializing sensor...");

    //initialize sensor
    activeStation->initialize();

}

/*!
 * \brief Controller::startHome
 */
void Controller::startHome(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("setting sensor to home position...");

    //set sensor to home position
    activeStation->home();

}

/*!
 * \brief Controller::startCompensation
 */
void Controller::startCompensation(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("starting compensation...");

    //start compensation
    activeStation->compensation();

}

/*!
 * \brief Controller::startChangeMotorState
 */
void Controller::startChangeMotorState(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("changing motor state...");

    //change motor state
    activeStation->motorState();

}

/*!
 * \brief Controller::startCustomAction
 * \param task
 */
void Controller::startCustomAction(const QString &task){
    //TODO implement custom action
}

/*!
 * \brief Controller::activeStationChangedCallback
 */
void Controller::activeStationChangedCallback(){

    //check job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = job->getActiveStation();
    if(activeStation.isNull()){
        return;
    }

    //connect sensor action results
    QObject::connect(activeStation.data(), &Station::commandFinished, this, &Controller::sensorActionFinished, Qt::AutoConnection);
    QObject::connect(activeStation.data(), &Station::measurementFinished, this, &Controller::measurementFinished, Qt::AutoConnection);

}

/*!
 * \brief Controller::measurementFinished
 * \param geomId
 * \param readings
 */
void Controller::measurementFinished(const int &geomId, const QList<QPointer<Reading> > &readings){

    //check job
    if(this->job.isNull()){
        return;
    }

    //add observations
    this->job->addMeasurementResults(geomId, readings);

}

/*!
 * \brief Controller::setJob
 * \param job
 */
void Controller::setJob(const QPointer<OiJob> &job){

    //check job
    if(job.isNull()){
        return;
    }

    //delete old job
    if(!this->job.isNull()){
        delete this->job;
    }

    //set and connect new job
    this->job = job;

    //log messages to console
    QObject::connect(this->job.data(), &OiJob::sendMessage, this, &Controller::logToConsole, Qt::AutoConnection);

    //active feature changes
    QObject::connect(this->job.data(), &OiJob::activeFeatureChanged, this, &Controller::activeFeatureChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::activeStationChanged, this, &Controller::activeStationChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::activeStationChanged, this, &Controller::activeStationChangedCallback, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::activeCoordinateSystemChanged, this, &Controller::activeCoordinateSystemChanged, Qt::AutoConnection);

    //feature(s) added or removed
    QObject::connect(this->job.data(), &OiJob::featureSetChanged, this, &Controller::featureSetChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::coordSystemSetChanged, this, &Controller::coordSystemSetChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::stationSetChanged, this, &Controller::stationSetChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::trafoParamSetChanged, this, &Controller::trafoParamSetChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometrySetChanged, this, &Controller::geometrySetChanged, Qt::AutoConnection);

    //group(s) added or removed
    QObject::connect(this->job.data(), &OiJob::availableGroupsChanged, this, &Controller::availableGroupsChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::activeGroupChanged, this, &Controller::activeGroupChanged, Qt::AutoConnection);

    //general feature attributes changed
    QObject::connect(this->job.data(), &OiJob::featureAttributesChanged, this, &Controller::featureAttributesChanged, Qt::AutoConnection);

    //feature specific attributes changed
    QObject::connect(this->job.data(), &OiJob::featureNameChanged,
                     this, &Controller::featureNameChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::featureGroupChanged,
                     this, &Controller::featureGroupChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::featureCommentChanged,
                     this, &Controller::featureCommentChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::featureIsUpdatedChanged,
                     this, &Controller::featureIsUpdatedChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::featureIsSolvedChanged,
                     this, &Controller::featureIsSolvedChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::featureFunctionsChanged,
                     this, &Controller::featureFunctionsChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::featureUsedForChanged,
                     this, &Controller::featureUsedForChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::featurePreviouslyNeededChanged,
                     this, &Controller::featurePreviouslyNeededChanged, Qt::AutoConnection);

    //geometry specific attributes changed
    QObject::connect(this->job.data(), &OiJob::geometryIsCommonChanged,
                     this, &Controller::geometryIsCommonChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometryNominalsChanged,
                     this, &Controller::geometryNominalsChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometryActualChanged,
                     this, &Controller::geometryActualChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometryObservationsChanged,
                     this, &Controller::geometryObservationsChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometryNominalSystemChanged,
                     this, &Controller::geometryNominalSystemChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometryStatisticChanged,
                     this, &Controller::geometryStatisticChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometrySimulationDataChanged,
                     this, &Controller::geometrySimulationDataChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometryMeasurementConfigChanged,
                     this, &Controller::geometryMeasurementConfigChanged, Qt::AutoConnection);

    //coordinate system specific attributes changed
    QObject::connect(this->job.data(), &OiJob::systemObservationsChanged,
                     this, &Controller::systemObservationsChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::systemTrafoParamsChanged,
                     this, &Controller::systemTrafoParamsChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::systemsNominalsChanged,
                     this, &Controller::systemsNominalsChanged, Qt::AutoConnection);

    //station specific attributes changed
    QObject::connect(this->job.data(), &OiJob::stationSensorChanged,
                     this, &Controller::stationSensorChanged, Qt::AutoConnection);

    //transformation parameter specific attributes changed
    QObject::connect(this->job.data(), &OiJob::trafoParamParametersChanged,
                     this, &Controller::trafoParamParametersChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::trafoParamSystemsChanged,
                     this, &Controller::trafoParamSystemsChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::trafoParamIsUsedChanged,
                     this, &Controller::trafoParamIsUsedChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::trafoParamValidTimeChanged,
                     this, &Controller::trafoParamValidTimeChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::trafoParamIsMovementChanged,
                     this, &Controller::trafoParamIsMovementChanged, Qt::AutoConnection);

    //pass the new job around
    ModelManager::setCurrentJob(this->job);
    this->exchanger.setCurrentJob(this->job);
    this->featureUpdater.setCurrentJob(this->job);

}

/*!
 * \brief Controller::initDisplayConfigs
 * Sets up initial display configs and passes them to model manager
 */
void Controller::initDisplayConfigs(){

    //create default configs
    FeatureTableColumnConfig featureTableColumnConfig;
    TrafoParamTableColumnConfig trafoParamTableColumnConfig;
    ParameterDisplayConfig parameterDisplayConfig;

    //pass the default configs to model manager
    ModelManager::setFeatureTableColumnConfig(featureTableColumnConfig);
    ModelManager::setTrafoParamColumnConfig(trafoParamTableColumnConfig);
    ModelManager::setParameterDisplayConfig(parameterDisplayConfig);

}

/*!
 * \brief Controller::initConfigManager
 */
void Controller::initConfigManager(){

    //load configs from config folder
    this->sensorConfigManager = new SensorConfigurationManager();
    this->sensorConfigManager->loadFromConfigFolder();

    //pass config managers to model manager
    ModelManager::setSensorConfigManager(this->sensorConfigManager);

}

/*!
 * \brief Controller::logToConsole
 * \param msg
 */
void Controller::logToConsole(const QString &msg){
    Console::getInstance()->addLine(msg);
}

/*!
 * \brief Controller::registerMetaTypes
 * Registers meta types to be able to use them in signal slot connections
 */
void Controller::registerMetaTypes(){

    qRegisterMetaType<MeasurementConfig>();
    qRegisterMetaType<SensorConfiguration>();
    qRegisterMetaType<QList<QPointer<Reading> > >();
    qRegisterMetaType<QPointer<Feature> >();
    qRegisterMetaType<QPointer<Function> >();
    qRegisterMetaType<QPointer<FeatureWrapper> >();
    qRegisterMetaType<QPointer<Observation> >();

}

/*!
 * \brief Controller::connectDataExchanger
 */
void Controller::connectDataExchanger(){

    QObject::connect(&this->exchanger, &DataExchanger::importFinished, this, &Controller::nominalImportFinished, Qt::AutoConnection);
    QObject::connect(&this->exchanger, &DataExchanger::updateProgress, this, &Controller::nominalImportProgressUpdated, Qt::AutoConnection);

}

/*!
 * \brief Controller::connectFeatureUpdater
 */
void Controller::connectFeatureUpdater(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    QObject::connect(&this->featureUpdater, &FeatureUpdater::featureRecalculated, this->job.data(), &OiJob::featureRecalculated, Qt::AutoConnection);
    QObject::connect(&this->featureUpdater, &FeatureUpdater::featuresRecalculated, this->job.data(), &OiJob::featuresRecalculated, Qt::AutoConnection);
    QObject::connect(&this->featureUpdater, &FeatureUpdater::trafoParamRecalculated, this->job.data(), &OiJob::trafoParamRecalculated, Qt::AutoConnection);

    QObject::connect(this->job.data(), &OiJob::recalcFeature, &this->featureUpdater, &FeatureUpdater::recalcFeature, Qt::AutoConnection);

}
