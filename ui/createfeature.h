#ifndef CREATEFEATURE_H
#define CREATEFEATURE_H

#include <QDialog>
#include <QShowEvent>
#include <QDesktopWidget>
#include "configuration.h"
#include "featurewrapper.h"
#include "featureattributesexchange.h"
#include "oifeaturestate.h"

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
    explicit CreateFeature(QWidget *parent = 0);
    ~CreateFeature();

    Configuration::FeatureTypes typeOfFeature;

    void availableGroupsChanged(QStringList myGroups);
    void setAvailableFunctions(QStringList functions, QString defaultFunction);

    void showEvent(QShowEvent *event);

signals:

    void createFeature(FeatureAttributesExchange fae);

    void createFeatureMConfig(Configuration::FeatureTypes typeOfFeature);

    void trafoParamCreated();

private slots:

    void initGUI();

    void receiveFeatureType(Configuration::FeatureTypes);

    void on_toolButton_create_clicked();

    void on_toolButton_cancel_clicked();

    void on_checkBox_Nominal_toggled(bool checked);

    void on_toolButton_mConfig_clicked();

    void on_checkBox_Actual_toggled(bool checked);

    void on_checkBox_movement_toggled(bool checked);

    void on_comboBox_destinationSystem_currentIndexChanged(const QString &arg1);

private:
    Ui::CreateFeature *ui;
};

#endif // CREATEFEATURE_H
