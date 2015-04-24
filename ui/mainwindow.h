#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QString>
#include <QIODevice>

#include "controller.h"
#include "featureattributes.h"
#include "modelmanager.h"

#include "createfeaturedialog.h"

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

    //save or load projects
    void saveProject();
    void loadProject(const QString &projectName, const QPointer<QIODevice> &device);

private slots:

    //#########################
    //actions triggered by user
    //#########################



private:
    Ui::MainWindow *ui;

private:

    Controller control;

    //#################################
    //set up models and connect dialogs
    //#################################

    void connectController();
    void assignModels();

    //#################
    //init GUI elements
    //#################

    void initFeatureTableViews();

    //############################
    //OpenIndy dialogs and widgets
    //############################

    CreateFeatureDialog createFeatureDialog;

};

#endif // MAINWINDOW_H
