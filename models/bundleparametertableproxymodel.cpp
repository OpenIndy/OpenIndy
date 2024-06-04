#include "bundleparametertableproxymodel.h"

/*!
 * \brief BundleParameterTableProxyModel::TrafoParamTableProxyModel
 * \param parent
 */
BundleParameterTableProxyModel::BundleParameterTableProxyModel(QObject *parent) : QSortFilterProxyModel(parent), trafoParamTableColumnConfig(true)
{
}

/*!
 * \brief BundleParameterTableProxyModel::mapFromSource
 * \param sourceIndex
 * \return
 */
QModelIndex BundleParameterTableProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    //get and check proxyIndex
    QModelIndex proxyIndex = QSortFilterProxyModel::mapFromSource(sourceIndex);
    if(!proxyIndex.isValid()){
        return proxyIndex;
    }

    //set up new column position (column sorting)
    int columnIndex = sourceIndex.column();
    int columnPosition = this->trafoParamTableColumnConfig.getColumnPosition((TrafoParamDisplayAttributes)getFeatureDisplayAttributes().at(columnIndex));
    //proxyIndex = this->createIndex(proxyIndex.row(), columnPosition);
    return this->index(proxyIndex.row(), columnPosition);

    return proxyIndex;
}

/*!
 * \brief BundleParameterTableProxyModel::mapToSource
 * \param proxyIndex
 * \return
 */
QModelIndex BundleParameterTableProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    //get and check sourceIndex
    QModelIndex sourceIndex = QSortFilterProxyModel::mapToSource(proxyIndex);
    if(!sourceIndex.isValid()){
        return sourceIndex;
    }

    //get and cast source model
    FeatureTableModel *source_model = dynamic_cast<FeatureTableModel *>(this->sourceModel());
    if(source_model == NULL){
        return QModelIndex();
    }

    //get display attribute at column position
    TrafoParamDisplayAttributes attr = this->trafoParamTableColumnConfig.getDisplayAttributeAt(proxyIndex.column());

    //get the index of the attribute in the list of all available attributes
    int sourceColumn = getFeatureDisplayAttributes().indexOf(attr);

    //set up correct column position for the source index
    sourceIndex = this->createIndex(sourceIndex.row(), sourceColumn);

    return source_model->index(sourceIndex.row(), sourceColumn);

    return sourceIndex;
}

/*!
 * \brief BundleParameterTableProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * \return
 */
bool BundleParameterTableProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{

    //get and cast source model
    FeatureTableModel *source_model = dynamic_cast<FeatureTableModel *>(this->sourceModel());
    if(source_model == NULL){
        return false;
    }

    //check if job is set
    if(source_model->getCurrentJob().isNull()){
        return false;
    }

    //check if the feature at index source_row exists
    if(source_model->getCurrentJob()->getFeatureCount() <= source_row){
        return false;
    }

    //get and check the feature
    QPointer<FeatureWrapper> feature = source_model->getCurrentJob()->getFeaturesList().at(source_row);
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //if the feature is no trafo param reject it
    if(feature->getTrafoParam().isNull()){
        return false;
    }

    //check if trafo param is bundle trafo param
    if(!this->isBundleStationTrafo(feature->getTrafoParam())){
        return false;
    }

    return true;
}

/*!
 * \brief BundleParameterTableProxyModel::filterAcceptsColumn
 * \param source_column
 * \param source_parent
 * \return
 */
bool BundleParameterTableProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{

    //get and check display attribute
    if(getFeatureDisplayAttributes().size() < source_column){
        return false;
    }
    int attr = getFeatureDisplayAttributes().at(source_column);

    //check if the attribute belongs to a trafo param feature
    if(!getIsTrafoParamDisplayAttribute(attr)){
        return false;
    }

    //check visibility of the attribute
    if(this->trafoParamTableColumnConfig.getColumnVisibility((TrafoParamDisplayAttributes)attr)){
        return true;
    }
    return false;

}

/*!
 * \brief BundleParameterTableProxyModel::lessThan
 * \param left
 * \param right
 * \return
 */
bool BundleParameterTableProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const{
    return true;
}

/*!
 * \brief BundleParameterTableProxyModel::isBundleStationTrafo
 * \param param
 * \return
 */
bool BundleParameterTableProxyModel::isBundleStationTrafo(const QPointer<TrafoParam> &param) const
{
    if((param->getStartSystem()->getIsStationSystem() && param->getDestinationSystem()->getIsBundleSystem()) ||
            (param->getStartSystem()->getIsBundleSystem() && param->getDestinationSystem()->getIsStationSystem())){
        return true;
    }
    return false;
}
