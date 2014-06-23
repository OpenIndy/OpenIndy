#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QPluginLoader>
#include <QDebug>
#include <QMessageBox>
#include <QPoint>

#include "controller.h"
#include "station.h"
#include "measurementconfigdialog.h"
#include "movementdialog.h"
#include "watchwindow.h"
#include "pluginloader.h"
#include "pluginloaderdialog.h"
#include "createfeature.h"
#include "sensorplugindialog.h"
#include "functionpluginloader.h"
#include "sensorinfodialog.h"
#include "featuredata.h"
#include "settingsdialog.h"
#include "scalarentitydialog.h"
#include "importnominalgeometrydialog.h"
#include "nominaldatadialog.h"
#include "edittrafoparamdialog.h"
#include "oiprojectexchanger.h"
#include "stationinfodialog.h"
#include "realtimedatadialog.h"

#include "featureoverviewdelegate.h"
#include "trafoparamdelegate.h"

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

#include "tablemodel.h"
#include "featureattributesexchange.h"
#include "guiconfiguration.h"

#include "oifeaturestate.h"

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

    QSignalMapper *signalMapper;

    int selectedFeature;

    Controller control;

    MeasurementConfigDialog mConfigDialog;
    MovementDialog moveDialog;
    PluginLoaderDialog pLoadDialog;

    CreateFeature *cFeatureDialog;
    ScalarEntityDialog *sEntityDialog;

    SensorPluginDialog sPluginDialog;
    FunctionPluginLoader fPluginDialog;
    SensorInfoDialog sInfoDialog;
    FeatureData fDataDialog;
    SettingsDialog setUpDialog;
    EditTrafoparamDialog trafoParamDialog;

    importNominalGeometryDialog importNominalDialog;
    NominalDataDialog nominalDialog;

    StationInfoDialog stationDialog;
    RealTimeDataDialog rtDataDialog;

    WatchWindow *watchWindow;

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

    //void sendActiveNominalfeature(FeatureWrapper *anf);
    void sendConfig(MeasurementConfig);
    void sendFeatureType(Configuration::FeatureTypes);
    void sendSelectedFeature(int featureIndex); //is emitted when a new active feature was selected by the user
    void sendCommandString(QString);
    void getAvailableFunctions();
    //TODO create a signal which will be emit every time if a new coordinate system was created and connect it to fillCoordSysComboBox()
    void sendActiveCoordSystem(QString coordSys);
    void sendDeleteFeatures(QList<FeatureWrapper*> myFeatures);

public slots:
    void showMessageBox(QString title, QString message);
    void showMessageBoxForDecision(QString title, QString message, OiFunctor *func);
    void resetFeatureSelection();
    void availableGroupsChanged(QMap<QString, int>);
    void updateGeometryIcons(QStringList availableGeometries);
    //void updateModel();

    //void updateCoordSys();

private slots:
    void featureContextMenu(const QPoint &point);
    void deleteFeatures(bool checked);
    void showProperties(bool checked);

    void ChangeCreateFeatureToolbar(int i);
    void CheckBoxNominalToggled(bool toggled);

    void changedStation();

    //void getActiveCoordSystem(QString coordSys);
    void handleTableViewClicked(const QModelIndex &);
    void handleTrafoParamClicked(const QModelIndex &);

    void initializeActions();

    void on_lineEdit_inputConsole_returnPressed();
    void setupCreateFeature();
    void setupLaserTrackerPad();
    void setupTotalStationPad();
    void receiveConfig(FeatureWrapper*,MeasurementConfig);
    void createFeature();

    //void fillCoordSysComboBox();

    void on_actionControl_pad_triggered();

    void on_actionMeasurement_Configuration_triggered();

    void on_actionConsole_triggered();

    void on_actionWatch_window_triggered();

    void on_actionLoad_plugins_triggered();

    void on_actionShow_hide_feature_toolbar_triggered();

    void on_actionCreate_point_triggered();

    void on_actionCreate_line_triggered();

    void on_actionCreate_plane_triggered();

    void on_actionCreate_sphere_triggered();

    void on_actionCreate_station_triggered();

    void on_actionCreate_coordinatesystem_triggered();

    void on_actionSet_instrument_triggered();

    void on_actionSet_function_triggered();

    void openCreateFeatureMConfig();

    void setUpStatusBar();

    void on_actionView_settings_triggered();

    void on_actionCreate_scalar_entity_triggered();

    void on_actionNominal_geometry_triggered();

    void on_actionCreate_trafoParam_triggered();

    void on_actionSave_as_triggered();

    void on_actionOpen_triggered();

    void on_actionCreate_cone_triggered();

    void on_actionCreate_cylinder_triggered();

    void on_actionCreate_ellipsoid_triggered();

    void on_actionCreate_hyperboloid_triggered();

    void on_actionCreate_paraboloid_triggered();

    void on_actionCreate_nurbs_triggered();

    void on_actionCreate_pointcloud_triggered();

    void on_actionCreate_circle_triggered();

    void on_actionActivate_station_triggered();


    void defaultCreateFeatureSettings();

    void on_tableView_data_customContextMenuRequested(const QPoint &pos);

    void on_tableView_trafoParam_customContextMenuRequested(const QPoint &pos);

    void on_comboBox_groups_currentIndexChanged(const QString &arg1);

    void on_actionShow_help_triggered();


    void on_actionPlugin_manager_triggered();

    void showCreateFeatureDialog(Configuration::FeatureTypes featureType);
    void showScalarEntityDialog(Configuration::FeatureTypes featureType);

    void clearCustomWidgets();

    void on_comboBox_activeCoordSystem_currentIndexChanged(const QString &arg1);
    void openStationGeomProperties(FeatureWrapper *fw);

    void on_actionSensor_real_time_data_triggered();

private:
    Ui::MainWindow *ui;

    QList<QAction*> customActions;

    QModelIndexList featuresToDelete;
    bool isTrafoParamSelected;

    void setConnects();
    void setModels();
};

#endif // MAINWINDOW_H
