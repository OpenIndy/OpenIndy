#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QShowEvent>
#include <QCloseEvent>
#include <QLayout>
#include <QCheckBox>
#include <QStringListModel>
#include <QDesktopWidget>
#include "unitconverter.h"
#include "plugintreeviewmodel.h"
#include "guiconfiguration.h"
#include "plugininfowidget.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    QStringList allFeatureAttributes;
    QStringList displayedFeatureAttributes;
    QStringListModel *m_featureAttributes;
    QStringListModel *m_displayedFeatureAttributes;

    QStringList allTrafoParamAttributes;
    QStringList displayedTrafoParamAttributes;
    QStringListModel *m_TrafoParamAttributes;
    QStringListModel *m_displayedTrafoParamAttributes;

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

    void getTrafoParamColumns();

    void on_toolButton_addFeatureAttribute_clicked();

    void on_toolButton_removeFeatureAttribute_clicked();

    void on_toolButton_addTrafoParamAttribute_clicked();

    void on_toolButton_removeTrafoParamAttribute_clicked();

    void on_treeView_plugins_clicked(const QModelIndex &index);

private:

    Ui::SettingsDialog *ui;

    PluginInfoWidget *myPluginInformation;
};

#endif // SETTINGSDIALOG_H
