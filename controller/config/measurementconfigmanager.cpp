#include "measurementconfigmanager.h"

/*!
 * \brief MeasurementConfigManager::MeasurementConfigManager
 */
MeasurementConfigManager::MeasurementConfigManager(QObject *parent) : QObject(parent){

    //connect geometry updates
    QObject::connect(this, static_cast<void (MeasurementConfigManager::*)()>(&MeasurementConfigManager::measurementConfigurationsChanged),
                     this, static_cast<void (MeasurementConfigManager::*)()>(&MeasurementConfigManager::updateGeometries), Qt::AutoConnection);
    QObject::connect(this, &MeasurementConfigManager::measurementConfigurationReplaced,
                     this, static_cast<void (MeasurementConfigManager::*)(const MeasurementConfig&, const MeasurementConfig &newMConfig)>
                     (&MeasurementConfigManager::updateGeometries), Qt::AutoConnection);

}

/*!
 * \brief MeasurementConfigManager::MeasurementConfigManager
 * \param copy
 * \param parent
 */
MeasurementConfigManager::MeasurementConfigManager(const MeasurementConfigManager &copy, QObject *parent){
    this->savedMeasurementConfigMap = copy.savedMeasurementConfigMap;
    this->savedMeasurementConfigList = copy.savedMeasurementConfigList;
    this->activeMeasurementConfigs = copy.activeMeasurementConfigs;
}

/*!
 * \brief MeasurementConfigManager::operator =
 * \param copy
 * \return
 */
MeasurementConfigManager &MeasurementConfigManager::operator=(const MeasurementConfigManager &copy){
    this->savedMeasurementConfigMap = copy.savedMeasurementConfigMap;
    this->savedMeasurementConfigList = copy.savedMeasurementConfigList;
    this->activeMeasurementConfigs = copy.activeMeasurementConfigs;
    return *this;
}

/*!
 * \brief MeasurementConfigManager::getCurrentJob
 * \return
 */
const QPointer<OiJob> &MeasurementConfigManager::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief MeasurementConfigManager::setCurrentJob
 * \param job
 */
void MeasurementConfigManager::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){

        //disconnect current job
        if(!this->currentJob.isNull()){
            this->disconnectJob();
        }

        this->currentJob = job;
        this->connectJob();

    }
}

/*!
 * \brief MeasurementConfigManager::hasSavedMeasurementConfig
 * Checks wether there is a saved measurement config with the given name
 * \param name
 * \return
 */
bool MeasurementConfigManager::hasSavedMeasurementConfig(const QString &name){
    return this->savedMeasurementConfigMap.contains(name);
}

/*!
 * \brief MeasurementConfigManager::hasProjectMeasurementConfig
 * Checks wether there is a project measurement config with the given name
 * \param name
 * \return
 */
bool MeasurementConfigManager::hasProjectMeasurementConfig(const QString &name){
    return this->projectMeasurementConfigMap.contains(name);
}

/*!
 * \brief MeasurementConfigManager::hasSavedMeasurementConfig
 * Checks wether there is a saved measurement config with the same name and parameters
 * \param mConfig
 * \return
 */
bool MeasurementConfigManager::hasSavedMeasurementConfig(const MeasurementConfig &mConfig){

    if(!this->savedMeasurementConfigMap.contains(mConfig.getName())){
        return false;
    }

    //get saved config and compare it to the given one
    MeasurementConfig savedConfig = this->savedMeasurementConfigMap.value(mConfig.getName());
    if(this->equals(savedConfig, mConfig)){
        return true;
    }

    return false;

}

/*!
 * \brief MeasurementConfigManager::hasProjectMeasurementConfig
 * Checks wether there is a project measurement config with the same name and parameters
 * \param mConfig
 * \return
 */
bool MeasurementConfigManager::hasProjectMeasurementConfig(const MeasurementConfig &mConfig){

    if(!this->projectMeasurementConfigMap.contains(mConfig.getName())){
        return false;
    }

    //get project config and compare it to the given one
    MeasurementConfig projectConfig = this->projectMeasurementConfigMap.value(mConfig.getName());
    if(this->equals(projectConfig, mConfig)){
        return true;
    }

    return false;

}

/*!
 * \brief MeasurementConfigManager::getSavedMeasurementConfig
 * \param name
 * \return
 */
MeasurementConfig MeasurementConfigManager::getSavedMeasurementConfig(const QString &name) const{
    return this->savedMeasurementConfigMap.value(name, MeasurementConfig());
}

/*!
 * \brief MeasurementConfigManager::getProjectMeasurementConfig
 * \param name
 * \return
 */
MeasurementConfig MeasurementConfigManager::getProjectMeasurementConfig(const QString &name) const{
    return this->projectMeasurementConfigMap.value(name, MeasurementConfig());
}

/*!
 * \brief MeasurementConfigManager::getSavedMeasurementConfigs
 * \return
 */
const QList<MeasurementConfig> &MeasurementConfigManager::getSavedMeasurementConfigs() const{
    return this->savedMeasurementConfigList;
}

/*!
 * \brief MeasurementConfigManager::getProjectMeasurementConfigs
 * \return
 */
const QList<MeasurementConfig> &MeasurementConfigManager::getProjectMeasurementConfigs() const{
    return this->projectMeasurementConfigList;
}

/*!
 * \brief MeasurementConfigManager::getActiveMeasurementConfig
 * \param type
 * \return
 */
MeasurementConfig MeasurementConfigManager::getActiveMeasurementConfig(const GeometryTypes &type) const{
    return this->activeMeasurementConfigs.value(type, MeasurementConfig());
}

/*!
 * \brief MeasurementConfigManager::addMeasurementConfig
 * \param mConfig
 */
void MeasurementConfigManager::addSavedMeasurementConfig(const MeasurementConfig &mConfig){

    //check if mConfig is valid
    if(!mConfig.getIsValid()){
        emit this->sendMessage("Cannot add a measurement configuration with an empty name", eErrorMessage);
        return;
    }

    //check if mConfig already exists
    if(this->savedMeasurementConfigMap.contains(mConfig.getName())){
        emit this->sendMessage(QString("A measurement configuration with the name %1 already exists").arg(mConfig.getName()), eErrorMessage);
        return;
    }

    //save mConfig
    MeasurementConfig savedConfig = mConfig;
    savedConfig.setIsSaved(true);
    this->saveMeasurementConfig(savedConfig);

}

/*!
 * \brief MeasurementConfigManager::addProjectMeasurementConfig
 * \param mConfig
 */
void MeasurementConfigManager::addProjectMeasurementConfig(const MeasurementConfig &mConfig){

    //check if mConfig is valid
    if(!mConfig.getIsValid()){
        emit this->sendMessage("Cannot add a measurement configuration with an empty name", eErrorMessage);
        return;
    }

    //check if mConfig already exists
    if(this->projectMeasurementConfigMap.contains(mConfig.getName())){
        emit this->sendMessage(QString("A measurement configuration with the name %1 already exists").arg(mConfig.getName()), eErrorMessage);
        return;
    }

    //save mConfig
    this->projectMeasurementConfigList.append(mConfig);
    this->projectMeasurementConfigMap.insert(mConfig.getName(), mConfig);

    emit this->measurementConfigurationsChanged();

}

/*!
 * \brief MeasurementConfigManager::removeMeasurementConfig
 * \param name
 */
void MeasurementConfigManager::removeSavedMeasurementConfig(const QString &name){

    //check name
    if(name.compare("") == 0){
        emit this->sendMessage("Cannot remove a measurement configuration with an empty name", eErrorMessage);
        return;
    }

    //check if the measurement config exists
    if(!this->savedMeasurementConfigMap.contains(name)){
        emit this->sendMessage(QString("A measurement configuration with the name %1 does not exist").arg(name), eErrorMessage);
        return;
    }

    //delete mConfig
    this->deleteMeasurementConfig(name);

}

/*!
 * \brief MeasurementConfigManager::removeProjectMeasurementConfig
 * \param name
 */
void MeasurementConfigManager::removeProjectMeasurementConfig(const QString &name){

    if(this->projectMeasurementConfigMap.contains(name)){

        MeasurementConfig mConfig = this->projectMeasurementConfigMap.take(name);
        this->projectMeasurementConfigList.removeOne(mConfig);

        emit this->measurementConfigurationsChanged();

    }

}

/*!
 * \brief MeasurementConfigManager::removeAllSavedMeasurementConfigs
 */
void MeasurementConfigManager::removeAllSavedMeasurementConfigs(){

    //get a list of saved measurement configs
    QList<MeasurementConfig> configs = this->getSavedMeasurementConfigs();

    //remove measurement configs
    foreach(const MeasurementConfig &mConfig, configs){
        this->deleteMeasurementConfig(mConfig.getName());
    }

}

/*!
 * \brief MeasurementConfigManager::removeAllProjectMeasurementConfigs
 */
void MeasurementConfigManager::removeAllProjectMeasurementConfigs(){

    this->projectMeasurementConfigList.clear();
    this->projectMeasurementConfigMap.clear();

    emit this->measurementConfigurationsChanged();

}

/*!
 * \brief MeasurementConfigManager::replaceMeasurementConfig
 * \param name
 * \param mConfig
 */
void MeasurementConfigManager::replaceMeasurementConfig(const QString &name, const MeasurementConfig &mConfig){

    //get the old measurement config
    if(!this->savedMeasurementConfigMap.contains(name)){
        return;
    }
    MeasurementConfig oldConfig = this->savedMeasurementConfigMap.value(name);

    //###########################
    //replace mConfig in database
    //###########################

    SystemDbManager::removeMeasurementConfig(name);
    SystemDbManager::addMeasurementConfig(mConfig.getName());

    //########################
    //replace mConfig xml file
    //########################

    //create xml document
    QDomDocument mConfigXml("measurementConfig");

    //add mConfig to document as xml
    QDomElement root = mConfig.toOpenIndyXML(mConfigXml);
    mConfigXml.appendChild(root);

    //get config folder (create if does not exist yet)
    QString appPath = qApp->applicationDirPath();
    QDir mConfigFolder(appPath.append("/config/measurementConfigs"));
    if(!mConfigFolder.exists()){
        mConfigFolder.mkpath(".");
    }

    //set the file name
    QString fileName = mConfig.getName();

    //remove old config file
    QFile oldConfigFile(mConfigFolder.absoluteFilePath(name + ".xml"));
    if(oldConfigFile.exists()){
        oldConfigFile.remove();
    }

    //save mConfig to xml file
    QFile configFile(mConfigFolder.absoluteFilePath(fileName.append(".xml")));
    configFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QTextStream stream(&configFile);
    mConfigXml.save(stream, 4);
    configFile.close();

    //###############################
    //replace mConfig in list and map
    //###############################

    //replace mConfig in map
    this->savedMeasurementConfigMap.remove(name);
    this->savedMeasurementConfigMap.insert(mConfig.getName(), mConfig);

    //replace mConfig in list
    int index = this->savedMeasurementConfigList.indexOf(oldConfig, 0);
    if(index != -1){
        this->savedMeasurementConfigList.replace(index, mConfig);
    }

    emit this->measurementConfigurationReplaced(oldConfig, mConfig);

}

/*!
 * \brief MeasurementConfigManager::loadFromConfigFolder
 */
void MeasurementConfigManager::loadFromConfigFolder(){

    //get application path
    QString appPath = qApp->applicationDirPath();
    QDir appFolder(appPath);

    //check if application folder exists and contains a subdirectory named config
    if(!appFolder.exists() || !appFolder.exists("config/measurementConfigs")){
        return;
    }

    //create folder object for measurement config folder
    QDir mConfigFolder(appPath.append("/config/measurementConfigs"));
    if(!mConfigFolder.exists()){
        return;
    }

    //get a list of all xml files inside the measurementConfigs folder
    QStringList xmlFilter;
    xmlFilter.append("*.xml");
    QStringList fileNames = mConfigFolder.entryList(xmlFilter, QDir::Files);

    //create a list that is filled with the names of the loaded measurement configs
    QStringList mConfigNames;

    //load all files and create a MeasurementConfig object for each of them
    for(int i = 0; i < fileNames.size(); i++){

        //create file from file name and check if it exists
        QFile mConfigFile(mConfigFolder.absoluteFilePath(fileNames.at(i)));
        if(!mConfigFile.exists()){
            continue;
        }

        //try to parse the file content to a QDomDocument
        QDomDocument mConfigXml;
        if(!mConfigXml.setContent(&mConfigFile) || mConfigXml.isNull()){
            continue;
        }

        //try to parse the file to a MeasurementConfig object
        MeasurementConfig savedConfig;
        QDomElement mConfigTag = mConfigXml.documentElement();
        if(!savedConfig.fromOpenIndyXML(mConfigTag)){
            continue;
        }
        savedConfig.setIsSaved(true);

        //check if a measurement config with the same name has been loaded before
        if(mConfigNames.contains(savedConfig.getName())){

            //delete the config file permanently
            mConfigFile.remove();
            continue;
        }
        mConfigNames.append(savedConfig.getName());

        this->addSavedMeasurementConfig(savedConfig);

        //add the loaded measurement config to the list of saved configs
        if(!this->savedMeasurementConfigMap.contains(savedConfig.getName())){
            this->savedMeasurementConfigMap.insert(savedConfig.getName(), savedConfig);
            this->savedMeasurementConfigList.append(savedConfig);
        }
    }

    //emit signals
    emit this->measurementConfigurationsChanged();
}

/*!
 * \brief MeasurementConfigManager::synchronize
 * \param other
 */
void MeasurementConfigManager::synchronize(const MeasurementConfigManager &other){

    //do not trigger signals during synchronization
    this->blockSignals(true);

    //remove measurement configs
    this->removeAllSavedMeasurementConfigs();
    this->removeAllProjectMeasurementConfigs();

    //add new configs
    QList<MeasurementConfig> savedConfigs = other.getSavedMeasurementConfigs();
    QList<MeasurementConfig> projectConfigs = other.getProjectMeasurementConfigs();
    foreach(const MeasurementConfig &mConfig, savedConfigs){
        this->addSavedMeasurementConfig(mConfig);
    }
    foreach(const MeasurementConfig &mConfig, projectConfigs){
        this->addProjectMeasurementConfig(mConfig);
    }

    //trigger edits again
    this->blockSignals(false);

    emit this->measurementConfigurationsChanged();

}

/*!
 * \brief MeasurementConfigManager::saveMeasurementConfig
 * \param mConfig
 */
void MeasurementConfigManager::saveMeasurementConfig(const MeasurementConfig &mConfig){

    //###################################
    //create config file at config folder
    //###################################

    //create xml document
    QDomDocument mConfigXml("measurementConfig");

    //add mConfig to document as xml
    QDomElement root = mConfig.toOpenIndyXML(mConfigXml);
    mConfigXml.appendChild(root);

    //get config folder (create if does not exist yet)
    QString appPath = qApp->applicationDirPath();
    QDir mConfigFolder(appPath.append("/config/measurementConfigs"));
    if(!mConfigFolder.exists()){
        mConfigFolder.mkpath(".");
    }

    //set the file name
    QString fileName = mConfig.getName();

    //save mConfig to xml file
    QFile configFile(mConfigFolder.absoluteFilePath(fileName.append(".xml")));
    configFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QTextStream stream(&configFile);
    mConfigXml.save(stream, 4);
    configFile.close();

    //##################
    //add database entry
    //##################

    SystemDbManager::addMeasurementConfig(mConfig.getName());

    //########################################
    //add mConfig to the list of saved configs
    //########################################

    if(!this->savedMeasurementConfigMap.contains(mConfig.getName())){
        this->savedMeasurementConfigMap.insert(mConfig.getName(), mConfig);
        this->savedMeasurementConfigList.append(mConfig);
    }

    //############
    //emit signals
    //############

    emit this->measurementConfigurationsChanged();

}

/*!
 * \brief MeasurementConfigManager::deleteMeasurementConfig
 * \param name
 */
void MeasurementConfigManager::deleteMeasurementConfig(const QString &name){

    //#####################################
    //remove config file from config folder
    //#####################################

    //get config folder
    QString appPath = qApp->applicationDirPath();
    QDir mConfigFolder(appPath.append("/config/measurementConfigs"));
    if(mConfigFolder.exists()){

        //get config file
        QFile configFile(mConfigFolder.absoluteFilePath(name + ".xml"));
        if(configFile.exists()){
            configFile.remove();
        }

    }

    //############################
    //remove mConfig from database
    //############################

    SystemDbManager::removeMeasurementConfig(name);

    //#############################################
    //remove mConfig from the list of saved configs
    //#############################################

    MeasurementConfig mConfig = this->savedMeasurementConfigMap.take(name);
    this->savedMeasurementConfigList.removeOne(mConfig);

    //############
    //emit signals
    //############

    emit this->measurementConfigurationsChanged();

}

/*!
 * \brief MeasurementConfigManager::updateGeometries
 * Calles whenever a measurement config has been added or removed
 */
void MeasurementConfigManager::updateGeometries(){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    //get a list of used measurement configs
    const QList<QPair<QString, bool> > &usedConfigs = this->currentJob->getUsedMeasurementConfigs();

    //check each used measurement config (wether it still exists)
    QList<QPair<QString, bool> > removedConfigs;
    QPair<QString, bool> key;
    foreach(key, usedConfigs){
        if(key.second && !this->savedMeasurementConfigMap.contains(key.first)){
            removedConfigs.append(key);
        }else if(!key.second && !this->projectMeasurementConfigMap.contains(key.first)){
            removedConfigs.append(key);
        }
    }

    //reset all geometry's mConfigs whose config has been removed
    foreach(key, removedConfigs){

        //get geometries by mConfig
        QList<QPointer<Geometry> > geometries = this->currentJob->getGeometriesByMConfig(key);

        //reset mConfigs
        foreach(const QPointer<Geometry> &geom, geometries){
            if(!geom.isNull()){
                geom->setMeasurementConfig(MeasurementConfig());
            }
        }

    }

}

/*!
 * \brief MeasurementConfigManager::updateGeometries
 * Called whenever the attributes of an existing measurement config have changed
 * \param oldMConfig
 * \param newMConfig
 */
void MeasurementConfigManager::updateGeometries(const MeasurementConfig &oldMConfig, const MeasurementConfig &newMConfig){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    //check both configs
    if(!oldMConfig.getIsValid() || !newMConfig.getIsValid()){
        return;
    }

    //get a list of geometries which are using the old config
    QPair<QString, bool> mConfig;
    mConfig.first = oldMConfig.getName();
    mConfig.second = oldMConfig.getIsSaved();
    QList<QPointer<Geometry> > geometries = this->currentJob->getGeometriesByMConfig(mConfig);

    //pass the new config to the geometries
    foreach(const QPointer<Geometry> &geom, geometries){
        if(!geom.isNull()){
            geom->setMeasurementConfig(newMConfig);
        }
    }

}

/*!
 * \brief MeasurementConfigManager::connectJob
 */
void MeasurementConfigManager::connectJob(){

}

/*!
 * \brief MeasurementConfigManager::disconnectJob
 */
void MeasurementConfigManager::disconnectJob(){

}

/*!
 * \brief MeasurementConfigManager::equals
 * \param mConfigA
 * \param mConfigB
 * \return
 */
bool MeasurementConfigManager::equals(const MeasurementConfig &mConfigA, const MeasurementConfig &mConfigB){

    //compare general attributes
    if(mConfigA.getName().compare(mConfigB.getName()) != 0){
        return false;
    }

    //compare measurement config values
    if(mConfigA.getCount() != mConfigB.getCount()
            || mConfigA.getIterations() != mConfigB.getIterations()
            || mConfigA.getMeasureTwoSides() != mConfigB.getMeasureTwoSides()
            || mConfigA.getTimeDependent() != mConfigB.getTimeDependent()
            || mConfigA.getDistanceDependent() != mConfigB.getDistanceDependent()
            || mConfigA.getTimeInterval() != mConfigB.getTimeInterval()
            || !almostEqual(mConfigA.getDistanceInterval(), mConfigB.getDistanceInterval(), 8)
            || mConfigA.getTypeOfReading() != mConfigB.getTypeOfReading()
            || mConfigA.getMeasureWithAim() != mConfigB.getMeasureWithAim()
            ){
        return false;
    }

    return true;

}
