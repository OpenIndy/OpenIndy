#ifndef USEDELEMENTSMODEL_H
#define USEDELEMENTSMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QPixmap>
#include <QList>
#include "featuretreeitem.h"

class UsedElementsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit UsedElementsModel(QObject *parent = 0);

    QModelIndex	index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex	parent(const QModelIndex & index) const;
    int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void refreshModel();
    void addElement(FeatureTreeItem *item);
    void removeElement(FeatureTreeItem *item);
    void removeAllElements();

    FeatureTreeItem *getSelectedItem(const QModelIndex &index);

private:
    QList<FeatureTreeItem*> items;
    FeatureTreeItem *rootItem;
    FeatureTreeItem *selectedItem;
};

#endif // USEDELEMENTSMODEL_H
