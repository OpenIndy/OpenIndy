#include "availableelementstreeviewproxymodel.h"

/*!
 * \brief AvailableElementsTreeViewProxyModel::AvailableElementsTreeViewProxyModel
 * \param parent
 */
AvailableElementsTreeViewProxyModel::AvailableElementsTreeViewProxyModel(QObject *parent) : QSortFilterProxyModel(parent){
    this->resetSelectedFunctionPosition();
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant AvailableElementsTreeViewProxyModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        return "available elements";

    }
    return QVariant();
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &AvailableElementsTreeViewProxyModel::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::setCurrentJob
 * \param job
 */
void AvailableElementsTreeViewProxyModel::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){

        //disconnect current job
        if(!this->currentJob.isNull()){
            this->disconnectJob();
        }

        this->currentJob = job;
        this->connectJob();

    }
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::setSelectedFunctionPosition
 * \param functionIndex
 * \param neededElementIndex
 */
void AvailableElementsTreeViewProxyModel::setSelectedFunctionPosition(const int &functionIndex, const int &neededElementIndex){
    this->functionPosition = functionPosition;
    this->neededElementIndex = neededElementIndex;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::getItemAtIndex
 * \param index
 * \return
 */
QPointer<FeatureTreeItem> AvailableElementsTreeViewProxyModel::getItemAtIndex(const QModelIndex &index){

    QPointer<FeatureTreeItem> item(NULL);

    //get source index
    QModelIndex sourceIndex = this->mapToSource(index);

    //get corresponding feature tree item
    if(sourceIndex.isValid()){
        item = static_cast<FeatureTreeItem*>(sourceIndex.internalPointer());
    }

    return item;

    /*FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
    if(model != NULL){
        QList<int> rowIndex;
        QModelIndex parent = this->mapToSource(index);
        rowIndex.append(parent.row());
        while(parent.parent().isValid()){
            parent = parent.parent();
            rowIndex.append(parent.row());
        }
        item = model->getRootItem();
        for(int i = rowIndex.size()-1; i >= 0; i--){
            item = item->getChild(rowIndex.at(i));
            if(item == NULL){
                break;
            }
        }
    }
    return item;*/
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{

    //check current job
    if(this->currentJob.isNull()){
        return false;
    }

    //get and check active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return false;
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check selected function position
    if(this->functionPosition < 0 || this->functionPosition >= feature->getFunctions().size()
            || feature->getFunctions().at(this->functionPosition).isNull() || this->neededElementIndex < 0
            || this->neededElementIndex >= feature->getFunctions().at(this->functionPosition)->getNeededElements().size()){
        return false;
    }

    //get needed element
    ElementTypes neededElement = feature->getFunctions().at(this->functionPosition)->getNeededElements().at(this->neededElementIndex).typeOfElement;

    //get feature tree item
    QPointer<FeatureTreeItem> item(NULL);
    if(!source_parent.isValid()){ //parent is root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL && !model->getRootItem().isNull() && source_row < model->getRootItem()->getChildCount()){
            item = model->getRootItem()->getChild(source_row);
        }
    }else{
        FeatureTreeItem *parent = static_cast<FeatureTreeItem*>(source_parent.internalPointer());
        if(parent != NULL && source_row < parent->getChildCount()){
            item = parent->getChild(source_row);
        }
    }
    if(item.isNull()){
        return false;
    }

    //check wether item contains an element of the type neededElement
    if(item->getHasElement(neededElement)){
        return true;
    }
    return false;

}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptsColumn
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptsColumn(int source_row, const QModelIndex &source_parent) const{
    return true;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::connectJob
 */
void AvailableElementsTreeViewProxyModel::connectJob(){

    QObject::connect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &AvailableElementsTreeViewProxyModel::resetSelectedFunctionPosition, Qt::AutoConnection);

}

/*!
 * \brief AvailableElementsTreeViewProxyModel::disconnectJob
 */
void AvailableElementsTreeViewProxyModel::disconnectJob(){

    QObject::disconnect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &AvailableElementsTreeViewProxyModel::resetSelectedFunctionPosition);

}

/*!
 * \brief AvailableElementsTreeViewProxyModel::resetSelectedFunctionPosition
 */
void AvailableElementsTreeViewProxyModel::resetSelectedFunctionPosition(){
    this->functionPosition = -1;
    this->neededElementIndex = -1;
}
