#ifndef MEASUREMENTCONFIGURATIONDIALOG_H
#define MEASUREMENTCONFIGURATIONDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QComboBox>
#include <QtSerialPort/QSerialPortInfo>
#include <QtNetwork/QNetworkInterface>
#include <QCloseEvent>
#include <QMap>
#include <QLineEdit>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QShowEvent>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QMenu>

#include "types.h"
#include "console.h"
#include "measurementconfig.h"
#include "modelmanager.h"
#include "measurementconfigurationlistdelegate.h"

using namespace oi;

namespace Ui {
class MeasurementConfigurationDialog;
}

/*!
 * \brief The MeasurementConfigurationDialog class
 */
class MeasurementConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MeasurementConfigurationDialog(QWidget *parent = 0);
    ~MeasurementConfigurationDialog();

    //#############################################
    //set the currently selected measurement config
    //#############################################

    void setMeasurementConfiguration(const MeasurementConfig &mConfig);

signals:

    //#############################################################################
    //signals to inform about measurement config selection (for the active feature)
    //#############################################################################

    void measurementConfigurationChanged(const MeasurementConfig &mConfig);

private slots:

    //#########################
    //actions triggered by user
    //#########################

    //measurement configs list view
    void on_listView_measurementConfigs_clicked(const QModelIndex &index);
    void measurementConfigContextMenuRequested(const QPoint &point);
    void removeSelectedMeasurementConfig();
    void cloneSelectedMeasurementConfig();

    //add new measurement configs
    void on_pushButton_add_clicked();

    //measurement config attributes changed
    void on_lineEdit_numMeas_textChanged(const QString &arg1);
    void on_lineEdit_iterations_textChanged(const QString &arg1);
    void on_comboBox_readingType_currentIndexChanged(const QString &arg1);
    void on_checkBox_twoFace_clicked();
    void on_checkBox_timeDependent_clicked();
    void on_lineEdit_timeInterval_textChanged(const QString &arg1);
    void on_checkBox_distanceDependent_clicked();
    void on_lineEdit_distancInterval_textChanged(const QString &arg1);

    //set measurement config for the active feature
    void on_pushButton_set_clicked();

    //triggered when measurement config name has been edited
    void measurementConfigNameChanged(const MeasurementConfig &mConfig);

    //set measurement config filter
    void on_checkBox_showAll_stateChanged(int arg1);

    void on_lineEdit_stablePoint_min_distance_textChanged(const QString &arg1);

    void on_lineEdit_stablePoint_threshold_textChanged(const QString &arg1);

    void on_checkBox_isStablePoint_clicked();

    void on_lineEdit_stablePoint_thresholdTime_textChanged(const QString &arg1);

    void on_scan_toggled(bool checked);

private:
    Ui::MeasurementConfigurationDialog *ui;

    //##############
    //helper methods
    //##############

    //update GUI elements from the selected measurement config
    void updateGuiFromMeasurementConfig(const MeasurementConfig &mConfig);

    //update the selected measurement config from GUI elements
    void updateMeasurementConfigFromSelection();

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

    void initGUI();
    void initModels();

    //################
    //helper variables
    //################

    MeasurementConfig selectedMeasurementConfig;

};

#endif // MEASUREMENTCONFIGURATIONDIALOG_H
