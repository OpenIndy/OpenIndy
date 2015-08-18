#include "observationproxymodel.h"

/*!
 * \brief ObservationProxyModel::ObservationProxyModel
 * \param parent
 */
ObservationProxyModel::ObservationProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

/*!
 * \brief ObservationProxyModel::getObservationTableColumnConfig
 * \return
 */
const ObservationTableColumnConfig &ObservationProxyModel::getObservationTableColumnConfig() const{
    return this->observationTableColumnConfig;
}

/*!
 * \brief ObservationProxyModel::setObservationTableColumnConfig
 * \param config
 */
void ObservationProxyModel::setObservationTableColumnConfig(const ObservationTableColumnConfig &config){
    this->observationTableColumnConfig = config;
    this->invalidateFilter();
}

/*!
 * \brief ObservationProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * \return
 */
bool ObservationProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{
    return true;
}

/*!
 * \brief ObservationProxyModel::filterAcceptsColumn
 * \param source_column
 * \param source_parent
 * \return
 */
bool ObservationProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{

    //check visibility of the column
    if(this->observationTableColumnConfig.getColumnVisibility((ObservationDisplayAttributes)source_column)){
        return true;
    }
    return false;

}
