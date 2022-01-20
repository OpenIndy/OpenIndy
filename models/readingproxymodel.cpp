#include "readingproxymodel.h"

ReadingProxyModel::ReadingProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

/*!
 * \brief ReadingProxyModel::getReadingTableColumnConfig
 * \return
 */
const ReadingTableColumnConfig &ReadingProxyModel::getReadingTableColumnConfig() const{
    return this->readingTableColumnConfig;
}

/*!
 * \brief ReadingProxyModel::setReadingTableColumnConfig
 * \param config
 */
void ReadingProxyModel::setReadingTableColumnConfig(const ReadingTableColumnConfig &config){
    this->readingTableColumnConfig = config;
    this->invalidateFilter();
}

/*!
 * \brief ReadingProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * \return
 */
bool ReadingProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{
    return true;
}

/*!
 * \brief ReadingProxyModel::filterAcceptsColumn
 * \param source_column
 * \param source_parent
 * \return
 */
bool ReadingProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{

    //check visibility of the column
    if(this->readingTableColumnConfig.getColumnVisibility((ReadingDisplayAttributes)source_column)){

        //get and check source model
        ReadingModel *sourceModel = static_cast<ReadingModel *>(this->sourceModel());
        if(sourceModel == NULL){
            return false;
        }

        //get and check active feature
        if(sourceModel->getCurrentJob().isNull()){
            return false;
        }
        QPointer<FeatureWrapper> feature = sourceModel->getCurrentJob()->getActiveFeature();
        if(feature.isNull() || feature->getGeometry().isNull()){
            return false;
        }

        //check each used reading type
        foreach(const ReadingTypes &type, feature->getGeometry()->getUsedReadingTypes()){
            if(getReadingDisplayAttributeVisibility((ReadingDisplayAttributes)source_column, type)){
                return true;
            }
        }

        return false;

    }
    return false;

}

/*!
 * \brief ReadingProxyModel::lessThan
 * \param source_left
 * \param source_right
 * \return
 */
bool ReadingProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    ReadingModel *source_model = dynamic_cast<ReadingModel *>(this->sourceModel());
    if(source_model == NULL){
        return false;
    }

    //check if job is set
    if(source_model->getCurrentJob().isNull()){
        return false;
    }

    //check feature, function and observation
    if(!source_model->getCurrentJob()->getActiveFeature().isNull() &&
            source_model->getCurrentJob()->getActiveFeature()->getFeature()->getFunctions().size() >= 1){

        //fit function
        QPointer<Function> function = source_model->getCurrentJob()->getActiveFeature()->getFeature()->getFunctions().at(0);
        QList<InputElement> inputElem = function->getInputElements().value(0);


        if( source_right.isValid()
            && source_left.isValid()
            && source_right.row() < inputElem.size()
            && source_left.row() < inputElem.size()
            && !inputElem.at(source_right.row()).observation.isNull()
            && !inputElem.at(source_left.row()).observation.isNull()){

            int fwRight = inputElem.at(source_right.row()).observation->getId();
            int fwLeft =  inputElem.at(source_left.row()).observation->getId();

            return fwLeft < fwRight;
        }

    }
    return false;
}
