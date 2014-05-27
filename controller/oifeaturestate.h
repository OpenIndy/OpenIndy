#ifndef OIFEATURESTATE_H
#define OIFEATURESTATE_H

#include <exception>

#include "featurewrapper.h"
#include "feature.h"
#include "geometry.h"
#include "station.h"
#include "coordinatesystem.h"
#include "console.h"
#include "configuration.h"

using namespace std;

class OiFeatureState
{
public:
    static bool isActiveFeature(const FeatureWrapper *myFeature);
    static bool isActiveStation(const Station *myStation);
    static bool isActiveCoordinateSystem(const CoordinateSystem *myCoordinateSystem);

    static int getFeatureCount();

    static QList<FeatureWrapper *> &getFeatures();
    static FeatureWrapper *getActiveFeature();
    static Station *getActiveStation();
    static CoordinateSystem *getActiveCoordinateSystem();

    static FeatureWrapper *addFeature(const Configuration::FeatureTypes featureType, const QString name);
    static bool setActiveFeature(const unsigned int id);
    static bool setActiveStation(const unsigned int id);
    static bool setActiveCoordinateSystem(const unsigned int id);

private:
    static QList<FeatureWrapper *> myFeatures; //list of all features in OpenIndy
    static FeatureWrapper *myActiveFeature; //the currently selected feature in table view
    static QList<CoordinateSystem *> myCoordinateSystems; //list of all non-station coordinate systems
    static QList<Station *> myStations; //list of all stations (each station has pointer to its coordinate system)
    static Station *myActiveStation; //the currently selected station
    static CoordinateSystem *myActiveCoordinateSystem; //the currently selected display coordinate system*/
    static QMap<QString, int> myAvailableGroups; //map of all available groups with number of assigned features
};

#endif // OIFEATURESTATE_H
