#include "oiconfigstate.h"

//initialize static variables
OiConfigState *OiConfigState::myConfigState = NULL;
QList<MeasurementConfig> OiConfigState::savedMeasurementConfigs;
QList<MeasurementConfig> OiConfigState::projectMeasurementConfigs;
QMap<QString, QList<Reading*> > OiConfigState::usedMeasurementConfigs;
QStringListModel *OiConfigState::measurementConfigNames = new QStringListModel();
QStringListModel *OiConfigState::sensorConfigNames = new QStringListModel();
QList<SensorConfiguration> OiConfigState::savedSensorConfigs;
QList<SensorConfiguration> OiConfigState::projectSensorConfigs;
SensorConfiguration OiConfigState::defaultSensorConfig;

/*!
 * \brief OiConfigState::OiConfigState
 * \param parent
 */
OiConfigState::OiConfigState(QObject *parent) : QObject(parent){
}

/*!
 * \brief OiConfigState::getInstance
 * Returns a new or the previously created instance of this class
 * \return
 */
OiConfigState *OiConfigState::getInstance(){
    if(OiConfigState::myConfigState == NULL){

        OiConfigState::myConfigState = new OiConfigState();

        //load all configs from config folder
        OiConfigState::loadSavedMeasurementConfigs();
        OiConfigState::loadSavedSensorConfigs();

    }
    return OiConfigState::myConfigState;
}

/*!
 * \brief OiConfigState::getMeasurementConfig
 * \param displayName
 * \return
 */
MeasurementConfig OiConfigState::getMeasurementConfig(QString displayName){

    //check saved measurement configs
    foreach(const MeasurementConfig &config, OiConfigState::savedMeasurementConfigs){
        QString configName = config.getDisplayName();
        if(configName.compare(displayName) == 0){
            return config;
        }
    }

    //check project measurement configs
    foreach(const MeasurementConfig &config, OiConfigState::projectMeasurementConfigs){
        if(config.getDisplayName().compare(displayName) == 0){
            return config;
        }
    }

    return MeasurementConfig();

}

/*!
 * \brief OiConfigState::addMeasurementConfig
 * Add a measurement config to OpenIndy permanently (check if it already exists, before)
 * \param mConfig
 * \return
 */
bool OiConfigState::addMeasurementConfig(MeasurementConfig &mConfig){

    //check mConfig wether a measurement config with the same name and/or parameters already exists and if it used, yet
    bool sameName, sameParameters, isUsed;
    OiConfigState::checkMeasurementConfig(mConfig, sameName, sameParameters, isUsed);

    //return false if the measurement config already exists with other parameters and is used by one or more readings
    if(sameName && !sameParameters && isUsed){
        return false;
    }

    //return false if the measurement config (project not saved) already exists with other parameters and is not used by any reading
    if(sameName && !sameParameters && !isUsed && !mConfig.getIsSaved()){
        return false;
    }

    //save measurement config if it does not exist yet
    if(!sameName){
        OiConfigState::saveMeasurementConfig(mConfig);
        mConfig.setIsSaved(true);
    }

    //override measurement config if parameters were changed and it is no project config
    if(sameName && !sameParameters && !isUsed){
        OiConfigState::saveMeasurementConfig(mConfig, true);
        mConfig.setIsSaved(true);
    }

    //set mConfig as default if it is a saved config
    /*if(!mConfig.getIsSaved()){
        return true;
    }

    //set mConfig as default measurement config for the geometry type of geom
    switch(geom->getTypeOfFeature()){
    case Configuration::eCircleFeature:
        Circle::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eCircleFeature, mConfig.getName());
        break;
    case Configuration::eConeFeature:
        Cone::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eConeFeature, mConfig.getName());
        break;
    case Configuration::eCylinderFeature:
        Cylinder::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eCylinderFeature, mConfig.getName());
        break;
    case Configuration::eEllipsoidFeature:
        Ellipsoid::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eEllipsoidFeature, mConfig.getName());
        break;
    case Configuration::eHyperboloidFeature:
        Hyperboloid::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eHyperboloidFeature, mConfig.getName());
        break;
    case Configuration::eLineFeature:
        Line::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eLineFeature, mConfig.getName());
        break;
    case Configuration::eNurbsFeature:
        Nurbs::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eNurbsFeature, mConfig.getName());
        break;
    case Configuration::eParaboloidFeature:
        Paraboloid::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eParaboloidFeature, mConfig.getName());
        break;
    case Configuration::ePlaneFeature:
        Plane::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::ePlaneFeature, mConfig.getName());
        break;
    case Configuration::ePointFeature:
        Point::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::ePointFeature, mConfig.getName());
        break;
    case Configuration::ePointCloudFeature:
        PointCloud::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::ePointCloudFeature, mConfig.getName());
        break;
    case Configuration::eScalarEntityAngleFeature:
        ScalarEntityAngle::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eScalarEntityAngleFeature, mConfig.getName());
        break;
    case Configuration::eScalarEntityDistanceFeature:
        ScalarEntityDistance::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eScalarEntityDistanceFeature, mConfig.getName());
        break;
    case Configuration::eScalarEntityMeasurementSeriesFeature:
        ScalarEntityMeasurementSeries::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eScalarEntityMeasurementSeriesFeature, mConfig.getName());
        break;
    case Configuration::eScalarEntityTemperatureFeature:
        ScalarEntityTemperature::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eScalarEntityTemperatureFeature, mConfig.getName());
        break;
    case Configuration::eSphereFeature:
        Sphere::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eSphereFeature, mConfig.getName());
        break;
    }*/

    return true;

}

/*!
 * \brief OiConfigState::setDefaultMeasurementConfig
 * \param mConfig
 * \param typeOfFeature
 * \return
 */
bool OiConfigState::setDefaultMeasurementConfig(MeasurementConfig mConfig, Configuration::FeatureTypes typeOfFeature){

    if(!mConfig.getIsValid() || !mConfig.getIsSaved()){
        return false;
    }

    //set mConfig as default measurement config for the feature type typeOfFeature
    switch(typeOfFeature){
    case Configuration::eCircleFeature:
        Circle::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eCircleFeature, mConfig.getName());
        break;
    case Configuration::eConeFeature:
        Cone::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eConeFeature, mConfig.getName());
        break;
    case Configuration::eCylinderFeature:
        Cylinder::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eCylinderFeature, mConfig.getName());
        break;
    case Configuration::eEllipsoidFeature:
        Ellipsoid::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eEllipsoidFeature, mConfig.getName());
        break;
    case Configuration::eHyperboloidFeature:
        Hyperboloid::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eHyperboloidFeature, mConfig.getName());
        break;
    case Configuration::eLineFeature:
        Line::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eLineFeature, mConfig.getName());
        break;
    case Configuration::eNurbsFeature:
        Nurbs::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eNurbsFeature, mConfig.getName());
        break;
    case Configuration::eParaboloidFeature:
        Paraboloid::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eParaboloidFeature, mConfig.getName());
        break;
    case Configuration::ePlaneFeature:
        Plane::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::ePlaneFeature, mConfig.getName());
        break;
    case Configuration::ePointFeature:
        Point::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::ePointFeature, mConfig.getName());
        break;
    case Configuration::ePointCloudFeature:
        PointCloud::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::ePointCloudFeature, mConfig.getName());
        break;
    case Configuration::eScalarEntityAngleFeature:
        ScalarEntityAngle::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eScalarEntityAngleFeature, mConfig.getName());
        break;
    case Configuration::eScalarEntityDistanceFeature:
        ScalarEntityDistance::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eScalarEntityDistanceFeature, mConfig.getName());
        break;
    case Configuration::eScalarEntityMeasurementSeriesFeature:
        ScalarEntityMeasurementSeries::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eScalarEntityMeasurementSeriesFeature, mConfig.getName());
        break;
    case Configuration::eScalarEntityTemperatureFeature:
        ScalarEntityTemperature::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eScalarEntityTemperatureFeature, mConfig.getName());
        break;
    case Configuration::eSphereFeature:
        Sphere::defaultMeasurementConfig = mConfig;
        SystemDbManager::setDefaultMeasurementConfig(Configuration::eSphereFeature, mConfig.getName());
        break;
    }

    return true;

}

/*!
 * \brief OiConfigState::createConfigFromSensor
 * Using a given senor plugin this function creates a sensor configuration which uses the defaults defined in the plugin
 * \param pluginName
 * \param sensorName
 * \return
 */
SensorConfiguration OiConfigState::createConfigFromSensor(QString pluginName, QString sensorName){

    SensorConfiguration sConfig;

    //load sensor plugin
    QString path = SystemDbManager::getPluginFilePath(sensorName, pluginName);
    Sensor *mySensor = PluginLoader::loadSensorPlugin(path, sensorName);

    if(mySensor == NULL){
        return sConfig;
    }

    //fill plugin name and sensor name and set sensor object
    sConfig.pluginName = mySensor->getMetaData()->pluginName;
    sConfig.sensorName = mySensor->getMetaData()->name;
    sConfig.mySensor = mySensor;
    sConfig.instrumentType = Configuration::getSensorTypeEnum(mySensor->getMetaData()->iid);

    //fill default accuracies
    QMap<QString, double> defaultAccuracies = *(mySensor->getDefaultAccuracy());
    QStringList sigmaTypes = defaultAccuracies.keys();
    for(int i = 0; i < sigmaTypes.size(); ++i){
        if(sigmaTypes.at(i).compare("sigmaAzimuth") == 0){
            sConfig.sigma.sigmaAzimuth = defaultAccuracies.value(sigmaTypes.at(i));
        }else if(sigmaTypes.at(i).compare("sigmaZenith") == 0){
            sConfig.sigma.sigmaZenith = defaultAccuracies.value(sigmaTypes.at(i));
        }else if(sigmaTypes.at(i).compare("sigmaDistance") == 0){
            sConfig.sigma.sigmaDistance = defaultAccuracies.value(sigmaTypes.at(i));
        }else if(sigmaTypes.at(i).compare("sigmaX") == 0){
            sConfig.sigma.sigmaXyz.setAt(0, defaultAccuracies.value(sigmaTypes.at(i)));
        }else if(sigmaTypes.at(i).compare("sigmaY") == 0){
            sConfig.sigma.sigmaXyz.setAt(1, defaultAccuracies.value(sigmaTypes.at(i)));
        }else if(sigmaTypes.at(i).compare("sigmaZ") == 0){
            sConfig.sigma.sigmaXyz.setAt(2, defaultAccuracies.value(sigmaTypes.at(i)));
        }else if(sigmaTypes.at(i).compare("sigmaTempDeg") == 0){
            sConfig.sigma.sigmaTemp = defaultAccuracies.value(sigmaTypes.at(i));
        }else if(sigmaTypes.at(i).compare("sigmaAngleXZ") == 0){
            sConfig.sigma.sigmaAngleXZ = defaultAccuracies.value(sigmaTypes.at(i));
        }else if(sigmaTypes.at(i).compare("sigmaAngleYZ") == 0){
            sConfig.sigma.sigmaAngleYZ = defaultAccuracies.value(sigmaTypes.at(i));
        }else{
            sConfig.sigma.sigmaUndefined.insert(sigmaTypes.at(i), defaultAccuracies.value(sigmaTypes.at(i)));
        }
    }

    //fill default connection parameters
    QList<Configuration::ConnectionTypes> supportedConnectionTypes = *(mySensor->getConnectionType());
    if(supportedConnectionTypes.size() > 0 && supportedConnectionTypes.at(0) == Configuration::eSerial){
        sConfig.connConfig->typeOfConnection = Configuration::eSerial;
        sConfig.connConfig->baudRate = QSerialPort::Baud1200;
        sConfig.connConfig->comPort = "COM1";
        sConfig.connConfig->dataBits = QSerialPort::Data5;
        sConfig.connConfig->flowControl = QSerialPort::NoFlowControl;
        sConfig.connConfig->parity = QSerialPort::NoParity;
        sConfig.connConfig->stopBits = QSerialPort::OneStop;
    }else if(supportedConnectionTypes.size() > 0 && supportedConnectionTypes.at(0) == Configuration::eNetwork){
        sConfig.connConfig->typeOfConnection = Configuration::eNetwork;
        sConfig.connConfig->ip = "127.0.0.1";
        sConfig.connConfig->port = "80";
    }

    //fill default integer parameters
    if(mySensor->getIntegerParameter() != NULL){
        QMap<QString,int> integerParameters = *(mySensor->getIntegerParameter());
        QStringList intParamNames = integerParameters.keys();
        for(int i = 0; i < intParamNames.size(); ++i){
            sConfig.integerParameter.insert(intParamNames.at(i), integerParameters.value(intParamNames.at(i)));
        }
    }

    //fill default double parameters
    if(mySensor->getDoubleParameter() != NULL){
        QMap<QString,double> doubleParameters = *(mySensor->getDoubleParameter());
        QStringList doubleParamNames = doubleParameters.keys();
        for(int i = 0; i < doubleParamNames.size(); ++i){
            sConfig.doubleParameter.insert(doubleParamNames.at(i), doubleParameters.value(doubleParamNames.at(i)));
        }
    }

    //fill default string parameters
    if(mySensor->getStringParameter() != NULL){
        QMap<QString,QStringList> stringParameters = *(mySensor->getStringParameter());
        QStringList stringParamNames = stringParameters.keys();
        for(int i = 0; i < stringParamNames.size(); ++i){
            sConfig.stringParameter.insert(stringParamNames.at(i), stringParameters.value(stringParamNames.at(i)).at(0));
        }
    }

    return sConfig;

}

/*!
 * \brief OiConfigState::getSensorConfig
 * \param displayName
 * \return
 */
SensorConfiguration OiConfigState::getSensorConfig(QString displayName){

    //check saved sensor configs
    foreach(const SensorConfiguration &config, OiConfigState::savedSensorConfigs){
        QString configName = config.getDisplayName();
        if(configName.compare(displayName) == 0){
            return config;
        }
    }

    //check project sensor configs
    foreach(const SensorConfiguration &config, OiConfigState::projectSensorConfigs){
        if(config.getDisplayName().compare(displayName) == 0){
            return config;
        }
    }

    return SensorConfiguration();

}

/*!
 * \brief OiConfigState::addSensorConfig
 * Adds the given sensor config to OpenIndy permanently or tries to edit an existing one
 * \param sConfig
 * \return
 */
bool OiConfigState::addSensorConfig(SensorConfiguration &sConfig){

    //check if the given config exists as a project sensor config, if so return
    foreach(const SensorConfiguration &config, OiConfigState::projectSensorConfigs){
        if(config.getDisplayName().compare(sConfig.getDisplayName()) == 0){
            return false;
        }
    }

    //add the fiven sensor config to OpenIndy (or edit an existing one)
    OiConfigState::saveSensorConfig(sConfig);
    return true;

}

/*!
 * \brief OiConfigState::addProjectMeasurementConfig
 * Adds a measurement config from another project to current OpenIndy set up
 * \param mConfig
 * \return
 */
/*bool OiConfigState::addProjectMeasurementConfig(const MeasurementConfig &mConfig){


    return false;
}*/

/*!
 * \brief OiConfigState::saveMeasurementConfig
 * Adds a measurement config permanently to this OpenIndy installation
 * \param mConfig
 * \return
 */
/*bool OiConfigState::saveMeasurementConfig(const MeasurementConfig &mConfig){


    return false;
}*/

/*!
 * \brief OiConfigState::getMeasurementConfigNames
 * Returns the names of all available measurement configs
 * \return
 */
QStringListModel *OiConfigState::getMeasurementConfigNames(){
    return OiConfigState::measurementConfigNames;
}

/*!
 * \brief OiConfigState::getSensorConfigNames
 * Returns the names of all available sensor configs
 * \return
 */
QStringListModel *OiConfigState::getSensorConfigNames(){
    return OiConfigState::sensorConfigNames;
}

/*!
 * \brief OiConfigState::defaultMeasurementConfigChanged
 * \param featureId
 */
void OiConfigState::geomMeasurementConfigChanged(int featureId){

//    FeatureWrapper *myGeometry = OiFeatureState::getFeature(featureId);



}

/*!
 * \brief OiConfigState::observationAdded
 * \param featureId
 */
void OiConfigState::observationAdded(int featureId){

    //get coordinate system with id featureId

    //

}

/*!
 * \brief OiConfigState::updateMeasurementConfigModels
 */
void OiConfigState::updateMeasurementConfigModels(){

    QStringList mConfigNames;

    //add saved measurement configs
    foreach(const MeasurementConfig &mConfig, OiConfigState::savedMeasurementConfigs){
        mConfigNames.append(mConfig.name);
    }

    //add project specific measurement configs
    foreach(const MeasurementConfig &mConfig, OiConfigState::projectMeasurementConfigs){
        mConfigNames.append(mConfig.name);
    }

    OiConfigState::measurementConfigNames->setStringList(mConfigNames);

}

/*!
 * \brief OiConfigState::updateSensorConfigModels
 */
void OiConfigState::updateSensorConfigModels(){

    QStringList sConfigNames;

    //add saved sensor configs
    foreach(const SensorConfiguration &sConfig, OiConfigState::savedSensorConfigs){
        sConfigNames.append(sConfig.name);
    }

    //add project specific sensor configs
    foreach(const SensorConfiguration &sConfig, OiConfigState::projectSensorConfigs){
        sConfigNames.append(sConfig.name);
    }

    OiConfigState::sensorConfigNames->setStringList(sConfigNames);

}

/*!
 * \brief OiConfigState::loadSavedMeasurementConfigs
 * Load all measurement configs from config folder next to the executable
 */
void OiConfigState::loadSavedMeasurementConfigs(){

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
        savedConfig.isSaved = true;

        //check if a measurement config with the same name has been loaded before
        if(mConfigNames.contains(savedConfig.getName())){

            //delete the config file permanently
            mConfigFile.remove();
            continue;

        }
        mConfigNames.append(savedConfig.getName());

        //add the loaded measurement config to the list of saved configs and emit the corresponding signal
        OiConfigState::savedMeasurementConfigs.append(savedConfig);
        OiConfigState::getInstance()->emitSignal(OiConfigState::eMeasurementConfigAdded);

    }

    //get default measurement configs
    QString mConfigName = "";
    mConfigName = SystemDbManager::getDefaultMeasurementConfig(Configuration::eCircleFeature);
    Circle::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(mConfigName);
    mConfigName = SystemDbManager::getDefaultMeasurementConfig(Configuration::eConeFeature);
    Cone::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(mConfigName);
    mConfigName = SystemDbManager::getDefaultMeasurementConfig(Configuration::eCylinderFeature);
    Cylinder::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(mConfigName);
    mConfigName = SystemDbManager::getDefaultMeasurementConfig(Configuration::eEllipsoidFeature);
    Ellipsoid::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(mConfigName);
    mConfigName = SystemDbManager::getDefaultMeasurementConfig(Configuration::eHyperboloidFeature);
    Hyperboloid::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(mConfigName);
    mConfigName = SystemDbManager::getDefaultMeasurementConfig(Configuration::eLineFeature);
    Line::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(mConfigName);
    mConfigName = SystemDbManager::getDefaultMeasurementConfig(Configuration::eNurbsFeature);
    Nurbs::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(mConfigName);
    mConfigName = SystemDbManager::getDefaultMeasurementConfig(Configuration::eParaboloidFeature);
    Paraboloid::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(mConfigName);
    mConfigName = SystemDbManager::getDefaultMeasurementConfig(Configuration::ePlaneFeature);
    Plane::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(mConfigName);
    mConfigName = SystemDbManager::getDefaultMeasurementConfig(Configuration::ePointFeature);
    Point::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(mConfigName);
    mConfigName = SystemDbManager::getDefaultMeasurementConfig(Configuration::ePointCloudFeature);
    PointCloud::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(mConfigName);
    mConfigName = SystemDbManager::getDefaultMeasurementConfig(Configuration::eScalarEntityAngleFeature);
    ScalarEntityAngle::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(mConfigName);
    mConfigName = SystemDbManager::getDefaultMeasurementConfig(Configuration::eScalarEntityDistanceFeature);
    ScalarEntityDistance::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(mConfigName);
    mConfigName = SystemDbManager::getDefaultMeasurementConfig(Configuration::eScalarEntityMeasurementSeriesFeature);
    ScalarEntityMeasurementSeries::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(mConfigName);
    mConfigName = SystemDbManager::getDefaultMeasurementConfig(Configuration::eScalarEntityTemperatureFeature);
    ScalarEntityTemperature::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(mConfigName);

    //set measurement config names model
    OiConfigState::updateMeasurementConfigModels();

}

/*!
 * \brief OiConfigState::loadSavedSensorConfigs
 * Load all sensor configs from config folder next to the executable
 */
void OiConfigState::loadSavedSensorConfigs(){

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

    //load all files and create a SensorConfig object for each of them
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

        //try to parse the file to a SensorConfig object
        SensorConfiguration savedConfig;
        QDomElement sConfigTag = sConfigXml.documentElement();
        if(!savedConfig.fromOpenIndyXML(sConfigTag)){
            continue;
        }
        savedConfig.isSaved = true;

        //check if a sensor config with the same name has been loaded before
        if(sConfigNames.contains(savedConfig.getName())){

            //delete the config file permanently
            sConfigFile.remove();
            continue;

        }
        sConfigNames.append(savedConfig.getName());

        //create sensor object and add it to sensor config
        QString path = SystemDbManager::getPluginFilePath(savedConfig.sensorName, savedConfig.pluginName);
        Sensor *mySensor = PluginLoader::loadSensorPlugin(path, savedConfig.sensorName);
        savedConfig.mySensor = mySensor;
        savedConfig.instrumentType = Configuration::getSensorTypeEnum(mySensor->getMetaData()->iid);

        //add the loaded sensor config to the list of saved configs and emit the corresponding signal
        OiConfigState::savedSensorConfigs.append(savedConfig);
        OiConfigState::getInstance()->emitSignal(OiConfigState::eSensorConfigAdded);

    }

    //get default sensor config
    QString sConfigName = SystemDbManager::getDefaultSensorConfig();
    OiConfigState::defaultSensorConfig = OiConfigState::getSensorConfig(sConfigName);

    //set sensor config names model
    OiConfigState::updateSensorConfigModels();

}

/*!
 * \brief OiConfigState::saveMeasurementConfig
 * \param mConfig
 * \param override
 */
void OiConfigState::saveMeasurementConfig(const MeasurementConfig &mConfig, bool override){

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

    //get a list of all xml files inside the measurementConfigs folder
    QStringList xmlFilter;
    xmlFilter.append("*.xml");
    QStringList fileNames = mConfigFolder.entryList(xmlFilter, QDir::Files);

    //set the file name
    QString fileName = mConfig.getName();
    if(!override){

        //create a unique file name
        QString fileName = mConfig.getName();
        while(fileNames.contains(fileName)){
            fileName.append("#");
        }

    }

    //save mConfig to xml file
    QFile configFile(mConfigFolder.absoluteFilePath(fileName.append(".xml")));
    configFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QTextStream stream(&configFile);
    mConfigXml.save(stream, 4);
    configFile.close();

    //save mConfig in database
    SystemDbManager::addMeasurementConfig(mConfig.getName());

    //add mConfig to list of saved measurement configs
    MeasurementConfig savedConfig = OiConfigState::getMeasurementConfig(mConfig.getDisplayName());
    if(!savedConfig.getIsValid()){
        OiConfigState::savedMeasurementConfigs.append(mConfig);
        OiConfigState::savedMeasurementConfigs.last().setIsSaved(true);
    }

    //update the measurement config names model
    OiConfigState::updateMeasurementConfigModels();

}

/*!
 * \brief OiConfigState::saveSensorConfig
 * \param sConfig
 */
void OiConfigState::saveSensorConfig(const SensorConfiguration &sConfig){

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

    //save sConfig in database
    SystemDbManager::addSensorConfig(sConfig.getName());

    //add sConfig to list of saved sensor configs
    SensorConfiguration savedConfig = OiConfigState::getSensorConfig(sConfig.getDisplayName());
    if(!savedConfig.getIsValid()){
        OiConfigState::savedSensorConfigs.append(sConfig);
        OiConfigState::savedSensorConfigs.last().setIsSaved(true);
    }

    //set sConfig as default config
    OiConfigState::defaultSensorConfig = sConfig;
    SystemDbManager::setDefaultSensorConfig(sConfig.getName());

    //update the sensor config names model
    OiConfigState::updateSensorConfigModels();

}

/*!
 * \brief OiConfigState::checkMeasurementConfig
 * Checks wether a measurement config with the same name (sameName) and the same parameters (sameParameters) already exists
 * and wether it has already been used (isUsed) for one or more readings in the current project.
 * \param mConfig
 * \param sameName
 * \param sameParameters
 * \param isUsed
 */
void OiConfigState::checkMeasurementConfig(MeasurementConfig &mConfig, bool &sameName, bool &sameParameters, bool &isUsed){

    //set default values
    sameName = false;
    sameParameters = false;
    isUsed = false;

    //check all saved measurement configs
    foreach(MeasurementConfig config, OiConfigState::savedMeasurementConfigs){

        //if config has the same name as mConfig
        if(config.getName().compare(mConfig.getName()) == 0){

            sameName = true;

            //if mConfig and config are exactly the same
            if(config == mConfig){
                sameParameters = true;
            }

            //check if config has already been used for a reading
            if(OiConfigState::usedMeasurementConfigs.contains(config.getDisplayName())
                    && OiConfigState::usedMeasurementConfigs.value(config.getDisplayName()).size() > 0){
                isUsed = true;
            }

            mConfig.setIsSaved(true);

            return;

        }

    }

    //check all project specific measurement configs
    foreach(MeasurementConfig config, OiConfigState::projectMeasurementConfigs){

        //if config has the same name as mConfig
        if(config.getDisplayName().compare(mConfig.getName()) == 0){

            sameName = true;

            //if mConfig and config are exactly the same
            if(config == mConfig){
                sameParameters = true;
            }

            //check if config has already been used for a reading
            if(OiConfigState::usedMeasurementConfigs.contains(config.getDisplayName())
                    && OiConfigState::usedMeasurementConfigs.value(config.getDisplayName()).size() > 0){
                isUsed = true;
            }

            return;

        }

    }

}

/*!
 * \brief OiConfigState::connectFeature
 * \param myFeature
 */
void OiConfigState::connectFeature(FeatureWrapper *myFeature){

    /*if(myFeature->getGeometry() != NULL){
        connect(myFeature->getGeometry(), SIGNAL(geomMyMeasurementConfigChanged(int)),
                OiConfigState::myConfigState, SLOT(geomMeasurementConfigChanged(int)));
    }*/

}

/*!
 * \brief OiConfigState::disconnectFeature
 * \param myFeature
 */
void OiConfigState::disconnectFeature(FeatureWrapper *myFeature){

    /*if(myFeature->getGeometry() != NULL){
        disconnect(myFeature->getGeometry(), SIGNAL(geomMyMeasurementConfigChanged(int)),
                   OiConfigState::myConfigState, SLOT(geomMeasurementConfigChanged(int)));
    }*/

}

/*!
 * \brief OiConfigState::emitSignal
 * Emits the given signal
 * \param mySignalType
 */
void OiConfigState::emitSignal(OiConfigState::SignalType mySignalType){
    switch(mySignalType){
    case OiConfigState::eMeasurementConfigAdded:
        emit this->measurementConfigAdded();
        break;
    case OiConfigState::eMeasurementConfigRemoved:
        emit this->measurementConfigRemoved();
        break;
    case OiConfigState::eActiveMeasurementConfigChanged:
        emit this->activeMeasurementConfigChanged();
        break;
    }
}
