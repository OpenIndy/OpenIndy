#ifndef PLUGINTREEVIEWMODEL_H
#define PLUGINTREEVIEWMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QPixmap>

#include "plugintreeitem.h"
#include "systemdbmanager.h"

class PluginTreeViewModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit PluginTreeViewModel(QObject *parent = 0);

    QModelIndex	index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex	parent(const QModelIndex & index) const;
    int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void refreshModel();

private:
    PluginTreeItem *rootItem;
};

#endif // PLUGINTREEVIEWMODEL_H
