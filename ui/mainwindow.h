#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QString>
#include <QIODevice>
#include <QListView>
#include <QSignalMapper>
#include <QClipboard>
#include <QCloseEvent>
#include <QWindow>
#include <QScreen>
#include <QDialog>
#include <QCompleter>
#include <QList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QGraphicsOpacityEffect>

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
#include "showlicensesdialog.h"
#include "stationpropertiesdialog.h"
#include "exportdialog.h"

#include "featuretabledelegate.h"
#include "trafoparamtabledelegate.h"
#include "bundlestationsmodel.h"

#include "projectconfig.h"
#include "clipboardutil.h"

#include "measurebehaviorlogic.h"

#if ENABLE_SOUND
#include <QSound>
#endif

#if defined(OI_MAIN_LIB)
#  define OI_MAIN_EXPORT Q_DECL_EXPORT
#else
#  define OI_MAIN_EXPORT Q_DECL_IMPORT
#endif

using namespace oi;

namespace Ui {
class MainWindow;
}

/*!
 * \brief The MainWindow class
 */
class OI_MAIN_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void loadProjectFile(QString file);
signals:

    //###################################
    //tasks to be performed by controller
    //###################################

    //add or remove features
    void addFeatures(const FeatureAttributes &attributes);
    void removeFeatures(const QSet<int> &featureIds);

    bool featureCreated(bool);

    //remove observations
    void removeObservations(const int &featureId);
    void removeAllObservations();

    void enableObservations(const int &featureId);
    void disableObservations(const int &featureId);

    //remove the sensor of the active station
    void removeActiveStationSensor();

    //sensor configs edited
    void sensorConfigurationsEdited(const SensorConfigurationManager &manager);

    //set sensor configuration for active sensor
    void sensorConfigurationChanged(const SensorConfiguration &sConfig, bool connectSensor);

    //set measurement configuration for active feature
    void measurementConfigurationChanged(const MeasurementConfig &mConfig);
    void saveUserConfig(const MeasurementConfig &mConfig);

    //recalculation of features
    void recalcAll();
    void recalcActiveFeature();
    void recalcFeature(const int &featureId);
    void recalcTrafoParam(const int &featureId);

    //import or export features
    void importNominals(const ExchangeParams &params);
    void exportFeatures(const ExchangeParams &params);

    //add or remove bundle system
    void addBundleSystem();
    void removeBundleSystem(const int &bundleId);

    //load or calculate bundle
    void updateBundleAdjustment(const int &bundleId, const QJsonObject &param);
    void loadBundleTemplate(const int &bundleId, const QJsonObject &bundleTemplate);
    void runBundle(const int &bundleId);

    //save or load projects
    void saveProject();
    void saveProject(const QString &fileName);
    void loadProject(const QString &projectName, const QPointer<QFileDevice> &device);

    //log messages
    void log(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

    void createTemplateFromJob();

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
    void sensorActionStarted(const QString &msg, const SensorAction sensorAction = SensorAction::eSensorActionUndefind, const bool enableFinishButton = false);
    void sensorActionFinished(const bool &success, const QString &msg, const SensorAction sensorAction = SensorAction::eSensorActionUndefind);
    void measurementCompleted();
    void measurementDone(bool success);

    //display messages
    int showMessageBox(const QString &msg, const MessageTypes &msgType);
    void showStatusMessage(const QString &msg, const MessageTypes &msgType);
    void showStatusSensor(const SensorStatus &code, const QString &msg);

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
    void on_actionCreate_cylinder_triggered();
    void on_actionCreate_circle_triggered();

    //plugin actions
    void on_actionLoad_plugins_triggered();
    void on_actionPlugin_manager_triggered();

    //import export actions
    void on_actionimport_triggered();
    void on_actionexport_triggered();

    //feature table view interactions
    void on_tableView_features_clicked(const QModelIndex &index);
    void on_tableView_features_doubleClicked(const QModelIndex &index);
    void tableViewFeaturesSelectionChangedByKeyboard(const QModelIndex &selected, const QModelIndex &deselected);
    void on_tableView_features_customContextMenuRequested(const QPoint &pos);
    void on_tableView_trafoParams_clicked(const QModelIndex &index);
    void on_tableView_trafoParams_doubleClicked(const QModelIndex &index);
    void tableViewTrafoParamsSelectionChangedByKeyboard(const QModelIndex &selected, const QModelIndex &deselected);
    void on_tableView_trafoParams_customContextMenuRequested(const QPoint &pos);
    void tableViewBundleParamsSelectionChangedByKeyboard(const QModelIndex &selected, const QModelIndex &deselected);
    void on_tableView_bundleParameter_clicked(const QModelIndex &index);

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
    void on_actionSave_as_template_triggered();

    //close OpenIndy
    void on_actionClose_triggered();
    void closeEvent(QCloseEvent *event);

    //show measurement config dialog
    void on_actionMeasurement_Configuration_triggered();

    //activate a station
    void on_actionActivate_station_triggered();

    //show settings dialog
    void on_actionView_settings_triggered();

    //resize table views
    void resizeTableView();

    //remove observations
    void on_actionShow_Licenses_triggered();
    void removeObservationOfActiveFeature();

    //show about dialog
    void on_actionAbout_OpenIndy_triggered();

    //add or remove bundle system
    void on_pushButton_addBundle_clicked();
    void on_pushButton_removeBundle_clicked();

    //load or calculate bundle
    void on_action_RunBundle_triggered();
    void on_pushButton_loadBundleTemplate_clicked();

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
    void copyDifferencesToClipboard();
    void pasteFromClipboard();

    //set up status bar
    void updateStatusBar();

    //update bundle view
    void bundleSelectionChanged();
    void bundleSettingsChanged();

    void on_actionShortcut_import_triggered();

    //trafo Param create...already exist use
    void createMessageBoxTrafoParamWarning();

    void on_actiondifferences_triggered();

    void on_lineEdit_tolerance_returnPressed();

    void on_tableView_FeatureDifferences_customContextMenuRequested(const QPoint &pos);

    void enableObservationsOfActiveFeature();
    void disableObservationsOfActiveFeature();

    void autoSaveProject();

    void on_actionNew_watch_window_triggered();

    void on_actionWatch_window_nearest_nominal_triggered();

    void updateCompleter();

    void on_lineEdit_searchFeatureName_returnPressed();

    void on_pushButton_showNextFoundFeature_clicked();

    void on_comboBox_sortBy_currentIndexChanged(int index);

    void measureBehaviorLogicFinished();

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
    void connectBundleView();
    void assignModels();

    //##################################
    //methods to initialize GUI elements
    //##################################

    void initFeatureTableViews();
    void initSensorPad();
    void initToolMenus();
    void initFilterComboBoxes();
    void initStatusBar();
    void initBundleView();
    void initFilterToolBar();

    //##############################
    //methods to update GUI elements
    //##############################

    //sensor type
    void activeSensorTypeChanged(const SensorTypes &type, const QList<SensorFunctions> &supportedActions, const QStringList &selfDefinedActions);

    //magnify
    void updateMagnifyWindow(const QPointer<FeatureWrapper> &feature);

    //filter
    void updateGroupFilterSize();
    void updateSystemFilterSize();
    void updateActualNominalFilterSize();

    //bundle view
    void resetBundleView();

    //save project help function
    void saveProjectAs(bool asTemplate = false);

    //load default bundle plugin
    void loadDefaultBundlePlugIn(int bundleID);

    //go automatically to next feature
    void autoSwitchToNextFeature(bool sucessMeasure);

    void startAutoSave();

    int saveProjectIfDigestChanged();

    //############################
    //OpenIndy dialogs and widgets
    //############################

    //dialogs
    CreateFeatureDialog createFeatureDialog;
    PluginLoaderDialog pluginLoaderDialog;
    ImportNominalDialog importNominalDialog;
    ExportDialog exportDialog;
    LoadingDialog loadingDialog;
    FeatureFunctionsDialog featureFunctionsDialog;
    SensorConfigurationDialog sensorConfigurationDialog;
    MoveSensorDialog moveSensorDialog;
    QMap<SensorAction, QPointer<SensorTaskInfoDialog>> sensorTaskInfoDialogs;
    PluginManagerDialog pluginManagerDialog;
    QMap<QVariant, QPointer<WatchWindowDialog> > watchWindowDialogs;
    MeasurementConfigurationDialog measurementConfigDialog;
    SettingsDialog settingsDialog;
    ActualPropertiesDialog actualPropertiesDialog;
    NominalPropertiesDialog nominalPropertiesDialog;
    TrafoParamPropertiesDialog trafoParamPropertiesDialog;
    AboutDialog aboutDialog;
    ShowLicensesDialog showLicensesDialog;
    StationPropertiesDialog stationPropertiesDialog;

    //widget with scalar input parameters
    ScalarParameterWidget *bundleParameterWidget;

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
    QAction *actionSearch;
    QList<QPointer<QAction> > selfDefinedActions;

    QPointer<QSignalMapper> customActionMapper;

    //##################
    //status bar widgets
    //##################

    QLabel *label_statusUnitMetric;
    QLabel *label_statusUnitAngular;
    QLabel *label_statusUnitTemperature;
    QLabel *label_statusSensor;
    QPropertyAnimation *animation_label_statusSensor;

    //######
    //models
    //######

    QPointer<BundleStationsModel> bundleStationsModel;
    QPointer<BundleGeometriesModel> bundleGeometriesModel;

    //#################
    //helper attributes
    //#################

    void enableOrDisableObservationsOfActiveFeature(bool);

    void showCentered(QDialog &dialog);

    void openWatchWindow(WatchWindowBehavior);

    void showFoundFeature(int index);
    QList<QPointer<FeatureWrapper> > foundFeatures;
    int showFoundFeatureIndex;

    ClipBoardUtil clipBoardUtil;

    MeasureBehaviorLogic measureBehaviorLogic;

    void measureBehaviorLogicStarted();
};

#endif // MAINWINDOW_H
