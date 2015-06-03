#include "featurecontainer.h"
/*
FeatureContainer::FeatureContainer(){

}

FeatureContainer::~FeatureContainer(){

}

const QList<QPointer<FeatureWrapper> > &FeatureContainer::getFeaturesList(){
    return this->featuresList;
}

QList<CoordinateSystem *> &FeatureContainer::getCoordinateSystemsList(){
    return this->coordSystems;
}

QList<Station *> &FeatureContainer::getStationsList(){
    return this->stationsList;
}

QList<TrafoParam *> &FeatureContainer::getTransformationParametersList(){
    return this->trafoParamsList;
}

QList<FeatureWrapper *> &FeatureContainer::getGeometriesList(){
    return this->geometriesList;
}

QList<int> FeatureContainer::getFeatureIdList(){
    return this->featuresIdMap.keys();
}

QStringList FeatureContainer::getFeatureNameList(){
    return this->featuresNameMap.keys();
}

QStringList FeatureContainer::getFeatureGroupList(){
    return this->featuresGroupMap.keys();
}

QPointer<FeatureWrapper> FeatureContainer::getFeatureById(const int &featureId){
    return this->featuresIdMap.value(featureId, NULL);
}

QList<QPointer<FeatureWrapper> > FeatureContainer::getFeaturesByName(const QString &name){
    return this->featuresNameMap.values(name);
}

QList<QPointer<FeatureWrapper> > FeatureContainer::getFeaturesByGroup(const QString &group){
    return this->featuresGroupMap.values(group);
}

QList<QPointer<FeatureWrapper> > FeatureContainer::getFeaturesByType(const FeatureTypes &type){
    return this->featuresTypeMap.values(type);
}

int FeatureContainer::getFeatureCount(){
    return this->featuresList.size();
}

int FeatureContainer::getStationCount(){
    return this->stationsList.size();
}

int FeatureContainer::getCoordinateSystemCount(){
    return this->coordSystems.size();
}

int FeatureContainer::getTransformationParameterCount(){
    return this->trafoParamsList.size();
}

int FeatureContainer::getGeometryCount(){
    return this->geometriesList.size();
}

bool FeatureContainer::addFeature(const QPointer<FeatureWrapper> &myFeature){

    //if the feature is not valid
    if(myFeature == NULL || myFeature->getFeature() == NULL){
        return false;
    }

    //if the feature already exists it is not added
    if(FeatureContainer::featuresIdMap.contains(myFeature->getFeature()->getId())){
        return false;
    }

    //add the feature to the feature lists and maps
    this->featuresList.append(myFeature);
    this->featuresIdMap.insert(myFeature->getFeature()->getId(), myFeature);
    this->featuresNameMap.insert(myFeature->getFeature()->getFeatureName(), myFeature);
    if(myFeature->getFeature()->getGroupName().compare("") != 0){
        this->featuresGroupMap.insert(myFeature->getFeature()->getGroupName(), myFeature);
    }
    switch(myFeature->getFeatureTypeEnum()){
    case eCoordinateSystemFeature:
        this->coordSystems.append(myFeature->getCoordinateSystem());
        break;
    case eStationFeature:
        this->stationsList.append(myFeature->getStation());
        break;
    case eTrafoParamFeature:
        this->trafoParamsList.append(myFeature->getTrafoParam());
        break;
    default:
        this->geometriesList.append(myFeature);
        break;
    }

    return true;

}

bool FeatureContainer::removeFeature(const int &featureId){

    FeatureWrapper *myFeature = this->featuresIdMap.value(featureId);
    if(FeatureContainer::removeFeature(featureId)){
        delete myFeature->getFeature();
        delete myFeature;
        return true;
    }
    return false;

}


bool FeatureContainer::removeFeature(const int &featureId){

    //check if the feature exists
    if(!this->featuresIdMap.contains(featureId)){
        return false;
    }

    FeatureWrapper *myFeature = this->featuresIdMap.value(featureId);

    //check if the feature is valid
    if(myFeature == NULL || myFeature->getFeature() == NULL){
        return false;
    }

    //remove the feature from lists and map
    this->featuresList.removeOne(myFeature);
    this->featuresIdMap.remove(featureId);
    this->featuresNameMap.remove(myFeature->getFeature()->getFeatureName(), myFeature);
    if(myFeature->getFeature()->getGroupName().compare("") != 0){
        this->featuresGroupMap.remove(myFeature->getFeature()->getGroupName(), myFeature);
    }
    switch(myFeature->getFeatureTypeEnum()){
    case eCoordinateSystemFeature:
        this->coordSystems.removeOne(myFeature->getCoordinateSystem());
        break;
    case eStationFeature:
        this->stationsList.removeOne(myFeature->getStation());
        break;
    case eTrafoParamFeature:
        this->trafoParamsList.removeOne(myFeature->getTrafoParam());
        break;
    default:
        this->geometriesList.removeOne(myFeature);
        break;
    }

    return true;

}

bool FeatureContainer::featureNameChanged(const int &featureId, const QString &oldName){

    FeatureWrapper *myFeature = this->featuresIdMap.value(featureId, NULL);

    //if the feature is not valid
    if(myFeature == NULL || myFeature->getFeature() == NULL){
        return false;
    }

    //remove and re-add the feature
    int numRemoved = this->featuresNameMap.remove(oldName, myFeature);

    if(numRemoved == 1){
        this->featuresNameMap.insert(myFeature->getFeature()->getFeatureName(), myFeature);
        return true;
    }
    return false;

}

bool FeatureContainer::featureGroupChanged(const int &featureId, const QString &oldGroup){

    FeatureWrapper *myFeature = this->featuresIdMap.value(featureId, NULL);

    //if the feature is not valid
    if(myFeature == NULL || myFeature->getFeature() == NULL){
        return false;
    }

    //remove and re-add the feature
    if(oldGroup.compare("") != 0){
        this->featuresGroupMap.remove(oldGroup, myFeature);
    }
    if(myFeature->getFeature()->getGroupName().compare("") != 0){
        this->featuresGroupMap.insert(myFeature->getFeature()->getGroupName(), myFeature);
        return true;
    }
    return false;

}
*/

/*!
 * \brief FeatureContainer::FeatureContainer
 */
FeatureContainer::FeatureContainer(){

}

/*!
 * \brief FeatureContainer::~FeatureContainer
 */
FeatureContainer::~FeatureContainer(){

}

/*!
 * \brief FeatureContainer::getFeaturesList
 * \return
 */
const QList<QPointer<FeatureWrapper> > &FeatureContainer::getFeaturesList() const{
    return this->featuresList;
}

/*!
 * \brief FeatureContainer::getCoordinateSystemsList
 * \return
 */
const QList<QPointer<CoordinateSystem> > &FeatureContainer::getCoordinateSystemsList() const{
    return this->coordSystems;
}

/*!
 * \brief FeatureContainer::getStationSystemsList
 * \return
 */
QList<QPointer<CoordinateSystem> > FeatureContainer::getStationSystemsList() const{
    QList<QPointer<CoordinateSystem> > stationSystems;
    foreach(const QPointer<Station> &station, this->stationsList){
        if(!station.isNull() && !station->getCoordinateSystem().isNull()){
            stationSystems.append(station->getCoordinateSystem());
        }
    }
    return stationSystems;
}

/*!
 * \brief FeatureContainer::getStationsList
 * \return
 */
const QList<QPointer<Station> > &FeatureContainer::getStationsList() const{
    return this->stationsList;
}

/*!
 * \brief FeatureContainer::getTransformationParametersList
 * \return
 */
const QList<QPointer<TrafoParam> > &FeatureContainer::getTransformationParametersList() const{
    return this->trafoParamsList;
}

/*!
 * \brief FeatureContainer::getGeometriesList
 * \return
 */
const QList<QPointer<FeatureWrapper> > &FeatureContainer::getGeometriesList() const{
    return this->geometriesList;
}

/*!
 * \brief FeatureContainer::getFeatureIdList
 * \return
 */
const QList<int> &FeatureContainer::getFeatureIdList() const{
    return this->featureIds;
}

/*!
 * \brief FeatureContainer::getFeatureNameList
 * \return
 */
const QStringList &FeatureContainer::getFeatureNameList() const{
    return this->featureNames;
}

/*!
 * \brief FeatureContainer::getFeatureGroupList
 * \return
 */
const QStringList &FeatureContainer::getFeatureGroupList() const{
    return this->featureGroups;
}

/*!
 * \brief FeatureContainer::getFeatureById
 * \param featureId
 * \return
 */
QPointer<FeatureWrapper> FeatureContainer::getFeatureById(const int &featureId) const{
    return this->featuresIdMap.value(featureId, QPointer<FeatureWrapper>(NULL));
}

/*!
 * \brief FeatureContainer::getFeaturesByName
 * \param name
 * \return
 */
QList<QPointer<FeatureWrapper> > FeatureContainer::getFeaturesByName(const QString &name) const{
    return this->featuresNameMap.values(name);
}

/*!
 * \brief FeatureContainer::getFeaturesByGroup
 * \param group
 * \return
 */
QList<QPointer<FeatureWrapper> > FeatureContainer::getFeaturesByGroup(const QString &group) const{
    return this->featuresGroupMap.values(group);
}

/*!
 * \brief FeatureContainer::getFeaturesByType
 * \param type
 * \return
 */
QList<QPointer<FeatureWrapper> > FeatureContainer::getFeaturesByType(const FeatureTypes &type) const{
    return this->featuresTypeMap.values(type);
}

/*!
 * \brief FeatureContainer::getFeatureCount
 * \return
 */
int FeatureContainer::getFeatureCount() const{
    return this->featuresList.size();
}

/*!
 * \brief FeatureContainer::getGeometryCount
 * \return
 */
int FeatureContainer::getGeometryCount() const{
    return this->geometriesList.size();
}

/*!
 * \brief FeatureContainer::getFeatureCount
 * \param type
 * \return
 */
int FeatureContainer::getFeatureCount(const FeatureTypes &type) const{
    return this->featuresTypeMap.count(type);
}

/*!
 * \brief FeatureContainer::addFeature
 * \param feature
 * \return
 */
bool FeatureContainer::addFeature(const QPointer<FeatureWrapper> &feature){

    //if the feature is not valid
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //if the feature already exists it is not added
    if(this->featuresIdMap.contains(feature->getFeature()->getId())){
        return false;
    }

    //add the feature to the feature lists and maps
    this->featuresList.append(feature);
    this->featuresIdMap.insert(feature->getFeature()->getId(), feature);
    this->featuresNameMap.insert(feature->getFeature()->getFeatureName(), feature);
    this->featuresTypeMap.insert(feature->getFeatureTypeEnum(), feature);
    if(feature->getFeature()->getGroupName().compare("") != 0){
        this->featuresGroupMap.insert(feature->getFeature()->getGroupName(), feature);
    }
    switch(feature->getFeatureTypeEnum()){
    case eCoordinateSystemFeature:
        this->coordSystems.append(feature->getCoordinateSystem());
        break;
    case eStationFeature:
        this->stationsList.append(feature->getStation());
        if(!feature->getStation()->getCoordinateSystem().isNull()){
            QPointer<FeatureWrapper> stationSystem = new FeatureWrapper();
            stationSystem->setCoordinateSystem(feature->getStation()->getCoordinateSystem());
            this->featuresIdMap.insert(feature->getStation()->getCoordinateSystem()->getId(), stationSystem);
        }
        break;
    case eTrafoParamFeature:
        this->trafoParamsList.append(feature->getTrafoParam());
        break;
    default:
        this->geometriesList.append(feature);
        break;
    }

    //update lists with ids, names and groups
    this->featureIds.append(feature->getFeature()->getId());
    if(this->featuresNameMap.values(feature->getFeature()->getFeatureName()).size() == 1){
        this->featureNames.append(feature->getFeature()->getFeatureName());
    }
    if(feature->getFeature()->getGroupName().compare("") != 0 &&
            !this->featureGroups.contains(feature->getFeature()->getGroupName())){
        this->featureGroups.append(feature->getFeature()->getGroupName());
    }

    return true;

}

/*!
 * \brief FeatureContainer::removeFeature
 * \param featureId
 * \return
 */
bool FeatureContainer::removeFeature(const int &featureId){

    //check if the feature exists
    if(!this->featuresIdMap.contains(featureId)){
        return false;
    }

    //get the feature pointer
    QPointer<FeatureWrapper> feature = this->featuresIdMap.value(featureId, QPointer<FeatureWrapper>());

    //check if the feature is valid
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //remove the feature from lists and map
    this->featuresList.removeOne(feature);
    this->featuresIdMap.remove(featureId);
    this->featuresNameMap.remove(feature->getFeature()->getFeatureName(), feature);
    this->featuresTypeMap.remove(feature->getFeatureTypeEnum(), feature);
    if(feature->getFeature()->getGroupName().compare("") != 0){
        this->featuresGroupMap.remove(feature->getFeature()->getGroupName(), feature);
    }
    switch(feature->getFeatureTypeEnum()){
    case eCoordinateSystemFeature:
        this->coordSystems.removeOne(feature->getCoordinateSystem());
        break;
    case eStationFeature:
        this->stationsList.removeOne(feature->getStation());
        break;
    case eTrafoParamFeature:
        this->trafoParamsList.removeOne(feature->getTrafoParam());
        break;
    default:
        this->geometriesList.removeOne(feature);
        break;
    }

    //update lists with ids, names and groups
    this->featureIds.removeOne(feature->getFeature()->getId());
    if(!this->featuresNameMap.contains(feature->getFeature()->getFeatureName())){
        this->featureNames.removeOne(feature->getFeature()->getFeatureName());
    }
    if(feature->getFeature()->getGroupName().compare("") != 0 &&
            !this->featuresGroupMap.contains(feature->getFeature()->getGroupName())){
        this->featureGroups.removeOne(feature->getFeature()->getGroupName());
    }

    //delete the feature
    delete feature->getFeature();
    delete feature;

    return true;

}

/*!
 * \brief FeatureContainer::checkAndClean
 * Checks wether there is a feature with the given id that was deleted but not removed from lists
 * \param featureId
 * \param name
 * \param group
 * \param type
 */
void FeatureContainer::checkAndClean(const int &featureId, const QString &name, const QString &group, const FeatureTypes &type){

    if(!this->featuresIdMap.contains(featureId)){
        return;
    }

    //clean feature list
    QList<int> deletion;
    int index = 0;
    foreach(const QPointer<FeatureWrapper> &feature, this->featuresList){
        if(feature.isNull() || feature->getFeature().isNull()){
            //delete feature wrapper
            if(!feature.isNull()){
                delete feature;
            }
            deletion.append(index);
        }
        index++;
    }
    for(int i = deletion.size(); i > 0; i--){
        this->featuresList.removeAt(i-1);
    }

    //clean coord system list
    deletion.clear();
    index = 0;
    foreach(const QPointer<CoordinateSystem> &coordSystem, this->coordSystems){
        if(coordSystem.isNull()){
            deletion.append(index);
        }
        index++;
    }
    for(int i = deletion.size(); i > 0; i--){
        this->coordSystems.removeAt(i-1);
    }

    //clean station list
    deletion.clear();
    index = 0;
    foreach(const QPointer<Station> &station, this->stationsList){
        if(station.isNull()){
            deletion.append(index);
        }
        index++;
    }
    for(int i = deletion.size(); i > 0; i--){
        this->stationsList.removeAt(i-1);
    }

    //clean trafo param list
    deletion.clear();
    index = 0;
    foreach(const QPointer<TrafoParam> &trafoParam, this->trafoParamsList){
        if(trafoParam.isNull()){
            deletion.append(index);
        }
        index++;
    }
    for(int i = deletion.size(); i > 0; i--){
        this->trafoParamsList.removeAt(i-1);
    }

    //clean geometry list
    deletion.clear();
    index = 0;
    foreach(const QPointer<FeatureWrapper> &geometry, this->geometriesList){
        if(geometry.isNull() || geometry->getFeature().isNull()){
            //delete feature wrapper
            if(!geometry.isNull()){
                delete geometry;
            }
            deletion.append(index);
        }
        index++;
    }
    for(int i = deletion.size(); i > 0; i--){
        this->geometriesList.removeAt(i-1);
    }

    //clean maps
    bool removeAttribute = true;
    this->featuresIdMap.remove(featureId);
    this->featureIds.removeOne(featureId);
    if(this->featuresNameMap.contains(name)){
        QList<QPointer<FeatureWrapper> > features = this->featuresNameMap.values(name);
        this->featuresNameMap.remove(name);
        foreach(const QPointer<FeatureWrapper> &feature, features){
            if(!feature.isNull() && feature->getFeature().isNull()){
                this->featuresNameMap.insert(name, feature);
                removeAttribute = false;
            }
        }
        if(removeAttribute){
            this->featureNames.removeOne(name);
        }else{
            removeAttribute = true;
        }
    }
    if(group.compare("") != 0 && this->featuresGroupMap.contains(group)){
        QList<QPointer<FeatureWrapper> > features = this->featuresGroupMap.values(group);
        this->featuresGroupMap.remove(group);
        foreach(const QPointer<FeatureWrapper> &feature, features){
            if(!feature.isNull() && feature->getFeature().isNull()){
                this->featuresGroupMap.insert(group, feature);
                removeAttribute = false;
            }
        }
        if(removeAttribute){
            this->featureGroups.removeOne(group);
        }
    }
    if(this->featuresTypeMap.contains(type)){
        QList<QPointer<FeatureWrapper> > features = this->featuresTypeMap.values(type);
        this->featuresTypeMap.remove(type);
        foreach(const QPointer<FeatureWrapper> &feature, features){
            if(!feature.isNull() && feature->getFeature().isNull()){
                this->featuresTypeMap.insert(type, feature);
                removeAttribute = false;
            }
        }
    }

}

/*!
 * \brief FeatureContainer::removeAll
 * Remove and delete all features
 */
void FeatureContainer::removeAll(){

    //delete all features
    foreach(const QPointer<FeatureWrapper> &feature, this->featuresList){
        if(!feature.isNull()){
            if(feature->getFeature().isNull()){
                delete feature->getFeature();
            }
            delete feature;
        }
    }

    //clear all lists and maps
    this->featuresList.clear();
    this->coordSystems.clear();
    this->stationsList.clear();
    this->trafoParamsList.clear();
    this->geometriesList.clear();
    this->featuresIdMap.clear();
    this->featuresNameMap.clear();
    this->featuresGroupMap.clear();
    this->featuresTypeMap.clear();
    this->featureIds.clear();
    this->featureNames.clear();
    this->featureGroups.clear();

}

/*!
 * \brief FeatureContainer::featureNameChanged
 * Is called whenever the name of a feature has changed
 * \param featureId
 * \param oldName
 * \return
 */
bool FeatureContainer::featureNameChanged(const int &featureId, const QString &oldName){

    //check if the feature exists
    if(!this->featuresIdMap.contains(featureId)){
        return false;
    }

    //get the feature pointer
    QPointer<FeatureWrapper> feature = this->featuresIdMap.value(featureId, QPointer<FeatureWrapper>());

    //check if the feature is valid
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //update lists and maps
    if(this->featuresNameMap.contains(oldName)){
        this->featuresNameMap.insert(feature->getFeature()->getFeatureName(), this->featuresNameMap.take(oldName));
    }
    if(!this->featuresNameMap.contains(oldName)){
        this->featureNames.removeOne(oldName);
    }
    if(!this->featureNames.contains(feature->getFeature()->getFeatureName())){
        this->featureNames.append(feature->getFeature()->getFeatureName());
    }

    return true;

}

/*!
 * \brief FeatureContainer::featureGroupChanged
 * Is called whenever the group of a feature has changed
 * \param featureId
 * \param oldGroup
 * \return
 */
bool FeatureContainer::featureGroupChanged(const int &featureId, const QString &oldGroup){

    //check if the feature exists
    if(!this->featuresIdMap.contains(featureId)){
        return false;
    }

    //get the feature pointer
    QPointer<FeatureWrapper> feature = this->featuresIdMap.value(featureId, QPointer<FeatureWrapper>());

    //check if the feature is valid
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //check if the old group was empty
    if(oldGroup.compare("") == 0){

        //check if the new group is empty
        if(feature->getFeature()->getGroupName().compare("") != 0){
            this->featuresGroupMap.insert(feature->getFeature()->getGroupName(), feature);
            if(!this->featureGroups.contains(feature->getFeature()->getGroupName())){
                this->featureGroups.append(feature->getFeature()->getGroupName());
            }
        }

    }else{

        //check if the new group is empty
        if(feature->getFeature()->getGroupName().compare("") != 0){

            if(this->featuresGroupMap.contains(oldGroup)){
                this->featuresGroupMap.insert(feature->getFeature()->getGroupName(), this->featuresGroupMap.take(oldGroup));
            }
            if(!this->featuresGroupMap.contains(oldGroup)){
                this->featureGroups.removeOne(oldGroup);
            }

        }else{

            this->featuresGroupMap.remove(oldGroup, feature);
            if(!this->featuresGroupMap.contains(oldGroup)){
                this->featureGroups.removeOne(oldGroup);
            }

        }

    }

    return true;

}
