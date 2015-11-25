#ifndef STATIONPROPERTIESDIALOG_H
#define STATIONPROPERTIESDIALOG_H

#include <QDialog>
#include <QDesktopWidget>
#include <QShowEvent>
#include <QCloseEvent>
#include <QPointer>
#include <QAbstractButton>
#include <QAbstractItemView>
#include <QClipboard>

#include "modelmanager.h"
#include "sensoraccuracydelegate.h"
#include "sensorparametersdelegate.h"

#include "sensorconfiguration.h"

namespace Ui {
class StationPropertiesDialog;
}

/*!
 * \brief The StationPropertiesDialog class
 */
class StationPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StationPropertiesDialog(QWidget *parent = 0);
    ~StationPropertiesDialog();

    //##############################
    //get or set station information
    //##############################

    //sensor config
    const SensorConfiguration &getSensorConfiguration() const;
    void setSensorConfiguration(const SensorConfiguration &sConfig);

    //active station state
    bool getIsActiveStation() const;
    void setIsActiveStation(bool isActiveStation);

signals:

    //##########################
    //signals to trigger actions
    //##########################

    //select another sensor config for the active station
    void openSensorConfigurationDialog();

    //save changes made to the sensor config
    void sensorConfigurationChanged(const SensorConfiguration &sConfig);

private slots:

    //#########################
    //actions triggered by user
    //#########################

    //ok or cancel
    void on_buttonBox_clicked(QAbstractButton *button);

    //open sensor config dialog
    void on_pushButton_sensorConfig1_clicked();

    //key pressed
    void keyPressEvent(QKeyEvent * e);

    //sensor config changed
    void updateSensorAccuracy(const SensorConfiguration &sConfig);
    void updateSensorParameters(const SensorConfiguration &sConfig);

private:
    Ui::StationPropertiesDialog *ui;

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);

    void initGUI();
    void initModels();

    //##############
    //helper methods
    //##############

    //copy values from and to clipboard
    void copyToClipboard();
    void pasteFromClipboard();

    //######
    //models
    //######

    QPointer<SensorAccuracyModel> accuracyModel;
    QPointer<SensorParametersModel> sensorParametersModel;

    //#################
    //helper attributes
    //#################

    SensorConfiguration sConfig;
    bool isActiveStation;

};

#endif // STATIONPROPERTIESDIALOG_H
