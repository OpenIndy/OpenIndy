#ifndef OIMODELMANAGER_H
#define OIMODELMANAGER_H

#include <QObject>
#include <QMap>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QStringListModel>

#include "systemdbmanager.h"
#include "oifeaturestate.h"

class OiModelManager : public QObject
{
    Q_OBJECT
private:
    explicit OiModelManager(QObject *parent = 0);

public:
    static OiModelManager *getInstance();

    static QStringListModel &getPluginNamesModel();
    static QStringListModel &getNominalSystemsModel();
    static QStringListModel &getGeometryTypesModel();

    static QStringListModel *getSimpleAsciiExchangePlugins(QString plugin);
    //static QStringListModel getDefinedFormatExchangePlugins();

    static QStandardItemModel &getSensorTypes();
    static QStandardItemModel &getBaudRateTypes();
    static QStandardItemModel &getDataBitTypes();
    static QStandardItemModel &getFlowControlTypes();
    static QStandardItemModel &getParityTypes();
    static QStandardItemModel &getStopBitTypes();
    static QStandardItemModel &getAvailableSerialPorts();
    static QStandardItemModel &getAvailableIpAdresses();

private:
    static OiModelManager *myInstance;

    static QStringListModel pluginNamesModel;

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

    //static QStringListModel simpleAsciiExchangePlugins;
    //static QStringListModel definedFormatExchangePlugins;

    void initModels();

    //TODO think about a way to manage models (delete them when not necessary)
};

#endif // OIMODELMANAGER_H
