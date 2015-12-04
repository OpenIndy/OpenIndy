#ifndef WATCHWINDOW_H
#define WATCHWINDOW_H

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

#include "types.h"
#include "util.h"
#include "station.h"
#include "oijob.h"
#include "featurewrapper.h"
#include "trafocontroller.h"
#include "parameterdisplayconfig.h"
#include "modelmanager.h"

using namespace oi;

/*!
 * \brief The WatchWindowSettings class
 */
class WatchWindowSettings{
public:
    WatchWindowSettings() : digits(2), readingType(eCartesianReading), polarType(0), reference(0){}

    //decimal digits for watch window values
    int digits;

    //reading type for the watch window values
    ReadingTypes readingType;

    //polar type (0 = az,ze,di; 1 = cross,distance)
    int polarType;

    //reference (0 = actual-nominal, 1 = nominal-actual)
    int reference;

    //display values and tolerance
    QMap<QString, double> displayValues;

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
    explicit WatchWindowDialog(QWidget *parent = 0);
    ~WatchWindowDialog();

    //###########################
    //get or set the feature type
    //###########################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

signals:

    //######################################
    //inform about actions triggered by user
    //######################################

    void startMeasure();
    void useLastReading();
    //void doSelfDefinedAction(QString key);
    void keyPressed(Qt::Key key);

    /*void sendSettingsReady(bool);
    void sendCheckBoxReady(bool);
    void sendGUIReady(bool);
    void closeWatchWindow();*/

private slots:

    //#########################
    //actions triggered by user
    //#########################

    void keyPressEvent(QKeyEvent * e);

    //update settings
    void on_spinBox_decimalDigits_valueChanged(int arg1);
    void on_comboBox_readingTypes_currentIndexChanged(const QString &arg1);
    void on_comboBox_polarMode_currentIndexChanged(const QString &arg1);
    void on_radioButton_actnom_clicked();
    void on_radioButton_nomact_clicked();
    void on_checkBox_x_clicked();
    void on_checkBox_y_clicked();
    void on_checkBox_z_clicked();
    void on_checkBox_d3d_clicked();
    void on_checkBox_azimuth_clicked();
    void on_checkBox_zenith_clicked();
    void on_checkBox_distance_clicked();
    void on_lineEdit_x_textChanged(const QString &arg1);
    void on_lineEdit_y_textChanged(const QString &arg1);
    void on_lineEdit_z_textChanged(const QString &arg1);
    void on_lineEdit_d3d_textChanged(const QString &arg1);
    void on_lineEdit_azimuth_textChanged(const QString &arg1);
    void on_lineEdit_zenith_textChanged(const QString &arg1);
    void on_lineEdit_distance_textChanged(const QString &arg1);

    //##################################
    //update watch window display values
    //##################################

    void realTimeReading(const QVariantMap &reading);

private:

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent * event);
    void closeEvent(QCloseEvent *event);

    void initGUI();
    void initModels();

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

private:
    Ui::WatchWindowDialog *ui;

    //#############################
    //current job and active sensor
    //#############################

    QPointer<OiJob> currentJob;

    //save active station here, to be able to disconnect it
    QPointer<Station> activeStation;

    //####################
    //dynamic GUI elements
    //####################

    //watch window values
    QVBoxLayout* masterLayout;
    QMap<QString, QLabel*> streamData;

    //################
    //display settings
    //################

    WatchWindowSettings settings;

    //#################
    //helper attributes
    //#################

    TrafoController trafoController;







/*
 *
 *     //bool isGUIReady;
 *    //bool isSettingsReady;
    //bool isCheckboxReady;
 *
 *    QThread listenerThread;

    double az;
 *
 *     WatchWindowListener *listener;
    int activeReadingType;

    bool checkFeatureValid();

    QString getUnitString(QString attribute);

    bool isVisible;

    void stopStream();
    void startStream();

*/

};

#endif // WATCHWINDOW_H
