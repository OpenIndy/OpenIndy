#include "oiconfigstate.h"

//initialize static variables
OiConfigState *OiConfigState::myConfigState = OiConfigState::getInstance();
QList<MeasurementConfig> OiConfigState::savedMeasurementConfigs;
QList<MeasurementConfig> OiConfigState::projectMeasurementConfigs;
QMap<QString, QList<Reading*> > OiConfigState::usedMeasurementConfigs;

/*!
 * \brief OiConfigState::OiConfigState
 * \param parent
 */
OiConfigState::OiConfigState(QObject *parent) : QObject(parent){

    //load all configs from config folder
    OiConfigState::loadSavedMeasurementConfigs();
    OiConfigState::loadSavedSensorConfigs();

}

/*!
 * \brief OiConfigState::getInstance
 * Returns a new or the previously created instance of this class
 * \return
 */
OiConfigState *OiConfigState::getInstance(){
    if(OiConfigState::myConfigState == NULL){
        OiConfigState::myConfigState = new OiConfigState();
    }
    return OiConfigState::myConfigState;
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
    QList<QString> mConfigNames;

    //load all files and create a MeasurementConfig object for each of them
    for(int i = 0; i < fileNames.size(); i++){

        //create file from file name and check if it exists
        QFile mConfigFile(fileNames.at(i));
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
        if(!savedConfig.fromOpenIndyXML(mConfigXml.documentElement())){
            continue;
        }
        savedConfig.isSaved = true;

        //check if a measurement config with the same name has been loaded before
        if(mConfigNames.contains(savedConfig.name)){

            //delete the config file permanently
            mConfigFile.remove();
            continue;

        }

        //add the loaded measurement config to the list of saved configs and emit the corresponding signal
        OiConfigState::savedMeasurementConfigs.append(savedConfig);
        OiConfigState::getInstance()->emitSignal(OiConfigState::eMeasurementConfigAdded);

    }

}

/*!
 * \brief OiConfigState::loadSavedSensorConfigs
 * Load all sensor configs from config folder next to the executable
 */
void OiConfigState::loadSavedSensorConfigs(){

}

/*!
 * \brief OiConfigState::connectFeature
 * \param myFeature
 */
void OiConfigState::connectFeature(FeatureWrapper *myFeature){

    if(myFeature->getGeometry() != NULL){
        connect(myFeature->getGeometry(), SIGNAL(geomMyMeasurementConfigChanged(int)), OiConfigState::myConfigState, SLOT(setMeasurementConfig(int)));
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
