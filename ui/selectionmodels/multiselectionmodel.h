#ifndef MULTISELECTIONMODEL_H
#define MULTISELECTIONMODEL_H

#include <QDebug>
#include <QAbstractItemModel>
#include <QItemSelectionModel>

class MultiSelectionModel : public QItemSelectionModel
{
    Q_OBJECT

public:
    MultiSelectionModel(QAbstractItemModel *myModel);
    ~MultiSelectionModel();

public slots:
    void select(const QItemSelection &selection, SelectionFlags command);
    void select(const QModelIndex &index, QItemSelectionModel::SelectionFlags command);

private:
    bool checkIndex(const QModelIndex &index, const QItemSelection &selection);
};

#endif // MULTISELECTIONMODEL_H
