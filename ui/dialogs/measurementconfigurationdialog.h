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
#include <QDoubleValidator>
#include <float.h>

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
    void saveUserConfig(const MeasurementConfig &mConfig);
    void initialized(); // currently used for testing

private slots:

    //#########################
    //actions triggered by user
    //#########################

    //measurement configs list view
    void on_listView_userConfigs_clicked(const QModelIndex &index);
    void on_listView_projectConfigs_clicked(const QModelIndex &index);

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
    void on_pushButton_set_to_feature_clicked();
    void on_pushButton_save_user_config_clicked();

    //triggered when measurement config name has been edited
    void measurementConfigNameChanged(const MeasurementConfig &mConfig);

    void on_comboBox_MeasurementMode_currentIndexChanged(int index);

    void on_comboBox_MeasurementType_currentIndexChanged(int index);

    void on_tabWidget_currentChanged(int index);

    void enableUIElements(MeasurementTypes type);
    void enableUIElements(const MeasurementConfig &mConfig);

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

    QPointer<QListView> currentListView();
    bool isProjectConfigSelected;
    void on_listView_configs_clicked(const QModelIndex &index);

};

#endif // MEASUREMENTCONFIGURATIONDIALOG_H
