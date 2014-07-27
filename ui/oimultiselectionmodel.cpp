#include "oimultiselectionmodel.h"

OiMultiSelectionModel::OiMultiSelectionModel(QAbstractItemModel *myModel) : QItemSelectionModel(myModel)
{
    connect(this, SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(updateSelection(QItemSelection,QItemSelection)));
}

OiMultiSelectionModel::~OiMultiSelectionModel()
{

}

/*!
 * \brief OiMultiSelectionModel::select
 * \param selection
 * \param command
 */
/*void OiMultiSelectionModel::select(const QItemSelection &selection, QItemSelectionModel::SelectionFlags command){

    QModelIndexList selectedIndices = selection.indexes();

    foreach(QModelIndex idx, selectedIndices){

        //QItemSelectionModel::select(idx, QItemSelectionModel::Select);
    }

}*/

void OiMultiSelectionModel::updateSelection(QItemSelection selected, QItemSelection deselected)
{

    /*QModelIndexList selectedIndices;

    foreach(QModelIndex idx, selectedIndices){

        if(this->isSelected(idx)){
            idx.
        }

    }*/

    qDebug() << "check";
}
