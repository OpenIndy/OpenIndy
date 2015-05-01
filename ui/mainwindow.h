#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QString>
#include <QIODevice>
#include <QListView>

#include "controller.h"
#include "featureattributes.h"
#include "modelmanager.h"
#include "console.h"
#include "exchangeParams.h"

#include "createfeaturedialog.h"
#include "pluginloaderdialog.h"
#include "importnominaldialog.h"
#include "loadingdialog.h"

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
    void startMove(const QPointer<Reading> &parameter);
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

    //import or export features
    void importNominals(const ExchangeParams &params);

    //save or load projects
    void saveProject();
    void loadProject(const QString &projectName, const QPointer<QIODevice> &device);

private slots:

    //#############################
    //tasks triggered by controller
    //#############################

    void importNominalsStarted();
    void importNominalsFinished(const bool &success);

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
    void on_actionCreate_torus_triggered();
    void on_actionCreate_slotted_hole_triggered();
    void on_actionCreate_ellipse_triggered();

    //plugin actions
    void on_actionLoad_plugins_triggered();

    //import export actions
    void on_action_importNominals_triggered();

private:
    Ui::MainWindow *ui;

private:

    Controller control;

    //#################################
    //set up models and connect dialogs
    //#################################

    void connectController();
    void connectDialogs();
    void assignModels();

    //#################
    //init GUI elements
    //#################

    void initFeatureTableViews();

    //############################
    //OpenIndy dialogs and widgets
    //############################

    CreateFeatureDialog createFeatureDialog;
    PluginLoaderDialog pluginLoaderDialog;
    ImportNominalDialog importNominalDialog;
    LoadingDialog loadingDialog;

};

#endif // MAINWINDOW_H
