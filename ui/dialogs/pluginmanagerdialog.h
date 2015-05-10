#ifndef PLUGINMANAGERDIALOG_H
#define PLUGINMANAGERDIALOG_H

#include <QDialog>
#include <QShowEvent>
#include <QCloseEvent>
#include <QLayout>
#include <QCheckBox>
#include <QStringListModel>
#include <QDesktopWidget>

#include "plugininfowidget.h"
#include "modelmanager.h"

namespace Ui {
class PluginManagerDialog;
}

/*!
 * \brief The PluginManagerDialog class
 * Dialog to get an overview of all loaded plugins and to add or remove them
 */
class PluginManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PluginManagerDialog(QWidget *parent = 0);
    ~PluginManagerDialog();

signals:

    //load new plugins
    void loadPlugins();

private slots:

    //#########################
    //actions triggered by user
    //#########################

    //add or remove plugins
    void on_pushButton_add_clicked();
    void on_pushButton_remove_clicked();

    //plugin selected
    void on_treeView_plugins_clicked(const QModelIndex &index);

private:

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);

    void initGUI();
    void initModels();

private:
    Ui::PluginManagerDialog *ui;

    PluginInfoWidget *pluginInformation;

    /*
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

    void changedColumnOrder();

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

    void on_toolButton_up_clicked();

    void on_toolButton_down_clicked();

private:

    Ui::SettingsDialog *ui;

    PluginInfoWidget *myPluginInformation;*/
};

#endif // PLUGINMANAGERDIALOG_H
