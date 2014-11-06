#ifndef OBSERVATIONPROXYMODEL_H
#define OBSERVATIONPROXYMODEL_H

#include <QSortFilterProxyModel>

#include "featurewrapper.h"
#include "guiconfiguration.h"
#include "oifeaturestate.h"

/*!
 * \brief The ObservationProxyModel class handles the displayed observations. Which attributes should be displayed is defined in the GUICOnfiguration class. Changes only should be done
 *  there, to avoid conflicts.
 */
class ObservationProxyModel : public QSortFilterProxyModel
{
public:

    explicit ObservationProxyModel(QObject *parent = 0);

protected:
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

#endif // OBSERVATIONPROXYMODEL_H
