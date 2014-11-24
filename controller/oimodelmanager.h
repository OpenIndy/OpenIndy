#ifndef OIMODELMANAGER_H
#define OIMODELMANAGER_H

#include <QObject>
#include <QMap>
#include <QAbstractItemModel>
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

private:
    static OiModelManager *myInstance;

    static QStringListModel pluginNamesModel;

    static QStringListModel nominalSystemsModel;

    static QStringListModel geometryTypes;

    //static QStringListModel simpleAsciiExchangePlugins;
    //static QStringListModel definedFormatExchangePlugins;

    void initModels();

    //TODO think about a way to manage models (delete them when not necessary)
};

#endif // OIMODELMANAGER_H
