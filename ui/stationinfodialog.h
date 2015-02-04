#ifndef STATIONINFODIALOG_H
#define STATIONINFODIALOG_H

#include <QDialog>
#include <QShowEvent>
#include <QCloseEvent>
#include <QMap>
#include <QLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtNetwork/QNetworkInterface>
#include <QDesktopWidget>
#include <QMessageBox>
#include "featurewrapper.h"
#include "oifeaturestate.h"

namespace Ui {
class StationInfoDialog;
}

/*!
 * \brief The StationInfoDialog class shows the current station and sensor attributes.
 * You can edit and send the changed ones to the active sensor.
 */
class StationInfoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StationInfoDialog(QWidget *parent = 0);
    ~StationInfoDialog();

    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

signals:
    void connectSensor();
    void disconnectSensor();
    void showStationGeomProperties(FeatureWrapper *fw);

private slots:
    void on_toolButton_ok_clicked();

    void on_toolButton_cancel_clicked();

    void initGUI();

    void getSensorConfiguration();

    void getReadingType();

    void disableAccuracyElements();

    void getSensorParameters();

    void destructDynamicGUI();

    void initSensorConfiguration();

    void initConnection();

    void disableConnectionSettings();

    void on_comboBox_connectiontype_currentIndexChanged(const QString &arg1);

    void on_toolButton_pointProperties_clicked();

private:

    SensorConfiguration sensorConfig;

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

    Ui::StationInfoDialog *ui;
};

#endif // STATIONINFODIALOG_H
