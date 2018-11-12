#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QShowEvent>
#include <QCloseEvent>
#include <QLayout>
#include <QCheckBox>
#include <QStringListModel>
#include <QDesktopWidget>

#include "modelmanager.h"
#include "parameterdisplayconfig.h"
#include "projectconfig.h"

using namespace oi;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    void updateDisplayConfigFromSelection();

signals:

    //########################
    //display settings changed
    //########################

    void setDisplayConfig(const ParameterDisplayConfig &config);

private slots:

    //#########################
    //actions triggered by user
    //#########################

    void on_comboBox_angleType_currentIndexChanged(const QString &arg1);
    void on_comboBox_distanceType_currentIndexChanged(const QString &arg1);
    void on_comboBox_temperatureType_currentIndexChanged(const QString &arg1);
    void on_lineEdit_angleDigits_textChanged(const QString &arg1);
    void on_lineEdit_distanceDigits_textChanged(const QString &arg1);
    void on_lineEdit_temperatureDigits_textChanged(const QString &arg1);

private:

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);

    void initGUI();
    void initModels();

    //##############
    //helper methods
    //##############



private:
    Ui::SettingsDialog *ui;

private slots:
    void on_pushButton_ok_clicked();

    void on_checkBox_sounds_toggled(bool checked);
};

#endif // SETTINGSDIALOG_H
