#ifndef TRAFOPARAMPROXYMODEL_H
#define TRAFOPARAMPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "featuretablemodel.h"
#include "featurewrapper.h"
#include "guiconfiguration.h"
#include "oijob.h"

class TrafoParamProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit TrafoParamProxyModel(QObject *parent = 0);

protected:
    bool filterAcceptsColumn ( int source_column, const QModelIndex & source_parent) const;
    bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent) const;

};

#endif // TRAFOPARAMPROXYMODEL_H
