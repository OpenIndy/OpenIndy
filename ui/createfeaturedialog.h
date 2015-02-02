#ifndef CREATEFEATUREDIALOG_H
#define CREATEFEATUREDIALOG_H

#include <QDialog>
#include <QShowEvent>

#include "configuration.h"
#include "featurewrapper.h"
#include "featureattributes.h"
#include "oifeaturestate.h"
#include "oimodelmanager.h"

namespace Ui {
class CreateFeatureDialog;
}

//! Dialog to create features in OpenIndy
class CreateFeatureDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateFeatureDialog(QWidget *parent = 0);
    ~CreateFeatureDialog();

    void setFeatureType(const Configuration::FeatureTypes &typeOfFeature);

signals:
    void addFeatures(const FeatureAttributes &attributes);

private slots:
    void showEvent(QShowEvent *event);

    //##################################
    //methods to initialize GUI elements
    //##################################

    void initGUI();
    void initModels();

    void initFunctionsModel();

    //################
    //GUI interactions
    //################

    void on_toolButton_cancel_clicked();
    void on_toolButton_create_clicked();
    void on_toolButton_mConfig_clicked();

    void featureAttributesFromGUI(FeatureAttributes &attributes);

private:
    Ui::CreateFeatureDialog *ui;

    Configuration::FeatureTypes typeOfFeature;

};


    /*

    void setFeatureType(Configuration::FeatureTypes typeOfFeature);


    //void availableGroupsChanged(QStringList myGroups);
    //void setAvailableFunctions(QStringList functions, QString defaultFunction);



signals:

    void createFeature(FeatureAttributes fae);
    void createFeatureMConfig(Configuration::FeatureTypes typeOfFeature);

    void trafoParamCreated();

private slots:



    //void receiveFeatureType(Configuration::FeatureTypes);

    void on_toolButton_create_clicked();
    void on_toolButton_cancel_clicked();
    void on_checkBox_Nominal_toggled(bool checked);
    void on_toolButton_mConfig_clicked();
    void on_checkBox_Actual_toggled(bool checked);
    void on_checkBox_movement_toggled(bool checked);
    void on_comboBox_destinationSystem_currentIndexChanged(const QString &arg1);



    Configuration::FeatureTypes typeOfFeature;

    void showEvent(QShowEvent *event);
    void initGUI();
};*/

#endif // CREATEFEATUREDIALOG_H
