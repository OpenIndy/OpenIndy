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
QList<sdb::Plugin> ModelManager::plugins;

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
    ModelManager::initUnitTypesModels();
    ModelManager::initPluginModels();

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
 * \brief ModelManager::getFeatureTreeViewModel
 * \return
 */
FeatureTreeViewModel &ModelManager::getFeatureTreeViewModel(){
    return ModelManager::featureTreeViewModel;
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
 * \brief ModelManager::getGroupNamesModel
 * \return
 */
QStringListModel &ModelManager::getGroupNamesModel(){
    return ModelManager::groupNamesModel;
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

    QPointer<QStringListModel> model(NULL);

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

    model = new QStringListModel(supportedGeometries);

    return model;

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

}

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
    ModelManager::featureTreeViewModel.setCurrentJob(ModelManager::currentJob);

    //connect the job to slots in model manager
    QObject::connect(ModelManager::currentJob.data(), &OiJob::coordSystemSetChanged, ModelManager::myInstance.data(), &ModelManager::coordSystemSetChanged, Qt::AutoConnection);
    QObject::connect(ModelManager::currentJob.data(), &OiJob::stationSetChanged, ModelManager::myInstance.data(), &ModelManager::stationSetChanged, Qt::AutoConnection);
    QObject::connect(ModelManager::currentJob.data(), &OiJob::availableGroupsChanged, ModelManager::myInstance.data(), &ModelManager::availableGroupsChanged, Qt::AutoConnection);

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
 * \brief ModelManager::updateParameterDisplayConfig
 * Passes the new parameter display config to all static models
 */
void ModelManager::updateParameterDisplayConfig(){

    //pass the parameter display config to all static models that need it
    ModelManager::featureTableModel.setParameterDisplayConfig(ModelManager::parameterDisplayConfig);

}

/*!
 * \brief ModelManager::updateCoordinateSystemsModel
 */
void ModelManager::updateCoordinateSystemsModel(){

    QStringList coordSystems;

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

}

/*!
 * \brief ModelManager::initFeatureTableModels
 */
void ModelManager::initFeatureTableModels(){

    //assign source models
    ModelManager::featureTableProxyModel.setSourceModel(&ModelManager::featureTableModel);
    ModelManager::featureTableProxyModel.setDynamicSortFilter(true);

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
 * \brief ModelManager::initPluginModels
 */
void ModelManager::initPluginModels(){

    //get plugins from database
    ModelManager::plugins = SystemDbManager::getPlugins();

    //update plugin names model
    QStringList pluginNames;
    foreach(const sdb::Plugin &plugin, plugins){
        pluginNames.append(plugin.name);
    }
    ModelManager::pluginNamesModel.setStringList(pluginNames);

}
