#ifndef READINGPROXYMODEL_H
#define READINGPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "guiconfiguration.h"
#include "geometry.h"
#include "oifeaturestate.h"

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
