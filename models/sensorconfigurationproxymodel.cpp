#include "sensorconfigurationproxymodel.h"

/*!
 * \brief SensorConfigurationProxyModel::SensorConfigurationProxyModel
 * \param parent
 */
SensorConfigurationProxyModel::SensorConfigurationProxyModel(QObject *parent) : QSortFilterProxyModel(parent){
    this->showAll = false;
}

/*!
 * \brief SensorConfigurationProxyModel::setFilter
 * \param showAll
 */
void SensorConfigurationProxyModel::setFilter(const bool &showAll){
    this->showAll = showAll;
    this->invalidateFilter();
}

/*!
 * \brief SensorConfigurationProxyModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant SensorConfigurationProxyModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        return "sensor configurations";

    }
    return QVariant();
}

/*!
 * \brief SensorConfigurationProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * \return
 */
bool SensorConfigurationProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{

    //get and check source model
    SensorConfigurationModel *sourceModel = static_cast<SensorConfigurationModel *>(this->sourceModel());
    if(sourceModel == NULL){
        return false;
    }

    //check if the index is a saved config
    if(source_row < sourceModel->getSensorConfigurationManager()->getSavedSensorConfigs().size()){
        return true;
    }

    //check if the index is a project sensor config
    if(source_row < this->rowCount(source_parent) && this->showAll == true){
        return true;
    }

    return false;

}

/*!
 * \brief SensorConfigurationProxyModel::filterAcceptsColumn
 * \param source_row
 * \param source_parent
 * \return
 */
bool SensorConfigurationProxyModel::filterAcceptsColumn(int source_row, const QModelIndex &source_parent) const{
    return true;
}
