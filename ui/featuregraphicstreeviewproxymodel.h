#ifndef FEATUREGRAPHICSTREEVIEWPROXYMODEL_H
#define FEATUREGRAPHICSTREEVIEWPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QColor>

#include "featuretreeitem.h"
#include "featurewrapper.h"
#include "oifeaturestate.h"

class FeatureGraphicsTreeViewProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit FeatureGraphicsTreeViewProxyModel(QObject *parent = 0);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setHeader(QString header);

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QModelIndex getSourceIndex(const QModelIndex & index);

protected:
    bool filterAcceptsColumn ( int source_column, const QModelIndex & source_parent ) const;
    bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;

private:
    QString header;

};

#endif // FEATUREGRAPHICSTREEVIEWPROXYMODEL_H
