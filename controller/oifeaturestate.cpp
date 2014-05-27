#include "oifeaturestate.h"

QList<FeatureWrapper*> OiFeatureState::features;
FeatureWrapper *OiFeatureState::myActiveFeature = NULL;
QList<CoordinateSystem*> OiFeatureState::myCoordinateSystems;
QList<Station*> OiFeatureState::myStations;
Station *OiFeatureState::myActiveStation = NULL;
CoordinateSystem *OiFeatureState::myActiveCoordinateSystem = NULL;
QMap<QString, int> OiFeatureState::myAvailableGroups;


/*!
 * \brief OiFeatureState::isActiveFeature
 * Returns true if the given feature is the active feature
 * \param myFeature
 * \return
 */
bool OiFeatureState::isActiveFeature(const FeatureWrapper *myFeature){
    try{

        if(OiFeatureState::myActiveFeature != NULL && OiFeatureState::myActiveFeature->getFeature() != NULL &&
                myFeature != NULL && myFeature->getFeature() != NULL){

            //compare the id of the given feature with the id of the active feature
            if(myFeature->getFeature()->id == OiFeatureState::myActiveFeature->getFeature()->id){
                return true;
            }

        }
        return false;

    }catch(exception &e){
        Console::addLine(e.what());
        return false;
    }
}

/*!
 * \brief OiFeatureState::isActiveStation
 * Returns true if the given station is the active station
 * \param myStation
 * \return
 */
bool OiFeatureState::isActiveStation(const Station *myStation){
    try{

        if(OiFeatureState::myActiveStation != NULL && myStation != NULL){

            //compare the id of the given station with the id of the active station
            if(myStation->id == OiFeatureState::myActiveStation->id){
                return true;
            }

        }
        return false;

    }catch(exception &e){
        Console::addLine(e.what());
        return false;
    }
}

/*!
 * \brief OiFeatureState::isActiveCoordinateSystem
 * Returns true if the given coordinate system is the active coordinate system
 * \param myCoordinateSystem
 * \return
 */
bool OiFeatureState::isActiveCoordinateSystem(const CoordinateSystem *myCoordinateSystem){
    try{

        if(OiFeatureState::myActiveCoordinateSystem != NULL && myCoordinateSystem != NULL){

            //compare the id of the given coordinate system with the id of the active coordinate system
            if(myCoordinateSystem->id == OiFeatureState::myActiveCoordinateSystem->id){
                return true;
            }

        }
        return false;

    }catch(exception &e){
        Console::addLine(e.what());
        return false;
    }
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
QList<FeatureWrapper *> &OiFeatureState::getFeatures(){
    return OiFeatureState::myFeatures;
}

/*!
 * \brief OiFeatureState::getActiveFeature
 * Returns a pointer to the active feature's feature wrapper or NULL.
 * \return
 */
FeatureWrapper *OiFeatureState::getActiveFeature(){
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
Station *OiFeatureState::getActiveStation(){
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
CoordinateSystem *OiFeatureState::getActiveCoordinateSystem(){
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
 * \brief OiFeatureState::addFeature
 * Creates a new feature and adds it to the list of features. A pointer to that new feature is returned
 * \param myFeatureType
 * \return
 */
FeatureWrapper *OiFeatureState::addFeature(const Configuration::FeatureTypes featureType, const QString name){
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
        myFeature->getFeature()->name = name;

        //add the feature to the list of features
        OiFeatureState::myFeatures.append(myFeature);

        return myFeature;

    }catch(exception &e){
        Console::addLine(e.what());
        return NULL;
    }
}

/*!
 * \brief OiFeatureState::setActiveFeature
 * If there is a feature with the given id mark that feature as active feature and return true. Otherwise return false
 * \param id
 * \return
 */
bool OiFeatureState::setActiveFeature(const unsigned int id){
    try{

        //iterate through all available features...
        foreach(FeatureWrapper *myFeature, OiFeatureState::myFeatures){

            //...and if they are valid...
            if(myFeature != NULL && myFeature->getFeature() != NULL){

                //...and if their id equals the given id...
                if(myFeature->getFeature()->id == id){

                    //set them as the active feature
                    OiFeatureState::myActiveFeature = myFeature;
                    return true;

                }

            }

        }
        return false;

    }catch(exception &e){
        Console::addLine(e.what());
        return false;
    }
}

/*!
 * \brief OiFeatureState::setActiveStation
 * If there is a station with the given id mark that station as active station and return true. Otherwise return false
 * \param id
 * \return
 */
bool OiFeatureState::setActiveStation(const unsigned int id){
    try{

        //iterate through all available stations...
        foreach(Station *myStation, OiFeatureState::myStations){

            //...and if they are valid...
            if(myStation != NULL){

                //...and if their id equals the given id...
                if(myStation->id == id){

                    //set them as the active station
                    OiFeatureState::myActiveStation = myStation;
                    return true;

                }

            }

        }
        return false;

    }catch(exception &e){
        Console::addLine(e.what());
        return false;
    }
}

/*!
 * \brief OiFeatureState::setActiveCoordinateSystem
 * If there is a coordinate system with the given id mark that system as active system and return true. Otherwise return false
 * \param id
 * \return
 */
bool OiFeatureState::setActiveCoordinateSystem(const unsigned int id){
    try{

        //iterate through all available coordinate systems...
        foreach(CoordinateSystem *mySystem, OiFeatureState::myCoordinateSystems){

            //...and if they are valid...
            if(mySystem != NULL){

                //...and if their id equals the given id...
                if(mySystem->id == id){

                    //set them as the active coordinate system
                    OiFeatureState::myActiveCoordinateSystem = mySystem;
                    return true;

                }

            }

        }
        return false;

    }catch(exception &e){
        Console::addLine(e.what());
        return false;
    }
}
