#include "usedelementsmodel.h"

UsedElementsModel::UsedElementsModel(QObject *parent)
    : QAbstractItemModel(parent){
    this->rootItem = new FeatureTreeItem("used elements");
    this->selectedItem = NULL;
}

/*!
 * \brief UsedElementsModel::index
 * Create model index for every node
 * \param row
 * \param column
 * \param parent
 * \return
 */
QModelIndex UsedElementsModel::index(int row, int column, const QModelIndex &parent) const{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FeatureTreeItem *parentItem = NULL;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FeatureTreeItem*>(parent.internalPointer());

    if(parentItem != NULL){

        FeatureTreeItem *childItem = parentItem->getChild(row);
        if (childItem != NULL)
            return createIndex(row, column, childItem);
        else
            return QModelIndex();

    }else{
        return QModelIndex();
    }
}

/*!
 * \brief UsedElementsModel::parent
 * \param index
 * \return
 */
QModelIndex UsedElementsModel::parent(const QModelIndex &index) const{
    if (!index.isValid())
        return QModelIndex();

    FeatureTreeItem *childItem = static_cast<FeatureTreeItem*>(index.internalPointer());

    if(childItem != NULL){

        FeatureTreeItem *parentItem = childItem->getParent();

        if (parentItem == NULL || parentItem == rootItem)
            return QModelIndex();

        return createIndex(parentItem->getIndex(), 0, parentItem);

    }else{
        return QModelIndex();
    }
}

/*!
 * \brief UsedElementsModel::rowCount
 * \param parent
 * \return
 */
int UsedElementsModel::rowCount(const QModelIndex &parent) const{
    FeatureTreeItem *childItem = NULL;
    if(!parent.isValid()){
        childItem = rootItem;
    }else{
        childItem = static_cast<FeatureTreeItem*>(parent.internalPointer());
    }
    if(childItem != NULL){
        return childItem->getChildCount();
    }
    return 0;
}

/*!
 * \brief UsedElementsModel::columnCount
 * \param parent
 * \return
 */
int UsedElementsModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

/*!
 * \brief UsedElementsModel::data
 * \param index
 * \param role
 * \return
 */
QVariant UsedElementsModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid())
        return QVariant();

    FeatureTreeItem *item = static_cast<FeatureTreeItem*>(index.internalPointer());

    if(item != NULL){
        if(role == Qt::DisplayRole){ //return display text for tree view item
            FeatureWrapper *myFeature = item->getFeature();
            if(myFeature != NULL && myFeature->getGeometry() != NULL){
                QString geomHeader = QString("%1 - %2").arg(myFeature->getGeometry()->getFeatureName())
                        .arg(myFeature->getGeometry()->getIsNominal()?QString("nominal (%1)")
                                                                           .arg(myFeature->getGeometry()->getNominalSystem()->getFeatureName()):"actual");
                return geomHeader;
            }
            return item->getDisplayValue();
        }else if(role == Qt::DecorationRole){ //return icon for tree view item
            if(item->getIsFeature() && item->getFeature()->getPoint() != NULL){
                QPixmap pix(":/Images/icons/point_5e8acf.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getLine() != NULL){
                QPixmap pix(":/Images/icons/line_5e8acf.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getPlane() != NULL){
                QPixmap pix(":/Images/icons/plane_5e8acf.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getSphere() != NULL){
                QPixmap pix(":/Images/icons/sphere_5e8acf.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getStation() != NULL){
                QPixmap pix(":/Images/icons/station.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getScalarEntityAngle() != NULL){
                QPixmap pix(":/Images/icons/scalarEntities_5e8acf.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getScalarEntityDistance() != NULL){
                QPixmap pix(":/Images/icons/scalarEntities_5e8acf.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getTrafoParam() != NULL){
                QPixmap pix(":/Images/icons/trafoParam.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getCoordinateSystem() != NULL){
                QPixmap pix(":/Images/icons/coordinateSystem.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getCircle() != NULL){
                QPixmap pix(":/Images/icons/circle_5e8acf.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getCone() != NULL){
                QPixmap pix(":/Images/icons/cone_5e8acf.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getCylinder() != NULL){
                QPixmap pix(":/Images/icons/cylinder_5e8acf.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getEllipsoid() != NULL){
                QPixmap pix(":/Images/icons/ellipse_5e8acf.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getHyperboloid() != NULL){
                QPixmap pix(":/Images/icons/hyperboloid_5e8acf.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getParaboloid() != NULL){
                QPixmap pix(":/Images/icons/paraboloid_5e8acf.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getNurbs() != NULL){
                QPixmap pix(":/Images/icons/nurbs_5e8acf.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }else if(item->getIsFeature() && item->getFeature()->getPointCloud() != NULL){
                QPixmap pix(":/Images/icons/pointCloud_5e8acf.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }
        }
    }

    return QVariant();
}

/*!
 * \brief UsedElementsModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant UsedElementsModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        return "used elements";
    }
    return QVariant();
}

/*!
 * \brief UsedElementsModel::updateModel
 * The model contains all elements used for the function at functionIndex of the active feature (at elementIndex)
 * \param functionIndex: index of the function in the list of functions of active feature
 * \param elementIndex: index of the needed elements of the function
 */
void UsedElementsModel::updateModel(int functionIndex, int elementIndex){

    //clear previous elements
    this->removeAllElements();

    //check if active feature is valid
    if(OiJob::getActiveFeature() == NULL || OiJob::getActiveFeature()->getFeature() == NULL){
        this->refreshModel();
        return;
    }
    FeatureWrapper *myFeature = OiJob::getActiveFeature();

    //check if a function exists at functionIndex and has needed elements at elementIndex
    if(myFeature->getFeature()->getFunctions().size() <= functionIndex
            || myFeature->getFeature()->getFunctions().at(functionIndex) == NULL
            || myFeature->getFeature()->getFunctions().at(functionIndex)->getNeededElements().size() <= elementIndex
            || !myFeature->getFeature()->getFunctions().at(functionIndex)->getFeatureOrder().contains(elementIndex)){
        this->refreshModel();
        return;
    }
    Function *myFunction = myFeature->getFeature()->getFunctions().at(functionIndex);

    //get a list of elements to display
    QList<InputFeature> displayElements = myFunction->getFeatureOrder().value(elementIndex);

    //add the elements to the list of elements
    switch(myFunction->getNeededElements().at(elementIndex).typeOfElement){
    case Configuration::ePointElement:
        for(int i = 0; i < displayElements.size(); i++){
            Point *p = myFunction->getPoint(displayElements.at(i).id);
            if(p != NULL){
                FeatureWrapper *pointWrapper = new FeatureWrapper();
                pointWrapper->setPoint(p);
                FeatureTreeItem *point = new FeatureTreeItem(p->getFeatureName());
                point->setFeature(pointWrapper);
                this->addElement(point);
            }
        }
        break;
    case Configuration::eLineElement:
        for(int i = 0; i < displayElements.size(); i++){
            Line *l = myFunction->getLine(displayElements.at(i).id);
            if(l != NULL){
                FeatureWrapper *lineWrapper = new FeatureWrapper();
                lineWrapper->setLine(l);
                FeatureTreeItem *line = new FeatureTreeItem(l->getFeatureName());
                line->setFeature(lineWrapper);
                this->addElement(line);
            }
        }
        break;
    case Configuration::ePlaneElement:
        for(int i = 0; i < displayElements.size(); i++){
            Plane *p = myFunction->getPlane(displayElements.at(i).id);
            if(p != NULL){
                FeatureWrapper *planeWrapper = new FeatureWrapper();
                planeWrapper->setPlane(p);
                FeatureTreeItem *plane = new FeatureTreeItem(p->getFeatureName());
                plane->setFeature(planeWrapper);
                this->addElement(plane);
            }
        }
        break;
    case Configuration::eSphereElement:
        for(int i = 0; i < displayElements.size(); i++){
            Sphere *s = myFunction->getSphere(displayElements.at(i).id);
            if(s != NULL){
                FeatureWrapper *sphereWrapper = new FeatureWrapper();
                sphereWrapper->setSphere(s);
                FeatureTreeItem *sphere = new FeatureTreeItem(s->getFeatureName());
                sphere->setFeature(sphereWrapper);
                this->addElement(sphere);
            }
        }
        break;
    case Configuration::eScalarEntityAngleElement:
        for(int i = 0; i < displayElements.size(); i++){
            ScalarEntityAngle *s = myFunction->getScalarEntityAngle(displayElements.at(i).id);
            if(s != NULL){
                FeatureWrapper *angleWrapper = new FeatureWrapper();
                angleWrapper->setScalarEntityAngle(s);
                FeatureTreeItem *angle = new FeatureTreeItem(s->getFeatureName());
                angle->setFeature(angleWrapper);
                this->addElement(angle);
            }
        }
        break;
    case Configuration::eScalarEntityDistanceElement:
        for(int i = 0; i < displayElements.size(); i++){
            ScalarEntityDistance *s = myFunction->getScalarEntityDistance(displayElements.at(i).id);
            if(s != NULL){
                FeatureWrapper *distanceWrapper = new FeatureWrapper();
                distanceWrapper->setScalarEntityDistance(s);
                FeatureTreeItem *distance = new FeatureTreeItem(s->getFeatureName());
                distance->setFeature(distanceWrapper);
                this->addElement(distance);
            }
        }
        break;
    case Configuration::eTrafoParamElement:
        for(int i = 0; i < displayElements.size(); i++){
            TrafoParam *tp = myFunction->getTrafoParam(displayElements.at(i).id);
            if(tp != NULL){
                FeatureWrapper *trafoParamWrapper = new FeatureWrapper();
                trafoParamWrapper->setTrafoParam(tp);
                FeatureTreeItem *trafoParam = new FeatureTreeItem(tp->getFeatureName());
                trafoParam->setFeature(trafoParamWrapper);
                this->addElement(trafoParam);
            }
        }
        break;
    case Configuration::eCoordinateSystemElement:
        for(int i = 0; i < displayElements.size(); i++){
            CoordinateSystem *cs = myFunction->getCoordinateSystem(displayElements.at(i).id);
            if(cs != NULL){
                FeatureWrapper *coordinateSystemWrapper = new FeatureWrapper();
                coordinateSystemWrapper->setCoordinateSystem(cs);
                FeatureTreeItem *coordinateSystem = new FeatureTreeItem(cs->getFeatureName());
                coordinateSystem->setFeature(coordinateSystemWrapper);
                this->addElement(coordinateSystem);
            }
        }
        break;
    case Configuration::eStationElement:
        for(int i = 0; i < displayElements.size(); i++){
            Station *s = myFunction->getStation(displayElements.at(i).id);
            if(s != NULL){
                FeatureWrapper *stationWrapper = new FeatureWrapper();
                stationWrapper->setStation(s);
                FeatureTreeItem *station = new FeatureTreeItem(s->getFeatureName());
                station->setFeature(stationWrapper);
                this->addElement(station);
            }
        }
        break;
    case Configuration::eObservationElement:
        for(int i = 0; i < displayElements.size(); i++){
            Observation *o = myFunction->getObservation(displayElements.at(i).id);
            if(o != NULL && o->myReading != NULL){
                FeatureTreeItem *observation = new FeatureTreeItem(QString::number(o->myReading->id));
                observation->setObservation(o);
                this->addElement(observation);
            }
        }
        break;
    case Configuration::eReadingCartesianElement:
        for(int i = 0; i < displayElements.size(); i++){
            Reading *r = myFunction->getReadingCartesian(displayElements.at(i).id);
            if(r != NULL){
                FeatureTreeItem *reading = new FeatureTreeItem(r->measuredAt.toString());
                reading->setReading(r);
                this->addElement(reading);
            }
        }
        break;
    case Configuration::eReadingDirectionElement:
        for(int i = 0; i < displayElements.size(); i++){
            Reading *r = myFunction->getReadingDirection(displayElements.at(i).id);
            if(r != NULL){
                FeatureTreeItem *reading = new FeatureTreeItem(r->measuredAt.toString());
                reading->setReading(r);
                this->addElement(reading);
            }
        }
        break;
    case Configuration::eReadingDistanceElement:
        for(int i = 0; i < displayElements.size(); i++){
            Reading *r = myFunction->getReadingDistance(displayElements.at(i).id);
            if(r != NULL){
                FeatureTreeItem *reading = new FeatureTreeItem(r->measuredAt.toString());
                reading->setReading(r);
                this->addElement(reading);
            }
        }
        break;
    case Configuration::eReadingPolarElement:
        for(int i = 0; i < displayElements.size(); i++){
            Reading *r = myFunction->getReadingPolar(displayElements.at(i).id);
            if(r != NULL){
                FeatureTreeItem *reading = new FeatureTreeItem(r->measuredAt.toString());
                reading->setReading(r);
                this->addElement(reading);
            }
        }
        break;
    case Configuration::eScalarEntityTemperatureElement:
        for(int i = 0; i < displayElements.size(); i++){
            ScalarEntityTemperature *s = myFunction->getScalarEntityTemperature(displayElements.at(i).id);
            if(s != NULL){
                FeatureWrapper *temperatureWrapper = new FeatureWrapper();
                temperatureWrapper->setScalarEntityTemperature(s);
                FeatureTreeItem *temperature = new FeatureTreeItem(s->getFeatureName());
                temperature->setFeature(temperatureWrapper);
                this->addElement(temperature);
            }
        }
        break;
    }

    this->refreshModel();

}

/*!
 * \brief UsedElementsModel::refreshModel
 */
void UsedElementsModel::refreshModel(){
    emit this->beginResetModel();
    emit this->endResetModel();

    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

/*!
 * \brief UsedElementsModel::removeElement
 * Remove the specified element
 * \param item
 */
void UsedElementsModel::removeElement(FeatureTreeItem *item){
    if(item != NULL){
        int id = -1;
        //get id of the element in item
        if(item->getIsFeature() && item->getFeature() != NULL && item->getFeature()->getFeature() != NULL){
            id = item->getFeature()->getFeature()->getId();
        }else if(item->getIsObservation() && item->getObservation() != NULL){
            id = item->getObservation()->getId();
        }else if(item->getIsReading() && item->getReading() != NULL){
            id = item->getReading()->id;
        }
        if(id > 0){ //if id was found
            int index = -1;
            //get index of element with id in the list of elements
            for(int i = 0; i < this->items.size(); i++){
                FeatureTreeItem *tmpItem = this->items.at(i);
                if(tmpItem->getFeature() != NULL && tmpItem->getFeature()->getFeature() != NULL){
                    if(id == tmpItem->getFeature()->getFeature()->getId()){
                        index = i;
                        break;
                    }
                }else if(tmpItem->getObservation() != NULL){
                    if(id == tmpItem->getObservation()->getId()){
                        index = i;
                        break;
                    }
                }else if(tmpItem->getReading() != NULL){
                    if(id == tmpItem->getReading()->id){
                        index = i;
                        break;
                    }
                }
            }
            if(index > 0){ //if index exists
                if(this->selectedItem == this->items.at(index)){ //if the item which will be deleted is the currently selected item
                    this->selectedItem = NULL;
                }
                if(this->items.at(index)->getFeature() != NULL){
                    delete this->items.at(index)->getFeature();
                }
                this->items.removeAt(index);
                this->rootItem->removeChild(index);
            }
        }
    }
}

/*!
 * \brief UsedElementsModel::addElement
 * Create and add an element
 * \param item
 */
void UsedElementsModel::addElement(FeatureTreeItem *item){
    if(item != NULL){
        this->items.append(item);
        this->rootItem->appendChild(item);
    }
}

/*!
 * \brief UsedElementsModel::removeAllElements
 * Delete and remove all used elements and feature wrapper objects
 */
void UsedElementsModel::removeAllElements(){
    foreach(FeatureTreeItem *item, this->items){
        if(item->getFeature() != NULL){
            delete item->getFeature();
        }
    }
    this->rootItem->deleteChildren();
    this->items.clear();
    this->selectedItem = NULL;
}

FeatureTreeItem *UsedElementsModel::getSelectedItem(const QModelIndex &index){
    FeatureTreeItem *item = NULL;
    item = static_cast<FeatureTreeItem*>(index.internalPointer());
    return item;
}
