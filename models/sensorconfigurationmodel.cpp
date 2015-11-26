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

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return 0;
    }

    int numSavedConfigs = this->sensorConfigManager->getSavedSensorConfigs().size();
    int numProjectConfigs = this->sensorConfigManager->getProjectSensorConfigs().size();

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

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return QVariant();
    }

    //check index
    if(!index.isValid()){
        return QVariant();
    }

    //check role
    if(role != Qt::DisplayRole){
        return QVariant();
    }

    //check if the index is a saved config
    if(index.row() < this->sensorConfigManager->getSavedSensorConfigs().size()){
        return this->sensorConfigManager->getSavedSensorConfigs().at(index.row()).getName();
    }

    //check if the index is a project sensor config
    if(index.row() < this->rowCount(index.parent())){
        return this->sensorConfigManager->getProjectSensorConfigs().at(index.row() - this->sensorConfigManager->getSavedSensorConfigs().size()).getName() + " (project)";
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

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return true;
    }

    //check index
    if(!index.isValid()){
        return true;
    }

    //check new name
    if(value.toString().compare("") == 0){
        return true;
    }

    //check if the index is a saved config
    if(index.row() < this->sensorConfigManager->getSavedSensorConfigs().size()){

        //check if there already is a sensor config with the new name
        SensorConfiguration sConfig = this->sensorConfigManager->getSavedSensorConfig(value.toString());
        if(!sConfig.getIsValid()){
            sConfig = this->sensorConfigManager->getSavedSensorConfigs().at(index.row());
            QString oldName = sConfig.getName();
            sConfig.setName(value.toString());
            this->sensorConfigManager->replaceSensorConfig(oldName, sConfig);
            return true;
        }

    }

    return true;

}

/*!
 * \brief SensorConfigurationModel::getIndex
 * \param name
 * \return
 */
QModelIndex SensorConfigurationModel::getIndex(const QString &name) const{

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return QModelIndex();
    }

    //get all saved configs and check names
    QList<SensorConfiguration> savedConfigs = this->sensorConfigManager->getSavedSensorConfigs();
    for(int i = 0; i < savedConfigs.size(); i++){
        if(savedConfigs.at(i).getName().compare(name) == 0){
            return this->createIndex(i, 0);
        }
    }

    QList<SensorConfiguration> projectConfigs = this->sensorConfigManager->getProjectSensorConfigs();
    for(int i = 0; i < projectConfigs.size(); i++){
        if(projectConfigs.at(i).getName().compare(name) == 0){
            return this->createIndex(i + savedConfigs.size(), 0);
        }
    }

    return QModelIndex();

}

/*!
 * \brief SensorConfigurationModel::getSensorType
 * \param index
 * \return
 */
SensorTypes SensorConfigurationModel::getSensorType(const QModelIndex &index) const{

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return eUndefinedSensor;
    }

    //check index
    if(!index.isValid()){
        return eUndefinedSensor;
    }

    //check if the index is a saved config
    if(index.row() < this->sensorConfigManager->getSavedSensorConfigs().size()){
        return this->sensorConfigManager->getSavedSensorConfigs().at(index.row()).getTypeOfSensor();
    }

    //check if the index is a project sensor config
    if(index.row() < this->rowCount(index.parent())){
        return this->sensorConfigManager->getProjectSensorConfigs().at(index.row() - this->sensorConfigManager->getSavedSensorConfigs().size()).getTypeOfSensor();
    }

    return eUndefinedSensor;

}

/*!
 * \brief SensorConfigurationModel::getSensorName
 * \param index
 * \return
 */
QString SensorConfigurationModel::getSensorName(const QModelIndex &index) const{

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return "";
    }

    //check index
    if(!index.isValid()){
        return "";
    }

    //check if the index is a saved config
    if(index.row() < this->sensorConfigManager->getSavedSensorConfigs().size()){
        return this->sensorConfigManager->getSavedSensorConfigs().at(index.row()).getSensorName();
    }

    //check if the index is a project sensor config
    if(index.row() < this->rowCount(index.parent())){
        return this->sensorConfigManager->getProjectSensorConfigs().at(index.row() - this->sensorConfigManager->getSavedSensorConfigs().size()).getSensorName();
    }

    return "";

}

/*!
 * \brief SensorConfigurationModel::getPluginName
 * \param index
 * \return
 */
QString SensorConfigurationModel::getPluginName(const QModelIndex &index) const{

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return "";
    }

    //check index
    if(!index.isValid()){
        return "";
    }

    //check if the index is a saved config
    if(index.row() < this->sensorConfigManager->getSavedSensorConfigs().size()){
        return this->sensorConfigManager->getSavedSensorConfigs().at(index.row()).getPluginName();
    }

    //check if the index is a project sensor config
    if(index.row() < this->rowCount(index.parent())){
        return this->sensorConfigManager->getProjectSensorConfigs().at(index.row() - this->sensorConfigManager->getSavedSensorConfigs().size()).getPluginName();
    }

    return "";

}

/*!
 * \brief SensorConfigurationModel::getConnectionConfig
 * \param index
 * \return
 */
ConnectionConfig SensorConfigurationModel::getConnectionConfig(const QModelIndex &index){

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return ConnectionConfig();
    }

    //check index
    if(!index.isValid()){
        return ConnectionConfig();
    }

    //check if the index is a saved config
    if(index.row() < this->sensorConfigManager->getSavedSensorConfigs().size()){
        return this->sensorConfigManager->getSavedSensorConfigs().at(index.row()).getConnectionConfig();
    }

    //check if the index is a project sensor config
    if(index.row() < this->rowCount(index.parent())){
        return this->sensorConfigManager->getProjectSensorConfigs().at(index.row() - this->sensorConfigManager->getSavedSensorConfigs().size()).getConnectionConfig();
    }

    return ConnectionConfig();

}

/*!
 * \brief SensorConfigurationModel::getAccuracy
 * \param index
 * \return
 */
Accuracy SensorConfigurationModel::getAccuracy(const QModelIndex &index){

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return Accuracy();
    }

    //check index
    if(!index.isValid()){
        return Accuracy();
    }

    //check if the index is a saved config
    if(index.row() < this->sensorConfigManager->getSavedSensorConfigs().size()){
        return this->sensorConfigManager->getSavedSensorConfigs().at(index.row()).getAccuracy();
    }

    //check if the index is a project sensor config
    if(index.row() < this->rowCount(index.parent())){
        return this->sensorConfigManager->getProjectSensorConfigs().at(index.row() - this->sensorConfigManager->getSavedSensorConfigs().size()).getAccuracy();
    }

    return Accuracy();

}

/*!
 * \brief SensorConfigurationModel::getIntegerParameter
 * \param index
 * \return
 */
QMap<QString, int> SensorConfigurationModel::getIntegerParameter(const QModelIndex &index){

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return QMap<QString, int>();
    }

    //check index
    if(!index.isValid()){
        return QMap<QString, int>();
    }

    //check if the index is a saved config
    if(index.row() < this->sensorConfigManager->getSavedSensorConfigs().size()){
        return this->sensorConfigManager->getSavedSensorConfigs().at(index.row()).getIntegerParameter();
    }

    //check if the index is a project sensor config
    if(index.row() < this->rowCount(index.parent())){
        return this->sensorConfigManager->getProjectSensorConfigs().at(index.row() - this->sensorConfigManager->getSavedSensorConfigs().size()).getIntegerParameter();
    }

    return QMap<QString, int>();

}

/*!
 * \brief SensorConfigurationModel::getDoubleParameter
 * \param index
 * \return
 */
QMap<QString, double> SensorConfigurationModel::getDoubleParameter(const QModelIndex &index){

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return QMap<QString, double>();
    }

    //check index
    if(!index.isValid()){
        return QMap<QString, double>();
    }

    //check if the index is a saved config
    if(index.row() < this->sensorConfigManager->getSavedSensorConfigs().size()){
        return this->sensorConfigManager->getSavedSensorConfigs().at(index.row()).getDoubleParameter();
    }

    //check if the index is a project sensor config
    if(index.row() < this->rowCount(index.parent())){
        return this->sensorConfigManager->getProjectSensorConfigs().at(index.row() - this->sensorConfigManager->getSavedSensorConfigs().size()).getDoubleParameter();
    }

    return QMap<QString, double>();

}

/*!
 * \brief SensorConfigurationModel::getStringParameter
 * \param index
 * \return
 */
QMap<QString, QString> SensorConfigurationModel::getStringParameter(const QModelIndex &index){

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return QMap<QString, QString>();
    }

    //check index
    if(!index.isValid()){
        return QMap<QString, QString>();
    }

    //check if the index is a saved config
    if(index.row() < this->sensorConfigManager->getSavedSensorConfigs().size()){
        return this->sensorConfigManager->getSavedSensorConfigs().at(index.row()).getStringParameter();
    }

    //check if the index is a project sensor config
    if(index.row() < this->rowCount(index.parent())){
        return this->sensorConfigManager->getProjectSensorConfigs().at(index.row() - this->sensorConfigManager->getSavedSensorConfigs().size()).getStringParameter();
    }

    return QMap<QString, QString>();

}

/*!
 * \brief SensorConfigurationModel::getIsSaved
 * \param index
 * \return
 */
bool SensorConfigurationModel::getIsSaved(const QModelIndex &index) const{

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return false;
    }

    //check index
    if(!index.isValid()){
        return false;
    }

    //check if the index is a saved config
    if(index.row() < this->sensorConfigManager->getSavedSensorConfigs().size()){
        return true;
    }

    return false;

}

/*!
 * \brief SensorConfigurationModel::addSensorConfig
 * \param sConfig
 * \return
 */
QModelIndex SensorConfigurationModel::addSensorConfig(const SensorConfiguration &sConfig){

    QModelIndex index;

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return index;
    }

    //check sConfig
    if(this->sensorConfigManager->getSavedSensorConfig(sConfig.getName()).getIsValid()){
        return index;
    }

    //add the sensor config
    this->sensorConfigManager->addSavedSensorConfig(sConfig);

    //get all saved sensor configs
    QList<SensorConfiguration> savedSensorConfigs = this->sensorConfigManager->getSavedSensorConfigs();
    for(int i = 0; i < savedSensorConfigs.size(); i++){
        if(savedSensorConfigs.at(i).getName().compare(sConfig.getName()) == 0){
            return this->createIndex(i, 0);
        }
    }

    this->updateModel();

    return index;

}

/*!
 * \brief SensorConfigurationModel::removeSensorConfig
 * \param index
 */
void SensorConfigurationModel::removeSensorConfig(const QModelIndex &index){

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return;
    }

    //check index
    if(!index.isValid()){
        return;
    }

    //check if the index is a saved config
    if(index.row() < this->sensorConfigManager->getSavedSensorConfigs().size()){
        this->sensorConfigManager->removeSavedSensorConfig(this->sensorConfigManager->getSavedSensorConfigs().at(index.row()).getName());
    }

    this->updateModel();

}

/*!
 * \brief SensorConfigurationModel::replaceSensorConfig
 * \param name
 * \param sConfig
 */
void SensorConfigurationModel::replaceSensorConfig(const QString &name, const SensorConfiguration &sConfig){

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return;
    }

    //check sensor config
    if(!sConfig.getIsValid()){
        return;
    }

    //replace the sensor config
    this->sensorConfigManager->replaceSensorConfig(name, sConfig);

}

/*!
 * \brief SensorConfigurationModel::getSensorConfigurationManager
 * \return
 */
const QPointer<SensorConfigurationManager> &SensorConfigurationModel::getSensorConfigurationManager() const{
    return this->sensorConfigManager;
}

/*!
 * \brief SensorConfigurationModel::setSensorConfigurationManager
 * \param manager
 */
void SensorConfigurationModel::setSensorConfigurationManager(const QPointer<SensorConfigurationManager> &manager){
    this->sensorConfigManager = manager;
    this->connectConfigManager();
    this->updateModel();
}

/*!
 * \brief SensorConfigurationModel::updateModel
 */
void SensorConfigurationModel::updateModel(){
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

/*!
 * \brief SensorConfigurationModel::connectConfigManager
 */
void SensorConfigurationModel::connectConfigManager(){

    //check sensor config manager
    if(this->sensorConfigManager.isNull()){
        return;
    }

    QObject::connect(this->sensorConfigManager.data(), &SensorConfigurationManager::sensorConfigurationsChanged,
                     this, &SensorConfigurationModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->sensorConfigManager.data(), &SensorConfigurationManager::activeSensorConfigurationChanged,
                     this, &SensorConfigurationModel::updateModel, Qt::AutoConnection);

}
