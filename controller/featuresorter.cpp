#include "featuresorter.h"

FeatureSorter::FeatureSorter() : mode(eSortStandard)
{
}

/*!
 * \brief FeatureSorter::operator ()
 * \param left
 * \param right
 * \return
 */
bool FeatureSorter::operator()(const int &left, const int &right){
    return this->lessThan(left, right);
}

/*!
 * \brief FeatureSorter::operator ()
 * \param left
 * \param right
 * \return
 */
bool FeatureSorter::operator()(const QPointer<FeatureWrapper> &left, const QPointer<FeatureWrapper> &right){
    return this->lessThan(left, right);
}

/*!
 * \brief FeatureSorter::getSortingMode
 * \return
 */
FeatureSorter::SortingMode FeatureSorter::getSortingMode() const{
    return this->mode;
}

/*!
 * \brief FeatureSorter::setSortingMode
 * \param mode
 */
void FeatureSorter::setSortingMode(FeatureSorter::SortingMode mode){
    this->mode = mode;
}

/*!
 * \brief FeatureSorter::getSortingConfig
 * \return
 */
const FeatureTableSortingConfig FeatureSorter::getSortingConfig() const{
    return this->config;
}

/*!
 * \brief FeatureSorter::setSortingConfig
 * \param config
 */
void FeatureSorter::setSortingConfig(const FeatureTableSortingConfig &config){
    this->config = config;
}

/*!
 * \brief FeatureSorter::getCurrentJob
 * \return
 */
const QPointer<OiJob> &FeatureSorter::getCurrentJob() const{
    return this->job;
}

/*!
 * \brief FeatureSorter::setCurrentJob
 * \param job
 */
void FeatureSorter::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){
        this->job = job;
    }
}

/*!
 * \brief FeatureSorter::lessThan
 * \param left
 * \param right
 * \return
 */
bool FeatureSorter::lessThan(const int &left, const int &right) const{

    //check job
    if(this->job.isNull()){
        return false;
    }

    //get features by id
    QPointer<FeatureWrapper> leftFeature = this->job->getFeatureById(left);
    QPointer<FeatureWrapper> rightFeature = this->job->getFeatureById(right);

    return this->lessThan(leftFeature, rightFeature);

}

/*!
 * \brief FeatureSorter::lessThan
 * \param left
 * \param right
 * \return
 */
bool FeatureSorter::lessThan(const QPointer<FeatureWrapper> &left, const QPointer<FeatureWrapper> &right) const{

    //check features
    if(left.isNull() || left->getFeature().isNull() || right.isNull() || right->getFeature().isNull()){
        return false;
    }

    //call the sorting method specified by mode
    switch(this->mode){
    case eSortStandard:
        return this->sortStandard(left, right);
    }

    return false;

}

/*!
 * \brief FeatureSorter::sortStandard
 * \param left
 * \param right
 * \return
 */
bool FeatureSorter::sortStandard(QPointer<FeatureWrapper> left, QPointer<FeatureWrapper> right) const{

    //get relevant properties of left and right feature
    QString leftName = left->getFeature()->getFeatureName();
    QString rightName = right->getFeature()->getFeatureName();
    int leftId = left->getFeature()->getId();
    int rightId = right->getFeature()->getId();
    bool leftNominal = (!left->getGeometry().isNull()
                        && left->getGeometry()->getIsNominal()) ? true : false;
    bool rightNominal = (!right->getGeometry().isNull()
                        && right->getGeometry()->getIsNominal()) ? true : false;

    //compare the properties of left and right feature
    if(leftName.compare(rightName) == 0){ //if feature names are equal

        //if one is actual and the other one is nominal
        if(leftNominal != rightNominal){
            return rightNominal;
        }

        return leftId < rightId;

    }else{ //if feature names are not equal

        //use the actual of a nominal for sorting
        if(!left->getGeometry().isNull() && left->getGeometry()->getIsNominal()
                && !left->getGeometry()->getMyMasterGeometry()->getActual().isNull()){
            leftId = left->getGeometry()->getMyMasterGeometry()->getActual()->getId();
        }
        if(!right->getGeometry().isNull() && right->getGeometry()->getIsNominal()
                && !right->getGeometry()->getMyMasterGeometry()->getActual().isNull()){
            rightId = right->getGeometry()->getMyMasterGeometry()->getActual()->getId();
        }

        return leftId < rightId;

    }

}
