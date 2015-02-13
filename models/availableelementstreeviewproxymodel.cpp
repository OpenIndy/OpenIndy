#include "availableelementstreeviewproxymodel.h"

AvailableElementsTreeViewProxyModel::AvailableElementsTreeViewProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    this->hideAll = true;
    this->neededElement = Configuration::eUndefinedElement;
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

        return header;
    }
    return QVariant();
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::setHeader
 * \param header
 */
void AvailableElementsTreeViewProxyModel::setHeader(QString header){
    this->header = header;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::getNeededElement
 * \return
 */
Configuration::ElementTypes AvailableElementsTreeViewProxyModel::getNeededElement(){
    return this->neededElement;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::getSelectedItem
 * \param index
 * \return
 */
FeatureTreeItem* AvailableElementsTreeViewProxyModel::getSelectedItem(const QModelIndex &index){
    FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
    FeatureTreeItem *item = NULL;
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
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::setFilter
 * \param typeOfElement
 * \param hideAll
 */
void AvailableElementsTreeViewProxyModel::setFilter(Configuration::ElementTypes typeOfElement, bool hideAll){
    this->hideAll = hideAll;
    this->neededElement = typeOfElement;
    this->invalidateFilter();
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{
    //if(source_parent.isValid() == false){ //root item
        /*FeatureTreeItem *root = static_cast<FeatureTreeItem*>(source_parent.internalPointer());
        if(root != NULL){
            if(root->getChildCount() > source_row){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && item->getChildCount() > 0 && item->getChild(0) != NULL && item->getChild(0)->getFeature() != NULL
                        && item->getChild(0)->getFeature()->getPoint() != NULL){
                    return false;
                }
            }
        }*/
    //}
    /*if(source_parent.isValid() == true){
        return false;
    }*/

    if(!this->hideAll){
        switch (this->neededElement){
            case Configuration::ePointElement:
                return this->filterAcceptPoint(source_row, source_parent);
            case Configuration::eLineElement:
                return this->filterAcceptLine(source_row, source_parent);
            case Configuration::ePlaneElement:
                return this->filterAcceptPlane(source_row, source_parent);
            case Configuration::eSphereElement:
                return this->filterAcceptSphere(source_row, source_parent);
            case Configuration::eScalarEntityAngleElement:
                return this->filterAcceptScalarEntityAngle(source_row, source_parent);
            case Configuration::eScalarEntityDistanceElement:
                return this->filterAcceptScalarEntityDistance(source_row, source_parent);
            case Configuration::eCoordinateSystemElement:
                return this->filterAcceptCoordinateSystem(source_row, source_parent);
            case Configuration::eStationElement:
                return this->filterAcceptStation(source_row, source_parent);
            case Configuration::eTrafoParamElement:
                return this->filterAcceptTrafoParam(source_row, source_parent);
            case Configuration::eObservationElement:
                return this->filterAcceptObservation(source_row, source_parent);
            case Configuration::eReadingCartesianElement:
                return this->filterAcceptReadingCartesian(source_row, source_parent);
            case Configuration::eReadingDirectionElement:
                return this->filterAcceptReadingDirection(source_row, source_parent);
            case Configuration::eReadingDistanceElement:
                return this->filterAcceptReadingDistance(source_row, source_parent);
            case Configuration::eReadingPolarElement:
                return this->filterAcceptReadingPolar(source_row, source_parent);
            case Configuration::eScalarEntityTemperatureElement:
                return this->filterAcceptScalarEntityTemperature(source_row, source_parent);
        }
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
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptPoint
 * Decide wether to accept a row of the treeview while a point is the needed element
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptPoint(int source_row, const QModelIndex & source_parent) const{
    if(!source_parent.isValid()){ //root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL){
            FeatureTreeItem *root = model->getRootItem();
            if(root != NULL){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && item->getChildCount() > 0){
                    FeatureTreeItem *possiblePoint = item->getChild(0);
                    //if child is not a point
                    if(possiblePoint != NULL && possiblePoint->getFeature() != NULL && possiblePoint->getFeature()->getPoint() == NULL){
                        return false;
                    }
                }
            }
        }
    }else{
        //TODO do not display the points that were already chosen
    }
    return true;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptLine
 * Decide wether to accept a row of the treeview while a line is the needed element
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptLine(int source_row, const QModelIndex & source_parent) const{
    if(!source_parent.isValid()){ //root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL){
            FeatureTreeItem *root = model->getRootItem();
            if(root != NULL){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && item->getChildCount() > 0){
                    FeatureTreeItem *possibleLine = item->getChild(0);
                    //if child is not a line
                    if(possibleLine != NULL && possibleLine->getFeature() != NULL && possibleLine->getFeature()->getLine() == NULL){
                        return false;
                    }
                }
            }
        }
    }else{
        //TODO do not display the lines that were already chosen
    }
    return true;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptPlane
 * Decide wether to accept a row of the treeview while a plane is the needed element
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptPlane(int source_row, const QModelIndex & source_parent) const{
    if(!source_parent.isValid()){ //root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL){
            FeatureTreeItem *root = model->getRootItem();
            if(root != NULL){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && item->getChildCount() > 0){
                    FeatureTreeItem *possiblePlane = item->getChild(0);
                    //if child is not a plane
                    if(possiblePlane != NULL && possiblePlane->getFeature() != NULL && possiblePlane->getFeature()->getPlane() == NULL){
                        return false;
                    }
                }
            }
        }
    }else{
        //TODO do not display the planes that were already chosen
    }
    return true;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptSphere
 * Decide wether to accept a row of the treeview while a sphere is the needed element
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptSphere(int source_row, const QModelIndex & source_parent) const{
    if(!source_parent.isValid()){ //root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL){
            FeatureTreeItem *root = model->getRootItem();
            if(root != NULL){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && item->getChildCount() > 0){
                    FeatureTreeItem *possibleSphere = item->getChild(0);
                    //if child is not a sphere
                    if(possibleSphere != NULL && possibleSphere->getFeature() != NULL && possibleSphere->getFeature()->getSphere() == NULL){
                        return false;
                    }
                }
            }
        }
    }else{
        //TODO do not display the spheres that were already chosen
    }
    return true;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptScalarEntityDistance
 * Decide wether to accept a row of the treeview while a scalar entity distance is the needed element
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptScalarEntityDistance(int source_row, const QModelIndex & source_parent) const{
    if(!source_parent.isValid()){ //root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL){
            FeatureTreeItem *root = model->getRootItem();
            if(root != NULL){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && item->getChildCount() > 0){
                    FeatureTreeItem *possibleScalarEntityDistance = item->getChild(0);
                    //if child is not a scalar entity distance
                    if(possibleScalarEntityDistance != NULL && possibleScalarEntityDistance->getFeature() != NULL
                            && possibleScalarEntityDistance->getFeature()->getScalarEntityDistance() == NULL){
                        return false;
                    }
                }
            }
        }
    }else{
        //TODO do not display the scalar entity distances that were already chosen
    }
    return true;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptScalarEntityAngle
 * Decide wether to accept a row of the treeview while a scalar entity angle is the needed element
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptScalarEntityAngle(int source_row, const QModelIndex & source_parent) const{
    if(!source_parent.isValid()){ //root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL){
            FeatureTreeItem *root = model->getRootItem();
            if(root != NULL){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && item->getChildCount() > 0){
                    FeatureTreeItem *possibleScalarEntityAngle = item->getChild(0);
                    //if child is not a scalar entity angle
                    if(possibleScalarEntityAngle != NULL && possibleScalarEntityAngle->getFeature() != NULL
                            && possibleScalarEntityAngle->getFeature()->getScalarEntityAngle() == NULL){
                        return false;
                    }
                }
            }
        }
    }else{
        //TODO do not display the scalar entity angles that were already chosen
    }
    return true;
}

bool AvailableElementsTreeViewProxyModel::filterAcceptScalarEntityTemperature(int source_row, const QModelIndex &source_parent) const
{
    if(!source_parent.isValid()){ //root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL){
            FeatureTreeItem *root = model->getRootItem();
            if(root != NULL){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && item->getChildCount() > 0){
                    FeatureTreeItem *possibleScalarEntityTemperature = item->getChild(0);
                    //if child is not a scalar entity distance
                    if(possibleScalarEntityTemperature != NULL && possibleScalarEntityTemperature->getFeature() != NULL
                            && possibleScalarEntityTemperature->getFeature()->getScalarEntityTemperature() == NULL){
                        return false;
                    }
                }
            }
        }
    }else{
        //TODO do not display the scalar entity distances that were already chosen
    }
    return true;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptStation
 * Decide wether to accept a row of the treeview while a station is the needed element
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptStation(int source_row, const QModelIndex & source_parent) const{
    if(!source_parent.isValid()){ //root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL){
            FeatureTreeItem *root = model->getRootItem();
            if(root != NULL){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && item->getChildCount() > 0){
                    FeatureTreeItem *possibleStation = item->getChild(0);
                    //if child is not a station
                    if(possibleStation != NULL && possibleStation->getFeature() != NULL && possibleStation->getFeature()->getStation() == NULL){
                        return false;
                    }
                }
            }
        }
    }else{
        //TODO do not display the stations that were already chosen
    }
    return true;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptCoordinateSystem
 * Decide wether to accept a row of the treeview while a coordinate system is the needed element
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptCoordinateSystem(int source_row, const QModelIndex & source_parent) const{
    if(!source_parent.isValid()){ //root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL){
            FeatureTreeItem *root = model->getRootItem();
            if(root != NULL){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && item->getChildCount() > 0){
                    FeatureTreeItem *possibleCoordinateSystem = item->getChild(0);
                    //if child is not a coordinate system
                    if(possibleCoordinateSystem != NULL && possibleCoordinateSystem->getFeature() != NULL
                            && possibleCoordinateSystem->getFeature()->getCoordinateSystem() == NULL){
                        return false;
                    }
                }
            }
        }
    }else{
        //TODO do not display the coordinate systems that were already chosen
    }
    return true;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptTrafoParam
 * Decide wether to accept a row of the treeview while a trafo param is the needed element
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptTrafoParam(int source_row, const QModelIndex & source_parent) const{
    if(!source_parent.isValid()){ //root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL){
            FeatureTreeItem *root = model->getRootItem();
            if(root != NULL){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && item->getChildCount() > 0){
                    FeatureTreeItem *possibleTrafoParam = item->getChild(0);
                    //if child is not a trafo param
                    if(possibleTrafoParam != NULL && possibleTrafoParam->getFeature() != NULL && possibleTrafoParam->getFeature()->getTrafoParam() == NULL){
                        return false;
                    }
                }
            }
        }
    }else{
        //TODO do not display the trafo params that were already chosen
    }
    return true;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptObservation
 * Decide wether to accept a row of the treeview while an observation is the needed element
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptObservation(int source_row, const QModelIndex & source_parent) const{
    if(!source_parent.isValid()){ //root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL){
            FeatureTreeItem *root = model->getRootItem();
            if(root != NULL){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && !item->getHasObservation()){
                    return false;
                }
            }
        }
    }else{
        FeatureTreeItem *parent = static_cast<FeatureTreeItem*>(source_parent.internalPointer());
        if(parent != NULL && parent->getChildCount() > source_row){
            FeatureTreeItem *item = parent->getChild(source_row);
            if(item != NULL && item->getIsFeature() && !item->getHasObservation()){ //if item is a feature without observations
                return false;
            }else if(item != NULL && item->getIsHeader() && item->getChildCount() > 0){
                if(item->getChild(0) != NULL && item->getChild(0)->getIsReading()){ //if item is the header "readings:"
                    return false;
                }
            }
        }
    }
    return true;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptReadingDistance
 * Decide wether to accept a row of the treeview while a distance reading is the needed element
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptReadingDistance(int source_row, const QModelIndex & source_parent) const{
    if(!source_parent.isValid()){ //root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL){
            FeatureTreeItem *root = model->getRootItem();
            if(root != NULL){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && !item->getHasReadingDistance()){
                    return false;
                }
            }
        }
    }else{
        FeatureTreeItem *parent = static_cast<FeatureTreeItem*>(source_parent.internalPointer());
        if(parent != NULL && parent->getChildCount() > source_row){
            FeatureTreeItem *item = parent->getChild(source_row);
            if(item != NULL && item->getIsFeature() && !item->getHasReadingDistance()){ //if item is a feature without distance readings
                return false;
            }else if(item != NULL && item->getIsHeader() && item->getChildCount() > 0){
                if(item->getChild(0) != NULL && item->getChild(0)->getIsObservation()){ //if item is the header "observations:"
                    return false;
                }
            }else if(item != NULL && item->getIsReading() && item->getReading() != NULL
                     && item->getReading()->typeofReading != Configuration::eDistance){ //if item is not a distance reading
                return false;
            }
        }
    }
    return true;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptReadingCartesian
 * Decide wether to accept a row of the treeview while a cartesian reading is the needed element
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptReadingCartesian(int source_row, const QModelIndex & source_parent) const{
    if(!source_parent.isValid()){ //root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL){
            FeatureTreeItem *root = model->getRootItem();
            if(root != NULL){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && !item->getHasReadingCartesian()){
                    return false;
                }
            }
        }
    }else{
        FeatureTreeItem *parent = static_cast<FeatureTreeItem*>(source_parent.internalPointer());
        if(parent != NULL && parent->getChildCount() > source_row){
            FeatureTreeItem *item = parent->getChild(source_row);
            if(item != NULL && item->getIsFeature() && !item->getHasReadingCartesian()){ //if item is a feature without cartesian readings
                return false;
            }else if(item != NULL && item->getIsHeader() && item->getChildCount() > 0){
                if(item->getChild(0) != NULL && item->getChild(0)->getIsObservation()){ //if item is the header "observations:"
                    return false;
                }
            }else if(item != NULL && item->getIsReading() && item->getReading() != NULL
                     && item->getReading()->typeofReading != Configuration::eCartesian){ //if item is not a cartesian reading
                return false;
            }
        }
    }
    return true;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptReadingPolar
 * Decide wether to accept a row of the treeview while a polar reading is the needed element
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptReadingPolar(int source_row, const QModelIndex & source_parent) const{
    if(!source_parent.isValid()){ //root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL){
            FeatureTreeItem *root = model->getRootItem();
            if(root != NULL){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && !item->getHasReadingPolar()){
                    return false;
                }
            }
        }
    }else{
        FeatureTreeItem *parent = static_cast<FeatureTreeItem*>(source_parent.internalPointer());
        if(parent != NULL && parent->getChildCount() > source_row){
            FeatureTreeItem *item = parent->getChild(source_row);
            if(item != NULL && item->getIsFeature() && !item->getHasReadingPolar()){ //if item is a feature without polar readings
                return false;
            }else if(item != NULL && item->getIsHeader() && item->getChildCount() > 0){
                if(item->getChild(0) != NULL && item->getChild(0)->getIsObservation()){ //if item is the header "observations:"
                    return false;
                }
            }else if(item != NULL && item->getIsReading() && item->getReading() != NULL
                     && item->getReading()->typeofReading != Configuration::ePolar){ //if item is not a polar reading
                return false;
            }
        }
    }
    return true;
}

/*!
 * \brief AvailableElementsTreeViewProxyModel::filterAcceptReadingDirection
 * Decide wether to accept a row of the treeview while a direction reading is the needed element
 * \param source_row
 * \param source_parent
 * \return
 */
bool AvailableElementsTreeViewProxyModel::filterAcceptReadingDirection(int source_row, const QModelIndex & source_parent) const{
    if(!source_parent.isValid()){ //root item
        FeatureTreeViewModel *model = dynamic_cast<FeatureTreeViewModel*>(this->sourceModel());
        if(model != NULL){
            FeatureTreeItem *root = model->getRootItem();
            if(root != NULL){
                FeatureTreeItem *item = root->getChild(source_row);
                if(item != NULL && !item->getHasReadingDirection()){
                    return false;
                }
            }
        }
    }else{
        FeatureTreeItem *parent = static_cast<FeatureTreeItem*>(source_parent.internalPointer());
        if(parent != NULL && parent->getChildCount() > source_row){
            FeatureTreeItem *item = parent->getChild(source_row);
            if(item != NULL && item->getIsFeature() && !item->getHasReadingDirection()){ //if item is a feature without direction readings
                return false;
            }else if(item != NULL && item->getIsHeader() && item->getChildCount() > 0){
                if(item->getChild(0) != NULL && item->getChild(0)->getIsObservation()){ //if item is the header "observations:"
                    return false;
                }
            }else if(item != NULL && item->getIsReading() && item->getReading() != NULL
                     && item->getReading()->typeofReading != Configuration::eDirection){ //if item is not a direction reading
                return false;
            }
        }
    }
    return true;
}
