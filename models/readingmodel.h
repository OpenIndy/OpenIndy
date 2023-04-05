#ifndef READINGMODEL_H
#define READINGMODEL_H

#include <QAbstractTableModel>

#include "types.h"
#include "util.h"
#include "oijob.h"
#include "parameterdisplayconfig.h"

using namespace oi;

/*!
 * \brief The ReadingModel class
 */
class ReadingModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ReadingModel(QObject *parent = 0);

    //########################################
    //override methods of abstract table model
    //########################################

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

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

signals:

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

private slots:

    //##############################################################
    //update the model when job state or display config have changed
    //##############################################################

    void activeFeatureChanged();
    void geometryObservationsChanged(const int & featureId);

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

};

#endif // READINGMODEL_H
