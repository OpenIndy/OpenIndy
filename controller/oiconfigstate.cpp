#include "oiconfigstate.h"

//initialize static variables
OiConfigState *OiConfigState::myConfigState = NULL;
QList<MeasurementConfig> OiConfigState::savedMeasurementConfigs;
QList<MeasurementConfig> OiConfigState::projectMeasurementConfigs;
QMap<QString, QList<Reading*> > OiConfigState::usedMeasurementConfigs;
QStringListModel *OiConfigState::measurementConfigNames = new QStringListModel();

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
const MeasurementConfig &OiConfigState::getMeasurementConfig(QString displayName){

    //check saved measurement configs
    foreach(const MeasurementConfig &config, OiConfigState::savedMeasurementConfigs){
        if(config.getDisplayName().compare(displayName) == 0){
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
 * \brief OiConfigState::setMeasurementConfig
 * Add a measurement config to OpenIndy permanently (check if it already exists, before)
 * \param mConfig
 * \return
 */
bool OiConfigState::setMeasurementConfig(MeasurementConfig mConfig){

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
 * \brief OiConfigState::defaultMeasurementConfigChanged
 * \param featureId
 */
void OiConfigState::defaultMeasurementConfigChanged(int featureId){


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
    /*Circle::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::eCircleFeature));
    Cone::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::eConeFeature));
    Cylinder::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::eCylinderFeature));
    Ellipsoid::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::eEllipsoidFeature));
    Hyperboloid::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::eHyperboloidFeature));
    Line::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::eLineFeature));
    Nurbs::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::eNurbsFeature));
    Paraboloid::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::eParaboloidFeature));
    Plane::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::ePlaneFeature));
    Point::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::ePointFeature));
    PointCloud::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::ePointCloudFeature));
    ScalarEntityAngle::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::eScalarEntityAngleFeature));
    ScalarEntityDistance::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::eScalarEntityDistanceFeature));
    ScalarEntityMeasurementSeries::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::eScalarEntityMeasurementSeriesFeature));
    ScalarEntityTemperature::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::eScalarEntityTemperatureFeature));
    //Station::defaultMeasurementConfig = OiConfigState::getMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(Configuration::eSphereFeature));
*/
    //set measurement config names model
    OiConfigState::updateMeasurementConfigModels();

}

/*!
 * \brief OiConfigState::loadSavedSensorConfigs
 * Load all sensor configs from config folder next to the executable
 */
void OiConfigState::loadSavedSensorConfigs(){

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
    OiConfigState::savedMeasurementConfigs.append(mConfig);

    //update the measurement config names model
    OiConfigState::updateMeasurementConfigModels();

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

    if(myFeature->getGeometry() != NULL){
        //connect(myFeature->getGeometry(), SIGNAL(geomMyMeasurementConfigChanged(int)), OiConfigState::myConfigState, SLOT(setMeasurementConfig(int)));
    }

}

/*!
 * \brief OiConfigState::disconnectFeature
 * \param myFeature
 */
void OiConfigState::disconnectFeature(FeatureWrapper *myFeature){

    if(myFeature->getGeometry() != NULL){
        disconnect(myFeature->getGeometry(), SIGNAL(geomMyMeasurementConfigChanged(int)), OiConfigState::myConfigState, SLOT(setMeasurementConfig(int)));
    }

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
