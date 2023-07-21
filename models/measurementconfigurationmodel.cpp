#include "measurementconfigurationmodel.h"

/*!
 * \brief MeasurementConfigurationModel::MeasurementConfigurationModel
 * \param parent
 */
MeasurementConfigurationModel::MeasurementConfigurationModel(QObject *parent) : QAbstractListModel(parent){

}

/*!
 * \brief MeasurementConfigurationModel::rowCount
 * \param parent
 * \return
 */
int MeasurementConfigurationModel::rowCount(const QModelIndex &parent) const{

    //check measurement config manager
    if(this->measurementConfigManager.isNull()){
        return 0;
    }

    return this->measurementConfigManager->getConfigs().size();

}

/*!
 * \brief MeasurementConfigurationModel::columnCount
 * \param parent
 * \return
 */
int MeasurementConfigurationModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

/*!
 * \brief MeasurementConfigurationModel::data
 * \param index
 * \param role
 * \return
 */
QVariant MeasurementConfigurationModel::data(const QModelIndex &index, int role) const{

    //check sensor config manager
    if(this->measurementConfigManager.isNull()){
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

    const QList<MeasurementConfig> configs = this->measurementConfigManager->getConfigs();
    if(index.row() < configs.size()){
        return configs.at(index.row()).getName();
    }

    return QVariant();

}

/*!
 * \brief MeasurementConfigurationModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant MeasurementConfigurationModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        return "measurement configurations";

    }
    return QVariant();
}

/*!
 * \brief MeasurementConfigurationModel::flags
 * \param index
 * \return
 */
Qt::ItemFlags MeasurementConfigurationModel::flags(const QModelIndex &index) const{
    Qt::ItemFlags myFlags = QAbstractListModel::flags(index);
    return (myFlags | Qt::ItemIsEditable);
}

/*!
 * \brief MeasurementConfigurationModel::setData
 * \param index
 * \param value
 * \param role
 * \return
 */
bool MeasurementConfigurationModel::setData(const QModelIndex &index, const QVariant &value, int role){

    //check measurement config manager
    if(this->measurementConfigManager.isNull()){
        return false;
    }

    //check index
    if(!index.isValid()){
        return false;
    }

    //check new name
    if(value.toString().compare("") == 0){
        return false;
    }

    const QList<MeasurementConfig> configs = this->measurementConfigManager->getConfigs();
    if(index.row() < configs.size()){

        //check if there already is a measurement config with the new name
        MeasurementConfig mConfig = this->measurementConfigManager->getUserConfig(value.toString());
        if(!mConfig.isValid()){
            mConfig = configs.at(index.row());
            MeasurementConfigKey oldKey = mConfig.getKey();
            mConfig.setName(value.toString());
            this->measurementConfigManager->replaceMeasurementConfig(oldKey, mConfig);
            emit this->measurementConfigNameChanged(mConfig);
            emit this->updateModel(); // force update: filter & sort model
            return true;
        }

    }

    return false;

}

/*!
 * \brief MeasurementConfigurationModel::getIndex
 * \param mConfig
 * \return
 */
QModelIndex MeasurementConfigurationModel::getIndex(const MeasurementConfig &mConfig) const{

    //check measurement config manager
    if(this->measurementConfigManager.isNull()){
        return QModelIndex();
    }

    const int idx = this->measurementConfigManager->getConfigs().indexOf(mConfig);
    if(idx >= 0){
        return this->createIndex(idx, 0);
    }

    return QModelIndex();

}

/*!
 * \brief MeasurementConfigurationModel::getMeasurementConfig
 * \param index
 * \return
 */
MeasurementConfig MeasurementConfigurationModel::getMeasurementConfig(const QModelIndex &index) const{

    //check measurement config manager
    if(this->measurementConfigManager.isNull()){
        return MeasurementConfig();
    }

    //check index
    if(!index.isValid()){
        return MeasurementConfig();
    }

    //check if the index is a measurement config
    const QList<MeasurementConfig> configs = this->measurementConfigManager->getConfigs();
    if(index.row() < configs.size()){
        return configs.at(index.row());
    }

    return MeasurementConfig();

}

bool MeasurementConfigurationModel::isUserConfig(const QModelIndex &index) const{

    //check sensor config manager
    if(this->measurementConfigManager.isNull()){
        return false;
    }

    //check index
    if(!index.isValid()){
        return false;
    }

    const QList<MeasurementConfig> configs = this->measurementConfigManager->getConfigs();
    if(index.row() < configs.size()){
        return configs.at(index.row()).isUserConfig();
    }

    return false;

}

/*!
 * \brief MeasurementConfigurationModel::addMeasurementConfig
 * \param mConfig
 * \return
 */
QModelIndex MeasurementConfigurationModel::addMeasurementConfig(const MeasurementConfig &mConfig){
    //check measurement config manager
    if(this->measurementConfigManager.isNull()){
        return QModelIndex();
    }

    //check mConfig
    if(this->measurementConfigManager->getUserConfig(mConfig.getName()).isValid()){
        return  QModelIndex();
    }

    //add the measurement config
    this->measurementConfigManager->saveUserConfig(mConfig);

    this->updateModel();


    //get all saved measurement configs
    const QList<MeasurementConfig> userConfigs = this->measurementConfigManager->getConfigs();
    for(int i = 0; i < userConfigs.size(); i++){
        if(userConfigs.at(i).getName().compare(mConfig.getName()) == 0){
            return this->createIndex(i, 0);
        }
    }

    return  QModelIndex();

}

/*!
 * \brief MeasurementConfigurationModel::removeMeasurementConfig
 * \param index
 */
void MeasurementConfigurationModel::removeMeasurementConfig(const QModelIndex &index){

    //check measurement config manager
    if(this->measurementConfigManager.isNull()){
        return;
    }

    //check index
    if(!index.isValid()){
        return;
    }

    //check if the index is a saved config
    if(index.row() < this->measurementConfigManager->getConfigs().size()){
        this->measurementConfigManager->removeUserConfig(getMeasurementConfig(index).getName());
    }

    this->updateModel();

}

/*!
 * \brief MeasurementConfigurationModel::replaceMeasurementConfig
 * \param oldKey
 * \param mConfig
 */
void MeasurementConfigurationModel::replaceMeasurementConfig(const MeasurementConfigKey &oldKey, const MeasurementConfig &mConfig){

    //check measurement config manager
    if(this->measurementConfigManager.isNull()){
        return;
    }

    //check measurement config
    if(!mConfig.isValid()){
        return;
    }

    //replace the measurement config
    this->measurementConfigManager->replaceMeasurementConfig(oldKey, mConfig);

}

/*!
 * \brief MeasurementConfigurationModel::cloneMeasurementConfig
 * \param mConfig
 * \return clone or invalid config
 */
MeasurementConfig MeasurementConfigurationModel::cloneMeasurementConfig(const MeasurementConfig &mConfig){
    MeasurementConfig invalid;
    //check measurement config manager
    if(this->measurementConfigManager.isNull()){
        return invalid;
    }

    //check mConfig
    if(!mConfig.isValid()){
        return invalid;
    }

    // find usable name
    QString name = mConfig.getName();
    while(measurementConfigManager->findConfig(name).isValid()) {
        name += ".";
    }

    //add the measurement config
    MeasurementConfig userConfig = mConfig;
    userConfig.setName(name);
    userConfig.makeUserConfig();
    this->measurementConfigManager->saveUserConfig(userConfig);

    this->updateModel();

    return userConfig;

}

/*!
 * \brief MeasurementConfigurationModel::getMeasurementConfigurationManager
 * \return
 */
const QPointer<MeasurementConfigManager> &MeasurementConfigurationModel::getMeasurementConfigurationManager() const{
    return this->measurementConfigManager;
}

/*!
 * \brief MeasurementConfigurationModel::setMeasurementConfigurationManager
 * \param manager
 */
void MeasurementConfigurationModel::setMeasurementConfigurationManager(const QPointer<MeasurementConfigManager> &manager){
    this->measurementConfigManager = manager;
    this->connectConfigManager();
    this->updateModel();
}

/*!
 * \brief MeasurementConfigurationModel::updateModel
 */
void MeasurementConfigurationModel::updateModel(){
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

/*!
 * \brief MeasurementConfigurationModel::connectConfigManager
 */
void MeasurementConfigurationModel::connectConfigManager(){

    //check measurement config manager
    if(this->measurementConfigManager.isNull()){
        return;
    }

    QObject::connect(this->measurementConfigManager.data(), &MeasurementConfigManager::measurementConfigurationsChanged,
                     this, &MeasurementConfigurationModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->measurementConfigManager.data(), &MeasurementConfigManager::activeMeasurementConfigurationChanged,
                     this, &MeasurementConfigurationModel::updateModel, Qt::AutoConnection);

}
