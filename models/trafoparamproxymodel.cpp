#include "trafoparamproxymodel.h"

TrafoParamProxyModel::TrafoParamProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

/*!
 * \brief TrafoParamProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * \return
 */
bool TrafoParamProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{

    //get and cast source model
    FeatureTableModel *source_model = dynamic_cast<FeatureTableModel *>(this->sourceModel());
    if(source_model == NULL){
        return false;
    }

    //check if current job is set
    if(source_model->getCurrentJob().isNull()){
        return false;
    }

    //filter by group name and feature type
    if(source_model->getCurrentJob()->getFeatures().at(source_row)->getTrafoParam() != NULL){

        if(source_model->getCurrentJob()->getActiveGroup().compare("") == 0){
            return true;
        }

        if(source_model->getCurrentJob()->getFeatures().at(source_row)->getFeature()->getGroupName().compare(source_model->getCurrentJob()->getActiveGroup()) == 0){
            return true;
        }
        return false;

    }else{
        return false;
    }
}

/*!
 * \brief TrafoParamProxyModel::filterAcceptsColumn
 * \param source_column
 * \param source_parent
 * \return
 */
bool TrafoParamProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{
    QList<int> displayColumns = GUIConfiguration::displayAttributes(GUIConfiguration::trafoParamAttributes,GUIConfiguration::allAttributes);
    if(displayColumns.contains(source_column)){
        return true;
    }else{
        return false;
    }
}
