#include "trafoparamproxymodel.h"

TrafoParamProxyModel::TrafoParamProxyModel(QList<FeatureWrapper*> &features,QObject *parent) :
    QSortFilterProxyModel(parent),features(features)
{
}

bool TrafoParamProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{
    //return true;
    if(this->features.at(source_row)->getTrafoParam() != NULL){
        return true;
    }else{
        return false;
    }
}

bool TrafoParamProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{
    if(source_column == 0 || source_column == 1 || source_column == 7 || source_column == 9
            || source_column == 19 || source_column == 20 || source_column == 21 || source_column == 22 || source_column == 23
            || source_column == 24 || source_column == 25 || source_column == 26 || source_column == 27 || source_column == 28
            || source_column == 29 || source_column == 32){
        return true;
    }else{
        return false;
    }

}

/*!
 * \brief TrafoParamProxyModel::getFeaturesAtIndices
 * Get all selected features
 * \param indices
 * \return
 */
QList<FeatureWrapper*> TrafoParamProxyModel::getFeaturesAtIndices(QModelIndexList &indices){
    QList<FeatureWrapper*> result;

    foreach(QModelIndex idx, indices){
        int position = -1;
        QModelIndex sourceModelIndex = this->mapToSource(idx); //get model index from source model
        if(sourceModelIndex.row() >= 0){
            position = sourceModelIndex.row();
        }
        if(this->features.size() > position && position >= 0){
            FeatureWrapper *myFeature = this->features.at(position);
            result.append(myFeature);
        }
    }

    return result;
}
