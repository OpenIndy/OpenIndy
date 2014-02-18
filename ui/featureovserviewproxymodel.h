#ifndef FEATUREOVSERVIEWPROXYMODEL_H
#define FEATUREOVSERVIEWPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "featurewrapper.h"

class FeatureOvserviewProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:

    QList<FeatureWrapper*> &features;

    explicit FeatureOvserviewProxyModel(QList<FeatureWrapper*> &features,QObject *parent = 0);
    
protected:
    bool filterAcceptsColumn ( int source_column, const QModelIndex & source_parent ) const;
    bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;
    
};

#endif // FEATUREOVSERVIEWPROXYMODEL_H
