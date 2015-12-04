#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QString>
#include <QIODevice>
#include <QListView>
#include <QSignalMapper>
#include <QClipboard>

#include "controller.h"
#include "featureattributes.h"
#include "modelmanager.h"
#include "console.h"
#include "exchangeParams.h"
#include "toolaction.h"

#include "createfeaturedialog.h"
#include "pluginloaderdialog.h"
#include "importnominaldialog.h"
#include "featurefunctionsdialog.h"
#include "loadingdialog.h"
#include "sensorconfigurationdialog.h"
#include "movesensordialog.h"
#include "sensortaskinfodialog.h"
#include "pluginmanagerdialog.h"
#include "watchwindowdialog.h"
#include "measurementconfigurationdialog.h"
#include "settingsdialog.h"
#include "actualpropertiesdialog.h"
#include "nominalpropertiesdialog.h"
#include "trafoparampropertiesdialog.h"
#include "aboutdialog.h"
#include "stationpropertiesdialog.h"

#include "featuretabledelegate.h"
#include "trafoparamtabledelegate.h"

using namespace oi;

namespace Ui {
class MainWindow;
}

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

    //###################################
    //tasks to be performed by controller
    //###################################

    //add or remove features
    void addFeatures(const FeatureAttributes &attributes);
    void removeFeatures(const QSet<int> &featureIds);

    //remove observations
    void removeObservations(const int &featureId);
    void removeAllObservations();

    //remove the sensor of the active station
    void removeActiveStationSensor();

    //sensor configs edited
    void sensorConfigurationsEdited(const SensorConfigurationManager &manager);

    //set sensor configuration for active sensor
    void sensorConfigurationChanged(const SensorConfiguration &sConfig, bool connectSensor);

    //set measurement configuration for active feature
    void measurementConfigurationChanged(const MeasurementConfig &mConfig);

    //recalculation of features
    void recalcAll();
    void recalcActiveFeature();
    void recalcFeature(const int &featureId);
    void recalcTrafoParam(const int &featureId);

    //import or export features
    void importNominals(const ExchangeParams &params);

    //save or load projects
    void saveProject();
    void saveProject(const QString &fileName);
    void loadProject(const QString &projectName, const QPointer<QIODevice> &device);

    //log messages
    void log(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

private slots:

    //#############################
    //tasks triggered by controller
    //#############################

    //nominal import
    void importNominalsStarted();
    void importNominalsFinished(const bool &success);

    //observation import
    void importObservationsStarted();
    void importObservationsFinished(const bool &success);

    //active feature changes
    void activeFeatureChanged();
    void activeStationChanged();
    void activeCoordinateSystemChanged();

    //feature(s) added or removed
    void coordSystemSetChanged();
    void stationSetChanged();
    void trafoParamSetChanged();

    //group(s) added or removed
    void availableGroupsChanged();
    void activeGroupChanged();

    //feature specific attributes changed
    void featureNameChanged(const int &featureId, const QString &oldName);

    //station changes
    void stationSensorChanged(const int &featureId);

    //hole job instance changed
    void currentJobChanged();

    //sensor actions
    void sensorActionStarted(const QString &name);
    void sensorActionFinished(const bool &success, const QString &msg);
    void measurementCompleted();

    //display messages
    void showMessageBox(const QString &msg, const MessageTypes &msgType);
    void showStatusMessage(const QString &msg, const MessageTypes &msgType);

    //#########################
    //actions triggered by user
    //#########################

    //key pressed
    void keyPressEvent(QKeyEvent * e);

    //create feature actions
    void on_actionCreate_point_triggered();
    void on_actionCreate_line_triggered();
    void on_actionCreate_plane_triggered();
    void on_actionCreate_sphere_triggered();
    void on_actionCreate_station_triggered();
    void on_actionCreate_coordinatesystem_triggered();
    void on_actionCreate_scalar_entity_triggered();
    void on_actionCreate_trafoParam_triggered();
    void on_actionCreate_cone_triggered();
    void on_actionCreate_cylinder_triggered();
    void on_actionCreate_ellipsoid_triggered();
    void on_actionCreate_hyperboloid_triggered();
    void on_actionCreate_paraboloid_triggered();
    void on_actionCreate_nurbs_triggered();
    void on_actionCreate_pointcloud_triggered();
    void on_actionCreate_circle_triggered();
    void on_actionCreate_torus_triggered();
    void on_actionCreate_slotted_hole_triggered();
    void on_actionCreate_ellipse_triggered();

    //plugin actions
    void on_actionLoad_plugins_triggered();
    void on_actionPlugin_manager_triggered();

    //import export actions
    void on_action_importNominals_triggered();

    //feature table view interactions
    void on_tableView_features_clicked(const QModelIndex &index);
    void tableViewFeaturesSelectionChangedByKeyboard(const QModelIndex &selected, const QModelIndex &deselected);
    void on_tableView_features_customContextMenuRequested(const QPoint &pos);
    void on_tableView_trafoParams_clicked(const QModelIndex &index);
    void tableViewTrafoParamsSelectionChangedByKeyboard(const QModelIndex &selected, const QModelIndex &deselected);
    void on_tableView_trafoParams_customContextMenuRequested(const QPoint &pos);

    //function dialog
    void on_actionSet_function_triggered();

    //sensor dialogs
    void on_actionSet_sensor_triggered();
    void setSensorConfiguration(const SensorConfiguration &sConfig);
    void showMoveSensorDialog();
    void on_actionStationProperties_triggered();

    //toggle visibility of widgets
    void on_actionControl_pad_triggered();
    void on_actionConsole_triggered();
    void on_actionMagnify_triggered();

    //active group, active coordinate system or actual nominal filter changed
    void on_comboBox_groups_currentIndexChanged(const QString &arg1);
    void on_comboBox_activeCoordSystem_currentIndexChanged(const QString &arg1);
    void on_comboBox_actualNominal_currentIndexChanged(const QString &arg1);

    //show watch window
    void on_actionWatch_window_triggered();

    //save or load projects
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();

    //close OpenIndy
    void on_actionClose_triggered();

    //show measurement config dialog
    void on_actionMeasurement_Configuration_triggered();

    //activate a station
    void on_actionActivate_station_triggered();

    //show settings dialog
    void on_actionView_settings_triggered();

    //show tool widget
    void showToolWidget(const QString &pluginName, const QString &toolName);

    //resize table views
    void resizeTableView();

    //remove observations
    void on_actionRemoveObservations_triggered();
    void removeObservationOfActiveFeature();

    //show about dialog
    void on_actionAbout_OpenIndy_triggered();

    //##############
    //helper methods
    //##############

    //helper methods that work on the active feature
    void showFeatureProperties(bool checked);

    //helper methods that work on a user defined selection of features
    void aimAndMeasureFeatures();
    void deleteFeatures(bool checked);

    //copy values from and to clipboard
    void copyToClipboard();
    void pasteFromClipboard();

    //set up status bar
    void updateStatusBar();

private:
    Ui::MainWindow *ui;

private:

    Controller control;

    //#################################
    //set up models and connect dialogs
    //#################################

    void connectController();
    void connectDialogs();
    void connectStatusBar();
    void assignModels();

    //##################################
    //methods to initialize GUI elements
    //##################################

    void initFeatureTableViews();
    void initSensorPad();
    void initToolMenus();
    void initFilterComboBoxes();
    void initStatusBar();

    //##############################
    //methods to update GUI elements
    //##############################

    void activeSensorTypeChanged(const SensorTypes &type, const QList<SensorFunctions> &supportedActions, const QStringList &selfDefinedActions);

    void updateMagnifyWindow(const QPointer<FeatureWrapper> &feature);

    void updateGroupFilterSize();
    void updateSystemFilterSize();
    void updateActualNominalFilterSize();

    //############################
    //OpenIndy dialogs and widgets
    //############################

    CreateFeatureDialog createFeatureDialog;
    PluginLoaderDialog pluginLoaderDialog;
    ImportNominalDialog importNominalDialog;
    LoadingDialog loadingDialog;
    FeatureFunctionsDialog featureFunctionsDialog;
    SensorConfigurationDialog sensorConfigurationDialog;
    MoveSensorDialog moveSensorDialog;
    SensorTaskInfoDialog sensorTaskInfoDialog;
    PluginManagerDialog pluginManagerDialog;
    WatchWindowDialog watchWindowDialog;
    MeasurementConfigurationDialog measurementConfigDialog;
    SettingsDialog settingsDialog;
    ActualPropertiesDialog actualPropertiesDialog;
    NominalPropertiesDialog nominalPropertiesDialog;
    TrafoParamPropertiesDialog trafoParamPropertiesDialog;
    AboutDialog aboutDialog;
    StationPropertiesDialog stationPropertiesDialog;

    //##########
    //sensor pad
    //##########

    //sensor actions
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionInitialize;
    QAction *actionMeasure;
    QAction *actionAim;
    QAction *actionMove;
    QAction *actionHome;
    QAction *actionChangeMotorState;
    QAction *actionToggleSightOrientation;
    QAction *actionCompensation;
    QList<QPointer<QAction> > selfDefinedActions;

    QPointer<QSignalMapper> customActionMapper;

    //##################
    //status bar widgets
    //##################

    QLabel *label_statusUnitMetric;
    QLabel *label_statusUnitAngular;
    QLabel *label_statusUnitTemperature;
    QLabel *label_statusSensor;

    //#################
    //helper attributes
    //#################

    //ordered list of feature id's that are currently aimed and measured (ALT + F3)
    QList<int> measureFeatures;

};

#endif // MAINWINDOW_H
