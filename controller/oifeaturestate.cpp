#include "oifeaturestate.h"

OiFeatureState *OiFeatureState::myFeatureState = OiFeatureState::getInstance();

QList<FeatureWrapper*> OiFeatureState::myFeatures;
FeatureWrapper *OiFeatureState::myActiveFeature = NULL;
QList<CoordinateSystem*> OiFeatureState::myCoordinateSystems;
QList<Station*> OiFeatureState::myStations;
Station *OiFeatureState::myActiveStation = NULL;
CoordinateSystem *OiFeatureState::myActiveCoordinateSystem = NULL;
QMap<QString, int> OiFeatureState::myAvailableGroups;
bool OiFeatureState::groupsToBeUpdated = false;

OiFeatureState::OiFeatureState(QObject *parent) : QObject(parent){

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
    return OiFeatureState::myFeatures.size();
}

/*!
 * \brief OiFeatureState::getFeatures
 * Returns a constant reference to the list of features
 * \return
 */
const QList<FeatureWrapper *> &OiFeatureState::getFeatures(){
    return OiFeatureState::myFeatures;
}

/*!
 * \brief OiFeatureState::getStations
 * \return
 */
const QList<Station *> &OiFeatureState::getStations(){
    return OiFeatureState::myStations;
}

/*!
 * \brief OiFeatureState::getCoordinateSystems
 * \return
 */
const QList<CoordinateSystem *> &OiFeatureState::getCoordinateSystems(){
    return OiFeatureState::myCoordinateSystems;
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
 * \brief OiFeatureState::addFeature
 * Creates a new feature and adds it to the list of features. A pointer to that new feature is returned
 * \param myFeatureType
 * \return
 */
FeatureWrapper *OiFeatureState::addFeature(Configuration::FeatureTypes featureType, QString name){
    try{

        FeatureWrapper *myFeature = new FeatureWrapper();

        //create feature and assign it to feature wrapper
        switch(featureType){
        case Configuration::ePointFeature:{
            Point *myPoint = new Point();
            myFeature->setPoint(myPoint);
            break;
        }case Configuration::eLineFeature:{
            Line *myLine = new Line();
            myFeature->setLine(myLine);
            break;
        }case Configuration::ePlaneFeature:{
            Plane *myPlane = new Plane();
            myFeature->setPlane(myPlane);
            break;
        }case Configuration::eSphereFeature:{
            Sphere *mySphere = new Sphere();
            myFeature->setSphere(mySphere);
            break;
        }case Configuration::eStationFeature:{
            Station *myStation = new Station(name);
            myFeature->setStation(myStation);
            OiFeatureState::myStations.append(myStation);
            break;
        }case Configuration::eCoordinateSystemFeature:{
            CoordinateSystem *myCoordinateSystem = new CoordinateSystem();
            myFeature->setCoordinateSystem(myCoordinateSystem);
            OiFeatureState::myCoordinateSystems.append(myCoordinateSystem);
            break;
        }case Configuration::eTrafoParamFeature:{
            TrafoParam *myTrafoParam = new TrafoParam();
            myFeature->setTrafoParam(myTrafoParam);
            break;
        }case Configuration::eScalarentityAngleFeature:{
            ScalarEntityAngle *myAngle = new ScalarEntityAngle();
            myFeature->setScalarEntityAngle(myAngle);
            break;
        }case Configuration::eScalarEntityDistanceFeature:{
            ScalarEntityDistance *myDistance = new ScalarEntityDistance();
            myFeature->setScalarEntityDistance(myDistance);
            break;
        }case Configuration::eScalarEntityTemperatureFeature:{
            ScalarEntityTemperature *myTemperature = new ScalarEntityTemperature();
            myFeature->setScalarEntityTemperature(myTemperature);
            break;
        }default:{
            delete myFeature;
            return NULL;
        }}

        //set feature's name
        myFeature->getFeature()->setFeatureName(name);

        //add the feature to the list of features
        OiFeatureState::myFeatures.append(myFeature);

        //connect the feature's signals to slots in OiFeatureState
        OiFeatureState::connectFeature(myFeature);

        OiFeatureState::myFeatureState->emitSignal(eFeatureSetChanged);

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

        //TODO hier evtl den Namen des neuen Features prüfen
        //TODO alle anderen Listen füllen

        if(myFeature != NULL && myFeature->getFeature() != NULL && myFeature->getFeature()->getFeatureName().compare("") != 0){

            //add the feature to the list of features
            OiFeatureState::myFeatures.append(myFeature);

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
                OiFeatureState::myFeatureState->emitSignal(eAvailableGroupsChanged);
            }

            OiFeatureState::myFeatureState->emitSignal(eFeatureSetChanged);

            return true;

        }
        return false;

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

            int removeFeatureIndex = OiFeatureState::getFeatureListIndex(myFeature->getFeature()->getId());

            if(removeFeatureIndex >= 0){

                //disconnect feature's signals from OiFeatureState
                OiFeatureState::disconnectFeature(myFeature);

                //if needed set active pointers to NULL
                if(OiFeatureState::myActiveCoordinateSystem != NULL
                        && OiFeatureState::myActiveCoordinateSystem->getId() == myFeature->getFeature()->getId()){
                    OiFeatureState::myActiveCoordinateSystem = NULL;
                    OiFeatureState::myFeatureState->emitSignal(eActiveCoordinateSystemChanged);
                }
                if(OiFeatureState::myActiveStation != NULL
                        && OiFeatureState::myActiveStation->getId() == myFeature->getFeature()->getId()){
                    OiFeatureState::myActiveStation = NULL;
                    OiFeatureState::myFeatureState->emitSignal(eActiveStationChanged);
                }
                if(OiFeatureState::myActiveFeature != NULL && OiFeatureState::myActiveFeature->getFeature() != NULL
                        && OiFeatureState::myActiveFeature->getFeature()->getId() == myFeature->getFeature()->getId()){
                    OiFeatureState::myActiveFeature = NULL;
                    OiFeatureState::myFeatureState->emitSignal(eActiveFeatureChanged);
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
                        OiFeatureState::myFeatureState->emitSignal(eAvailableGroupsChanged);
                    }
                }

                //remove feature from list of features
                OiFeatureState::myFeatures.removeAt(removeFeatureIndex);
                if(myFeature->getStation() != NULL){
                    int removeStationIndex = OiFeatureState::getStationListIndex(myFeature->getFeature()->getId());
                    if(removeStationIndex >= 0){
                        OiFeatureState::myStations.removeAt(removeStationIndex);
                    }
                }else if(myFeature->getCoordinateSystem() != NULL){
                    int removeCoordinateSystemIndex = OiFeatureState::getCoordinateSystemIndex(myFeature->getFeature()->getId());
                    if(removeCoordinateSystemIndex >= 0){
                        OiFeatureState::myCoordinateSystems.removeAt(removeCoordinateSystemIndex);
                    }
                }

                //delete the feature
                delete myFeature->getFeature();
                delete myFeature;

                OiFeatureState::myFeatureState->emitSignal(eFeatureSetChanged);

                return true;

            }
            return false;

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
        foreach(FeatureWrapper *myFeature, OiFeatureState::myFeatures){

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
                OiFeatureState::myFeatureState, SLOT(setActiveFeature(int)));
        connect(myFeature->getFeature(), SIGNAL(featureGroupChanged(int)),
                OiFeatureState::myFeatureState, SLOT(setFeatureGroups(int)));

        //station specific connects
        if(myFeature->getStation() != NULL){

            connect(myFeature->getStation(), SIGNAL(activeStationChanged(int)),
                    OiFeatureState::myFeatureState, SLOT(setActiveStation(int)));

        }

        //coordinate system specific connects
        if(myFeature->getCoordinateSystem() != NULL){

            connect(myFeature->getCoordinateSystem(), SIGNAL(activeCoordinateSystemChanged(int)),
                    OiFeatureState::myFeatureState, SLOT(setActiveCoordinateSystem(int)));

        }

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
            OiFeatureState::myFeatureState, SLOT(setActiveFeature(int)));
    disconnect(myFeature->getFeature(), SIGNAL(featureGroupChanged(int)),
            OiFeatureState::myFeatureState, SLOT(setFeatureGroups(int)));
}

/*!
 * \brief OiFeatureState::setActiveFeature
 * \param featureId
 */
void OiFeatureState::setActiveFeature(int featureId){
    try{

        int featureIndex = OiFeatureState::getFeatureListIndex(featureId);
        if(featureIndex >= 0){

            if(OiFeatureState::myFeatures.at(featureIndex)->getFeature()->getIsActiveFeature()){

                //set the feature as active feature
                OiFeatureState::myActiveFeature = OiFeatureState::myFeatures.at(featureIndex);

                //set isActive of active feature to true and all other feature's isActive property to false
                for(unsigned int i = 0; i < OiFeatureState::getFeatureCount(); i++){
                    if(i != featureIndex && OiFeatureState::myFeatures.at(i) != NULL
                            && OiFeatureState::myFeatures.at(i)->getFeature() != NULL){

                        OiFeatureState::myFeatures.at(i)->getFeature()->setActiveFeatureState(false);

                    }else{

                        OiFeatureState::myFeatures.at(i)->getFeature()->setActiveFeatureState(true);

                    }
                }

                OiFeatureState::myFeatureState->emitSignal(eActiveFeatureChanged);

            }

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

        int stationIndex = OiFeatureState::getStationListIndex(featureId);
        if(stationIndex >= 0){

            if(OiFeatureState::myStations.at(stationIndex)->getIsActiveStation()){

                //set the station as active station
                OiFeatureState::myActiveStation = OiFeatureState::myStations.at(stationIndex);

                //set isActiveStation of active station to true and all other station's isActiveStation property to false
                for(unsigned int i = 0; i < OiFeatureState::myStations.size(); i++){
                    if(i != stationIndex && OiFeatureState::myStations.at(i) != NULL){

                        OiFeatureState::myStations.at(i)->setActiveStationState(false);

                    }else{

                        OiFeatureState::myStations.at(i)->setActiveStationState(true);

                    }
                }

                OiFeatureState::myFeatureState->emitSignal(eActiveStationChanged);

            }

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

        int csIndex = OiFeatureState::getCoordinateSystemIndex(featureId);
        if(csIndex >= 0){

            if(OiFeatureState::myCoordinateSystems.at(csIndex)->getIsActiveCoordinateSystem()){

                //set the coordinate system as active coordinate system
                OiFeatureState::myActiveCoordinateSystem = OiFeatureState::myCoordinateSystems.at(csIndex);

                //set isActiveCoordinateSystem of active system to true and all other system's isActiveCoordinateSystem property to false
                for(unsigned int i = 0; i < OiFeatureState::myCoordinateSystems.size(); i++){
                    if(i != csIndex && OiFeatureState::myCoordinateSystems.at(i) != NULL){

                        OiFeatureState::myCoordinateSystems.at(i)->setActiveCoordinateSystemState(false);

                    }else{

                        OiFeatureState::myCoordinateSystems.at(i)->setActiveCoordinateSystemState(true);

                    }
                }

                OiFeatureState::myFeatureState->emitSignal(eActiveCoordinateSystemChanged);

            }

        }

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::setFeatureGroups
 * \param featureId
 */
void OiFeatureState::setFeatureGroups(int featureId){
    try{

        int featureIndex = OiFeatureState::getFeatureListIndex(featureId);
        if(featureIndex >= 0){

            OiFeatureState::updateAvailableGroups();

            OiFeatureState::myFeatureState->emitSignal(eAvailableGroupsChanged);

        }

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::getFeatureListIndex
 * \param featureId
 * \return
 */
int OiFeatureState::getFeatureListIndex(int featureId){
    for(unsigned int i = 0; i < OiFeatureState::getFeatureCount(); i++){
        FeatureWrapper *myFeature = OiFeatureState::myFeatures.at(i);
        if(myFeature != NULL && myFeature->getFeature() != NULL && myFeature->getFeature()->getId() == featureId){
            return i;
        }
    }
    return -1;
}

/*!
 * \brief OiFeatureState::getStationListIndex
 * \param featureId
 * \return
 */
int OiFeatureState::getStationListIndex(int featureId){
    for(unsigned int i = 0; i < OiFeatureState::myStations.size(); i++){
        Station *myStation = OiFeatureState::myStations.at(i);
        if(myStation != NULL && myStation->getId() == featureId){
            return i;
        }
    }
    return -1;
}

/*!
 * \brief OiFeatureState::getCoordinateSystemIndex
 * \param featureId
 * \return
 */
int OiFeatureState::getCoordinateSystemIndex(int featureId){
    for(unsigned int i = 0; i < OiFeatureState::myCoordinateSystems.size(); i++){
        CoordinateSystem *mySystem = OiFeatureState::myCoordinateSystems.at(i);
        if(mySystem != NULL && mySystem->getId() == featureId){
            return i;
        }
    }
    return -1;
}
