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

    void updateDisplayConfigFromSelection();

private:
    Ui::SettingsDialog *ui;

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

#endif // SETTINGSDIALOG_H
