#include "oifeaturestateemitter.h"

OiFeatureStateEmitter *OiFeatureStateEmitter::myStateEmitter = NULL;

OiFeatureStateEmitter::OiFeatureStateEmitter(QObject *parent) :
    QObject(parent)
{
}

/*!
 * \brief OiFeatureStateEmitter::getInstance
 * \return
 */
const OiFeatureStateEmitter *OiFeatureStateEmitter::getInstance(){
    if(OiFeatureStateEmitter::myStateEmitter == NULL){
        OiFeatureStateEmitter::myStateEmitter = new OiFeatureStateEmitter();
    }
    return OiFeatureStateEmitter::myStateEmitter;
}
