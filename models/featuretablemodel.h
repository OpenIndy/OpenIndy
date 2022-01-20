#ifndef FEATURETABLEMODEL_H
#define FEATURETABLEMODEL_H

#include <atomic>
#include <QAbstractTableModel>
#include <QObject>
#include <QPointer>
#include <QColor>
#include <QMessageBox>

#include "oijob.h"
#include "types.h"
#include "util.h"
#include "parameterdisplayconfig.h"
#include "console.h"
#include "measurementconfigmanager.h"
#include "pluginloader.h"
#include "systemdbmanager.h"

using namespace oi;

/*!
 * \brief The FeatureTableModel class
 * Model with all features of an OpenIndy job
 */
class FeatureTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit FeatureTableModel(const QPointer<OiJob> &job, QObject *parent = 0);
    explicit FeatureTableModel(QObject *parent = 0);

    //########################################
    //override methods of abstract table model
    //########################################

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    //##############################
    //get id of the feature at index
    //##############################

    int getFeatureIdAtIndex(const QModelIndex &index);

    //#########################
    //get or set active feature
    //#########################

    QPointer<FeatureWrapper> getActiveFeature() const;
    void setActiveFeature(const QModelIndex &index);
    void setActiveFeature(const int &id);

    QPointer<Station> getActiveStation() const;
    void setActiveStation(const QModelIndex &index);
    void setActiveStation(const int &id);

    QPointer<CoordinateSystem> getActiveCoordinateSystem() const;
    void setActiveCoordinateSystem(const QString &name);

    //#######################
    //get or set active group
    //#######################

    const QString getActiveGroupName() const;
    void setActiveGroupName(const QString &group);

    //################################
    //get or set actual nominal filter
    //################################

    const ActualNominalFilter &getActualNominalFilter() const;
    void setActualNominalFilter(const ActualNominalFilter &filter);

    //###############################
    //get or set current OpenIndy job
    //###############################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    //#########################
    //get or set display config
    //#########################

    const ParameterDisplayConfig &getParameterDisplayConfig() const;
    void setParameterDisplayConfig(const ParameterDisplayConfig &config);

    //#########################
    //get or set config manager
    //#########################

    const QPointer<MeasurementConfigManager> &getMeasurementConfigManager() const;
    void setMeasurementConfigManager(const QPointer<MeasurementConfigManager> &mConfigManager);

signals:

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

    void recalcActiveFeature();

private slots:

    //##############################################################
    //update the model when job state or display config have changed
    //##############################################################
    void requestUpdateModel();
    void updateModelIfRequested();
    void updateModel();

private:

    //##############
    //helper methods
    //##############

    QVariant getDisplayValue(const QPointer<FeatureWrapper> &feature, const int &column) const;
    QVariant getBackgroundValue(const QPointer<FeatureWrapper> &feature, const int &column) const;

    void connectJob();
    void disconnectJob();

private:

    //###########
    //current job
    //###########

    QPointer<OiJob> currentJob;

    //##############
    //display config
    //##############

    ParameterDisplayConfig parameterDisplayConfig;

    //##############
    //config manager
    //##############

    QPointer<MeasurementConfigManager> measurementConfigManager;

    //#################
    //helper attributes
    //#################

    ActualNominalFilter actualNominalFilter;
    
    std::atomic<bool> updateRequested;
};

#endif // FEATURETABLEMODEL_H
