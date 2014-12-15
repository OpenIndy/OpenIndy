#ifndef GEOMETRYTYPESPROXYMODEL_H
#define GEOMETRYTYPESPROXYMODEL_H

#include <QSortFilterProxyModel>

#include "configuration.h"

class GeometryTypesProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit GeometryTypesProxyModel(QObject *parent = 0);

    void setFilter(QList<Configuration::GeometryTypes> myFilter);

protected:
    bool filterAcceptsColumn ( int source_column, const QModelIndex & source_parent ) const;
    bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;

private:
    QList<Configuration::GeometryTypes> myFilter; //geometry types that shall be displayed

};

#endif // GEOMETRYTYPESPROXYMODEL_H
