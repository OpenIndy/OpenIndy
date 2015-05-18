#ifndef READINGPROXYMODEL_H
#define READINGPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "oijob.h"
#include "readingmodel.h"
#include "readingtablecolumnconfig.h"

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

protected:

    //###########################################
    //override methods of sort filter proxy model
    //###########################################

    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent ) const;
    bool filterAcceptsColumn (int source_column, const QModelIndex &source_parent ) const;

private:

    //##############
    //display config
    //##############

    ReadingTableColumnConfig readingTableColumnConfig;

};

#endif // READINGPROXYMODEL_H
