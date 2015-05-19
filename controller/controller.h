#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QPointer>
#include <QString>
#include <QIODevice>
#include <QFile>
#include <QFileInfo>
#include <QRegExp>

#include "oijob.h"
#include "modelmanager.h"
#include "console.h"
#include "featuretablecolumnconfig.h"
#include "trafoparamtablecolumnconfig.h"
#include "parameterdisplayconfig.h"
#include "featureattributes.h"
#include "exchangeParams.h"
#include "dataexchanger.h"
#include "sensorconfigurationmanager.h"
#include "measurementconfigmanager.h"
#include "featureupdater.h"
#include "projectexchanger.h"
#include "pluginloader.h"

/*!
 * \brief The Controller class
 */
class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = 0);

public:

    //###############################
    //getter to query available tools
    //###############################

    QList<QPointer<Tool> > getAvailableTools() const;

public slots:

    //#######################
    //tasks triggered by user
    //#######################

    //add or remove features
    void addFeatures(const FeatureAttributes &attributes);

    //recalculation
    void recalcActiveFeature();

    //set sensor configuration for active sensor
    void sensorConfigurationChanged(const QString &name, const bool &connectSensor);

    //set measurement configuration for active feature
    void measurementConfigurationChanged(const QString &name);

    //set active feature states
    //void setActiveFeature(const int &featureId);
    void setActiveStation(const int &featureId);
    void setActiveCoordinateSystem(const int &featureId);

    //import or export features
    void importNominals(const ExchangeParams &params);

    //import observations
    void importObservations(const QString &filename);

    //set display configs
    void setFeatureTableColumnConfig(const FeatureTableColumnConfig &config);
    void setTrafoParamColumnConfig(const TrafoParamTableColumnConfig &config);
    void setObservationTableColumnConfig(const ObservationTableColumnConfig &config);
    void setReadingTableColumnConfig(const ReadingTableColumnConfig &config);
    void setParameterDisplayConfig(const ParameterDisplayConfig &config);

    //save or load a job
    void saveProject();
    void saveProject(const QString &fileName);
    void loadProject(const QString &projectName, const QPointer<QIODevice> &device);
    const QPointer<OiJob> &createDefaultJob();

    //sensor actions
    void startConnect();
    void startDisconnect();
    void startMeasurement();
    void startMove(const Reading &reading);
    void startAim();
    void startToggleSight();
    void startInitialize();
    void startHome();
    void startCompensation();
    void startChangeMotorState();
    void startCustomAction(const QString &task);

signals:

    //#################################################
    //signals to inform about current job state changes
    //#################################################

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
    void featureIsUpdatedChanged(const int &featureId);
    void featureIsSolvedChanged(const int &featureId);
    void featureFunctionsChanged(const int &featureId);
    void featureUsedForChanged(const int &featureId);
    void featurePreviouslyNeededChanged(const int &featureId);

    //geometry specific attributes changed
    void geometryIsCommonChanged(const int &featureId);
    void geometryNominalsChanged(const int &featureId);
    void geometryActualChanged(const int &featureId);
    void geometryObservationsChanged(const int &featureId);
    void geometryNominalSystemChanged(const int &featureId);
    void geometryStatisticChanged(const int &featureId);
    void geometrySimulationDataChanged(const int &featureId);
    void geometryMeasurementConfigChanged(const int &featureId);

    //coordinate system specific attributes changed
    void systemObservationsChanged(const int &featureId, const int &obsId);
    void systemTrafoParamsChanged(const int &featureId);
    void systemsNominalsChanged(const int &featureId);

    //station specific attributes changed
    void stationSensorChanged(const int &featureId);

    //transformation parameter specific attributes changed
    void trafoParamParametersChanged(const int &featureId);
    void trafoParamSystemsChanged(const int &featureId);
    void trafoParamIsUsedChanged(const int &featureId);
    void trafoParamValidTimeChanged(const int &featureId);
    void trafoParamIsMovementChanged(const int &featureId);

    //#################################
    //import export task status changes
    //#################################

    //nominal import
    void nominalImportStarted();
    void nominalImportProgressUpdated(const int &progress, const QString &msg);
    void nominalImportFinished(const bool &success);

    //observation import
    void observationImportStarted();
    void observationImportProgressUpdated(const int &progress, const QString &msg);
    void observationImportFinished(const bool &success);

    //##############
    //sensor actions
    //##############

    void sensorActionStarted(const QString &name);
    void sensorActionFinished(const bool &success, const QString &msg);

private slots:

    //###################################
    //slots to react on job state changes
    //###################################

    void activeStationChangedCallback();

    //################################
    //slots to react on sensor results
    //################################

    void measurementFinished(const int &geomId, const QList<QPointer<Reading> > &readings);

private:

    //##############
    //helper methods
    //##############

    void setJob(const QPointer<OiJob> &job);

    void initDisplayConfigs();
    void initConfigManager();

    void logToConsole(const QString &msg);

    void registerMetaTypes();

    //######################
    //connect helper objects
    //######################

    void connectDataExchanger();
    void connectFeatureUpdater();

private:

    //##################
    //general attributes
    //##################

    QPointer<OiJob> job;

    //data exchanger
    DataExchanger exchanger;

    //feature updater
    FeatureUpdater featureUpdater;

    //config manager
    QPointer<SensorConfigurationManager> sensorConfigManager;
    QPointer<MeasurementConfigManager> measurementConfigManager;

};

#endif // CONTROLLER_H
