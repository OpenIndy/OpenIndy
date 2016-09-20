#include "featuretabletreeproxymodel.h"

/*!
 * \brief FeatureTableTreeProxyModel::FeatureTableTreeProxyModel
 * \param parent
 */
FeatureTableTreeProxyModel::FeatureTableTreeProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

/*!
 * \brief FeatureTableTreeProxyModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &FeatureTableTreeProxyModel::getCurrentJob() const
{
    return this->job;
}

/*!
 * \brief FeatureTableTreeProxyModel::setCurrentJob
 * \param job
 */
void FeatureTableTreeProxyModel::setCurrentJob(const QPointer<OiJob> &job)
{
    if(!job.isNull()){
        this->job = job;
        this->sorter.setCurrentJob(job);
    }
}

/*!
 * \brief FeatureTableTreeProxyModel::getFeatureTableColumnConfig
 * \return
 */
const FeatureTableColumnConfig &FeatureTableTreeProxyModel::getFeatureTableColumnConfig() const
{
    return this->featureTableColumnConfig;
}

/*!
 * \brief FeatureTableTreeProxyModel::setFeatureTableColumnConfig
 * \param config
 */
void FeatureTableTreeProxyModel::setFeatureTableColumnConfig(const FeatureTableColumnConfig &config)
{
    this->featureTableColumnConfig = config;
    this->invalidateFilter();
}

/*!
 * \brief FeatureTableTreeProxyModel::mapFromSource
 * \param sourceIndex
 * \return
 */
/*QModelIndex FeatureTableTreeProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    //get and check proxyIndex
    QModelIndex proxyIndex = QSortFilterProxyModel::mapFromSource(sourceIndex);
    if(!proxyIndex.isValid()){
        return proxyIndex;
    }

    //set up new column position
    int columnIndex = sourceIndex.column();
    int columnPosition = this->featureTableColumnConfig.getColumnPosition((FeatureDisplayAttributes)getFeatureDisplayAttributes().at(columnIndex));
    return this->index(proxyIndex.row(), columnPosition);
}*/

/*!
 * \brief FeatureTableTreeProxyModel::mapToSource
 * \param proxyIndex
 * \return
 */
/*QModelIndex FeatureTableTreeProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    //get and check SourceIndex
    QModelIndex sourceIndex = QSortFilterProxyModel::mapToSource(proxyIndex);
    if(!sourceIndex.isValid()){
        return sourceIndex;
    }

    //get and cast source model
    FeatureTableTreeModel *source_model = dynamic_cast<FeatureTableTreeModel *>(this->sourceModel());
    if(source_model == NULL){
        return QModelIndex();
    }

    //get display attribute at column position
    FeatureDisplayAttributes attr = this->featureTableColumnConfig.getDisplayAttributeAt(proxyIndex.column());

    //get the index of the attribute in the list of all availabte attributes
    int sourceColumn = getFeatureDisplayAttributes().indexOf(attr);

    //set up correct column position for the source index
    sourceIndex = this->createIndex(sourceIndex.row(), sourceColumn);
    return sourceIndex;
    //return source_model->index(sourceIndex.row(),sourceColumn);
}*/

/*!
 * \brief FeatureTableTreeProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * \return
 */
bool FeatureTableTreeProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    //get and cast source model
    FeatureTableTreeModel *source_model = dynamic_cast<FeatureTableTreeModel *>(this->sourceModel());
    if(source_model == NULL){
        return false;
    }

    //check if job is set
    if(source_model->getCurrentJob().isNull()){
        return false;
    }

    //get and check the feature
    FeatureItem *item = static_cast<FeatureItem *>(source_parent.internalPointer());

    if(item == NULL){
        item = source_model->getRootItem();
    }
    if(item == NULL){
        return false;
    }
    FeatureItem *child = NULL;
    if(item->childCount() >source_row){
        child = item->child(source_row);
    }

    if(!child){
        return false;
    }
    //dont show trafoParam features
    if(child->getItemData()->getFeatureTypeEnum() == eTrafoParamFeature){
        return false;
    }

    //filter by group name
    /*if(source_model->getCurrentJob()->getActiveGroup().compare("All Groups") != 0){
        if(child->getItemData()->getFeature()->getGroupName().compare(source_model->getCurrentJob()->getActiveGroup()) != 0){
            return false;
        }
    }*/
    return true;
}

/*!
 * \brief FeatureTableTreeProxyModel::filterAcceptsColumn
 * \param source_column
 * \param source_parent
 * \return
 */
bool FeatureTableTreeProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    //get and check display attribute
    if(getFeatureDisplayAttributes().size() < source_column){
        return false;
    }
    int attr = getFeatureDisplayAttributes().at(source_column);

    //check if the attribute belongs to a non trafo param feature
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
 * \brief FeatureTableTreeProxyModel::lessThan
 * \param left
 * \param right
 * \return
 */
/*bool FeatureTableTreeProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    //get and cast source model
    FeatureTableTreeModel *source_model = dynamic_cast<FeatureTableTreeModel *>(this->sourceModel());
    if(source_model == NULL || source_model->getCurrentJob().isNull()){
        return false;
    }

    //Check indexes
    if(!left.isValid() || !right.isValid()){
        return false;
    }

    //get and check row index for each model index
    int leftIndex = left.row();
    int rightIndex = right.row();
}*/
