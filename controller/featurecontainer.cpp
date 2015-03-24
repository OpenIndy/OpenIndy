#include "featurecontainer.h"

/*!
 * \brief FeatureContainer::getFeaturesList
 * \return
 */
FeatureContainer::FeatureContainer(){

}

/*!
 * \brief FeatureContainer::getFeaturesList
 * \return
 */
QList<FeatureWrapper *> &FeatureContainer::getFeaturesList(){
    return this->myFeaturesList;
}

/*!
 * \brief FeatureContainer::getCoordinateSystemsList
 * \return
 */
QList<CoordinateSystem *> &FeatureContainer::getCoordinateSystemsList(){
    return this->myCoordinateSystemsList;
}

/*!
 * \brief FeatureContainer::getStationsList
 * \return
 */
QList<Station *> &FeatureContainer::getStationsList(){
    return this->myStationsList;
}

/*!
 * \brief FeatureContainer::getTransformationParametersList
 * \return
 */
QList<TrafoParam *> &FeatureContainer::getTransformationParametersList(){
    return this->myTransformationParametersList;
}

/*!
 * \brief FeatureContainer::getGeometriesList
 * \return
 */
QList<FeatureWrapper *> &FeatureContainer::getGeometriesList(){
    return this->myGeometriesList;
}

/*!
 * \brief FeatureContainer::getFeatureIdList
 * \return
 */
QList<int> FeatureContainer::getFeatureIdList(){
    return this->myFeaturesIdMap.keys();
}

/*!
 * \brief FeatureContainer::getFeatureNameList
 * \return
 */
QStringList FeatureContainer::getFeatureNameList(){
    return this->myFeaturesNameMap.keys();
}

/*!
 * \brief FeatureContainer::getFeatureGroupList
 * \return
 */
QStringList FeatureContainer::getFeatureGroupList(){
    return this->myFeaturesGroupMap.keys();
}

/*!
 * \brief FeatureContainer::getFeatureById
 * \param featureId
 * \return
 */
FeatureWrapper *FeatureContainer::getFeatureById(const int &featureId){
    return this->myFeaturesIdMap.value(featureId, NULL);
}

/*!
 * \brief FeatureContainer::getFeaturesByName
 * \param name
 * \return
 */
QList<FeatureWrapper *> FeatureContainer::getFeaturesByName(const QString &name){
    return this->myFeaturesNameMap.values(name);
}

/*!
 * \brief FeatureContainer::getFeaturesByGroup
 * \param group
 * \return
 */
QList<FeatureWrapper *> FeatureContainer::getFeaturesByGroup(const QString &group){
    return this->myFeaturesGroupMap.values(group);
}

/*!
 * \brief FeatureContainer::getFeaturesByType
 * \param type
 * \return
 */
QList<FeatureWrapper *> FeatureContainer::getFeaturesByType(const FeatureTypes &type){
    return this->myFeaturesTypeMap.values(type);
}

/*!
 * \brief FeatureContainer::getFeatureCount
 * \return
 */
int FeatureContainer::getFeatureCount(){
    return this->myFeaturesList.size();
}

/*!
 * \brief FeatureContainer::getStationCount
 * \return
 */
int FeatureContainer::getStationCount(){
    return this->myStationsList.size();
}

/*!
 * \brief FeatureContainer::getCoordinateSystemCount
 * \return
 */
int FeatureContainer::getCoordinateSystemCount(){
    return this->myCoordinateSystemsList.size();
}

/*!
 * \brief FeatureContainer::getTransformationParameterCount
 * \return
 */
int FeatureContainer::getTransformationParameterCount(){
    return this->myTransformationParametersList.size();
}

/*!
 * \brief FeatureContainer::getGeometryCount
 * \return
 */
int FeatureContainer::getGeometryCount(){
    return this->myGeometriesList.size();
}

/*!
 * \brief FeatureContainer::addFeature
 * \param myFeature
 * \return
 */
bool FeatureContainer::addFeature(FeatureWrapper *myFeature){

    //if the feature is not valid
    if(myFeature == NULL || myFeature->getFeature() == NULL){
        return false;
    }

    //if the feature already exists it is not added
    if(FeatureContainer::myFeaturesIdMap.contains(myFeature->getFeature()->getId())){
        return false;
    }

    //add the feature to the feature lists and maps
    this->myFeaturesList.append(myFeature);
    this->myFeaturesIdMap.insert(myFeature->getFeature()->getId(), myFeature);
    this->myFeaturesNameMap.insert(myFeature->getFeature()->getFeatureName(), myFeature);
    if(myFeature->getFeature()->getGroupName().compare("") != 0){
        this->myFeaturesGroupMap.insert(myFeature->getFeature()->getGroupName(), myFeature);
    }
    switch(myFeature->getFeatureTypeEnum()){
    case eCoordinateSystemFeature:
        this->myCoordinateSystemsList.append(myFeature->getCoordinateSystem());
        break;
    case eStationFeature:
        this->myStationsList.append(myFeature->getStation());
        break;
    case eTrafoParamFeature:
        this->myTransformationParametersList.append(myFeature->getTrafoParam());
        break;
    default:
        this->myGeometriesList.append(myFeature);
        break;
    }

    return true;

}

/*!
 * \brief FeatureContainer::removeAndDeleteFeature
 * \param featureId
 * \return
 */
bool FeatureContainer::removeAndDeleteFeature(const int &featureId){

    FeatureWrapper *myFeature = this->myFeaturesIdMap.value(featureId);
    if(FeatureContainer::removeFeature(featureId)){
        delete myFeature->getFeature();
        delete myFeature;
        return true;
    }
    return false;

}

/*!
 * \brief FeatureContainer::removeFeature
 * \param featureId
 * \return
 */
bool FeatureContainer::removeFeature(const int &featureId){

    //check if the feature exists
    if(!this->myFeaturesIdMap.contains(featureId)){
        return false;
    }

    FeatureWrapper *myFeature = this->myFeaturesIdMap.value(featureId);

    //check if the feature is valid
    if(myFeature == NULL || myFeature->getFeature() == NULL){
        return false;
    }

    //remove the feature from lists and map
    this->myFeaturesList.removeOne(myFeature);
    this->myFeaturesIdMap.remove(featureId);
    this->myFeaturesNameMap.remove(myFeature->getFeature()->getFeatureName(), myFeature);
    if(myFeature->getFeature()->getGroupName().compare("") != 0){
        this->myFeaturesGroupMap.remove(myFeature->getFeature()->getGroupName(), myFeature);
    }
    switch(myFeature->getFeatureTypeEnum()){
    case eCoordinateSystemFeature:
        this->myCoordinateSystemsList.removeOne(myFeature->getCoordinateSystem());
        break;
    case eStationFeature:
        this->myStationsList.removeOne(myFeature->getStation());
        break;
    case eTrafoParamFeature:
        this->myTransformationParametersList.removeOne(myFeature->getTrafoParam());
        break;
    default:
        this->myGeometriesList.removeOne(myFeature);
        break;
    }

    return true;

}

/*!
 * \brief FeatureContainer::featureNameChanged
 * \param featureId
 * \param oldName
 * \return
 */
bool FeatureContainer::featureNameChanged(const int &featureId, const QString &oldName){

    FeatureWrapper *myFeature = this->myFeaturesIdMap.value(featureId, NULL);

    //if the feature is not valid
    if(myFeature == NULL || myFeature->getFeature() == NULL){
        return false;
    }

    //remove and re-add the feature
    int numRemoved = this->myFeaturesNameMap.remove(oldName, myFeature);

    if(numRemoved == 1){
        this->myFeaturesNameMap.insert(myFeature->getFeature()->getFeatureName(), myFeature);
        return true;
    }
    return false;

}

/*!
 * \brief FeatureContainer::featureGroupChanged
 * \param featureId
 * \param oldGroup
 * \return
 */
bool FeatureContainer::featureGroupChanged(const int &featureId, const QString &oldGroup){

    FeatureWrapper *myFeature = this->myFeaturesIdMap.value(featureId, NULL);

    //if the feature is not valid
    if(myFeature == NULL || myFeature->getFeature() == NULL){
        return false;
    }

    //remove and re-add the feature
    if(oldGroup.compare("") != 0){
        this->myFeaturesGroupMap.remove(oldGroup, myFeature);
    }
    if(myFeature->getFeature()->getGroupName().compare("") != 0){
        this->myFeaturesGroupMap.insert(myFeature->getFeature()->getGroupName(), myFeature);
        return true;
    }
    return false;

}
