#ifndef OIJOB_H
#define OIJOB_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QStringList>

#include "featurecontainer.h"
#include "featureattributes.h"
#include "oiconfigstate.h"
#include "oiunitconverter.h"

class OiJob : public QObject
{
    Q_OBJECT

public:
    explicit OiJob(QObject *parent = 0);

    //####################################################
    //methods to access the current feature state
    //####################################################

    //number of features
    int getFeatureCount();
    int getStationCount();
    int getCoordinateSystemCount();
    int getGeometryCount();
    int getTrafoParamCount();

    //access feature lists
    const QList<FeatureWrapper *> &getFeatures();
    const QList<Station *> &getStations();
    const QList<CoordinateSystem *> &getCoordinateSystems();
    const QList<TrafoParam *> &getTransformationParameters();
    const QList<FeatureWrapper *> &getGeometries();

    //activate features
    bool activateFeature(const int &featureId);
    bool activateStation(const int &featureId);
    bool activateCoordinateSystem(const int &featureId);

    //access specific features
    QList<FeatureWrapper *> getFeaturesByName(const QString &name);
    QList<FeatureWrapper *> getFeaturesByGroup(const QString &group);
    FeatureWrapper *getFeature(const int &featureId);
    QList<FeatureWrapper *> getFeaturesByFeatureType(const Configuration::FeatureTypes &type);

    //add feature(s)
    QList<FeatureWrapper *> addFeatures(const FeatureAttributes &attributes);
    bool addFeature(FeatureWrapper *myFeature);
    bool addFeatures(const QList<FeatureWrapper *> &myFeatures);

    /*static FeatureWrapper *addFeature(FeatureAttributes attributes);
    static bool addFeature(FeatureWrapper *myFeature);
    static QList<FeatureWrapper *> addFeatures(FeatureAttributes attributes);
    static bool addFeatures(const QList<FeatureWrapper *> &myFeatures);*/

    //remove feature(s)
    bool removeFeature(FeatureWrapper *myFeature);

    //access active features
    FeatureWrapper* getActiveFeature();
    Station* getActiveStation();
    CoordinateSystem* getActiveCoordinateSystem();

    //get or set feature groups
    QStringList getAvailableGroups();
    const QString &getActiveGroup();
    bool setActiveGroup(const QString &group);


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

    FeatureContainer myFeatureContainer;

    //################################
    //active features and active group
    //################################

    //active features
    FeatureWrapper *myActiveFeature;
    Station *myActiveStation;
    CoordinateSystem *myActiveCoordinateSystem;

    //active group
    QString myActiveGroup;

    //################
    //helper functions
    //################

    //feature name validation and create feature functions
    QList<FeatureWrapper *> createFeatures(const FeatureAttributes &attributes);
    void createFeatureName(QString &outputName, int &index, QString inputName, int count = 1);
    bool validateFeatureName(const Configuration::FeatureTypes &featureType, const QString &featureName,
                                    bool isNominal = false, CoordinateSystem *myNomSys = NULL);

    //feature connections
    void connectFeature(FeatureWrapper *myFeature);
    void disconnectFeature(FeatureWrapper *myFeature);

};

#endif // OIJOB_H
