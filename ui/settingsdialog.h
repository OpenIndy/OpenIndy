#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QShowEvent>
#include <QCloseEvent>
#include <QLayout>
#include <QCheckBox>
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
signals:
    void modelChanged();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    void initGUI();

    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

    void saveSettings();

    void displayFeatureColumns();

    void displayTrafoParamColumns();

    void getFeatureColumns();

    void destructFeatureColumns();

private:

    QVBoxLayout *featureAttrLayout;
    QVBoxLayout *trafoParamAttrLayout;

    QList<QLayout*> featureLayouts;
    QList<QCheckBox*> featureCheckbox;
    QList<QLayout*> trafoParamLayouts;
    QList<QCheckBox*> trafoParamCheckbox;

    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
