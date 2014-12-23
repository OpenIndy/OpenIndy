#ifndef FEATUREOVSERVIEWPROXYMODEL_H
#define FEATUREOVSERVIEWPROXYMODEL_H

#include <QSortFilterProxyModel>

#include "featurewrapper.h"
#include "guiconfiguration.h"
#include "oifeaturestate.h"

/*!
 * \brief The FeatureTableProxyModel class  is a proxy model that filters all relevant feature attributes.
 * This proxy model only displays geometries, stations and coordinate systems with all their relevant attributes.
 */
class FeatureTableProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit FeatureTableProxyModel(QObject *parent = 0);

    QList<FeatureWrapper*> getFeaturesAtIndices(QModelIndexList &indices);

public slots:
    void activeGroupChanged();
    
protected:
    bool filterAcceptsColumn ( int source_column, const QModelIndex & source_parent ) const;
    bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    void sortNominalToActual();
    
};

#endif // FEATUREOVSERVIEWPROXYMODEL_H
