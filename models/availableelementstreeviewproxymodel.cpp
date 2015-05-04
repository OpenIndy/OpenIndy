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
            return;
        }

        //get and check corresponding feature tree item
        QPointer<FeatureTreeItem> item(NULL);
        if(!sourceIndex.isValid()){
            return;
        }
        item = static_cast<FeatureTreeItem*>(sourceIndex.internalPointer());
        if(item.isNull()){
            return;
        }

        //check if item is or contains the needed element type
        if(!item->getHasElement(neededElement)){
            return;
        }

        this->addInputElement(feature, function, neededElement, item);

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
 * \param target
 * \param function
 * \param type
 * \param item
 */
void AvailableElementsTreeViewProxyModel::addInputElement(Feature *target, Function *function, const ElementTypes &type, const QPointer<FeatureTreeItem> &item){

    //check item
    if(item.isNull()){
        return;
    }

    //if this item is of the right type
    if(item->getElementType() == type){

        switch(type){
        case eCircleElement:{

            //check circle
            if(item->getFeature().isNull() || item->getFeature()->getCircle().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getCircle()->getId());
            element.typeOfElement = eCircleElement;
            element.circle = item->getFeature()->getCircle();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getCircle()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eConeElement:{

            //check cone
            if(item->getFeature().isNull() || item->getFeature()->getCone().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getCone()->getId());
            element.typeOfElement = eConeElement;
            element.cone = item->getFeature()->getCone();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getCone()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eCylinderElement:{

            //check cylinder
            if(item->getFeature().isNull() || item->getFeature()->getCylinder().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getCylinder()->getId());
            element.typeOfElement = eCylinderElement;
            element.cylinder = item->getFeature()->getCylinder();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getCylinder()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eEllipseElement:{

            //check ellipse
            if(item->getFeature().isNull() || item->getFeature()->getEllipse().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getEllipse()->getId());
            element.typeOfElement = eEllipseElement;
            element.ellipse = item->getFeature()->getEllipse();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getEllipse()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eEllipsoidElement:{

            //check ellipsoid
            if(item->getFeature().isNull() || item->getFeature()->getEllipsoid().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getEllipsoid()->getId());
            element.typeOfElement = eEllipsoidElement;
            element.ellipsoid = item->getFeature()->getEllipsoid();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getEllipsoid()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eHyperboloidElement:{

            //check hyperboloid
            if(item->getFeature().isNull() || item->getFeature()->getHyperboloid().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getHyperboloid()->getId());
            element.typeOfElement = eHyperboloidElement;
            element.hyperboloid = item->getFeature()->getHyperboloid();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getHyperboloid()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eLineElement:{

            //check line
            if(item->getFeature().isNull() || item->getFeature()->getLine().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getLine()->getId());
            element.typeOfElement = eLineElement;
            element.line = item->getFeature()->getLine();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getLine()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eNurbsElement:{

            //check nurbs
            if(item->getFeature().isNull() || item->getFeature()->getNurbs().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getNurbs()->getId());
            element.typeOfElement = eNurbsElement;
            element.nurbs = item->getFeature()->getNurbs();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getNurbs()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eParaboloidElement:{

            //check paraboloid
            if(item->getFeature().isNull() || item->getFeature()->getParaboloid().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getParaboloid()->getId());
            element.typeOfElement = eParaboloidElement;
            element.paraboloid = item->getFeature()->getParaboloid();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getParaboloid()->addUsedFor(target->getFeatureWrapper());

            return;

        }case ePlaneElement:{

            //check plane
            if(item->getFeature().isNull() || item->getFeature()->getPlane().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getPlane()->getId());
            element.typeOfElement = ePlaneElement;
            element.plane = item->getFeature()->getPlane();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getPlane()->addUsedFor(target->getFeatureWrapper());

            return;

        }case ePointElement:{

            //check point
            if(item->getFeature().isNull() || item->getFeature()->getPoint().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getPoint()->getId());
            element.typeOfElement = ePointElement;
            element.point = item->getFeature()->getPoint();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getPoint()->addUsedFor(target->getFeatureWrapper());

            return;

        }case ePointCloudElement:{

            //check point cloud
            if(item->getFeature().isNull() || item->getFeature()->getPointCloud().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getPointCloud()->getId());
            element.typeOfElement = ePointCloudElement;
            element.pointCloud = item->getFeature()->getPointCloud();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getPointCloud()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eScalarEntityAngleElement:{

            //check angle
            if(item->getFeature().isNull() || item->getFeature()->getScalarEntityAngle().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getScalarEntityAngle()->getId());
            element.typeOfElement = eScalarEntityAngleElement;
            element.scalarEntityAngle = item->getFeature()->getScalarEntityAngle();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getScalarEntityAngle()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eScalarEntityDistanceElement:{

            //check distance
            if(item->getFeature().isNull() || item->getFeature()->getScalarEntityDistance().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getScalarEntityDistance()->getId());
            element.typeOfElement = eScalarEntityDistanceElement;
            element.scalarEntityDistance = item->getFeature()->getScalarEntityDistance();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getScalarEntityDistance()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eScalarEntityMeasurementSeriesElement:{

            //check series
            if(item->getFeature().isNull() || item->getFeature()->getScalarEntityMeasurementSeries().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getScalarEntityMeasurementSeries()->getId());
            element.typeOfElement = eScalarEntityMeasurementSeriesElement;
            element.scalarEntityMeasurementSeries = item->getFeature()->getScalarEntityMeasurementSeries();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getScalarEntityMeasurementSeries()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eScalarEntityTemperatureElement:{

            //check temperature
            if(item->getFeature().isNull() || item->getFeature()->getScalarEntityTemperature().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getScalarEntityTemperature()->getId());
            element.typeOfElement = eScalarEntityTemperatureElement;
            element.scalarEntityTemperature = item->getFeature()->getScalarEntityTemperature();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getScalarEntityTemperature()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eSlottedHoleElement:{

            //check slotted hole
            if(item->getFeature().isNull() || item->getFeature()->getSlottedHole().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getSlottedHole()->getId());
            element.typeOfElement = eSlottedHoleElement;
            element.slottedHole = item->getFeature()->getSlottedHole();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getSlottedHole()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eSphereElement:{

            //check sphere
            if(item->getFeature().isNull() || item->getFeature()->getSphere().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getSphere()->getId());
            element.typeOfElement = eSphereElement;
            element.sphere = item->getFeature()->getSphere();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getSphere()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eTorusElement:{

            //check torus
            if(item->getFeature().isNull() || item->getFeature()->getTorus().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getTorus()->getId());
            element.typeOfElement = eTorusElement;
            element.torus = item->getFeature()->getTorus();
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getTorus()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eDirectionElement:{

            //check direction
            if(item->getFeature().isNull() || item->getFeature()->getGeometry().isNull() ||
                    !item->getFeature()->getGeometry()->hasDirection() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getGeometry()->getId());
            element.typeOfElement = eDirectionElement;
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getGeometry()->addUsedFor(target->getFeatureWrapper());

            return;

        }case ePositionElement:{

            //check position
            if(item->getFeature().isNull() || item->getFeature()->getGeometry().isNull() ||
                    !item->getFeature()->getGeometry()->hasPosition() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getGeometry()->getId());
            element.typeOfElement = ePositionElement;
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getGeometry()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eRadiusElement:{

            //check radius
            if(item->getFeature().isNull() || item->getFeature()->getGeometry().isNull() ||
                    !item->getFeature()->getGeometry()->hasRadius() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getGeometry()->getId());
            element.typeOfElement = eRadiusElement;
            element.geometry = item->getFeature()->getGeometry();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getGeometry()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eCoordinateSystemElement:{

            //check coordinate system
            if(item->getFeature().isNull() || item->getFeature()->getCoordinateSystem().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getCoordinateSystem()->getId());
            element.typeOfElement = eCoordinateSystemElement;
            element.coordSystem = item->getFeature()->getCoordinateSystem();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getCoordinateSystem()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eStationElement:{

            //check station
            if(item->getFeature().isNull() || item->getFeature()->getStation().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getStation()->getId());
            element.typeOfElement = eStationElement;
            element.station = item->getFeature()->getStation();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getStation()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eTrafoParamElement:{

            //check trafo param
            if(item->getFeature().isNull() || item->getFeature()->getTrafoParam().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getFeature()->getTrafoParam()->getId());
            element.typeOfElement = eTrafoParamElement;
            element.trafoParam = item->getFeature()->getTrafoParam();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            target->addPreviouslyNeeded(item->getFeature());
            item->getFeature()->getTrafoParam()->addUsedFor(target->getFeatureWrapper());

            return;

        }case eObservationElement:{

            //check observation
            if(item->getObservation().isNull() || target->getFeatureWrapper().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getObservation()->getId());
            element.typeOfElement = eObservationElement;
            element.observation = item->getObservation();
            function->addInputElement(element, this->neededElementIndex);

            //create dependencies
            if(!target->getFeatureWrapper()->getGeometry().isNull()){
                target->getFeatureWrapper()->getGeometry()->addObservation(item->getObservation());
                item->getObservation()->addTargetGeometry(target->getFeatureWrapper()->getGeometry());
            }

            return;

        }case eReadingCartesianElement:{

            //check reading
            if(item->getReading().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getReading()->getId());
            element.typeOfElement = eReadingCartesianElement;
            element.cartesianReading = item->getReading();
            function->addInputElement(element, this->neededElementIndex);

            return;

        }case eReadingPolarElement:{

            //check reading
            if(item->getReading().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getReading()->getId());
            element.typeOfElement = eReadingPolarElement;
            element.polarReading = item->getReading();
            function->addInputElement(element, this->neededElementIndex);

            return;

        }case eReadingDistanceElement:{

            //check reading
            if(item->getReading().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getReading()->getId());
            element.typeOfElement = eReadingDistanceElement;
            element.distanceReading = item->getReading();
            function->addInputElement(element, this->neededElementIndex);

            return;

        }case eReadingDirectionElement:{

            //check reading
            if(item->getReading().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getReading()->getId());
            element.typeOfElement = eReadingDirectionElement;
            element.directionReading = item->getReading();
            function->addInputElement(element, this->neededElementIndex);

            return;

        }case eReadingTemperatureElement:{

            //check reading
            if(item->getReading().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getReading()->getId());
            element.typeOfElement = eReadingTemperatureElement;
            element.temperatureReading = item->getReading();
            function->addInputElement(element, this->neededElementIndex);

            return;

        }case eReadingLevelElement:{

            //check reading
            if(item->getReading().isNull()){
                return;
            }

            //create and add input element
            InputElement element(item->getReading()->getId());
            element.typeOfElement = eReadingLevelElement;
            element.levelReading = item->getReading();
            function->addInputElement(element, this->neededElementIndex);

            return;

        }
        }

        return;

    }

    //if this item contains one or more items of the right type
    if(item->getHasElement(type)){
        for(int i = 0; i < item->getChildCount(); i++){
            this->addInputElement(target, function, type, item->getChild(i));
        }
    }

}
