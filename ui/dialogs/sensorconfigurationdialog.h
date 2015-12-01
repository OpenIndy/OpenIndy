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
#include "sensorconfiguration.h"

#include "modelmanager.h"
#include "sensorconfigurationlistdelegate.h"
#include "sensoraccuracydelegate.h"
#include "sensorparametersdelegate.h"

using namespace oi;

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

    //#########
    //messaging
    //#########

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

    //#############################################
    //signals to inform about sensor config changes
    //#############################################

    //sensor configs edited
    void sensorConfigurationsEdited(const SensorConfigurationManager &manager);

    //set and/or connect a sensor config
    void setSensorConfig(const SensorConfiguration &sConfig);

private slots:

    //#########################
    //actions triggered by user
    //#########################

    //save, set or cancel
    void on_pushButton_save_clicked();
    void on_pushButton_setAndConnect_clicked();
    void on_pushButton_cancel_clicked();

    //add or remove sensor configs
    void on_pushButton_remove_clicked();
    void on_pushButton_add_clicked();

    //#######################
    //sensor config callbacks
    //#######################

    //active sensor config changed
    void activeSensorConfigChanged();

    //sensor config renamed
    void sensorConfigRenamed();

    //sensor configs added or removed
    void sensorConfigsChanged();

    //sensor config definition changed
    void sensorAccuracyChanged(const SensorConfiguration &sConfig);
    void sensorParametersChanged(const SensorConfiguration &sConfig);
    void sensorConnectionChanged();
    void sensorPluginChanged();

    //sensor type changed
    void on_comboBox_availableSensorTypes_currentIndexChanged(const QString &arg1);

    //#########################
    //selection model callbacks
    //#########################

    void configSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void sensorPluginSelectionChanged(const QModelIndex &index);

private:
    Ui::SensorConfigurationDialog *ui;

    //##############
    //helper methods
    //##############

    void updateGuiFromSensorConfig(const SensorConfiguration &sConfig);

    void toggleVisibility(const SensorConfiguration &sConfig);
    void toggleCommandVisibility();

    void setChangesMade(bool changesMade);

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

    void initGUI();
    void initModels();
    void connectModels();

    //#################
    //helper attributes
    //#################

    //indicates wether there are unsaved changes
    bool changesMade;

    //######
    //models
    //######

    //sensor models
    QPointer<SensorConfigurationModel> sensorConfigModel;
    QPointer<SensorConfigurationProxyModel> sensorConfigProxyModel;
    QPointer<SensorAccuracyModel> sensorAccuracyModel;
    QPointer<SensorParametersModel> sensorParametersModel;

};

#endif // SENSORCONFIGURATIONDIALOG_H
