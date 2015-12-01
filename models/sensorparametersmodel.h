#ifndef SENSORPARAMETERSMODEL_H
#define SENSORPARAMETERSMODEL_H

#include <QAbstractTableModel>
#include <QObject>

#include "parameterdisplayconfig.h"
#include "sensorconfiguration.h"

/*!
 * \brief The SensorParametersModel class
 * Model with all sensor parameters of a sensor configuration
 */
class SensorParametersModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SensorParametersModel(QObject *parent = 0);

    //########################################
    //override methods of abstract table model
    //########################################

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    //#########################
    //get or set display config
    //#########################

    const ParameterDisplayConfig &getParameterDisplayConfig() const;
    void setParameterDisplayConfig(const ParameterDisplayConfig &config);

    //########################
    //get or set sensor config
    //########################

    const oi::SensorConfiguration &getSensorConfiguration() const;
    void setSensorConfiguration(const oi::SensorConfiguration &sConfig);

    //#########################
    //get or set readonly state
    //#########################

    bool getIsReadOnly() const;
    void setIsReadOnly(bool isReadOnly);

signals:

    //##################################
    //inform about sensor config changes
    //##################################

    void sensorConfigurationChanged(const SensorConfiguration &sConfig);

private slots:

    //###################################################
    //update the model when the sensor config has changed
    //###################################################

    void updateModel();

private:

    //#################
    //helper attributes
    //#################

    oi::SensorConfiguration sConfig;
    bool isReadOnly;

    //##############
    //display config
    //##############

    ParameterDisplayConfig parameterDisplayConfig;
    
};

#endif // SENSORPARAMETERSMODEL_H
