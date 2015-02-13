#include "geometrytypesproxymodel.h"

GeometryTypesProxyModel::GeometryTypesProxyModel(QObject *parent)
{

}

/*!
 * \brief GeometryTypesProxyModel::setFilter
 * \param myFilter
 */
void GeometryTypesProxyModel::setFilter(QList<Configuration::GeometryTypes> myFilter){

    this->myFilter = myFilter;

    this->invalidateFilter();

}

/*!
 * \brief GeometryTypesProxyModel::filterAcceptsColumn
 * \param source_column
 * \param source_parent
 * \return
 */
bool GeometryTypesProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{

    return true;

}

/*!
 * \brief GeometryTypesProxyModel::filterAcceptsRow
 * Only display geometry types that are within the myFilter-list
 * \param source_row
 * \param source_parent
 * \return
 */
bool GeometryTypesProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{

    Configuration::GeometryTypes typeOfGeometry = (Configuration::GeometryTypes)source_row;

    if(this->myFilter.contains(typeOfGeometry)){
        return true;
    }
    return false;

}
