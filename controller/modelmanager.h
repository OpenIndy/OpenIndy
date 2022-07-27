#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>
#include <QPointer>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QSerialPortInfo>
#include <QHostAddress>
#include <QNetworkInterface>

#include "oimetadata.h"
#include "systemdbmanager.h"
#include "oijob.h"
#include "featuretablemodel.h"
#include "featuretableproxymodel.h"
#include "trafoparamtableproxymodel.h"
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
#include "sensorconfigurationmanager.h"
#include "sensortablemodel.h"
#include "sensortableproxymodel.h"
#include "sensorconfigurationmodel.h"
#include "sensorconfigurationproxymodel.h"
#include "plugintreeviewmodel.h"
#include "measurementconfigurationmodel.h"
#include "measurementconfigurationproxymodel.h"
#include "availablefunctionslistmodel.h"
#include "availablefunctionslistproxymodel.h"
#include "observationmodel.h"
#include "observationproxymodel.h"
#include "readingmodel.h"
#include "readingproxymodel.h"
#include "functionstatisticmodel.h"
#include "sensoraccuracymodel.h"
#include "sensorparametersmodel.h"
#include "bundlesystemsmodel.h"
#include "bundletemplatesmodel.h"
#include "bundlestationsmodel.h"
#include "bundlegeometriesmodel.h"
#include "functionweightstablemodel.h"
#include "functionweightproxymodel.h"
#include "bundleparametertableproxymodel.h"
#include "featuredifferencetablemodel.h"
#include "featuredifferenceproxymodel.h"

#if defined(OI_MAIN_LIB)
#  define OI_MAIN_EXPORT Q_DECL_EXPORT
#else
#  define OI_MAIN_EXPORT Q_DECL_IMPORT
#endif

using namespace oi;

class Controller;

/*!
 * \brief The ModelManager class
 */
class OI_MAIN_EXPORT ModelManager : public QObject
{
    friend class Controller;
    Q_OBJECT

private:
    explicit ModelManager(QObject *parent = 0);

signals:

    //############
    //log messages
    //############

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

    //#######################
    //display configs changed
    //#######################

    void parameterDisplayConfigChanged(const ParameterDisplayConfig &dConfig);

public:

    //################################################################
    //init models in model manager (only called once at the beginning)
    //################################################################

    static void init();

    static void testInit(QStringList entityTypes,
                         QPointer<MeasurementConfigManager> measurementConfigManager,
                         QList<sdb::Plugin> plugins);

    //############################
    //get or set the current OiJob
    //############################

    static const QPointer<OiJob> &getCurrentJob();
    static void setCurrentJob(const QPointer<OiJob> &job);

    //##########################
    //get or set config managers
    //##########################

    static const QPointer<SensorConfigurationManager> &getSensorConfigManager();
    static void setSensorConfigManager(const QPointer<SensorConfigurationManager> &sensorConfigManager);

    static const QPointer<MeasurementConfigManager> &getMeasurementConfigManager();
    static void setMeasurementConfigManager(const QPointer<MeasurementConfigManager> &measurementConfigManager);

    //##########################
    //get or set display configs
    //##########################

    static const FeatureTableColumnConfig &getFeatureTableColumnConfig();
    static void setFeatureTableColumnConfig(const FeatureTableColumnConfig &config);

    static const TrafoParamTableColumnConfig &getTrafoParamTableColumnConfig();
    static void setTrafoParamColumnConfig(const TrafoParamTableColumnConfig &config);

    static const ObservationTableColumnConfig &getObservationTableColumnConfig();
    static void setObservationTableColumnConfig(const ObservationTableColumnConfig &config);

    static const ReadingTableColumnConfig &getReadingTableColumnConfig();
    static void setReadingTableColumnConfig(const ReadingTableColumnConfig &config);

    static const ParameterDisplayConfig &getParameterDisplayConfig();
    static void setParameterDisplayConfig(const ParameterDisplayConfig &config);

    //#############################################
    //get static models (models that may be reused)
    //#############################################

    //general feature models
    static FeatureTableModel &getFeatureTableModel();
    static FeatureTableProxyModel &getFeatureTableProxyModel();
    static TrafoParamTableProxyModel &getTrafoParamTableProxyModel();
    static FeatureTreeViewModel &getFeatureTreeViewModel();
    static AvailableElementsTreeViewProxyModel &getAvailableElementsTreeViewProxyModel();
    static UsedElementsModel &getUsedElementsModel();
    static BundleParameterTableProxyModel &getBundleParameterTableProxyModel();
    static FeatureDifferenceTableModel &getFeatureDifferenceTableModel();
    static FeatureDifferenceProxyModel &getFeatureDifferenceProxyModel();

    //coordinate system models
    static QStringListModel &getCoordinateSystemsModel();
    static QStringListModel &getNominalSystemsModel();
    static BundleSystemsModel &getBundleSystemsModel();

    //group name model
    static QStringListModel &getGroupNamesModel();

    //actual nominal filter model
    static QStringListModel &getActualNominalFilterModel();

    //unit types models
    static QStringListModel &getUnitTypesModel(const DimensionType &dimension);

    //plugin models
    static QStringListModel &getPluginNamesModel();
    static PluginTreeViewModel &getPluginTreeViewModel();

    //reading types models
    static QStringListModel &getReadingTypeNamesModel();

    //scalar entity type model
    static QStringListModel &getScalarEntityTypeNamesModel();

    //function models
    static FunctionTableModel &getFunctionTableModel();
    static FunctionTableProxyModel &getFunctionTableProxyModel();
    static ActiveFeatureFunctionsModel &getActiveFeatureFunctionsModel();
    static FunctionWeightsTableModel &getFunctionWeightTableModel();
    static FunctionWeightProxyModel &getFunctionWeightProxyModel();

    //sensor models
    static QStringListModel &getSensorTypeNamesModel();
    static SensorTableModel &getSensorTableModel();
    static SensorTableProxyModel &getSensorTableProxyModel();

    //sensor connection models
    static QStandardItemModel &getBaudRateTypesModel();
    static QStandardItemModel &getDataBitTypesModel();
    static QStandardItemModel &getFlowControlTypesModel();
    static QStandardItemModel &getParityTypesModel();
    static QStandardItemModel &getStopBitTypesModel();
    static QStandardItemModel &getAvailableSerialPortsModel();
    static QStandardItemModel &getAvailableIpAdressesModel();

    //measurement config models
    static MeasurementConfigurationModel &getMeasurementConfigurationModel();
    static MeasurementConfigurationProxyModel &getMeasurementConfigurationProxyModel();

    //get observation and reading models
    static ObservationModel &getObservationModel();
    static ObservationProxyModel &getObservationProxyModel();
    static ReadingModel &getReadingModel();
    static ReadingProxyModel &getReadingProxyModel();

    //bundle templates
    static BundleTemplatesModel &getBundleTemplatesModel();

    //############################################################
    //get dynamic models (models that are newly created each time)
    //############################################################

    //plugin models
    static QPointer<QStringListModel> getExchangeSimpleAsciiNames(const QString &pluginName);
    static QPointer<QStringListModel> getExchangeSimpleAsciiDelimiters(const QString &pluginName, const QString &exchangeName);
    static QPointer<QStringListModel> getExchangeSimpleAsciiSupportedGeometries(const QString &pluginName, const QString &exchangeName);

    //function list filter model
    static QPointer<AvailableFunctionsListProxyModel> getAvailableFunctionsProxyModel();

    //statistic models
    static QPointer<FunctionStatisticModel> getFunctionStatisticModel();

    //sensor models
    static QPointer<SensorConfigurationModel> getSensorConfigurationModel(QObject *parent = NULL);
    static QPointer<SensorConfigurationProxyModel> getSensorConfigurationProxyModel(QPointer<SensorConfigurationModel> sourceModel);
    static QPointer<SensorAccuracyModel> getSensorAccuracyModel(QObject *parent = NULL);
    static QPointer<SensorParametersModel> getSensorParametersModel(QObject *parent = NULL);

    //bundle models
    static QPointer<BundleStationsModel> getBundleStationsModel(QObject *parent = NULL);
    static QPointer<BundleGeometriesModel> getBundleGeometriesModel(QObject *parent = NULL);

private slots:

    //##########################################
    //update models on current job state changes
    //##########################################

    //feature(s) added or removed
    void featureSetChanged();
    void coordSystemSetChanged();
    void stationSetChanged();

    //groups added or removed
    void availableGroupsChanged();

    //feature specific attributes changed
    void featureNameChanged(const int &featureId, const QString &oldName);

private:

    static QPointer<OiJob> currentJob;

    //#############
    //static models
    //#############

    //general feature models
    static FeatureTableModel featureTableModel;
    static FeatureTableProxyModel featureTableProxyModel;
    static TrafoParamTableProxyModel trafoParamTableProxyModel;
    static FeatureTreeViewModel featureTreeViewModel;
    static AvailableElementsTreeViewProxyModel availableElementsTreeViewProxyModel;
    static UsedElementsModel usedElementsModel;
    static BundleParameterTableProxyModel bundleParameterTableProxyModel;
    static FeatureDifferenceTableModel featureDifferenceTableModel;
    static FeatureDifferenceProxyModel featureDifferenceProxyModel;

    //coordinate systems models
    static QStringListModel coordinateSystemsModel;
    static QStringListModel nominalSystemsModel;
    static BundleSystemsModel bundleSystemsModel;

    //group name model
    static QStringListModel groupNamesModel;

    //actual nominal filter model
    static QStringListModel actualNominalFilterModel;

    //unit types models
    static QStringListModel metricUnitTypesModel;
    static QStringListModel angularUnitTypesModel;
    static QStringListModel temperatureUnitTypesModel;

    //plugin models
    static QStringListModel pluginNamesModel;
    static PluginTreeViewModel pluginTreeViewModel;

    //reading models
    static QStringListModel readingTypeNamesModel;

    //scalar entity type model
    static QStringListModel scalarEntityTypeNamesModel;

    //function models
    static FunctionTableModel functionTableModel;
    static FunctionTableProxyModel functionTableProxyModel;
    static ActiveFeatureFunctionsModel activeFeatureFunctionsModel;
    static AvailableFunctionsListModel functionsListModel;
    static FunctionWeightsTableModel functionWeightsTableModel;
    static FunctionWeightProxyModel functionWeightProxyModel;

    //sensor models
    static QStringListModel sensorTypeNamesModel;
    static SensorTableModel sensorTableModel;
    static SensorTableProxyModel sensorTableProxyModel;

    //sensor connection models
    static QStandardItemModel baudRateTypesModel;
    static QStandardItemModel dataBitTypesModel;
    static QStandardItemModel flowControlTypesModel;
    static QStandardItemModel parityTypesModel;
    static QStandardItemModel stopBitTypesModel;
    static QStandardItemModel availableSerialPortsModel;
    static QStandardItemModel availableIpAdressesModel;

    //measurement config models
    static MeasurementConfigurationModel measurementConfigurationModel;
    static MeasurementConfigurationProxyModel measurementConfigurationProxyModel;

    //observation and reading models
    static ObservationModel observationModel;
    static ObservationProxyModel observationProxyModel;
    static ReadingModel readingModel;
    static ReadingProxyModel readingProxyModel;

    //bundle templates
    static BundleTemplatesModel bundleTemplatesModel;

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
    static void updateObservationTableColumnConfig();
    static void updateReadingTableColumnConfig();
    static void updateParameterDisplayConfig();
    static void updateSensorConfigManager();
    static void updateMeasurementConfigManager();

    //################################################################
    //update models (called every time a connected event is triggered)
    //################################################################

    static void updateCoordinateSystemsModel();
    static void updateNominalSystemsModel();
    static void updateGroupsModel();
    static void updateSensorConfigNamesModel();

    //########################################################
    //model initialization (called only once at the beginning)
    //########################################################

    static void initFeatureTableModels();
    static void initFeatureTreeViewModels();

    static void initFunctionTableModels();

    static void initSensorTypeNamesModel();
    static void initSensorTableModels();
    static void initSensorListViewModels();

    static void initMeasurementConfigModels();

    static void initSensorConnectionModels();

    static void initUnitTypesModels();

    static void initReadingTypesModels();

    static void initScalarEntityTypesModels();

    static void initPluginModels();

    static void initGroupsModel();

    static void initActualNominalFilterModel();

    static void initObservationModels();
    static void initReadingModels();

    //###############
    //display configs
    //###############

    static FeatureTableColumnConfig featureTableColumnConfig; //defines which columns shall be visible in feature table proxy model
    static TrafoParamTableColumnConfig trafoParamTableColumnConfig; //defines which columns shall be visible in trafo param proxy model
    static ObservationTableColumnConfig observationTableColumnConfig; //defines which columns shall be visible in observation proxy model
    static ReadingTableColumnConfig readingTableColumnConfig; //defines which columns shall be visible in reading proxy model
    static ParameterDisplayConfig parameterDisplayConfig; //defines in which unit and with how many digits a parameter value shall be displayed

    //##############
    //config manager
    //##############

    static QPointer<SensorConfigurationManager> sensorConfigManager;
    static QPointer<MeasurementConfigManager> measurementConfigManager;

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
