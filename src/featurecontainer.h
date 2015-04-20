#ifndef FEATURECONTAINER_H
#define FEATURECONTAINER_H

#include <QPointer>
#include <QString>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QMultiMap>

#include "featurewrapper.h"
#include "feature.h"
#include "coordinatesystem.h"
#include "station.h"
#include "trafoparam.h"
#include "types.h"

/*!
 * \brief The FeatureContainer class
 * Works as a container for all features in an OiJob.
 * The features are stored as lists and also as maps (e.g. with their id as key).
 * The lists and maps are kept in sync.
 */
class FeatureContainer{

public:
    FeatureContainer();

    ~FeatureContainer();

    //###############
    //access features
    //###############

    //getter to access feature lists
    const QList<QPointer<FeatureWrapper> > &getFeaturesList() const;
    const QList<QPointer<CoordinateSystem> > &getCoordinateSystemsList() const;
    QList<QPointer<CoordinateSystem> > getStationSystemsList() const;
    const QList<QPointer<Station> > &getStationsList() const;
    const QList<QPointer<TrafoParam> > &getTransformationParametersList() const;
    const QList<QPointer<FeatureWrapper> > &getGeometriesList() const;

    //getter to get a list of all feature ids, names and groups
    const QList<int> &getFeatureIdList() const;
    const QStringList &getFeatureNameList() const;
    const QStringList &getFeatureGroupList() const;

    //getter to access features by id, name, group or type
    QPointer<FeatureWrapper> getFeatureById(const int &featureId) const;
    QList<QPointer<FeatureWrapper> > getFeaturesByName(const QString &name) const;
    QList<QPointer<FeatureWrapper> > getFeaturesByGroup(const QString &group) const;
    QList<QPointer<FeatureWrapper> > getFeaturesByType(const FeatureTypes &type) const;

    //######################
    //get number of features
    //######################

    int getFeatureCount() const;
    int getGeometryCount() const;
    int getFeatureCount(const FeatureTypes &type) const;

    //############################
    //add or remove single feature
    //############################

    bool addFeature(const QPointer<FeatureWrapper> &feature);
    bool removeFeature(const int &featureId);

    void checkAndClean(const int &featureId, const QString &name, const QString &group, const FeatureTypes &type);

    void removeAll();

    //#######################################################
    //callbacks on feature changes to update keys helper maps
    //#######################################################

    bool featureNameChanged(const int &featureId, const QString &oldName);
    bool featureGroupChanged(const int &featureId, const QString &oldGroup);

private:

    //######################
    //feature lists and maps
    //######################

    //feature lists (useful to provide lists of features by category (geometries, stations etc.))
    QList<QPointer<FeatureWrapper> > featuresList; //list of all features in OpenIndy
    QList<QPointer<CoordinateSystem> > coordSystems; //list of all non-station coordinate systems
    QList<QPointer<Station> > stationsList; //list of all stations (each station has pointer to its coordinate system)
    QList<QPointer<TrafoParam> > trafoParamsList; //list of all trafo params
    QList<QPointer<FeatureWrapper> > geometriesList; //list of all geometry features

    //feature maps (useful to quickly find a feature with a given id, name or group etc.)
    QMap<int, QPointer<FeatureWrapper> > featuresIdMap; //map of all features in OpenIndy with their id as key
    QMultiMap<QString, QPointer<FeatureWrapper> > featuresNameMap; //map of all features in OpenIndy with their name as key
    QMultiMap<QString, QPointer<FeatureWrapper> > featuresGroupMap; //map of all features in OpenIndy with their group as key
    QMultiMap<FeatureTypes, QPointer<FeatureWrapper> > featuresTypeMap; // map of all features in OpenIndy with their type as key

    //lists with ids, names and groups
    QList<int> featureIds;
    QStringList featureNames;
    QStringList featureGroups;

};

#endif // FEATURECONTAINER_H
