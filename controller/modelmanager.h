#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>
#include <QPointer>
#include <QStringListModel>

#include "oijob.h"
#include "featuretablemodel.h"
#include "featuretableproxymodel.h"
#include "featuretablecolumnconfig.h"
#include "trafoparamtablecolumnconfig.h"
#include "parameterdisplayconfig.h"

/*!
 * \brief The ModelManager class
 */
class ModelManager : public QObject
{
    Q_OBJECT

private:
    explicit ModelManager(QObject *parent = 0);

public:

    //############################
    //init models in model manager
    //############################

    static void init();

    //############################
    //get or set the current OiJob
    //############################

    static const QPointer<OiJob> &getCurrentJob();
    static void setCurrentJob(const QPointer<OiJob> &job);

    //###################
    //set display configs
    //###################

    static const FeatureTableColumnConfig &getFeatureTableColumnConfig();
    static void setFeatureTableColumnConfig(const FeatureTableColumnConfig &config);

    static const TrafoParamTableColumnConfig &getTrafoParamTableColumnConfig();
    static void setTrafoParamColumnConfig(const TrafoParamTableColumnConfig &config);

    static const ParameterDisplayConfig &getParameterDisplayConfig();
    static void setParameterDisplayConfig(const ParameterDisplayConfig &config);

    //#############################################
    //get static models (models that may be reused)
    //#############################################

    //general feature models
    static FeatureTableModel &getFeatureTableModel();
    static FeatureTableProxyModel &getFeatureTableProxyModel();

    //coordinate system models
    static QStringListModel &getCoordinateSystemsModel();
    static QStringListModel &getNominalSystemsModel();

private slots:

    //##########################################
    //update models on current job state changes
    //##########################################

    //feature(s) added or removed
    void coordSystemSetChanged();
    void stationSetChanged();

private:

    static QPointer<OiJob> currentJob;

    //#############
    //static models
    //#############

    //general feature models
    static FeatureTableModel featureTableModel;
    static FeatureTableProxyModel featureTableProxyModel;

    //coordinate systems models
    static QStringListModel coordinateSystemsModel;
    static QStringListModel nominalSystemsModel;

private:

    //##############
    //helper methods
    //##############

    static void updateJob();
    static void updateFeatureTableColumnConfig();
    static void updateTrafoParamTableColumnConfig();
    static void updateParameterDisplayConfig();

    //update models
    static void updateCoordinateSystemsModel();
    static void updateNominalSystemsModel();
    static void updateGroupsModel();

    //####################
    //model initialization
    //####################

    static void initFeatureTableModels();

    //###############
    //display configs
    //###############

    static FeatureTableColumnConfig featureTableColumnConfig; //defines which columns shall be visible in feature table proxy model
    static TrafoParamTableColumnConfig trafoParamTableColumnConfig; //defines which columns shall be visible in trafo param proxy model
    static ParameterDisplayConfig parameterDisplayConfig; //defines in which unit and with how many digits a parameter value shall be displayed

    //#############################
    //single model manager instance
    //#############################

    static QPointer<ModelManager> myInstance;

};

#endif // MODELMANAGER_H
