#ifndef BUNDLESYSTEMSMODEL_H
#define BUNDLESYSTEMSMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>
#include <QPixmap>
#include <QPointer>

#include "types.h"
#include "util.h"
#include "oijob.h"

using namespace oi;

/*!
 * \brief The BundleSystemsModel class
 * Model that holds all available bundle systems
 */
class BundleSystemsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BundleSystemsModel(QObject *parent = 0);

    //#######################################
    //override methods of abstract item model
    //#######################################

    int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    //###############################
    //get or set current OpenIndy job
    //###############################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    //###################
    //get selected system
    //###################

    int getSelectedBundleSystem(const QModelIndex &index);

signals:

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

private slots:

    //###########################################
    //update the model when bundle systems change
    //###########################################

    void updateModel();

private:

    //##############
    //helper methods
    //##############

    void connectJob();
    void disconnectJob();

    //callbacks OiJob
    void featureSetChanged();

private:

    //##############
    //helper methods
    //##############

    QList<QPointer<CoordinateSystem> > bundleSystems;

    //###########
    //current job
    //###########

    QPointer<OiJob> currentJob;

};

#endif // BUNDLESYSTEMSMODEL_H
