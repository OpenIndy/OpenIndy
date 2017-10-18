#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QPointer>
#include <QString>
#include <QIODevice>
#include <QFile>
#include <QFileInfo>
#include <QRegExp>
#include <QThread>

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
#include "oiwebsocketserver.h"

using namespace oi;

/*!
 * \brief The Controller class
 */
class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = 0);
    ~Controller();

public:

    //###############################
    //getter to query available tools
    //###############################

    const QList<QPointer<Tool> > &getAvailableTools() const;

public slots:

    //#######################
    //tasks triggered by user
    //#######################

    //add or remove features
    void addFeatures(const FeatureAttributes &attributes);
    void removeFeatures(const QSet<int> &featureIds);

    //change feature parameters
    void setNominalParameters(const int &featureId, const QMap<GeometryParameters, double> &parameters);
    void setTrafoParamParameters(const int &featureId, const QMap<TrafoParamParameters, double> &parameters);

    //recalculation
    void recalcActiveFeature();

    //set sensor configuration for active station
    void setSensorConfig(const SensorConfiguration &sConfig, bool connectSensor);

    //remove the sensor of the active station
    void removeActiveStationSensor();

    //sensor configs edited
    void sensorConfigurationsEdited(const SensorConfigurationManager &manager);

    //active station's sensor config edited
    void sensorConfigurationUpdated(const SensorConfiguration &sConfig);

    //set measurement configuration for active feature
    void measurementConfigurationChanged(const MeasurementConfig &mConfig);

    //import or export features
    void importNominals(const ExchangeParams &params);

    //add or remove observations
    void importObservations(const QString &filename);
    void removeObservations(const int &featureId);
    void removeAllObservations();

    //set display configs
    void setFeatureTableColumnConfig(const FeatureTableColumnConfig &config);
    void setTrafoParamColumnConfig(const TrafoParamTableColumnConfig &config);
    void setObservationTableColumnConfig(const ObservationTableColumnConfig &config);
    void setReadingTableColumnConfig(const ReadingTableColumnConfig &config);
    void setParameterDisplayConfig(const ParameterDisplayConfig &config);

    //add or remove bundle system
    void addBundleSystem();
    void removeBundleSystem(const int &id);

    //load or calculate bundle
    QJsonObject getBundleTemplate(const int &bundleId);
    QPointer<BundleAdjustment> getBundleAdjustment(const int &bundleId);
    void updateBundleAdjustment(const int &bundleId, const QJsonObject &param);
    void loadBundleTemplate(const int &bundleId, const QJsonObject &bundleTemplate);
    void runBundle(const int &bundleId);

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
    void startAimAndMeasure();
    void startToggleSight();
    void startInitialize();
    void startHome();
    void startCompensation();
    void startChangeMotorState();
    void startCustomAction(const QString &task);
    void startWatchWindow(ReadingTypes streamFormat);
    void stopWatchWindow();

    //log messages to the specified destination
    void log(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest);

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

    bool featureCreated(bool created);

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
    //void trafoParamIsMovementChanged(const int &featureId);

    //whole job instance changed
    void currentJobChanged();

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

    //sensor actions
    void sensorActionStarted(const QString &name);
    void sensorActionFinished(const bool &success, const QString &msg);
    void measurementCompleted();

    //#############
    //show messages
    //#############

    //messaging
    void showMessageBox(const QString &msg, const MessageTypes &msgType);
    void showStatusMessage(const QString &msg, const MessageTypes &msgType);
    void showClientMessage(const QString &msg, const MessageTypes &msgType);

    //#################
    //update status bar
    //#################

    void updateStatusBar();

    //########################
    //trigger GUI interactions
    //########################

    void saveAsTriggered();

    //##############################
    //web socket server interactions
    //##############################

    //start and stop server
    void startWebSocketServer();
    void stopWebSocketServer();

private slots:

    //###################################
    //slots to react on job state changes
    //###################################

    //active station changed
    void activeStationChangedCallback();

    //################################
    //slots to react on sensor results
    //################################

    void measurementFinished(const int &geomId, const QList<QPointer<Reading> > &readings);

private:

    //##############
    //helper methods
    //##############

    //set OpenIndy job
    void setJob(const QPointer<OiJob> &job);

    //init configs and plugins
    void initDisplayConfigs();
    void initConfigManager();
    void initToolPlugins();

    //register meta types
    void registerMetaTypes();

    //start or stop OpenIndy server
    void initServer();
    void startServer();
    void stopServer();

    //create feature helpers
    bool createActualFromNominal(const QPointer<Geometry> &geometry);
    void addFunctionsAndMConfigs(const QList<QPointer<FeatureWrapper> > &actuals,
                                 const MeasurementConfig &mConfig, const QString &path, const QString &fName);

    //###############
    //set up connects
    //###############

    //connect helper objects
    void connectDataExchanger();
    void connectFeatureUpdater();
    void connectRequestHandler();

    //connect tools
    void connectToolPlugin(const QPointer<Tool> &tool);

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

    //thread and server instance
    QThread serverThread;
    QPointer<OiWebSocketServer> webSocketServer;

    //request handler for web socket requests
    OiRequestHandler requestHandler;

    //tool plugins
    QList<QPointer<Tool> > toolPlugins;

};

#endif // CONTROLLER_H
