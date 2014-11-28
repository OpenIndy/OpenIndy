#ifndef SENSORPLUGINDIALOG_H
#define SENSORPLUGINDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QComboBox>
#include <QtSerialPort/QSerialPortInfo>
#include <QtNetwork/QNetworkInterface>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>
#include <QCloseEvent>
#include <QMap>
#include <QLineEdit>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QShowEvent>
#include <QDesktopWidget>

#include "configuration.h"
#include "console.h"
#include "sensorconfiguration.h"
#include "sensor.h"
#include "unitconverter.h"
#include "oiconfigstate.h"
#include "oimodelmanager.h"

namespace Ui {
class SensorPluginDialog;
}

class SensorPluginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SensorPluginDialog(QWidget *parent = 0);
    ~SensorPluginDialog();

    //ConnectionConfig *connConfig;


    //Accuracy sigma;



signals:
    void sendSensorType(Configuration::SensorTypes);
    void selectedPlugin(int);
    void sendSensorConfig(SensorConfiguration *sc, bool connect);
    void selectedTempPlugin(int index);

private slots:

    void closeEvent(QCloseEvent *event);

    void showEvent(QShowEvent *event);

    //void receiveTempSensor(Sensor *s);

    void receiveModel(QSqlQueryModel* sqlModel);



    void on_pushButton_cancel_clicked();



    //void initSensorConfig();

    int getCode(QComboBox *cb, QString type);

    QString getLabel(QComboBox *cb, Configuration::ConnectionTypes cT);

    void initGUI();

    void on_comboBox_availableSensorTypes_currentIndexChanged(const QString &arg1);



    void on_comboBox_typeOfConnection_currentIndexChanged(const QString &arg1);

    void disableConnectionSettings();

    void disableAccuracyElements();












    //-------------------------------------------

    void on_comboBox_sensorConfig_currentIndexChanged(const QString &text);
    void handleTableClicked(const QModelIndex &idx);
    void on_pushButton_ok_clicked();

private:
    Ui::SensorPluginDialog *ui;

    int selectedIndex;

    QSqlQueryModel *qSqlModel;

    //Sensor *tmpSensor;

    Configuration::SensorTypes TypeOfSensor;

    //SensorConfiguration *sensorConfig;

    QVBoxLayout *masterAccuracyLayout;
    QVBoxLayout *masterSensorConfigLayout;

    QMap<QString, QLineEdit*> integerParameter;
    QMap<QString, QLabel*> integerParameterLabel;

    QMap<QString, QLineEdit*> doubleParameter;
    QMap<QString, QLabel*> doubleParameterLabel;

    QMap<QString, QComboBox*> stringParameter;
    QMap<QString, QLabel*> stringParameterLabel;

    QMap<QString, QLineEdit*> undefinedSigma;
    QMap<QString, QLabel*> undefinedSigmaLabel;

    QMap<QString, QLayout*> accuracyLayouts;
    QMap<QString, QLayout*> sensorConfigLayouts;




    //----------------------------------------

    void initModels();

    void setSelectedSensorConfig(SensorConfiguration selectedSConfig);
    void setGUIFromSensorConfig();
    void setSensorConfigFromGUI();

    void destructDynamicGUI();

    //create and fill GUI elements from selected sensor config
    void setLabelUnits();
    void setAccuracyFromConfig();
    void setConnectionTypeFromConfig();
    void setConnectionParametersFromConfig();
    void setSensorParametersFromConfig();

    //set selected sensor config from GUI elements
    void setAccuracyFromGUI();
    void setConnectionFromGUI();
    void setSensorParametersFromGUI();

    SensorConfiguration selectedSConfig; //currently selected sensor config in sensor plugin dialog

};

#endif // SENSORPLUGINDIALOG_H
