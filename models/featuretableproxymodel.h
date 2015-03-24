#ifndef FEATUREOVSERVIEWPROXYMODEL_H
#define FEATUREOVSERVIEWPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "featuretablemodel.h"
#include "featurewrapper.h"
#include "oijob.h"

//! model that holds all features except trafo params and filters by selected group name
class FeatureTableProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit FeatureTableProxyModel(QObject *parent = 0);
    
protected:
    bool filterAcceptsColumn ( int source_column, const QModelIndex & source_parent ) const;
    bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    
};

#endif // FEATUREOVSERVIEWPROXYMODEL_H
