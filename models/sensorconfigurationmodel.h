#ifndef SENSORCONFIGURATIONMODEL_H
#define SENSORCONFIGURATIONMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>
#include <QPixmap>
#include <QPointer>

#include "types.h"
#include "util.h"
#include "sensorconfigurationmanager.h"

/*!
 * \brief The SensorConfigurationTreeViewModel class
 * Model that holds all available sensor configurations
 */
class SensorConfigurationModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SensorConfigurationModel(QObject *parent = 0);

    //#######################################
    //override methods of abstract item model
    //#######################################

    int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    //#####################################
    //get informations about sensor configs
    //#####################################

    QModelIndex getIndex(const QString &name) const;

    SensorTypes getSensorType(const QModelIndex &index) const;
    QString getSensorName(const QModelIndex &index) const;
    QString getPluginName(const QModelIndex &index) const;

    ConnectionConfig getConnectionConfig(const QModelIndex &index);

    Accuracy getAccuracy(const QModelIndex &index);

    QMap<QString, int> getIntegerParameter(const QModelIndex &index);
    QMap<QString, double> getDoubleParameter(const QModelIndex &index);
    QMap<QString, QString> getStringParameter(const QModelIndex &index);

    bool getIsSaved(const QModelIndex &index) const;

    //############################
    //add or remove sensor configs
    //############################

    QModelIndex addSensorConfig(const SensorConfiguration &sConfig);
    void removeSensorConfig(const QModelIndex &index);

    void replaceSensorConfig(const QString &name, const SensorConfiguration &sConfig);

    //#######################################
    //get or set sensor configuration manager
    //#######################################

    const QPointer<SensorConfigurationManager> &getSensorConfigurationManager() const;
    void setSensorConfigurationManager(const QPointer<SensorConfigurationManager> &manager);

private slots:

    //###########################################
    //update the model when configuration changes
    //###########################################

    void updateModel();

private:

    //##############
    //helper methods
    //##############

    void connectConfigManager();

    //#####################
    //sensor config manager
    //#####################

    QPointer<SensorConfigurationManager> sensorConfigManager;

};

#endif // SENSORCONFIGURATIONMODEL_H
