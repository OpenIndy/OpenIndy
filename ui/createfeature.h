#ifndef CREATEFEATURE_H
#define CREATEFEATURE_H

#include <QDialog>
#include "configuration.h"
#include "featurewrapper.h"

namespace Ui {
class CreateFeature;
}
/*!
 * \brief This class contains all functions for creating features except scalar entities.
 */
class CreateFeature : public QDialog
{
    Q_OBJECT

public:
    explicit CreateFeature(QList<FeatureWrapper*> &features, QWidget *parent = 0);
    ~CreateFeature();

    Configuration::FeatureTypes typeOfFeature;

    //used for creating trafoparams and filling comboboxes
    QList<FeatureWrapper*> &featureList;

signals:

    void createFeature(int count, int featureType, QString name,
                       bool actual, bool nominal, bool isCommonPoint, CoordinateSystem *nominalSystem);

    void createTrafoParam(int count, int featureType, QString name,
                           CoordinateSystem *startSystem, CoordinateSystem *destSystem);
    void createFeatureMConfig();

private slots:

    void initGUI();

    void receiveFeatureType(Configuration::FeatureTypes);

    void on_toolButton_create_clicked();

    void on_toolButton_cancel_clicked();

    void on_checkBox_Nominal_toggled(bool checked);

    void on_toolButton_mConfig_clicked();

private:
    Ui::CreateFeature *ui;
};

#endif // CREATEFEATURE_H
