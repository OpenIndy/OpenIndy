#include "oiconfigstate.h"

OiConfigState *OiConfigState::myConfigState = OiConfigState::getInstance();

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
    }
    return OiConfigState::myConfigState;
}

/*!
 * \brief OiConfigState::getActiveMeasurementConfig
 * \return
 */
const MeasurementConfig &OiConfigState::getActiveMeasurementConfig(){
    return OiConfigState::myConfigState->activeMeasurementConfig;
}

void OiConfigState::featureSetChanged()
{

}

/*!
 * \brief OiConfigState::setMeasurementConfig
 * \param id
 */
void OiConfigState::setMeasurementConfig(int id){

    /*FeatureWrapper *myFeature = OiFeatureState::getFeature(id);
    if(myFeature != NULL && myFeature->getGeometry() != NULL){

        //remove the feature from old measurement config map
        QMap< MeasurementConfig, QList<FeatureWrapper*> >::iterator i;
        for (i = OiConfigState::usedMeasurementConfigs.begin(); i != OiConfigState::usedMeasurementConfigs.end(); ++i){
            QList<FeatureWrapper*> myFeatures = i.value();
            if(myFeatures.contains(myFeature)){
                myFeatures.removeOne(myFeature);
                if(myFeatures.size() == 0){ //if the measurement config is not used anymore
                    OiConfigState::usedMeasurementConfigs.remove(i.key());
                }else{ //if there are other features which use this measurement config
                    OiConfigState::usedMeasurementConfigs.insert(i.key(), myFeatures);
                }
                break;
            }
        }

    }*/
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

}
