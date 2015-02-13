#ifndef MATRIXMODEL_H
#define MATRIXMODEL_H

#include <QAbstractTableModel>
#include "oimat.h"

class MatrixModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit MatrixModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    OiMat matrix;

signals:

public slots:
    void updateModel(OiMat matrix);
};

#endif // MATRIXMODEL_H
