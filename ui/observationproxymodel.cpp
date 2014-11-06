#include "observationproxymodel.h"

ObservationProxyModel::ObservationProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

/*!
 * \brief :filterAcceptsColumn filters the displayed columns
 * \param source_column
 * \param source_parent
 * \return
 */
bool ObservationProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    QList<int> displayColumns = GUIConfiguration::displayAttributes(GUIConfiguration::obsAttributes,GUIConfiguration::allObsAttributes);
    if(displayColumns.contains(source_column)){
        return true;
    }else{
        return false;
    }
}

/*!
 * \brief filterAcceptsRow filters rows in observation model
 * \param source_row
 * \param source_parent
 * \return
 */
bool ObservationProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    //allow all observations to be displayed
    return true;
}
