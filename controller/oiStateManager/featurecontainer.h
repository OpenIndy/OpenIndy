#ifndef FEATURECONTAINER_H
#define FEATURECONTAINER_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QMultiMap>

#include "featurewrapper.h"
#include "feature.h"
#include "coordinatesystem.h"
#include "station.h"
#include "trafoparam.h"

class OiFeatureState;

/*!
 * \brief The FeatureContainer class
 * Works as a container for all features in OpenIndy.
 * The features are stored as lists and also as maps (e.g. with their id as key).
 * The lists and maps are kept in sync.
 */
class FeatureContainer{
    friend class OiFeatureState;

private:
    FeatureContainer();

    //getter to access feature lists
    static QList<FeatureWrapper *> &getFeaturesList();
    static QList<CoordinateSystem *> &getCoordinateSystemsList();
    static QList<Station *> &getStationsList();
    static QList<TrafoParam *> &getTransformationParametersList();
    static QList<FeatureWrapper *> &getGeometriesList();

    //getter to get a list of all feature ids and names
    static QList<int> getFeatureIdList();
    static QStringList getFeatureNameList();
    static QStringList getFeatureGroupList();

    //getter to access features by id or name
    static FeatureWrapper *getFeatureById(const int &featureId);
    static QList<FeatureWrapper *> getFeaturesByName(const QString &name);
    static QList<FeatureWrapper *> getFeaturesByGroup(const QString &group);

    //getter to get the number of available features
    static int getFeatureCount();
    static int getStationCount();
    static int getCoordinateSystemCount();
    static int getTransformationParameterCount();
    static int getGeometryCount();

    //add or remove features
    static bool addFeature(FeatureWrapper *myFeature);
    static bool removeAndDeleteFeature(const int &featureId);
    static bool removeFeature(const int &featureId);

    //callbacks on feature changes
    static bool featureNameChanged(const int &featureId, const QString &oldName);
    static bool featureGroupChanged(const int &featureId, const QString &oldGroup);

private:

    //feature lists (useful to provide lists of features by category (geometries, stations etc.))
    static QList<FeatureWrapper *> myFeaturesList; //list of all features in OpenIndy
    static QList<CoordinateSystem *> myCoordinateSystemsList; //list of all non-station coordinate systems
    static QList<Station *> myStationsList; //list of all stations (each station has pointer to its coordinate system)
    static QList<TrafoParam *> myTransformationParametersList; //list of all trafo params
    static QList<FeatureWrapper *> myGeometriesList; //list of all geometry features

    //feature maps (useful to quickly find a feature with a given id, name or group)
    static QMap<int, FeatureWrapper *> myFeaturesIdMap; //map of all features in OpenIndy with their id as key
    static QMultiMap<QString, FeatureWrapper *> myFeaturesNameMap; //map of all features in OpenIndy with their name as key
    static QMultiMap<QString, FeatureWrapper *> myFeaturesGroupMap; //map of all features in OpenIndy with their group as key

};

#endif // FEATURECONTAINER_H
