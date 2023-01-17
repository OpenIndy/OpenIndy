#include "featuredifferenceproxymodel.h"

/*!
 * \brief FeatureDifferenceProxyModel::FeatureDifferenceProxyModel
 * \param parent
 */
FeatureDifferenceProxyModel::FeatureDifferenceProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    this->sort(0);
}

/*!
 * \brief FeatureTableProxyModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &FeatureDifferenceProxyModel::getCurrentJob() const{
    return this->job;
}

/*!
 * \brief FeatureTableProxyModel::setCurrentJob
 * \param job
 */
void FeatureDifferenceProxyModel::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){
        this->job = job;
        this->sorter.setCurrentJob(job);
    }
}

/*!
 * \brief FeatureTableProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * \return
 */
bool FeatureDifferenceProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{

   //get and cast source model
    FeatureDifferenceTableModel *source_model = dynamic_cast<FeatureDifferenceTableModel *>(this->sourceModel());
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

    //if the feature is a trafo param reject it
    if(!feature->getTrafoParam().isNull()){
        return false;
    }

    if(feature->getGeometry().isNull()){
        return false;
    }

    if(!feature->getGeometry().isNull() && feature->getGeometry()->getIsNominal()){
        return false;
    }

    if(!feature->getGeometry().isNull() && feature->getGeometry()->getNominals().isEmpty()){
        return false;
    }

    //filter by group name
    if(source_model->getCurrentJob()->getActiveGroup().compare("All Groups") != 0){
        if(feature->getFeature()->getGroupName().compare(source_model->getCurrentJob()->getActiveGroup()) != 0){
            return false;
        }
    }
    return true;
}

/*!
 * \brief FeatureTableProxyModel::filterAcceptsColumn
 * \param source_column
 * \param source_parent
 * \return
 */
bool FeatureDifferenceProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{

    return true;
}

void FeatureDifferenceProxyModel::setSortingMode(FeatureSorter::SortingMode mode)
{
    this->sorter.setSortingMode(mode);
    this->invalidate();
}

bool FeatureDifferenceProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const{

    //get and cast source model
    FeatureDifferenceTableModel *source_model = dynamic_cast<FeatureDifferenceTableModel *>(this->sourceModel());
    if(source_model == NULL || source_model->getCurrentJob().isNull()){
        return false;
    }

    //check indexes
    if(!left.isValid() || !right.isValid()){
        return false;
    }

    //get and check row index for each model index
    int leftIndex = left.row();
    int rightIndex = right.row();
    if(source_model->getCurrentJob()->getFeaturesList().size() <= leftIndex || source_model->getCurrentJob()->getFeaturesList().size() <= rightIndex){
        return false;
    }

    //get features
    QPointer<FeatureWrapper> leftFeature = source_model->getCurrentJob()->getFeaturesList().at(leftIndex);
    QPointer<FeatureWrapper> rightFeature = source_model->getCurrentJob()->getFeaturesList().at(rightIndex);

    return this->sorter.lessThan(leftFeature, rightFeature);

}
