#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QShowEvent>

#include "unitconverter.h"
#include "plugintreeviewmodel.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    void setPluginsModel(PluginTreeViewModel *model);

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    void initGUI();

    void showEvent(QShowEvent * event);

    void saveSettings();

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
