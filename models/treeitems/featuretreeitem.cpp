#include "featuretreeitem.h"

/*!
 * \brief FeatureTreeItem::FeatureTreeItem
 * Creates a tree item that holds a header
 * \param display
 * \param parent
 */
FeatureTreeItem::FeatureTreeItem(const QVariant &display, QObject *parent) : QObject(parent){

    //set general tree item attributes
    this->childCount = 0;
    this->index = -1;

    //define what kind of tree item this item is
    this->setIsHeader();

    //set display value
    this->displayValue = display;

}

/*!
 * \brief FeatureTreeItem::FeatureTreeItem
 * Creates a tree item that holds a feature
 * \param feature
 * \param parent
 */
FeatureTreeItem::FeatureTreeItem(const QPointer<FeatureWrapper> &feature, QObject *parent) : QObject(parent){

    //set general tree item attributes
    this->childCount = 0;
    this->index = -1;

    //define what kind of tree item this item is
    this->setIsFeature();

    //check feature
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    //set information about child items
    this->containedElementTypes.append(getElementTypeEnum(feature->getFeatureTypeEnum()));
    if(!feature->getGeometry().isNull()){
        if(feature->getGeometry()->hasDirection()){
            this->containedElementTypes.append(eDirectionElement);
        }
        if(feature->getGeometry()->hasPosition()){
            this->containedElementTypes.append(ePositionElement);
        }
        if(feature->getGeometry()->hasRadius()){
            this->containedElementTypes.append(eRadiusElement);
        }
    }

    //set feature
    this->feature = feature;

    //set up item and its children
    this->setUpFeature();

}

/*!
 * \brief FeatureTreeItem::FeatureTreeItem
 * Creates a tree item that holds an observation
 * \param observation
 * \param parent
 */
FeatureTreeItem::FeatureTreeItem(const QPointer<Observation> &observation, QObject *parent) : QObject(parent){

    //set general tree item attributes
    this->childCount = 0;
    this->index = -1;

    //define what kind of tree item this item is
    this->setIsObservation();

    //check observation
    if(observation.isNull() || observation->getReading().isNull()){
        return;
    }

    //set information about child items
    this->containedElementTypes.append(eObservationElement);

    //set observation
    this->observation = observation;

    //set up item and its children
    this->setUpObservation();

}

/*!
 * \brief FeatureTreeItem::FeatureTreeItem
 * Creates a tree item that holds a reading
 * \param reading
 * \param parent
 */
FeatureTreeItem::FeatureTreeItem(const QPointer<Reading> &reading, QObject *parent) : QObject(parent){

    //set general tree item attributes
    this->childCount = 0;
    this->index = -1;

    //define what kind of tree item this item is
    this->setIsReading();

    //check reading
    if(reading.isNull()){
        return;
    }

    //set information about child items
    this->containedElementTypes.append(getElementTypeEnum(reading->getTypeOfReading()));

    //set reading
    this->reading = reading;

    //set up item and its children
    this->setUpReading();

}

/*!
 * \brief FeatureTreeItem::~FeatureTreeItem
 * Deletes this item and all its children
 */
FeatureTreeItem::~FeatureTreeItem(){
    this->deleteChildren();
}

/*!
 * \brief FeatureTreeItem::getIsHeader
 * \return
 */
const bool &FeatureTreeItem::getIsHeader() const{
    return this->isHeader;
}

/*!
 * \brief FeatureTreeItem::getIsFeature
 * \return
 */
const bool &FeatureTreeItem::getIsFeature() const{
    return this->isFeature;
}

/*!
 * \brief FeatureTreeItem::getIsObservation
 * \return
 */
const bool &FeatureTreeItem::getIsObservation() const{
    return this->isObservation;
}

/*!
 * \brief FeatureTreeItem::getIsReading
 * \return
 */
const bool &FeatureTreeItem::getIsReading() const{
    return this->isReading;
}

/*!
 * \brief FeatureTreeItem::getFeature
 * \return
 */
const QPointer<FeatureWrapper> &FeatureTreeItem::getFeature() const{
    return this->feature;
}

/*!
 * \brief FeatureTreeItem::getObservation
 * \return
 */
const QPointer<Observation> &FeatureTreeItem::getObservation() const{
    return this->observation;
}

/*!
 * \brief FeatureTreeItem::getReading
 * \return
 */
const QPointer<Reading> &FeatureTreeItem::getReading() const{
    return this->reading;
}


/*!
 * \brief FeatureTreeItem::getElementType
 * Returns the type of element of this item. If the item is no element it returns eUndefinedElement
 * \return
 */
ElementTypes FeatureTreeItem::getElementType() const{

    if(this->getIsObservation()){

        return eObservationElement;

    }else if(this->getIsReading() && !this->reading.isNull()){

        switch(this->reading->getTypeOfReading()){
        case eDistanceReading:
            return eReadingDistanceElement;
        case eDirectionReading:
            return eReadingDirectionElement;
        case ePolarReading:
            return eReadingPolarElement;
        case eCartesianReading:
            return eReadingCartesianElement;
        case eTemperatureReading:
            return eReadingTemperatureElement;
        case eLevelReading:
            return eReadingLevelElement;
        }

    }else if(this->getIsFeature() && !this->feature.isNull()){

        switch(this->feature->getFeatureTypeEnum()){
        case ePointFeature:
            return ePointElement;
        case eLineFeature:
            return eLineElement;
        case ePlaneFeature:
            return ePlaneElement;
        case eSphereFeature:
            return eSphereElement;
        case eCircleFeature:
            return eCircleElement;
        case eConeFeature:
            return eConeElement;
        case eCylinderFeature:
            return eCylinderElement;
        case eEllipsoidFeature:
            return eEllipsoidElement;
        case eHyperboloidFeature:
            return eHyperboloidElement;
        case eParaboloidFeature:
            return eParaboloidElement;
        case ePointCloudFeature:
            return ePointCloudElement;
        case eScalarEntityMeasurementSeriesFeature:
            return eScalarEntityMeasurementSeriesElement;
        case eScalarEntityAngleFeature:
            return eScalarEntityAngleElement;
        case eScalarEntityDistanceFeature:
            return eScalarEntityDistanceElement;
        case eCoordinateSystemFeature:
            return eCoordinateSystemElement;
        case eStationFeature:
            return eStationElement;
        case eTrafoParamFeature:
            return eTrafoParamElement;
        case eScalarEntityTemperatureFeature:
            return eScalarEntityTemperatureElement;
        case eEllipseFeature:
            return eEllipseElement;
        case eNurbsFeature:
            return eNurbsElement;
        case eSlottedHoleFeature:
            return eSlottedHoleElement;
        case eTorusFeature:
            return eTorusElement;
        }

    }

    return eUndefinedElement;

}

/*!
 * \brief FeatureTreeItem::getHasElement
 * \return
 */
bool FeatureTreeItem::getHasElement(const ElementTypes &typeOfElement) const{
    return this->containedElementTypes.contains(typeOfElement);
}

/*!
 * \brief FeatureTreeItem::getChildCount
 * \return
 */
int FeatureTreeItem::getChildCount() const{
    return this->childCount;
}

/*!
 * \brief FeatureTreeItem::appendChild
 * \param child
 */
void FeatureTreeItem::appendChild(const QPointer<FeatureTreeItem> &child){

    //check child
    if(child.isNull()){
        return;
    }

    //set index and parent for child
    child->index = this->children.size();
    child->parent = this;

    //add the child
    this->children.append(child);
    this->childCount++;

    //update information about child items
    foreach(const ElementTypes &type, child->containedElementTypes){
        this->setHasElement(true, type);
    }

}

/*!
 * \brief FeatureTreeItem::getChild
 * \param index
 * \return
 */
QPointer<FeatureTreeItem> FeatureTreeItem::getChild(const int &index){
    if(this->childCount > index){
        return this->children.at(index);
    }
    return QPointer<FeatureTreeItem>(NULL);
}

/*!
 * \brief FeatureTreeItem::removeChild
 * \param index
 */
/*void FeatureTreeItem::removeChild(const int &index){
    if(this->childCount > index){
        QPointer<FeatureTreeItem> itemToRemove = this->children.at(index);
        if(!itemToRemove.isNull()){
            itemToRemove->deleteChildren();
            delete itemToRemove.data();
        }
        this->children.removeAt(index);
        this->childCount--;
    }
}*/

/*!
 * \brief FeatureTreeItem::deleteChildren
 */
void FeatureTreeItem::deleteChildren(){
    qDeleteAll(this->children);
    this->children.clear();
    this->childCount = 0;
}

/*!
 * \brief FeatureTreeItem::getIndex
 * \return
 */
const int &FeatureTreeItem::getIndex() const{
    return this->index;
}

/*!
 * \brief FeatureTreeItem::hasParent
 * \return
 */
bool FeatureTreeItem::hasParent() const{
    if(this->parent.isNull()){
        return false;
    }
    return true;
}

/*!
 * \brief FeatureTreeItem::getParent
 * \return
 */
const QPointer<FeatureTreeItem> &FeatureTreeItem::getParent() const{
    return this->parent;
}

/*!
 * \brief FeatureTreeItem::getDisplayValue
 * \return
 */
const QVariant &FeatureTreeItem::getDisplayValue() const{
    return this->displayValue;
}

/*!
 * \brief FeatureTreeItem::setIsHeader
 */
void FeatureTreeItem::setIsHeader(){
    this->isHeader = true;
    this->isFeature = false;
    this->isObservation = false;
    this->isReading = false;
}

/*!
 * \brief FeatureTreeItem::setIsFeature
 */
void FeatureTreeItem::setIsFeature(){
    this->isHeader = false;
    this->isFeature = true;
    this->isObservation = false;
    this->isReading = false;
}

/*!
 * \brief FeatureTreeItem::setIsObservation
 */
void FeatureTreeItem::setIsObservation(){
    this->isHeader = false;
    this->isFeature = false;
    this->isObservation = true;
    this->isReading = false;
}

/*!
 * \brief FeatureTreeItem::setIsReading
 */
void FeatureTreeItem::setIsReading(){
    this->isHeader = false;
    this->isFeature = false;
    this->isObservation = false;
    this->isReading = true;
}

/*!
 * \brief FeatureTreeItem::setHasElement
 * \param hasElement
 * \param typeOfElement
 */
void FeatureTreeItem::setHasElement(const bool &hasElement, const ElementTypes &typeOfElement){

    //update contained elements for this item
    if(hasElement && !this->containedElementTypes.contains(typeOfElement)){
        this->containedElementTypes.append(typeOfElement);
    }else if(!hasElement){
        this->containedElementTypes.removeOne(typeOfElement);
    }

    //update contained elements for this item's parent item
    if(!this->parent.isNull()){
        this->parent->setHasElement(hasElement, typeOfElement);
    }

}

/*!
 * \brief FeatureTreeItem::setUpFeature
 * Set up children for the tree item
 */
void FeatureTreeItem::setUpFeature(){

    if(this->feature->getFeatureTypeEnum() == eStationFeature){

        //check station
        if(this->feature->getStation().isNull() || this->feature->getStation()->getCoordinateSystem().isNull()){
            return;
        }

        //set display value
        this->displayValue = this->feature->getStation()->getFeatureName();

        //add observations and readings as children
        FeatureTreeItem *itemObservations = new FeatureTreeItem("observations:");
        FeatureTreeItem *itemReadings = new FeatureTreeItem("readings:");
        foreach(const QPointer<Observation> &obs, this->feature->getStation()->getCoordinateSystem()->getObservations()){
            if(!obs.isNull() && !obs->getReading().isNull()){
                FeatureTreeItem *itemObservation = new FeatureTreeItem(obs);
                itemObservations->appendChild(itemObservation);
                FeatureTreeItem *itemReading = new FeatureTreeItem(obs->getReading());
                itemReadings->appendChild(itemReading);
            }
        }
        this->appendChild(itemObservations);
        this->appendChild(itemReadings);

    }else if(this->feature->getFeatureTypeEnum() == eCoordinateSystemFeature){

        //check coordinate system
        if(this->feature->getCoordinateSystem().isNull() || this->feature->getCoordinateSystem()->getIsStationSystem()){
            return;
        }

        //set display value
        this->displayValue = this->feature->getCoordinateSystem()->getFeatureName();

        //add nominals as children
        /*FeatureTreeItem *itemNominals = new FeatureTreeItem("nominals:");
        foreach(const QPointer<FeatureWrapper> &nominal, this->feature->getCoordinateSystem()->getNominals()){
            if(!nominal.isNull() && !nominal->getGeometry().isNull()){
                FeatureTreeItem *itemNominal = new FeatureTreeItem(nominal);
                itemNominals->appendChild(itemNominal);
            }
        }
        this->appendChild(itemNominals);*/

    }else if(this->feature->getFeatureTypeEnum() == eTrafoParamFeature){

        //check trafo param
        if(this->feature->getTrafoParam().isNull()){
            return;
        }

        //set display value
        this->displayValue = this->feature->getTrafoParam()->getFeatureName();

        //add start and destination system as children
        if(this->feature->getTrafoParam()->getStartSystem().isNull()
                || this->feature->getTrafoParam()->getDestinationSystem().isNull()){
            return;
        }
        FeatureTreeItem *itemStartSystem = new FeatureTreeItem(QString("start system: %1").arg(this->feature->getTrafoParam()->getStartSystem()->getFeatureName()));
        FeatureTreeItem *itemDestinationSystem = new FeatureTreeItem(QString("destination system: %1").arg(this->feature->getTrafoParam()->getDestinationSystem()->getFeatureName()));
        this->appendChild(itemStartSystem);
        this->appendChild(itemDestinationSystem);

    }else if(getIsGeometry(this->feature->getFeatureTypeEnum())){

        //check geometry
        if(this->feature->getGeometry().isNull() || (this->feature->getGeometry()->getIsNominal() && this->feature->getGeometry()->getNominalSystem().isNull())){
            return;
        }

        //set display value
        this->displayValue = QString("%1 - %2").arg(this->feature->getGeometry()->getFeatureName())
                .arg(this->feature->getGeometry()->getIsNominal()?QString("nominal (%1)")
                                                                  .arg(this->feature->getGeometry()->getNominalSystem()->getFeatureName()):"actual");

        //add observations and readings as children (only for actuals)
        if(this->feature->getGeometry()->getIsNominal()){
            return;
        }
        FeatureTreeItem *itemObservations = new FeatureTreeItem("observations:");
        FeatureTreeItem *itemReadings = new FeatureTreeItem("readings:");
        foreach(const QPointer<Observation> &obs, this->feature->getGeometry()->getObservations()){
            if(!obs.isNull() && !obs->getReading().isNull()){
                FeatureTreeItem *itemObservation = new FeatureTreeItem(obs);
                itemObservations->appendChild(itemObservation);
                FeatureTreeItem *itemReading = new FeatureTreeItem(obs->getReading());
                itemReadings->appendChild(itemReading);
            }
        }
        this->appendChild(itemObservations);
        this->appendChild(itemReadings);

    }

}

/*!
 * \brief FeatureTreeItem::setUpObservation
 * Set up children for the tree item
 */
void FeatureTreeItem::setUpObservation(){

    //set display value
    //this->displayValue = this->observation->getReading()->getMeasuredAt().toString();
    /*if(this->observation->getTargetGeometries().size() > 0){
        this->displayValue = "id:" + QString::number(this->observation->getId()) + " " +  this->observation->getTargetGeometries().at(0)->getFeatureName();
    }else{
        this->displayValue = "id:" + QString::number(this->observation->getId()) + " " + this->observation->getStation()->getFeatureName();
    }*/

    if(!this->observation->getMeasuredTargetGeometry().isNull()){
        this->displayValue = "id:" + QString::number(this->observation->getId()) + " " +  this->observation->getMeasuredTargetGeometry()->getFeatureName();
    }else{
        this->displayValue = "id:" + QString::number(this->observation->getId()) + " " + this->observation->getStation()->getFeatureName();
    }
}

/*!
 * \brief FeatureTreeItem::setUpReading
 * Set up children for the tree item
 */
void FeatureTreeItem::setUpReading(){

    //set display value
    this->displayValue = this->reading->getMeasuredAt().toString();
    /*if(this->reading->getObservation()->getTargetGeometries().size() > 0){
        this->displayValue = "id:" + QString::number(this->reading->getId()) + this->reading->getObservation()->getTargetGeometries().at(0)->getFeatureName();
    }else{
        this->displayValue = "id:" + QString::number(this->reading->getId()) + this->reading->getObservation()->getStation()->getFeatureName();
    }*/
    if(!this->reading->getObservation()->getMeasuredTargetGeometry().isNull()){
        this->displayValue = "id:" + QString::number(this->reading->getId()) + this->reading->getObservation()->getMeasuredTargetGeometry()->getFeatureName();
    }else{
        this->displayValue = "id:" + QString::number(this->reading->getId()) + this->reading->getObservation()->getStation()->getFeatureName();
    }
}

/*!
 * \brief FeatureTreeItem::getId
 * \return feature / observation / reading id or -1
 */
int FeatureTreeItem::getId() {

    if(getIsFeature() && !getFeature().isNull() && !getFeature()->getFeature().isNull()){
        return getFeature()->getFeature()->getId();
    }else if(getIsObservation() && !getObservation().isNull()){
        return getObservation()->getId();
    }else if(getIsReading() && !getReading().isNull()){
        return getReading()->getId();
    }

    return  -1;
}
