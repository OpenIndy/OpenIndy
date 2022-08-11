#include "modelmanager.h"

//single model manager instance
QPointer<ModelManager> ModelManager::myInstance = new ModelManager();

//init static variables
QPointer<OiJob> ModelManager::currentJob;
FeatureTableModel ModelManager::featureTableModel;
FeatureTableProxyModel ModelManager::featureTableProxyModel;
FeatureTableColumnConfig ModelManager::featureTableColumnConfig;
TrafoParamTableColumnConfig ModelManager::trafoParamTableColumnConfig;
ParameterDisplayConfig ModelManager::parameterDisplayConfig;
QStringListModel ModelManager::coordinateSystemsModel;
QStringListModel ModelManager::nominalSystemsModel;
QStringListModel ModelManager::metricUnitTypesModel;
QStringListModel ModelManager::angularUnitTypesModel;
QStringListModel ModelManager::temperatureUnitTypesModel;
QStringListModel ModelManager::dummyStringListModel;
QStringListModel ModelManager::pluginNamesModel;
QStringListModel ModelManager::groupNamesModel;
FeatureTreeViewModel ModelManager::featureTreeViewModel;
AvailableElementsTreeViewProxyModel ModelManager::availableElementsTreeViewProxyModel;
UsedElementsModel ModelManager::usedElementsModel;
FunctionTableModel ModelManager::functionTableModel;
FunctionTableProxyModel ModelManager::functionTableProxyModel;
ActiveFeatureFunctionsModel ModelManager::activeFeatureFunctionsModel;
QPointer<SensorConfigurationManager> ModelManager::sensorConfigManager;
SensorTableModel ModelManager::sensorTableModel;
SensorTableProxyModel ModelManager::sensorTableProxyModel;
//SensorConfigurationModel ModelManager::sensorConfigurationModel;
//SensorConfigurationProxyModel ModelManager::sensorConfigurationProxyModel;
QStringListModel ModelManager::sensorTypeNamesModel;
QList<sdb::Plugin> ModelManager::plugins;
QStandardItemModel ModelManager::baudRateTypesModel;
QStandardItemModel ModelManager::dataBitTypesModel;
QStandardItemModel ModelManager::flowControlTypesModel;
QStandardItemModel ModelManager::parityTypesModel;
QStandardItemModel ModelManager::stopBitTypesModel;
QStandardItemModel ModelManager::availableSerialPortsModel;
QStandardItemModel ModelManager::availableIpAdressesModel;
QStringListModel ModelManager::readingTypeNamesModel;
PluginTreeViewModel ModelManager::pluginTreeViewModel;
TrafoParamTableProxyModel ModelManager::trafoParamTableProxyModel;
MeasurementConfigurationModel ModelManager::measurementConfigurationModel;
MeasurementConfigurationProxyModel ModelManager::measurementConfigurationProxyModel;
QPointer<MeasurementConfigManager> ModelManager::measurementConfigManager;
AvailableFunctionsListModel ModelManager::functionsListModel;
ObservationModel ModelManager::observationModel;
ObservationProxyModel ModelManager::observationProxyModel;
ReadingModel ModelManager::readingModel;
ReadingProxyModel ModelManager::readingProxyModel;
ObservationTableColumnConfig ModelManager::observationTableColumnConfig;
ReadingTableColumnConfig ModelManager::readingTableColumnConfig;
QStringListModel ModelManager::scalarEntityTypeNamesModel;
QStringListModel ModelManager::actualNominalFilterModel;
BundleSystemsModel ModelManager::bundleSystemsModel;
BundleTemplatesModel ModelManager::bundleTemplatesModel;
FunctionWeightsTableModel ModelManager::functionWeightsTableModel;
FunctionWeightProxyModel ModelManager::functionWeightProxyModel;
BundleParameterTableProxyModel ModelManager::bundleParameterTableProxyModel;
FeatureDifferenceTableModel ModelManager::featureDifferenceTableModel;
FeatureDifferenceProxyModel ModelManager::featureDifferenceProxyModel;

/*!
 * \brief ModelManager::ModelManager
 */
ModelManager::ModelManager(QObject *parent) : QObject(parent){

}

/*!
 * \brief ModelManager::init
 */
void ModelManager::init(){

    //init models
    ModelManager::initFeatureTableModels();
    ModelManager::initFeatureTreeViewModels();
    ModelManager::initFunctionTableModels();
    ModelManager::initUnitTypesModels();
    ModelManager::initPluginModels();
    ModelManager::initSensorTableModels();
    ModelManager::initSensorListViewModels();
    ModelManager::initSensorTypeNamesModel();
    ModelManager::initSensorConnectionModels();
    ModelManager::initReadingTypesModels();
    ModelManager::initGroupsModel();
    ModelManager::initMeasurementConfigModels();
    ModelManager::initObservationModels();
    ModelManager::initReadingModels();
    ModelManager::initScalarEntityTypesModels();
    ModelManager::initActualNominalFilterModel();

}

void ModelManager::testInit(QStringList entityTypes,
                            QPointer<MeasurementConfigManager> measurementConfigManager,
                            QList<sdb::Plugin> plugins) {

    ModelManager::plugins = plugins;
    ModelManager::functionsListModel.setPlugins(ModelManager::plugins);

    ModelManager::scalarEntityTypeNamesModel.setStringList(entityTypes);

    ModelManager::setMeasurementConfigManager(measurementConfigManager);
    ModelManager::initMeasurementConfigModels();

}

/*!
 * \brief ModelManager::getCurrentJob
 * \return
 */
const QPointer<OiJob> &ModelManager::getCurrentJob(){
    return ModelManager::currentJob;
}

/*!
 * \brief ModelManager::setCurrentJob
 * \param job
 */
void ModelManager::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){
        ModelManager::currentJob = job;
        ModelManager::updateJob();
    }
}

/*!
 * \brief ModelManager::getSensorConfigManager
 * \return
 */
const QPointer<SensorConfigurationManager> &ModelManager::getSensorConfigManager(){
    return ModelManager::sensorConfigManager;
}

/*!
 * \brief ModelManager::setSensorConfigManager
 * \param sensorConfigManager
 */
void ModelManager::setSensorConfigManager(const QPointer<SensorConfigurationManager> &sensorConfigManager){
    ModelManager::sensorConfigManager = sensorConfigManager;
    ModelManager::updateSensorConfigManager();
}

/*!
 * \brief ModelManager::getMeasurementConfigManager
 * \return
 */
const QPointer<MeasurementConfigManager> &ModelManager::getMeasurementConfigManager(){
    return ModelManager::measurementConfigManager;
}

/*!
 * \brief ModelManager::setMeasurementConfigManager
 * \param measurementConfigManager
 */
void ModelManager::setMeasurementConfigManager(const QPointer<MeasurementConfigManager> &measurementConfigManager){
    ModelManager::measurementConfigManager = measurementConfigManager;
    ModelManager::updateMeasurementConfigManager();
}

/*!
 * \brief ModelManager::getFeatureTableColumnConfig
 * \return
 */
const FeatureTableColumnConfig &ModelManager::getFeatureTableColumnConfig(){
    return ModelManager::featureTableColumnConfig;
}

/*!
 * \brief ModelManager::setFeatureTableColumnConfig
 * \param config
 */
void ModelManager::setFeatureTableColumnConfig(const FeatureTableColumnConfig &config){
    ModelManager::featureTableColumnConfig = config;
    featureTableProxyModel.setFeatureTableColumnConfig(config); // TODO oder war dafür updateFeatureTableColumnConfig() vorgesehen
    ModelManager::updateFeatureTableColumnConfig();
}

/*!
 * \brief ModelManager::getTrafoParamTableColumnConfig
 * \return
 */
const TrafoParamTableColumnConfig &ModelManager::getTrafoParamTableColumnConfig(){
    return ModelManager::trafoParamTableColumnConfig;
}

/*!
 * \brief ModelManager::setTrafoParamColumnConfig
 * \param config
 */
void ModelManager::setTrafoParamColumnConfig(const TrafoParamTableColumnConfig &config){
    ModelManager::trafoParamTableColumnConfig = config;
    ModelManager::updateTrafoParamTableColumnConfig();
}

/*!
 * \brief ModelManager::getObservationTableColumnConfig
 * \return
 */
const ObservationTableColumnConfig &ModelManager::getObservationTableColumnConfig(){
    return ModelManager::observationTableColumnConfig;
}

/*!
 * \brief ModelManager::setObservationTableColumnConfig
 * \param config
 */
void ModelManager::setObservationTableColumnConfig(const ObservationTableColumnConfig &config){
    ModelManager::observationTableColumnConfig = config;
    ModelManager::updateObservationTableColumnConfig();
}

/*!
 * \brief ModelManager::getReadingTableColumnConfig
 * \return
 */
const ReadingTableColumnConfig &ModelManager::getReadingTableColumnConfig(){
    return ModelManager::readingTableColumnConfig;
}

/*!
 * \brief ModelManager::setReadingTableColumnConfig
 * \param config
 */
void ModelManager::setReadingTableColumnConfig(const ReadingTableColumnConfig &config){
    ModelManager::readingTableColumnConfig = config;
    ModelManager::updateReadingTableColumnConfig();
}

/*!
 * \brief ModelManager::getParameterDisplayConfig
 * \return
 */
const ParameterDisplayConfig &ModelManager::getParameterDisplayConfig(){
    return ModelManager::parameterDisplayConfig;
}

/*!
 * \brief ModelManager::setParameterDisplayConfig
 * \param config
 */
void ModelManager::setParameterDisplayConfig(const ParameterDisplayConfig &config){
    ModelManager::parameterDisplayConfig = config;
    ModelManager::updateParameterDisplayConfig();
}

/*!
 * \brief ModelManager::getFeatureTableModel
 * \return
 */
FeatureTableModel &ModelManager::getFeatureTableModel(){
    return ModelManager::featureTableModel;
}

/*!
 * \brief ModelManager::getFeatureTableProxyModel
 * \return
 */
FeatureTableProxyModel &ModelManager::getFeatureTableProxyModel(){
    return ModelManager::featureTableProxyModel;
}

/*!
 * \brief ModelManager::getTrafoParamTableProxyModel
 * \return
 */
TrafoParamTableProxyModel &ModelManager::getTrafoParamTableProxyModel(){
    return ModelManager::trafoParamTableProxyModel;
}

/*!
 * \brief ModelManager::getFeatureTreeViewModel
 * \return
 */
FeatureTreeViewModel &ModelManager::getFeatureTreeViewModel(){
    return ModelManager::featureTreeViewModel;
}

/*!
 * \brief ModelManager::getAvailableElementsTreeViewProxyModel
 * \return
 */
AvailableElementsTreeViewProxyModel &ModelManager::getAvailableElementsTreeViewProxyModel(){
    return ModelManager::availableElementsTreeViewProxyModel;
}

/*!
 * \brief ModelManager::getUsedElementsModel
 * \return
 */
UsedElementsModel &ModelManager::getUsedElementsModel(){
    return ModelManager::usedElementsModel;
}

/*!
 * \brief ModelManager::getBundleParameterTableProxyModel
 * \return
 */
BundleParameterTableProxyModel &ModelManager::getBundleParameterTableProxyModel()
{
    return ModelManager::bundleParameterTableProxyModel;
}

/*!
 * \brief ModelManager::getFeatureDifferenceTableModel
 * \return
 */
FeatureDifferenceTableModel &ModelManager::getFeatureDifferenceTableModel()
{
    return ModelManager::featureDifferenceTableModel;
}

/*!
 * \brief ModelManager::getFeatureDifferenceProxyModel
 * \return
 */
FeatureDifferenceProxyModel &ModelManager::getFeatureDifferenceProxyModel()
{
    return ModelManager::featureDifferenceProxyModel;
}

/*!
 * \brief ModelManager::getCoordinateSystemsModel
 * \return
 */
QStringListModel &ModelManager::getCoordinateSystemsModel(){
    return ModelManager::coordinateSystemsModel;
}

/*!
 * \brief ModelManager::getNominalSystemsModel
 * \return
 */
QStringListModel &ModelManager::getNominalSystemsModel(){
    return ModelManager::nominalSystemsModel;
}

/*!
 * \brief ModelManager::getBundleSystemsModel
 * \return
 */
BundleSystemsModel &ModelManager::getBundleSystemsModel(){
    return ModelManager::bundleSystemsModel;
}

/*!
 * \brief ModelManager::getGroupNamesModel
 * \return
 */
QStringListModel &ModelManager::getGroupNamesModel(){
    return ModelManager::groupNamesModel;
}

/*!
 * \brief ModelManager::getActualNominalFilterModel
 * \return
 */
QStringListModel &ModelManager::getActualNominalFilterModel(){
    return ModelManager::actualNominalFilterModel;
}

/*!
 * \brief ModelManager::getUnitTypesModel
 * \param dimension
 * \return
 */
QStringListModel &ModelManager::getUnitTypesModel(const DimensionType &dimension){
    switch(dimension){
    case eMetric:
        return ModelManager::metricUnitTypesModel;
    case eAngular:
        return ModelManager::angularUnitTypesModel;
    case eTemperature:
        return ModelManager::temperatureUnitTypesModel;
    default:
        return ModelManager::dummyStringListModel;
    }
}

/*!
 * \brief ModelManager::getPluginNamesModel
 * \return
 */
QStringListModel &ModelManager::getPluginNamesModel(){
    return ModelManager::pluginNamesModel;
}

/*!
 * \brief ModelManager::getPluginTreeViewModel
 * \return
 */
PluginTreeViewModel &ModelManager::getPluginTreeViewModel(){
    return ModelManager::pluginTreeViewModel;
}

/*!
 * \brief ModelManager::getReadingTypeNamesModel
 * \return
 */
QStringListModel &ModelManager::getReadingTypeNamesModel(){
    return ModelManager::readingTypeNamesModel;
}

/*!
 * \brief ModelManager::getScalarEntityTypeNamesModel
 * \return
 */
QStringListModel &ModelManager::getScalarEntityTypeNamesModel(){
    return ModelManager::scalarEntityTypeNamesModel;
}

/*!
 * \brief ModelManager::getFunctionTableModel
 * \return
 */
FunctionTableModel &ModelManager::getFunctionTableModel(){
    return ModelManager::functionTableModel;
}

/*!
 * \brief ModelManager::getFunctionTableProxyModel
 * \return
 */
FunctionTableProxyModel &ModelManager::getFunctionTableProxyModel(){
    return ModelManager::functionTableProxyModel;
}

/*!
 * \brief ModelManager::getActiveFeatureFunctionsModel
 * \return
 */
ActiveFeatureFunctionsModel &ModelManager::getActiveFeatureFunctionsModel(){
    return ModelManager::activeFeatureFunctionsModel;
}

/*!
 * \brief ModelManager::getFunctionWeightTableModel
 * \return
 */
FunctionWeightsTableModel &ModelManager::getFunctionWeightTableModel()
{
    return ModelManager::functionWeightsTableModel;
}

/*!
 * \brief ModelManager::getFunctionWeightProxyModel
 * \return
 */
FunctionWeightProxyModel &ModelManager::getFunctionWeightProxyModel()
{
    return ModelManager::functionWeightProxyModel;
}

/*!
 * \brief ModelManager::getSensorTypeNamesModel
 * \return
 */
QStringListModel &ModelManager::getSensorTypeNamesModel(){
    return ModelManager::sensorTypeNamesModel;
}

/*!
 * \brief ModelManager::getSensorTableModel
 * \return
 */
SensorTableModel &ModelManager::getSensorTableModel(){
    return ModelManager::sensorTableModel;
}

/*!
 * \brief ModelManager::getSensorTableProxyModel
 * \return
 */
SensorTableProxyModel &ModelManager::getSensorTableProxyModel(){
    return ModelManager::sensorTableProxyModel;
}

/*!
 * \brief ModelManager::getBaudRateTypesModel
 * \return
 */
QStandardItemModel &ModelManager::getBaudRateTypesModel(){
    return ModelManager::baudRateTypesModel;
}

/*!
 * \brief ModelManager::getDataBitTypesModel
 * \return
 */
QStandardItemModel &ModelManager::getDataBitTypesModel(){
    return ModelManager::dataBitTypesModel;
}

/*!
 * \brief ModelManager::getFlowControlTypesModel
 * \return
 */
QStandardItemModel &ModelManager::getFlowControlTypesModel(){
    return ModelManager::flowControlTypesModel;
}

/*!
 * \brief ModelManager::getParityTypesModel
 * \return
 */
QStandardItemModel &ModelManager::getParityTypesModel(){
    return ModelManager::parityTypesModel;
}

/*!
 * \brief ModelManager::getStopBitTypesModel
 * \return
 */
QStandardItemModel &ModelManager::getStopBitTypesModel(){
    return ModelManager::stopBitTypesModel;
}

/*!
 * \brief ModelManager::getAvailableSerialPortsModel
 * \return
 */
QStandardItemModel &ModelManager::getAvailableSerialPortsModel(){
    return ModelManager::availableSerialPortsModel;
}

/*!
 * \brief ModelManager::getAvailableIpAdressesModel
 * \return
 */
QStandardItemModel &ModelManager::getAvailableIpAdressesModel(){
    return ModelManager::availableIpAdressesModel;
}

/*!
 * \brief ModelManager::getMeasurementConfigurationModel
 * \return
 */
MeasurementConfigurationModel &ModelManager::getMeasurementConfigurationModel(){
    return ModelManager::measurementConfigurationModel;
}

/*!
 * \brief ModelManager::getMeasurementConfigurationProxyModel
 * \return
 */
MeasurementConfigurationProxyModel &ModelManager::getMeasurementConfigurationProxyModel(){
    return ModelManager::measurementConfigurationProxyModel;
}

/*!
 * \brief ModelManager::getObservationModel
 * \return
 */
ObservationModel &ModelManager::getObservationModel(){
    return ModelManager::observationModel;
}

/*!
 * \brief ModelManager::getObservationProxyModel
 * \return
 */
ObservationProxyModel &ModelManager::getObservationProxyModel(){
    return ModelManager::observationProxyModel;
}

/*!
 * \brief ModelManager::getReadingModel
 * \return
 */
ReadingModel &ModelManager::getReadingModel(){
    return ModelManager::readingModel;
}

/*!
 * \brief ModelManager::getReadingProxyModel
 * \return
 */
ReadingProxyModel &ModelManager::getReadingProxyModel(){
    return ModelManager::readingProxyModel;
}

/*!
 * \brief ModelManager::getBundleTemplatesModel
 * \return
 */
BundleTemplatesModel &ModelManager::getBundleTemplatesModel(){
    return ModelManager::bundleTemplatesModel;
}

/*!
 * \brief ModelManager::getExchangeSimpleAsciiNames
 * \param pluginName
 * \return
 */
QPointer<QStringListModel> ModelManager::getExchangeSimpleAsciiNames(const QString &pluginName){

    QPointer<QStringListModel> model(NULL);

    QStringList exchanges;
    foreach(const sdb::Plugin &plugin, ModelManager::plugins){

        //check if the plugin is the one that was queried and add all simple ascii exchanges
        if(plugin.name.compare(pluginName) == 0){
            foreach(const sdb::Exchange &exchange, plugin.exchanges){
                if(exchange.iid.compare(OiMetaData::iid_ExchangeSimpleAscii) == 0){
                    exchanges.append(exchange.name);
                }
            }
        }

    }
    model = new QStringListModel(exchanges);

    return model;

}

/*!
 * \brief ModelManager::getExchangeSimpleAsciiDelimiters
 * \param pluginName
 * \param exchangeName
 * \return
 */
QPointer<QStringListModel> ModelManager::getExchangeSimpleAsciiDelimiters(const QString &pluginName, const QString &exchangeName){

    QPointer<QStringListModel> model(NULL);

    QStringList delimiters;

    //get plugin file path
    QString path;
    foreach(const sdb::Plugin &plugin, ModelManager::plugins){
        if(plugin.name.compare(pluginName) == 0){
            path = plugin.file_path;
            break;
        }
    }

    //get and check exchange plugin
    QPointer<ExchangeSimpleAscii> exchange = PluginLoader::loadExchangeSimpleAsciiPlugin(path, exchangeName);
    if(!exchange.isNull()){
        delimiters = exchange->getSupportedDelimiters();
    }

    model = new QStringListModel(delimiters);

    return model;

}

/*!
 * \brief ModelManager::getExchangeSimpleAsciiSupportedGeometries
 * \param pluginName
 * \param exchangeName
 * \return
 */
QPointer<QStringListModel> ModelManager::getExchangeSimpleAsciiSupportedGeometries(const QString &pluginName, const QString &exchangeName){

    QStringList supportedGeometries;

    //get plugin file path
    QString path;
    foreach(const sdb::Plugin &plugin, ModelManager::plugins){
        if(plugin.name.compare(pluginName) == 0){
            path = plugin.file_path;
            break;
        }
    }

    //get and check exchange plugin
    QPointer<ExchangeSimpleAscii> exchange = PluginLoader::loadExchangeSimpleAsciiPlugin(path, exchangeName);
    if(!exchange.isNull()){
        foreach(const GeometryTypes &type, exchange->getSupportedGeometries()){
            supportedGeometries.append(getGeometryTypeName(type));
        }
    }

    return new QStringListModel(supportedGeometries);

}

/*!
 * \brief ModelManager::getAvailableFunctionsProxyModel
 * \return
 */
QPointer<AvailableFunctionsListProxyModel> ModelManager::getAvailableFunctionsProxyModel(){
    QPointer<AvailableFunctionsListProxyModel> model = new AvailableFunctionsListProxyModel();
    model->setSourceModel(&ModelManager::functionsListModel);
    return model;
}

/*!
 * \brief ModelManager::getFunctionStatisticModel
 * \return
 */
QPointer<FunctionStatisticModel> ModelManager::getFunctionStatisticModel(){

    //check the current job
    if(ModelManager::currentJob.isNull() || ModelManager::currentJob->getActiveFeature().isNull()){
        return QPointer<FunctionStatisticModel>();
    }

    QPointer<Feature> activeFeature = ModelManager::currentJob->getActiveFeature()->getFeature();

    QPointer<FunctionStatisticModel> model = new FunctionStatisticModel(activeFeature);
    model->setParameterDisplayConfig(ModelManager::parameterDisplayConfig);
    return model;
}

/*!
 * \brief ModelManager::getSensorConfigurationModel
 * \return
 */
QPointer<SensorConfigurationModel> ModelManager::getSensorConfigurationModel(QObject *parent){

    //create sensor config model
    QPointer<SensorConfigurationModel> model = new SensorConfigurationModel(parent);

    //assign current sensor config manager
    if(!ModelManager::sensorConfigManager.isNull()){
        SensorConfigurationManager manager(*ModelManager::sensorConfigManager.data());
        model->setSensorConfigurationManager(manager);
    }

    return model;

}

/*!
 * \brief ModelManager::getSensorConfigurationProxyModel
 * \param sourceModel
 * \return
 */
QPointer<SensorConfigurationProxyModel> ModelManager::getSensorConfigurationProxyModel(QPointer<SensorConfigurationModel> sourceModel){

    //check source model
    if(sourceModel.isNull()){
        return QPointer<SensorConfigurationProxyModel>();
    }

    QPointer<SensorConfigurationProxyModel> model = new SensorConfigurationProxyModel(sourceModel->parent());
    model->setSourceModel(sourceModel);

    return model;

}

/*!
 * \brief ModelManager::getSensorAccuracyModel
 * \return
 */
QPointer<SensorAccuracyModel> ModelManager::getSensorAccuracyModel(QObject *parent){

    //create and connect model
    QPointer<SensorAccuracyModel> model = new SensorAccuracyModel(parent);
    if(!ModelManager::myInstance.isNull()){
        QObject::connect(ModelManager::myInstance, &ModelManager::parameterDisplayConfigChanged,
                         model, &SensorAccuracyModel::setParameterDisplayConfig, Qt::AutoConnection);
    }

    return model;

}

/*!
 * \brief ModelManager::getSensorParametersModel
 * \param parent
 * \return
 */
QPointer<SensorParametersModel> ModelManager::getSensorParametersModel(QObject *parent){

    //create and connect model
    QPointer<SensorParametersModel> model = new SensorParametersModel(parent);
    if(!ModelManager::myInstance.isNull()){
        QObject::connect(ModelManager::myInstance, &ModelManager::parameterDisplayConfigChanged,
                         model, &SensorParametersModel::setParameterDisplayConfig, Qt::AutoConnection);
    }

    return model;

}

/*!
 * \brief ModelManager::getBundleStationsModel
 * \param parent
 * \return
 */
QPointer<BundleStationsModel> ModelManager::getBundleStationsModel(QObject *parent){
    QPointer<BundleStationsModel> model = new BundleStationsModel(parent);
    return model;
}

/*!
 * \brief ModelManager::getBundleGeometriesModel
 * \param parent
 * \return
 */
QPointer<BundleGeometriesModel> ModelManager::getBundleGeometriesModel(QObject *parent){
    QPointer<BundleGeometriesModel> model = new BundleGeometriesModel(parent);
    return model;
}

/*!
 * \brief ModelManager::featureSetChanged
 */
void ModelManager::featureSetChanged(){

    //check the current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //resort table model
    ModelManager::featureTableProxyModel.sort(0);

}

/*!
 * \brief ModelManager::coordSystemSetChanged
 */
void ModelManager::coordSystemSetChanged(){

    //check the current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //update coordinate systems model
    ModelManager::updateCoordinateSystemsModel();

    //update nominal systems model
    ModelManager::updateNominalSystemsModel();

}

/*!
 * \brief ModelManager::stationSetChanged
 */
void ModelManager::stationSetChanged(){

    //check the current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //update coordinate systems model
    ModelManager::updateCoordinateSystemsModel();

}

/*!
 * \brief ModelManager::availableGroupsChanged
 */
void ModelManager::availableGroupsChanged(){

    //check the current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //update groups model
    ModelManager::updateGroupsModel();

}

/*!
 * \brief ModelManager::featureNameChanged
 * \param featureId
 * \param oldName
 */
void ModelManager::featureNameChanged(const int &featureId, const QString &oldName){

    //check the current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //update coordinate systems model if the name of a coordinate system has been changed
    QPointer<FeatureWrapper> feature = ModelManager::currentJob->getFeatureById(featureId);
    if(!feature.isNull() && (!feature->getCoordinateSystem().isNull() || !feature->getStation().isNull())){
        ModelManager::updateCoordinateSystemsModel();
        ModelManager::updateNominalSystemsModel();
    }

    //resort table model
    //ModelManager::featureTableProxyModel.sort(0);

}

/*!
 * \brief ModelManager::sensorConfigurationsChanged
 */
/*void ModelManager::sensorConfigurationsChanged(){

    ModelManager::updateSensorConfigNamesModel();

}*/

/*!
 * \brief ModelManager::updateJob
 * Passes the new job to all static models
 */
void ModelManager::updateJob(){

    //check the current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //pass the job to all static models that need it
    ModelManager::featureTableModel.setCurrentJob(ModelManager::currentJob);

    ModelManager::featureTableProxyModel.setCurrentJob(ModelManager::currentJob);

    ModelManager::featureTreeViewModel.setCurrentJob(ModelManager::currentJob);
    ModelManager::activeFeatureFunctionsModel.setCurrentJob(ModelManager::currentJob);
    ModelManager::functionTableProxyModel.setCurrentJob(ModelManager::currentJob);
    ModelManager::usedElementsModel.setCurrentJob(ModelManager::currentJob);
    ModelManager::availableElementsTreeViewProxyModel.setCurrentJob(ModelManager::currentJob);
    ModelManager::observationModel.setCurrentJob(ModelManager::currentJob);
    ModelManager::readingModel.setCurrentJob(ModelManager::currentJob);
    ModelManager::bundleSystemsModel.setCurrentJob(ModelManager::currentJob);
    ModelManager::featureDifferenceTableModel.setCurrentJob(ModelManager::currentJob);
    ModelManager::featureDifferenceProxyModel.setCurrentJob(ModelManager::currentJob);

    ModelManager::functionWeightsTableModel.setCurrentJob(ModelManager::currentJob);

    //connect the job to slots in model manager
    QObject::connect(ModelManager::currentJob.data(), &OiJob::coordSystemSetChanged, ModelManager::myInstance.data(), &ModelManager::coordSystemSetChanged, Qt::AutoConnection);
    QObject::connect(ModelManager::currentJob.data(), &OiJob::stationSetChanged, ModelManager::myInstance.data(), &ModelManager::stationSetChanged, Qt::AutoConnection);
    QObject::connect(ModelManager::currentJob.data(), &OiJob::availableGroupsChanged, ModelManager::myInstance.data(), &ModelManager::availableGroupsChanged, Qt::AutoConnection);
    QObject::connect(ModelManager::currentJob.data(), &OiJob::featureSetChanged, ModelManager::myInstance.data(), &ModelManager::featureSetChanged, Qt::AutoConnection);
    QObject::connect(ModelManager::currentJob.data(), &OiJob::featureNameChanged, ModelManager::myInstance.data(), &ModelManager::featureNameChanged, Qt::AutoConnection);

    //update models
    ModelManager::myInstance->featureSetChanged();
    ModelManager::myInstance->coordSystemSetChanged();
    ModelManager::myInstance->availableGroupsChanged();

}

/*!
 * \brief ModelManager::updateFeatureTableColumnConfig
 */
void ModelManager::updateFeatureTableColumnConfig(){

}

/*!
 * \brief ModelManager::updateTrafoParamTableColumnConfig
 */
void ModelManager::updateTrafoParamTableColumnConfig(){

}

/*!
 * \brief ModelManager::updateObservationTableColumnConfig
 */
void ModelManager::updateObservationTableColumnConfig(){

}

/*!
 * \brief ModelManager::updateReadingTableColumnConfig
 */
void ModelManager::updateReadingTableColumnConfig(){

}

/*!
 * \brief ModelManager::updateParameterDisplayConfig
 * Passes the new parameter display config to all static models
 */
void ModelManager::updateParameterDisplayConfig(){

    //pass the parameter display config to all static models that need it
    ModelManager::featureTableModel.setParameterDisplayConfig(ModelManager::parameterDisplayConfig);
    ModelManager::observationModel.setParameterDisplayConfig(ModelManager::parameterDisplayConfig);
    ModelManager::readingModel.setParameterDisplayConfig(ModelManager::parameterDisplayConfig);
    ModelManager::featureDifferenceTableModel.setParameterDisplayConfig(ModelManager::parameterDisplayConfig);

    if(!ModelManager::myInstance.isNull()){
        emit ModelManager::myInstance->parameterDisplayConfigChanged(ModelManager::parameterDisplayConfig);
    }

}

/*!
 * \brief ModelManager::updateSensorConfigManager
 */
void ModelManager::updateSensorConfigManager(){

}

/*!
 * \brief ModelManager::updateMeasurementConfigManager
 */
void ModelManager::updateMeasurementConfigManager(){

    //pass the measurement config manager to all static models that need it
    ModelManager::measurementConfigurationModel.setMeasurementConfigurationManager(ModelManager::measurementConfigManager);
    ModelManager::featureTableModel.setMeasurementConfigManager(ModelManager::measurementConfigManager);

}

/*!
 * \brief ModelManager::updateCoordinateSystemsModel
 */
void ModelManager::updateCoordinateSystemsModel(){

    QStringList coordSystems;

    //check current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //add all nominal systems
    foreach(const QPointer<CoordinateSystem> &system, ModelManager::currentJob->getCoordinateSystemsList()){

        //check system
        if(system.isNull()){
            continue;
        }

        //add system's name
        coordSystems.append(system->getFeatureName());

    }

    //add all station systems
    foreach(const QPointer<CoordinateSystem> &system, ModelManager::currentJob->getStationSystemsList()){

        //check system
        if(system.isNull()){
            continue;
        }

        //add system's name
        coordSystems.append(system->getFeatureName());

    }

    ModelManager::coordinateSystemsModel.setStringList(coordSystems);

}

/*!
 * \brief ModelManager::updateNominalSystemsModel
 */
void ModelManager::updateNominalSystemsModel(){

    QStringList coordSystems;

    //check current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //add all nominal systems
    foreach(const QPointer<CoordinateSystem> &system, ModelManager::currentJob->getCoordinateSystemsList()){

        //check system
        if(system.isNull()){
            continue;
        }

        //add system's name
        coordSystems.append(system->getFeatureName());

    }

    ModelManager::nominalSystemsModel.setStringList(coordSystems);

}

/*!
 * \brief ModelManager::updateGroupsModel
 */
void ModelManager::updateGroupsModel(){

    QStringList groups;

    //get a list of all available groups
    if(!ModelManager::currentJob.isNull()){
        groups = ModelManager::currentJob->getFeatureGroupList();
        groups.sort();
    }

    //add default entry (all groups) if not exists
    if(groups.size() == 0){
        groups.append("All Groups");
    }
    if(groups.first().compare("All Groups") != 0){
        groups.push_front("All Groups");
    }

    if(groups.size() == 1 && !groups.contains("Group01")){
        groups.append("Group01");
    }

    ModelManager::groupNamesModel.setStringList(groups);

}

/*!
 * \brief ModelManager::initFeatureTableModels
 */
void ModelManager::initFeatureTableModels(){

    //assign source models
    ModelManager::featureTableProxyModel.setSourceModel(&ModelManager::featureTableModel);
    ModelManager::trafoParamTableProxyModel.setSourceModel(&ModelManager::featureTableModel);
    ModelManager::bundleParameterTableProxyModel.setSourceModel(&ModelManager::featureTableModel);
    ModelManager::featureDifferenceProxyModel.setSourceModel(&ModelManager::featureDifferenceTableModel);

    //connect models
    if(ModelManager::myInstance.isNull()){
        QObject::connect(&ModelManager::featureTableModel, &FeatureTableModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
        QObject::connect(&ModelManager::featureTableProxyModel, &FeatureTableProxyModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
        QObject::connect(&ModelManager::trafoParamTableProxyModel, &TrafoParamTableProxyModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
        QObject::connect(&ModelManager::bundleParameterTableProxyModel, &BundleParameterTableProxyModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
        QObject::connect(&ModelManager::featureDifferenceTableModel, &FeatureDifferenceTableModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
        QObject::connect(&ModelManager::featureDifferenceProxyModel, &FeatureDifferenceProxyModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
    }

}

/*!
 * \brief ModelManager::initFeatureTreeViewModels
 */
void ModelManager::initFeatureTreeViewModels(){

    //assign source models
    ModelManager::availableElementsTreeViewProxyModel.setSourceModel(&ModelManager::featureTreeViewModel);

    ModelManager::functionWeightProxyModel.setSourceModel(&ModelManager::functionWeightsTableModel);

    //connect models
    if(ModelManager::myInstance.isNull()){
        QObject::connect(&ModelManager::featureTreeViewModel, &FeatureTreeViewModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
        QObject::connect(&ModelManager::availableElementsTreeViewProxyModel, &AvailableElementsTreeViewProxyModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
    }

    //set header data
    //ModelManager::usedElementsModel.setHeaderData(1, Qt::Horizontal, "used elements");

}

/*!
 * \brief ModelManager::initFunctionTableModels
 */
void ModelManager::initFunctionTableModels(){

    //assign source models
    ModelManager::functionTableProxyModel.setSourceModel(&ModelManager::functionTableModel);

    //connect models
    if(ModelManager::myInstance.isNull()){
        QObject::connect(&ModelManager::functionTableModel, &FunctionTableModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
        QObject::connect(&ModelManager::functionTableProxyModel, &FunctionTableProxyModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
    }

}

/*!
 * \brief ModelManager::initSensorTypeNamesModel
 */
void ModelManager::initSensorTypeNamesModel(){

    QStringList sensorTypes;

    foreach(const SensorTypes &type, getAvailableSensorTypes()){
        sensorTypes.append(getSensorTypeName(type));
    }

    ModelManager::sensorTypeNamesModel.setStringList(sensorTypes);

}

/*!
 * \brief ModelManager::initSensorTableModels
 */
void ModelManager::initSensorTableModels(){

    //assign source models
    ModelManager::sensorTableProxyModel.setSourceModel(&ModelManager::sensorTableModel);

    //connect models
    if(ModelManager::myInstance.isNull()){
        QObject::connect(&ModelManager::sensorTableModel, &SensorTableModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
        QObject::connect(&ModelManager::sensorTableProxyModel, &SensorTableProxyModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
    }

}

/*!
 * \brief ModelManager::initSensorListViewModels
 */
void ModelManager::initSensorListViewModels(){
/*
    //assign source models
    ModelManager::sensorConfigurationProxyModel.setSourceModel(&ModelManager::sensorConfigurationModel);

    //connect models
    if(ModelManager::myInstance.isNull()){
        QObject::connect(&ModelManager::sensorConfigurationModel, &SensorConfigurationModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
        QObject::connect(&ModelManager::sensorConfigurationProxyModel, &SensorConfigurationProxyModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
    }
*/
}

/*!
 * \brief ModelManager::initMeasurementConfigModels
 */
void ModelManager::initMeasurementConfigModels(){

    //assign source models
    ModelManager::measurementConfigurationProxyModel.setSourceModel(&ModelManager::measurementConfigurationModel);

    //connect models
    if(ModelManager::myInstance.isNull()){
        QObject::connect(&ModelManager::measurementConfigurationModel, &MeasurementConfigurationModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
        QObject::connect(&ModelManager::measurementConfigurationProxyModel, &MeasurementConfigurationProxyModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
    }

}

/*!
 * \brief ModelManager::initSensorConnectionModels
 */
void ModelManager::initSensorConnectionModels(){

    //initialize baud rate model
    QStandardItem *baud1200 = new QStandardItem("1200");
    baud1200->setData(QSerialPort::Baud1200, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud1200);
    QStandardItem *baud2400 = new QStandardItem("2400");
    baud2400->setData(QSerialPort::Baud2400, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud2400);
    QStandardItem *baud4800 = new QStandardItem("4800");
    baud4800->setData(QSerialPort::Baud4800, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud4800);
    QStandardItem *baud9600 = new QStandardItem("9600");
    baud9600->setData(QSerialPort::Baud9600, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud9600);
    QStandardItem *baud19200 = new QStandardItem("19200");
    baud19200->setData(QSerialPort::Baud19200, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud19200);
    QStandardItem *baud38400 = new QStandardItem("38400");
    baud38400->setData(QSerialPort::Baud38400, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud38400);
    QStandardItem *baud57600 = new QStandardItem("57600");
    baud57600->setData(QSerialPort::Baud57600, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud57600);
    QStandardItem *baud115200 = new QStandardItem("115200");
    baud115200->setData(QSerialPort::Baud115200, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud115200);

    //initialize databits model
    QStandardItem *data5 = new QStandardItem("5");
    data5->setData(QSerialPort::Data5, Qt::UserRole);
    ModelManager::dataBitTypesModel.appendRow(data5);
    QStandardItem *data6 = new QStandardItem("6");
    data6->setData(QSerialPort::Data6, Qt::UserRole);
    ModelManager::dataBitTypesModel.appendRow(data6);
    QStandardItem *data7 = new QStandardItem("7");
    data7->setData(QSerialPort::Data7, Qt::UserRole);
    ModelManager::dataBitTypesModel.appendRow(data7);
    QStandardItem *data8 = new QStandardItem("8");
    data8->setData(QSerialPort::Data8, Qt::UserRole);
    ModelManager::dataBitTypesModel.appendRow(data8);

    //initialize flow control model
    QStandardItem *noControl = new QStandardItem("no flowcontrol");
    noControl->setData(QSerialPort::NoFlowControl, Qt::UserRole);
    ModelManager::flowControlTypesModel.appendRow(noControl);
    QStandardItem *hardwareControl = new QStandardItem("hardware flowcontrol");
    hardwareControl->setData(QSerialPort::HardwareControl, Qt::UserRole);
    ModelManager::flowControlTypesModel.appendRow(hardwareControl);
    QStandardItem *softwareControl = new QStandardItem("software flowcontrol");
    softwareControl->setData(QSerialPort::SoftwareControl, Qt::UserRole);
    ModelManager::flowControlTypesModel.appendRow(softwareControl);
    QStandardItem *unknownControl = new QStandardItem("unknown flowcontrol");
    unknownControl->setData(QSerialPort::UnknownFlowControl, Qt::UserRole);
    ModelManager::flowControlTypesModel.appendRow(unknownControl);

    //initialize parity model
    QStandardItem *noParity = new QStandardItem("no parity");
    noParity->setData(QSerialPort::NoParity, Qt::UserRole);
    ModelManager::parityTypesModel.appendRow(noParity);
    QStandardItem *evenParity = new QStandardItem("even parity");
    evenParity->setData(QSerialPort::EvenParity, Qt::UserRole);
    ModelManager::parityTypesModel.appendRow(evenParity);
    QStandardItem *oddParity = new QStandardItem("odd parity");
    oddParity->setData(QSerialPort::OddParity, Qt::UserRole);
    ModelManager::parityTypesModel.appendRow(oddParity);
    QStandardItem *spaceParity = new QStandardItem("space parity");
    spaceParity->setData(QSerialPort::SpaceParity, Qt::UserRole);
    ModelManager::parityTypesModel.appendRow(spaceParity);
    QStandardItem *markParity = new QStandardItem("mark parity");
    markParity->setData(QSerialPort::MarkParity, Qt::UserRole);
    ModelManager::parityTypesModel.appendRow(markParity);
    QStandardItem *unknownParity = new QStandardItem("unknown parity");
    unknownParity->setData(QSerialPort::UnknownParity, Qt::UserRole);
    ModelManager::parityTypesModel.appendRow(unknownParity);

    //initialize stop bits model
    QStandardItem *oneStop = new QStandardItem("one stop");
    oneStop->setData(QSerialPort::OneStop, Qt::UserRole);
    ModelManager::stopBitTypesModel.appendRow(oneStop);
    QStandardItem *oneHalfStop = new QStandardItem("one and half");
    oneHalfStop->setData(QSerialPort::OneAndHalfStop, Qt::UserRole);
    ModelManager::stopBitTypesModel.appendRow(oneHalfStop);
    QStandardItem *twoStop = new QStandardItem("two stop");
    twoStop->setData(QSerialPort::TwoStop, Qt::UserRole);
    ModelManager::stopBitTypesModel.appendRow(twoStop);
    QStandardItem *unknownStop = new QStandardItem("unknown stopbits");
    unknownStop->setData(QSerialPort::UnknownStopBits, Qt::UserRole);
    ModelManager::stopBitTypesModel.appendRow(unknownStop);

    //initialize serial ports model
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        QStandardItem *serialPort = new QStandardItem(info.portName());
        ModelManager::availableSerialPortsModel.appendRow(serialPort);
    }

    //initialize ip4 addresses model
    QList<QHostAddress> ipAdresses = QNetworkInterface::allAddresses();
    foreach(const QHostAddress &adress, ipAdresses){
        if(adress.protocol() == QAbstractSocket::IPv4Protocol){
            QStandardItem *ip = new QStandardItem(adress.toString());
            ModelManager::availableIpAdressesModel.appendRow(ip);
        }
    }

}

/*!
 * \brief ModelManager::initUnitTypesModels
 */
void ModelManager::initUnitTypesModels(){

    QStringList metricUnitTypes;
    metricUnitTypes.append(getUnitTypeName(eUnitInch));
    metricUnitTypes.append(getUnitTypeName(eUnitMeter));
    metricUnitTypes.append(getUnitTypeName(eUnitMilliMeter));
    ModelManager::metricUnitTypesModel.setStringList(metricUnitTypes);

    QStringList angularUnitTypes;
    angularUnitTypes.append(getUnitTypeName(eUnitArcSeconds));
    angularUnitTypes.append(getUnitTypeName(eUnitDecimalDegree));
    angularUnitTypes.append(getUnitTypeName(eUnitGon));
    angularUnitTypes.append(getUnitTypeName(eUnitMilliRadians));
    angularUnitTypes.append(getUnitTypeName(eUnitRadiant));
    ModelManager::angularUnitTypesModel.setStringList(angularUnitTypes);

    QStringList temperatureUnitTypes;
    temperatureUnitTypes.append(getUnitTypeName(eUnitFahrenheit));
    temperatureUnitTypes.append(getUnitTypeName(eUnitGrad));
    temperatureUnitTypes.append(getUnitTypeName(eUnitKelvin));
    ModelManager::temperatureUnitTypesModel.setStringList(temperatureUnitTypes);

}

/*!
 * \brief ModelManager::initReadingTypesModels
 */
void ModelManager::initReadingTypesModels(){

    QStringList readingTypes;
    readingTypes.append(getReadingTypeName(eCartesianReading));
    readingTypes.append(getReadingTypeName(eDistanceReading));
    readingTypes.append(getReadingTypeName(ePolarReading));
    readingTypes.append(getReadingTypeName(eDirectionReading));
    readingTypes.append(getReadingTypeName(eTemperatureReading));
    readingTypes.append(getReadingTypeName(eLevelReading));
    ModelManager::readingTypeNamesModel.setStringList(readingTypes);

}

/*!
 * \brief ModelManager::initScalarEntityTypesModels
 */
void ModelManager::initScalarEntityTypesModels(){

    QStringList scalarEntityTypes;
    scalarEntityTypes.append(getFeatureTypeName(eScalarEntityAngleFeature));
    scalarEntityTypes.append(getFeatureTypeName(eScalarEntityDistanceFeature));
    scalarEntityTypes.append(getFeatureTypeName(eScalarEntityMeasurementSeriesFeature));
    scalarEntityTypes.append(getFeatureTypeName(eScalarEntityTemperatureFeature));
    ModelManager::scalarEntityTypeNamesModel.setStringList(scalarEntityTypes);

}

/*!
 * \brief ModelManager::initPluginModels
 */
void ModelManager::initPluginModels(){

    //get plugins from database
    ModelManager::plugins = SystemDbManager::getPlugins();

    //pass the plugins list to all static models that need it
    ModelManager::functionTableModel.setPlugins(ModelManager::plugins);
    ModelManager::sensorTableModel.setPlugins(ModelManager::plugins);
    ModelManager::pluginTreeViewModel.setPlugins(ModelManager::plugins);
    ModelManager::functionsListModel.setPlugins(ModelManager::plugins);

    //update plugin names model
    QStringList pluginNames;
    foreach(const sdb::Plugin &plugin, plugins){
        pluginNames.append(plugin.name);
    }
    ModelManager::pluginNamesModel.setStringList(pluginNames);

}

/*!
 * \brief ModelManager::initGroupsModel
 */
void ModelManager::initGroupsModel(){

    ModelManager::updateGroupsModel();

}

/*!
 * \brief ModelManager::initActualNominalFilterModel
 */
void ModelManager::initActualNominalFilterModel(){

    //create filter options
    QList<ActualNominalFilter> actualNominalFilters = getAvailableActualNominalFilters();
    QStringList items;
    foreach(const ActualNominalFilter &filter, actualNominalFilters){
        items.append(getActualNominalFilterName(filter));
    }

    ModelManager::actualNominalFilterModel.setStringList(items);

}

/*!
 * \brief ModelManager::initObservationModels
 */
void ModelManager::initObservationModels(){

    //assign source models
    ModelManager::observationProxyModel.setSourceModel(&ModelManager::observationModel);

    //connect models
    if(ModelManager::myInstance.isNull()){
        QObject::connect(&ModelManager::observationModel, &ObservationModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
        QObject::connect(&ModelManager::observationProxyModel, &ObservationProxyModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
    }

}

/*!
 * \brief ModelManager::initReadingModels
 */
void ModelManager::initReadingModels(){

    //assign source models
    ModelManager::readingProxyModel.setSourceModel(&ModelManager::readingModel);

    //connect models
    if(ModelManager::myInstance.isNull()){
        QObject::connect(&ModelManager::readingModel, &ReadingModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
        QObject::connect(&ModelManager::readingProxyModel, &ReadingProxyModel::sendMessage, ModelManager::myInstance.data(), &ModelManager::sendMessage, Qt::AutoConnection);
    }

}
