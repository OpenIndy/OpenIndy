#include "oifunctionemitter.h"

OiFunctionEmitter::OiFunctionEmitter(QObject *parent) :
    OiEmitter(parent)
{
}

/*!
 * \brief OiFunctionEmitter::addFeature
 * Emits a signal so that the given feature appears in OpenIndy
 * \param myFeature
 */
void OiFunctionEmitter::addFeature(FeatureWrapper *myFeature){
    if(myFeature != NULL && myFeature->getFeature() != NULL){
        emit this->featureAdded(myFeature);
    }
}
