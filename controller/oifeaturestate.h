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
#include "oiconfigstate.h"

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
    static FeatureWrapper *addFeature(Configuration::FeatureTypes featureType, bool isNominal, QString name);
    static bool addFeature(FeatureWrapper *myFeature);
    static bool removeFeature(FeatureWrapper *myFeature);

    static const QList<Station *> &getStations();
    static const QList<CoordinateSystem *> &getCoordinateSystems();

    static FeatureWrapper* getActiveFeature();
    static Station* getActiveStation();
    static CoordinateSystem* getActiveCoordinateSystem();

    static const QMap<QString, int> getAvailableGroups();

    static QString getActiveGroup();
    static void setActiveGroup(QString group);

    FeatureWrapper *getFeature(int featureId);

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

    void addPCSegmentAsFeature(FeatureWrapper *segment);

private:
    static OiFeatureState *myFeatureState;

    static QList<FeatureWrapper *> myFeatures; //list of all features in OpenIndy
    static FeatureWrapper *myActiveFeature; //the currently selected feature in table view
    static QList<CoordinateSystem *> myCoordinateSystems; //list of all non-station coordinate systems
    static QList<Station *> myStations; //list of all stations (each station has pointer to its coordinate system)
    static Station *myActiveStation; //the currently selected station
    static CoordinateSystem *myActiveCoordinateSystem; //the currently selected display coordinate system*/
    static QMap<QString, int> myAvailableGroups; //map of all available groups with number of assigned features
    static QString myActiveGroup; //currently selected feature group

    static bool groupsToBeUpdated;

    static int getFeatureListIndex(int featureId);
    static int getStationListIndex(int featureId);
    static int getCoordinateSystemIndex(int featureId);
    static int getStationCoordinayteSystemIndex(int featureId);
    static void updateAvailableGroups();
    static void connectFeature(FeatureWrapper *myFeature);
    static void disconnectFeature(FeatureWrapper *myFeature);

    static bool validateFeatureName(Configuration::FeatureTypes featureType, QString featureName,
                                    bool isNominal = false, CoordinateSystem *myNomSys = NULL, int featureId = -1);

    enum SignalType{
        eActiveFeatureChanged,
        eActiveStationChanged,
        eActiveCoordinateSystemChanged,
        eFeatureSetChanged,
        eCoordSysSetChanged,
        eAvailableGroupsChanged,
        eFeatureAttributesChanged,
        eFeatureFunctionsChanged,
        eGeomObservationsChanged
    };

    void emitSignal(SignalType mySignalType);

};

#endif // OIFEATURESTATE_H
