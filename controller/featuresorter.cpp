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
    case eSortBy_Name_ActNom:
        return this->sortName(left, right);
    case eSortBy_Group_Name_ActNom:
        return this->sortGroup(left, right);
    case eSortBy_Id_ActNom:
        return this->sortId(left, right);
    }

    return false;

}

/*!
 * \brief FeatureSorter::sortStandard
 * adding an actual feature to a nominal, or a nominal feature to an existing actual will not impact the order of all features in the job.
 * the added feature will be added next to the existing one
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

        //check the ID of the actual and nominal if this feature
        if(!left->getGeometry().isNull()){

            if(!left->getGeometry()->getIsNominal()){

                //check for the lower ID and use this for sorting
                //so the order of the job will not be swapped, and the new feature will be added to the existing feature
                if(left->getGeometry()->getNominals().size() > 0){
                    if(left->getGeometry()->getId() < left->getGeometry()->getNominals().at(0)->getId()){
                        leftId = left->getGeometry()->getId();
                    }else{
                        leftId = left->getGeometry()->getNominals().at(0)->getId();
                    }
                }else{
                    leftId = left->getGeometry()->getId();
                }
            }else{

                //check for the lower ID and use this for sorting
                //so the order of the job will not be swapped, and the new feature will be added to the existing feature
                if(!left->getGeometry()->getActual().isNull()){
                    if(left->getGeometry()->getId() < left->getGeometry()->getActual()->getId()){
                        leftId = left->getGeometry()->getId();
                    }else{
                        leftId = left->getGeometry()->getActual()->getId();
                    }
                }else{
                    leftId = left->getGeometry()->getId();
                }
            }
        }
        if(!right->getGeometry().isNull()){

            if(!right->getGeometry()->getIsNominal()){

                //check for the lower ID and use this for sorting
                //so the order of the job will not be swapped, and the new feature will be added to the existing feature
                if(right->getGeometry()->getNominals().size() > 0){
                    if(right->getGeometry()->getId() < right->getGeometry()->getNominals().at(0)->getId()){
                        rightId = right->getGeometry()->getId();
                    }else{
                        rightId = right->getGeometry()->getNominals().at(0)->getId();
                    }
                }else{
                    rightId = right->getGeometry()->getId();
                }
            }else{

                //check for the lower ID and use this for sorting
                //so the order of the job will not be swapped, and the new feature will be added to the existing feature
                if(!right->getGeometry()->getActual().isNull()){
                    if(right->getGeometry()->getId() < right->getGeometry()->getActual()->getId()){
                        rightId = right->getGeometry()->getId();
                    }else{
                        rightId = right->getGeometry()->getActual()->getId();
                    }
                }else{
                    rightId = right->getGeometry()->getId();
                }
            }
        }
        return leftId < rightId;
    }
}

bool FeatureSorter::sortName(QPointer<FeatureWrapper> left, QPointer<FeatureWrapper> right) const {
    if(left->getFeature()->getFeatureName().compare(right->getFeature()->getFeatureName(), Qt::CaseInsensitive) < 0) {
        return true;
    } else if(left->getFeature()->getFeatureName().compare(right->getFeature()->getFeatureName(), Qt::CaseInsensitive) == 0) {
        return left->getGeometry()->getIsNominal() < right->getGeometry()->getIsNominal();
    }

    return false;
}

bool FeatureSorter::sortGroup(QPointer<FeatureWrapper> left, QPointer<FeatureWrapper> right) const {
    if(left->getFeature()->getGroupName().compare(right->getFeature()->getGroupName(), Qt::CaseInsensitive) < 0) {
        return true;
    } else if(left->getFeature()->getGroupName().compare(right->getFeature()->getGroupName(), Qt::CaseInsensitive) == 0) {
        if(left->getFeature()->getFeatureName().compare(right->getFeature()->getFeatureName(), Qt::CaseInsensitive) < 0) {
            return true;
        } else if(left->getFeature()->getFeatureName().compare(right->getFeature()->getFeatureName(), Qt::CaseInsensitive) == 0) {
            return left->getGeometry()->getIsNominal() < right->getGeometry()->getIsNominal();
        }
    }

    return false;
}

bool FeatureSorter::sortId(QPointer<FeatureWrapper> left, QPointer<FeatureWrapper> right) const {
    return false;
}
