#ifndef FEATURETABLEPROXYMODEL_H
#define FEATURETABLEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "oijob.h"
#include "featuretablemodel.h"
#include "featuretablecolumnconfig.h"

/*!
 * \brief The FeatureTableProxyModel class
 */
class FeatureTableProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit FeatureTableProxyModel(QObject *parent = 0);

    //#########################
    //get or set display config
    //#########################

    const FeatureTableColumnConfig &getFeatureTableColumnConfig() const;
    void setFeatureTableColumnConfig(const FeatureTableColumnConfig &config);

    //##############################
    //get id of the feature at index
    //##############################

    //int getFeatureIdAtIndex(const QModelIndex &index);
    
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

    FeatureTableColumnConfig featureTableColumnConfig;
    
};

#endif // FEATURETABLEPROXYMODEL_H
