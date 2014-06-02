#ifndef OIFEATURESTATE_H
#define OIFEATURESTATE_H

#include <QObject>
#include <exception>

#include "featurewrapper.h"
#include "feature.h"
#include "geometry.h"
#include "station.h"
#include "coordinatesystem.h"
#include "console.h"
#include "configuration.h"

using namespace std;

class OiFeatureState : public QObject
{
    Q_OBJECT
private:
    explicit OiFeatureState(QObject *parent = 0);

public:
    static OiFeatureState *getInstance();

    static int getFeatureCount();

    static const QList<FeatureWrapper *> &getFeatures();
    static FeatureWrapper *addFeature(Configuration::FeatureTypes featureType, QString name);
    static bool addFeature(FeatureWrapper *myFeature);
    static bool removeFeature(FeatureWrapper *myFeature);

    static const QList<Station *> &getStations();
    static const QList<CoordinateSystem *> &getCoordinateSystems();

    static FeatureWrapper* getActiveFeature();
    static Station* getActiveStation();
    static CoordinateSystem* getActiveCoordinateSystem();

    static const QMap<QString, int> getAvailableGroups();

signals:
    void activeFeatureChanged();
    void activeStationChanged();
    void activeCoordinateSystemChanged();

    void featureSetChanged();

    void availableGroupsChanged();

private slots:
    void setActiveFeature(int featureId);
    void setActiveStation(int featureId);
    void setActiveCoordinateSystem(int featureId);
    void setFeatureGroups(int featureId);

private:
    static OiFeatureState *myFeatureState;

    static QList<FeatureWrapper *> myFeatures; //list of all features in OpenIndy
    static FeatureWrapper *myActiveFeature; //the currently selected feature in table view
    static QList<CoordinateSystem *> myCoordinateSystems; //list of all non-station coordinate systems
    static QList<Station *> myStations; //list of all stations (each station has pointer to its coordinate system)
    static Station *myActiveStation; //the currently selected station
    static CoordinateSystem *myActiveCoordinateSystem; //the currently selected display coordinate system*/
    static QMap<QString, int> myAvailableGroups; //map of all available groups with number of assigned features

    static bool groupsToBeUpdated;

    static int getFeatureListIndex(int featureId);
    static int getStationListIndex(int featureId);
    static int getCoordinateSystemIndex(int featureId);
    static void updateAvailableGroups();
    static void connectFeature(FeatureWrapper *myFeature);
    static void disconnectFeature(FeatureWrapper *myFeature);

    enum SignalType{
        eActiveFeatureChanged,
        eActiveStationChanged,
        eActiveCoordinateSystemChanged,
        eFeatureSetChanged,
        eAvailableGroupsChanged
    };

    void emitSignal(SignalType mySignalType);

};

#endif // OIFEATURESTATE_H
