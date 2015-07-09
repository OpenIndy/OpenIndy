#ifndef TRAFOPARAMTABLEPROXYMODEL_H
#define TRAFOPARAMTABLEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "oijob.h"
#include "featuretablemodel.h"
#include "trafoparamtablecolumnconfig.h"

using namespace oi;

/*!
 * \brief The TrafoParamTableProxyModel class
 */
class TrafoParamTableProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit TrafoParamTableProxyModel(QObject *parent = 0);

    //#########################
    //get or set display config
    //#########################

    const TrafoParamTableColumnConfig &getTrafoParamTableColumnConfig() const;
    void setTrafoParamTableColumnConfig(const TrafoParamTableColumnConfig &config);

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
    
};

#endif // TRAFOPARAMTABLEPROXYMODEL_H
