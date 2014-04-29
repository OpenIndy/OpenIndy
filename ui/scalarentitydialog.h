#ifndef SCALARENTITYDIALOG_H
#define SCALARENTITYDIALOG_H

#include <QDialog>
#include "configuration.h"
#include "featurewrapper.h"
#include "QShowEvent"

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

signals:

    void createEntity(int count, int featureType, QString name,
                       bool actual, bool nominal, bool isCommonPoint, CoordinateSystem *nominalSystem);

    void createFeatureMConfig();

private slots:
    void initGUI();

    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_checkBox_nominal_toggled(bool checked);

    void on_comboBox_scalarEntityType_currentTextChanged(const QString &arg1);

    void on_toolButton_mConfig_clicked();

    void showEvent(QShowEvent *event);

private:
    Ui::ScalarEntityDialog *ui;
};

#endif // SCALARENTITYDIALOG_H
