#ifndef FEATUREDIFFERENCETABLEMODEL_H
#define FEATUREDIFFERENCETABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QPointer>
#include <QColor>

#include "oijob.h"
#include "types.h"
#include "util.h"

using namespace oi;

/*!
 * \brief The FeatureDifferenceTableModel class
 */
class FeatureDifferenceTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit FeatureDifferenceTableModel(const QPointer<OiJob> &job, QObject *parent = 0);
    explicit FeatureDifferenceTableModel(QObject *parent = 0);

    //########################################
    //override methods of abstract table model
    //########################################

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);


    //###############################
    //get or set current OpenIndy job
    //###############################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

signals:

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

private slots:

    //##############################################################
    //update the model when job state or display config have changed
    //##############################################################

    void updateModel();

private:
    //###########
    //current job
    //###########
    void connectJob();
    void disconnectJob();

    QPointer<OiJob> currentJob;
};

#endif // FEATUREDIFFERENCETABLEMODEL_H
