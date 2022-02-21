#include "featuretableproxymodel.h"

FeatureTableProxyModel::FeatureTableProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

/*!
 * \brief FeatureTableProxyModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &FeatureTableProxyModel::getCurrentJob() const{
    return this->job;
}

/*!
 * \brief FeatureTableProxyModel::setCurrentJob
 * \param job
 */
void FeatureTableProxyModel::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){
        this->job = job;
        this->sorter.setCurrentJob(job);
    }
}

/*!
 * \brief FeatureTableProxyModel::getFeatureTableColumnConfig
 * \return
 */
const FeatureTableColumnConfig &FeatureTableProxyModel::getFeatureTableColumnConfig() const{
    return this->featureTableColumnConfig;
}

/*!
 * \brief FeatureTableProxyModel::setFeatureTableColumnConfig
 * \param config
 */
void FeatureTableProxyModel::setFeatureTableColumnConfig(const FeatureTableColumnConfig &config){
    this->featureTableColumnConfig = config;
    this->invalidateFilter();
}

/*!
 * \brief FeatureTableProxyModel::mapFromSource
 * Reimplemented here to be able to sort columns
 * \param sourceIndex
 * \return
 */
QModelIndex FeatureTableProxyModel::mapFromSource(const QModelIndex &sourceIndex) const{

    //get and check proxyIndex
    QModelIndex proxyIndex = QSortFilterProxyModel::mapFromSource(sourceIndex);
    if(!proxyIndex.isValid()){
        return proxyIndex;
    }

    //set up new column position (column sorting)
    int columnIndex = sourceIndex.column();
    int columnPosition = this->featureTableColumnConfig.getColumnPosition((FeatureDisplayAttributes)getFeatureDisplayAttributes().at(columnIndex));
    //proxyIndex = this->createIndex(proxyIndex.row(), columnPosition);
    return this->index(proxyIndex.row(), columnPosition);

    return proxyIndex;

}

/*!
 * \brief FeatureTableProxyModel::mapToSource
 * Reimplemented here to be able to sort columns
 * \param proxyIndex
 * \return
 */
QModelIndex FeatureTableProxyModel::mapToSource(const QModelIndex &proxyIndex) const{

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
    FeatureDisplayAttributes attr = this->featureTableColumnConfig.getDisplayAttributeAt(proxyIndex.column());

    //get the index of the attribute in the list of all available attributes
    int sourceColumn = getFeatureDisplayAttributes().indexOf(attr);

    //set up correct column position for the source index
    sourceIndex = this->createIndex(sourceIndex.row(), sourceColumn);

    return source_model->index(sourceIndex.row(), sourceColumn);

    return sourceIndex;

}

void FeatureTableProxyModel::setSortingMode(FeatureSorter::SortingMode mode)
{
    this->sorter.setSortingMode(mode);
    this->invalidate();
}

/*!
 * \brief FeatureTableProxyModel::filterAcceptsRow
 * Filter features by group name and feature type
 * \param source_row
 * \param source_parent
 * \return
 */
bool FeatureTableProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{

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

    //if the feature is a trafo param reject it
    if(!feature->getTrafoParam().isNull()){
        return false;
    }

    //filter by group name
    if(source_model->getCurrentJob()->getActiveGroup().compare("All Groups") != 0){
        if(feature->getFeature()->getGroupName().compare(source_model->getCurrentJob()->getActiveGroup()) != 0){
            return false;
        }
    }

    //filter by actual nominal
    if((source_model->getActualNominalFilter() == eFilterActual && !feature->getGeometry().isNull() && feature->getGeometry()->getIsNominal())
            || (source_model->getActualNominalFilter() == eFilterNominal && (feature->getGeometry().isNull() || !feature->getGeometry()->getIsNominal()))){
        return false;
    }

    return true;

}

/*!
 * \brief FeatureTableProxyModel::filterAcceptsColumn
 * \param source_column
 * \param source_parent
 * \return
 */
bool FeatureTableProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{

    //get and check display attribute
    if(getFeatureDisplayAttributes().size() < source_column){
        return false;
    }
    int attr = getFeatureDisplayAttributes().at(source_column);

    //check if the attribute belongs to a non-trafo param feature
    if(!getIsFeatureDisplayAttribute(attr)){
        return false;
    }

    //check visibility of the attribute
    if(this->featureTableColumnConfig.getColumnVisibility((FeatureDisplayAttributes)attr)){
        return true;
    }
    return false;

}

/*!
 * \brief FeatureTableProxyModel::lessThan
 * Sort features by id and put nominals to actuals
 * \param left
 * \param right
 * \return
 */
bool FeatureTableProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const{

    //get and cast source model
    FeatureTableModel *source_model = dynamic_cast<FeatureTableModel *>(this->sourceModel());
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
