#include "oifeaturestate.h"

OiFeatureState *OiFeatureState::myFeatureState = NULL;

FeatureWrapper *OiFeatureState::myActiveFeature = NULL;
Station *OiFeatureState::myActiveStation = NULL;
CoordinateSystem *OiFeatureState::myActiveCoordinateSystem = NULL;

QMap<QString, int> OiFeatureState::myAvailableGroups;
bool OiFeatureState::groupsToBeUpdated = false;
QString OiFeatureState::myActiveGroup = "";

FeatureContainer OiFeatureState::myFeatureContainer;

OiFeatureState::OiFeatureState(QObject *parent) : QObject(parent){
}

/* \brief sortID
* \param f1
* \param f2
* \return
* comperator function for sorting FeatureWrapper* by id
*/
bool sortID(FeatureWrapper *f1, FeatureWrapper *f2){
   return f1->getFeature()->getId() < f2->getFeature()->getId();
}

/*!
 * \brief OiFeatureState::getInstance
 * Returns a new or the previously created instance of this class
 * \return
 */
OiFeatureState *OiFeatureState::getInstance(){
    if(OiFeatureState::myFeatureState == NULL){
        OiFeatureState::myFeatureState = new OiFeatureState();
    }
    return OiFeatureState::myFeatureState;
}

/*!
 * \brief OiFeatureState::getFeatureCount
 * Returns the number of features
 * \return
 */
int OiFeatureState::getFeatureCount(){
    return OiFeatureState::myFeatureContainer.getFeatureCount();
}

/*!
 * \brief OiFeatureState::getFeatures
 * Returns a constant reference to the list of features
 * \return
 */
const QList<FeatureWrapper *> &OiFeatureState::getFeatures(){
    return OiFeatureState::myFeatureContainer.getFeaturesList();
}

/*!
 * \brief OiFeatureState::getStations
 * \return
 */
const QList<Station *> &OiFeatureState::getStations(){
    return OiFeatureState::myFeatureContainer.getStationsList();
}

/*!
 * \brief OiFeatureState::getCoordinateSystems
 * \return
 */
const QList<CoordinateSystem *> &OiFeatureState::getCoordinateSystems(){
    return OiFeatureState::myFeatureContainer.getCoordinateSystemsList();
}

/*!
 * \brief OiFeatureState::getTransformationParameters
 * \return
 */
const QList<TrafoParam *> &OiFeatureState::getTransformationParameters(){
    return OiFeatureState::myFeatureContainer.getTransformationParametersList();
}

/*!
 * \brief OiFeatureState::getGeometries
 * \return
 */
const QList<FeatureWrapper *> &OiFeatureState::getGeometries(){
    return OiFeatureState::myFeatureContainer.getGeometriesList();
}

/*!
 * \brief OiFeatureState::getFeaturesOfGroup
 * Get all features of a special group
 * \param group
 * \return
 */
QList<FeatureWrapper *> OiFeatureState::getFeaturesOfGroup(QString group){

    QList<FeatureWrapper *> features;

    foreach(FeatureWrapper *fw, OiFeatureState::myFeatureContainer.getFeaturesList()){
        if(fw != NULL && fw->getFeature() != NULL && fw->getFeature()->getGroupName().compare(group) == 0){
            features.append(fw);
        }
    }

    return features;

}

/*!
 * \brief OiFeatureState::getFeaturesWithName
 * \param name
 * \return
 */
QList<FeatureWrapper *> OiFeatureState::getFeaturesByName(QString name){
    return OiFeatureState::myFeatureContainer.getFeaturesByName(name);
}

/*!
 * \brief OiFeatureState::getActiveFeature
 * Returns a pointer to the active feature's feature wrapper or NULL.
 * \return
 */
FeatureWrapper* OiFeatureState::getActiveFeature(){
    try{

        if(OiFeatureState::myActiveFeature != NULL && OiFeatureState::myActiveFeature->getFeature() != NULL){
            return OiFeatureState::myActiveFeature;
        }
        return NULL;

    }catch(exception &e){
        Console::addLine(e.what());
        return NULL;
    }
}

/*!
 * \brief OiFeatureState::getActiveStation
 * Returns a pointer to the active station or NULL.
 * \return
 */
Station* OiFeatureState::getActiveStation(){
    try{

        if(OiFeatureState::myActiveStation != NULL){
            return OiFeatureState::myActiveStation;
        }
        return NULL;

    }catch(exception &e){
        Console::addLine(e.what());
        return NULL;
    }
}

/*!
 * \brief OiFeatureState::getActiveCoordinateSystem
 * Returns a pointer to the active coordinate system or NULL.
 * \return
 */
CoordinateSystem* OiFeatureState::getActiveCoordinateSystem(){
    try{

        if(OiFeatureState::myActiveCoordinateSystem != NULL){
            return OiFeatureState::myActiveCoordinateSystem;
        }
        return NULL;

    }catch(exception &e){
        Console::addLine(e.what());
        return NULL;
    }
}

/*!
 * \brief OiFeatureState::getAvailableGroups
 * \return
 */
const QMap<QString, int> OiFeatureState::getAvailableGroups(){
    return OiFeatureState::myAvailableGroups;
}

/*!
 * \brief OiFeatureState::getActiveGroup
 * \return
 */
QString OiFeatureState::getActiveGroup(){
    return OiFeatureState::myActiveGroup;
}

/*!
 * \brief OiFeatureState::setActiveGroup
 */
void OiFeatureState::setActiveGroup(QString group){
    if(group.compare("") != 0){
        OiFeatureState::myActiveGroup = group;
    }
}

/*!
 * \brief OiFeatureState::getFeature
 * \param featureId
 * \return
 */
FeatureWrapper *OiFeatureState::getFeature(int featureId){
    return OiFeatureState::myFeatureContainer.getFeatureById(featureId);
}

/*!
 * \brief OiFeatureState::getNominalSystem
 * \param name
 * \return
 */
CoordinateSystem *OiFeatureState::getNominalSystem(QString name){
    foreach(CoordinateSystem *c, OiFeatureState::myFeatureContainer.getCoordinateSystemsList()){
        if(c != NULL && c->getFeatureName().compare(name) == 0){
            return c;
        }
    }
    return NULL;
}

/*!
 * \brief OiFeatureState::sortFeatures
 */
void OiFeatureState::sortFeatures()
{
    /*for(int i=0; i<OiFeatureState::getFeatureCount();i++){
        if(OiFeatureState::getFeatures().at(i)->getGeometry() != NULL && OiFeatureState::getFeatures().at(i)->getGeometry()->getIsNominal() == false){
            for(int k=0;k<OiFeatureState::getFeatures().size();k++){
                if(OiFeatureState::getFeatures().at(k)->getGeometry() != NULL && OiFeatureState::getFeatures().at(k)->getGeometry()->getIsNominal() == true){
                    if(OiFeatureState::getFeatures().at(i)->getFeature()->getFeatureName().compare(OiFeatureState::getFeatures().at(k)->getFeature()->getFeatureName(),Qt::CaseSensitive)==0){
                        if(!(i ==k-1)){
                            OiFeatureState::myFeatureContainer.getFeaturesList().insert(i+1,OiFeatureState::myFeatureContainer.getFeaturesList().at(k));
                            if(i<k){
                                OiFeatureState::myFeatureContainer.getFeaturesList().removeAt(k+1);
                            }else{
                                i -= 1;
                                OiFeatureState::myFeatureContainer.getFeaturesList().removeAt(k);
                            }
                            k -= 1;
                        }
                    }
                }
            }
        }else if(OiFeatureState::myFeatureContainer.getFeaturesList().at(i)->getStation() != NULL){
            for(int j=0;j<OiFeatureState::myFeatureContainer.getFeaturesList().size();j++){
                if(OiFeatureState::myFeatureContainer.getFeaturesList().at(j)->getPoint() != NULL
                        && OiFeatureState::myFeatureContainer.getFeaturesList().at(j)->getPoint()->getIsNominal() == true){
                    if(OiFeatureState::myFeatureContainer.getFeaturesList().at(i)->getStation()->getFeatureName().compare(OiFeatureState::myFeatureContainer.getFeaturesList().at(j)->getPoint()->getFeatureName(),Qt::CaseSensitive)==0){
                        if(!(i ==j-1)){
                            OiFeatureState::myFeatureContainer.getFeaturesList().insert(i+1,OiFeatureState::myFeatureContainer.getFeaturesList().at(j));
                            if(i<j){
                                OiFeatureState::myFeatureContainer.getFeaturesList().removeAt(j+1);
                            }else{
                                i -= 1;
                                OiFeatureState::myFeatureContainer.getFeaturesList().removeAt(j);
                            }
                            j -= 1;
                        }
                    }
                }
            }
        }
    }*/
}

void OiFeatureState::sortFeaturesById()
{
    //qSort(myFeatures.begin(), myFeatures.end(), sortID);
}

/*!
 * \brief OiFeatureState::resetFeatureLists
 * Delete all OpenIndy features and clear all feature lists
 */
void OiFeatureState::resetFeatureLists(){

    //reset active states
    myActiveFeature = NULL;
    myActiveStation = NULL;
    myActiveCoordinateSystem = NULL;
    myActiveGroup = "All Groups";

    //get a list of feature-ids and delete them
    QList<int> featureIds = OiFeatureState::myFeatureContainer.getFeatureIdList();
    foreach(int id, featureIds){
        OiFeatureState::myFeatureContainer.removeAndDeleteFeature(id);
    }

    OiFeatureState::getInstance()->emitSignal(OiFeatureState::eFeatureSetChanged);

}

/*!
 * \brief OiFeatureState::addFeature
 * Creates a new feature and adds it to the list of features. A pointer to that new feature is returned
 * \param myFeatureType
 * \return
 */
FeatureWrapper *OiFeatureState::addFeature(FeatureAttributesExchange attributes){
    try{

        FeatureWrapper *myFeature;
        myFeature = new FeatureWrapper();

        //create feature and assign it to feature wrapper
        switch(attributes.featureType){
        case Configuration::ePointFeature:{
            Point *myPoint = new Point(attributes.nominal);
            myFeature->setPoint(myPoint);
            break;
        }case Configuration::eLineFeature:{
            Line *myLine = new Line(attributes.nominal);
            myFeature->setLine(myLine);
            break;
        }case Configuration::ePlaneFeature:{
            Plane *myPlane = new Plane(attributes.nominal);
            myFeature->setPlane(myPlane);
            break;
        }case Configuration::eSphereFeature:{
            Sphere *mySphere = new Sphere(attributes.nominal);
            myFeature->setSphere(mySphere);
            break;
        }case Configuration::eStationFeature:{
            Station *myStation = new Station(attributes.name);
            myFeature->setStation(myStation);
            break;
        }case Configuration::eCoordinateSystemFeature:{
            CoordinateSystem *myCoordinateSystem = new CoordinateSystem();
            myFeature->setCoordinateSystem(myCoordinateSystem);
            break;
        }case Configuration::eTrafoParamFeature:{
            TrafoParam *myTrafoParam = new TrafoParam();
            myFeature->setTrafoParam(myTrafoParam);
            break;
        }case Configuration::eScalarEntityAngleFeature:{
            ScalarEntityAngle *myAngle = new ScalarEntityAngle(attributes.nominal);
            myFeature->setScalarEntityAngle(myAngle);
            break;
        }case Configuration::eScalarEntityDistanceFeature:{
            ScalarEntityDistance *myDistance = new ScalarEntityDistance(attributes.nominal);
            myFeature->setScalarEntityDistance(myDistance);
            break;
        }case Configuration::eScalarEntityTemperatureFeature:{
            ScalarEntityTemperature *myTemperature = new ScalarEntityTemperature(attributes.nominal);
            myFeature->setScalarEntityTemperature(myTemperature);
            break;
        }default:{
            delete myFeature;
            return NULL;
        }}

        //set feature's name
        if(OiFeatureState::validateFeatureName(attributes.featureType, attributes.name, attributes.nominal)){
            myFeature->getFeature()->setFeatureName(attributes.name);
        }else{
            while(!OiFeatureState::validateFeatureName(attributes.featureType, attributes.name.append("_new"), attributes.nominal)){}
            myFeature->getFeature()->setFeatureName(attributes.name);
        }

        //store the feature
        OiFeatureState::myFeatureContainer.addFeature(myFeature);

        //connect the feature's signals to slots in OiFeatureState
        OiFeatureState::connectFeature(myFeature);

        OiFeatureState::getInstance()->emitSignal(eFeatureSetChanged);

        return myFeature;

    }catch(exception &e){
        Console::addLine(e.what());
        return NULL;
    }
}

/*!
 * \brief OiFeatureState::addFeature
 * Adds a feature to the list of features
 * \param feature
 * \return
 */
bool OiFeatureState::addFeature(FeatureWrapper *myFeature){
    try{

        if(myFeature != NULL && myFeature->getFeature() != NULL && myFeature->getFeature()->getFeatureName().compare("") != 0){

            //check if feature with this id already exists
            if(OiFeatureState::myFeatureContainer.getFeatureById(myFeature->getFeature()->getId()) != NULL){
                myFeature->getFeature()->setId(Configuration::generateID());
            }

            //check feature's name
            bool nameValid = false;
            if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getIsNominal()){
                nameValid = OiFeatureState::validateFeatureName(myFeature->getTypeOfFeature(), myFeature->getFeature()->getFeatureName(), true, myFeature->getGeometry()->getNominalSystem());
            }else{
                nameValid = OiFeatureState::validateFeatureName(myFeature->getTypeOfFeature(), myFeature->getFeature()->getFeatureName());
            }
            if(!nameValid){
                QString name = myFeature->getFeature()->getFeatureName();
                if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getIsNominal()){
                    while(!OiFeatureState::validateFeatureName(myFeature->getTypeOfFeature(), name.append("_new"), true, myFeature->getGeometry()->getNominalSystem())){}
                }else{
                    while(!OiFeatureState::validateFeatureName(myFeature->getTypeOfFeature(), name.append("_new"))){}
                }
                myFeature->getFeature()->setFeatureName(name);
            }

            //add the feature to the list of features, stations, coordinate systems, trafo params and geometries
            OiFeatureState::myFeatureContainer.addFeature(myFeature);

            //add nominal to nominal list of coordinate system
            if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getNominalSystem() != NULL){
                myFeature->getGeometry()->getNominalSystem()->addNominal(myFeature);
            }

            //connect the feature's signals to slots in OiFeatureState
            OiFeatureState::connectFeature(myFeature);

            //if a group is set for the new feature emit the group changed signal
            if(myFeature->getFeature()->getGroupName().compare("") != 0){
                QString group = myFeature->getFeature()->getGroupName();
                if(OiFeatureState::myAvailableGroups.contains(group)){
                    OiFeatureState::myAvailableGroups.insert(group, 1);
                }else{
                    int count = OiFeatureState::myAvailableGroups.find(group).value();
                    OiFeatureState::myAvailableGroups.insert(group, count+1);
                }
                OiFeatureState::getInstance()->emitSignal(eAvailableGroupsChanged);
            }

            OiFeatureState::getInstance()->emitSignal(eFeatureSetChanged);

            return true;

        }
        return false;

    }catch(exception &e){
        Console::addLine(e.what());
        return false;
    }
}

/*!
 * \brief OiFeatureState::addFeatures
 * Creates and adds features by using the given feature attributes
 * \param attributes
 * \return
 */
QList<FeatureWrapper *> OiFeatureState::addFeatures(FeatureAttributesExchange attributes){
    try{

        QString name = attributes.name;
        QString index = 0;

        for(int i = 0; i < attributes.count; i++){



        }

    }catch(exception &e){
        Console::addLine(e.what());
        return QList<FeatureWrapper *>();
    }
}

/*!
 * \brief OiFeatureState::addFeatures
 * \param myFeatures
 * \return
 */
bool OiFeatureState::addFeatures(const QList<FeatureWrapper *> &myFeatures){
    try{

        foreach(FeatureWrapper *myFeature, myFeatures){

            if(myFeature != NULL && myFeature->getFeature() != NULL && myFeature->getFeature()->getFeatureName().compare("") != 0){

                //check if feature with this id already exists
                if(OiFeatureState::myFeatureContainer.getFeatureById(myFeature->getFeature()->getId()) != NULL){
                    myFeature->getFeature()->setId(Configuration::generateID());
                }

                //check feature's name
                bool nameValid = false;
                if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getIsNominal()){
                    nameValid = OiFeatureState::validateFeatureName(myFeature->getTypeOfFeature(), myFeature->getFeature()->getFeatureName(), true, myFeature->getGeometry()->getNominalSystem());
                }else{
                    nameValid = OiFeatureState::validateFeatureName(myFeature->getTypeOfFeature(), myFeature->getFeature()->getFeatureName());
                }
                if(!nameValid){
                    QString name = myFeature->getFeature()->getFeatureName();
                    if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getIsNominal()){
                        while(!OiFeatureState::validateFeatureName(myFeature->getTypeOfFeature(), name.append("_new"), true, myFeature->getGeometry()->getNominalSystem())){}
                    }else{
                        while(!OiFeatureState::validateFeatureName(myFeature->getTypeOfFeature(), name.append("_new"))){}
                    }
                    myFeature->getFeature()->setFeatureName(name);
                }

                //add the feature to the list of features, stations, coordinate systems, trafo params and geometries
                OiFeatureState::myFeatureContainer.addFeature(myFeature);

                //add nominal to nominal list of coordinate system
                if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getNominalSystem() != NULL){
                    myFeature->getGeometry()->getNominalSystem()->addNominal(myFeature);
                }

                //connect the feature's signals to slots in OiFeatureState
                OiFeatureState::connectFeature(myFeature);

                //if a group is set for the new feature emit the group changed signal
                if(myFeature->getFeature()->getGroupName().compare("") != 0){
                    QString group = myFeature->getFeature()->getGroupName();
                    if(OiFeatureState::myAvailableGroups.contains(group)){
                        OiFeatureState::myAvailableGroups.insert(group, 1);
                    }else{
                        int count = OiFeatureState::myAvailableGroups.find(group).value();
                        OiFeatureState::myAvailableGroups.insert(group, count+1);
                    }
                    OiFeatureState::getInstance()->emitSignal(eAvailableGroupsChanged);
                }

            }

        }

        OiFeatureState::getInstance()->emitSignal(eFeatureSetChanged);

        return true;

    }catch(exception &e){
        Console::addLine(e.what());
        return false;
    }
}

/*!
 * \brief OiFeatureState::removeFeature
 * \param feature
 * \return
 */
bool OiFeatureState::removeFeature(FeatureWrapper *myFeature){
    try{

        if(myFeature != NULL && myFeature->getFeature() != NULL){

            //int removeFeatureIndex = OiFeatureState::getFeatureListIndex(myFeature->getFeature()->getId());

            //if(removeFeatureIndex >= 0){

                //disconnect feature's signals from OiFeatureState
                OiFeatureState::disconnectFeature(myFeature);

                //if needed set active pointers to NULL
                if(OiFeatureState::myActiveCoordinateSystem != NULL
                        && OiFeatureState::myActiveCoordinateSystem->getId() == myFeature->getFeature()->getId()){
                    OiFeatureState::myActiveCoordinateSystem = NULL;
                    OiFeatureState::getInstance()->emitSignal(eActiveCoordinateSystemChanged);
                }
                if(OiFeatureState::myActiveStation != NULL
                        && OiFeatureState::myActiveStation->getId() == myFeature->getFeature()->getId()){
                    OiFeatureState::myActiveStation = NULL;
                    OiFeatureState::getInstance()->emitSignal(eActiveStationChanged);
                }
                if(OiFeatureState::myActiveFeature != NULL && OiFeatureState::myActiveFeature->getFeature() != NULL
                        && OiFeatureState::myActiveFeature->getFeature()->getId() == myFeature->getFeature()->getId()){
                    OiFeatureState::myActiveFeature = NULL;
                    OiFeatureState::getInstance()->emitSignal(eActiveFeatureChanged);
                }

                //remove group from groups map if needed
                if(myFeature->getFeature()->getGroupName().compare("") != 0){
                    QString group = myFeature->getFeature()->getGroupName();
                    if(OiFeatureState::myAvailableGroups.contains(group)){
                        int count = OiFeatureState::myAvailableGroups.find(group).value();
                        if(count == 1){
                            OiFeatureState::myAvailableGroups.remove(group);
                        }else{
                            OiFeatureState::myAvailableGroups.insert(group, count-1);
                        }
                        OiFeatureState::getInstance()->emitSignal(eAvailableGroupsChanged);
                    }
                }

                //remove feature from list of features and delete it
                OiFeatureState::myFeatureContainer.removeAndDeleteFeature(myFeature->getFeature()->getId());

                OiFeatureState::getInstance()->emitSignal(eFeatureSetChanged);

                return true;

            //}
            //return false;

        }
        return false;

    }catch(exception &e){
        Console::addLine(e.what());
        return false;
    }
}

/*!
 * \brief OiFeatureState::emitSignal
 * Emits the given signal
 * \param mySignalType
 */
void OiFeatureState::emitSignal(OiFeatureState::SignalType mySignalType){
    switch(mySignalType){
    case eActiveCoordinateSystemChanged:
        emit this->activeCoordinateSystemChanged();
        break;
    case eActiveFeatureChanged:
        emit this->activeFeatureChanged();
        break;
    case eActiveStationChanged:
        emit this->activeStationChanged();
        break;
    case eAvailableGroupsChanged:
        emit this->availableGroupsChanged();
        break;
    case eFeatureSetChanged:
        emit this->featureSetChanged();
        break;
    case eCoordSysSetChanged:
        emit this->coordSystemSetChanged();
        break;
    case eFeatureAttributesChanged:
        emit this->featureAttributesChanged();
        break;
    case eGeomObservationsChanged:
        emit this->geometryObservationsChanged();
        break;
    }
}

/*!
 * \brief OiFeatureState::updateAvailableGroups
 * Refill groups map
 */
void OiFeatureState::updateAvailableGroups(){
    try{

        //clear groups map
        OiFeatureState::myAvailableGroups.clear();

        //iterate through all features and add their group names
        foreach(FeatureWrapper *myFeature, OiFeatureState::myFeatureContainer.getFeaturesList()){

            if(myFeature != NULL && myFeature->getFeature() != NULL && myFeature->getFeature()->getGroupName().compare("") != 0){

                QString groupName = myFeature->getFeature()->getGroupName();
                if(OiFeatureState::myAvailableGroups.contains(groupName)){
                    int count = OiFeatureState::myAvailableGroups.find(groupName).value();
                    OiFeatureState::myAvailableGroups.insert(groupName, count+1);
                }else{
                    OiFeatureState::myAvailableGroups.insert(groupName, 1);
                }

            }

        }

        //check if active group is still available
        QStringList myGroups = OiFeatureState::myAvailableGroups.keys();
        if(!myGroups.contains(OiFeatureState::myActiveGroup)){
            OiFeatureState::myActiveGroup = "All Groups";
        }

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::connectFeature
 * \param myFeature
 */
void OiFeatureState::connectFeature(FeatureWrapper *myFeature){
    try{

        //general feature connects
        connect(myFeature->getFeature(), SIGNAL(featureIsActiveChanged(int)),
                OiFeatureState::getInstance(), SLOT(setActiveFeature(int)), Qt::DirectConnection);
        connect(myFeature->getFeature(), SIGNAL(featureGroupChanged(int)),
                OiFeatureState::getInstance(), SLOT(setFeatureGroup(int)), Qt::DirectConnection);
        connect(myFeature->getFeature(), SIGNAL(featureNameChanged(int,QString)),
                OiFeatureState::getInstance(), SLOT(setFeatureName(int,QString)), Qt::DirectConnection);
        connect(myFeature->getFeature(), SIGNAL(featureAboutToBeDeleted(int)),
                OiFeatureState::getInstance(), SLOT(removeFeature(int)), Qt::DirectConnection);

        //geometry specific connects
        if(myFeature->getGeometry() != NULL){

            connect(myFeature->getGeometry(), SIGNAL(geomMyObservationsChanged(int)),
                    OiFeatureState::getInstance(), SLOT(setGeometryObservations(int)), Qt::DirectConnection);
            connect(myFeature->getGeometry(), SIGNAL(geomMyActualChanged(int)),
                    OiFeatureState::getInstance(), SLOT(setGeometryActual(int)), Qt::DirectConnection);

            //only for point clouds
            if(myFeature->getPointCloud() != NULL){
                connect(myFeature->getPointCloud(), SIGNAL(pcSegmentAdded(FeatureWrapper*)),
                        OiFeatureState::getInstance(), SLOT(addPCSegmentAsFeature(FeatureWrapper*)), Qt::DirectConnection);
            }

        }

        //station specific connects
        if(myFeature->getStation() != NULL){

            connect(myFeature->getStation(), SIGNAL(activeStationChanged(int)),
                    OiFeatureState::getInstance(), SLOT(setActiveStation(int)), Qt::DirectConnection);
            connect(myFeature->getStation()->coordSys, SIGNAL(activeCoordinateSystemChanged(int)),
                    OiFeatureState::getInstance(), SLOT(setActiveCoordinateSystem(int)), Qt::DirectConnection);

        }

        //coordinate system specific connects
        if(myFeature->getCoordinateSystem() != NULL){

            connect(myFeature->getCoordinateSystem(), SIGNAL(activeCoordinateSystemChanged(int)),
                    OiFeatureState::getInstance(), SLOT(setActiveCoordinateSystem(int)), Qt::DirectConnection);
            connect(myFeature->getCoordinateSystem(), SIGNAL(nominalsChanged(int)),
                    OiFeatureState::getInstance(), SLOT(setSystemsNominals(int)), Qt::DirectConnection);

        }

        //call OiConfigState's connect method
        OiConfigState::connectFeature(myFeature);

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::disconnectFeature
 * \param myFeature
 */
void OiFeatureState::disconnectFeature(FeatureWrapper *myFeature){
    disconnect(myFeature->getFeature(), SIGNAL(featureIsActiveChanged(int)),
            OiFeatureState::getInstance(), SLOT(setActiveFeature(int)));
    disconnect(myFeature->getFeature(), SIGNAL(featureGroupChanged(int)),
               OiFeatureState::getInstance(), SLOT(setFeatureGroup(int)));

    //call OiConfigState's disconnect method
    OiConfigState::disconnectFeature(myFeature);
}

/*!
 * \brief OiFeatureState::validateFeatureName
 * \param featureType the type of the feature that shall be added
 * \param featureName the name of the feature that shall be added
 * \param isNominal true if a nominal shall be added, false if not
 * \param myNomSys a pointer to the nominal system of the feature (only if isNominal = true)
 * \return
 */
bool OiFeatureState::validateFeatureName(Configuration::FeatureTypes featureType, QString featureName, bool isNominal, CoordinateSystem *myNomSys){
    try{

        //do not accept empty names
        if(featureName.compare("") == 0){
            return false;
        }

        //get a list of all features with name featureName
        QList<FeatureWrapper *> equalNameFeatures = OiFeatureState::myFeatureContainer.getFeaturesByName(featureName);

        //accept featureName if no other feature with the same name exists
        if(equalNameFeatures.size() == 0){
            return true;
        }

        if(isNominal){

            //reject featureName if there is another nominal with the same type, name and nominal system or
            //if there is a station, coordinate system or trafo param feature with the same name
            foreach(FeatureWrapper *myFeature, equalNameFeatures){

                if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getIsNominal()
                        && myFeature->getGeometry()->getNominalSystem() == myNomSys
                        && myFeature->getTypeOfFeature() == featureType){
                    return false;
                }else if(myFeature->getTypeOfFeature() == Configuration::eCoordinateSystemFeature
                         || myFeature->getTypeOfFeature() == Configuration::eTrafoParamFeature
                         || myFeature->getTypeOfFeature() == Configuration::eStationFeature){
                    return false;
                }

            }

        }else{

            //reject featureName if a station, coordinate system or trafo param feature shall be added and its name is already used
            if(featureType == Configuration::eCoordinateSystemFeature || featureType == Configuration::eStationFeature
                    || featureType == Configuration::eTrafoParamFeature){
                return false;
            }

            //reject featureName if an actual geometry shall be added with the same name and type
            foreach(FeatureWrapper *myFeature, equalNameFeatures){
                if(myFeature->getGeometry() != NULL && !myFeature->getGeometry()->getIsNominal()
                        && myFeature->getTypeOfFeature() == featureType){
                    return false;
                }
            }

        }

        return true;

    }catch(exception &e){
        Console::addLine(e.what());
        return false;
    }
}

/*!
 * \brief OiFeatureState::setActiveFeature
 * \param featureId
 */
void OiFeatureState::setActiveFeature(int featureId){
    try{

        //get the feature with id featureId
        FeatureWrapper *myFeature = OiFeatureState::myFeatureContainer.getFeatureById(featureId);

        //if no feature exists with that id
        if(myFeature == NULL){
            return;
        }

        //if the feature was set to be the active feature
        if(myFeature->getFeature()->getIsActiveFeature()){

            //set the active feature pointer to that feature ...
            OiFeatureState::myActiveFeature = myFeature;

            //... and set isActive of active feature to true and all other feature's isActive property to false
            foreach(FeatureWrapper *f, OiFeatureState::myFeatureContainer.getFeaturesList()){
                if(f->getFeature()->getId() == featureId){
                    f->getFeature()->setActiveFeatureState(true);
                }else{
                    f->getFeature()->setActiveFeatureState(false);
                }
            }

            //emit signal to inform that active feature has changed
            OiFeatureState::getInstance()->emitSignal(eActiveFeatureChanged);

        }

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::setActiveStation
 * \param featureId
 */
void OiFeatureState::setActiveStation(int featureId){
    try{

        //get the station with id featureId
        FeatureWrapper *myStation = OiFeatureState::myFeatureContainer.getFeatureById(featureId);

        //if no station exists with that id
        if(myStation == NULL || myStation->getStation() == NULL){
            return;
        }

        //if the station was set to be the active station
        if(myStation->getStation()->getIsActiveStation()){

            //set the active station pointer to that station ...
            OiFeatureState::myActiveStation = myStation->getStation();

            //... and set isActiveStation of active station to true and all other station's isActiveStation property to false
            foreach(Station *s, OiFeatureState::myFeatureContainer.getStationsList()){
                if(s->getId() == featureId){
                    s->setActiveStationState(true);
                }else{
                    s->setActiveStationState(false);
                }
            }

            //emit signal to inform that active station has changed
            OiFeatureState::getInstance()->emitSignal(eActiveStationChanged);

        }

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::setActiveCoordinateSystem
 * \param featureId
 */
void OiFeatureState::setActiveCoordinateSystem(int featureId){
    try{

        //get the coordinate system with id featureId
        CoordinateSystem *mySystem = NULL;
        FeatureWrapper *myCoordinateSystem = OiFeatureState::myFeatureContainer.getFeatureById(featureId);

        if(myCoordinateSystem != NULL){
            mySystem = myCoordinateSystem->getCoordinateSystem();
        }

        //if no coordinate system with that id exists
        if(myCoordinateSystem == NULL || myCoordinateSystem->getCoordinateSystem() == NULL){

            //check if there is a station system with id featureId
            foreach(Station *s, OiFeatureState::myFeatureContainer.getStationsList()){
                if(s->coordSys->getId() == featureId){
                    mySystem = s->coordSys;
                    break;
                }
            }

        }

        if(mySystem == NULL){
            return;
        }

        //if the coordinate system was set to be the active coordinate system
        if(mySystem->getIsActiveCoordinateSystem()){

            //set the active coordinate system pointer to that coordinate system
            OiFeatureState::myActiveCoordinateSystem = mySystem;

            //set isActiveCoordinateSystem of active system to true and all other system's isActiveCoordinateSystem property to false
            foreach(CoordinateSystem *c, OiFeatureState::myFeatureContainer.getCoordinateSystemsList()){
                if(c->getId() == featureId){
                    c->setActiveCoordinateSystemState(true);
                }else{
                    c->setActiveCoordinateSystemState(false);
                }
            }
            foreach(Station *s, OiFeatureState::myFeatureContainer.getStationsList()){
                if(s->coordSys->getId() == featureId){
                    s->coordSys->setActiveCoordinateSystemState(true);
                }else{
                    s->coordSys->setActiveCoordinateSystemState(false);
                }
            }

            //emit signal to inform that active coordinate system has changed
            OiFeatureState::getInstance()->emitSignal(eActiveCoordinateSystemChanged);

        }

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::setFeatureGroups
 * \param featureId
 */
void OiFeatureState::setFeatureGroup(int featureId){
    try{

        OiFeatureState::updateAvailableGroups();

        OiFeatureState::getInstance()->emitSignal(eAvailableGroupsChanged);

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::setFeatureName
 * \param featureId
 * \param oldName
 */
void OiFeatureState::setFeatureName(int featureId, QString oldName){
    try{

        OiFeatureState::getInstance()->emitSignal(eFeatureAttributesChanged);

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::setFeatureComment
 * \param featureId
 */
void OiFeatureState::setFeatureComment(int featureId){
    try{

        OiFeatureState::getInstance()->emitSignal(eFeatureAttributesChanged);

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::setFeatureFunctions
 * \param featureId
 */
void OiFeatureState::setFeatureFunctions(int featureId){
    try{

        OiFeatureState::getInstance()->emitSignal(eFeatureFunctionsChanged);

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::setGeometryActual
 * \param featureId
 */
void OiFeatureState::setGeometryActual(int featureId){
    try{

        FeatureWrapper *myNominal = OiFeatureState::getFeature(featureId);

        if(myNominal == NULL || myNominal->getGeometry() == NULL){
            return;
        }


        myNominal->getGeometry()->getMyActual();
        FeatureWrapper *newActual;
        newActual= new FeatureWrapper();

        /*switch (myNominal->getTypeOfFeature()) {
        case Configuration::ePointFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eLineFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::ePlaneFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eSphereFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eStationFeature:

            break;
        case Configuration::eCoordinateSystemFeature:

            break;
        case Configuration::eTrafoParamFeature:

            break;
        case Configuration::ePointCloudFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eCircleFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eConeFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eCylinderFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eEllipsoidFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eHyperboloidFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eNurbsFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eParaboloidFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eScalarEntityAngleFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eScalarEntityDistanceFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eScalarEntityTemperatureFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eScalarEntityMeasurementSeriesFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        default:
            break;
        }*/

        OiFeatureState::getInstance()->emitSignal(eFeatureSetChanged);

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::setGeometryNominals
 * \param featureId
 */
void OiFeatureState::setGeometryNominals(int featureId){
    try{

        OiFeatureState::getInstance()->emitSignal(eFeatureSetChanged);

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::setGeometryObservations
 * \param featureId
 */
void OiFeatureState::setGeometryObservations(int featureId){
    try{

        OiFeatureState::getInstance()->emitSignal(eGeomObservationsChanged);

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::setSystemsNominals
 * \param featureId
 */
void OiFeatureState::setSystemsNominals(int featureId){
    try{

        /*int featureIndex = OiFeatureState::getFeatureListIndex(featureId);
        if(featureIndex >= 0){

            //check if the added nominal already exists in OpenIndy
            FeatureWrapper *nominalSystem = OiFeatureState::myFeatures.at(featureIndex);
            FeatureWrapper *myNominal = nominalSystem->getCoordinateSystem()->getNominals().last();
            int nominalIndex = OiFeatureState::getFeatureListIndex(myNominal->getGeometry()->getId());
            if(nominalIndex >= 0){
                OiFeatureState::getInstance()->emitSignal(eSystemNominalsChanged);
            }else{
                myNominal->getGeometry()->setId(Configuration::generateID());
                OiFeatureState::addFeature(myNominal);
                OiFeatureState::getInstance()->emitSignal(eSystemNominalsChanged);
            }

        }*/

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::removeFeature
 * Remove a feature from lists and map
 * \param featureId
 */
void OiFeatureState::removeFeature(int featureId){
    OiFeatureState::myFeatureContainer.removeFeature(featureId);
}

/*!
 * \brief OiFeatureState::addPCSegmentAsFeature
 * Add a segment, detected from a point cloud to the list of features in OpenIndy
 * \param segment
 */
void OiFeatureState::addPCSegmentAsFeature(FeatureWrapper *segment){

    qDebug() << "pc segment as feature featurestate";

    OiFeatureState::addFeature(segment);

}

/*!
 * \brief OiFeatureState::getFeatureListIndex
 * \param featureId
 * \return
 */
/*int OiFeatureState::getFeatureListIndex(int featureId){
    for(unsigned int i = 0; i < OiFeatureState::getFeatureCount(); i++){
        FeatureWrapper *myFeature = OiFeatureState::myFeatureContainer.getFeaturesList().at(i);
        if(myFeature != NULL && myFeature->getFeature() != NULL && myFeature->getFeature()->getId() == featureId){
            return i;
        }
    }
    return -1;
}*/

/*!
 * \brief OiFeatureState::getStationListIndex
 * \param featureId
 * \return
 */
/*int OiFeatureState::getStationListIndex(int featureId){
    for(unsigned int i = 0; i < OiFeatureState::myFeatureContainer.getStationsList().size(); i++){
        Station *myStation = OiFeatureState::myFeatureContainer.getStationsList().at(i);
        if(myStation != NULL && myStation->getId() == featureId){
            return i;
        }
    }
    return -1;
}*/

/*!
 * \brief OiFeatureState::getCoordinateSystemIndex
 * \param featureId
 * \return
 */
/*int OiFeatureState::getCoordinateSystemIndex(int featureId){
    for(unsigned int i = 0; i < OiFeatureState::myFeatureContainer.getCoordinateSystemsList().size(); i++){
        CoordinateSystem *mySystem = OiFeatureState::myFeatureContainer.getCoordinateSystemsList().at(i);
        if(mySystem != NULL && mySystem->getId() == featureId){
            return i;
        }
    }
    return -1;
}*/

/*!
 * \brief OiFeatureState::getStationCoordinayteSystemIndex
 * \param featureId
 * \return
 */
/*int OiFeatureState::getStationCoordinayteSystemIndex(int featureId){
    for(unsigned int i = 0; i < OiFeatureState::myFeatureContainer.getStationsList().size(); i++){
        Station *myStation = OiFeatureState::myFeatureContainer.getStationsList().at(i);
        if(myStation != NULL && myStation->coordSys != NULL && myStation->coordSys->getId() == featureId){
            return i;
        }
    }
    return -1;
}*/
