#include "availablefunctionslistproxymodel.h"

/*!
 * \brief AvailableFunctionsListProxyModel::AvailableFunctionsListProxyModel
 * \param parent
 */
AvailableFunctionsListProxyModel::AvailableFunctionsListProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

/*!
 * \brief AvailableFunctionsListProxyModel::setFilter
 * \param type
 */
void AvailableFunctionsListProxyModel::setFilter(const FeatureTypes &type){
    this->filter = type;
    this->invalidateFilter();
}

/*!
 * \brief AvailableFunctionsListProxyModel::getFunctionAtIndex
 * \param index
 * \return
 */
sdb::Function AvailableFunctionsListProxyModel::getFunctionAtIndex(const int &index) const{

    sdb::Function result;

    //check index
    if(index < 0){
        return result;
    }

    //get and cast source model
    AvailableFunctionsListModel *source_model = dynamic_cast<AvailableFunctionsListModel *>(this->sourceModel());
    if(source_model == NULL){
        return result;
    }

    //get the function at index
    QModelIndex functionIndex = this->index(index, 0);
    QModelIndex sourceIndex = this->mapToSource(functionIndex);
    sdb::Function currentFunction = source_model->getFunctionAtIndex(source_model->index(sourceIndex.row(), 0));
    if(currentFunction.name.compare("") == 0){
        return result;
    }

    return currentFunction;

}

/*!
 * \brief AvailableFunctionsListProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableFunctionsListProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{

    //get and cast source model
    AvailableFunctionsListModel *source_model = dynamic_cast<AvailableFunctionsListModel *>(this->sourceModel());
    if(source_model == NULL){
        return false;
    }

    //get a list of available first function for the specified feature type
    QList<sdb::Function> functions = SystemDbManager::getCreateFunctions(this->filter);
    if(functions.size() == 0){
        return false;
    }

    //get the function at index
    sdb::Function currentFunction = source_model->getFunctionAtIndex(source_model->index(source_row, 0));
    if(currentFunction.name.compare("") == 0){
        return false;
    }

    //check if the current function is a create function for the specified feature type
    foreach(const sdb::Function &function, functions){
        if(function.name.compare(currentFunction.name) == 0
                && function.plugin.name.compare(currentFunction.plugin.name) == 0){
            return true;
        }
    }

    return false;

}

/*!
 * \brief AvailableFunctionsListProxyModel::filterAcceptsColumn
 * \param source_column
 * \param source_parent
 * \return
 */
bool AvailableFunctionsListProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{
    return true;
}
