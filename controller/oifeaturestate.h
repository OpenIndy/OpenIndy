#ifndef OIFEATURESTATE_H
#define OIFEATURESTATE_H

#include <QObject>
#include <exception>
#include <QMap>
#include <QMultiMap>

#include "featurewrapper.h"
#include "feature.h"
#include "geometry.h"
#include "station.h"
#include "coordinatesystem.h"
#include "console.h"
#include "configuration.h"
#include "oiconfigstate.h"
#include "oijob.h"
#include "oijobstate.h"
#include "featureattributesexchange.h"

using namespace std;

struct FeatureContainer;

/*!
 * \brief The OiFeatureState class
 * A static manager class that holds all features and works as an observer that informs about feature changes
 */
class OiFeatureState : public QObject
{
    Q_OBJECT

private:
    explicit OiFeatureState(QObject *parent = 0);

public:
    static OiFeatureState *getInstance();

    static int getFeatureCount();

    static const QList<FeatureWrapper *> &getFeatures();
    static FeatureWrapper *getFeature(int featureId);

    static FeatureWrapper *addFeature(FeatureAttributesExchange attributes);
    static bool addFeature(FeatureWrapper *myFeature);
    static QList<FeatureWrapper *> addFeatures(FeatureAttributesExchange attributes);
    static bool addFeatures(const QList<FeatureWrapper *> &myFeatures);

    static bool removeFeature(FeatureWrapper *myFeature);

    static const QList<Station *> &getStations();
    static const QList<CoordinateSystem *> &getCoordinateSystems();
    static const QList<TrafoParam *> &getTransformationParameters();
    static const QList<FeatureWrapper *> &getGeometries();
    static QList<FeatureWrapper *> getFeaturesOfGroup(QString group);

    static FeatureWrapper* getActiveFeature();
    static Station* getActiveStation();
    static CoordinateSystem* getActiveCoordinateSystem();

    static const QMap<QString, int> getAvailableGroups();

    static QString getActiveGroup();
    static void setActiveGroup(QString group);

    static CoordinateSystem *getNominalSystem(QString name);

    static void sortFeatures();
    static void sortFeaturesById();
    static void resetFeatureLists();

signals:
    void activeFeatureChanged(); //emitted when active feature has changed
    void activeStationChanged(); //emitted when active station has changed
    void activeCoordinateSystemChanged(); //emitted when display coordinate system has changed

    void featureSetChanged(); //emitted when features were created or deleted
    void coordSystemSetChanged(); //emitted when coordinate systems (nominal or station) where created or deleted

    void availableGroupsChanged(); //emitted when the set of groups which features belong to has changed

    void featureAttributesChanged(); //emitted when a feature's attributes like name, comment etc. have changed

    void featureFunctionsChanged(); //emitted when a features's functions have changed

    void geometryObservationsChanged(); //emitted when observations were added or removed

    void systemNominalsChanged(); //emitted when nominals where added or removed from a coordinate system

private slots:
    void setActiveFeature(int featureId); //is called when a feature becomes the active feature
    void setActiveStation(int featureId); //is called when a station becomes the active station
    void setActiveCoordinateSystem(int featureId); //is called when a coordinate system becomes the display system

    void setFeatureGroup(int featureId); //is called when the group attribute of a feature is changed
    void setFeatureName(int featureId, QString oldName); //is called when the name of a feature is changed
    void setFeatureComment(int featureId); //is called when the comment of a feature is changed
    void setFeatureFunctions(int featureId); //is called when the list of functions of a feature was edited

    void setGeometryActual(int featureId); //is called when the actual geometry of a nominal is set
    void setGeometryNominals(int featureId); //is called when a nominal was added or removed from an actual geometry
    void setGeometryObservations(int featureId); //is called when an observations was added or removed from a geometry

    void setSystemsNominals(int featureId); //is called when a nominal was added to a nominal coordinate system

    void removeFeature(int featureId); //is called when the destructor of a feature is entered

    void addPCSegmentAsFeature(FeatureWrapper *segment);

private:
    static OiFeatureState *myFeatureState;

    static FeatureContainer myFeatureContainer; //container that holds all features as lists and maps

    static FeatureWrapper *myActiveFeature; //the currently selected feature in table view
    static Station *myActiveStation; //the currently selected station
    static CoordinateSystem *myActiveCoordinateSystem; //the currently selected display coordinate system

    static QMap<QString, int> myAvailableGroups; //map of all available groups with number of assigned features
    static QString myActiveGroup; //currently selected feature group

    static bool groupsToBeUpdated;

    /*static int getFeatureListIndex(int featureId);
    static int getStationListIndex(int featureId);
    static int getCoordinateSystemIndex(int featureId);
    static int getStationCoordinayteSystemIndex(int featureId);*/
    static void updateAvailableGroups();

    static void connectFeature(FeatureWrapper *myFeature);
    static void disconnectFeature(FeatureWrapper *myFeature);

    static bool validateFeatureName(Configuration::FeatureTypes featureType, QString featureName,
                                    bool isNominal = false, CoordinateSystem *myNomSys = NULL);

    enum SignalType{
        eActiveFeatureChanged,
        eActiveStationChanged,
        eActiveCoordinateSystemChanged,
        eFeatureSetChanged,
        eCoordSysSetChanged,
        eAvailableGroupsChanged,
        eFeatureAttributesChanged,
        eFeatureFunctionsChanged,
        eGeomObservationsChanged,
        eSystemNominalsChanged
    };

    void emitSignal(SignalType mySignalType);

};

/*!
 * \brief The FeatureContainer struct
 * Works as a container for all features in OpenIndy.
 * The features are stored as lists and also as maps (with their id as key).
 * The lists and maps are kept in sync.
 */
class FeatureContainer : public QObject{
    Q_OBJECT

public:
    explicit FeatureContainer(QObject *parent = 0) : QObject(parent){}

    //getter to access feature lists
    QList<FeatureWrapper *> &getFeaturesList(){ return this->myFeaturesList; }
    QList<CoordinateSystem *> &getCoordinateSystemsList(){ return this->myCoordinateSystemsList; }
    QList<Station *> &getStationsList(){ return this->myStationsList; }
    QList<TrafoParam *> &getTransformationParametersList(){ return this->myTransformationParametersList; }
    QList<FeatureWrapper *> &getGeometriesList(){ return this->myGeometriesList; }

    //getter to get a list of all feature ids
    QList<int> getFeatureIdList(){ return this->myFeaturesIdMap.keys(); }
    QList<QString> getFeatureNameList(){ return this->myFeaturesNameMap.keys(); }

    //getter to access a single feature by its id
    FeatureWrapper *getFeatureById(int id){ return this->myFeaturesIdMap.value(id, NULL); }
    QList<FeatureWrapper *> getFeaturesByName(QString name){ return this->myFeaturesNameMap.values(name); }

    //getter to get the number of available features
    int getFeatureCount(){ return this->myFeaturesList.size(); }
    int getStationCount(){ return this->myStationsList.size(); }
    int getCoordinateSystemCount(){ return this->myCoordinateSystemsList.size(); }
    int getTransformationParameterCount(){ return this->myTransformationParametersList.size(); }
    int getGeometryCount(){ return this->myGeometriesList.size(); }

public slots:

    //! stores a feature in lists and maps for later access
    bool addFeature(FeatureWrapper *myFeature){

        //if the feature is not valid
        if(myFeature == NULL || myFeature->getFeature() == NULL){
            return false;
        }

        //if the feature already exists it is not added
        if(this->myFeaturesIdMap.contains(myFeature->getFeature()->getId())){
            return false;
        }

        //add the feature to the feature lists and maps
        this->myFeaturesList.append(myFeature);
        this->myFeaturesIdMap.insert(myFeature->getFeature()->getId(), myFeature);
        this->myFeaturesNameMap.insert(myFeature->getFeature()->getFeatureName(), myFeature);
        switch(myFeature->getTypeOfFeature()){
        case Configuration::eCoordinateSystemFeature:
            this->myCoordinateSystemsList.append(myFeature->getCoordinateSystem());
            break;
        case Configuration::eStationFeature:
            this->myStationsList.append(myFeature->getStation());
            break;
        case Configuration::eTrafoParamFeature:
            this->myTransformationParametersList.append(myFeature->getTrafoParam());
            break;
        default:
            this->myGeometriesList.append(myFeature);
            break;
        }
        this->updateDisplayList();

        return true;

    }

    //! removes the feature with the given id permanently and also calls the feature's destructor
    bool removeAndDeleteFeature(int id){

        FeatureWrapper *myFeature = this->myFeaturesIdMap.value(id);
        if(this->removeFeature(id)){
            delete myFeature->getFeature();
            delete myFeature;
            return true;
        }
        return false;

    }

    //! removes the feature with the given id permanently
    bool removeFeature(int id){

        //check if the feature exists
        if(!this->myFeaturesIdMap.contains(id)){
            return false;
        }

        FeatureWrapper *myFeature = this->myFeaturesIdMap.value(id);

        //check if the feature is valid
        if(myFeature == NULL || myFeature->getFeature() == NULL){
            return false;
        }

        //remove the feature from lists and map
        this->myFeaturesList.removeOne(myFeature);
        this->myFeaturesIdMap.remove(id);
        this->myFeaturesNameMap.remove(myFeature->getFeature()->getFeatureName(), myFeature);
        switch(myFeature->getTypeOfFeature()){
        case Configuration::eCoordinateSystemFeature:
            this->myCoordinateSystemsList.removeOne(myFeature->getCoordinateSystem());
            break;
        case Configuration::eStationFeature:
            this->myStationsList.removeOne(myFeature->getStation());
            break;
        case Configuration::eTrafoParamFeature:
            this->myTransformationParametersList.removeOne(myFeature->getTrafoParam());
            break;
        default:
            this->myGeometriesList.removeOne(myFeature);
            break;
        }
        this->updateDisplayList();

        return true;

    }

    //! sets up the display list so that actuals and nominals are next to each other
    void updateDisplayList(){

        //clear display list
        this->myDisplayFeaturesList.clear();

        //refill display list
        int currentIndex = 0;
        while(currentIndex < this->myDisplayFeaturesList.size()){

            //get the next feature at currentIndex
            FeatureWrapper *currentFeature = this->myFeaturesList.at(currentIndex);

            //check if current feature has already been added
            if(this->myDisplayFeaturesList.contains(currentFeature)){
                currentIndex++;
                continue;
            }

            //if current feature is no geometry
            if(currentFeature->getGeometry() == NULL){
                this->myDisplayFeaturesList.append(currentFeature);
                currentIndex++;
                continue;
            }

            //get a list of all features with the same name
            QList<FeatureWrapper *> equalNameFeatures = this->getFeaturesByName(currentFeature->getFeature()->getFeatureName());

            //if current feature is a nominal geometry
            if(currentFeature->getGeometry()->getIsNominal()){

                //add actual first if it exists
                if(currentFeature->getGeometry()->getMyActual() != NULL){
                    FeatureWrapper *myActual = this->getFeatureById(currentFeature->getGeometry()->getMyActual()->getId());
                    if(myActual != NULL){
                        this->myDisplayFeaturesList.append(myActual);
                    }
                }

                //add all nominals with equal name
                this->myDisplayFeaturesList.append(currentFeature);
                foreach(FeatureWrapper *myFeature, equalNameFeatures){
                    if(myFeature != NULL && myFeature->getGeometry() != NULL && !this->myDisplayFeaturesList.contains(myFeature)){
                        this->myDisplayFeaturesList.append(myFeature);
                    }
                }

            }

            //if current feature is an actual geometry
            this->myDisplayFeaturesList.append(currentFeature);

            //TODO

        }

    }

private:

    //feature lists (useful to provide lists of features by category (geometries, stations etc.)
    QList<FeatureWrapper *> myFeaturesList; //list of all features in OpenIndy
    QList<CoordinateSystem *> myCoordinateSystemsList; //list of all non-station coordinate systems
    QList<Station *> myStationsList; //list of all stations (each station has pointer to its coordinate system)
    QList<TrafoParam *> myTransformationParametersList; //list of all trafo params
    QList<FeatureWrapper *> myGeometriesList; //list of all geometry features

    //feature maps (useful to quickly find a feature with a given id or name)
    QMap<int, FeatureWrapper *> myFeaturesIdMap; //map of all features in OpenIndy with their id as key
    QMultiMap<QString, FeatureWrapper *> myFeaturesNameMap; //map of all features in OpenIndy with their name as key

    //feature list use for display (sorted by name and nominal-actual relations))
    QList<FeatureWrapper *> myDisplayFeaturesList; //list of all features in OpenIndy

};

#endif // OIFEATURESTATE_H
