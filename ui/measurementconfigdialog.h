#ifndef MEASUREMENTCONFIGDIALOG_H
#define MEASUREMENTCONFIGDIALOG_H

#include <QDialog>
#include "measurementconfig.h"
#include "configuration.h"
#include "featurewrapper.h"
#include <QComboBox>
#include <QCloseEvent>
#include <QMessageBox>

namespace Ui {
class MeasurementConfigDialog;
}

class MeasurementConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MeasurementConfigDialog(QWidget *parent = 0);
    ~MeasurementConfigDialog();

    Station *myStation;

    MeasurementConfig *mConfig;
    //Configuration config;

    FeatureWrapper *activeFeature;

signals:
    void sendConfig(FeatureWrapper*, MeasurementConfig*);

public slots:

    void setStation(Station *s);
    
private slots:

    void closeEvent(QCloseEvent *event);

    void resetActiveFeature();

    void initGUI();

    void receiveConfig(MeasurementConfig*);

    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    int getCode(QComboBox *cb, QString label);

    QString getLabel(QComboBox *cb, int code);



private:
    Ui::MeasurementConfigDialog *ui;
};

#endif // MEASUREMENTCONFIGDIALOG_H
