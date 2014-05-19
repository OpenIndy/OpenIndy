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

#include "configuration.h"
#include "console.h"
#include "sensorconfiguration.h"
#include "sensor.h"
#include "unitconverter.h"

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

    void receiveTempSensor(Sensor *s);

    void receiveModel(QSqlQueryModel* sqlModel);

    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    void handleTableClicked(const QModelIndex &);

    void initSensorConfig();

    int getCode(QComboBox *cb, QString type);

    QString getLabel(QComboBox *cb, Configuration::ConnectionTypes cT);

    void initGUI();

    void on_comboBox_availableSensorTypes_currentIndexChanged(const QString &arg1);

    void getConnectionType();

    void on_comboBox_typeOfConnection_currentIndexChanged(const QString &arg1);

    void disableConnectionSettings();

    void disableAccuracyElements();

    void getReadingType();

    void destructDynamicGUI();

    void getSensorParameters();

    void setLabelUnits();

private:
    Ui::SensorPluginDialog *ui;

    int selectedIndex;

    QSqlQueryModel *qSqlModel;

    Sensor *tmpSensor;

    Configuration::SensorTypes TypeOfSensor;

    SensorConfiguration *sensorConfig;

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

};

#endif // SENSORPLUGINDIALOG_H
