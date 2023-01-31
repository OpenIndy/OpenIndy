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

    // initial read only standard configs
    MeasurementConfig fastPoint;
    fastPoint.setName("FastPoint");
    fastPoint.setMaxObservations(1);
    fastPoint.setTimeInterval(0);
    fastPoint.setMeasurementType(MeasurementTypes::eSinglePoint_MeasurementType);
    fastPoint.setMeasurementMode(MeasurementModes::eFast_MeasurementMode);
    fastPoint.setDistanceInterval(0);
    fastPoint.setMeasureTwoSides(false);
    fastPoint.setIsSaved(true);
    this->savedMeasurementConfigMap.insert(fastPoint.getName(), fastPoint);
    this->savedMeasurementConfigList.append(fastPoint);

    MeasurementConfig stdPoint;
    stdPoint.setName("StdPoint");
    stdPoint.setMaxObservations(1);
    stdPoint.setTimeInterval(0);
    stdPoint.setMeasurementType(MeasurementTypes::eSinglePoint_MeasurementType);
    stdPoint.setMeasurementMode(MeasurementModes::eStandard_MeasurementMode);
    stdPoint.setDistanceInterval(0);
    stdPoint.setMeasureTwoSides(false);
    stdPoint.setIsSaved(true);
    this->savedMeasurementConfigMap.insert(stdPoint.getName(), stdPoint);
    this->savedMeasurementConfigList.append(stdPoint);

    MeasurementConfig precisePoint;
    precisePoint.setName("PrecisePoint");
    precisePoint.setMaxObservations(1);
    precisePoint.setTimeInterval(0);
    precisePoint.setMeasurementType(MeasurementTypes::eSinglePoint_MeasurementType);
    precisePoint.setMeasurementMode(MeasurementModes::ePrecise_MeasurementMode);
    precisePoint.setDistanceInterval(0);
    precisePoint.setMeasureTwoSides(false);
    precisePoint.setIsSaved(true);
    this->savedMeasurementConfigMap.insert(precisePoint.getName(), precisePoint);
    this->savedMeasurementConfigList.append(precisePoint);

    MeasurementConfig stdTwoSide;
    stdTwoSide.setName("StdTwoSide");
    stdTwoSide.setMaxObservations(1);
    stdTwoSide.setTimeInterval(0);
    stdTwoSide.setMeasurementType(MeasurementTypes::eSinglePoint_MeasurementType);
    stdTwoSide.setMeasurementMode(MeasurementModes::eStandard_MeasurementMode);
    stdTwoSide.setDistanceInterval(0);
    stdTwoSide.setMeasureTwoSides(true);
    stdTwoSide.setIsSaved(true);
    this->savedMeasurementConfigMap.insert(stdTwoSide.getName(), stdTwoSide);
    this->savedMeasurementConfigList.append(stdTwoSide);

    MeasurementConfig level;
    level.setName("level"); /* lower case */
    level.setMaxObservations(1);
    level.setTimeInterval(0);
    level.setMeasurementType(MeasurementTypes::eLevel_MeasurementType);
    level.setMeasurementMode(MeasurementModes::eFast_MeasurementMode);
    level.setDistanceInterval(0);
    level.setMeasureTwoSides(false);
    level.setIsSaved(true);
    this->savedMeasurementConfigMap.insert(level.getName(), level);
    this->savedMeasurementConfigList.append(level);
}

/*!
 * \brief MeasurementConfigManager::MeasurementConfigManager
 * \param copy
 * \param parent
 */
MeasurementConfigManager::MeasurementConfigManager(const MeasurementConfigManager &copy, QObject *parent){
    this->userConfigs = copy.userConfigs;
    this->projectConfigs = copy.projectConfigs;
    this->activeMeasurementConfigs = copy.activeMeasurementConfigs;
}

/*!
 * \brief MeasurementConfigManager::operator =
 * \param copy
 * \return
 */
MeasurementConfigManager &MeasurementConfigManager::operator=(const MeasurementConfigManager &copy){
    this->userConfigs = copy.userConfigs;
    this->projectConfigs = copy.projectConfigs;
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

bool MeasurementConfigManager::isStandardConfig(const QString &name) {
    return QString("FastPoint").compare(name) == 0
            || QString("StdPoint").compare(name) == 0
            || QString("PrecisePoint").compare(name) == 0
            || QString("StdTwoSide").compare(name) == 0
            || QString("level").compare(name) /* lower case */ == 0;
}

/*!
 * \brief MeasurementConfigManager::isUserConfig
 * Checks wether there is a saved measurement config with the given name
 * \param name
 * \return
 */
bool MeasurementConfigManager::isUserConfig(const QString &name){
    return this->userConfigs.contains(name);
}

/*!
 * \brief MeasurementConfigManager::isProjectConfig
 * Checks wether there is a project measurement config with the given name
 * \param name
 * \return
 */
bool MeasurementConfigManager::isProjectConfig(const QString &name){
    return this->projectConfigs.contains(name);
}

/*!
 * \brief MeasurementConfigManager::isUserConfig
 * Checks wether there is a saved measurement config with the same name and parameters
 * \param mConfig
 * \return
 */
bool MeasurementConfigManager::isUserConfig(const MeasurementConfig &mConfig){

    if(!this->userConfigs.contains(mConfig.getName())){
        return false;
    }

    //get saved config and compare it to the given one
    MeasurementConfig userConfig = this->userConfigs.value(mConfig.getName()); // TODO OI-948
    return userConfig == mConfig;

}

/*!
 * \brief MeasurementConfigManager::isProjectConfig
 * Checks wether there is a project measurement config with the same name and parameters
 * \param mConfig
 * \return
 */
bool MeasurementConfigManager::isProjectConfig(const MeasurementConfig &mConfig){

    if(!this->projectConfigs.contains(mConfig.getName())){
        return false;
    }

    //get project config and compare it to the given one
    MeasurementConfig projectConfig = this->projectConfigs.value(mConfig.getName()); // TODO OI-948
    return projectConfig == mConfig;

}

/*!
 * \brief MeasurementConfigManager::getUserConfig
 * \param name
 * \return
 */
MeasurementConfig MeasurementConfigManager::getUserConfig(const QString &name) const{
    return this->userConfigs.value(name, MeasurementConfig());
}

/*!
 * \brief MeasurementConfigManager::getProjectConfig
 * \param name
 * \return
 */
MeasurementConfig MeasurementConfigManager::getProjectConfig(const QString &name) const{
    return this->projectConfigs.value(name, MeasurementConfig());
}

/*!
 * \brief MeasurementConfigManager::getUserConfigs
 * \return
 */
const QList<MeasurementConfig> MeasurementConfigManager::getUserConfigs() const{
    return this->userConfigs.values();
}

/*!
 * \brief MeasurementConfigManager::getProjectConfigs
 * \return
 */
const QList<MeasurementConfig> MeasurementConfigManager::getProjectConfigs() const{
    return this->projectConfigs.values();
}

/*!
 * \brief MeasurementConfigManager::getActiveConfig
 * \param type
 * \return
 */
MeasurementConfig MeasurementConfigManager::getActiveConfig(const GeometryTypes &type) const{
    return this->activeMeasurementConfigs.value(type, MeasurementConfig());
}

/*!
 * \brief MeasurementConfigManager::addMeasurementConfig
 * \param mConfig
 */
void MeasurementConfigManager::addUserConfig(const MeasurementConfig &mConfig){

    //check if mConfig is valid
    if(!mConfig.getIsValid()){
        emit this->sendMessage("Cannot add a measurement configuration with an empty name", eErrorMessage);
        return;
    }

    //check if mConfig already exists
    if(this->userConfigs.contains(mConfig.getName())){
        emit this->sendMessage(QString("A measurement configuration with the name %1 already exists").arg(mConfig.getName()), eErrorMessage);
        return;
    }

    //save mConfig
    MeasurementConfig savedConfig = mConfig;
    savedConfig.isUserConfig(true);
    this->saveConfig(savedConfig);

}

/*!
 * \brief MeasurementConfigManager::addProjectConfig
 * \param mConfig
 */
void MeasurementConfigManager::addProjectConfig(const MeasurementConfig &mConfig){

    //check if mConfig is valid
    if(!mConfig.getIsValid()){
        emit this->sendMessage("Cannot add a measurement configuration with an empty name", eErrorMessage);
        return;
    }

    //check if mConfig already exists
    if(this->projectConfigs.contains(mConfig.getName())){
        emit this->sendMessage(QString("A measurement configuration with the name %1 already exists").arg(mConfig.getName()), eErrorMessage);
        return;
    }

    //save mConfig
    this->projectConfigs.insert(mConfig.getName(), mConfig);

    emit this->measurementConfigurationsChanged();

}

/*!
 * \brief MeasurementConfigManager::removeMeasurementConfig
 * \param name
 */
void MeasurementConfigManager::removeUserConfig(const QString &name){

    //check name
    if(name.compare("") == 0){
        emit this->sendMessage("Cannot remove a measurement configuration with an empty name", eErrorMessage);
        return;
    }

    //check if the measurement config exists
    if(!this->userConfigs.contains(name)){
        emit this->sendMessage(QString("A measurement configuration with the name %1 does not exist").arg(name), eErrorMessage);
        return;
    }

    //delete mConfig
    this->deleteMeasurementConfig(name);

}

/*!
 * \brief MeasurementConfigManager::removeProjectConfig
 * \param name
 */
void MeasurementConfigManager::removeProjectConfig(const QString &name){

    if(this->projectConfigs.contains(name)){

        this->projectConfigs.remove(name);

        emit this->measurementConfigurationsChanged();

    }

}

/*!
 * \brief MeasurementConfigManager::removeAllUserConfigs
 */
void MeasurementConfigManager::removeAllUserConfigs(){

    //get a list of saved measurement configs
    QList<MeasurementConfig> configs = this->getUserConfigs();

    //remove measurement configs
    foreach(const MeasurementConfig &mConfig, configs){
        this->deleteMeasurementConfig(mConfig.getName());
    }

}

/*!
 * \brief MeasurementConfigManager::removeAllProjectConfigs
 */
void MeasurementConfigManager::removeAllProjectConfigs(){

    this->projectConfigs.clear();

    emit this->measurementConfigurationsChanged();

}

/*!
 * \brief MeasurementConfigManager::replaceMeasurementConfig
 * \param name
 * \param mConfig
 */
void MeasurementConfigManager::replaceMeasurementConfig(const QString &name, const MeasurementConfig &mConfig){

    //get the old measurement config
    if(!this->userConfigs.contains(name)){
        return;
    }
    MeasurementConfig oldConfig = this->userConfigs.value(name);

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
    this->userConfigs.remove(name);
    this->userConfigs.insert(mConfig.getName(), mConfig);

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
        if(this->isStandardConfig(savedConfig.getName())) { // skip standard config from xml
            continue;
        }
        savedConfig.isUserConfig(true);

        //check if a measurement config with the same name has been loaded before
        if(mConfigNames.contains(savedConfig.getName())){

            //delete the config file permanently
            mConfigFile.remove();
            continue;
        }
        mConfigNames.append(savedConfig.getName());

        this->addUserConfig(savedConfig);

        //add the loaded measurement config to the list of saved configs
        if(!this->userConfigs.contains(savedConfig.getName())){
            this->userConfigs.insert(savedConfig.getName(), savedConfig);
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
    this->removeAllUserConfigs();
    this->removeAllProjectConfigs();

    //add new configs
    QList<MeasurementConfig> savedConfigs = other.getUserConfigs();
    QList<MeasurementConfig> projectConfigs = other.getProjectConfigs();
    foreach(const MeasurementConfig &mConfig, savedConfigs){
        this->addUserConfig(mConfig);
    }
    foreach(const MeasurementConfig &mConfig, projectConfigs){
        this->addProjectConfig(mConfig);
    }

    //trigger edits again
    this->blockSignals(false);

    emit this->measurementConfigurationsChanged();

}

/*!
 * \brief MeasurementConfigManager::saveConfig
 * \param mConfig
 */
void MeasurementConfigManager::saveConfig(const MeasurementConfig &mConfig){

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

    if(!this->userConfigs.contains(mConfig.getName())){
        this->userConfigs.insert(mConfig.getName(), mConfig);
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

    this->userConfigs.remove(name);

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
        if(key.second && !this->userConfigs.contains(key.first)){
            removedConfigs.append(key);
        }else if(!key.second && !this->projectConfigs.contains(key.first)){
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
    mConfig.second = oldMConfig.isUserConfig();
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

