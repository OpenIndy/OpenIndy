#ifndef OIMODELMANAGER_H
#define OIMODELMANAGER_H

#include <QObject>
#include <QMap>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QSerialPortInfo>
#include <QHostAddress>
#include <QNetworkInterface>

#include "systemdbmanager.h"
#include "oifeaturestate.h"
#include "unitconverter.h"
#include "configuration.h"

#include "geometrytypesproxymodel.h"
#include "featuretablemodel.h"
#include "featuretableproxymodel.h"
#include "trafoparamproxymodel.h"
#include "featuretreeviewmodel.h"
#include "availableelementstreeviewproxymodel.h"
#include "usedelementsmodel.h"
#include "featuregraphicstreeviewproxymodel.h"
#include "plugintreeviewmodel.h"

class OiModelManager : public QObject
{
    Q_OBJECT
private:
    explicit OiModelManager(QObject *parent = 0);

public:
    static OiModelManager *getInstance();

    //#########################################
    //static models (models that may be reused)
    //#########################################

    static QStringListModel &getPluginNamesModel();
    static PluginTreeViewModel &getPluginTreeViewModel();
    static QSqlQueryModel &getCreateFunctionsModel();
    static QSqlQueryModel &getChangeFunctionsModel();
    static QSqlQueryModel &getSensorsModel(Configuration::SensorTypes typeOfSensor);

    static QStringListModel &getCoordinateSystemsModel();
    static QStringListModel &getNominalSystemsModel();

    static QStringListModel &getGeometryTypesModel();

    static QStandardItemModel &getSensorTypes();
    static QStandardItemModel &getBaudRateTypes();
    static QStandardItemModel &getDataBitTypes();
    static QStandardItemModel &getFlowControlTypes();
    static QStandardItemModel &getParityTypes();
    static QStandardItemModel &getStopBitTypes();
    static QStandardItemModel &getAvailableSerialPorts();
    static QStandardItemModel &getAvailableIpAdresses();

    static QStringListModel &getDistanceUnitsModel();
    static QStringListModel &getAngleUnitsModel();
    static QStringListModel &getTemperatureUnitsModel();

    static QStringListModel &getGroupNamesModel();

    static FeatureTableModel &getFeatureTableModel();
    static FeatureTableProxyModel &getFeatureTableProxyModel();
    static TrafoParamProxyModel &getTrafoParamProxyModel();
    static FeatureTreeViewModel &getFeatureTreeViewModel();
    static FeatureGraphicsTreeViewProxyModel &getFeatureGraphicsModel();

    //##########################################################
    //dynamic models (models that cannot or shall not be shared)
    //##########################################################

    static GeometryTypesProxyModel *getGeometryTypesFilterModel();



    static QStringListModel *getSimpleAsciiExchangePlugins(QString plugin);
    //static QStringListModel getDefinedFormatExchangePlugins();

    static AvailableElementsTreeViewProxyModel *getAvailableElementsModel(Configuration::ElementTypes filter);
    static UsedElementsModel *getUsedElementsModel(int functionIndex, int elementIndex);

    static QStandardItemModel *getFunctionTreeViewModel();

private slots:
    void featureSetChanged();
    void activeFeatureChanged();
    void activeStationChanged();
    void activeCoordinateSystemChanged();
    void availableGroupsChanged();
    void coordSystemSetChanged();
    void featuresRecalculated();

private:
    static OiModelManager *myInstance;

    //model instances
    static QStringListModel pluginNamesModel;
    static QStringListModel coordinateSystemsModel;
    static QStringListModel nominalSystemsModel;
    static QStringListModel geometryTypes;
    static QStandardItemModel sensorTypes;
    static QStandardItemModel baudRateTypes;
    static QStandardItemModel dataBitTypes;
    static QStandardItemModel flowControlTypes;
    static QStandardItemModel parityTypes;
    static QStandardItemModel stopBitTypes;
    static QStandardItemModel availableSerialPorts;
    static QStandardItemModel availableIpAdresses;
    static QStringListModel distanceUnitsModel;
    static QStringListModel angleUnitsModel;
    static QStringListModel temperatureUnitsModel;
    static QStringListModel groupNamesModel;
    static FeatureTableModel featureTableModel;
    static FeatureTableProxyModel featureTableProxyModel;
    static TrafoParamProxyModel trafoParamProxyModel;
    static FeatureTreeViewModel featureTreeViewModel;
    static FeatureGraphicsTreeViewProxyModel featureGraphicsModel;
    static PluginTreeViewModel pluginTreeViewModel;
    static QSqlQueryModel createFeatureFunctionsModel;
    static QSqlQueryModel changeFeatureFunctionsModel;
    static QSqlQueryModel laserTrackerModel;
    static QSqlQueryModel totalStationModel;
    static QSqlQueryModel undefinedSensorModel;

    //initialize and connect all models provided by OiModelManager
    void initModels();
    void connectModels();

    //helper methods to initialize the different model types
    void initSensorModels();
    void initCoordinateSystemModels();
    void initUnitModels();
    void initGroupNameModels();
    void initPluginModels();




    //TODO think about a way to manage models (delete them when not necessary)
};

#endif // OIMODELMANAGER_H
