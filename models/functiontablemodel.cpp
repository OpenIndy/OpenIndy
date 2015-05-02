#include "functiontablemodel.h"

/*!
 * \brief FeatureTableModel::FeatureTableModel
 * \param parent
 */
FunctionTableModel::FunctionTableModel(QObject *parent) : QAbstractTableModel(parent){

}

/*!
 * \brief FunctionTableModel::rowCount
 * \param parent
 * \return
 */
int FunctionTableModel::rowCount(const QModelIndex &parent) const{

    int numFunctionPlugins = 0;

    foreach(const sdb::Plugin &plugin, this->plugins){
        numFunctionPlugins += plugin.functions.size();
    }

    return numFunctionPlugins;

}

/*!
 * \brief FunctionTableModel::columnCount
 * \param parent
 * \return
 */
int FunctionTableModel::columnCount(const QModelIndex &parent) const{
    return 3;
}

/*!
 * \brief FunctionTableModel::data
 * \param index
 * \param role
 * \return
 */
QVariant FunctionTableModel::data(const QModelIndex &index, int role) const{

    //check model index
    if(!index.isValid()){
        return QVariant();
    }

    //check role
    if(role != Qt::DisplayRole){
        return QVariant();
    }

    int numFunctionPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all function of the current plugin
        foreach(const sdb::Function &function, plugin.functions){

            //if the current function corresponds to the given row index
            if(index.row() == numFunctionPlugins){

                if(index.column() == 0){
                    return function.name;
                }else if(index.column() == 1){
                    return plugin.name;
                }else if(index.column() == 2){
                    return function.iid;
                }

            }

            numFunctionPlugins++;
        }

    }

    return QVariant();

}

/*!
 * \brief FunctionTableModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant FunctionTableModel::headerData(int section, Qt::Orientation orientation, int role) const{

    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        if(section == 0){
            return "function";
        }else if(section == 1){
            return "plugin";
        }else if(section == 2){
            return "iid";
        }

    }

    return QVariant();

}

/*!
 * \brief FunctionTableModel::getPlugins
 * \return
 */
const QList<sdb::Plugin> &FunctionTableModel::getPlugins() const{
    return plugins;
}

/*!
 * \brief FunctionTableModel::setPlugins
 * \param plugins
 */
void FunctionTableModel::setPlugins(const QList<sdb::Plugin> &plugins){
    emit this->beginResetModel();
    this->plugins = plugins;
    emit this->endResetModel();
    this->updateModel();
}

/*!
 * \brief FunctionTableModel::updateModel
 */
void FunctionTableModel::updateModel(){
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

/*!
 * \brief FunctionTableModel::connectJob
 */
void FunctionTableModel::connectJob(){

}

/*!
 * \brief FunctionTableModel::disconnectJob
 */
void FunctionTableModel::disconnectJob(){

}
