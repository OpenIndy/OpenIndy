#ifndef READINGPROXYMODEL_H
#define READINGPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "guiconfiguration.h"
#include "geometry.h"
#include "oifeaturestate.h"

/*!
 * \brief The ReadingProxyModel class handles the displayed readings. Which attributes should be displayed is defined in the GUICOnfiguration class. Changes only should be done
 *  there, to avoid conflicts. Only one reading type can be displayed at the same time. All used reading type of a geometry are displayed in a combobox in the feature info dialog.
 */
class ReadingProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ReadingProxyModel(QObject *parent = 0);

protected:
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

signals:

public slots:

};

#endif // READINGPROXYMODEL_H
