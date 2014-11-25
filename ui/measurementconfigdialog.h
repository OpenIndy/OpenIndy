#ifndef MEASUREMENTCONFIGDIALOG_H
#define MEASUREMENTCONFIGDIALOG_H

#include <QDialog>
#include "measurementconfig.h"
#include "configuration.h"
#include "featurewrapper.h"
#include "console.h"
#include <QComboBox>
#include <QCloseEvent>
#include <QMessageBox>
#include <QShowEvent>
#include <QDesktopWidget>

#include "oifeaturestate.h"
#include "oiconfigstate.h"

namespace Ui {
class MeasurementConfigDialog;
}

/*!
 * \brief The MeasurementConfigDialog class for defining the measurement configuration
 */
class MeasurementConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MeasurementConfigDialog(QWidget *parent = 0);
    ~MeasurementConfigDialog();

    void setMeasurementConfig(MeasurementConfig mConfig);



signals:
    //void sendConfig(FeatureWrapper*, MeasurementConfig);
    void measurementConfigSelected(MeasurementConfig mConfig);

public slots:


private slots:
    void initGUI();
    void showEvent(QShowEvent *event);

    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();

    void on_comboBox_existingConfigs_currentIndexChanged(const QString &configName);




    void closeEvent(QCloseEvent *event);



    //void receiveConfig(MeasurementConfig);



    //int getCode(QComboBox *cb, QString label);

    //QString getLabel(QComboBox *cb, int code);





private:
    Ui::MeasurementConfigDialog *ui;

    MeasurementConfig mConfig; //currently selected measurement config

};

#endif // MEASUREMENTCONFIGDIALOG_H
