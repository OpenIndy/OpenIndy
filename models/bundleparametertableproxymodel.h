#ifndef BUNDLEPARAMETERTABLEPROXYMODEL_H
#define BUNDLEPARAMETERTABLEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "oijob.h"
#include "featuretablemodel.h"
#include "trafoparamtablecolumnconfig.h"

using namespace oi;

/*!
 * \brief The BundleParameterTableProxyModel class
 */
class BundleParameterTableProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit BundleParameterTableProxyModel(QObject *parent = 0);

    //#########################
    //get or set display config
    //#########################

    const TrafoParamTableColumnConfig &getTrafoParamTableColumnConfig() const;
    void setTrafoParamTableColumnConfig(const TrafoParamTableColumnConfig &config);

    //###########################################
    //override methods of sort filter proxy model
    //###########################################

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const;

signals:

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);
    
protected:

    //###########################################
    //override methods of sort filter proxy model
    //###########################################

    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent ) const;
    bool filterAcceptsColumn (int source_column, const QModelIndex &source_parent ) const;

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:

    //##############
    //display config
    //##############

    TrafoParamTableColumnConfig trafoParamTableColumnConfig;

    //#############
    //helper method
    //#############

    bool isBundleStationTrafo(const QPointer<TrafoParam> &param) const;
};

#endif // BUNDLEPARAMETERTABLEPROXYMODEL_H
