#include "trafoparamtableproxymodel.h"

/*!
 * \brief TrafoParamTableProxyModel::TrafoParamTableProxyModel
 * \param parent
 */
TrafoParamTableProxyModel::TrafoParamTableProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

/*!
 * \brief TrafoParamTableProxyModel::getTrafoParamTableColumnConfig
 * \return
 */
const TrafoParamTableColumnConfig &TrafoParamTableProxyModel::getTrafoParamTableColumnConfig() const{
    return this->trafoParamTableColumnConfig;
}

/*!
 * \brief TrafoParamTableProxyModel::setTrafoParamTableColumnConfig
 * \param config
 */
void TrafoParamTableProxyModel::setTrafoParamTableColumnConfig(const TrafoParamTableColumnConfig &config){
    this->trafoParamTableColumnConfig = config;
    this->invalidateFilter();
}

/*!
 * \brief TrafoParamTableProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * \return
 */
bool TrafoParamTableProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{

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

    //filter by group name
    if(source_model->getCurrentJob()->getActiveGroup().compare("All Groups") == 0){
        return true;
    }else{
        if(feature->getFeature()->getGroupName().compare(source_model->getCurrentJob()->getActiveGroup()) == 0){
            return true;
        }
        return false;
    }

    return false;

}

/*!
 * \brief TrafoParamTableProxyModel::filterAcceptsColumn
 * \param source_column
 * \param source_parent
 * \return
 */
bool TrafoParamTableProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{

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
 * \brief TrafoParamTableProxyModel::lessThan
 * \param left
 * \param right
 * \return
 */
bool TrafoParamTableProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const{
    return true;
}
