#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QPointer>

#include "oijob.h"
#include "featuretablemodel.h"
#include "featuretableproxymodel.h"
#include "featuretablecolumnconfig.h"
#include "trafoparamtablecolumnconfig.h"
#include "parameterdisplayconfig.h"

/*!
 * \brief The ModelManager class
 */
class ModelManager
{

private:
    ModelManager();

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

    //#########################################
    //static models (models that may be reused)
    //#########################################

    //general feature models
    static FeatureTableModel &getFeatureTableModel();
    static FeatureTableProxyModel &getFeatureTableProxyModel();

private:

    static QPointer<OiJob> currentJob;

    //#############
    //static models
    //#############

    //general feature models
    static FeatureTableModel featureTableModel;
    static FeatureTableProxyModel featureTableProxyModel;

private:

    //##############
    //helper methods
    //##############

    static void updateJob();
    static void updateFeatureTableColumnConfig();
    static void updateTrafoParamTableColumnConfig();
    static void updateParameterDisplayConfig();

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

};

#endif // MODELMANAGER_H
