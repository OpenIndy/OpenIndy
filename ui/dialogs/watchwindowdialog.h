#ifndef WATCHWINDOW_H
#define WATCHWINDOW_H

#include <atomic>
#include <QDialog>
#include <QKeyEvent>
#include <QMap>
#include <QVariantMap>
#include <QLabel>
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QPointer>
#include <QDesktopWidget>
#include <functional>

#include "types.h"
#include "util.h"
#include "station.h"
#include "oijob.h"
#include "featurewrapper.h"
#include "trafocontroller.h"
#include "parameterdisplayconfig.h"
#include "modelmanager.h"
#include "feature.h"
#include "watchwindowutil.h"

enum DisplayAttributes{
    eName = 0,
    eX,
    eY,
    eZ,
    eD3D,
    eNotDeclared // should be the last element!
};

enum WatchWindowBehavior{
    eShowAlwaysActiveFeature = 0, // show always the active feature
    eShowCurrentSelectedFeature,  // open new watch window for current selected feature
    eShowNearestNominal
};

using namespace oi;

/*!
 * \brief The WatchWindowSettings class
 */
class WatchWindowSettings{
public:
    WatchWindowSettings() : digits(2), readingType(eCartesianReading), showLastMeasurement(true){}

    //decimal digits for watch window values
    int digits;

    //reading type for the watch window values
    ReadingTypes readingType;

    //display values and tolerance
    QMap<DisplayAttributes, double> displayValues;

    bool showLastMeasurement;
};


namespace Ui {
class WatchWindowDialog;
}


/*!
 * \brief The WatchWindowDialog class
 * Handles displaying of differences between active feature (sensor) and its nominal value.
 */
class WatchWindowDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit WatchWindowDialog(WatchWindowBehavior behavior, QPointer<OiJob> job, QList<QPointer<FeatureWrapper> > features, QWidget *parent = 0);
    ~WatchWindowDialog();

signals:

    //######################################
    //inform about actions triggered by user
    //######################################

    //####################
    //start sensor actions
    //####################

    void startMeasure();
    void useLastReading();
    void startStreaming(ReadingTypes streamFormat);
    void stopStreaming();

private slots:

    //#########################
    //actions triggered by user
    //#########################

    //update settings
    void on_spinBox_decimalDigits_valueChanged(int arg1);
    void on_checkBox_x_clicked();
    void on_checkBox_y_clicked();
    void on_checkBox_z_clicked();
    void on_checkBox_d3d_clicked();
    void on_lineEdit_tolerance_x_textChanged(const QString &arg1);
    void on_lineEdit_tolerance_y_textChanged(const QString &arg1);
    void on_lineEdit_tolerance_z_textChanged(const QString &arg1);
    void on_lineEdit_tolerance_d3d_textChanged(const QString &arg1);

    //##################################
    //update watch window display values
    //##################################

    void realTimeReading(const QVariantMap &reading);

    //switch tab and update geometries
    void on_toolBox_currentChanged(int index);

    //hides x, y ,z, d3D if no current reading available
    void clearWatchWindow();

    void on_checkBox_showLastMeasurement_clicked();

private:

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent * event);
    void closeEvent(QCloseEvent *event);

    void initGUI();

    //##############################
    //connect job and active station
    //##############################

    void connectSensor();
    void connectJob();

    //#############################################
    //helper methods to display watch window values
    //#############################################

    void setUpCartesianWatchWindow(const QVariantMap &reading);

    void getDefaultSettings();
    void resizeWatchWindowValues();

    Ui::WatchWindowDialog *ui;

    //#############################
    //current job and active sensor
    //#############################
    WatchWindowBehavior behavior;
    QPointer<OiJob> currentJob;
    QList<QPointer<FeatureWrapper> > features;
    //save active station here, to be able to disconnect it
    QPointer<Station> activeStation;

    //####################
    //dynamic GUI elements
    //####################

    //watch window values
    QVBoxLayout* masterLayout;
    QMap<DisplayAttributes, QLabel*> streamData;
    QMap<DisplayAttributes, int> masterLayoutIndex;

    //################
    //display settings
    //################

    WatchWindowSettings settings;

    //#################
    //helper attributes
    //#################

    TrafoController trafoController;

    //###############################################
    //helper attribute to rescale the labels and text
    //###############################################

    bool lablesRescaled;
    int oldWindowHeight;
    int oldWindowWidth;

    //set to "true" if watch window was currently updateded
    std::atomic<bool> watchWindowUpdated;

    void addLabel(DisplayAttributes att,  QFont f);
    void setDisplayValue(DisplayAttributes attr, QString name, std::function<double()> v);
    QString getNameLabel(QPointer<FeatureWrapper> feature);

    QPointer<FeatureWrapper> getFeature(OiVec trackerXYZ);
    void setVisibility();

    WatchWindowUtil util;

};

#endif // WATCHWINDOW_H
