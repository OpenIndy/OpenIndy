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

    //connect the job to slots in model manager
    QObject::connect(ModelManager::currentJob.data(), SIGNAL(coordSystemSetChanged()), ModelManager::myInstance.data(), SLOT(coordSystemSetChanged()), Qt::AutoConnection);
    QObject::connect(ModelManager::currentJob.data(), SIGNAL(stationSetChanged()), ModelManager::myInstance.data(), SLOT(stationSetChanged()), Qt::AutoConnection);

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
 * \brief ModelManager::initFeatureTableModels
 */
void ModelManager::initFeatureTableModels(){

    //assign source models
    ModelManager::featureTableProxyModel.setSourceModel(&ModelManager::featureTableModel);
    ModelManager::featureTableProxyModel.setDynamicSortFilter(true);

}
