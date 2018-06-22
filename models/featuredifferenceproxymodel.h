#ifndef FEATUREDIFFERENCEPROXYMODEL_H
#define FEATUREDIFFERENCEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "oijob.h"
#include "featuredifferencetablemodel.h"

using namespace oi;

/*!
 * \brief The FeatureDifferenceProxyModel class
 */
class FeatureDifferenceProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit FeatureDifferenceProxyModel(QObject *parent = 0);

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

protected:

    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent ) const;
    bool filterAcceptsColumn (int source_column, const QModelIndex &source_parent ) const;

private:

    //###########
    //current job
    //###########

    QPointer<OiJob> job;

};

#endif // FEATUREDIFFERENCEPROXYMODEL_H
