#ifndef OBSERVATIONMODEL_H
#define OBSERVATIONMODEL_H

#include <QAbstractTableModel>

#include "types.h"
#include "util.h"
#include "oijob.h"
#include "parameterdisplayconfig.h"

using namespace oi;

/*!
 * \brief The ObservationModel class
 * Model that represents all information and attributes of the observations of the active feature
 */
class ObservationModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ObservationModel(QObject *parent = 0);

    //########################################
    //override methods of abstract table model
    //########################################

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

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

public slots:

    void setObservationUseStateByContextmenu(bool use, const QModelIndex &index);
    int getObservationIdByIndex(const QModelIndex &index);

signals:

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

    //#########################
    //use or unuse observations
    //#########################

    void setShouldBeUsed(const QPointer<FeatureWrapper> &target, const int &functionIndex, const int &neededElementIndex,
                         const int &elementId, const bool &use, const bool &recalc);

private slots:

    //##############################################################
    //update the model when job state or display config have changed
    //##############################################################

    void activeFeatureChanged();
    void geometryObservationsChanged(const int & featureId);
    void featureRecalculated(const int &featureId);

private:

    //##############
    //helper methods
    //##############

    void updateModel();
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

    QPointer<Observation> getObservation(const QModelIndex &index);
};

#endif // OBSERVATIONMODEL_H
