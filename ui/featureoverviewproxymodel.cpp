#include "featureoverviewproxymodel.h"

/*!
 * \brief FeatureOvserviewProxyModel constructor
 * \param QList<FeatureWrapper*> &features
 * \param parent
 */
FeatureOverviewProxyModel::FeatureOverviewProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

/*!
 * \brief filterAcceptsRow function that filters all relevant feature types. Transformation parameters will not be displayed
 * \param source_row
 * \param source_parent
 * \return
 */
bool FeatureOverviewProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{
    if(OiFeatureState::getActiveGroup().compare("All Groups") == 0){
        if(OiFeatureState::getFeatures().at(source_row)->getTrafoParam() != NULL){
            return false;
        }else{
            return true;
        }
    }else{
        if(OiFeatureState::getFeatures().at(source_row)->getTrafoParam() != NULL || OiFeatureState::getFeatures().at(source_row)->getFeature()->getGroupName().compare(OiFeatureState::getActiveGroup()) != 0){
            return false;
        }else{
            return true;
        }
    }
}

void FeatureOverviewProxyModel::sortNominalToActual()
{/*
    int row = 2;
    for(int i=0; i<row-1;i++){
        if(row < 0){
            return;
        }
        if(this->features.at(row)->getFeature()->name.compare(this->features.at(i)->getFeature()->name)==0){
            if(this->features.at(row)->getGeometry() != NULL && this->features.at(row)->getGeometry()->isNominal){
                beginMoveRows(this->parent,i,i,this->parent,row+1);
            }else if(this->features.at(i)->getGeometry() != NULL && this->features.at(i)->getGeometry()->isNominal){
                beginMoveRows(this->parent,row,row,this->parent,i+1);
            }
        }
    }*/
}

/*!
 * \brief filterAcceptsColumn function that filters all relevant attributes for the displayed feature types.
 * \param source_column
 * \param source_parent
 * \return
 */
bool FeatureOverviewProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{
    /*if(source_column == 19 || source_column == 20 || source_column == 21 || source_column == 22 || source_column == 23
            || source_column == 24 || source_column == 25 || source_column == 26 || source_column == 27
            || source_column == 28 || source_column == 29){
        return false;
    }else{
        return true;
    }*/
    QList<int> displayColumns = GUIConfiguration::displayAttributes(GUIConfiguration::featureAttributes,GUIConfiguration::allAttributes);
    if(displayColumns.contains(source_column)){
        return true;
    }else{
        return false;
    }
}

/*!
 * \brief FeatureOvserviewProxyModel::getFeatureAtIndex
 * Get all selected features
 * \param indices
 * \return
 */
QList<FeatureWrapper*> FeatureOverviewProxyModel::getFeaturesAtIndices(QModelIndexList &indices){
    QList<FeatureWrapper*> result;

    foreach(QModelIndex idx, indices){
        int position = -1;
        QModelIndex sourceModelIndex = this->mapToSource(idx); //get model index from source model
        if(sourceModelIndex.row() >= 0){
            position = sourceModelIndex.row();
        }
        if(OiFeatureState::getFeatures().size() > position && position >= 0){
            FeatureWrapper *myFeature = OiFeatureState::getFeatures().at(position);
            result.append(myFeature);
        }
    }

    return result;
}

/*!
 * \brief FeatureOvserviewProxyModel::activeGroupChanged
 */
void FeatureOverviewProxyModel::activeGroupChanged(){
    this->invalidateFilter();
}
