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

    int numUserConfigs = this->measurementConfigManager->getUserConfigs().size();
    int numProjectConfigs = this->measurementConfigManager->getProjectConfigs().size();

    return numUserConfigs + numProjectConfigs;

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

    //check if the index is a saved config
    if(index.row() < this->measurementConfigManager->getUserConfigs().size()){
        return this->measurementConfigManager->getUserConfigs().at(index.row()).getName();
    }

    //check if the index is a project measurement config
    if(index.row() < this->rowCount(index.parent())){
        return this->measurementConfigManager->getProjectConfigs().at(index.row() - this->measurementConfigManager->getUserConfigs().size()).getName();
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

    //check if the index is a saved config
    if(index.row() < this->measurementConfigManager->getUserConfigs().size()){

        //check if there already is a measurement config with the new name
        MeasurementConfig mConfig = this->measurementConfigManager->getUserConfig(value.toString());
        if(!mConfig.getIsValid()){
            mConfig = this->measurementConfigManager->getUserConfigs().at(index.row());
            QString oldName = mConfig.getName();
            mConfig.setName(value.toString());
            mConfig.isUserConfig(true);
            this->measurementConfigManager->replaceMeasurementConfig(oldName, mConfig);
            emit this->measurementConfigNameChanged(mConfig);
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

    //get all saved configs and check names
    if(mConfig.isUserConfig()){
        QList<MeasurementConfig> userConfigs = this->measurementConfigManager->getUserConfigs();
        for(int i = 0; i < userConfigs.size(); i++){
            if(userConfigs.at(i).getName().compare(mConfig.getName()) == 0){
                return this->createIndex(i, 0);
            }
        }
    } else { //get all project configs and check names
        QList<MeasurementConfig> projectConfigs = this->measurementConfigManager->getProjectConfigs();
        for(int i = 0; i < projectConfigs.size(); i++){
            if(projectConfigs.at(i).getName().compare(mConfig.getName()) == 0){
                return this->createIndex(i + this->measurementConfigManager->getUserConfigs().size(), 0);
            }
        }
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
    if(index.row() < this->measurementConfigManager->getUserConfigs().size()){
        return this->measurementConfigManager->getUserConfigs().at(index.row());
    }

    //check if the index is a project measurement config
    if(index.row() < this->rowCount(index.parent())){
        return this->measurementConfigManager->getProjectConfigs().at(index.row() - this->measurementConfigManager->getUserConfigs().size());
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

    //check if the index is a saved config
    if(index.row() < this->measurementConfigManager->getUserConfigs().size()){
        return true;
    }

    return false;

}

/*!
 * \brief MeasurementConfigurationModel::addMeasurementConfig
 * \param mConfig
 * \return
 */
QModelIndex MeasurementConfigurationModel::addMeasurementConfig(const MeasurementConfig &mConfig){

    QModelIndex index;

    //check measurement config manager
    if(this->measurementConfigManager.isNull()){
        return index;
    }

    //check mConfig
    if(this->measurementConfigManager->getUserConfig(mConfig.getName()).getIsValid()){
        return index;
    }

    //add the measurement config
    this->measurementConfigManager->saveUserConfig(mConfig);

    //get all saved measurement configs
    QList<MeasurementConfig> savedMeasurementConfigs = this->measurementConfigManager->getUserConfigs();
    for(int i = 0; i < savedMeasurementConfigs.size(); i++){
        if(savedMeasurementConfigs.at(i).getName().compare(mConfig.getName()) == 0){
            return this->createIndex(i, 0);
        }
    }

    this->updateModel();

    return index;

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
    if(index.row() < this->measurementConfigManager->getUserConfigs().size()){
        this->measurementConfigManager->removeUserConfig(this->measurementConfigManager->getUserConfigs().at(index.row()).getName());
    }

    this->updateModel();

}

/*!
 * \brief MeasurementConfigurationModel::replaceMeasurementConfig
 * \param name
 * \param mConfig
 */
void MeasurementConfigurationModel::replaceMeasurementConfig(const QString &name, const MeasurementConfig &mConfig){

    //check measurement config manager
    if(this->measurementConfigManager.isNull()){
        return;
    }

    //check measurement config
    if(!mConfig.getIsValid()){
        return;
    }

    //replace the measurement config
    this->measurementConfigManager->replaceMeasurementConfig(name, mConfig);

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
    if(!mConfig.getIsValid() || mConfig.isUserConfig()){
        return invalid;
    }

    //add the measurement config
    MeasurementConfig userConfig = mConfig;
    userConfig.isUserConfig(true);
    userConfig.isEditable(true);
    this->measurementConfigManager->saveUserConfig(mConfig);

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
