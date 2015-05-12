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
void AvailableElementsTreeViewProxyModel::setSelectedFunctionPosition(const int &functionPosition, const int &neededElementIndex){

    //reset old function position
    this->resetSelectedFunctionPosition();

    //set function index and input element index
    this->functionPosition = functionPosition;
    this->neededElementIndex = neededElementIndex;

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //get and check active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return;
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check selected function position
    if(this->functionPosition < 0 || this->functionPosition >= feature->getFunctions().size()
            || feature->getFunctions().at(this->functionPosition).isNull()){
        return;
    }
    this->connectedFunction = feature->getFunctions().at(this->functionPosition);

    //connect the specified function
    QObject::connect(this->connectedFunction.data(), &Function::inputElementsChanged, this, &AvailableElementsTreeViewProxyModel::invalidateFilter, Qt::AutoConnection);

    this->invalidateFilter();

}

/*!
 * \brief AvailableElementsTreeViewProxyModel::validateSelection
 * \param selection
 * \return
 */
bool AvailableElementsTreeViewProxyModel::validateSelection(const QModelIndexList &selection) const{

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

    //check each model index
    foreach(const QModelIndex &index, selection){

        //get source index
        QModelIndex sourceIndex = this->mapToSource(index);
        if(!sourceIndex.isValid()){
            return false;
        }

        //get and check corresponding feature tree item
        QPointer<FeatureTreeItem> item(NULL);
        if(!sourceIndex.isValid()){
            return false;
        }
        item = static_cast<FeatureTreeItem*>(sourceIndex.internalPointer());
        if(item.isNull()){
            return false;
        }

        //check if item is or contains the needed element type
        if(!item->getHasElement(neededElement)){
            return false;
        }

    }

    return true;

}

/*!
 * \brief AvailableElementsTreeViewProxyModel::addInputElements
 * \param selection
 */
void AvailableElementsTreeViewProxyModel::addInputElements(const QModelIndexList &selection){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //get and check active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return;
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check selected function position
    if(this->functionPosition < 0 || this->functionPosition >= feature->getFunctions().size()
            || feature->getFunctions().at(this->functionPosition).isNull() || this->neededElementIndex < 0
            || this->neededElementIndex >= feature->getFunctions().at(this->functionPosition)->getNeededElements().size()){
        return;
    }
    Function *function = feature->getFunctions().at(this->functionPosition);

    //get needed element
    ElementTypes neededElement = feature->getFunctions().at(this->functionPosition)->getNeededElements().at(this->neededElementIndex).typeOfElement;

    //check each model index
    foreach(const QModelIndex &index, selection){

        //get source index
        QModelIndex sourceIndex = this->mapToSource(index);
        if(!sourceIndex.isValid()){
            continue;
        }

        //get and check corresponding feature tree item
        QPointer<FeatureTreeItem> item(NULL);
        if(!sourceIndex.isValid()){
            continue;
        }
        item = static_cast<FeatureTreeItem*>(sourceIndex.internalPointer());
        if(item.isNull()){
            continue;
        }

        //check if item is or contains the needed element type
        if(!item->getHasElement(neededElement)){
            continue;
        }

        this->addInputElement(item, function->getNeededElements().at(this->neededElementIndex).typeOfElement);

    }

}

/*!
 * \brief AvailableElementsTreeViewProxyModel::getItemAtIndex
 * \param index
 * \return
 */
/*QPointer<FeatureTreeItem> AvailableElementsTreeViewProxyModel::getItemAtIndex(const QModelIndex &index){

    QPointer<FeatureTreeItem> item(NULL);

    //get source index
    QModelIndex sourceIndex = this->mapToSource(index);

    //get corresponding feature tree item
    if(sourceIndex.isValid()){
        item = static_cast<FeatureTreeItem*>(sourceIndex.internalPointer());
    }

    return item;



    FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
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
    return item;
}*/

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
    Function *function = feature->getFunctions().at(this->functionPosition);

    //get needed element
    ElementTypes neededElement = function->getNeededElements().at(this->neededElementIndex).typeOfElement;

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

    //check wether the item's type equals the needed element type but the item is already used or equals the feature to be calculated
    if(item->getElementType() == neededElement){

        //check if the element equals the feature to be calculated
        if(item->getIsFeature() && !item->getFeature().isNull() && !item->getFeature()->getFeature().isNull()){
            return !(feature->getId() == item->getFeature()->getFeature()->getId());
        }

        //check if function already contains the element
        QMap<int, QList<InputElement> > inputElements = function->getInputElements();
        if(item->getIsFeature() && !item->getFeature().isNull() && !item->getFeature()->getFeature().isNull()){
            return !inputElements.contains(item->getFeature()->getFeature()->getId());
        }else if(item->getIsObservation() && !item->getObservation().isNull()){
            return !inputElements.contains(item->getObservation()->getId());
        }else if(item->getIsReading() && !item->getReading().isNull()){
            return !inputElements.contains(item->getReading()->getId());
        }

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

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    QObject::connect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &AvailableElementsTreeViewProxyModel::resetSelectedFunctionPosition, Qt::AutoConnection);

    QObject::connect(this, &AvailableElementsTreeViewProxyModel::addInputFeature, this->currentJob.data(), &OiJob::addInputFeature, Qt::AutoConnection);
    QObject::connect(this, &AvailableElementsTreeViewProxyModel::addInputObservation, this->currentJob.data(), &OiJob::addInputObservation, Qt::AutoConnection);
    QObject::connect(this, &AvailableElementsTreeViewProxyModel::addInputReading, this->currentJob.data(), &OiJob::addInputReading, Qt::AutoConnection);

}

/*!
 * \brief AvailableElementsTreeViewProxyModel::disconnectJob
 */
void AvailableElementsTreeViewProxyModel::disconnectJob(){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    QObject::disconnect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &AvailableElementsTreeViewProxyModel::resetSelectedFunctionPosition);

    QObject::disconnect(this, &AvailableElementsTreeViewProxyModel::addInputFeature, this->currentJob.data(), &OiJob::addInputFeature);
    QObject::disconnect(this, &AvailableElementsTreeViewProxyModel::addInputObservation, this->currentJob.data(), &OiJob::addInputObservation);
    QObject::disconnect(this, &AvailableElementsTreeViewProxyModel::addInputReading, this->currentJob.data(), &OiJob::addInputReading);

}

/*!
 * \brief AvailableElementsTreeViewProxyModel::resetSelectedFunctionPosition
 */
void AvailableElementsTreeViewProxyModel::resetSelectedFunctionPosition(){

    //reset function index and input element index
    this->functionPosition = -1;
    this->neededElementIndex = -1;

    //disconnect function
    if(!this->connectedFunction.isNull()){
        QObject::disconnect(this->connectedFunction.data(), &Function::inputElementsChanged, this, &AvailableElementsTreeViewProxyModel::invalidateFilter);
        this->connectedFunction = QPointer<Function>(NULL);
    }

}

/*!
 * \brief AvailableElementsTreeViewProxyModel::addInputElement
 * Add input elements recursively
 * \param item
 * \param type
 */
void AvailableElementsTreeViewProxyModel::addInputElement(const QPointer<FeatureTreeItem> &item, const ElementTypes &type){

    //check item
    if(item.isNull()){
        return;
    }

    //check if the type is one of direction, position or radius and the item is a corresponding geometry
    bool elementCheck = false;
    if(type == eDirectionElement && !item->getFeature().isNull() && !item->getFeature()->getGeometry().isNull()
            && item->getFeature()->getGeometry()->hasDirection()){
        elementCheck = true;
    }else if(type == ePositionElement && !item->getFeature().isNull() && !item->getFeature()->getGeometry().isNull()
             && item->getFeature()->getGeometry()->hasPosition()){
         elementCheck = true;
     }else if(type == eRadiusElement && !item->getFeature().isNull() && !item->getFeature()->getGeometry().isNull()
              && item->getFeature()->getGeometry()->hasRadius()){
          elementCheck = true;
      }

    //if this item is of the right type
    if(item->getElementType() == type || elementCheck){

        //check if the element equals the feature to be calculated
        if(item->getIsFeature() && !item->getFeature().isNull() && !item->getFeature()->getFeature().isNull()){
            if(this->currentJob->getActiveFeature()->getFeature()->getId() == item->getFeature()->getFeature()->getId()){
                return;
            }
        }

        //check if function already contains the element
        QMap<int, QList<InputElement> > inputElements = this->currentJob->getActiveFeature()->getFeature()->getFunctions().at(this->functionPosition)->getInputElements();
        if(item->getIsFeature() && !item->getFeature().isNull() && !item->getFeature()->getFeature().isNull()){
            if(inputElements.contains(item->getFeature()->getFeature()->getId())){
                return;
            }
        }else if(item->getIsObservation() && !item->getObservation().isNull()){
            if(inputElements.contains(item->getObservation()->getId())){
                return;
            }
        }else if(item->getIsReading() && !item->getReading().isNull()){
            if(inputElements.contains(item->getReading()->getId())){
                return;
            }
        }

        if(!item->getFeature().isNull()){
            emit this->addInputFeature(this->currentJob->getActiveFeature(), this->functionPosition,
                                       this->neededElementIndex, item->getFeature());
        }else if(!item->getReading().isNull()){
            emit this->addInputReading(this->currentJob->getActiveFeature(), this->functionPosition,
                                       this->neededElementIndex, item->getReading());
        }else if(!item->getObservation().isNull()){
            emit this->addInputObservation(this->currentJob->getActiveFeature(), this->functionPosition,
                                       this->neededElementIndex, item->getObservation());
        }

        return;

    }

    //if this item contains one or more items of the right type
    if(item->getHasElement(type)){
        for(int i = 0; i < item->getChildCount(); i++){
            this->addInputElement(item->getChild(i), type);
        }
    }

}
