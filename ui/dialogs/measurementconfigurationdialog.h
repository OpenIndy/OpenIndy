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

#if defined(OI_MAIN_LIB)
#  define OI_MAIN_EXPORT Q_DECL_EXPORT
#else
#  define OI_MAIN_EXPORT Q_DECL_IMPORT
#endif

using namespace oi;

namespace Ui {
class MeasurementConfigurationDialog;
}

/*!
 * \brief The MeasurementConfigurationDialog class
 */
class OI_MAIN_EXPORT MeasurementConfigurationDialog : public QDialog
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
    void initialized(); // currently used for testing

private slots:

    //#########################
    //actions triggered by user
    //#########################

    //measurement configs list view
    void on_listView_userConfigs_clicked(const QModelIndex &index);
    void measurementConfigContextMenuRequested(const QPoint &point);
    void removeSelectedMeasurementConfig();
    void cloneSelectedMeasurementConfig();

    //add new measurement configs
    void on_pushButton_add_clicked();

    //measurement config attributes changed
    void on_lineEdit_maxObservations_textChanged(const QString &arg1);
    void on_checkBox_twoFace_clicked();
    void on_lineEdit_timeInterval_textChanged(const QString &arg1);
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

    void enableUIElements(const MeasurementTypes &type);

    void on_comboBox_MeasurementMode_currentIndexChanged(int index);

    void on_comboBox_MeasurementType_currentIndexChanged(int index);

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
