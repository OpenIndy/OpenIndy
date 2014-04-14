#ifndef SCALARENTITYDIALOG_H
#define SCALARENTITYDIALOG_H

#include <QDialog>
#include "configuration.h"
#include "featurewrapper.h"

namespace Ui {
class ScalarEntityDialog;
}

class ScalarEntityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScalarEntityDialog(QList<FeatureWrapper*> &features, QWidget *parent = 0);
    ~ScalarEntityDialog();

    QList<FeatureWrapper*> &featureList;

    void availableGroupsChanged(QStringList myGroups);

signals:

    void createEntity(int count, int featureType, QString name, QString group,
                       bool actualNominal, bool isCommonPoint, double value, CoordinateSystem *nominalSystem);

    void createFeatureMConfig();

private slots:
    void initGUI();

    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_checkBox_nominal_toggled(bool checked);

    void on_comboBox_scalarEntityType_currentTextChanged(const QString &arg1);

    void on_toolButton_mConfig_clicked();

private:
    Ui::ScalarEntityDialog *ui;
};

#endif // SCALARENTITYDIALOG_H
