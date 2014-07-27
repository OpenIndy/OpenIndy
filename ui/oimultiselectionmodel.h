#ifndef OIMULTISELECTIONMODEL_H
#define OIMULTISELECTIONMODEL_H

#include <QDebug>
#include <QAbstractItemModel>
#include <QItemSelectionModel>

class OiMultiSelectionModel : public QItemSelectionModel
{
    Q_OBJECT
public:
    OiMultiSelectionModel(QAbstractItemModel *myModel);
    ~OiMultiSelectionModel();

public slots:
    //void select(const QItemSelection &selection, SelectionFlags command);

    void updateSelection(QItemSelection,QItemSelection);
};

#endif // OIMULTISELECTIONMODEL_H
