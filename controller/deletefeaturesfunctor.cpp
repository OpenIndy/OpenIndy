#include "deletefeaturesfunctor.h"

#include "controller.h"

DeleteFeaturesFunctor::~DeleteFeaturesFunctor(){

}

/*!
 * \brief DeleteFeaturesFunctor::operator ()
 * Call controller method to delete features
 * \param attributes
 */
void DeleteFeaturesFunctor::operator()(QVariantList attributes){
    if(attributes.size() == 1){
        bool command = attributes.at(0).toBool();
        if(c != NULL){
            c->deleteFeaturesCallback(command);
        }
    }
}
