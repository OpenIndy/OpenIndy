#ifndef SCALARENTITYDIALOG_H
#define SCALARENTITYDIALOG_H

#include <QDialog>
#include "configuration.h"
#include "featurewrapper.h"
#include <QShowEvent>
#include <QDesktopWidget>
#include "featureattributesexchange.h"
#include "oifeaturestate.h"

namespace Ui {
class ScalarEntityDialog;
}

class ScalarEntityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScalarEntityDialog(QWidget *parent = 0);
    ~ScalarEntityDialog();

    void availableGroupsChanged(QStringList myGroups);
    void setAvailableFunctions(QStringList functions, QString defaultFunction);

signals:

    void createFeature(FeatureAttributesExchange fae);

    void createFeatureMConfig();

private slots:
    void initGUI();

    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_checkBox_nominal_toggled(bool checked);

    void on_comboBox_scalarEntityType_currentTextChanged(const QString &arg1);

    void on_toolButton_mConfig_clicked();

    void showEvent(QShowEvent *event);

    void on_checkBox_actual_toggled(bool checked);

private:
    Ui::ScalarEntityDialog *ui;
};

#endif // SCALARENTITYDIALOG_H
