#include "featuregraphicstreeviewproxymodel.h"

FeatureGraphicsTreeViewProxyModel::FeatureGraphicsTreeViewProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

/*!
 * \brief FeatureGraphicsTreeViewProxyModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant FeatureGraphicsTreeViewProxyModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        return header;
    }
    return QVariant();
}

/*!
 * \brief FeatureGraphicsTreeViewProxyModel::setHeader
 * \param header
 */
void FeatureGraphicsTreeViewProxyModel::setHeader(QString header){
    this->header = header;
}

/*!
 * \brief FeatureGraphicsTreeViewProxyModel::data
 * \param index
 * \param role
 * \return
 */
QVariant FeatureGraphicsTreeViewProxyModel::data(const QModelIndex &index, int role) const{
    QModelIndex source_index = this->mapToSource(index);
    if (role == Qt::BackgroundRole){
        if(source_index.isValid()){
            FeatureTreeItem *item = static_cast<FeatureTreeItem*>(source_index.internalPointer());
            if(item != NULL && item->getIsFeature()){
                if(OiFeatureState::getActiveFeature() != NULL && item->getFeature()->getFeature() == OiFeatureState::getActiveFeature()->getFeature()){
                    return QColor(Qt::green);
                }
                if(item->getFeature()->getStation() != NULL){
                    return QColor(Qt::lightGray);
                }
            }
        }
    }
    return this->sourceModel()->data(source_index, role);
}

/*!
 * \brief FeatureGraphicsTreeViewProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * \return
 */
bool FeatureGraphicsTreeViewProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{
    return true;
}

/*!
 * \brief FeatureGraphicsTreeViewProxyModel::filterAcceptsColumn
 * \param source_column
 * \param source_parent
 * \return
 */
bool FeatureGraphicsTreeViewProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{
    return true;
}
