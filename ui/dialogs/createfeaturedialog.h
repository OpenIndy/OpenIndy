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

using namespace oi;

namespace Ui {
class CreateFeatureDialog;
}

/*!
 * \brief The CreateFeatureDialog class
 */
class CreateFeatureDialog : public QDialog
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

private slots:

    //#########################
    //actions triggered by user
    //#########################

    void on_toolButton_ok_clicked();
    void on_toolButton_cancel_clicked();
    void on_checkBox_nominal_toggled(bool checked);
    void on_checkBox_actual_toggled(bool checked);

    void on_comboBox_entityType_currentIndexChanged(const QString &arg1);

private:

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);

    void initGUI();
    void initModels();

    void initFunctionsModel();

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

    void setDialogName();

    QPointer<OiJob> currentJob;

};

#endif // CREATEFEATUREDIALOG_H
