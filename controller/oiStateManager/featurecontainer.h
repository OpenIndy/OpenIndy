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

class OiJob;

/*!
 * \brief The FeatureContainer class
 * Works as a container for all features in an OiJob.
 * The features are stored as lists and also as maps (e.g. with their id as key).
 * The lists and maps are kept in sync.
 */
class FeatureContainer{
    friend class OiJob;

private:
    FeatureContainer();

    //getter to access feature lists
    QList<FeatureWrapper *> &getFeaturesList();
    QList<CoordinateSystem *> &getCoordinateSystemsList();
    QList<Station *> &getStationsList();
    QList<TrafoParam *> &getTransformationParametersList();
    QList<FeatureWrapper *> &getGeometriesList();

    //getter to get a list of all feature ids and names
    QList<int> getFeatureIdList();
    QStringList getFeatureNameList();
    QStringList getFeatureGroupList();

    //getter to access features by id or name
    FeatureWrapper *getFeatureById(const int &featureId);
    QList<FeatureWrapper *> getFeaturesByName(const QString &name);
    QList<FeatureWrapper *> getFeaturesByGroup(const QString &group);

    //getter to get the number of available features
    int getFeatureCount();
    int getStationCount();
    int getCoordinateSystemCount();
    int getTransformationParameterCount();
    int getGeometryCount();

    //add or remove features
    bool addFeature(FeatureWrapper *myFeature);
    bool removeAndDeleteFeature(const int &featureId);
    bool removeFeature(const int &featureId);

    //callbacks on feature changes
    bool featureNameChanged(const int &featureId, const QString &oldName);
    bool featureGroupChanged(const int &featureId, const QString &oldGroup);

private:

    //feature lists (useful to provide lists of features by category (geometries, stations etc.))
    QList<FeatureWrapper *> myFeaturesList; //list of all features in OpenIndy
    QList<CoordinateSystem *> myCoordinateSystemsList; //list of all non-station coordinate systems
    QList<Station *> myStationsList; //list of all stations (each station has pointer to its coordinate system)
    QList<TrafoParam *> myTransformationParametersList; //list of all trafo params
    QList<FeatureWrapper *> myGeometriesList; //list of all geometry features

    //feature maps (useful to quickly find a feature with a given id, name or group)
    QMap<int, FeatureWrapper *> myFeaturesIdMap; //map of all features in OpenIndy with their id as key
    QMultiMap<QString, FeatureWrapper *> myFeaturesNameMap; //map of all features in OpenIndy with their name as key
    QMultiMap<QString, FeatureWrapper *> myFeaturesGroupMap; //map of all features in OpenIndy with their group as key

};

#endif // FEATURECONTAINER_H
