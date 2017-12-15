#ifndef READINGPROXYMODEL_H
#define READINGPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "oijob.h"
#include "readingmodel.h"
#include "readingtablecolumnconfig.h"

using namespace oi;

/*!
 * \brief The ReadingProxyModel class
 */
class ReadingProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit ReadingProxyModel(QObject *parent = 0);

    //#########################
    //get or set display config
    //#########################

    const ReadingTableColumnConfig &getReadingTableColumnConfig() const;
    void setReadingTableColumnConfig(const ReadingTableColumnConfig &config);

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
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;

private:

    //##############
    //display config
    //##############

    ReadingTableColumnConfig readingTableColumnConfig;

};

#endif // READINGPROXYMODEL_H
