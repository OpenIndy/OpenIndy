#include "controller.h"

/*!
 * \brief Controller::Controller
 * \param parent
 */
Controller::Controller(QObject *parent) : QObject(parent){

    //register meta types
    this->registerMetaTypes();

    //initialize and connect model manager
    ModelManager::init();
    if(!ModelManager::myInstance.isNull()){
        QObject::connect(ModelManager::myInstance.data(), &ModelManager::sendMessage, this, &Controller::log, Qt::AutoConnection);
    }

    //initialize display configs
    this->initDisplayConfigs();

    //initialize config manager
    this->initConfigManager();

    //init and start OpenIndy server
    this->initServer();
    this->startServer();

    //initialize tool plugins
    this->initToolPlugins();

    //connect helper objects
    this->connectDataExchanger();
    this->connectFeatureUpdater();
    this->connectRequestHandler();

}

/*!
 * \brief Controller::~Controller
 */
Controller::~Controller(){

    //stop web socket server thread
    if(this->serverThread.isRunning()){
        this->serverThread.quit();
        this->serverThread.wait();
    }

}

/*!
 * \brief Controller::getAvailableTools
 * \return
 */
const QList<QPointer<Tool> > &Controller::getAvailableTools() const{
    return this->toolPlugins;
}

/*!
 * \brief Controller::addFeatures
 * \param attributes
 */
void Controller::addFeatures(const FeatureAttributes &attributes){

    //check job
     if(this->job.isNull()){
        this->log("No active job", eErrorMessage, eMessageBoxMessage);
        return;
    }

    QList<QPointer<FeatureWrapper> > features = this->job->addFeatures(attributes);
    if(features.size() == 0){
        this->log("No feature were created - See console output for details", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get saved measurement config
    MeasurementConfig mConfig = this->measurementConfigManager->getSavedMeasurementConfig(attributes.mConfig);

    //check if there is at least a function or a measurement config that shall be set
    if(!mConfig.getIsValid() && (attributes.functionPlugin.first.compare("") == 0
                                 || attributes.functionPlugin.second.compare("") == 0)){
        return;
    }

    //create functions and measurement configs for the created features
    this->addFunctionsAndMConfigs(features, mConfig, attributes.functionPlugin.second, attributes.functionPlugin.first);

}

/*!
 * \brief Controller::removeFeatures
 * \param featureIds
 */
void Controller::removeFeatures(const QSet<int> &featureIds){

    //check job
    if(this->job.isNull()){
        this->log("No active job", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //remove features
    if(!this->job->removeFeatures(featureIds)){
        this->log("Error while removeing features", eErrorMessage, eMessageBoxMessage);
    }

}

/*!
 * \brief Controller::setNominalParameters
 * \param featureId
 * \param parameters
 */
void Controller::setNominalParameters(const int &featureId, const QMap<GeometryParameters, double> &parameters){

    //check job
    if(this->job.isNull()){
        this->log("No active job", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get and check the feature by its id
    QPointer<FeatureWrapper> feature = this->job->getFeatureById(featureId);
    if(feature.isNull() || feature->getGeometry().isNull()){
        this->log(QString("No geometry with the id %1").arg(featureId), eErrorMessage, eMessageBoxMessage);
        return;
    }

    //update the feature's parameters
    feature->getGeometry()->setUnknownParameters(parameters);

}

/*!
 * \brief Controller::setTrafoParamParameters
 * \param featureId
 * \param parameters
 */
void Controller::setTrafoParamParameters(const int &featureId, const QMap<TrafoParamParameters, double> &parameters){

    //check job
    if(this->job.isNull()){
        this->log("No active job", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get and check the feature by its id
    QPointer<FeatureWrapper> feature = this->job->getFeatureById(featureId);
    if(feature.isNull() || feature->getTrafoParam().isNull()){
        this->log(QString("No trafo param with the id %1").arg(featureId), eErrorMessage, eMessageBoxMessage);
        return;
    }

    //update the feature's parameters
    feature->getTrafoParam()->setUnknownParameters(parameters);
    feature->getTrafoParam()->setIsSolved(true);

}

/*!
 * \brief Controller::recalcActiveFeature
 */
void Controller::recalcActiveFeature(){

    //check job
    if(this->job.isNull()){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> feature = this->job->getActiveFeature();
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    //recalculate the active feature
    if(!feature->getTrafoParam().isNull()){
        this->featureUpdater.recalcTrafoParam(feature->getTrafoParam());
    }else{
        this->featureUpdater.recalcFeature(feature->getFeature());
    }

}

/*!
 * \brief Controller::sensorConfigurationChanged
 * Set a new sensor configuration for the active sensor
 * \param name
 * \param connectSensor
 */
void Controller::setSensorConfig(const SensorConfiguration &sConfig, bool connectSensor){

    //check job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get and check the specified sensor config
    if(!sConfig.getIsValid()){
        this->log("Invalid sensor configuration selected", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get and check plugin information
    sdb::Plugin plugin = SystemDbManager::getPlugin(sConfig.getPluginName());
    if(plugin.id == -1){
        this->log(QString("No plugin available with the name %1").arg(sConfig.getPluginName()), eErrorMessage, eMessageBoxMessage);
        return;
    }

    //create sensor instance and assign it to the active station
    QPointer<Sensor> sensor = PluginLoader::loadSensorPlugin(plugin.file_path, sConfig.getSensorName());
    if(sensor.isNull()){
        this->log(QString("No sensor available with the name %1").arg(sConfig.getSensorName()), eErrorMessage, eMessageBoxMessage);
        return;
    }

    //set active station's sensor
    sensor->setSensorConfiguration(sConfig);
    activeStation->setSensor(sensor);

    //connect the sensor
    if(connectSensor){
        this->startConnect();
    }

    //update the active sensor config of the config manager
    if(!this->sensorConfigManager.isNull()){
        this->sensorConfigManager->setActiveSensorConfig(sConfig);
    }

}

/*!
 * \brief Controller::removeActiveStationSensor
 * Resets the sensor of the active station
 */
void Controller::removeActiveStationSensor(){

    //check job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> station = this->job->getActiveStation();
    if(station.isNull()){
        return;
    }

    //reset the sensor of the active station
    station->resetSensor();

}

/*!
 * \brief Controller::sensorConfigurationsEdited
 * Synchronize the sensor config manager with the given one
 * \param manager
 */
void Controller::sensorConfigurationsEdited(const SensorConfigurationManager &manager){

    //check job
    if(this->job.isNull()){
        return;
    }

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return;
    }

    //synchronize the managers
    this->sensorConfigManager->synchronize(manager);

}

/*!
 * \brief Controller::sensorConfigurationUpdated
 * Update the current sensor configuration of the active station
 * \param sConfig
 */
void Controller::sensorConfigurationUpdated(const SensorConfiguration &sConfig){

    //check job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //set sensor configuration
    activeStation->setSensorConfiguration(sConfig);

}

/*!
 * \brief Controller::measurementConfigurationChanged
 * \param mConfig
 */
void Controller::measurementConfigurationChanged(const MeasurementConfig &mConfig){

    //check job
    if(this->job.isNull()){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> activeFeature = this->job->getActiveFeature();
    if(activeFeature.isNull() || activeFeature->getGeometry().isNull()){
        this->log("No active geometry selected", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get and check the specified measurement config
    if(!mConfig.getIsValid()){
        this->log("No measurement configuration selected", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //set measurement config for the active feature
    activeFeature->getGeometry()->getMyMasterGeometry()->setMeasurementConfig(mConfig);

}

/*!
 * \brief Controller::setActiveStation
 * \param featureId
 */
/*void Controller::setActiveStation(const int &featureId){

}*/

/*!
 * \brief Controller::setActiveCoordinateSystem
 * \param featureId
 */
/*void Controller::setActiveCoordinateSystem(const int &featureId){

}*/

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
 * \brief Controller::importObservations
 * \param filename
 */
void Controller::importObservations(const QString &filename){

    //try to start import observations task
    if(!this->exchanger.importObservations(filename)){
        return;
    }

    //show loading dialog during import
    emit this->observationImportStarted();

}

/*!
 * \brief Controller::removeObservations
 * \param featureId
 */
void Controller::removeObservations(const int &featureId){

    //check job
    if(this->job.isNull()){
        this->log("No job available", eErrorMessage, eMessageBoxMessage);
        return;
    }

    this->job->removeObservations(featureId);

}

/*!
 * \brief Controller::removeAllObservations
 */
void Controller::removeAllObservations(){

    //check job
    if(this->job.isNull()){
        this->log("No job available", eErrorMessage, eMessageBoxMessage);
        return;
    }

    this->job->removeAllObservations();

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
 * \brief Controller::setObservationTableColumnConfig
 * \param config
 */
void Controller::setObservationTableColumnConfig(const ObservationTableColumnConfig &config){
    ModelManager::setObservationTableColumnConfig(config);
}

/*!
 * \brief Controller::setReadingTableColumnConfig
 * \param config
 */
void Controller::setReadingTableColumnConfig(const ReadingTableColumnConfig &config){
    ModelManager::setReadingTableColumnConfig(config);
}

/*!
 * \brief Controller::setParameterDisplayConfig
 * \param config
 */
void Controller::setParameterDisplayConfig(const ParameterDisplayConfig &config){
    ModelManager::setParameterDisplayConfig(config);
    emit this->updateStatusBar();
}

/*!
 * \brief Controller::addBundleSystem
 */
void Controller::addBundleSystem(){

    //check job
    if(this->job.isNull()){
        this->log("No job available", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //create feature name
    QString bundleName = "Bundle01";
    int index = 1;
    while(!this->job->validateFeatureName(bundleName, eCoordinateSystemFeature)){
        if(index < 9){
            bundleName = QString("Bundle0%1").arg(++index);
        }else{
            bundleName = QString("Bundle%1").arg(++index);
        }
    }

    //create feature attributes
    FeatureAttributes attr;
    attr.count = 1;
    attr.typeOfFeature = eCoordinateSystemFeature;
    attr.name = bundleName;
    attr.isBundleSystem = true;

    //add feature
    this->job->addFeatures(attr);

}

/*!
 * \brief Controller::removeBundleSystem
 * \param id
 */
void Controller::removeBundleSystem(const int &id){

    //check job
    if(this->job.isNull()){
        this->log("No job available", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //remove bundle system
    this->job->removeFeature(id);

}

/*!
 * \brief Controller::getBundleTemplate
 * \param bundleId
 * \return
 */
QJsonObject Controller::getBundleTemplate(const int &bundleId){

    //check job
    if(this->job.isNull()){
        this->log("No job available", eErrorMessage, eMessageBoxMessage);
        return QJsonObject();
    }

    //get and check feature
    QPointer<FeatureWrapper> feature = this->job->getFeatureById(bundleId);
    if(feature.isNull() || feature->getCoordinateSystem().isNull()){
        return QJsonObject();
    }
    QPointer<CoordinateSystem> bundleSystem = feature->getCoordinateSystem();

    return bundleSystem->getBundleTemplate();

}

/*!
 * \brief Controller::getBundleAdjustment
 * Returns the bundle plugin of the coordinate system defined by bundleId
 * \param bundleId
 * \return
 */
QPointer<oi::BundleAdjustment> Controller::getBundleAdjustment(const int &bundleId){

    //check job
    if(this->job.isNull()){
        this->log("No job available", eErrorMessage, eMessageBoxMessage);
        return QPointer<oi::BundleAdjustment>(NULL);
    }

    //get and check feature
    QPointer<FeatureWrapper> feature = this->job->getFeatureById(bundleId);
    if(feature.isNull() || feature->getCoordinateSystem().isNull()){
        return QPointer<oi::BundleAdjustment>(NULL);
    }
    QPointer<CoordinateSystem> bundleSystem = feature->getCoordinateSystem();

    return bundleSystem->getBundlePlugin();

}

/*!
 * \brief Controller::updateBundleAdjustment
 * Updates the bundle plugin of the coordinate system defined by bundleId
 * \param bundleId
 * \param param
 */
void Controller::updateBundleAdjustment(const int &bundleId, const QJsonObject &param){

    //check job
    if(this->job.isNull()){
        this->log("No job available", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get and check feature
    QPointer<FeatureWrapper> feature = this->job->getFeatureById(bundleId);
    if(feature.isNull() || feature->getCoordinateSystem().isNull()){
        return;
    }
    QPointer<CoordinateSystem> bundleSystem = feature->getCoordinateSystem();

    //get and check bundle plugin
    QPointer<BundleAdjustment> bundlePlugin = bundleSystem->getBundlePlugin();
    if(bundlePlugin.isNull()){
        return;
    }

    //update scalar parameters
    ScalarInputParams scalarParams;
    QJsonArray params = param.value("integerParameter").toArray();
    for(int i = 0; i < params.size(); i++){
        QJsonObject intParam = params.at(i).toObject();
        if(intParam.contains("name") && intParam.contains("value")){
            scalarParams.intParameter.insert(intParam.value("name").toString(), intParam.value("value").toInt());
        }
    }
    params = param.value("doubleParameter").toArray();
    for(int i = 0; i < params.size(); i++){
        QJsonObject doubleParam = params.at(i).toObject();
        if(doubleParam.contains("name") && doubleParam.contains("value")){
            scalarParams.doubleParameter.insert(doubleParam.value("name").toString(), doubleParam.value("value").toDouble());
        }
    }
    params = param.value("stringParameter").toArray();
    for(int i = 0; i < params.size(); i++){
        QJsonObject stringParam = params.at(i).toObject();
        if(stringParam.contains("name") && stringParam.contains("value")){
            scalarParams.stringParameter.insert(stringParam.value("name").toString(), stringParam.value("value").toString());
        }
    }

    //update input stations
    QList<BundleStation> inputStations;
    params = param.value("inputStations").toArray();
    for(int i = 0; i < params.size(); i++){
        QJsonObject station = params.at(i).toObject();
        if(!station.contains("id") || !station.contains("used")
                || !station.value("used").toBool()){
            continue;
        }
        BundleStation bundleStation;
        bundleStation.id = station.value("id").toInt();
        bundleStation.tx = station.value("tx").toBool();
        bundleStation.ty = station.value("ty").toBool();
        bundleStation.tz = station.value("tz").toBool();
        bundleStation.rx = station.value("rx").toBool();
        bundleStation.ry = station.value("ry").toBool();
        bundleStation.rz = station.value("rz").toBool();
        bundleStation.m = station.value("m").toBool();
        inputStations.append(bundleStation);
    }

    //set up input parameters
    bundlePlugin->setScalarInputParams(scalarParams);
    bundlePlugin->setInputStations(inputStations);

}

/*!
 * \brief Controller::loadBundleTemplate
 * \param bundleId
 * \param bundleTemplate
 */
void Controller::loadBundleTemplate(const int &bundleId, const QJsonObject &bundleTemplate){

    //check job
    if(this->job.isNull()){
        this->log("No job available", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get bundle system
    QPointer<FeatureWrapper> feature = this->job->getFeatureById(bundleId);
    if(feature.isNull() || feature->getCoordinateSystem().isNull()){
        this->log(QString("No bundle system with id %1").arg(bundleId), eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get bundle and plugin name
    QString bundleName, pluginName;
    bundleName = bundleTemplate.value("plugin").toObject().value("name").toString();
    pluginName = bundleTemplate.value("plugin").toObject().value("pluginName").toString();

    //get and check plugin information
    sdb::Plugin plugin = SystemDbManager::getPlugin(pluginName);
    if(plugin.id == -1){
        this->log(QString("No plugin available with the name %1").arg(pluginName), eErrorMessage, eMessageBoxMessage);
        return;
    }

    //load bundle plugin
    QPointer<BundleAdjustment> bundlePlugin(NULL);
    bundlePlugin = PluginLoader::loadBundleAdjustmentPlugin(plugin.file_path, bundleName);
    if(bundlePlugin.isNull()){
        this->log(QString("Cannot load bundle template %1").arg(bundleTemplate.value("name").toString()), eErrorMessage, eMessageBoxMessage);
        return;
    }

    //set up bundle plugin
    feature->getCoordinateSystem()->setBundleTemplate(bundleTemplate);
    feature->getCoordinateSystem()->setBundlePlugin(bundlePlugin);

    this->log(QString("Bundle template %1 loaded successfully").arg(bundleTemplate.value("name").toString()), eInformationMessage, eMessageBoxMessage);

}

/*!
 * \brief Controller::runBundle
 * \param bundleId
 */
void Controller::runBundle(const int &bundleId){

    //check job
    if(this->job.isNull()){
        this->log("No job available", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get bundle system
    QPointer<FeatureWrapper> feature = this->job->getFeatureById(bundleId);
    if(feature.isNull() || feature->getCoordinateSystem().isNull()){
        this->log(QString("No bundle system with id %1").arg(bundleId), eErrorMessage, eMessageBoxMessage);
        return;
    }
    QPointer<CoordinateSystem> bundleSystem = feature->getCoordinateSystem();

    //check bundle plugin
    if(bundleSystem->getBundlePlugin().isNull()){
        this->log(QString("No bundle plugin loaded for bundle %1").arg(bundleSystem->getFeatureName()), eErrorMessage, eMessageBoxMessage);
        return;
    }

    //calculate bundle adjustment
    if(!this->featureUpdater.recalcBundle(bundleSystem)){
        this->log(QString("Error when calculating bundle %1").arg(bundleSystem->getFeatureName()), eErrorMessage, eMessageBoxMessage);
        return;
    }
    this->log(QString("Bundle %1 successfully calculated").arg(bundleSystem->getFeatureName()), eInformationMessage, eMessageBoxMessage);

}

/*!
 * \brief Controller::saveProject
 */
void Controller::saveProject(){

    //check job
    if(this->job.isNull()){
        this->log("No job available", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get and check name and file path
    QString name = this->job->getJobName();
    QPointer<QIODevice> device = this->job->getJobDevice();
    if(name.compare("") == 0 || device.isNull()){
        emit this->saveAsTriggered();
        return;
    }

    //get project xml
    QDomDocument project = ProjectExchanger::saveProject(this->job);
    if(project.isNull()){
        this->log("Error while creating project XML", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //save project xml
    bool isOpen = device->open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    if(isOpen){
        QTextStream stream(device);
        project.save(stream, 4);
        device->close();
    }else{
        this->log(QString("Cannot open file %1").arg(name), eInformationMessage, eStatusBarMessage);
    }

    this->log("OpenIndy project successfully stored.", eInformationMessage, eStatusBarMessage);

}

/*!
 * \brief Controller::saveProject
 * \param fileName
 */
void Controller::saveProject(const QString &fileName){

    //check job
    if(this->job.isNull()){
        this->log("No job available", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //set name
    QFileInfo info(fileName);
    this->job->setJobName(info.fileName());

    //set device
    QPointer<QIODevice> device = new QFile(fileName);
    this->job->setJobDevice(device);

    //get project xml
    QDomDocument project = ProjectExchanger::saveProject(this->job);
    if(project.isNull()){
        this->log("Error while creating project XML", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //save project xml
    bool isOpen = this->job->getJobDevice()->open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    if(isOpen){
        QTextStream stream(this->job->getJobDevice());
        project.save(stream, 4);
        this->job->getJobDevice()->close();
    }else{
        this->log(QString("Cannot open file %1").arg(info.fileName()), eInformationMessage, eStatusBarMessage);
    }

    emit this->currentJobChanged();

    this->log("OpenIndy project successfully stored.", eInformationMessage, eStatusBarMessage);

}

/*!
 * \brief Controller::loadProject
 * \param projectName
 * \param device
 */
void Controller::loadProject(const QString &projectName, const QPointer<QIODevice> &device){

    //check device
    if(device.isNull()){
        this->log("No device", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //create empty job
    QPointer<OiJob> newJob(NULL);

    //load xml file to DOM tree
    QDomDocument project;
    try{
        device->open(QIODevice::ReadOnly);
        project.setContent(device);
        device->close();
    }catch(const exception &e){
        this->log("Error while opening OpenIndy xml file.", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //load project from xml
    newJob = ProjectExchanger::loadProject(project);

    //check job
    if(newJob.isNull()){
        this->log("Error while parsing OpenIndy xml file.", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //set project meta data
    newJob->setJobDevice(device);
    newJob->setJobName(projectName);

    //delete old job
    if(!this->job.isNull()){
        delete this->job;
    }

    //set new job
    this->setJob(newJob);

    //switch to active coordinate system
    this->featureUpdater.switchCoordinateSystem();

    //connect active station
    this->activeStationChangedCallback();

    this->log("OpenIndy project successfully loaded.", eInformationMessage, eConsoleMessage);

}

/*!
 * \brief Controller::createDefaultJob
 * \return
 */
const QPointer<OiJob> &Controller::createDefaultJob(){

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

    return this->job;

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
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
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
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
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
        this->log("No active feature", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //check sensor
    if(!activeStation->getIsSensorConnected()){
        this->log("No sensor connected to the active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //create actual from nominal (if a nominal is selected)
    if(activeFeature->getGeometry()->getIsNominal()){
        if(!this->createActualFromNominal(activeFeature->getGeometry())
                || activeFeature->getGeometry()->getMyMasterGeometry()->getActual().isNull()
                || activeFeature->getGeometry()->getMyMasterGeometry()->getActual()->getFeatureWrapper().isNull()){
            this->log("Cannot create actual for nominal", eErrorMessage, eMessageBoxMessage);
            return;
        }
        activeFeature = activeFeature->getGeometry()->getMyMasterGeometry()->getActual()->getFeatureWrapper();
        activeFeature->getFeature()->setActiveFeatureState(true);
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("performing measurement...");

    //perform measurement
    int id = activeFeature->getGeometry()->getId();
    MeasurementConfig mConfig = activeFeature->getGeometry()->getMyMasterGeometry()->getMeasurementConfig();
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
        this->log("No active feature", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //check sensor
    if(!activeStation->getIsSensorConnected()){
        this->log("No sensor connected to the active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("moving sensor...");

    //move sensor
    if(reading.getTypeOfReading() == eCartesianReading){
        activeStation->move(reading.getCartesianReading().xyz.getAt(0),
                            reading.getCartesianReading().xyz.getAt(1),
                            reading.getCartesianReading().xyz.getAt(2), false);
        return;
    }else if(reading.getTypeOfReading() == ePolarReading){
        activeStation->move(reading.getPolarReading().azimuth,
                            reading.getPolarReading().zenith,
                            reading.getPolarReading().distance, false, false);
        return;
    }

    emit this->sensorActionFinished(false, "Selected reading type is not supported");

}

/*!
 * \brief Controller::startAim
 */
void Controller::startAim(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> activeFeature = this->job->getActiveFeature();
    if(activeFeature.isNull() || activeFeature->getGeometry().isNull()){
        this->log("No active feature", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get and check active coordinate system
    QPointer<CoordinateSystem> activeCoordinateSystem = this->job->getActiveCoordinateSystem();
    if(activeCoordinateSystem.isNull()){
        this->log("No active coordinate system", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull() || activeStation->getCoordinateSystem().isNull()){
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //check sensor
    if(!activeStation->getIsSensorConnected()){
        this->log("No sensor connected to the active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //transform position of the active feature into the station coordinate system
    if(!activeFeature->getGeometry()->hasPosition()){
        this->log("Active feature has no position to aim", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //create trafo controller instance to get the transformation matrix
    TrafoController trafoController;
    OiMat t(4, 4);
    OiVec pos;

    //transform the active feature into the station coordinate system
    //switch between actual and nominal if the active feature is not solved
    if(activeFeature->getGeometry()->getIsNominal()){ //nominal

        //transform the nominal into the station coordinate system
        if(trafoController.getTransformationMatrix(t, activeFeature->getGeometry()->getNominalSystem(), activeStation->getCoordinateSystem())){
            pos = activeFeature->getGeometry()->getPosition().getVectorH();
            pos = t * pos;
        }else if(!activeFeature->getGeometry()->getMyMasterGeometry()->getActual().isNull() && activeFeature->getGeometry()->getMyMasterGeometry()->getActual()->hasPosition()
                 && activeFeature->getGeometry()->getMyMasterGeometry()->getActual()->getIsSolved()
                 && trafoController.getTransformationMatrix(t, activeCoordinateSystem, activeStation->getCoordinateSystem())){
            pos = activeFeature->getGeometry()->getMyMasterGeometry()->getActual()->getPosition().getVectorH();
            pos = t * pos;
        }

    }else{ //actual

        //transform the actual into the station coordinate system
        if(activeFeature->getGeometry()->getIsSolved() && trafoController.getTransformationMatrix(t, activeCoordinateSystem, activeStation->getCoordinateSystem())){
            pos = activeFeature->getGeometry()->getPosition().getVectorH();
            pos = t * pos;
        }else if(activeFeature->getGeometry()->getMyMasterGeometry()->getNominals().size() > 0){

            //use nominal instead of actual
            foreach(const QPointer<Geometry> &nominal, activeFeature->getGeometry()->getMyMasterGeometry()->getNominals()){
                if(nominal->hasPosition() && trafoController.getTransformationMatrix(t, nominal->getNominalSystem(), activeStation->getCoordinateSystem())){
                    pos = nominal->getPosition().getVectorH();
                    pos = t * pos;
                    break;
                }
            }

        }

    }

    //aim the active feature if a valid position has been found
    if(pos.getSize() == 4){
        emit this->sensorActionStarted("moving sensor...");
        activeStation->move(pos.getAt(0), pos.getAt(1), pos.getAt(2), false);
    }

}

/*!
 * \brief Controller::startAimAndMeasure
 */
void Controller::startAimAndMeasure(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> activeFeature = this->job->getActiveFeature();
    if(activeFeature.isNull() || activeFeature->getGeometry().isNull()){
        this->log("No active feature", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get and check active coordinate system
    QPointer<CoordinateSystem> activeCoordinateSystem = this->job->getActiveCoordinateSystem();
    if(activeCoordinateSystem.isNull()){
        this->log("No active coordinate system", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull() || activeStation->getCoordinateSystem().isNull()){
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //check sensor
    if(!activeStation->getIsSensorConnected()){
        this->log("No sensor connected to the active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //transform position of the active feature into the station coordinate system
    if(!activeFeature->getGeometry()->hasPosition()){
        this->log("Active feature has no position to aim", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //create trafo controller instance to get the transformation matrix
    TrafoController trafoController;
    OiMat t(4, 4);
    OiVec pos;

    //transform the active feature into the station coordinate system
    //switch between actual and nominal if the active feature is not solved
    if(activeFeature->getGeometry()->getIsNominal()){ //nominal

        //transform the nominal into the station coordinate system
        if(trafoController.getTransformationMatrix(t, activeFeature->getGeometry()->getNominalSystem(), activeStation->getCoordinateSystem())){
            pos = activeFeature->getGeometry()->getPosition().getVectorH();
            pos = t * pos;
        }else if(!activeFeature->getGeometry()->getMyMasterGeometry()->getActual().isNull() && activeFeature->getGeometry()->getMyMasterGeometry()->getActual()->hasPosition()
                 && activeFeature->getGeometry()->getMyMasterGeometry()->getActual()->getIsSolved()
                 && trafoController.getTransformationMatrix(t, activeCoordinateSystem, activeStation->getCoordinateSystem())){
            pos = activeFeature->getGeometry()->getMyMasterGeometry()->getActual()->getPosition().getVectorH();
            pos = t * pos;
        }

    }else{ //actual

        //transform the actual into the station coordinate system
        if(activeFeature->getGeometry()->getIsSolved() && trafoController.getTransformationMatrix(t, activeCoordinateSystem, activeStation->getCoordinateSystem())){
            pos = activeFeature->getGeometry()->getPosition().getVectorH();
            pos = t * pos;
        }else if(activeFeature->getGeometry()->getMyMasterGeometry()->getNominals().size() > 0){

            //use nominal instead of actual
            foreach(const QPointer<Geometry> &nominal, activeFeature->getGeometry()->getMyMasterGeometry()->getNominals()){
                if(nominal->hasPosition() && trafoController.getTransformationMatrix(t, nominal->getNominalSystem(), activeStation->getCoordinateSystem())){
                    pos = nominal->getPosition().getVectorH();
                    pos = t * pos;
                    break;
                }
            }

        }

    }

    //aim the active feature if a valid position has been found
    if(pos.getSize() == 4){
        emit this->sensorActionStarted("moving sensor...");
        activeStation->move(pos.getAt(0), pos.getAt(1), pos.getAt(2), true, activeFeature->getGeometry()->getId(),
                            activeFeature->getGeometry()->getMyMasterGeometry()->getMeasurementConfig());
    }

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
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //check sensor
    if(!activeStation->getIsSensorConnected()){
        this->log("No sensor connected to the active station", eErrorMessage, eMessageBoxMessage);
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
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //check sensor
    if(!activeStation->getIsSensorConnected()){
        this->log("No sensor connected to the active station", eErrorMessage, eMessageBoxMessage);
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
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //check sensor
    if(!activeStation->getIsSensorConnected()){
        this->log("No sensor connected to the active station", eErrorMessage, eMessageBoxMessage);
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
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //check sensor
    if(!activeStation->getIsSensorConnected()){
        this->log("No sensor connected to the active station", eErrorMessage, eMessageBoxMessage);
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
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //check sensor
    if(!activeStation->getIsSensorConnected()){
        this->log("No sensor connected to the active station", eErrorMessage, eMessageBoxMessage);
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

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //check sensor
    if(!activeStation->getIsSensorConnected()){
        this->log("No sensor connected to the active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted(QString("custom action: %1 ...").arg(task));

    //do self defined action
    activeStation->selfDefinedAction(task);

}

/*!
 * \brief Controller::startWatchWindow
 * \param streamFormat
 */
void Controller::startWatchWindow(ReadingTypes streamFormat){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //check sensor
    if(!activeStation->getIsSensorConnected()){
        this->log("No sensor connected to the active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //start streaming
    activeStation->setStreamFormat(streamFormat);
    activeStation->startReadingStream();

}

/*!
 * \brief Controller::stopWatchWindow
 */
void Controller::stopWatchWindow(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //check sensor
    if(!activeStation->getIsSensorConnected()){
        this->log("No sensor connected to the active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //stop streaming
    activeStation->stopReadingStream();

}

/*!
 * \brief Controller::log
 * Prints a message to the specified destination
 * \param msg
 * \param msgType
 * \param msgDest
 */
void Controller::log(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest){

    switch(msgDest){
    case eConsoleMessage:
        Console::getInstance()->addLine(msg, msgType);
        break;
    case eMessageBoxMessage:
        Console::getInstance()->addLine(msg, msgType);
        emit this->showMessageBox(msg, msgType);
        break;
    case eStatusBarMessage:
        Console::getInstance()->addLine(msg, msgType);
        emit this->showStatusMessage(msg, msgType);
        break;
    }

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

    //disconnect all stations
    QList<QPointer<Station> > stations = this->job->getStationsList();
    foreach(const QPointer<Station> &station, stations){

        //check station
        if(station.isNull()){
            continue;
        }

        //disconnect all slots from signals
        QObject::disconnect(station, &Station::commandFinished, 0, 0);
        QObject::disconnect(station, &Station::measurementFinished, 0, 0);

    }

    //connect sensor action results of active station
    QObject::connect(activeStation, &Station::commandFinished, this, &Controller::sensorActionFinished, Qt::AutoConnection);
    QObject::connect(activeStation, &Station::measurementFinished, this, &Controller::measurementFinished, Qt::AutoConnection);

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

    //get and check feature with the id geomId
    QPointer<FeatureWrapper> feature = this->job->getFeatureById(geomId);
    if(feature.isNull() || feature->getGeometry().isNull()){
        foreach(const QPointer<Reading> &reading, readings){
            if(!reading.isNull()){
                delete reading;
            }
        }
        return;
    }

    //if the feature is a nominal add the observation to its actual
    if(feature->getGeometry()->getIsNominal()){

        //check actual
        if(!feature->getGeometry()->getMyMasterGeometry()->getActual().isNull() && !feature->getGeometry()->getMyMasterGeometry()->getActual()->getFeatureWrapper().isNull()
                && !feature->getGeometry()->getMyMasterGeometry()->getActual()->getFeatureWrapper()->getGeometry().isNull()){ //has actual
            feature = feature->getGeometry()->getMyMasterGeometry()->getActual()->getFeatureWrapper();
        }else if(feature->getGeometry()->getMyMasterGeometry()->getActual().isNull()){ //does not have an actual

            //create a new feature
            FeatureAttributes attributes;
            attributes.count = 1;
            attributes.group = feature->getFeature()->getGroupName();
            attributes.name = feature->getFeature()->getFeatureName();
            attributes.typeOfFeature = feature->getFeatureTypeEnum();
            QList<QPointer<FeatureWrapper> > features = this->job->addFeatures(attributes);

            //check feature
            if(features.size() == 1 && !features.at(0).isNull() && !features.at(0)->getGeometry().isNull()){
                feature = features.at(0);
            }

        }

        //check feature
        if(feature.isNull() || feature->getGeometry().isNull() || feature->getGeometry()->getIsNominal()){
            foreach(const QPointer<Reading> &reading, readings){
                if(!reading.isNull()){
                    delete reading;
                }
            }
            return;
        }

    }

    //add observations
    this->job->addMeasurementResults(geomId, readings);

    //complete the measurement task by informing MainWindow about its success
    emit this->measurementCompleted();

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
    QObject::connect(this->job.data(), &OiJob::sendMessage, this, &Controller::log, Qt::AutoConnection);

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
    /*QObject::connect(this->job.data(), &OiJob::trafoParamValidTimeChanged,
                     this, &Controller::trafoParamValidTimeChanged, Qt::AutoConnection);*/
    /*QObject::connect(this->job.data(), &OiJob::trafoParamIsMovementChanged,
                     this, &Controller::trafoParamIsMovementChanged, Qt::AutoConnection);*/

    //pass the new job around
    ModelManager::setCurrentJob(this->job);
    this->exchanger.setCurrentJob(this->job);
    this->featureUpdater.setCurrentJob(this->job);
    this->requestHandler.setCurrentJob(this->job);
    if(!this->measurementConfigManager.isNull()){
        this->measurementConfigManager->setCurrentJob(this->job);
    }

    emit this->currentJobChanged();

}

/*!
 * \brief Controller::initDisplayConfigs
 * Sets up initial display configs and passes them to model manager
 */
void Controller::initDisplayConfigs(){

    //create default configs
    FeatureTableColumnConfig featureTableColumnConfig;
    TrafoParamTableColumnConfig trafoParamTableColumnConfig;
    ObservationTableColumnConfig observationTableColumnConfig;
    ReadingTableColumnConfig readingTableColumnConfig;
    ParameterDisplayConfig parameterDisplayConfig;

    //pass the default configs to model manager
    ModelManager::setFeatureTableColumnConfig(featureTableColumnConfig);
    ModelManager::setTrafoParamColumnConfig(trafoParamTableColumnConfig);
    ModelManager::setObservationTableColumnConfig(observationTableColumnConfig);
    ModelManager::setReadingTableColumnConfig(readingTableColumnConfig);
    ModelManager::setParameterDisplayConfig(parameterDisplayConfig);

}

/*!
 * \brief Controller::initConfigManager
 */
void Controller::initConfigManager(){

    //load configs from config folder
    this->sensorConfigManager = new SensorConfigurationManager();
    this->sensorConfigManager->loadFromConfigFolder();
    this->measurementConfigManager = new MeasurementConfigManager();
    this->measurementConfigManager->loadFromConfigFolder();

    //pass config managers to model manager
    ModelManager::setSensorConfigManager(this->sensorConfigManager);
    ModelManager::setMeasurementConfigManager(this->measurementConfigManager);

    //pass config manager to project exchanger
    ProjectExchanger::setMeasurementConfigManager(this->measurementConfigManager);

    //pass config manager to request handler
    this->requestHandler.setMeasurementConfigManager(this->measurementConfigManager);
    this->requestHandler.setSensorConfigManager(this->sensorConfigManager);

    //connect config manager
    QObject::connect(this->sensorConfigManager.data(), &SensorConfigurationManager::sendMessage, this, &Controller::log, Qt::AutoConnection);
    QObject::connect(this->measurementConfigManager.data(), &MeasurementConfigManager::sendMessage, this, &Controller::log, Qt::AutoConnection);

}

/*!
 * \brief Controller::initToolPlugins
 */
void Controller::initToolPlugins(){

    //get available tools from database
    QList<sdb::Tool> toolPlugins = SystemDbManager::getTools();

    //load all tool plugins and add them to the list of tool plugins
    this->toolPlugins.clear();
    foreach(const sdb::Tool &tool, toolPlugins){

        QPointer<Tool> toolPlugin = PluginLoader::loadToolPlugin(tool.plugin.file_path, tool.name);
        if(!toolPlugin.isNull()){
            toolPlugin->setAttribute(Qt::WA_DeleteOnClose, false);
            this->connectToolPlugin(toolPlugin);
            this->toolPlugins.append(toolPlugin);
        }

    }

}

/*!
 * \brief Controller::registerMetaTypes
 * Registers meta types to be able to use them in signal slot connections
 */
void Controller::registerMetaTypes(){

    qRegisterMetaType<oi::MessageTypes>("MessageTypes");
    qRegisterMetaType<oi::MessageDestinations>("MessageDestinations");
    qRegisterMetaType<oi::MeasurementConfig>("MeasurementConfig");
    qRegisterMetaType<oi::SensorConfiguration>("SensorConfiguration");
    qRegisterMetaType<QList<QPointer<oi::Reading> > >("QList<QPointer<Reading> >");
    qRegisterMetaType<QPointer<oi::Feature> >("QPointer<Feature>");
    qRegisterMetaType<QPointer<oi::Function> >("QPointer<Function>");
    qRegisterMetaType<QPointer<oi::FeatureWrapper> >("QPointer<FeatureWrapper>");
    qRegisterMetaType<QPointer<oi::Observation> >("QPointer<Observation>");
    qRegisterMetaType<oi::OiRequestResponse>("OiRequestResponse");
    qRegisterMetaType<ParameterDisplayConfig>("ParameterDisplayConfig");

    qRegisterMetaType<oi::Sensor>("Sensor");
    qRegisterMetaType<QPointer<oi::Sensor> >("QPointer<Sensor>");
    qRegisterMetaType<oi::ReadingTypes>("ReadingTypes");
    qRegisterMetaType<QList<oi::ReadingTypes> >("QList<ReadingTypes>");
    qRegisterMetaType<oi::SensorTypes>("SensorTypes");
    qRegisterMetaType<QList<oi::SensorTypes> >("QList<SensorTypes>");
    qRegisterMetaType<oi::ConnectionTypes>("ConnectionTypes");
    qRegisterMetaType<QList<oi::ConnectionTypes> >("QList<ConnectionTypes>");
    qRegisterMetaType<oi::SensorFunctions>("SensorFunctions");
    qRegisterMetaType<QList<oi::SensorFunctions> >("QList<SensorFunctions>");

}

/*!
 * \brief Controller::initServer
 */
void Controller::initServer(){

    //create server object
    this->webSocketServer = new OiWebSocketServer();

    //connect server object
    QObject::connect(this, &Controller::startWebSocketServer, this->webSocketServer, &OiWebSocketServer::startServer, Qt::QueuedConnection);
    QObject::connect(this, &Controller::stopWebSocketServer, this->webSocketServer, &OiWebSocketServer::stopServer, Qt::QueuedConnection);
    QObject::connect(this->webSocketServer, &OiWebSocketServer::sendMessage, this, &Controller::log, Qt::QueuedConnection);
    QObject::connect(this->webSocketServer, &OiWebSocketServer::sendRequest, &this->requestHandler, &OiRequestHandler::receiveRequest, Qt::QueuedConnection);
    QObject::connect(&this->requestHandler, &OiRequestHandler::sendResponse, this->webSocketServer, &OiWebSocketServer::receiveResponse, Qt::QueuedConnection);

    //move server object to thread
    if(!this->serverThread.isRunning()){
        this->serverThread.start();
    }
    this->webSocketServer->moveToThread(&this->serverThread);

}

/*!
 * \brief Controller::startServer
 */
void Controller::startServer(){

    //check server instance
    if(this->webSocketServer.isNull()){
        return;
    }

    emit this->startWebSocketServer();

}

/*!
 * \brief Controller::stopServer
 */
void Controller::stopServer(){

    //check server instance
    if(this->webSocketServer.isNull()){
        return;
    }

    emit this->stopWebSocketServer();

}

/*!
 * \brief Controller::createActualFromNominal
 * \param geometry
 * \return
 */
bool Controller::createActualFromNominal(const QPointer<Geometry> &geometry){

    //check job
    if(this->job.isNull()){
        return false;
    }

    //check geometry
    if(geometry.isNull() || geometry->getFeatureWrapper().isNull()){
        return false;
    }

    //check if actual already exists
    if(!geometry->getMyMasterGeometry()->getActual().isNull()){
        return true;
    }

    //set up feature attributes
    FeatureAttributes attr;
    attr.count = 1;
    attr.typeOfFeature = geometry->getFeatureWrapper()->getFeatureTypeEnum();
    attr.name = geometry->getFeatureName();
    attr.group = geometry->getGroupName();
    attr.isActual = true;

    //create actual
    this->job->addFeatures(attr);
    if(geometry->getMyMasterGeometry()->getActual().isNull() || geometry->getMyMasterGeometry()->getActual()->getFeatureWrapper().isNull()){
        return false;
    }

    //set function and measurement config
    QList<QPointer<FeatureWrapper> > actuals;
    actuals.append(geometry->getMyMasterGeometry()->getActual()->getFeatureWrapper());
    MeasurementConfig mConfig;
    if(!this->measurementConfigManager.isNull()){
        mConfig = this->measurementConfigManager->getActiveMeasurementConfig(getGeometryTypeEnum(attr.typeOfFeature));
    }
    sdb::Function defaultFunction = SystemDbManager::getDefaultFunction(attr.typeOfFeature);
    this->addFunctionsAndMConfigs(actuals, mConfig, defaultFunction.plugin.file_path, defaultFunction.name);

    return true;


}

/*!
 * \brief Controller::addFunctionsAndMConfigs
 * \param actuals
 * \param mConfig
 * \param path
 * \param fName
 */
void Controller::addFunctionsAndMConfigs(const QList<QPointer<FeatureWrapper> > &actuals, const MeasurementConfig &mConfig, const QString &path, const QString &fName){

    //check job
    if(this->job.isNull()){
        return;
    }

    foreach(const QPointer<FeatureWrapper> &feature, actuals){

        //check feature
        if(feature.isNull() || feature->getFeature().isNull()){
            continue;
        }

        //check if the feature is a nominal geometry
        if(feature->getGeometry().isNull() || feature->getGeometry()->getIsNominal() || feature->getGeometry()->getMyMasterGeometry().isNull()){
            continue;
        }
        /*if(!feature->getGeometry().isNull() && !feature->getMasterGeometry()->getActual().isNull()){
            continue;
        }*/

        //load function plugin
        QPointer<Function> function(NULL);
        if(path != 0 && fName != 0){
            function = PluginLoader::loadFunctionPlugin(path, fName);
        }

        //assign function and measurement config to feature
        this->job->blockSignals(true);
        if(!function.isNull()){
            feature->getFeature()->addFunction(function);
        }
        if(mConfig.getIsValid() && !feature->getGeometry()->getMyMasterGeometry()->getActual().isNull()){
            feature->getGeometry()->getMyMasterGeometry()->setMeasurementConfig(mConfig);
        }
        this->job->blockSignals(false);
    }
}

/*!
 * \brief Controller::connectDataExchanger
 */
void Controller::connectDataExchanger(){

    //messaging
    QObject::connect(&this->exchanger, &DataExchanger::sendMessage, this, &Controller::log, Qt::AutoConnection);

    //nominal import
    QObject::connect(&this->exchanger, &DataExchanger::nominalImportFinished, this, &Controller::nominalImportFinished, Qt::AutoConnection);
    QObject::connect(&this->exchanger, &DataExchanger::updateNominalImportProgress, this, &Controller::nominalImportProgressUpdated, Qt::AutoConnection);

    //observation import
    QObject::connect(&this->exchanger, &DataExchanger::observationImportFinished, this, &Controller::observationImportFinished, Qt::AutoConnection);
    QObject::connect(&this->exchanger, &DataExchanger::updateObservationImportProgress, this, &Controller::observationImportProgressUpdated, Qt::AutoConnection);

}

/*!
 * \brief Controller::connectFeatureUpdater
 */
void Controller::connectFeatureUpdater(){

}

/*!
 * \brief Controller::connectRequestHandler
 */
void Controller::connectRequestHandler(){

    //sensor actions
    QObject::connect(&this->requestHandler, &OiRequestHandler::startAim, this, &Controller::startAim, Qt::AutoConnection);
    QObject::connect(&this->requestHandler, &OiRequestHandler::startMeasurement, this, &Controller::startMeasurement, Qt::AutoConnection);
    QObject::connect(&this->requestHandler, &OiRequestHandler::startReadingStream, this, &Controller::startWatchWindow, Qt::AutoConnection);
    QObject::connect(&this->requestHandler, &OiRequestHandler::stopReadingStream, this, &Controller::stopWatchWindow, Qt::AutoConnection);

    //connect streaming
    QObject::connect(this, &Controller::sensorActionStarted, &this->requestHandler, &OiRequestHandler::sensorActionStarted, Qt::AutoConnection);
    QObject::connect(this, &Controller::sensorActionFinished, &this->requestHandler, &OiRequestHandler::sensorActionFinished, Qt::AutoConnection);
    QObject::connect(this, &Controller::showClientMessage, &this->requestHandler, &OiRequestHandler::log, Qt::AutoConnection);
    QObject::connect(this, &Controller::activeFeatureChanged, &this->requestHandler, &OiRequestHandler::activeFeatureChanged, Qt::AutoConnection);
    QObject::connect(this, &Controller::activeStationChanged, &this->requestHandler, &OiRequestHandler::activeStationChanged, Qt::AutoConnection);
    QObject::connect(this, &Controller::activeCoordinateSystemChanged, &this->requestHandler, &OiRequestHandler::activeCoordinateSystemChanged, Qt::AutoConnection);
    QObject::connect(this, &Controller::featureSetChanged, &this->requestHandler, &OiRequestHandler::featureSetChanged, Qt::AutoConnection);
    QObject::connect(this, &Controller::featureAttributesChanged, &this->requestHandler, &OiRequestHandler::featureAttributesChanged, Qt::AutoConnection);

}

/*!
 * \brief Controller::connectToolPlugin
 * \param tool
 */
void Controller::connectToolPlugin(const QPointer<Tool> &tool){

    if(tool.isNull()){
        return;
    }

    QObject::connect(tool.data(), &Tool::startConnect, this, &Controller::startConnect, Qt::AutoConnection);
    QObject::connect(tool.data(), &Tool::startDisconnect, this, &Controller::startDisconnect, Qt::AutoConnection);
    QObject::connect(tool.data(), &Tool::startMeasurement, this, &Controller::startMeasurement, Qt::AutoConnection);
    QObject::connect(tool.data(), &Tool::startMove, this, &Controller::startMove, Qt::AutoConnection);
    QObject::connect(tool.data(), &Tool::startAim, this, &Controller::startAim, Qt::AutoConnection);
    QObject::connect(tool.data(), &Tool::startAimAndMeasure, this, &Controller::startAimAndMeasure, Qt::AutoConnection);
    QObject::connect(tool.data(), &Tool::startToggleSight, this, &Controller::startToggleSight, Qt::AutoConnection);
    QObject::connect(tool.data(), &Tool::startInitialize, this, &Controller::startInitialize, Qt::AutoConnection);
    QObject::connect(tool.data(), &Tool::startHome, this, &Controller::startHome, Qt::AutoConnection);
    QObject::connect(tool.data(), &Tool::startCompensation, this, &Controller::startCompensation, Qt::AutoConnection);
    QObject::connect(tool.data(), &Tool::startChangeMotorState, this, &Controller::startChangeMotorState, Qt::AutoConnection);
    QObject::connect(tool.data(), &Tool::startCustomAction, this, &Controller::startCustomAction, Qt::AutoConnection);
    QObject::connect(tool.data(), &Tool::sendMessage, this, &Controller::log, Qt::AutoConnection);

}
