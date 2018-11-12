#include "sensorconfigurationmanager.h"

/*!
 * \brief SensorConfigurationManager::SensorConfigurationManager
 */
SensorConfigurationManager::SensorConfigurationManager(QObject *parent) : QObject(parent){

}

/*!
 * \brief SensorConfigurationManager::SensorConfigurationManager
 * \param copy
 * \param parent
 */
SensorConfigurationManager::SensorConfigurationManager(const SensorConfigurationManager &copy, QObject *parent) : QObject(parent){
    this->savedSensorConfigsMap = copy.savedSensorConfigsMap;
    this->savedSensorConfigsList = copy.savedSensorConfigsList;
    this->projectSensorConfigsMap = copy.projectSensorConfigsMap;
    this->projectSensorConfigsList = copy.projectSensorConfigsList;
    this->activeSensorConfig = copy.activeSensorConfig;
}

/*!
 * \brief SensorConfigurationManager::operator =
 * \param copy
 * \return
 */
SensorConfigurationManager &SensorConfigurationManager::operator=(const SensorConfigurationManager &copy){
    this->savedSensorConfigsMap = copy.savedSensorConfigsMap;
    this->savedSensorConfigsList = copy.savedSensorConfigsList;
    this->projectSensorConfigsMap = copy.projectSensorConfigsMap;
    this->projectSensorConfigsList = copy.projectSensorConfigsList;
    this->activeSensorConfig = copy.activeSensorConfig;
    return *this;
}

/*!
 * \brief SensorConfigurationManager::hasSavedSensorConfig
 * Checks wether there is a saved sensor config with the given name
 * \param name
 * \return
 */
bool SensorConfigurationManager::hasSavedSensorConfig(const QString &name){
    return this->savedSensorConfigsMap.contains(name);
}

/*!
 * \brief SensorConfigurationManager::hasProjectSensorConfig
 * Checks wether there is a project sensor config with the given name
 * \param name
 * \return
 */
bool SensorConfigurationManager::hasProjectSensorConfig(const QString &name){
    return this->projectSensorConfigsMap.contains(name);
}

/*!
 * \brief SensorConfigurationManager::hasSavedSensorConfig
 * Checks wether there is a saved sensor config with the same name and parameters
 * \param sConfig
 * \return
 */
bool SensorConfigurationManager::hasSavedSensorConfig(const SensorConfiguration &sConfig){

    if(!this->savedSensorConfigsMap.contains(sConfig.getName())){
        return false;
    }

    //get saved config and compare it to the given one
    SensorConfiguration savedConfig = this->savedSensorConfigsMap.value(sConfig.getName());

    return this->equals(sConfig, savedConfig);

}

/*!
 * \brief SensorConfigurationManager::hasProjectSensorConfig
 * Checks wether there is a project sensor config with the same name and parameters
 * \param sConfig
 * \return
 */
bool SensorConfigurationManager::hasProjectSensorConfig(const SensorConfiguration &sConfig){

    if(!this->projectSensorConfigsMap.contains(sConfig.getName())){
        return false;
    }

    //get project config and compare it to the given one
    SensorConfiguration projectConfig = this->projectSensorConfigsMap.value(sConfig.getName());

    return this->equals(sConfig, projectConfig);

}

/*!
 * \brief SensorConfigurationManager::getSensorConfig
 * Returns the sensor config with the specified name
 * \param name
 * \return
 */
SensorConfiguration SensorConfigurationManager::getSavedSensorConfig(const QString &name) const{
    return this->savedSensorConfigsMap.value(name, SensorConfiguration());
}

/*!
 * \brief SensorConfigurationManager::getProjectSensorConfig
 * \param name
 * \return
 */
SensorConfiguration SensorConfigurationManager::getProjectSensorConfig(const QString &name) const{
    return this->projectSensorConfigsMap.value(name, SensorConfiguration());
}

/*!
 * \brief SensorConfigurationManager::getSavedSensorConfigs
 * \return
 */
const QList<SensorConfiguration> &SensorConfigurationManager::getSavedSensorConfigs() const{
    return this->savedSensorConfigsList;
}

/*!
 * \brief SensorConfigurationManager::getProjectSensorConfigs
 * \return
 */
const QList<SensorConfiguration> &SensorConfigurationManager::getProjectSensorConfigs() const{
    return this->projectSensorConfigsList;
}

/*!
 * \brief SensorConfigurationManager::getActiveSensorConfig
 * Returns the active sensor config
 * \return
 */
const SensorConfiguration &SensorConfigurationManager::getActiveSensorConfig() const{
    return this->activeSensorConfig;
}

/*!
 * \brief SensorConfigurationManager::setActiveSensorConfig
 * \param sConfig
 */
void SensorConfigurationManager::setActiveSensorConfig(const SensorConfiguration &sConfig){
    this->activeSensorConfig = sConfig;
    emit this->activeSensorConfigurationChanged();
}

/*!
 * \brief SensorConfigurationManager::addSavedSensorConfig
 * \param sConfig
 * \param save
 */
void SensorConfigurationManager::addSavedSensorConfig(const SensorConfiguration &sConfig, bool save){

    //check if sConfig is valid
    if(!sConfig.getIsValid()){
        emit this->sendMessage("Cannot add a sensor configuration with an empty name", eErrorMessage);
        return;
    }

    //check if sConfig already exists
    if(this->savedSensorConfigsMap.contains(sConfig.getName())){
        emit this->sendMessage(QString("A sensor configuration with the name %1 already exists").arg(sConfig.getName()), eErrorMessage);
        return;
    }

    //save sConfig
    this->saveSensorConfig(sConfig, save);


}

/*!
 * \brief SensorConfigurationManager::addProjectSensorConfig
 * \param sConfig
 */
void SensorConfigurationManager::addProjectSensorConfig(const SensorConfiguration &sConfig){

    //check if sConfig is valid
    if(!sConfig.getIsValid()){
        emit this->sendMessage("Cannot add a sensor configuration with an empty name", eErrorMessage);
        return;
    }

    //check if sConfig already exists
    if(this->projectSensorConfigsMap.contains(sConfig.getName())){
        emit this->sendMessage(QString("A sensor configuration with the name %1 already exists").arg(sConfig.getName()), eErrorMessage);
        return;
    }

    //save sConfig
    this->projectSensorConfigsList.append(sConfig);
    this->projectSensorConfigsMap.insert(sConfig.getName(), sConfig);

}

/*!
 * \brief SensorConfigurationManager::removeSavedSensorConfig
 * \param name
 * \param save
 */
void SensorConfigurationManager::removeSavedSensorConfig(const QString &name, bool save){

    //check name
    if(name.compare("") == 0){
        emit this->sendMessage("Cannot remove a sensor configuration with an empty name", eErrorMessage);
        return;
    }

    //check if the sensor config exists
    if(!this->savedSensorConfigsMap.contains(name)){
        emit this->sendMessage(QString("A sensor configuration with the name %1 does not exist").arg(name), eErrorMessage);
        return;
    }

    //delete sConfig
    this->deleteSensorConfig(name, save);

}

/*!
 * \brief SensorConfigurationManager::removeProjectSensorConfig
 * \param name
 */
void SensorConfigurationManager::removeProjectSensorConfig(const QString &name){

    //check if the sensor config exists
    if(!this->projectSensorConfigsMap.contains(name)){
        emit this->sendMessage(QString("A sensor configuration with the name %1 does not exist").arg(name), eErrorMessage);
        return;
    }

    //remove sensor config
    SensorConfiguration sConfig = this->projectSensorConfigsMap.take(name);
    this->projectSensorConfigsList.removeOne(sConfig);

}

/*!
 * \brief SensorConfigurationManager::removeAllSensorConfigs
 */
void SensorConfigurationManager::removeAllSavedSensorConfigs(bool save){

    //get a list of saved sensor configs
    QList<SensorConfiguration> configs = this->getSavedSensorConfigs();

    //remove sensor configs
    foreach(const SensorConfiguration &sConfig, configs){
        this->deleteSensorConfig(sConfig.getName(), save);
    }

}

/*!
 * \brief SensorConfigurationManager::removeAllProjectSensorConfigs
 */
void SensorConfigurationManager::removeAllProjectSensorConfigs(){

    this->projectSensorConfigsList.clear();
    this->projectSensorConfigsMap.clear();

    emit this->sensorConfigurationsChanged();

}

/*!
 * \brief SensorConfigurationManager::replaceSensorConfig
 * Replaces the sensor config with the name name by the given sensor config
 * \param name
 * \param sConfig
 * \param save
 */
void SensorConfigurationManager::replaceSensorConfig(const QString &name, const SensorConfiguration &sConfig, bool save){

    //get the old sensor config
    if(!this->savedSensorConfigsMap.contains(name)){
        return;
    }
    SensorConfiguration oldConfig = this->savedSensorConfigsMap.value(name);

    //check active config
    bool isActive = false;
    if(this->activeSensorConfig.getIsSaved() && this->activeSensorConfig.getName().compare(name) == 0){
        isActive = true;
    }

    //###########################
    //replace mConfig in database
    //###########################

    if(save){
        SystemDbManager::removeSensorConfig(name);
        SystemDbManager::addSensorConfig(sConfig.getName());
    }

    //########################
    //replace sConfig xml file
    //########################

    if(save){

        //create xml document
        QDomDocument sConfigXml("sensorConfig");

        //add mConfig to document as xml
        QDomElement root = sConfig.toOpenIndyXML(sConfigXml);
        sConfigXml.appendChild(root);

        //get config folder (create if does not exist yet)
        QString appPath = qApp->applicationDirPath();
        QDir sConfigFolder(appPath.append("/config/sensorConfigs"));
        if(!sConfigFolder.exists()){
            sConfigFolder.mkpath(".");
        }

        //set the file name
        QString fileName = sConfig.getName();

        //remove old config file
        QFile oldConfigFile(sConfigFolder.absoluteFilePath(name + ".xml"));
        if(oldConfigFile.exists()){
            oldConfigFile.remove();
        }

        //save mConfig to xml file
        QFile configFile(sConfigFolder.absoluteFilePath(fileName.append(".xml")));
        configFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
        QTextStream stream(&configFile);
        sConfigXml.save(stream, 4);
        configFile.close();

    }

    //###############################
    //replace sConfig in list and map
    //###############################

    //replace sConfig in map
    this->savedSensorConfigsMap.remove(name);
    this->savedSensorConfigsMap.insert(sConfig.getName(), sConfig);

    //replace sConfig in list
    int index = this->savedSensorConfigsList.indexOf(oldConfig, 0);
    if(index != -1){
        this->savedSensorConfigsList.replace(index, sConfig);
    }

    //update active sensor config
    if(isActive){
        this->activeSensorConfig = sConfig;
    }

    emit this->sensorConfigurationReplaced(oldConfig, sConfig);

}

/*!
 * \brief SensorConfigurationManager::loadFromConfigFolder
 * Loads all available sensor configs from the config folder at application directory
 */
void SensorConfigurationManager::loadFromConfigFolder(){

    //get application path
    QString appPath = qApp->applicationDirPath();
    QDir appFolder(appPath);

    //check if application folder exists and contains a subdirectory named config
    if(!appFolder.exists() || !appFolder.exists("config/sensorConfigs")){
        return;
    }

    //create folder object for sensor config folder
    QDir sConfigFolder(appPath.append("/config/sensorConfigs"));
    if(!sConfigFolder.exists()){
        return;
    }

    //get a list of all xml files inside the sensorConfigs folder
    QStringList xmlFilter;
    xmlFilter.append("*.xml");
    QStringList fileNames = sConfigFolder.entryList(xmlFilter, QDir::Files);

    //create a list that is filled with the names of the loaded sensor configs
    QStringList sConfigNames;

    //load all files and create a SensorConfiguration object for each of them
    for(int i = 0; i < fileNames.size(); i++){

        //create file from file name and check if it exists
        QFile sConfigFile(sConfigFolder.absoluteFilePath(fileNames.at(i)));
        if(!sConfigFile.exists()){
            continue;
        }

        //try to parse the file content to a QDomDocument
        QDomDocument sConfigXml;
        if(!sConfigXml.setContent(&sConfigFile) || sConfigXml.isNull()){
            continue;
        }

        //try to parse the file to a SensorConfiguration object
        SensorConfiguration savedConfig;
        QDomElement sConfigTag = sConfigXml.documentElement();
        if(!savedConfig.fromOpenIndyXML(sConfigTag)){
            continue;
        }
        savedConfig.setIsSaved(true);

        //check if a sensor config with the same name has been loaded before
        if(sConfigNames.contains(savedConfig.getName())){

            //delete the config file permanently
            sConfigFile.remove();
            continue;

        }
        sConfigNames.append(savedConfig.getName());
        this->addSavedSensorConfig(savedConfig);

        //add the loaded sensor config to the list of saved configs
        if(!this->savedSensorConfigsMap.contains(savedConfig.getName())){
            this->savedSensorConfigsMap.insert(savedConfig.getName(), savedConfig);
            this->savedSensorConfigsList.append(savedConfig);
        }
    }

    //get default sensor config
    QString sConfigName = SystemDbManager::getDefaultSensorConfig();
    if(this->savedSensorConfigsMap.contains(sConfigName)){
        this->activeSensorConfig = this->savedSensorConfigsMap.value(sConfigName);
        emit this->activeSensorConfigurationChanged();
    }

    //emit signals
    emit this->sensorConfigurationsChanged();

}

/*!
 * \brief SensorConfigurationManager::synchronize
 * \param other
 */
void SensorConfigurationManager::synchronize(const SensorConfigurationManager &other){

    //do not trigger signals during synchronization
    this->blockSignals(true);

    //remove sensor configs
    this->removeAllSavedSensorConfigs();
    this->removeAllProjectSensorConfigs();

    //add new configs
    QList<SensorConfiguration> savedConfigs = other.getSavedSensorConfigs();
    QList<SensorConfiguration> projectConfigs = other.getProjectSensorConfigs();
    foreach(const SensorConfiguration &sConfig, savedConfigs){
        this->addSavedSensorConfig(sConfig);
    }
    foreach(const SensorConfiguration &sConfig, projectConfigs){
        this->addProjectSensorConfig(sConfig);
    }

    //trigger edits again
    this->blockSignals(false);

    emit this->sensorConfigurationsChanged();

}

/*!
 * \brief SensorConfigurationManager::saveSensorConfig
 * \param sConfig
 * \param save
 */
void SensorConfigurationManager::saveSensorConfig(const SensorConfiguration &sConfig, bool save){

    //###################################
    //create config file at config folder
    //###################################

    if(save){

        //create xml document
        QDomDocument sConfigXml("sensorConfig");

        //add sConfig to document as xml
        QDomElement root = sConfig.toOpenIndyXML(sConfigXml);
        sConfigXml.appendChild(root);

        //get config folder (create if does not exist yet)
        QString appPath = qApp->applicationDirPath();
        QDir sConfigFolder(appPath.append("/config/sensorConfigs"));
        if(!sConfigFolder.exists()){
            sConfigFolder.mkpath(".");
        }

        //set the file name
        QString fileName = sConfig.getName();

        //save sConfig to xml file
        QFile configFile(sConfigFolder.absoluteFilePath(fileName.append(".xml")));
        configFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
        QTextStream stream(&configFile);
        sConfigXml.save(stream, 4);
        configFile.close();

    }

    //##################
    //add database entry
    //##################

    if(save){
        SystemDbManager::addSensorConfig(sConfig.getName());
    }

    //########################################
    //add sConfig to the list of saved configs
    //########################################

    if(!this->savedSensorConfigsMap.contains(sConfig.getName())){
        this->savedSensorConfigsMap.insert(sConfig.getName(), sConfig);
        this->savedSensorConfigsList.append(sConfig);
    }

    //############
    //emit signals
    //############

    emit this->sensorConfigurationsChanged();

}

/*!
 * \brief SensorConfigurationManager::deleteSensorConfig
 * \param name
 * \param save
 */
void SensorConfigurationManager::deleteSensorConfig(const QString &name, bool save){

    //#####################################
    //remove config file from config folder
    //#####################################

    //get config folder
    if(save){
        QString appPath = qApp->applicationDirPath();
        QDir sConfigFolder(appPath.append("/config/sensorConfigs"));
        if(sConfigFolder.exists()){

            //get config file
            QFile configFile(sConfigFolder.absoluteFilePath(name + ".xml"));
            if(configFile.exists()){
                configFile.remove();
            }

        }
    }

    //############################
    //remove sConfig from database
    //############################

    if(save){
        SystemDbManager::removeSensorConfig(name);
    }

    //#############################################
    //remove sConfig from the list of saved configs
    //#############################################

    SensorConfiguration sConfig = this->savedSensorConfigsMap.take(name);
    this->savedSensorConfigsList.removeOne(sConfig);

    //reset active sensor config
    if(name.compare(this->activeSensorConfig.getName()) == 0 && this->activeSensorConfig.getIsSaved()){
        this->activeSensorConfig = SensorConfiguration();
        emit this->activeSensorConfigurationChanged();
    }

    //############
    //emit signals
    //############

    emit this->sensorConfigurationsChanged();

}

/*!
 * \brief SensorConfigurationManager::equals
 * \param sConfigA
 * \param sConfigB
 * \return
 */
bool SensorConfigurationManager::equals(const SensorConfiguration &sConfigA, const SensorConfiguration &sConfigB){

    //compare general attributes
    if(sConfigA.getName().compare(sConfigB.getName()) != 0
            || sConfigA.getIsSaved() != sConfigB.getIsSaved()
            || sConfigA.getPluginName().compare(sConfigB.getPluginName()) != 0
            || sConfigA.getSensorName().compare(sConfigB.getSensorName()) != 0
            || sConfigA.getTypeOfSensor() != sConfigB.getTypeOfSensor()){
        return false;
    }

    //compare accuracy values
    if(!almostEqual(sConfigA.getAccuracy().sigmaAzimuth, sConfigB.getAccuracy().sigmaAzimuth, 8)
            || !almostEqual(sConfigA.getAccuracy().sigmaZenith, sConfigB.getAccuracy().sigmaZenith, 8)
            || !almostEqual(sConfigA.getAccuracy().sigmaDistance, sConfigB.getAccuracy().sigmaDistance, 8)
            || !almostEqual(sConfigA.getAccuracy().sigmaXyz.getAt(0), sConfigB.getAccuracy().sigmaXyz.getAt(0), 8)
            || !almostEqual(sConfigA.getAccuracy().sigmaXyz.getAt(1), sConfigB.getAccuracy().sigmaXyz.getAt(1), 8)
            || !almostEqual(sConfigA.getAccuracy().sigmaXyz.getAt(2), sConfigB.getAccuracy().sigmaXyz.getAt(2), 8)
            || !almostEqual(sConfigA.getAccuracy().sigmaTemp, sConfigB.getAccuracy().sigmaTemp, 8)
            || !almostEqual(sConfigA.getAccuracy().sigmaI, sConfigB.getAccuracy().sigmaI, 8)
            || !almostEqual(sConfigA.getAccuracy().sigmaJ, sConfigB.getAccuracy().sigmaJ, 8)
            || !almostEqual(sConfigA.getAccuracy().sigmaK, sConfigB.getAccuracy().sigmaK, 8)
            || sConfigA.getAccuracy().sigmaUndefined.keys() != sConfigB.getAccuracy().sigmaUndefined.keys()){
        return false;
    }

    //compare undefined accuracies
    QList<QString> keys = sConfigA.getAccuracy().sigmaUndefined.keys();
    foreach(const QString &key, keys){
        if(!almostEqual(sConfigA.getAccuracy().sigmaUndefined.value(key),
                        sConfigB.getAccuracy().sigmaUndefined.value(key), 8)){
            return false;
        }
    }

    //compare string parameters
    if(sConfigA.getStringParameter() != sConfigB.getStringParameter()){
        return false;
    }

    //compare double parameters
    if(sConfigA.getDoubleParameter().keys() != sConfigB.getDoubleParameter().keys()){
        return false;
    }else{
        QList<QString> keys = sConfigA.getDoubleParameter().keys();
        foreach(const QString &key, keys){
            if(!almostEqual(sConfigA.getDoubleParameter().value(key),
                            sConfigB.getDoubleParameter().value(key), 8)){
                return false;
            }
        }
    }

    //compare integer parameters
    if(sConfigA.getIntegerParameter() != sConfigB.getIntegerParameter()){
        return false;
    }

    //compare available string parameters
    if(sConfigA.getAvailableStringParameter() != sConfigB.getAvailableStringParameter()){
        return false;
    }

    return true;

}
