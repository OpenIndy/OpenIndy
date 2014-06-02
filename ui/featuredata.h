#ifndef FEATUREDATA_H
#define FEATUREDATA_H

#include <QDialog>
#include "featurewrapper.h"
#include "observationmodel.h"
#include "readingmodel.h"
#include "function.h"
#include "functionstatistic.h"
#include "matrixmodel.h"
#include "unitconverter.h"

#include <QDebug>
#include <QCloseEvent>
#include <QMap>
#include <QLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtNetwork/QNetworkInterface>
#include <QShowEvent>
#include <QDesktopWidget>

#include "oifeaturestate.h"

namespace Ui {
class FeatureData;
}
/*!
 * \brief The FeatureData class is a gui window, that shows information and attributes about the selected feature.
 */
class FeatureData : public QDialog
{
    Q_OBJECT

public:
    explicit FeatureData(QWidget *parent = 0);
    ~FeatureData();

    ReadingModel *rModel;
    ObservationModel *oModel;
    FunctionStatistic *fModel;
    MatrixModel *qxxModel;
    MatrixModel *sxxModel;

    //void getActiveFeature(FeatureWrapper *activeFeature);

    void showEvent(QShowEvent *event);

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void initGUI();

    void on_comboBox_displayedFunction_currentIndexChanged(const QString &arg1);

    void closeEvent(QCloseEvent *event);

    void getSensorConfiguration();

    void getReadingType();

    void disableAccuracyElements();

    void getSensorParameters();

    void destructDynamicGUI();

    void initSensorConfiguration();

    void on_pushButton_setSensorConfig_clicked();

    void initConnection();

    void disableConnectionSettings();

    void on_comboBox_connectiontype_currentIndexChanged(const QString &arg1);

    void on_pushButton_connectionChanged_clicked();

private:

    bool connectionChanged;

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

    Ui::FeatureData *ui;
};

#endif // FEATUREDATA_H
