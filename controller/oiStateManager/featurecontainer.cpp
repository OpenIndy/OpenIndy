#include "featurecontainer.h"

//initialize static variables
QList<FeatureWrapper *> FeatureContainer::myFeaturesList;
QList<CoordinateSystem *> FeatureContainer::myCoordinateSystemsList;
QList<Station *> FeatureContainer::myStationsList;
QList<TrafoParam *> FeatureContainer::myTransformationParametersList;
QList<FeatureWrapper *> FeatureContainer::myGeometriesList;
QMap<int, FeatureWrapper *> FeatureContainer::myFeaturesIdMap;
QMultiMap<QString, FeatureWrapper *> FeatureContainer::myFeaturesNameMap;
QMultiMap<QString, FeatureWrapper *> FeatureContainer::myFeaturesGroupMap;

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
    return FeatureContainer::myFeaturesList;
}

/*!
 * \brief FeatureContainer::getCoordinateSystemsList
 * \return
 */
QList<CoordinateSystem *> &FeatureContainer::getCoordinateSystemsList(){
    return FeatureContainer::myCoordinateSystemsList;
}

/*!
 * \brief FeatureContainer::getStationsList
 * \return
 */
QList<Station *> &FeatureContainer::getStationsList(){
    return FeatureContainer::myStationsList;
}

/*!
 * \brief FeatureContainer::getTransformationParametersList
 * \return
 */
QList<TrafoParam *> &FeatureContainer::getTransformationParametersList(){
    return FeatureContainer::myTransformationParametersList;
}

/*!
 * \brief FeatureContainer::getGeometriesList
 * \return
 */
QList<FeatureWrapper *> &FeatureContainer::getGeometriesList(){
    return FeatureContainer::myGeometriesList;
}

/*!
 * \brief FeatureContainer::getFeatureIdList
 * \return
 */
QList<int> FeatureContainer::getFeatureIdList(){
    return FeatureContainer::myFeaturesIdMap.keys();
}

/*!
 * \brief FeatureContainer::getFeatureNameList
 * \return
 */
QStringList FeatureContainer::getFeatureNameList(){
    return FeatureContainer::myFeaturesNameMap.keys();
}

/*!
 * \brief FeatureContainer::getFeatureGroupList
 * \return
 */
QStringList FeatureContainer::getFeatureGroupList(){
    return FeatureContainer::myFeaturesGroupMap.keys();
}

/*!
 * \brief FeatureContainer::getFeatureById
 * \param featureId
 * \return
 */
FeatureWrapper *FeatureContainer::getFeatureById(const int &featureId){
    return FeatureContainer::myFeaturesIdMap.value(featureId, NULL);
}

/*!
 * \brief FeatureContainer::getFeaturesByName
 * \param name
 * \return
 */
QList<FeatureWrapper *> FeatureContainer::getFeaturesByName(const QString &name){
    return FeatureContainer::myFeaturesNameMap.values(name);
}

/*!
 * \brief FeatureContainer::getFeaturesByGroup
 * \param group
 * \return
 */
QList<FeatureWrapper *> FeatureContainer::getFeaturesByGroup(const QString &group){
    return FeatureContainer::myFeaturesGroupMap.values(group);
}

/*!
 * \brief FeatureContainer::getFeatureCount
 * \return
 */
int FeatureContainer::getFeatureCount(){
    return FeatureContainer::myFeaturesList.size();
}

/*!
 * \brief FeatureContainer::getStationCount
 * \return
 */
int FeatureContainer::getStationCount(){
    return FeatureContainer::myStationsList.size();
}

/*!
 * \brief FeatureContainer::getCoordinateSystemCount
 * \return
 */
int FeatureContainer::getCoordinateSystemCount(){
    return FeatureContainer::myCoordinateSystemsList.size();
}

/*!
 * \brief FeatureContainer::getTransformationParameterCount
 * \return
 */
int FeatureContainer::getTransformationParameterCount(){
    return FeatureContainer::myTransformationParametersList.size();
}

/*!
 * \brief FeatureContainer::getGeometryCount
 * \return
 */
int FeatureContainer::getGeometryCount(){
    return FeatureContainer::myGeometriesList.size();
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
    FeatureContainer::myFeaturesList.append(myFeature);
    FeatureContainer::myFeaturesIdMap.insert(myFeature->getFeature()->getId(), myFeature);
    FeatureContainer::myFeaturesNameMap.insert(myFeature->getFeature()->getFeatureName(), myFeature);
    if(myFeature->getFeature()->getGroupName().compare("") != 0){
        FeatureContainer::myFeaturesGroupMap.insert(myFeature->getFeature()->getGroupName(), myFeature);
    }
    switch(myFeature->getTypeOfFeature()){
    case Configuration::eCoordinateSystemFeature:
        FeatureContainer::myCoordinateSystemsList.append(myFeature->getCoordinateSystem());
        break;
    case Configuration::eStationFeature:
        FeatureContainer::myStationsList.append(myFeature->getStation());
        break;
    case Configuration::eTrafoParamFeature:
        FeatureContainer::myTransformationParametersList.append(myFeature->getTrafoParam());
        break;
    default:
        FeatureContainer::myGeometriesList.append(myFeature);
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

    FeatureWrapper *myFeature = FeatureContainer::myFeaturesIdMap.value(featureId);
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
    if(!FeatureContainer::myFeaturesIdMap.contains(featureId)){
        return false;
    }

    FeatureWrapper *myFeature = FeatureContainer::myFeaturesIdMap.value(featureId);

    //check if the feature is valid
    if(myFeature == NULL || myFeature->getFeature() == NULL){
        return false;
    }

    //remove the feature from lists and map
    FeatureContainer::myFeaturesList.removeOne(myFeature);
    FeatureContainer::myFeaturesIdMap.remove(featureId);
    FeatureContainer::myFeaturesNameMap.remove(myFeature->getFeature()->getFeatureName(), myFeature);
    if(myFeature->getFeature()->getGroupName().compare("") != 0){
        FeatureContainer::myFeaturesGroupMap.remove(myFeature->getFeature()->getGroupName(), myFeature);
    }
    switch(myFeature->getTypeOfFeature()){
    case Configuration::eCoordinateSystemFeature:
        FeatureContainer::myCoordinateSystemsList.removeOne(myFeature->getCoordinateSystem());
        break;
    case Configuration::eStationFeature:
        FeatureContainer::myStationsList.removeOne(myFeature->getStation());
        break;
    case Configuration::eTrafoParamFeature:
        FeatureContainer::myTransformationParametersList.removeOne(myFeature->getTrafoParam());
        break;
    default:
        FeatureContainer::myGeometriesList.removeOne(myFeature);
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

    FeatureWrapper *myFeature = FeatureContainer::myFeaturesIdMap.value(featureId, NULL);

    //if the feature is not valid
    if(myFeature == NULL || myFeature->getFeature() == NULL){
        return false;
    }

    //remove and re-add the feature
    int numRemoved = FeatureContainer::myFeaturesNameMap.remove(oldName, myFeature);

    if(numRemoved == 1){
        FeatureContainer::myFeaturesNameMap.insert(myFeature->getFeature()->getFeatureName(), myFeature);
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

    FeatureWrapper *myFeature = FeatureContainer::myFeaturesIdMap.value(featureId, NULL);

    //if the feature is not valid
    if(myFeature == NULL || myFeature->getFeature() == NULL){
        return false;
    }

    //remove and re-add the feature
    if(oldGroup.compare("") != 0){
        FeatureContainer::myFeaturesGroupMap.remove(oldGroup, myFeature);
    }
    if(myFeature->getFeature()->getGroupName().compare("") != 0){
        FeatureContainer::myFeaturesGroupMap.insert(myFeature->getFeature()->getGroupName(), myFeature);
        return true;
    }
    return false;

}
