#ifndef AVAILABLEFUNCTIONSLISTPROXYMODEL_H
#define AVAILABLEFUNCTIONSLISTPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "oijob.h"
#include "types.h"
#include "util.h"
#include "availablefunctionslistmodel.h"
#include "oimetadata.h"
#include "pluginloader.h"

/*!
 * \brief The AvailableFunctionsListProxyModel class
 * Model that holds all available functions filtered by feature type
 */
class AvailableFunctionsListProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit AvailableFunctionsListProxyModel(QObject *parent = 0);

    //######################
    //filter by feature type
    //######################

    void setFilter(const FeatureTypes &type);

    //#########################
    //get the function at index
    //#########################

    sdb::Function getFunctionAtIndex(const int &index) const;
    
protected:

    //###########################################
    //override methods of sort filter proxy model
    //###########################################

    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent ) const;
    bool filterAcceptsColumn (int source_column, const QModelIndex &source_parent ) const;
    
private:

    //#################
    //helper attributes
    //#################

    FeatureTypes filter;

};

#endif // AVAILABLEFUNCTIONSLISTPROXYMODEL_H
