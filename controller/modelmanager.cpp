#include "modelmanager.h"

//init static variables
QPointer<OiJob> ModelManager::currentJob;
FeatureTableModel ModelManager::featureTableModel;
FeatureTableProxyModel ModelManager::featureTableProxyModel;
FeatureTableColumnConfig ModelManager::featureTableColumnConfig;
TrafoParamTableColumnConfig ModelManager::trafoParamTableColumnConfig;
ParameterDisplayConfig ModelManager::parameterDisplayConfig;

/*!
 * \brief ModelManager::ModelManager
 */
ModelManager::ModelManager(){

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
 * \brief ModelManager::initFeatureTableModels
 */
void ModelManager::initFeatureTableModels(){

    //assign source models
    ModelManager::featureTableProxyModel.setSourceModel(&ModelManager::featureTableModel);
    ModelManager::featureTableProxyModel.setDynamicSortFilter(true);

}
