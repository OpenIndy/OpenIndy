#include "matrixmodel.h"

MatrixModel::MatrixModel(QObject *parent):QAbstractTableModel(parent)
{

}

int MatrixModel::rowCount(const QModelIndex &parent) const
{
        return this->matrix.getRowCount();
}

int MatrixModel::columnCount(const QModelIndex &parent) const
{
    return this->matrix.getColCount();
}

QVariant MatrixModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(Qt::DisplayRole == role){

        return QString::number(this->matrix.getAt(index.row(),index.column()),'f',6);
    }

    return QVariant();
}

void MatrixModel::updateModel(OiMat matrix)
{
    this->matrix = matrix;
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}
