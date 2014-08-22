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
 * Only select items whose parents are not selected
 * \param selection
 * \param command
 */
void OiMultiSelectionModel::select(const QItemSelection &selection, QItemSelectionModel::SelectionFlags command){

    //if a single item gets selected...
    if( !command.testFlag(QItemSelectionModel::NoUpdate)
            && command.testFlag(QItemSelectionModel::Clear)
            && command.testFlag(QItemSelectionModel::Select)
            && !command.testFlag(QItemSelectionModel::Deselect)
            && !command.testFlag(QItemSelectionModel::Toggle)
            && !command.testFlag(QItemSelectionModel::Current)
            && command.testFlag(QItemSelectionModel::Rows)
            && !command.testFlag(QItemSelectionModel::Columns) ){

        //...accept the selection
        QItemSelectionModel::select(selection, command);
        return;

    }

    //if multiple items get selected by shift...
    if( !command.testFlag(QItemSelectionModel::NoUpdate)
            && !command.testFlag(QItemSelectionModel::Clear)
            && command.testFlag(QItemSelectionModel::Select)
            && !command.testFlag(QItemSelectionModel::Deselect)
            && !command.testFlag(QItemSelectionModel::Toggle)
            && command.testFlag(QItemSelectionModel::Current)
            && command.testFlag(QItemSelectionModel::Rows)
            && !command.testFlag(QItemSelectionModel::Columns) ){

        //...check the selected indices for no duplicates on different levels
        QItemSelection newSelection;
        foreach(QModelIndex index, selection.indexes()){
            if(this->checkIndex(index, selection)){
                QItemSelectionRange range(index);
                newSelection.append(range);
            }
        }
        foreach(QModelIndex index, this->selection().indexes()){
            if(this->checkIndex(index, selection)){
                QItemSelectionRange range(index);
                newSelection.append(range);
            }
        }
        QItemSelectionModel::select(newSelection, QItemSelectionModel::Clear | QItemSelectionModel::Select
                                    | QItemSelectionModel::Current | QItemSelectionModel::Rows);
        return;

    }

    //if multiple items get selected by mouse marking...
    if( !command.testFlag(QItemSelectionModel::NoUpdate)
            && command.testFlag(QItemSelectionModel::Clear)
            && command.testFlag(QItemSelectionModel::Select)
            && !command.testFlag(QItemSelectionModel::Deselect)
            && !command.testFlag(QItemSelectionModel::Toggle)
            && command.testFlag(QItemSelectionModel::Current)
            && command.testFlag(QItemSelectionModel::Rows)
            && !command.testFlag(QItemSelectionModel::Columns) ){

        //...check the selected indices for no duplicates on different levels
        QItemSelection newSelection;
        foreach(QModelIndex index, selection.indexes()){
            if(this->checkIndex(index, selection)){
                QItemSelectionRange range(index);
                newSelection.append(range);
            }
        }
        QItemSelectionModel::select(newSelection, command);
        return;

    }

    //if an item gets selected while current selection stays (ctrl)...
    if( !command.testFlag(QItemSelectionModel::NoUpdate)
            && !command.testFlag(QItemSelectionModel::Clear)
            && command.testFlag(QItemSelectionModel::Select)
            && !command.testFlag(QItemSelectionModel::Deselect)
            && !command.testFlag(QItemSelectionModel::Toggle)
            && !command.testFlag(QItemSelectionModel::Current)
            && command.testFlag(QItemSelectionModel::Rows)
            && !command.testFlag(QItemSelectionModel::Columns) ){

        //...check the selected indices for no duplicates on different levels
        QItemSelection newSelection;
        foreach(QModelIndex index, selection.indexes()){
            if(this->checkIndex(index, selection)){
                QItemSelectionRange range(index);
                newSelection.append(range);
            }
        }
        foreach(QModelIndex index, this->selection().indexes()){
            if(this->checkIndex(index, selection)){
                QItemSelectionRange range(index);
                newSelection.append(range);
            }
        }
        QItemSelectionModel::select(newSelection, QItemSelectionModel::Clear | QItemSelectionModel::Select
                                    | QItemSelectionModel::Current | QItemSelectionModel::Rows);
        return;

    }

    //if further items get selected by ctrl + mouse marking
    if( !command.testFlag(QItemSelectionModel::NoUpdate)
            && !command.testFlag(QItemSelectionModel::Clear)
            && !command.testFlag(QItemSelectionModel::Select)
            && !command.testFlag(QItemSelectionModel::Deselect)
            && command.testFlag(QItemSelectionModel::Toggle)
            && command.testFlag(QItemSelectionModel::Current)
            && command.testFlag(QItemSelectionModel::Rows)
            && !command.testFlag(QItemSelectionModel::Columns) ){

        //...check the selected indices for no duplicates on different levels
        QItemSelection newSelection;
        foreach(QModelIndex index, selection.indexes()){
            if(this->checkIndex(index, selection)){
                QItemSelectionRange range(index);
                newSelection.append(range);
            }
        }
        foreach(QModelIndex index, this->selection().indexes()){
            if(this->checkIndex(index, selection)){
                QItemSelectionRange range(index);
                newSelection.append(range);
            }
        }
        QItemSelectionModel::select(newSelection, QItemSelectionModel::Clear | QItemSelectionModel::Select
                                    | QItemSelectionModel::Current | QItemSelectionModel::Rows);
        return;

    }

    //default
    QItemSelectionModel::select(selection, command);
    return;

}

/*!
 * \brief OiMultiSelectionModel::select
 * \param index
 * \param command
 */
void OiMultiSelectionModel::select(const QModelIndex &index, QItemSelectionModel::SelectionFlags command){

    //call parent select method
    QItemSelectionModel::select(index, command);

}

/*!
 * \brief OiMultiSelectionModel::checkIndex
 * Checks wether one of the parents of index is selected or gets selected, too. Returns true if no parent gets selected
 * \param index
 * \param selection
 * \return
 */
bool OiMultiSelectionModel::checkIndex(const QModelIndex &index, const QItemSelection &selection){

    QModelIndex indexHelper = index;
    while(indexHelper.parent().isValid()){
        if(selection.contains(indexHelper.parent()) || this->selection().contains(indexHelper.parent())){
            return false;
        }
        indexHelper = indexHelper.parent();
    }

    return true;

}
