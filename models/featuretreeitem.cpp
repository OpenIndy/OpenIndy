#include "featuretreeitem.h"

FeatureTreeItem::FeatureTreeItem(QVariant display)
    :parent(NULL), feature(NULL), reading(NULL), observation(NULL), childCount(0),
      isHeader(true), isFeature(false), isObservation(false), isReading(false), hasObservation(false),
      hasCartesianReading(false), hasDirectionReading(false), hasDistanceReading(false), hasPolarReading(false),
      hasLevelReading(false), hasTemperatureReading(false)
{
    this->displayValue = display;
    this->index = 0;
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
bool FeatureTreeItem::getIsHeader(){
    return this->isHeader;
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
 * \brief FeatureTreeItem::getIsFeature
 * \return
 */
bool FeatureTreeItem::getIsFeature(){
    return this->isFeature;
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
 * \brief FeatureTreeItem::setFeature
 * \param feature
 */
void FeatureTreeItem::setFeature(FeatureWrapper *feature){
    this->feature = feature;
    this->setIsFeature();
}

/*!
 * \brief FeatureTreeItem::getFeature
 * \return
 */
FeatureWrapper* FeatureTreeItem::getFeature(){
    return this->feature;
}

/*!
 * \brief FeatureTreeItem::getIsObservation
 * \return
 */
bool FeatureTreeItem::getIsObservation(){
    return this->isObservation;
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
 * \brief FeatureTreeItem::setObservation
 * \param observation
 */
void FeatureTreeItem::setObservation(Observation *observation){
    this->observation = observation;
    this->setIsObservation();
    this->setHasObservation(true);
}

/*!
 * \brief FeatureTreeItem::getObservation
 * \return
 */
Observation* FeatureTreeItem::getObservation(){
    return this->observation;
}

/*!
 * \brief FeatureTreeItem::getIsReading
 * \return
 */
bool FeatureTreeItem::getIsReading(){
    return this->isReading;
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
 * \brief FeatureTreeItem::setReading
 * \param reading
 */
void FeatureTreeItem::setReading(Reading *reading){
    this->reading = reading;
    switch (reading->typeofReading){
        case Configuration::eCartesian:
            this->setHasReadingCartesian(true);
        case Configuration::eDirection:
            this->setHasReadingDirection(true);
        case Configuration::eDistance:
            this->setHasReadingDistance(true);
        case Configuration::ePolar:
            this->setHasReadingPolar(true);
        case Configuration::eLevel:
            this->setHasReadingLevel(true);
        case Configuration::eTemperatur:
            this->setHasReadingTemperature(true);
    }
    this->setIsReading();
}

/*!
 * \brief FeatureTreeItem::getReading
 * \return
 */
Reading* FeatureTreeItem::getReading(){
    return this->reading;
}

/*!
 * \brief FeatureTreeItem::getElementType
 * Returns the type of element of this item. If the item is no element it returns eUndefinedElement
 * \return
 */
Configuration::ElementTypes FeatureTreeItem::getElementType(){

    if(this->getIsObservation()){
        return Configuration::eObservationElement;
    }else if(this->getIsReading() && this->getReading() != NULL){
        switch(this->getReading()->typeofReading){
            case Configuration::eDistance:
                return Configuration::eReadingDistanceElement;
            case Configuration::eDirection:
                return Configuration::eReadingDirectionElement;
            case Configuration::ePolar:
                return Configuration::eReadingPolarElement;
            case Configuration::eCartesian:
                return Configuration::eReadingCartesianElement;
            case Configuration::eTemperatur:
                return Configuration::eReadingTemperatureElement;
            case Configuration::eLevel:
                return Configuration::eReadingLevelElement;
        }
    }else if(this->getIsFeature() && this->getFeature() != NULL){
        switch(this->getFeature()->getTypeOfFeature()){
            case Configuration::ePointFeature:
                return Configuration::ePointElement;
            case Configuration::eLineFeature:
                return Configuration::eLineElement;
            case Configuration::ePlaneFeature:
                return Configuration::ePlaneElement;
            case Configuration::eSphereFeature:
                return Configuration::eSphereElement;
            case Configuration::eCircleFeature:
                return Configuration::eCircleElement;
            case Configuration::eConeFeature:
                return Configuration::eConeElement;
            case Configuration::eCylinderFeature:
                return Configuration::eCylinderElement;
            case Configuration::eEllipsoidFeature:
                return Configuration::eEllipsoidElement;
            case Configuration::eHyperboloidFeature:
                return Configuration::eHyperboloidElement;
            case Configuration::eParaboloidFeature:
                return Configuration::eParaboloidElement;
            case Configuration::ePointCloudFeature:
                return Configuration::ePointCloudElement;
            case Configuration::eScalarEntityMeasurementSeriesFeature:
                return Configuration::eScalarEntityMeasurementSeriesElement;
            case Configuration::eScalarEntityAngleFeature:
                return Configuration::eScalarEntityAngleElement;
            case Configuration::eScalarEntityDistanceFeature:
                return Configuration::eScalarEntityDistanceElement;
            case Configuration::eCoordinateSystemFeature:
                return Configuration::eCoordinateSystemElement;
            case Configuration::eStationFeature:
                return Configuration::eStationElement;
            case Configuration::eTrafoParamFeature:
                return Configuration::eTrafoParamElement;
            case Configuration::eScalarEntityTemperatureFeature:
                return Configuration::eScalarEntityTemperatureElement;
        }
    }

    return Configuration::eUndefinedElement;

}

/*!
 * \brief FeatureTreeItem::setHasObservation
 * \param hasObservation
 */
void FeatureTreeItem::setHasObservation(bool hasObservation){
    this->hasObservation = hasObservation;
    if(this->hasParent() && this->parent != NULL){
        this->parent->setHasObservation(hasObservation);
    }
}

/*!
 * \brief FeatureTreeItem::getHasObservation
 * \return
 */
bool FeatureTreeItem::getHasObservation(){
    return this->hasObservation;
}

/*!
 * \brief FeatureTreeItem::setHasReadingCartesian
 * \param hasReadingCartesian
 */
void FeatureTreeItem::setHasReadingCartesian(bool hasReadingCartesian){
    this->hasCartesianReading = hasReadingCartesian;
    if(this->hasParent() && this->parent != NULL){
        this->parent->setHasReadingCartesian(hasReadingCartesian);
    }
}

/*!
 * \brief FeatureTreeItem::getHasReadingCartesian
 * \return
 */
bool FeatureTreeItem::getHasReadingCartesian(){
    return this->hasCartesianReading;
}

/*!
 * \brief FeatureTreeItem::setHasReadingDirection
 * \param hasReadingDirection
 */
void FeatureTreeItem::setHasReadingDirection(bool hasReadingDirection){
    this->hasCartesianReading = hasReadingDirection;
    if(this->hasParent() && this->parent != NULL){
        this->parent->setHasReadingDirection(hasReadingDirection);
    }
}

/*!
 * \brief FeatureTreeItem::getHasReadingDirection
 * \return
 */
bool FeatureTreeItem::getHasReadingDirection(){
    return this->hasDirectionReading;
}

/*!
 * \brief FeatureTreeItem::setHasReadingDistance
 * \param hasReadingDistance
 */
void FeatureTreeItem::setHasReadingDistance(bool hasReadingDistance){
    this->hasCartesianReading = hasReadingDistance;
    if(this->hasParent() && this->parent != NULL){
        this->parent->setHasReadingDistance(hasReadingDistance);
    }
}

/*!
 * \brief FeatureTreeItem::getHasReadingDistance
 * \return
 */
bool FeatureTreeItem::getHasReadingDistance(){
    return this->hasDistanceReading;
}

/*!
 * \brief FeatureTreeItem::setHasReadingLevel
 * \param hasReadingLevel
 */
void FeatureTreeItem::setHasReadingLevel(bool hasReadingLevel){
    this->hasCartesianReading = hasReadingLevel;
    if(this->hasParent() && this->parent != NULL){
        this->parent->setHasReadingLevel(hasReadingLevel);
    }
}

/*!
 * \brief FeatureTreeItem::getHasReadingLevel
 * \return
 */
bool FeatureTreeItem::getHasReadingLevel(){
    return this->hasLevelReading;
}

/*!
 * \brief FeatureTreeItem::setHasReadingPolar
 * \param hasReadingPolar
 */
void FeatureTreeItem::setHasReadingPolar(bool hasReadingPolar){
    this->hasCartesianReading = hasReadingPolar;
    if(this->hasParent() && this->parent != NULL){
        this->parent->setHasReadingPolar(hasReadingPolar);
    }
}

/*!
 * \brief FeatureTreeItem::getHasReadingPolar
 * \return
 */
bool FeatureTreeItem::getHasReadingPolar(){
    return this->hasPolarReading;
}

/*!
 * \brief FeatureTreeItem::setHasReadingTemperature
 * \param hasReadingTemperature
 */
void FeatureTreeItem::setHasReadingTemperature(bool hasReadingTemperature){
    this->hasCartesianReading = hasReadingTemperature;
    if(this->hasParent() && this->parent != NULL){
        this->parent->setHasReadingTemperature(hasReadingTemperature);
    }
}

/*!
 * \brief FeatureTreeItem::getHasReadingTemperature
 * \return
 */
bool FeatureTreeItem::getHasReadingTemperature(){
    return this->hasTemperatureReading;
}

/*!
 * \brief FeatureTreeItem::appendChild
 * \param child
 */
void FeatureTreeItem::appendChild(QPointer<FeatureTreeItem> child){
    child->setIndex(this->children.size());
    child->setParent(this);
    this->children.append(child);
    this->childCount++;
}

/*!
 * \brief FeatureTreeItem::getChild
 * \param index
 * \return
 */
QPointer<FeatureTreeItem> FeatureTreeItem::getChild(int index){
    if(this->childCount > index){
        return this->children.at(index);
    }
    return NULL;
}

/*!
 * \brief FeatureTreeItem::removeChild
 * \param index
 */
void FeatureTreeItem::removeChild(int index){
    if(this->childCount > index){
        FeatureTreeItem *itemToRemove = this->children.at(index);
        if(itemToRemove != NULL){
            itemToRemove->deleteChildren();
            delete itemToRemove;
        }
        this->children.removeAt(index);
        this->childCount--;
    }
}

/*!
 * \brief FeatureTreeItem::deleteChildren
 */
void FeatureTreeItem::deleteChildren(){
    qDeleteAll(this->children);
    this->children.clear();
    this->childCount = 0;
}

/*!
 * \brief FeatureTreeItem::getChildCount
 * \return
 */
int FeatureTreeItem::getChildCount(){
    return this->childCount;
}

/*!
 * \brief FeatureTreeItem::setIndex
 * \param index
 */
void FeatureTreeItem::setIndex(int index){
    this->index = index;
}

/*!
 * \brief FeatureTreeItem::getIndex
 * \return
 */
int FeatureTreeItem::getIndex(){
    return this->index;
}

/*!
 * \brief FeatureTreeItem::hasParent
 * \return
 */
bool FeatureTreeItem::hasParent(){
    if(this->parent != NULL){
        return true;
    }
    return false;
}

/*!
 * \brief FeatureTreeItem::setParent
 * \param parent
 */
void FeatureTreeItem::setParent(QPointer<FeatureTreeItem> parent){
    this->parent = parent;
}

/*!
 * \brief FeatureTreeItem::getParent
 * \return
 */
FeatureTreeItem* FeatureTreeItem::getParent(){
    return this->parent;
}

/*!
 * \brief FeatureTreeItem::getDisplayValue
 * \return
 */
QVariant FeatureTreeItem::getDisplayValue(){
    return this->displayValue;
}
