#include "availablefunctionslistmodel.h"

/*!
 * \brief AvailableFunctionsListModel::AvailableFunctionsListModel
 * \param parent
 */
AvailableFunctionsListModel::AvailableFunctionsListModel(QObject *parent) : QAbstractListModel(parent){

}

/*!
 * \brief AvailableFunctionsListModel::rowCount
 * \param parent
 * \return
 */
int AvailableFunctionsListModel::rowCount(const QModelIndex &parent) const{

    int rowCount = 0;

    //get the number of function plugins
    foreach(const sdb::Plugin &plugin, this->plugins){
        rowCount += plugin.functions.size();
    }

    return rowCount;

}

/*!
 * \brief AvailableFunctionsListModel::columnCount
 * \param parent
 * \return
 */
int AvailableFunctionsListModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

/*!
 * \brief AvailableFunctionsListModel::data
 * \param index
 * \param role
 * \return
 */
QVariant AvailableFunctionsListModel::data(const QModelIndex &index, int role) const{

    if(!index.isValid())
        return QVariant();

    if(role != Qt::DisplayRole){
        return QVariant();
    }

    int rowCount = 0;

    //get the name of the function at index.row()
    foreach(const sdb::Plugin &plugin, this->plugins){
        foreach(const sdb::Function &function, plugin.functions){
            if(rowCount == index.row()){
                return function.name;
            }
            rowCount++;
        }
    }

    return QVariant();

}

/*!
 * \brief AvailableFunctionsListModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant AvailableFunctionsListModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < 1)){

        return "available functions";
    }
    return QVariant();
}

/*!
 * \brief AvailableFunctionsListModel::getFunctionAtIndex
 * \param index
 * \return
 */
sdb::Function AvailableFunctionsListModel::getFunctionAtIndex(const QModelIndex &index) const{

    sdb::Function result;

    if(!index.isValid())
        return result;

    int rowCount = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){
        foreach(const sdb::Function &function, plugin.functions){
            if(rowCount == index.row()){
                result = function;
                result.plugin = plugin;
                return result;
            }
            rowCount++;
        }
    }

    return result;

}

/*!
 * \brief AvailableFunctionsListModel::getDefaultFunction
 * \param type
 * \return
 */
QPair<QString, QString> AvailableFunctionsListModel::getDefaultFunction(const FeatureTypes &type) const{

    sdb::Function defaultFunction = SystemDbManager::getDefaultFunction(type);
    if(defaultFunction.name.compare("") != 0){
        QPair<QString, QString> result;
        result.first = defaultFunction.name;
        result.second = defaultFunction.plugin.file_path;
        return result;
    }

    return QPair<QString, QString>();

}

/*!
 * \brief AvailableFunctionsListModel::setDefaultFunction
 * \param type
 * \param function
 */
void AvailableFunctionsListModel::setDefaultFunction(const FeatureTypes &type, const QPair<QString, QString> &function){
    SystemDbManager::setDefaultFunction(type, function.first, function.second);
}

/*!
 * \brief AvailableFunctionsListModel::getPlugins
 * \return
 */
const QList<sdb::Plugin> &AvailableFunctionsListModel::getPlugins() const{
    return this->plugins;
}

/*!
 * \brief AvailableFunctionsListModel::setPlugins
 * \param plugins
 */
void AvailableFunctionsListModel::setPlugins(const QList<sdb::Plugin> &plugins){
    this->plugins = plugins;
    this->updateModel();
}

/*!
 * \brief AvailableFunctionsListModel::updateModel
 */
void AvailableFunctionsListModel::updateModel(){
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}
