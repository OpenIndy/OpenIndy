#ifndef CREATEFEATUREDIALOG_H
#define CREATEFEATUREDIALOG_H

#include <QDialog>
#include <QShowEvent>
#include <QDesktopWidget>

#include "featurewrapper.h"
#include "featureattributes.h"
#include "modelmanager.h"
#include "systemdbmanager.h"
#include "util.h"
#include "uiutil.h"

#if defined(OI_MAIN_LIB)
#  define OI_MAIN_EXPORT Q_DECL_EXPORT
#else
#  define OI_MAIN_EXPORT Q_DECL_IMPORT
#endif

using namespace oi;

namespace Ui {
class CreateFeatureDialog;
}

/*!
 * \brief The CreateFeatureDialog class
 */
class OI_MAIN_EXPORT CreateFeatureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateFeatureDialog(QWidget *parent = 0);
    ~CreateFeatureDialog();

    //###########################
    //get or set the feature type
    //###########################

    const FeatureTypes &getFeatureType() const;
    void setFeatureType(const FeatureTypes &type);

    bool featureCreated(bool created);

    void setCurrentJob(const QPointer<OiJob> &job);

signals:

    //#######################################
    //create features specified by attributes
    //#######################################

    void addFeatures(const FeatureAttributes &attributes);
    void initialized(); // currently used for testing

private slots:

    //#########################
    //actions triggered by user
    //#########################

    void on_toolButton_ok_clicked();
    void on_toolButton_cancel_clicked();
    void on_checkBox_nominal_toggled(bool checked);
    void on_checkBox_actual_toggled(bool checked);

    void on_comboBox_entityType_currentIndexChanged(const QString &entityTypeName);

    void on_comboBox_function_currentIndexChanged(const int index);

private:

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);

    void initGUI();
    void initModels();

    void initFunctionsModel();
    void initMeasurementConfigModel(QList<ElementTypes> neededElements);

    void toggleActualLabels(bool toggle);

    void toggleNominalLabels(bool toggle);

    //################
    //GUI interactions
    //################

    void featureAttributesFromGUI(FeatureAttributes &attributes);

    bool created;

private:
    Ui::CreateFeatureDialog *ui;

    //#################
    //helper attributes
    //#################

    FeatureTypes typeOfFeature;

    QPointer<AvailableFunctionsListProxyModel> functionListModel;
    QPointer<MeasurementConfigurationProxyModel> measurementConfigurationModel;

    void setDialogName();

    QPointer<OiJob> currentJob;

};

#endif // CREATEFEATUREDIALOG_H
