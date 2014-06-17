#include "p_changeradius.h"

/*!
 * \brief ChangeRadius::getMetaData
 * \return
 */
PluginMetaData* ChangeRadius::getMetaData(){
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "ChangeRadius";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1")
            .arg("This functions adds the specified offset to the radius of the target geometry");
    metaData->iid = "de.openIndy.Plugin.Function.ObjectTransformation.v001";
    return metaData;
}

/*!
 * \brief ChangeRadius::getNeededElements
 * \return
 */
QList<InputParams> ChangeRadius::getNeededElements(){
    QList<InputParams> result;
    InputParams param;
    param.index = 0;
    param.description = "Select a distance which will be added as an offset to the radius of the target geometry.";
    param.infinite = false;
    param.typeOfElement = Configuration::eScalarEntityDistanceElement;
    result.append(param);
    return result;
}

/*!
 * \brief ChangeRadius::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> ChangeRadius::applicableFor(){
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::eSphereFeature);
    return result;
}

/*!
 * \brief ChangeRadius::exec
 * \param targetSphere
 * \return
 */
bool ChangeRadius::exec(Sphere &targetSphere){
    if(this->isValid()){
        ScalarEntityDistance *seDistance = this->getDistance();
        if(seDistance != NULL){
            this->attachOffsetToSphere(targetSphere, seDistance);
            return true;
        }else{
            this->writeToConsole("Not enough valid geometries available for calculation");
            return false;
        }
    }else{
        this->writeToConsole("Not enough valid geometries available for calculation");
        return false;
    }
}

/*!
 * \brief ChangeRadius::getDistance
 * \return
 */
ScalarEntityDistance* ChangeRadius::getDistance(){
    ScalarEntityDistance *result = NULL;
    foreach(ScalarEntityDistance *sed, this->scalarEntityDistances){
        if(result == NULL && sed->getIsSolved()){
            result = sed;
            this->setUseState(result->getId(), true);
        }else{
            this->setUseState(sed->getId(), false);
        }
    }
    return result;
}

/*!
 * \brief ChangeRadius::attachOffsetToSphere
 * \param targetSphere
 * \param distance
 */
void ChangeRadius::attachOffsetToSphere(Sphere &targetSphere, ScalarEntityDistance *distance){
    if(targetSphere.getIsSolved() && distance->getIsSolved()){ //if all elements are solved
        if(targetSphere.getIsUpdated() && distance->getIsUpdated()){ //if all elements are in the same coordinate system
            targetSphere.radius = targetSphere.radius + distance->getDistance();
        }
    }
}
