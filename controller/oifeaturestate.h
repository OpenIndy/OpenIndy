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
#include "featureattributes.h"

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

    //###############################################################################
    //public functions to access the current feature state from everywhere in OpenIny
    //###############################################################################

    //number of features
    static int getFeatureCount();
    static int getStationCount();
    static int getCoordinateSystemCount();
    static int getGeometryCount();
    static int getTrafoParamCount();

    //access feature lists
    static const QList<FeatureWrapper *> &getFeatures();
    static const QList<Station *> &getStations();
    static const QList<CoordinateSystem *> &getCoordinateSystems();
    static const QList<TrafoParam *> &getTransformationParameters();
    static const QList<FeatureWrapper *> &getGeometries();

    //activate features
    static bool makeActiveFeature(const int &featureId);
    static bool makeActiveStation(const int &featureId);
    static bool makeActiveCoordinateSystem(const int &featureId);

    //access specific features
    static QList<FeatureWrapper *> getFeaturesByName(const QString &name);
    static QList<FeatureWrapper *> getFeaturesByGroup(const QString &group);
    static FeatureWrapper *getFeature(const int &featureId);

    //add feature(s)
    static QList<FeatureWrapper *> addFeatures(const FeatureAttributes &attributes);
    static bool addFeature(FeatureWrapper *myFeature);
    static bool addFeatures(const QList<FeatureWrapper *> &myFeatures);

    /*static FeatureWrapper *addFeature(FeatureAttributes attributes);
    static bool addFeature(FeatureWrapper *myFeature);
    static QList<FeatureWrapper *> addFeatures(FeatureAttributes attributes);
    static bool addFeatures(const QList<FeatureWrapper *> &myFeatures);*/

    //remove feature(s)
    static bool removeFeature(FeatureWrapper *myFeature);

    //access active features
    static FeatureWrapper* getActiveFeature();
    static Station* getActiveStation();
    static CoordinateSystem* getActiveCoordinateSystem();

    //get or set feature groups
    static const QMap<QString, int> getAvailableGroups();
    static QString getActiveGroup();
    static void setActiveGroup(QString group);


    //static CoordinateSystem *getNominalSystem(QString name);
    //static void resetFeatureLists();


signals:

    //##############################################
    //signals to inform about OpenIndy state changes
    //##############################################

    //active feature changes
    void activeFeatureChanged();
    void activeStationChanged();
    void activeCoordinateSystemChanged();

    //feature(s) added or removed
    void featureSetChanged();
    void coordSystemSetChanged();

    //group(s) added or removed
    void availableGroupsChanged();

    //general feature attributes changed
    void featureAttributesChanged();
    void featureFunctionsChanged();

    //geometry specific attributes changed
    void geometryObservationsChanged();
    void geometryMeasurementConfigChanged();

    //coordinate system specific attributes changed
    void systemNominalsChanged();
    void systemObservationsAdded();

private slots:

    //###########################################################################
    //internal slots that are called when a feature changes (or is added/removed)
    //###########################################################################

    //active feature changes
    void setActiveFeature(const int &featureId);
    void setActiveStation(const int &featureId);
    void setActiveCoordinateSystem(const int &featureId);

    //general feature attributes changed
    void setFeatureGroup(const int &featureId);
    void setFeatureName(const int &featureId, const QString &oldName);
    void setFeatureComment(const int &featureId);
    void setFeatureFunctions(const int &featureId);

    //geometry specific attributes changed
    void setGeometryActual(const int &featureId);
    void setGeometryNominals(const int &featureId);
    void setGeometryObservations(const int &featureId);
    void setGeometryMeasurementConfig(const int &featureId);

    //coordinate system specific attributes changed
    void setSystemsNominals(const int &featureId);
    void setSystemObservations(const int &featureId, const int &obsId);

    void removeFeature(const int &featureId);
    void addPCSegmentAsFeature(FeatureWrapper *segment);

private:
    static OiFeatureState *myFeatureState;

    //container that holds all features as lists and maps
    static FeatureContainer myFeatureContainer;

    //active features
    static FeatureWrapper *myActiveFeature;
    static Station *myActiveStation;
    static CoordinateSystem *myActiveCoordinateSystem;

    //groups
    static QMap<QString, int> myAvailableGroups;
    static QString myActiveGroup;


    static bool validateFeatureName(Configuration::FeatureTypes featureType, QString featureName,
                                    bool isNominal = false, CoordinateSystem *myNomSys = NULL);

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
