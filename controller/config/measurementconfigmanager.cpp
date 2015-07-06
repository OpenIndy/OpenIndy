#include "measurementconfigmanager.h"

/*!
 * \brief MeasurementConfigManager::MeasurementConfigManager
 */
MeasurementConfigManager::MeasurementConfigManager(QObject *parent) : QObject(parent){

}

/*!
 * \brief MeasurementConfigManager::MeasurementConfigManager
 * \param copy
 * \param parent
 */
MeasurementConfigManager::MeasurementConfigManager(const MeasurementConfigManager &copy, QObject *parent){
    this->savedMeasurementConfigs = copy.savedMeasurementConfigs;
    this->activeMeasurementConfigs = copy.activeMeasurementConfigs;
}

/*!
 * \brief MeasurementConfigManager::operator =
 * \param copy
 * \return
 */
MeasurementConfigManager &MeasurementConfigManager::operator=(const MeasurementConfigManager &copy){
    this->savedMeasurementConfigs = copy.savedMeasurementConfigs;
    this->activeMeasurementConfigs = copy.activeMeasurementConfigs;
    return *this;
}

/*!
 * \brief MeasurementConfigManager::getSavedMeasurementConfig
 * \param name
 * \return
 */
MeasurementConfig MeasurementConfigManager::getSavedMeasurementConfig(const QString &name) const{
    return this->savedMeasurementConfigs.value(name, MeasurementConfig());
}

/*!
 * \brief MeasurementConfigManager::getSavedMeasurementConfigs
 * \return
 */
QList<MeasurementConfig> MeasurementConfigManager::getSavedMeasurementConfigs() const{
    return this->savedMeasurementConfigs.values();
}

/*!
 * \brief MeasurementConfigManager::getProjectMeasurementConfigs
 * \return
 */
QList<MeasurementConfig> MeasurementConfigManager::getProjectMeasurementConfigs() const{
    return this->projectMeasurementConfigs.values();
}

/*!
 * \brief MeasurementConfigManager::getActiveMeasurementConfig
 * \param type
 * \return
 */
const MeasurementConfig &MeasurementConfigManager::getActiveMeasurementConfig(const GeometryTypes &type) const{
    return this->activeMeasurementConfigs.value(type, MeasurementConfig());
}

/*!
 * \brief MeasurementConfigManager::addMeasurementConfig
 * \param mConfig
 */
void MeasurementConfigManager::addMeasurementConfig(const MeasurementConfig &mConfig){

    //check if mConfig is valid
    if(!mConfig.getIsValid()){
        Console::getInstance()->addLine("Cannot add a measurement configuration with an empty name", eErrorMessage);
        return;
    }

    //check if mConfig already exists
    if(this->savedMeasurementConfigs.contains(mConfig.getName())){
        Console::getInstance()->addLine(QString("A measurement configuration with the name %1 already exists").arg(mConfig.getName()), eErrorMessage);
        return;
    }

    //save mConfig
    this->saveMeasurementConfig(mConfig);

}

/*!
 * \brief MeasurementConfigManager::removeMeasurementConfig
 * \param name
 */
void MeasurementConfigManager::removeMeasurementConfig(const QString &name){

    //check name
    if(name.compare("") == 0){
        Console::getInstance()->addLine("Cannot remove a measurement configuration with an empty name", eErrorMessage);
        return;
    }

    //check if the measurement config exists
    if(!this->savedMeasurementConfigs.contains(name)){
        Console::getInstance()->addLine(QString("A measurement configuration with the name %1 does not exist").arg(name), eErrorMessage);
        return;
    }

    //delete mConfig
    this->deleteMeasurementConfig(name);

}

/*!
 * \brief MeasurementConfigManager::replaceMeasurementConfig
 * \param name
 * \param mConfig
 */
void MeasurementConfigManager::replaceMeasurementConfig(const QString &name, const MeasurementConfig &mConfig){

    //get the old measurement config
    if(!this->savedMeasurementConfigs.contains(name)){
        return;
    }
    MeasurementConfig oldConfig = this->savedMeasurementConfigs.value(name);

    //remove the old measurement config
    this->removeMeasurementConfig(oldConfig.getName());

    //add the new measurement config
    this->addMeasurementConfig(mConfig);

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

        //add the loaded measurement config to the list of saved configs and emit the corresponding signal
        this->savedMeasurementConfigs.insert(savedConfig.getName(), savedConfig);

    }

    //get default measurement config
    /*QString mConfigName = SystemDbManager::getDefaultMeasurementConfig();
    if(this->savedSensorConfigs.contains(mConfigName)){
        this->activeSensorConfig = this->savedSensorConfigs.value(sConfigName);
        emit this->activeSensorConfigurationChanged();
    }*/

    //emit signals
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
    //add sConfig to the list of saved configs
    //########################################

    this->savedMeasurementConfigs.insert(mConfig.getName(), mConfig);

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
    //remove sConfig from database
    //############################

    SystemDbManager::removeMeasurementConfig(name);

    //#############################################
    //remove mConfig from the list of saved configs
    //#############################################

    this->savedMeasurementConfigs.remove(name);

    //reset active sensor config
    /*if(name.compare(this->activeSensorConfig.getName()) == 0){
        this->activeSensorConfig = SensorConfiguration();
        emit this->activeSensorConfigurationChanged();
    }*/

    //############
    //emit signals
    //############

    emit this->measurementConfigurationsChanged();

}
