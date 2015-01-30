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

class FeatureContainer;

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
    static QList<FeatureWrapper *> getFeaturesByName(QString name);
    static QList<FeatureWrapper *> getFeaturesByGroup(QString group);
    static FeatureWrapper *getFeature(int featureId);

    static const QList<Station *> &getStations();
    static const QList<CoordinateSystem *> &getCoordinateSystems();
    static const QList<TrafoParam *> &getTransformationParameters();
    static const QList<FeatureWrapper *> &getGeometries();

    static FeatureWrapper *addFeature(FeatureAttributes attributes);
    static bool addFeature(FeatureWrapper *myFeature);
    static QList<FeatureWrapper *> addFeatures(FeatureAttributes attributes);
    static bool addFeatures(const QList<FeatureWrapper *> &myFeatures);

    static bool removeFeature(FeatureWrapper *myFeature);

    static FeatureWrapper* getActiveFeature();
    static Station* getActiveStation();
    static CoordinateSystem* getActiveCoordinateSystem();

    static const QMap<QString, int> getAvailableGroups();

    static QString getActiveGroup();
    static void setActiveGroup(QString group);

    static CoordinateSystem *getNominalSystem(QString name);

    static void resetFeatureLists();

    static bool validateFeatureName(Configuration::FeatureTypes featureType, QString featureName,
                                    bool isNominal = false, CoordinateSystem *myNomSys = NULL);

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
    void geometryMeasurementConfigChanged(); //emitted when measurement config was set

    void systemNominalsChanged(); //emitted when nominals where added or removed from a coordinate system

    void systemObservationsAdded(); //emitted when observations were added to a coordinate system

private slots:
    void setActiveFeature(const int &featureId); //is called when a feature becomes the active feature
    void setActiveStation(const int &featureId); //is called when a station becomes the active station
    void setActiveCoordinateSystem(const int &featureId); //is called when a coordinate system becomes the display system

    void setFeatureGroup(const int &featureId); //is called when the group attribute of a feature is changed
    void setFeatureName(const int &featureId, const QString &oldName); //is called when the name of a feature is changed
    void setFeatureComment(const int &featureId); //is called when the comment of a feature is changed
    void setFeatureFunctions(const int &featureId); //is called when the list of functions of a feature was edited

    void setGeometryActual(const int &featureId); //is called when the actual geometry of a nominal is set
    void setGeometryNominals(const int &featureId); //is called when a nominal was added or removed from an actual geometry
    void setGeometryObservations(const int &featureId); //is called when an observations was added or removed from a geometry
    void setGeometryMeasurementConfig(const int &featureId); //is called when the measurement config of a geometry has changed

    void setSystemsNominals(const int &featureId); //is called when a nominal was added to a nominal coordinate system

    void removeFeature(const int &featureId); //is called when the destructor of a feature is entered

    void addPCSegmentAsFeature(FeatureWrapper *segment);

    void setSystemObservations(const int &featureId, const int &obsId);

private:
    static OiFeatureState *myFeatureState;

    static FeatureContainer myFeatureContainer; //container that holds all features as lists and maps

    static FeatureWrapper *myActiveFeature; //the currently selected feature in table view
    static Station *myActiveStation; //the currently selected station
    static CoordinateSystem *myActiveCoordinateSystem; //the currently selected display coordinate system

    static QMap<QString, int> myAvailableGroups; //map of all available groups with number of assigned features
    static QString myActiveGroup; //currently selected feature group

    static bool groupsToBeUpdated;

    static void updateAvailableGroups();

    static QList<FeatureWrapper *> createFeatures(const FeatureAttributes &attributes);
    static void createFeatureName(QString &outputName, int &index, QString inputName, int count = 1);

    static void connectFeature(FeatureWrapper *myFeature);
    static void disconnectFeature(FeatureWrapper *myFeature);

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
        eGeomMeasurementConfigChanged,
        eSystemNominalsChanged,
        eSystemObservationsChanged
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

    //getter to get a list of all feature ids and names
    QList<int> getFeatureIdList() const{ return this->myFeaturesIdMap.keys(); }
    QList<QString> getFeatureNameList() const{ return this->myFeaturesNameMap.keys(); }

    //getter to access features by id or name
    FeatureWrapper *getFeatureById(const int &featureId) const{ return this->myFeaturesIdMap.value(featureId, NULL); }
    QList<FeatureWrapper *> getFeaturesByName(const QString &name) const{ return this->myFeaturesNameMap.values(name); }

    //getter to get the number of available features
    int getFeatureCount() const{ return this->myFeaturesList.size(); }
    int getStationCount() const{ return this->myStationsList.size(); }
    int getCoordinateSystemCount() const{ return this->myCoordinateSystemsList.size(); }
    int getTransformationParameterCount() const{ return this->myTransformationParametersList.size(); }
    int getGeometryCount() const{ return this->myGeometriesList.size(); }

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

        return true;

    }

    //! re-add the feature in the map with feature name as key
    bool renameFeature(int featureId, QString oldName){

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

};

#endif // OIFEATURESTATE_H
