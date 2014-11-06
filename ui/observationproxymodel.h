#ifndef OBSERVATIONPROXYMODEL_H
#define OBSERVATIONPROXYMODEL_H

#include <QSortFilterProxyModel>

#include "featurewrapper.h"
#include "guiconfiguration.h"
#include "oifeaturestate.h"

class ObservationProxyModel : public QSortFilterProxyModel
{
public:

    explicit ObservationProxyModel(QObject *parent = 0);

protected:
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

#endif // OBSERVATIONPROXYMODEL_H
