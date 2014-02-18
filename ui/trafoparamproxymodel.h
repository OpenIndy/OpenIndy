#ifndef TRAFOPARAMPROXYMODEL_H
#define TRAFOPARAMPROXYMODEL_H

#include "featurewrapper.h"
#include <QSortFilterProxyModel>

class TrafoParamProxyModel : public QSortFilterProxyModel
{
public:

    QList<FeatureWrapper*> &features;

    explicit TrafoParamProxyModel(QList<FeatureWrapper*> &features,QObject *parent = 0);

protected:
    bool filterAcceptsColumn ( int source_column, const QModelIndex & source_parent ) const;
    bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;
};

#endif // TRAFOPARAMPROXYMODEL_H
