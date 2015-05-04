#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>
#include <QPointer>
#include <QStringListModel>

#include "oimetadata.h"
#include "systemdbmanager.h"
#include "oijob.h"
#include "featuretablemodel.h"
#include "featuretableproxymodel.h"
#include "featuretablecolumnconfig.h"
#include "trafoparamtablecolumnconfig.h"
#include "parameterdisplayconfig.h"
#include "pluginloader.h"
#include "featuretreeviewmodel.h"
#include "availableelementstreeviewproxymodel.h"
#include "usedelementsmodel.h"
#include "activefeaturefunctionsmodel.h"
#include "functiontablemodel.h"
#include "functiontableproxymodel.h"

/*!
 * \brief The ModelManager class
 */
class ModelManager : public QObject
{
    Q_OBJECT

private:
    explicit ModelManager(QObject *parent = 0);

public:

    //################################################################
    //init models in model manager (only called once at the beginning)
    //################################################################

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
    static FeatureTreeViewModel &getFeatureTreeViewModel();
    static AvailableElementsTreeViewProxyModel &getAvailableElementsTreeViewProxyModel();
    static UsedElementsModel &getUsedElementsModel();

    //coordinate system models
    static QStringListModel &getCoordinateSystemsModel();
    static QStringListModel &getNominalSystemsModel();

    //group name model
    static QStringListModel &getGroupNamesModel();

    //unit types models
    static QStringListModel &getUnitTypesModel(const DimensionType &dimension);

    //plugin models
    static QStringListModel &getPluginNamesModel();

    //function models
    static FunctionTableModel &getFunctionTableModel();
    static FunctionTableProxyModel &getFunctionTableProxyModel();
    static ActiveFeatureFunctionsModel &getActiveFeatureFunctionsModel();

    //############################################################
    //get dynamic models (models that are newly created each time)
    //############################################################

    //plugin models
    static QPointer<QStringListModel> getExchangeSimpleAsciiNames(const QString &pluginName);
    static QPointer<QStringListModel> getExchangeSimpleAsciiDelimiters(const QString &pluginName, const QString &exchangeName);
    static QPointer<QStringListModel> getExchangeSimpleAsciiSupportedGeometries(const QString &pluginName, const QString &exchangeName);

private slots:

    //##########################################
    //update models on current job state changes
    //##########################################

    //feature(s) added or removed
    void coordSystemSetChanged();
    void stationSetChanged();

    //groups added or removed
    void availableGroupsChanged();

private:

    static QPointer<OiJob> currentJob;

    //#############
    //static models
    //#############

    //general feature models
    static FeatureTableModel featureTableModel;
    static FeatureTableProxyModel featureTableProxyModel;
    static FeatureTreeViewModel featureTreeViewModel;
    static AvailableElementsTreeViewProxyModel availableElementsTreeViewProxyModel;
    static UsedElementsModel usedElementsModel;

    //coordinate systems models
    static QStringListModel coordinateSystemsModel;
    static QStringListModel nominalSystemsModel;

    //group name model
    static QStringListModel groupNamesModel;

    //unit types models
    static QStringListModel metricUnitTypesModel;
    static QStringListModel angularUnitTypesModel;
    static QStringListModel temperatureUnitTypesModel;

    //plugin models
    static QStringListModel pluginNamesModel;

    //function models
    static FunctionTableModel functionTableModel;
    static FunctionTableProxyModel functionTableProxyModel;
    static ActiveFeatureFunctionsModel activeFeatureFunctionsModel;

    //##################
    //empty dummy models
    //##################

    static QStringListModel dummyStringListModel;

private:

    //##############
    //helper methods
    //##############

    static void updateJob();
    static void updateFeatureTableColumnConfig();
    static void updateTrafoParamTableColumnConfig();
    static void updateParameterDisplayConfig();

    //################################################################
    //update models (called every time a connected event is triggered)
    //################################################################

    static void updateCoordinateSystemsModel();
    static void updateNominalSystemsModel();
    static void updateGroupsModel();

    //########################################################
    //model initialization (called only once at the beginning)
    //########################################################

    static void initFeatureTableModels();
    static void initFeatureTreeViewModels();

    static void initFunctionTableModels();

    static void initUnitTypesModels();

    static void initPluginModels();

    //###############
    //display configs
    //###############

    static FeatureTableColumnConfig featureTableColumnConfig; //defines which columns shall be visible in feature table proxy model
    static TrafoParamTableColumnConfig trafoParamTableColumnConfig; //defines which columns shall be visible in trafo param proxy model
    static ParameterDisplayConfig parameterDisplayConfig; //defines in which unit and with how many digits a parameter value shall be displayed

    //###############################################################
    //save sdb::Plugin so that the database must only be queried once
    //###############################################################

    static QList<sdb::Plugin> plugins;

    //#############################
    //single model manager instance
    //#############################

    static QPointer<ModelManager> myInstance;

};

#endif // MODELMANAGER_H
