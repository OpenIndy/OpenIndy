#ifndef OIJOB_H
#define OIJOB_H

#include <QObject>
#include <QPointer>
#include <QString>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QMultiMap>
#include <QIODevice>
#include <QSet>
#include <QRegExp>
#include <QStringRef>

#include "featurecontainer.h"
#include "featureattributes.h"
#include "types.h"
#include "util.h"

/*!
 * \brief The OiJob class
 * Represents an OpenIndy job (holds all features and active states)
 */
class OiJob : public QObject
{
    Q_OBJECT

public:
    explicit OiJob(QObject *parent = 0);

    ~OiJob();

    //####################################
    //get or set meta information of a job
    //####################################

    const QString &getJobName() const;
    void setJobName(const QString &jobName);

    const QPointer<QIODevice> &getJobDevice() const;
    void setJobDevice(const QPointer<QIODevice> &jobDevice);

    int generateUniqueId();

    bool validateFeatureName(const QString &name, const FeatureTypes &type, const bool &isNominal = false,
                             const QPointer<CoordinateSystem> &nominalSystem = QPointer<CoordinateSystem>(NULL));

    //###########################################
    //methods to access the current feature state
    //###########################################

    //number of features
    int getFeatureCount() const;
    int getGeometryCount() const;
    int getFeatureCount(const FeatureTypes &type) const;

    //access a list of all feature ids and names
    const QList<int> &getFeatureIdList() const;
    const QStringList &getFeatureNameList() const;
    const QStringList &getFeatureGroupList() const;

    //access feature lists
    const QList<QPointer<FeatureWrapper> > &getFeaturesList() const;
    const QList<QPointer<CoordinateSystem> > &getCoordinateSystemsList() const;
    const QList<QPointer<Station> > &getStationsList() const;
    const QList<QPointer<TrafoParam> > &getTransformationParametersList() const;
    const QList<QPointer<FeatureWrapper> > &getGeometriesList() const;

    //access specific features
    QPointer<FeatureWrapper> getFeatureById(const int &featureId) const;
    QList<QPointer<FeatureWrapper> > getFeaturesByName(const QString &name) const;
    QList<QPointer<FeatureWrapper> > getFeaturesByGroup(const QString &group) const;
    QList<QPointer<FeatureWrapper> > getFeaturesByType(const FeatureTypes &type) const;

    //access active features
    const QPointer<FeatureWrapper> &getActiveFeature() const;
    const QPointer<Station> &getActiveStation() const;
    const QPointer<CoordinateSystem> &getActiveCoordinateSystem() const;

    //get or set feature groups
    const QString &getActiveGroup() const;
    bool setActiveGroup(const QString &group);

    //######################
    //add or remove features
    //######################

    bool addFeature(const QPointer<FeatureWrapper> &feature);
    QList<QPointer<FeatureWrapper> > addFeatures(const FeatureAttributes &fAttr);
    bool addFeatures(const QList<QPointer<FeatureWrapper> > &features);

    bool removeFeature(const int &featureId);
    bool removeFeature(const QPointer<FeatureWrapper> &feature);

    void removeAll();


    //activate features
    //bool activateFeature(const int &featureId);
    //bool activateStation(const int &featureId);
    //bool activateCoordinateSystem(const int &featureId);

    /*static FeatureWrapper *addFeature(FeatureAttributes attributes);
    static bool addFeature(FeatureWrapper *myFeature);
    static QList<FeatureWrapper *> addFeatures(FeatureAttributes attributes);
    static bool addFeatures(const QList<FeatureWrapper *> &myFeatures);*/

    //static CoordinateSystem *getNominalSystem(QString name);
    //static void resetFeatureLists();

signals:

    //#########################################
    //signals to inform about job state changes
    //#########################################

    //active feature changes
    void activeFeatureChanged();
    void activeStationChanged();
    void activeCoordinateSystemChanged();

    //feature(s) added or removed
    void featureSetChanged();
    void coordSystemSetChanged();
    void stationSetChanged();
    void trafoParamSetChanged();
    void geometrySetChanged();

    //group(s) added or removed
    void availableGroupsChanged();
    void activeGroupChanged();

    //general feature attributes changed
    void featureAttributesChanged();

    //feature specific attributes changed
    void featureNameChanged(const int &featureId, const QString &oldName);
    void featureGroupChanged(const int &featureId, const QString &oldGroup);
    void featureCommentChanged(const int &featureId);
    void featureIsUpdatedChanged(const int &featureId); //currently not connected
    void featureIsSolvedChanged(const int &featureId);
    void featureFunctionsChanged(const int &featureId);
    void featureUsedForChanged(const int &featureId);
    void featurePreviouslyNeededChanged(const int &featureId);

    //geometry specific attributes changed
    void geometryIsCommon(const int &featureId);
    void geometryNominals(const int &featureId);
    void geometryActual(const int &featureId);
    void geometryObservations(const int &featureId);
    void geometryNominalSystem(const int &featureId);
    void geometryStatistic(const int &featureId);
    void geometrySimulationData(const int &featureId);
    void geometryMeasurementConfig(const int &featureId);

    //coordinate system specific attributes changed
    void systemObservations(const int &featureId, const int &obsId);
    void systemTrafoParams(const int &featureId);
    void systemsNominals(const int &featureId);

    //station specific attributes changed
    void stationSensor(const int &featureId);

    //transformation parameter specific attributes changed
    void trafoParamParameters(const int &featureId);
    void trafoParamSystems(const int &featureId);
    void trafoParamIsUsed(const int &featureId);
    void trafoParamValidTime(const int &featureId);
    void trafoParamIsMovement(const int &featureId);

    //###################################################
    //general signals to inform OpenIndy about job issues
    //###################################################

    void sendMessage(const QString &msg);

private slots:

    //###########################################################################
    //internal slots that are called when a feature changes (or is added/removed)
    //###########################################################################

    //active feature changes
    void setActiveFeature(const int &featureId);
    void setActiveStation(const int &featureId);
    void setActiveCoordinateSystem(const int &featureId);

    //feature specific attributes changed
    void setFeatureName(const int &featureId, const QString &oldName);
    void setFeatureGroup(const int &featureId, const QString &oldGroup);
    void setFeatureComment(const int &featureId);
    void setFeatureIsUpdated(const int &featureId); //currently not connected
    void setFeatureIsSolved(const int &featureId);
    void setFeatureFunctions(const int &featureId);
    void setFeatureUsedFor(const int &featureId);
    void setFeaturePreviouslyNeeded(const int &featureId);

    //geometry specific attributes changed
    void setGeometryIsCommon(const int &featureId);
    void setGeometryNominals(const int &featureId);
    void setGeometryActual(const int &featureId);
    void setGeometryObservations(const int &featureId);
    void setGeometryNominalSystem(const int &featureId);
    void setGeometryStatistic(const int &featureId);
    void setGeometrySimulationData(const int &featureId);
    void setGeometryMeasurementConfig(const int &featureId);

    //coordinate system specific attributes changed
    void setSystemObservations(const int &featureId, const int &obsId);
    void setSystemTrafoParams(const int &featureId);
    void setSystemsNominals(const int &featureId);

    //station specific attributes changed
    void setStationSensor(const int &featureId);

    //transformation parameter specific attributes changed
    void setTrafoParamParameters(const int &featureId);
    void setTrafoParamSystems(const int &featureId);
    void setTrafoParamIsUsed(const int &featureId);
    void setTrafoParamValidTime(const int &featureId);
    void setTrafoParamIsMovement(const int &featureId);

private:

    //##############################
    //connect or disconnect features
    //##############################

    void connectFeature(const QPointer<FeatureWrapper> &feature);
    void disconnectFeature(const QPointer<FeatureWrapper> &feature);

    //##############
    //helper methods
    //##############

    QStringList createFeatureNames(const QString &name, const int &count) const;

private:

    //#########################
    //meta information of a job
    //#########################

    QString jobName; //the name under that the job is saved
    QPointer<QIODevice> jobDevice; //ther device where the job is saved

    //################################
    //active features and active group
    //################################

    //active features
    QPointer<FeatureWrapper> activeFeature;
    QPointer<Station> activeStation;
    QPointer<CoordinateSystem> activeCoordinateSystem;

    //active group
    QString activeGroup;

    //################################
    //feature container and current id
    //################################

    FeatureContainer featureContainer; //all features of this job

    int nextId; //the next free id an element of this job could get
















    //TODO check if still necessary
    //void removeFeature(const int &featureId);
    //void addPCSegmentAsFeature(FeatureWrapper *segment);

private:





    //################
    //helper functions
    //################

    //feature name validation and create feature functions
    //QList<FeatureWrapper *> createFeatures(const FeatureAttributes &attributes);
    //void createFeatureName(QString &outputName, int &index, QString inputName, int count = 1);
    //bool validateFeatureName(const FeatureTypes &featureType, const QString &featureName,
    //                                bool isNominal = false, CoordinateSystem *myNomSys = NULL);




};

#endif // OIJOB_H
