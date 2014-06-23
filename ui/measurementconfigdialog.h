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

    MeasurementConfig mConfig;

signals:
    void sendConfig(FeatureWrapper*, MeasurementConfig);

public slots:
    void showEvent(QShowEvent *event);

private slots:

    void closeEvent(QCloseEvent *event);

    void initGUI();

    void receiveConfig(MeasurementConfig);

    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    int getCode(QComboBox *cb, QString label);

    QString getLabel(QComboBox *cb, int code);



private:
    Ui::MeasurementConfigDialog *ui;
};

#endif // MEASUREMENTCONFIGDIALOG_H
