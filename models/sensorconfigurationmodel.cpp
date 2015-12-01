#include "sensorconfigurationmodel.h"

/*!
 * \brief SensorConfigurationModel::ActiveFeatureFunctionsModel
 * \param parent
 */
SensorConfigurationModel::SensorConfigurationModel(QObject *parent) : QAbstractListModel(parent){

}

/*!
 * \brief SensorConfigurationModel::rowCount
 * \param parent
 * \return
 */
int SensorConfigurationModel::rowCount(const QModelIndex &parent) const{

    int numSavedConfigs = this->sensorConfigManager.getSavedSensorConfigs().size();
    int numProjectConfigs = this->sensorConfigManager.getProjectSensorConfigs().size();

    return numSavedConfigs + numProjectConfigs;

}

/*!
 * \brief SensorConfigurationModel::columnCount
 * \param parent
 * \return
 */
int SensorConfigurationModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

/*!
 * \brief SensorConfigurationModel::data
 * \param index
 * \param role
 * \return
 */
QVariant SensorConfigurationModel::data(const QModelIndex &index, int role) const{

    //check index
    if(!index.isValid()){
        return QVariant();
    }

    //check role
    if(role == Qt::DisplayRole){

        //check if the index is a saved config
        if(index.row() < this->sensorConfigManager.getSavedSensorConfigs().size()){
            return this->sensorConfigManager.getSavedSensorConfigs().at(index.row()).getName();
        }

        //check if the index is a project sensor config
        if(index.row() < this->rowCount(index.parent())){
            return this->sensorConfigManager.getProjectSensorConfigs().at(index.row() - this->sensorConfigManager.getSavedSensorConfigs().size()).getName() + " (project)";
        }

    }else if(role == Qt::FontRole){

        //check wether index represents the active config
        bool isActiveConfig = false;
        if(index.row() < this->sensorConfigManager.getSavedSensorConfigs().size()){
            if(this->sensorConfigManager.getSavedSensorConfigs().at(index.row()) ==
                    this->sensorConfigManager.getActiveSensorConfig()){
                isActiveConfig = true;
            }
        }else if(index.row() < this->rowCount(index.parent())){
            if(this->sensorConfigManager.getProjectSensorConfigs().at(index.row() - this->sensorConfigManager.getSavedSensorConfigs().size()) ==
                    this->sensorConfigManager.getActiveSensorConfig()){
                isActiveConfig = true;
            }
        }

        //the active config is displayed bold
        if(isActiveConfig){
            QFont font;
            font.setBold(true);
            return font;
        }

    }

    return QVariant();

}

/*!
 * \brief SensorConfigurationModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant SensorConfigurationModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        return "sensor configurations";

    }
    return QVariant();
}

/*!
 * \brief SensorConfigurationModel::flags
 * \param index
 * \return
 */
Qt::ItemFlags SensorConfigurationModel::flags(const QModelIndex &index) const{
    Qt::ItemFlags myFlags = QAbstractListModel::flags(index);
    return (myFlags | Qt::ItemIsEditable);
}

/*!
 * \brief SensorConfigurationModel::setData
 * \param index
 * \param value
 * \param role
 * \return
 */
bool SensorConfigurationModel::setData(const QModelIndex &index, const QVariant &value, int role){

    //check index
    if(!index.isValid()){
        return true;
    }

    //get row index
    int rowIndex = index.row();

    //check new name
    if(value.toString().compare("") == 0){
        return true;
    }

    //check if the index is a saved config
    if(rowIndex < this->sensorConfigManager.getSavedSensorConfigs().size()){

        //check if there already is a sensor config with the new name
        SensorConfiguration sConfig = this->sensorConfigManager.getSavedSensorConfig(value.toString());
        if(!sConfig.getIsValid()){
            sConfig = this->sensorConfigManager.getSavedSensorConfigs().at(rowIndex);
            QString oldName = sConfig.getName();
            sConfig.setName(value.toString());
            this->sensorConfigManager.replaceSensorConfig(oldName, sConfig, false);
            emit this->sensorConfigRenamed();
            return true;
        }

    }

    return true;

}

/*!
 * \brief SensorConfigurationModel::getSensorConfigurationManager
 * \return
 */
const SensorConfigurationManager &SensorConfigurationModel::getSensorConfigurationManager() const{
    return this->sensorConfigManager;
}

/*!
 * \brief SensorConfigurationModel::setSensorConfigurationManager
 * \param manager
 */
void SensorConfigurationModel::setSensorConfigurationManager(const SensorConfigurationManager &manager){

    //set up new config manager
    emit this->beginResetModel();
    this->sensorConfigManager = manager;
    emit this->endResetModel();

    //connect manager
    QObject::connect(&this->sensorConfigManager, &SensorConfigurationManager::sensorConfigurationsChanged,
                     this, &SensorConfigurationModel::sensorConfigsChanged, Qt::AutoConnection);
    QObject::connect(&this->sensorConfigManager, &SensorConfigurationManager::activeSensorConfigurationChanged,
                     this, &SensorConfigurationModel::activeSensorConfigChanged, Qt::AutoConnection);

    this->updateModel();

}

/*!
 * \brief SensorConfigurationModel::addSensorConfiguration
 * \return
 */
SensorConfiguration SensorConfigurationModel::addSensorConfiguration(){

    //create sensor config
    SensorConfiguration sConfig;
    sConfig.setName("newConfig");
    sConfig.setTypeOfSensor(eUndefinedSensor);
    sConfig.setIsSaved(true);
    int index = 0;
    while(this->sensorConfigManager.hasSavedSensorConfig(sConfig.getName())){
        index ++;
        sConfig.setName(QString("%1%2").arg(sConfig.getName()).arg(QString::number(index)));
    }

    //add config
    this->sensorConfigManager.addSavedSensorConfig(sConfig, false);

    this->updateModel();

    return sConfig;

}

/*!
 * \brief SensorConfigurationModel::removeSensorConfiguration
 * \param index
 * \return
 */
bool SensorConfigurationModel::removeSensorConfiguration(const QModelIndex &index){

    //check index
    if(!index.isValid()){
        return false;
    }

    //get row index
    int rowIndex = index.row();

    SensorConfiguration sConfig;

    //check if the index is a saved config or a project config
    if(rowIndex < this->sensorConfigManager.getSavedSensorConfigs().size()){
        sConfig = this->sensorConfigManager.getSavedSensorConfigs().at(rowIndex);
    }else if(rowIndex < this->rowCount(index.parent())){
        sConfig = this->sensorConfigManager.getProjectSensorConfigs().at(rowIndex - this->sensorConfigManager.getSavedSensorConfigs().size());
    }

    this->sensorConfigManager.removeSavedSensorConfig(sConfig.getName(), false);
    return true;

}

/*!
 * \brief SensorConfigurationModel::updateSensorAccuracy
 * \param sConfig
 */
void SensorConfigurationModel::updateSensorAccuracy(const SensorConfiguration &sConfig){

    //check configs
    if(!sConfig.getIsValid() || !this->sensorConfigManager.getActiveSensorConfig().getIsValid()){
        return;
    }

    //get and update active sensor config
    SensorConfiguration activeConfig = this->sensorConfigManager.getActiveSensorConfig();
    activeConfig.setAccuracy(sConfig.getAccuracy());

    //replace the old config
    this->sensorConfigManager.replaceSensorConfig(activeConfig.getName(), activeConfig, false);

}

/*!
 * \brief SensorConfigurationModel::updateSensorParameters
 * \param sConfig
 */
void SensorConfigurationModel::updateSensorParameters(const SensorConfiguration &sConfig){

    //check configs
    if(!sConfig.getIsValid() || !this->sensorConfigManager.getActiveSensorConfig().getIsValid()){
        return;
    }

    //get and update active sensor config
    SensorConfiguration activeConfig = this->sensorConfigManager.getActiveSensorConfig();
    activeConfig.setIntegerParameter(sConfig.getIntegerParameter());
    activeConfig.setDoubleParameter(sConfig.getDoubleParameter());
    activeConfig.setStringParameter(sConfig.getStringParameter());

    //replace the old config
    this->sensorConfigManager.replaceSensorConfig(activeConfig.getName(), activeConfig, false);

}

/*!
 * \brief SensorConfigurationModel::updateSensorPlugin
 * \param sConfig
 */
void SensorConfigurationModel::updateSensorPlugin(const SensorConfiguration &sConfig){

    //check configs
    if(!sConfig.getIsValid() || !this->sensorConfigManager.getActiveSensorConfig().getIsValid()){
        return;
    }

    //get and update active sensor config
    SensorConfiguration activeConfig = this->sensorConfigManager.getActiveSensorConfig();
    activeConfig.setSensorName(sConfig.getSensorName());
    activeConfig.setPluginName(sConfig.getPluginName());
    activeConfig.setTypeOfSensor(sConfig.getTypeOfSensor());
    activeConfig.setAccuracy(sConfig.getAccuracy());
    activeConfig.setConnectionConfig(sConfig.getConnectionConfig());
    activeConfig.setIntegerParameter(sConfig.getIntegerParameter());
    activeConfig.setDoubleParameter(sConfig.getDoubleParameter());
    activeConfig.setStringParameter(sConfig.getStringParameter());
    activeConfig.setAvailableStringParameter(sConfig.getAvailableStringParameter());

    //replace the old config
    this->sensorConfigManager.replaceSensorConfig(activeConfig.getName(), activeConfig, false);

    emit this->activeSensorConfigChanged();

}

/*!
 * \brief SensorConfigurationModel::updateSensorConnection
 * \param sConfig
 */
void SensorConfigurationModel::updateSensorConnection(const SensorConfiguration &sConfig){

    //check configs
    if(!sConfig.getIsValid() || !this->sensorConfigManager.getActiveSensorConfig().getIsValid()){
        return;
    }

    //get and update active sensor config
    SensorConfiguration activeConfig = this->sensorConfigManager.getActiveSensorConfig();
    activeConfig.setConnectionConfig(sConfig.getConnectionConfig());

    //replace the old config
    this->sensorConfigManager.replaceSensorConfig(activeConfig.getName(), activeConfig, false);

}

/*!
 * \brief SensorConfigurationModel::getActiveSensorConfig
 * \return
 */
const SensorConfiguration &SensorConfigurationModel::getActiveSensorConfig() const{
    return this->sensorConfigManager.getActiveSensorConfig();
}

/*!
 * \brief SensorConfigurationModel::getActiveSensorConfigIndex
 * \return
 */
QModelIndex SensorConfigurationModel::getActiveSensorConfigIndex() const{

    //get and check active sensor config
    const SensorConfiguration &sConfig = this->sensorConfigManager.getActiveSensorConfig();
    if(!sConfig.getIsValid()){
        return QModelIndex();
    }

    //get index of the active sensor config
    int rowIndex = 0;
    for(rowIndex = 0; rowIndex < this->sensorConfigManager.getSavedSensorConfigs().size(); rowIndex++){

        //compare sensor configs
        if(sConfig.getIsSaved() && sConfig == this->sensorConfigManager.getSavedSensorConfigs().at(rowIndex)){
            return this->index(rowIndex, 0);
        }

    }
    for(int i = 0; i < (this->sensorConfigManager.getProjectSensorConfigs().size() + rowIndex); i++){

        //compare sensor configs
        if(!sConfig.getIsSaved() && sConfig == this->sensorConfigManager.getProjectSensorConfigs().at(i)){
            return this->index(rowIndex + i, 0);
        }

    }

}

/*!
 * \brief SensorConfigurationModel::setActiveSensorConfig
 * \param sConfig
 */
void SensorConfigurationModel::setActiveSensorConfig(const SensorConfiguration &sConfig){

    //check config
    if(!sConfig.getIsValid()){
        return;
    }

    this->sensorConfigManager.setActiveSensorConfig(sConfig);

    this->updateModel();

}

/*!
 * \brief SensorConfigurationModel::setActiveSensorConfig
 * \param index
 */
void SensorConfigurationModel::setActiveSensorConfig(const QModelIndex &index){

    //check index
    if(!index.isValid()){
        return;
    }

    //get row index
    int rowIndex = index.row();

    //get sensor config
    SensorConfiguration sConfig;
    if(rowIndex < this->sensorConfigManager.getSavedSensorConfigs().size()){
        sConfig = this->sensorConfigManager.getSavedSensorConfigs().at(rowIndex);
    }else{
        rowIndex = rowIndex - this->sensorConfigManager.getSavedSensorConfigs().size();
        if(rowIndex >= 0 && rowIndex < this->sensorConfigManager.getProjectSensorConfigs().size()){
            sConfig = this->sensorConfigManager.getProjectSensorConfigs().at(rowIndex);
        }
    }

    //check config
    if(!sConfig.getIsValid()){
        return;
    }

    this->sensorConfigManager.setActiveSensorConfig(sConfig);

    this->updateModel();

}

/*!
 * \brief SensorConfigurationModel::updateModel
 */
void SensorConfigurationModel::updateModel(){
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}
