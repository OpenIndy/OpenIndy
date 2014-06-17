#ifndef TRAFOPARAMPROXYMODEL_H
#define TRAFOPARAMPROXYMODEL_H

#include <QSortFilterProxyModel>

#include "featurewrapper.h"
#include "guiconfiguration.h"
#include "oifeaturestate.h"

class TrafoParamProxyModel : public QSortFilterProxyModel
{
public:

    explicit TrafoParamProxyModel(QObject *parent = 0);

    QList<FeatureWrapper*> getFeaturesAtIndices(QModelIndexList &indices);

protected:
    bool filterAcceptsColumn ( int source_column, const QModelIndex & source_parent ) const;
    bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;
};

#endif // TRAFOPARAMPROXYMODEL_H
