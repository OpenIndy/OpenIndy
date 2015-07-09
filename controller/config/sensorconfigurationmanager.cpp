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
    this->savedSensorConfigs = copy.savedSensorConfigs;
    this->activeSensorConfig = copy.activeSensorConfig;
}

/*!
 * \brief SensorConfigurationManager::operator =
 * \param copy
 * \return
 */
SensorConfigurationManager &SensorConfigurationManager::operator=(const SensorConfigurationManager &copy){
    this->savedSensorConfigs = copy.savedSensorConfigs;
    this->activeSensorConfig = copy.activeSensorConfig;
    return *this;
}

/*!
 * \brief SensorConfigurationManager::getSensorConfig
 * Returns the sensor config with the specified name
 * \param name
 * \return
 */
SensorConfiguration SensorConfigurationManager::getSavedSensorConfig(const QString &name) const{
    return this->savedSensorConfigs.value(name, SensorConfiguration());
}

/*!
 * \brief SensorConfigurationManager::getSavedSensorConfigs
 * \return
 */
QList<SensorConfiguration> SensorConfigurationManager::getSavedSensorConfigs() const{
    return this->savedSensorConfigs.values();
}

/*!
 * \brief SensorConfigurationManager::getProjectSensorConfigs
 * \return
 */
QList<SensorConfiguration> SensorConfigurationManager::getProjectSensorConfigs() const{
    return this->projectSensorConfigs.values();
}

/*!
 * \brief SensorConfigurationManager::getActiveSensorConfig
 * Returns the active sensor config
 * \param name
 * \return
 */
const SensorConfiguration &SensorConfigurationManager::getActiveSensorConfig(const QString &name) const{
    return this->activeSensorConfig;
}

/*!
 * \brief SensorConfigurationManager::addSensorConfig
 * \param sConfig
 */
void SensorConfigurationManager::addSensorConfig(const SensorConfiguration &sConfig){

    //check if sConfig is valid
    if(!sConfig.getIsValid()){
        Console::getInstance()->addLine("Cannot add a sensor configuration with an empty name", eErrorMessage);
        return;
    }

    //check if sConfig already exists
    if(this->savedSensorConfigs.contains(sConfig.getName())){
        Console::getInstance()->addLine(QString("A sensor configuration with the name %1 already exists").arg(sConfig.getName()), eErrorMessage);
        return;
    }

    //save sConfig
    this->saveSensorConfig(sConfig);


}

/*!
 * \brief SensorConfigurationManager::removeSensorConfig
 * \param name
 */
void SensorConfigurationManager::removeSensorConfig(const QString &name){

    //check name
    if(name.compare("") == 0){
        Console::getInstance()->addLine("Cannot remove a sensor configuration with an empty name", eErrorMessage);
        return;
    }

    //check if the sensor config exists
    if(!this->savedSensorConfigs.contains(name)){
        Console::getInstance()->addLine(QString("A sensor configuration with the name %1 does not exist").arg(name), eErrorMessage);
        return;
    }

    //delete sConfig
    this->deleteSensorConfig(name);

}

/*!
 * \brief SensorConfigurationManager::replaceSensorConfig
 * Replaces the sensor config with the name name by the given sensor config
 * \param name
 * \param sConfig
 */
void SensorConfigurationManager::replaceSensorConfig(const QString &name, const SensorConfiguration &sConfig){

    //get the old sensor config
    if(!this->savedSensorConfigs.contains(name)){
        return;
    }
    SensorConfiguration oldConfig = this->savedSensorConfigs.value(name);

    //remove the old sensor config
    this->removeSensorConfig(oldConfig.getName());

    //add the new sensor config
    this->addSensorConfig(sConfig);

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

        //add the loaded sensor config to the list of saved configs and emit the corresponding signal
        this->savedSensorConfigs.insert(savedConfig.getName(), savedConfig);

    }

    //get default sensor config
    QString sConfigName = SystemDbManager::getDefaultSensorConfig();
    if(this->savedSensorConfigs.contains(sConfigName)){
        this->activeSensorConfig = this->savedSensorConfigs.value(sConfigName);
        emit this->activeSensorConfigurationChanged();
    }

    //emit signals
    emit this->sensorConfigurationsChanged();

}

/*!
 * \brief SensorConfigurationManager::saveSensorConfig
 * \param sConfig
 */
void SensorConfigurationManager::saveSensorConfig(const SensorConfiguration &sConfig){

    //###################################
    //create config file at config folder
    //###################################

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

    //##################
    //add database entry
    //##################

    SystemDbManager::addSensorConfig(sConfig.getName());

    //########################################
    //add sConfig to the list of saved configs
    //########################################

    this->savedSensorConfigs.insert(sConfig.getName(), sConfig);

    //############
    //emit signals
    //############

    emit this->sensorConfigurationsChanged();

}

/*!
 * \brief SensorConfigurationManager::deleteSensorConfig
 * \param name
 */
void SensorConfigurationManager::deleteSensorConfig(const QString &name){

    //#####################################
    //remove config file from config folder
    //#####################################

    //get config folder
    QString appPath = qApp->applicationDirPath();
    QDir sConfigFolder(appPath.append("/config/sensorConfigs"));
    if(sConfigFolder.exists()){

        //get config file
        QFile configFile(sConfigFolder.absoluteFilePath(name + ".xml"));
        if(configFile.exists()){
            configFile.remove();
        }

    }

    //############################
    //remove sConfig from database
    //############################

    SystemDbManager::removeSensorConfig(name);

    //#############################################
    //remove sConfig from the list of saved configs
    //#############################################

    this->savedSensorConfigs.remove(name);

    //reset active sensor config
    if(name.compare(this->activeSensorConfig.getName()) == 0){
        this->activeSensorConfig = SensorConfiguration();
        emit this->activeSensorConfigurationChanged();
    }

    //############
    //emit signals
    //############

    emit this->sensorConfigurationsChanged();

}
