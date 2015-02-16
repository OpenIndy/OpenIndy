#include "featuretableproxymodel.h"

/*!
 * \brief FeatureOvserviewProxyModel constructor
 * \param QList<FeatureWrapper*> &features
 * \param parent
 */
FeatureTableProxyModel::FeatureTableProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

/*!
 * \brief filterAcceptsRow function that filters all relevant feature types. Transformation parameters will not be displayed
 * \param source_row
 * \param source_parent
 * \return
 */
bool FeatureTableProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{

    if(OiJob::getFeatures().size() <= source_row){
        return false;
    }

    if(OiJob::getActiveGroup().compare("All Groups") == 0){
        if(OiJob::getFeatures().at(source_row)->getTrafoParam() != NULL){
            return false;
        }else{
            return true;
        }
    }else{
        if(OiJob::getFeatures().at(source_row)->getTrafoParam() != NULL || OiJob::getFeatures().at(source_row)->getFeature()->getGroupName().compare(OiJob::getActiveGroup()) != 0){
            return false;
        }else{
            return true;
        }
    }
}

/*!
 * \brief FeatureOverviewProxyModel::lessThan
 * \param left
 * \param right
 * \return
 */
bool FeatureTableProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const{

    //check indexes
    if(!left.isValid() || !right.isValid()){
        return false;
    }

    //get row index for each model index
    int leftIndex = left.row();
    int rightIndex = right.row();

    //get relevant properties of left and right feature
    QString leftName = "", rightName = "";
    int leftId = 0, rightId = 0;
    bool leftNominal = false, rightNominal = false;
    FeatureWrapper *leftFeature = OiJob::getFeatures().at(leftIndex);
    FeatureWrapper *rightFeature = OiJob::getFeatures().at(rightIndex);
    if(leftFeature != NULL && leftFeature->getFeature() != NULL
            && rightFeature != NULL && rightFeature->getFeature() != NULL){
        leftName = leftFeature->getFeature()->getFeatureName();
        rightName = rightFeature->getFeature()->getFeatureName();
        leftId = leftFeature->getFeature()->getId();
        rightId = rightFeature->getFeature()->getId();
        if(leftFeature->getGeometry() != NULL){
            leftNominal = leftFeature->getGeometry()->getIsNominal();
        }
        if(rightFeature->getGeometry() != NULL){
            rightNominal = rightFeature->getGeometry()->getIsNominal();
        }
    }

    //compare the properties of left and right feature
    if(leftName.compare(rightName) == 0){ //if feature names are equal

        if(leftNominal != rightNominal){ //if one is actual and the other one is nominal
            return leftNominal;
        }

        return leftId > rightId;

    }else{ //if feature names are not equal

        //get smallest id of features with equal name
        int leftSmallestId = -1;
        int rightSmallestId = -1;
        QList<FeatureWrapper *> leftFeatures = OiJob::getFeaturesByName(leftName);
        QList<FeatureWrapper *> rightFeatures = OiJob::getFeaturesByName(rightName);
        for(int i = 0; i < leftFeatures.size(); i++){
            if(leftSmallestId == -1 || leftFeatures.at(i)->getFeature()->getId() < leftSmallestId){
                leftSmallestId = leftFeatures.at(i)->getFeature()->getId();
            }
        }
        for(int i = 0; i < rightFeatures.size(); i++){
            if(rightSmallestId == -1 || rightFeatures.at(i)->getFeature()->getId() < rightSmallestId){
                rightSmallestId = rightFeatures.at(i)->getFeature()->getId();
            }
        }

        return leftSmallestId > rightSmallestId;

    }

}

void FeatureTableProxyModel::sortNominalToActual()
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
bool FeatureTableProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{
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
QList<FeatureWrapper*> FeatureTableProxyModel::getFeaturesAtIndices(QModelIndexList &indices){
    QList<FeatureWrapper*> result;

    foreach(QModelIndex idx, indices){
        int position = -1;
        QModelIndex sourceModelIndex = this->mapToSource(idx); //get model index from source model
        if(sourceModelIndex.row() >= 0){
            position = sourceModelIndex.row();
        }
        if(OiJob::getFeatures().size() > position && position >= 0){
            FeatureWrapper *myFeature = OiJob::getFeatures().at(position);
            result.append(myFeature);
        }
    }

    return result;
}

/*!
 * \brief FeatureOvserviewProxyModel::activeGroupChanged
 */
void FeatureTableProxyModel::activeGroupChanged(){
    this->invalidateFilter();
}
