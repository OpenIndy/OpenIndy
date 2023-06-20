#include "controller.h"
#include "defaultsconfiginit.h"
/*!
 * \brief Controller::Controller
 * \param parent
 */
Controller::Controller(QObject *parent) : QObject(parent){

}

void Controller::init() {
    //register meta types
    this->registerMetaTypes();

    //load and restore project unit settings
    if(!ProjectConfig::loadProjectPathConfigFile()) {
        this->log("Could not load project path", eErrorMessage, eMessageBoxMessage);
    }

    //load config from file
    if(!ProjectConfig::loadProjectSettingsConfigFile()) {
        this->log("Could not load project settings", eErrorMessage, eMessageBoxMessage);
    }

    //initialize and connect model manager
    ModelManager::init();
    if(!ModelManager::myInstance.isNull()){
        QObject::connect(ModelManager::myInstance.data(), &ModelManager::sendMessage, this, &Controller::log, Qt::QueuedConnection);

        // "compress" updateModel signals
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), &ModelManager::myInstance->getFeatureTableModel(), SLOT(updateModelIfRequested()));
        timer->start(250);
    }

    QObject::connect(PluginLoader::getInstance().data(), &PluginLoader::sendMessage, this, &Controller::log, Qt::QueuedConnection);

    //initialize display configs
    this->initDisplayConfigs();

    //initialize config manager
    this->initConfigManager();

#ifdef OI_WEBSOCKETSERVER_ENABLED
    //init and start OpenIndy server
    this->initServer();
    this->startServer();
#endif

    //initialize tool plugins
    this->initToolPlugins();

    this->initDefaults(); // after read measurement confing & after load functions

    //connect helper objects
    this->connectDataExchanger();
    this->connectFeatureUpdater();
#ifdef OI_WEBSOCKETSERVER_ENABLED
    this->connectRequestHandler();
#endif

    this->sensorWorkerThread = new QThread();
    this->sensorWorkerThread->setObjectName("Sensor Worker");
    this->sensorWorkerThread->start();

}

/*!
 * \brief Controller::~Controller
 */
Controller::~Controller(){

    //stop web socket server thread
#ifdef OI_WEBSOCKETSERVER_ENABLED
    if(this->serverThread.isRunning()){
        this->serverThread.quit();
        this->serverThread.wait();
    }
#endif

    QPointer<Console> console = Console::getInstance();
    if(!console.isNull()) {
        delete Console::getInstance().data();
    }

    if(!this->sensorWorkerThread.isNull()) {
        if(this->sensorWorkerThread->isRunning()) {
            this->sensorWorkerThread->quit();
            this->sensorWorkerThread->wait();
        }
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
        emit this->featureCreated(false);
        return;
    }
    emit this->featureCreated(true);

    //check used states of trafo params
    for(int i = 0; i<features.size(); i++){
        if(!features.at(i)->getTrafoParam().isNull()){

            //check if other trafos with this coordinate systems already exist and are active/ used
            for(int j=0; j<this->job->getTransformationParametersList().size(); j++){

                if((features.at(i)->getTrafoParam()->getStartSystem() == this->job->getTransformationParametersList().at(j)->getStartSystem()
                        && features.at(i)->getTrafoParam()->getDestinationSystem() == this->job->getTransformationParametersList().at(j)->getDestinationSystem())
                        || (features.at(i)->getTrafoParam()->getStartSystem() == this->job->getTransformationParametersList().at(j)->getDestinationSystem()
                            && features.at(i)->getTrafoParam()->getDestinationSystem() == this->job->getTransformationParametersList().at(j)->getStartSystem())){

                    if(this->job->getTransformationParametersList().at(j)->getFeatureName().compare(features.at(i)->getFeature()->getFeatureName()) != 0){

                        if(this->job->getTransformationParametersList().at(j)->getIsUsed()){
                            features.at(i)->getTrafoParam()->setIsUsed(false);
                            emit this->requestMessageBoxTrafoParam();
                            break;
                        }
                    }
                }
            }
        }
    }

    //check if there is at least a function or a measurement config that shall be set
    if(!attributes.measurementConfig.isValid() && (attributes.functionPlugin.first.compare("") == 0
                                 || attributes.functionPlugin.second.compare("") == 0)){
        return;
    }

    //create functions and measurement configs for the created features
    this->addFunctionsAndMConfigs(features, attributes.measurementConfig, attributes.functionPlugin.second, attributes.functionPlugin.first);

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

    this->featureUpdater.recalcFeature(feature->getFeature());
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
    QPointer<Station> activeStation = this->getActiveStation();
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
    QObject::connect(sensor, &Sensor::sensorStatus, this, &Controller::sensorStatus);

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
    QPointer<Station> station = this->getActiveStation();
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
    QPointer<Station> activeStation = this->getActiveStation();
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
    if(!mConfig.isValid()){
        this->log("No measurement configuration selected", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //set measurement config for the active feature
    activeFeature->getGeometry()->setMeasurementConfig(mConfig);
}
void Controller::saveUserConfig(const MeasurementConfig &mConfig){
    if( mConfig.isUserConfig()
        && !mConfig.isStandardConfig()
        ) {
        measurementConfigManager->saveUserConfig(mConfig); // "add" to config/measurementConfigs folder
    } else {
        this->log(QString("Cannot save a measurement configuration \"%1\" because is it not vaild user config").arg(mConfig.getName()), eErrorMessage, eMessageBoxMessage);
    }
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
 * \brief Controller::exportFeatures
 * \param params
 */
void Controller::exportFeatures(const ExchangeParams &params)
{
    //try to start export features
    if(!this->exchanger.exportData(params)){
        return;
    }
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

void Controller::enableObservations(const int &featureId) {

    //check job
    if(this->job.isNull()){
        this->log("No job available", eErrorMessage, eMessageBoxMessage);
        return;
    }

    this->job->enableObservations(featureId);

}
void Controller::disableObservations(const int &featureId){

    //check job
    if(this->job.isNull()){
        this->log("No job available", eErrorMessage, eMessageBoxMessage);
        return;
    }

    this->job->disableObservations(featureId);

}

void Controller::removeObservationsById(const QList<int> selectedIds)
{
    //check job
    if(this->job.isNull()){
        this->log("No job available", eErrorMessage, eMessageBoxMessage);
        return;
    }

    this->job->removeObservations(this->job->getActiveFeature()->getFeature()->getId(), selectedIds);
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

    this->log(QString("Bundle template %1 loaded successfully").arg(bundleTemplate.value("name").toString()), eInformationMessage, eConsoleMessage);

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
 * \brief check if digest has changed and update digest in job
 */
bool Controller::hasProjectDigestChanged() {
    if(this->job.isNull()){
        return false;
    }
    QString preDigest = this->job->getDigest();
    projectExchanger.saveProject(this->job);
    return preDigest != job->getDigest(); // not equal, that means project data changed
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

    QList<QPointer<Tool> > tools = this->getAvailableTools();
    QList<QPointer<Tool> >::iterator toolIt;
    for (toolIt = tools.begin(); toolIt != tools.end(); ++toolIt) {
        const QPointer<Tool>& tool = *toolIt;
        if(!tool->saveProjectEnabled()) {
            this->log(QString("Saving project denied by tool: %1").arg(tool->getMetaData().name), eErrorMessage, eMessageBoxMessage);
            return;
        }
    }

    //get and check name and file path
    QString name = this->job->getJobName();
    QPointer<QFileDevice> fileDevice = this->job->getJobDevice();
    if(name.compare("") == 0 || fileDevice.isNull()){
        emit this->saveAsTriggered();
        return;
    }

    QMutexLocker locker(&saveProjectMutex); // synchronize write file

    //get project xml
    QDomDocument project = projectExchanger.saveProject(this->job);

    if(project.isNull()){
        this->log("Error while creating project XML", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //save project xml
    QSaveFile saveFile(fileDevice->fileName());
    if(saveFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
        QTextStream stream(&saveFile);
        project.save(stream, 4);
        saveFile.commit();

        this->log(QString("OpenIndy project \"%1\" successfully stored.").arg(fileDevice->fileName()), eInformationMessage, eStatusBarMessage);
    }else{
        this->log(QString("Cannot open file  \"%1\"").arg(fileDevice->fileName()), eInformationMessage, eStatusBarMessage);
        QMessageBox msgBox;
        msgBox.setText("data source could not be found.");
        msgBox.setInformativeText("Please check your saved data, before you continue.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        return;
    }

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
    if(!info.absoluteDir().exists()){
        this->log("Directory not exists", eErrorMessage, eMessageBoxMessage);
        return;
    }
    this->job->setJobName(fileName);

    //set device
    this->job->setJobDevice(new QFile(fileName));

    this->saveProject();

    emit this->currentJobChanged(); // maybe changed TODO: emit if this->saveProject(); is successful
}

/*!
 * \brief Controller::loadProject
 * \param projectName
 * \param device
 */
void Controller::loadProject(const QString &projectName, const QPointer<QFileDevice> &device){

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
    newJob = projectExchanger.loadProject(project);

    //check job
    if(newJob.isNull()){
        this->log("Error while parsing OpenIndy xml file.", eErrorMessage, eMessageBoxMessage);
        return;
    }

    bool compatible = false;
    QString msg;
    switch(newJob->checkCompatibilty()) {
    case eCheckResult_match:
    case eCheckResult_oi_gt_job:
        compatible = true;
        break;
    case eCheckResult_job_wo_valid_version:
        compatible = false;
        msg = "This OpenIndy project has no vaild version. Continue opening project?";
        break;
    case eCheckResult_oi_wo_valid_version:
        compatible = false;
        msg = "This OpenIndy has no vaild version. Continue opening project?";
        break;
    case eCheckResult_oi_lt_job:
        compatible = false;
        msg = "This OpenIndy version is older than OpenIndy project version. "
              "Please use newer OpenIndy version. Continue opening project?";
        break;
    case eCheckResult_job_lt_oi_22_1:
        compatible = false;
        msg = "This OpenIndy version is newer than OpenIndy project version. "
              "Please migrate project otherwise project measurement config will not work propperly. "
              "Continue opening project?";
        break;
    default:
        compatible = false;
        msg = "This OpenIndy version is not compatible with OpenIndy project version. Continue opening project?";
        break;
    }

    if(compatible || (emit this->showMessageBox(msg, eQuestionMessage) == QMessageBox::Yes) ) {

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

        this->log(QString("OpenIndy project \"%1\" successfully loaded.").arg(device->fileName()), eInformationMessage, eConsoleMessage);
    } else {
        this->log(QString("OpenIndy project \"%1\" was not loaded.").arg(device->fileName()), eInformationMessage, eConsoleMessage);
    }


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

    this->addBundleSystem();

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
    QPointer<Station> activeStation = this->getActiveStation();
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
    QPointer<Station> activeStation = this->getActiveStation();
    if(activeStation.isNull()){
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("disconnecting sensor...");

    //disconnect sensor
    activeStation->disconnectSensor();
}

void Controller::startMeasurement(){
    this->_startMeasurement(false);
}

/*!
 * \brief Controller::startMeasurement
 */
void Controller::_startMeasurement(bool dummyPoint){

    QPointer<Station> activeStation = getConnectedActiveStation();
    if(activeStation.isNull()){
       return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> activeFeature = this->job->getActiveFeature();
    if(activeFeature.isNull() || activeFeature->getGeometry().isNull()){
        this->log("No active feature", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //create actual from nominal (if a nominal is selected)
    if(activeFeature->getGeometry()->getIsNominal()){
        if(!this->createActualFromNominal(activeFeature->getGeometry())
                || activeFeature->getGeometry()->getActual().isNull()
                || activeFeature->getGeometry()->getActual()->getFeatureWrapper().isNull()){
            this->log("Cannot create actual for nominal", eErrorMessage, eMessageBoxMessage);
            return;
        }
        activeFeature = activeFeature->getGeometry()->getActual()->getFeatureWrapper();
        activeFeature->getFeature()->setActiveFeatureState(true);
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("performing measurement...", true);

    //perform measurement
    int id = activeFeature->getGeometry()->getId();
    MeasurementConfig mConfig = activeFeature->getGeometry()->getMeasurementConfig();
    mConfig.setTransientData("isDummyPoint", dummyPoint); // use MeasurementConfig for "transportation"
    activeStation->measure(id, mConfig);

}

/*!
 * \brief Controller::startMove
 * \param reading
 */
void Controller::startMove(const Reading &reading){

    QPointer<Station> activeStation = getConnectedActiveStation();
    if(activeStation.isNull()){
       return;
    }


    //get and check active feature
    QPointer<FeatureWrapper> activeFeature = this->job->getActiveFeature();
    if(activeFeature.isNull() || activeFeature->getGeometry().isNull()){
        this->log("No active feature", eErrorMessage, eMessageBoxMessage);
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
    QPointer<Station> activeStation = this->getActiveStation();
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
        }else if(!activeFeature->getGeometry()->getActual().isNull() && activeFeature->getGeometry()->getActual()->hasPosition()
                 && activeFeature->getGeometry()->getActual()->getIsSolved()
                 && trafoController.getTransformationMatrix(t, activeCoordinateSystem, activeStation->getCoordinateSystem())){
            pos = activeFeature->getGeometry()->getActual()->getPosition().getVectorH();
            pos = t * pos;
        }

    }else{ //actual

        //transform the actual into the station coordinate system
        if(activeFeature->getGeometry()->getIsSolved() && trafoController.getTransformationMatrix(t, activeCoordinateSystem, activeStation->getCoordinateSystem())){
            pos = activeFeature->getGeometry()->getPosition().getVectorH();
            pos = t * pos;
        }else if(activeFeature->getGeometry()->getNominals().size() > 0){

            //use nominal instead of actual
            foreach(const QPointer<Geometry> &nominal, activeFeature->getGeometry()->getNominals()){
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
    QPointer<Station> activeStation = this->getActiveStation();
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
        }else if(!activeFeature->getGeometry()->getActual().isNull() && activeFeature->getGeometry()->getActual()->hasPosition()
                 && activeFeature->getGeometry()->getActual()->getIsSolved()
                 && trafoController.getTransformationMatrix(t, activeCoordinateSystem, activeStation->getCoordinateSystem())){
            pos = activeFeature->getGeometry()->getActual()->getPosition().getVectorH();
            pos = t * pos;
        }

    }else{ //actual

        //transform the actual into the station coordinate system
        if(activeFeature->getGeometry()->getIsSolved() && trafoController.getTransformationMatrix(t, activeCoordinateSystem, activeStation->getCoordinateSystem())){
            pos = activeFeature->getGeometry()->getPosition().getVectorH();
            pos = t * pos;
        }else if(activeFeature->getGeometry()->getNominals().size() > 0){

            //use nominal instead of actual
            foreach(const QPointer<Geometry> &nominal, activeFeature->getGeometry()->getNominals()){
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
                            activeFeature->getGeometry()->getMeasurementConfig());
    }

}

/*!
 * \brief Controller::startToggleSight
 */
void Controller::startToggleSight(){

    QPointer<Station> activeStation = getConnectedActiveStation();
    if(activeStation.isNull()){
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

    QPointer<Station> activeStation = getConnectedActiveStation();
    if(activeStation.isNull()){
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

    QPointer<Station> activeStation = getConnectedActiveStation();
    if(activeStation.isNull()){
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

    QPointer<Station> activeStation = getConnectedActiveStation();
    if(activeStation.isNull()){
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

    QPointer<Station> activeStation = getConnectedActiveStation();
    if(activeStation.isNull()){
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

    QPointer<Station> activeStation = getConnectedActiveStation();
    if(activeStation.isNull()){
       return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted(QString("custom action: %1 ...").arg(task));

    //do self defined action
    activeStation->selfDefinedAction(task);

}

/**
 * @brief Controller::getConnectedActiveStation
 * @return connected active station or 0
 */
QPointer<Station> Controller::getConnectedActiveStation() {
    //check current job
    if(this->job.isNull()){
        return 0;
    }

    //get and check active station
    QPointer<Station> activeStation = this->getActiveStation();
    if(activeStation.isNull()){
        this->log("No active station", eErrorMessage, eMessageBoxMessage);
        return 0;
    }

    //check sensor
    if(!activeStation->getIsSensorConnected()){
        this->log("No sensor connected to the active station", eErrorMessage, eMessageBoxMessage);
        return 0;
    }

    return activeStation;
}

QPointer<Station> Controller::getActiveStation() {
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(!activeStation.isNull()) {
        activeStation->setSensorWorkerThread(this->sensorWorkerThread);
    }
    return activeStation;
}

/*!
 * \brief Controller::startReadingStream
 * \param streamFormat
 */
void Controller::startReadingStream(ReadingTypes streamFormat){
    QPointer<Station> activeStation = getConnectedActiveStation();
    if(activeStation.isNull()){
       return;
    }

    //start streaming
    activeStation->setStreamFormat(streamFormat);
    activeStation->startReadingStream();

}

/*!
 * \brief Controller::stopReadingStream
 */
void Controller::stopReadingStream(){

    QPointer<Station> activeStation = getConnectedActiveStation();
    if(activeStation.isNull()){
       return;
    }

    //stop streaming
    activeStation->stopReadingStream();

}

void Controller::finishMeasurement(){
    qDebug() << "Controller::finishMeasurement()";
    QPointer<Station> activeStation = getConnectedActiveStation();
    if(activeStation.isNull()){
       return;
    }

    qDebug() << "Controller::finishMeasurement() emit";
    //finish Measurement streaming
    emit activeStation->finishMeasurement();

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
 * \brief Controller::initConfigs
 */
void Controller::initConfigs()
{
    this->initConfigManager();
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
        QObject::disconnect(station, &Station::measurementDone, 0, 0);
        QObject::disconnect(station, &Station::sensorMessage, 0, 0);
    }

    //connect sensor action results of active station
    QObject::connect(activeStation, &Station::commandFinished, this, &Controller::sensorActionFinished, Qt::AutoConnection);
    QObject::connect(activeStation, &Station::measurementFinished, this, &Controller::measurementFinished, Qt::AutoConnection);
    QObject::connect(activeStation, &Station::measurementDone, this, &Controller::measurementDone, Qt::AutoConnection);
    QObject::connect(activeStation, &Station::sensorMessage, this, &Controller::log, Qt::AutoConnection);

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
        if(!feature->getGeometry()->getActual().isNull() && !feature->getGeometry()->getActual()->getFeatureWrapper().isNull()
                && !feature->getGeometry()->getActual()->getFeatureWrapper()->getGeometry().isNull()){ //has actual
            feature = feature->getGeometry()->getActual()->getFeatureWrapper();
        }else if(feature->getGeometry()->getActual().isNull()){ //does not have an actual

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

    //check if feature has a fit function, else regret measurement
    if(feature->getGeometry()->getFunctions().size() == 0 || feature->getGeometry()->getFunctions().first().isNull()){
        this->log("Measurement failed - no fit function set", eErrorMessage, eMessageBoxMessage);
        return;
    }

    if(feature->getGeometry()->getFunctions().first()->getMetaData().iid != FitFunction_iidd){
        this->log("Measurement failed - no fit function set", eErrorMessage, eMessageBoxMessage);
        return;
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
#ifdef OI_WEBSOCKETSERVER_ENABLED
    this->requestHandler.setCurrentJob(this->job);
#endif
    if(!this->measurementConfigManager.isNull()){
        this->measurementConfigManager->setCurrentJob(this->job);
    }

    emit this->currentJobChanged();
    emit this->setCurrentJob(this->job); // set current job to other modules or tools

}

/*!
 * \brief Controller::getActiveFeature
 * \return
 */
QPointer<FeatureWrapper> Controller::getActiveFeature()
{
    return this->job->getActiveFeature();
}

/*!
 * \brief Controller::initDisplayConfigs
 * Sets up initial display configs and passes them to model manager
 */
void Controller::initDisplayConfigs(){

    //create default configs
    FeatureTableColumnConfig featureTableColumnConfig(ProjectConfig::getDisplayColumns());
    TrafoParamTableColumnConfig trafoParamTableColumnConfig;
    ObservationTableColumnConfig observationTableColumnConfig;
    ReadingTableColumnConfig readingTableColumnConfig;
    ParameterDisplayConfig parameterDisplayConfig = ProjectConfig::getParameterDisplayConfig();

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

    if(this->sensorConfigManager.isNull()) {
        //load configs from config folder
        this->sensorConfigManager = new SensorConfigurationManager();
        this->sensorConfigManager->loadFromConfigFolder();
        this->measurementConfigManager = new MeasurementConfigManager();
        this->measurementConfigManager->loadFromConfigFolder();

        //pass config managers to model manager
        ModelManager::setSensorConfigManager(this->sensorConfigManager);
        ModelManager::setMeasurementConfigManager(this->measurementConfigManager);

        //pass config manager to project exchanger
        projectExchanger.setMeasurementConfigManager(this->measurementConfigManager);

        //pass config manager to request handler
#ifdef OI_WEBSOCKETSERVER_ENABLED
        this->requestHandler.setMeasurementConfigManager(this->measurementConfigManager);
        this->requestHandler.setSensorConfigManager(this->sensorConfigManager);
#endif
        //connect config manager
        QObject::connect(this->sensorConfigManager.data(), &SensorConfigurationManager::sendMessage, this, &Controller::log, Qt::AutoConnection);
        QObject::connect(this->measurementConfigManager.data(), &MeasurementConfigManager::sendMessage, this, &Controller::log, Qt::AutoConnection);

        this->exchanger.setMesaurementConfigManager(this->measurementConfigManager);
    }
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

void Controller::initDefaults(){
    QString path;
    path = QString("%1%2").arg(qApp->applicationDirPath()).arg("/config/defaults.json");
    DefaultsConfigInit defaultsConfig;
    defaultsConfig.init(path);
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

#ifdef OI_WEBSOCKETSERVER_ENABLED
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
#endif

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
    if(!geometry->getActual().isNull()){
        return true;
    }

    //set up feature attributes
    FeatureAttributes attr;
    attr.count = 1;
    attr.typeOfFeature = geometry->getFeatureWrapper()->getFeatureTypeEnum();
    attr.name = geometry->getFeatureName();
    attr.group = geometry->getGroupName();
    attr.isActual = true;

    //get measurement config
    QString elementConfigName = SystemDbManager::getDefaultMeasurementConfig(getElementTypeName(getElementTypeEnum(geometry->getFeatureWrapper()->getFeatureTypeString())));
    MeasurementConfig mConfig = this->measurementConfigManager->getUserConfig(elementConfigName);

    //create actual
    this->job->addFeatures(attr);
    if(geometry->getActual().isNull() || geometry->getActual()->getFeatureWrapper().isNull()){
        return false;
    }

    //set function and measurement config
    QList<QPointer<FeatureWrapper> > actuals;
    actuals.append(geometry->getActual()->getFeatureWrapper());

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
        if(!feature->getGeometry().isNull() && feature->getGeometry()->getIsNominal()){
            continue;
        }

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
        if(mConfig.isValid() && !feature->getGeometry().isNull()){
            feature->getGeometry()->setMeasurementConfig(mConfig);
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

    QObject::connect(this, &Controller::setExchangeParams, &this->exchanger, &DataExchanger::setExchangeParams, Qt::AutoConnection);
}

/*!
 * \brief Controller::connectFeatureUpdater
 */
void Controller::connectFeatureUpdater(){

}

#ifdef OI_WEBSOCKETSERVER_ENABLED
/*!
 * \brief Controller::connectRequestHandler
 */
void Controller::connectRequestHandler(){
    //sensor actions
    QObject::connect(&this->requestHandler, &OiRequestHandler::startAim, this, &Controller::startAim, Qt::AutoConnection);
    QObject::connect(&this->requestHandler, &OiRequestHandler::startMeasurement, this, &Controller::startMeasurement, Qt::AutoConnection);
    QObject::connect(&this->requestHandler, &OiRequestHandler::startReadingStream, this, &Controller::startReadingStream, Qt::AutoConnection);
    QObject::connect(&this->requestHandler, &OiRequestHandler::stopReadingStream, this, &Controller::stopReadingStream, Qt::AutoConnection);

    //connect streaming
    QObject::connect(this, &Controller::sensorActionStarted, &this->requestHandler, &OiRequestHandler::sensorActionStarted, Qt::QueuedConnection);
    QObject::connect(this, &Controller::sensorActionFinished, &this->requestHandler, &OiRequestHandler::sensorActionFinished, Qt::QueuedConnection);
    QObject::connect(this, &Controller::showClientMessage, &this->requestHandler, &OiRequestHandler::log, Qt::QueuedConnection);
    QObject::connect(this, &Controller::activeFeatureChanged, &this->requestHandler, &OiRequestHandler::activeFeatureChanged, Qt::QueuedConnection);
    QObject::connect(this, &Controller::activeStationChanged, &this->requestHandler, &OiRequestHandler::activeStationChanged, Qt::QueuedConnection);
    QObject::connect(this, &Controller::activeCoordinateSystemChanged, &this->requestHandler, &OiRequestHandler::activeCoordinateSystemChanged, Qt::QueuedConnection);
    QObject::connect(this, &Controller::featureSetChanged, &this->requestHandler, &OiRequestHandler::featureSetChanged, Qt::QueuedConnection);
    QObject::connect(this, &Controller::featureAttributesChanged, &this->requestHandler, &OiRequestHandler::featureAttributesChanged, Qt::QueuedConnection);
}
#endif

/*!
 * \brief Controller::connectToolPlugin
 * \param tool
 */
void Controller::connectToolPlugin(const QPointer<Tool> &tool){

    if(tool.isNull()){
        return;
    }

    // Tool -> Controller
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

    // Controller -> Tool
    QObject::connect(this, &Controller::setCurrentJob, tool.data(), &Tool::setCurrentJob, Qt::AutoConnection);

}

/*!
 * \brief Controller::showToolWidget
 * \param pluginName
 * \param toolName
 */
void Controller::showToolWidget(const QString &pluginName, const QString &toolName){

    //get a list of available tool plugins
    const QList<QPointer<Tool> > &tools = this->getAvailableTools();

    //search the list for the specified tool
    foreach(const QPointer<Tool> &tool, tools){

        if(tool.isNull()){
            continue;
        }

        if(tool->getMetaData().pluginName.compare(pluginName) == 0
                && tool->getMetaData().name.compare(toolName) == 0){
            tool->setJob(this->job);
            tool->show();
            break;
        }

    }

}

void Controller::startSearch(){
    QPointer<Station> activeStation = getConnectedActiveStation();
    if(activeStation.isNull()){
       return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("performing search...", false /* TODO */);

    activeStation->search();
}

void Controller::createTemplateFromJob(){

    //check job
    if(this->job.isNull()){
        this->log("No job available", eErrorMessage, eMessageBoxMessage);
        return;
    }

    this->job->createTemplateFromJob();

}


