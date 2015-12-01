#ifndef SENSORTABLEMODEL_H
#define SENSORTABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QPointer>
#include <QList>

#include "oijob.h"
#include "types.h"
#include "util.h"
#include "systemdbmanager.h"
#include "oimetadata.h"
#include "pluginloader.h"

using namespace oi;

/*!
 * \brief The SensorTableModel class
 * Model that holds all available sensor plugins
 */
class SensorTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SensorTableModel(QObject *parent = 0);

    //########################################
    //override methods of abstract table model
    //########################################

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    //######################
    //get sensor information
    //######################

    //get or set selection
    QPair<QString, QString> getSelectedSensorPlugin() const;
    void selectSensorPlugin(const QModelIndex &index);
    void selectSensorPlugin(const SensorConfiguration &sConfig);
    void selectSensorPlugin(const QString &sensorName, const QString &pluginName);

    //get general sensor information
    SensorTypes getSensorType() const;
    SensorTypes getSensorType(const QModelIndex &index) const;
    QString getSensorDescription() const;
    QString getSensorDescription(const QModelIndex &index) const;
    QString getPluginFilePath() const;
    QString getPluginFilePath(const QModelIndex &index) const;

    //get supported connection / reading or action types
    QList<ReadingTypes> getSupportedReadingTypes() const;
    QList<SensorFunctions> getSupportedSensorActions() const;
    QList<ConnectionTypes> getSupportedConnectionTypes() const;

    //get default sensor config
    SensorConfiguration getDefaultSensorConfig() const;

    //######################################
    //get or set currently available plugins
    //######################################

    const QList<sdb::Plugin> &getPlugins() const;
    void setPlugins(const QList<sdb::Plugin> &plugins);

signals:

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

    //##################################
    //inform about sensor plugin changes
    //##################################

    void selectedSensorPluginChanged();

public slots:

    //##############################################
    //update the model when the plugins were changed
    //##############################################

    void updateModel();

private:

    //###########################
    //currently available plugins
    //###########################

    QList<sdb::Plugin> plugins;

    //####################
    //save selected plugin
    //####################

    QPair<QString, QString> selectedSensor; //first=plugin, second=sensor
    
};

#endif // SENSORTABLEMODEL_H
