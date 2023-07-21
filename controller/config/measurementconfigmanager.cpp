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
    fastPoint.makeStandardConfig();
    this->configs.insert(fastPoint.getKey(), fastPoint);

    MeasurementConfig stdPoint;
    stdPoint.setName("StdPoint");
    stdPoint.setMaxObservations(1);
    stdPoint.setTimeInterval(0);
    stdPoint.setMeasurementType(MeasurementTypes::eSinglePoint_MeasurementType);
    stdPoint.setMeasurementMode(MeasurementModes::eStandard_MeasurementMode);
    stdPoint.setDistanceInterval(0);
    stdPoint.setMeasureTwoSides(false);
    stdPoint.makeStandardConfig();
    this->configs.insert(stdPoint.getKey(), stdPoint);

    MeasurementConfig precisePoint;
    precisePoint.setName("PrecisePoint");
    precisePoint.setMaxObservations(1);
    precisePoint.setTimeInterval(0);
    precisePoint.setMeasurementType(MeasurementTypes::eSinglePoint_MeasurementType);
    precisePoint.setMeasurementMode(MeasurementModes::ePrecise_MeasurementMode);
    precisePoint.setDistanceInterval(0);
    precisePoint.setMeasureTwoSides(false);
    precisePoint.makeStandardConfig();
    this->configs.insert(precisePoint.getKey(), precisePoint);

    MeasurementConfig stdTwoSide;
    stdTwoSide.setName("StdTwoSide");
    stdTwoSide.setMaxObservations(1);
    stdTwoSide.setTimeInterval(0);
    stdTwoSide.setMeasurementType(MeasurementTypes::eSinglePoint_MeasurementType);
    stdTwoSide.setMeasurementMode(MeasurementModes::eStandard_MeasurementMode);
    stdTwoSide.setDistanceInterval(0);
    stdTwoSide.setMeasureTwoSides(true);
    stdTwoSide.makeStandardConfig();
    this->configs.insert(stdTwoSide.getKey(), stdTwoSide);

    MeasurementConfig level;
    level.setName("level"); /* lower case */
    level.setMaxObservations(1);
    level.setTimeInterval(0);
    level.setMeasurementType(MeasurementTypes::eLevel_MeasurementType);
    level.setMeasurementMode(MeasurementModes::eFast_MeasurementMode);
    level.setDistanceInterval(0);
    level.setMeasureTwoSides(false);
    level.makeStandardConfig();
    this->configs.insert(level.getKey(), level);

    for(const MeasurementConfig c : configs) {
        SystemDbManager::addMeasurementConfig(c.getName()); // insert if name not exists
        addProjectConfig(c);
    }
}

/*!
 * \brief MeasurementConfigManager::MeasurementConfigManager
 * \param copy
 * \param parent
 */
MeasurementConfigManager::MeasurementConfigManager(const MeasurementConfigManager &copy, QObject *parent){
    this->configs = copy.configs;
    this->activeMeasurementConfigs = copy.activeMeasurementConfigs;
}

/*!
 * \brief MeasurementConfigManager::operator =
 * \param copy
 * \return
 */
MeasurementConfigManager &MeasurementConfigManager::operator=(const MeasurementConfigManager &copy){
    this->configs = copy.configs;
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

MeasurementConfig MeasurementConfigManager::getUserConfig(const QString &name) const{
    return configs.value(MeasurementConfigKey(name, eUserConfig), MeasurementConfig());
}

MeasurementConfig MeasurementConfigManager::getProjectConfig(const QString &name) const{
    return configs.value(MeasurementConfigKey(name, eProjectConfig), MeasurementConfig());
}

MeasurementConfig MeasurementConfigManager::getStandardConfig(const QString &name) const{
    return configs.value(MeasurementConfigKey(name, eStandardConfig), MeasurementConfig());
}

/**
 * @brief MeasurementConfigManager::findConfig find MeasurementConfig by name with fallback
 * @param name
 * @return found MeasurementConfig or invalid MeasurementConfig
 */
MeasurementConfig MeasurementConfigManager::findConfig(const QString &name) const{
    MeasurementConfig mc = this->getStandardConfig(name);
    mc = mc.isValid() ? mc : this->getProjectConfig(name);  // fallback
    mc = mc.isValid() ? mc : this->getUserConfig(name);     // fallback
    return mc;
}

MeasurementConfig MeasurementConfigManager::getConfig(const MeasurementConfigKey &key) const {
    return configs.value(key, MeasurementConfig());
}

/*!
 * \brief MeasurementConfigManager::getUserConfigs
 * \return
 */
const QList<MeasurementConfig> MeasurementConfigManager::getUserConfigs() const{
    QList<MeasurementConfig> l;
    for(const MeasurementConfig c : configs) {
        if(c.isUserConfig()) {
            l.append(c);
        }
    }
    return l;
}

const QList<MeasurementConfig> MeasurementConfigManager::getConfigs() const{
    return configs.values();
}

/*!
 * \brief MeasurementConfigManager::getProjectConfigs
 * \return
 */
const QList<MeasurementConfig> MeasurementConfigManager::getProjectConfigs() const{
    QList<MeasurementConfig> l;
    for(const MeasurementConfig c : configs) {
        if(c.isProjectConfig()) {
            l.append(c);
        }
    }
    return l;
}
const QList<QString> MeasurementConfigManager::getUserConfigNames() const{
    QList<QString> l;
    foreach(const MeasurementConfig c, configs) {
        if(c.isUserConfig()) {
            l.append(c.getName());
        }
    }
    return l;
}

/*!
 * \brief MeasurementConfigManager::getProjectConfigs
 * \return
 */
const QList<QString> MeasurementConfigManager::getProjectConfigNames() const{
    QList<QString> l;
    foreach(const MeasurementConfig c, configs) {
        if(c.isProjectConfig()) {
            l.append(c.getName());
        }
    }
    return l;
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
 * \brief MeasurementConfigManager::saveUserConfig
 * \param mConfig
 */
void MeasurementConfigManager::saveUserConfig(const MeasurementConfig &mConfig){

    //check if mConfig is valid
    if(!mConfig.isValid()){
        emit this->sendMessage("Cannot add a user measurement configuration with an empty name", eErrorMessage);
        return;
    }

    if(!mConfig.isUserConfig()) {
        emit this->sendMessage(QString("Is not a user measurement configuration %1").arg(mConfig.getName()), eErrorMessage);
        return;
    }

    //check if mConfig already exists
    if(this->configs.contains(mConfig.getKey())){
        emit this->sendMessage(QString("A user measurement configuration with the name %1 already exists").arg(mConfig.getName()), eErrorMessage);
        return;
    }

    //save mConfig
    MeasurementConfig userConfig = mConfig;
    userConfig.makeUserConfig();
    this->saveConfig(userConfig);

}

/*!
 * \brief MeasurementConfigManager::addProjectConfig
 * \param mConfig
 */
void MeasurementConfigManager::addProjectConfig(const MeasurementConfig &mConfig){

    //check if mConfig is valid
    if(!mConfig.isValid()){
        emit this->sendMessage("Cannot add a project measurement configuration with an empty name", eErrorMessage);
        return;
    }

    if(!mConfig.isProjectConfig()) {
        emit this->sendMessage(QString("Is not a project measurement configuration %1").arg(mConfig.getName()), eErrorMessage);
        return;
    }

    //check if mConfig already exists
    if(this->configs.contains(mConfig.getKey())){
        emit this->sendMessage(QString("A project measurement configuration with the name %1 already exists").arg(mConfig.getName()), eErrorMessage);
        return;
    }

    MeasurementConfigKey key(mConfig.getName(), eStandardConfig);
    if(this->configs.contains(key)) {
        emit this->sendMessage(QString("A standard measurement configuration with the name %1 already exists").arg(mConfig.getName()), eErrorMessage);
        return;
    }

    //save mConfig
    MeasurementConfig projectConfig = mConfig;
    this->configs.insert(projectConfig.getKey(), projectConfig);

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
    if(!getUserConfigNames().contains(name)){
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

    MeasurementConfigKey key(name, eProjectConfig);
    if(this->configs.contains(key)){

        this->configs.remove(key);

        emit this->measurementConfigurationsChanged();

    }

}

/*!
 * \brief MeasurementConfigManager::replaceMeasurementConfig
 * \param name
 * \param mConfig
 */
void MeasurementConfigManager::replaceMeasurementConfig(const MeasurementConfigKey &oldKey, const MeasurementConfig &mConfig){

    MeasurementConfig oldConfig = this->getConfig(oldKey);
    if(!oldConfig.isValid()){
        return;
    }

    SystemDbManager::removeMeasurementConfig(oldConfig.getName()); // TODO OI-948 keep name because of FK of default measurement config ???
    SystemDbManager::addMeasurementConfig(mConfig.getName());

    // write user config as xml file
    if(mConfig.isUserConfig()) {
        this->saveToConfigFolder(oldConfig.getName(), mConfig);
    }

    //replace mConfig in map
    this->configs.remove(oldKey);
    this->configs.insert(mConfig.getKey(), mConfig);

    emit this->measurementConfigurationReplaced(oldConfig, mConfig);

}

void MeasurementConfigManager::saveToConfigFolder(const QString &name, const MeasurementConfig &mConfig) {
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
        MeasurementConfig userConfig;
        QDomElement mConfigTag = mConfigXml.documentElement();
        if(!userConfig.fromOpenIndyXML(mConfigTag)){
            continue;
        }
        if(userConfig.isStandardConfig()) { // skip standard config from xml
            continue;
        }
        userConfig.makeUserConfig();

        //check if a measurement config with the same name has been loaded before
        if(mConfigNames.contains(userConfig.getName())){

            //delete the config file permanently
            mConfigFile.remove();
            continue;
        }
        mConfigNames.append(userConfig.getName());

        this->saveUserConfig(userConfig);

        //add the loaded measurement config to the list of saved configs
        if(!this->configs.contains(userConfig.getKey())){
            this->configs.insert(userConfig.getKey(), userConfig);
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
    this->configs.clear();

    //add new configs
    foreach(const MeasurementConfig &mConfig, other.getConfigs()){
        if(mConfig.isUserConfig()) {
            this->saveUserConfig(mConfig);
        } else {
            this->addProjectConfig(mConfig);
        }
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

    SystemDbManager::addMeasurementConfig(mConfig.getName()); // TODO OI-948 add key ???

    //########################################
    //add mConfig to the list of saved configs
    //########################################

    if(!this->configs.contains(mConfig.getKey())){ // TODO OI-948 pull up
        this->configs.insert(mConfig.getKey(), mConfig);
    }

    //############
    //emit signals
    //############

    emit this->measurementConfigurationsChanged(); // TODO OI-948 pull up

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

    MeasurementConfigKey key(name, eUserConfig);
    this->configs.remove(key);

    //############
    //emit signals
    //############

    emit this->measurementConfigurationsChanged();

}

/*!
 * \brief MeasurementConfigManager::updateGeometries
 * Calles whenever a measurement config has been added or removed
 */
void MeasurementConfigManager::updateGeometries(){ // TODO OI-948 remove

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    //get a list of used measurement configs
    const QList<MeasurementConfigKey> &usedConfigs = this->currentJob->getUsedMeasurementConfigs();

    //check each used measurement config (wether it still exists)
    QList<MeasurementConfigKey> removedConfigs;
    MeasurementConfigKey key;
    foreach(key, usedConfigs){
        if(!this->configs.contains(key)){
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
                geom->setMeasurementConfig(MeasurementConfigKey());
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
    if(!oldMConfig.isValid() || !newMConfig.isValid()){
        return;
    }

    //get a list of geometries which are using the old config
    QList<QPointer<Geometry> > geometries = this->currentJob->getGeometriesByMConfig(oldMConfig.getKey());

    //pass the new config to the geometries
    foreach(const QPointer<Geometry> &geom, geometries){
        if(!geom.isNull()){
            geom->setMeasurementConfig(newMConfig.getKey());
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

