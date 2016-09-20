#ifndef FEATURETABLETREEPROXYMODEL_H
#define FEATURETABLETREEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "oijob.h"
#include "featuretabletreemodel.h"
#include "featuretablecolumnconfig.h"
#include "featuresorter.h"

using namespace oi;

/*!
 * \brief The FeatureTableTreeProxyModel class
 */
class FeatureTableTreeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit FeatureTableTreeProxyModel(QObject *parent = 0);

    //###############################
    //get or set current OpenIndy job
    //###############################
    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    //#########################
    //get or set display config
    //#########################

    const FeatureTableColumnConfig &getFeatureTableColumnConfig() const;
    void setFeatureTableColumnConfig(const FeatureTableColumnConfig &config);

    //###########################################
    //override methods of sort filter proxy model
    //###########################################

    //QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
    //QModelIndex mapToSource(const QModelIndex &proxyIndex) const;

signals:

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

protected:

    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent ) const;
    bool filterAcceptsColumn (int source_column, const QModelIndex &source_parent ) const;

    //bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:

    //##############
    //display config
    //##############

    FeatureTableColumnConfig featureTableColumnConfig;

    //###########
    //current job
    //###########

    QPointer<OiJob> job;

    //##############
    //feature sorter
    //##############

    FeatureSorter sorter;
};

#endif // FEATURETABLETREEPROXYMODEL_H
