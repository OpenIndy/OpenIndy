#ifndef SENSORCONFIGURATIONDIALOG_H
#define SENSORCONFIGURATIONDIALOG_H

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
#include "sensorconfiguration.h"
#include "modelmanager.h"
#include "scalarparameterwidget.h"
#include "sensorconfigurationlistdelegate.h"

namespace Ui {
class SensorConfigurationDialog;
}

/*!
 * \brief The SensorConfigurationDialog class
 */
class SensorConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SensorConfigurationDialog(QWidget *parent = 0);
    ~SensorConfigurationDialog();

signals:

    //###############################################
    //signals to inform about sensor config selection
    //###############################################

    void setSensorConfiguration(const QString &name);

private slots:

    //#################################
    //scalar parameter changed callback
    //#################################

    void scalarParameterChanged();

    //#########################
    //actions triggered by user
    //#########################

    //sensor configs list view
    void on_listView_sensorConfigs_clicked(const QModelIndex &index);
    void sensorConfigContextMenuRequested(const QPoint &point);
    void removeSelectedSensorConfig();

    //add new sensor config
    void on_pushButton_add_clicked();

    //sensor types combo box
    void on_comboBox_availableSensorTypes_currentIndexChanged(const QString &arg1);

    //set or cancel
    void on_pushButton_cancel_clicked();
    void on_pushButton_set_clicked();

    //sensor config parameters changed

    //sensor plugin changed
    void on_tableView_sensorPlugins_clicked(const QModelIndex &index);
    void on_comboBox_typeOfConnection_currentIndexChanged(const QString &arg1);
    void on_comboBox_ip_currentTextChanged(const QString &arg1);
    void on_lineEdit_port_textChanged(const QString &arg1);
    void on_comboBox_comPort_currentIndexChanged(int index);
    void on_comboBox_baudrate_currentIndexChanged(int index);
    void on_comboBox_databits_currentIndexChanged(int index);
    void on_comboBox_flowcontrol_currentIndexChanged(int index);
    void on_comboBox_parity_currentIndexChanged(int index);
    void on_comboBox_stopbits_currentIndexChanged(int index);

private:
    Ui::SensorConfigurationDialog *ui;

    //#################
    //helper attributes
    //#################

    //widget with scalar input parameters
    ScalarParameterWidget *scalarParameterWidget;

    //##############
    //helper methods
    //##############

    //update GUI elements from the selected sensor config
    void updateConnectionConfigFromSensorConfig(const ConnectionConfig &cConfig);
    void updateAccuracyFromSensorConfig(const Accuracy &accuracy);
    void updateScalarParametersFromSensorConfig(const QMap<QString, int> &intParams,
                                                const QMap<QString, double> &doubleParams,
                                                const QMap<QString, QString> &stringParams,
                                                const QMultiMap<QString, QString> &availableStringOptions);

    //update the selected sensor config from GUI elements
    void updateSensorConfigFromSelection();

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);

    void initGUI();
    void initModels();

};

#endif // SENSORCONFIGURATIONDIALOG_H
