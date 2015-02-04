#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QPluginLoader>
#include <QDebug>
#include <QMessageBox>
#include <QPoint>
#include <QKeyEvent>
#include <QClipboard>
#include <QtAlgorithms>

#include "controller.h"
#include "station.h"
#include "measurementconfigdialog.h"
#include "movementdialog.h"
#include "watchwindow.h"
#include "pluginloader.h"
#include "pluginloaderdialog.h"
#include "createfeaturedialog.h"
#include "sensorplugindialog.h"
#include "functionpluginloader.h"
#include "sensorinfodialog.h"
#include "featuredata.h"
#include "settingsdialog.h"
#include "scalarentitydialog.h"
#include "importnominaldialog.h"
#include "exportnominaldialog.h"
#include "nominaldatadialog.h"
#include "edittrafoparamdialog.h"
#include "oiprojectexchanger.h"
#include "stationinfodialog.h"
#include "realtimedatadialog.h"

#include "featureoverviewdelegate.h"
#include "trafoparamdelegate.h"

#include "oisimulationwidget.h"

#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QDir>
#include <QDesktopServices>

#include <QMetaObject>
#include <QMetaEnum>
#include <QString>
#include <QSignalMapper>

#include <QtXml>

#include "featuretablemodel.h"
#include "featureattributesexchange.h"
#include "guiconfiguration.h"

#include "oifeaturestate.h"

#include "oitoolaction.h"

namespace Ui {
class MainWindow;
}

/*!
 * \brief The MainWindow class that contains the gui elements for the main view of openIndy and all connects to other gui elements.
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

    //active feature states
    void setActiveFeature(const int &featureId);
    void setActiveStation(const int &featureId);
    void setActiveCoordinateSystem(const int &featureId);

    //add or remove features
    void addFeatures(const FeatureAttributes &attributes);
    void removeFeature(const int &featureId);

    //sensor methods
    void startMeasurement();
    void addMeasurement();
    void startMove(Reading *parameter);
    void startAim();
    void startConnect();
    void startDisconnect();
    void startToggleSight();
    void startInitialize();
    void startHome();
    void startCompensation();
    void startChangeMotorState();
    void startCustomAction(const QString &s);

    //recalculation of features
    void recalcAll();
    void recalcActiveFeature();
    void recalcFeature(const int &featureId);
    void recalcTrafoParam(const int &featureId);

    //save or load projects
    void saveProject();
    void loadProject(const QString &projectName, QIODevice *myDevice);

private slots:

    //#########################
    //actions triggered by user
    //#########################

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

    //show create feature dialogs
    void showCreateFeatureDialog(Configuration::FeatureTypes featureType);
    void showScalarEntityDialog(Configuration::FeatureTypes featureType);

    //save and load project actions
    void on_actionSave_as_triggered();
    void on_actionOpen_triggered();

    //table view interactions
    void handleTableViewClicked(const QModelIndex &index);
    void handleTrafoParamClicked(const QModelIndex &index);
    void on_tableView_data_customContextMenuRequested(const QPoint &pos);
    void on_tableView_trafoParam_customContextMenuRequested(const QPoint &pos);
    void featureContextMenu(const QPoint &point);
    void handleViewDoubleClick(int index);

    //open or close dialogs
    void on_actionControl_pad_triggered();
    void on_actionMeasurement_Configuration_triggered();
    void on_actionConsole_triggered();
    void on_actionWatch_window_triggered();
    void on_actionLoad_plugins_triggered();
    void on_actionShow_hide_feature_toolbar_triggered();
    void on_actionSet_instrument_triggered();
    void on_actionSet_function_triggered();
    void openCreateFeatureMConfig(Configuration::FeatureTypes typeOfFeature);
    void on_actionView_settings_triggered();
    void showProperties(bool checked);

    void on_actionActivate_station_triggered();

private:

    Controller control;

    //####################################
    //set up models and connect controller
    //####################################

    void assignModels();
    void connectController();

    //#################
    //init GUI elements
    //#################

    void initFeatureTableView();
    void initDialogs();

    //############################
    //OpenIndy dialogs and widgets
    //############################

    CreateFeatureDialog createFeatureDlg;





    MeasurementConfigDialog mConfigDialog;

    MovementDialog moveDialog;
    PluginLoaderDialog pLoadDialog;
    OiSimulationWidget simulationWidget;


    ScalarEntityDialog sEntityDialog;

    SensorPluginDialog sPluginDialog;
    FunctionPluginLoader fPluginDialog;
    SensorInfoDialog sInfoDialog;
    FeatureData fDataDialog;
    SettingsDialog setUpDialog;
    EditTrafoparamDialog trafoParamDialog;

    NominalDataDialog nominalDialog;

    StationInfoDialog stationDialog;
    RealTimeDataDialog rtDataDialog;

    WatchWindow watchWindow;

    //import and export dialogs
    ImportNominalDialog importNominalDialog;
    ExportNominalDialog exportNominalDialog;




    //---------------------------------------

    void closeEvent(QCloseEvent * event);

    QSignalMapper *signalMapper;

    int selectedFeature;





    //actions
    //create feature
    QLabel *labelWarning;
    QLabel *labelName;
    QLabel *labelCount;
    QComboBox *comboBoxFeatureType;
    QLineEdit *lineEditName;
    QLabel *labelGroup;
    QComboBox *comboBoxGroup;
    QSpinBox *spinBoxNumber;
    QCheckBox *checkBoxActual;
    QCheckBox *checkBoxCommonPoint;
    QAction *actionCreate;
    QLabel *labelNominalSystem;
    QComboBox *comboBoxNominalSystem;
    QAction * actionMConfig;
    QCheckBox *checkBoxNominal;

    //sensor control pad
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
    QLabel *labelSensorControlName;

    //seperators create feature
    QAction *cFsep;
    QAction *cFsep1;
    QAction *cFsep2;
    QAction *cFsep3;
    QAction *cFsep4;
    QAction *cFsep5;
    QAction *cFsep6;
    QAction *cFsep7;
    QAction *cFsep8;

    //seperators control pad
    QAction *cPsep;
    QAction *cPsep1;
    QAction *cPsep2;
    QAction *cPsep3;
    QAction *cPsep4;
    QAction *cPsep5;
    QAction *cPsep6;
    QAction *cPsep7;
    QAction *cPsep8;
    QAction *cPsep9;
    QAction *cPsep10;



signals:

    /*
    //void sendActiveNominalfeature(FeatureWrapper *anf);
    void sendConfig(MeasurementConfig);
    void sendFeatureType(Configuration::FeatureTypes);
    void sendSelectedFeature(int featureIndex); //is emitted when a new active feature was selected by the user
    void sendCommandString(QString);
    void getAvailableFunctions();
    //TODO create a signal which will be emit every time if a new coordinate system was created and connect it to fillCoordSysComboBox()
    void sendActiveCoordSystem(QString coordSys);
    void sendDeleteFeatures(QList<FeatureWrapper*> myFeatures);

    //void startStakeOut(QDomDocument request); //emitted when the user has configured a stake out task
    //void nextStakeOutGeometry(); //emitted when the next geometry has to be selected
*/

    void watchWindowKeyPressed(Qt::Key key);

public slots:
    void showMessageBox(QString title, QString message);
    void showMessageBoxForDecision(QString title, QString message, OiFunctor *func);
    void resetFeatureSelection();
    void availableGroupsChanged(QMap<QString, int>);
    void updateGeometryIcons(QStringList availableGeometries);
    void trafoParamAdded();
    void resizeTableView();
    //void updateModel();

    //void updateCoordSys();

    //database methods
    void createOiToolActions();

    void emitWatchWindowKeyPressed(Qt::Key);

private slots:

    void deleteFeatures(bool checked);


    void ChangeCreateFeatureToolbar(int i);
    void CheckBoxNominalToggled(bool toggled);

    void changedStation();

    //void getActiveCoordSystem(QString coordSys);


    //void setColumnOrder();

    void initializeActions();

    void setupCreateFeature();

    //initiates all standard actions for sensors in the control pad
    void initSensorActions();
    //initiates all tracker specific actions in the control pad
    void initTrackerActions();
    //initiates all total station specifix actions in the control pad
    void initTotalStationActions();

    void setupSensorPad();
    void setupLaserTrackerPad();
    void setupTotalStationPad();

    //void receiveConfig(FeatureWrapper*,MeasurementConfig);
    void createFeature();

    //void setActiveCoordinateSystem();
    void isSensorConnected(bool b);
    void setSensorState(int sState,QString sensorMsg);
    void sensorDisconnected();

    //void fillCoordSysComboBox();



    void setUpStatusBar();










    void defaultCreateFeatureSettings();



    void on_comboBox_groups_currentIndexChanged(const QString &arg1);

    void on_actionShow_help_triggered();


    void on_actionPlugin_manager_triggered();



    void clearCustomWidgets();

    void on_comboBox_activeCoordSystem_currentIndexChanged(const QString &arg1);
    void openStationGeomProperties(FeatureWrapper *fw);

    void on_actionSensor_real_time_data_triggered();

    void on_actionSimulation_triggered();

    void on_treeView_featureOverview_clicked(const QModelIndex &index);

    //close dialogs
    void closeAllOpenDialogs();
    void setDialogsNULL();

    //copy clipboard tableview
    void keyPressEvent(QKeyEvent *e);

    void selectionChangedByKeyboard(QModelIndex newIdx, QModelIndex oldIdx);

    void setMagnifyValues();

    void copyValuesFromView();
	
    //tools
    void showOiToolWidget(OiTool* oiToolWidget);

    //import menu entries
    void on_action_importNominals_triggered();
    void on_action_importMeasurementConfigs_triggered();
    void on_action_importSensorConfigs_triggered();

    //export menu entries
    void on_action_exportNominals_triggered();
    void on_action_exportMeasurementConfigs_triggered();
    void on_action_exportSensorConfigs_triggered();


    void setMeasurementConfig(MeasurementConfig mConfig); //set the measurement config of the active feature
    void setDefaultMeasurementConfig(MeasurementConfig mConfig); //set the default measurement config

    void on_actionMagnify_triggered();

    void closeWatchWindow();

private:
    Ui::MainWindow *ui;

    QList<QAction*> customActions;

    QModelIndexList featuresToDelete;
    bool isTrafoParamSelected;



    //void getDefaultFeatureHeaderOrder();
    //QStringList lastFeatureHeaderOrder;

    Configuration::FeatureTypes currentCreateFeature;

    void checkControlPadVisible();

    void setUpControlPad();
};

#endif // MAINWINDOW_H
