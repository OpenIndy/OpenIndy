#ifndef OIFEATURESTATE_H
#define OIFEATURESTATE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QStringList>

#include "featurecontainer.h"
#include "featureattributes.h"
#include "oiconfigstate.h"

/*
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

class FeatureContainer;*/

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

    //################################################################################
    //public functions to access the current feature state from everywhere in OpenIndy
    //################################################################################

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
    static bool activateFeature(const int &featureId);
    static bool activateStation(const int &featureId);
    static bool activateCoordinateSystem(const int &featureId);

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
    static QStringList getAvailableGroups();
    static const QString &getActiveGroup();
    static bool setActiveGroup(const QString &group);


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
    void setFeatureGroup(const int &featureId, const QString &oldGroup);
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


    //TODO check if still necessary
    void removeFeature(const int &featureId);
    void addPCSegmentAsFeature(FeatureWrapper *segment);

private:
    static OiFeatureState *myFeatureState;

    //################################
    //active features and active group
    //################################

    //active features
    static FeatureWrapper *myActiveFeature;
    static Station *myActiveStation;
    static CoordinateSystem *myActiveCoordinateSystem;

    //active group
    static QString myActiveGroup;

    //################
    //helper functions
    //################

    //feature name validation and create feature functions
    static QList<FeatureWrapper *> createFeatures(const FeatureAttributes &attributes);
    static void createFeatureName(QString &outputName, int &index, QString inputName, int count = 1);
    static bool validateFeatureName(const Configuration::FeatureTypes &featureType, const QString &featureName,
                                    bool isNominal = false, CoordinateSystem *myNomSys = NULL);

    //feature connections
    static void connectFeature(FeatureWrapper *myFeature);
    static void disconnectFeature(FeatureWrapper *myFeature);

    //emit state changes
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

#endif // OIFEATURESTATE_H
