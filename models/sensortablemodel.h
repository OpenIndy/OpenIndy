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

    QModelIndex getSelectedIndex() const;
    void selectSensorPlugin(const QString &sensorName, const QString &pluginName);

    QString getSensorDescription(const QModelIndex &index) const;
    QString getSensorName(const QModelIndex &index) const;
    QString getPluginName(const QModelIndex &index) const;
    SensorTypes getSensorType(const QModelIndex &index) const;
    QString getPluginFilePath(const QModelIndex &index) const;

    QList<ConnectionTypes> getSupportedConnectionTypes(const QModelIndex &index);

    Accuracy getAccuracy(const QModelIndex &index);

    QMap<QString, int> getIntegerParameter();
    QMap<QString, double> getDoubleParameter();
    QMultiMap<QString, QString> getStringParameter();

    //######################################
    //get or set currently available plugins
    //######################################

    const QList<sdb::Plugin> &getPlugins() const;
    void setPlugins(const QList<sdb::Plugin> &plugins);

private slots:

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

    QModelIndex selectedIndex;
    
};

#endif // SENSORTABLEMODEL_H
