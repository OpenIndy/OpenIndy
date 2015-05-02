#include "featuretreeviewmodel.h"

/*!
 * \brief FeatureTreeViewModel::FeatureTreeViewModel
 * \param job
 * \param parent
 */
FeatureTreeViewModel::FeatureTreeViewModel(QPointer<OiJob> job, QObject *parent) : QAbstractItemModel(parent){

    this->rootItem = new FeatureTreeItem("features");
    this->setCurrentJob(job);

}

/*!
 * \brief FeatureTreeViewModel::FeatureTreeViewModel
 * \param parent
 */
FeatureTreeViewModel::FeatureTreeViewModel(QObject *parent){

    this->rootItem = new FeatureTreeItem("features");

}

/*!
 * \brief FeatureTreeViewModel::rowCount
 * \param parent
 * \return
 */
int FeatureTreeViewModel::rowCount(const QModelIndex &parent) const{

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
 * \brief FeatureTreeViewModel::columnCount
 * \param parent
 * \return
 */
int FeatureTreeViewModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

/*!
 * \brief FeatureTreeViewModel::data
 * \param index
 * \param role
 * \return
 */
QVariant FeatureTreeViewModel::data(const QModelIndex &index, int role) const{

    if (!index.isValid())
        return QVariant();

    FeatureTreeItem *item = static_cast<FeatureTreeItem*>(index.internalPointer());

    if(item != NULL){
        if(role == Qt::DisplayRole){ //return display text for tree view item
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
            }else if(item->getIsFeature() && item->getFeature()->getScalarEntityTemperature() != NULL){
                QPixmap pix(":/Images/icons/scalarEntities_5e8acf.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }
        }
    }

    return QVariant();

}

/*!
 * \brief FeatureTreeViewModel::index
 * Create model index for every node
 * \param row
 * \param column
 * \param parent
 * \return
 */
QModelIndex FeatureTreeViewModel::index(int row, int column, const QModelIndex &parent) const{

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
 * \brief FeatureTreeViewModel::parent
 * \param index
 * \return
 */
QModelIndex FeatureTreeViewModel::parent(const QModelIndex &index) const{

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
 * \brief FeatureTreeViewModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &FeatureTreeViewModel::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief FeatureTreeViewModel::setCurrentJob
 * \param job
 */
void FeatureTreeViewModel::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){

        //disconnect current job
        if(!this->currentJob.isNull()){
            this->disconnectJob();
        }

        this->currentJob = job;
        this->connectJob();
        this->updateModel();

    }
}

/*!
 * \brief FeatureTreeViewModel::getRootItem
 * \return
 */
const QPointer<FeatureTreeItem> &FeatureTreeViewModel::getRootItem() const{
    return this->rootItem;
}

/*!
 * \brief FeatureTreeViewModel::updateModel
 */
void FeatureTreeViewModel::updateModel(){

    //check if a job is set and the root (header) item exists
    if(this->currentJob.isNull() || this->rootItem.isNull()){
        return;
    }

    //start resetting the model
    emit this->beginResetModel();

    //delete the old tree hierarchy
    this->rootItem->deleteChildren();

    //add all stations
    if(this->currentJob->getStationsList().size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eStationElement)));
        foreach(const QPointer<Station> &station, this->currentJob->getStationsList()){
            if(!station.isNull() && !station->getFeatureWrapper().isNull()){
                FeatureTreeItem *itemStation = new FeatureTreeItem(station->getFeatureWrapper());
                item->appendChild(itemStation);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all nominal systems
    if(this->currentJob->getCoordinateSystemsList().size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eCoordinateSystemElement)));
        foreach(const QPointer<CoordinateSystem> &system, this->currentJob->getCoordinateSystemsList()){
            if(!system.isNull() && !system->getFeatureWrapper().isNull()){
                FeatureTreeItem *itemSystem = new FeatureTreeItem(system->getFeatureWrapper());
                item->appendChild(itemSystem);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all trafo params
    if(this->currentJob->getTransformationParametersList().size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eTrafoParamElement)));
        foreach(const QPointer<TrafoParam> &trafoParam, this->currentJob->getTransformationParametersList()){
            if(!trafoParam.isNull() && !trafoParam->getFeatureWrapper().isNull()){
                FeatureTreeItem *itemTrafoParam = new FeatureTreeItem(trafoParam->getFeatureWrapper());
                item->appendChild(itemTrafoParam);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all geometries
    this->addGeometries();

    //reset finished
    emit this->endResetModel();

    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();

}

/*!
 * \brief FeatureTreeViewModel::connectJob
 */
void FeatureTreeViewModel::connectJob(){

    QObject::connect(this->currentJob.data(), &OiJob::featureSetChanged, this, &FeatureTreeViewModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::featureNameChanged, this, &FeatureTreeViewModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::geometryObservationsChanged, this, &FeatureTreeViewModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::geometryNominalSystemChanged, this, &FeatureTreeViewModel::updateModel, Qt::AutoConnection);

}

/*!
 * \brief FeatureTreeViewModel::disconnectJob
 */
void FeatureTreeViewModel::disconnectJob(){

    QObject::disconnect(this->currentJob.data(), &OiJob::featureSetChanged, this, &FeatureTreeViewModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::featureNameChanged, this, &FeatureTreeViewModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::geometryObservationsChanged, this, &FeatureTreeViewModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::geometryNominalSystemChanged, this, &FeatureTreeViewModel::updateModel);

}

/*!
 * \brief FeatureTreeViewModel::addGeometries
 */
void FeatureTreeViewModel::addGeometries(){

    //add all circles
    if(this->currentJob->getFeaturesByType(eCircleFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eCircleElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eCircleFeature)){
            if(!feature.isNull() && !feature->getCircle().isNull()){
                FeatureTreeItem *itemCircle = new FeatureTreeItem(feature);
                item->appendChild(itemCircle);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all cones
    if(this->currentJob->getFeaturesByType(eConeFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eConeElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eConeFeature)){
            if(!feature.isNull() && !feature->getCone().isNull()){
                FeatureTreeItem *itemCone = new FeatureTreeItem(feature);
                item->appendChild(itemCone);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all cylinders
    if(this->currentJob->getFeaturesByType(eCylinderFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eCylinderElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eCylinderFeature)){
            if(!feature.isNull() && !feature->getCylinder().isNull()){
                FeatureTreeItem *itemCylinder = new FeatureTreeItem(feature);
                item->appendChild(itemCylinder);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all ellipses
    if(this->currentJob->getFeaturesByType(eEllipseFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eEllipseElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eEllipseFeature)){
            if(!feature.isNull() && !feature->getEllipse().isNull()){
                FeatureTreeItem *itemEllipse = new FeatureTreeItem(feature);
                item->appendChild(itemEllipse);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all ellipsoids
    if(this->currentJob->getFeaturesByType(eEllipsoidFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eEllipsoidElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eEllipsoidFeature)){
            if(!feature.isNull() && !feature->getEllipsoid().isNull()){
                FeatureTreeItem *itemEllipsoid = new FeatureTreeItem(feature);
                item->appendChild(itemEllipsoid);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all hyperboloids
    if(this->currentJob->getFeaturesByType(eHyperboloidFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eHyperboloidElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eHyperboloidFeature)){
            if(!feature.isNull() && !feature->getHyperboloid().isNull()){
                FeatureTreeItem *itemHyperboloid = new FeatureTreeItem(feature);
                item->appendChild(itemHyperboloid);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all lines
    if(this->currentJob->getFeaturesByType(eLineFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eLineElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eLineFeature)){
            if(!feature.isNull() && !feature->getLine().isNull()){
                FeatureTreeItem *itemLine = new FeatureTreeItem(feature);
                item->appendChild(itemLine);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all nurbs
    if(this->currentJob->getFeaturesByType(eNurbsFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eNurbsElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eNurbsFeature)){
            if(!feature.isNull() && !feature->getNurbs().isNull()){
                FeatureTreeItem *itemNurbs = new FeatureTreeItem(feature);
                item->appendChild(itemNurbs);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all paraboloids
    if(this->currentJob->getFeaturesByType(eParaboloidFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eParaboloidElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eParaboloidFeature)){
            if(!feature.isNull() && !feature->getParaboloid().isNull()){
                FeatureTreeItem *itemParaboloid = new FeatureTreeItem(feature);
                item->appendChild(itemParaboloid);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all planes
    if(this->currentJob->getFeaturesByType(ePlaneFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(ePlaneElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(ePlaneFeature)){
            if(!feature.isNull() && !feature->getPlane().isNull()){
                FeatureTreeItem *itemPlane = new FeatureTreeItem(feature);
                item->appendChild(itemPlane);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all points
    if(this->currentJob->getFeaturesByType(ePointFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(ePointElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(ePointFeature)){
            if(!feature.isNull() && !feature->getPoint().isNull()){
                FeatureTreeItem *itemPoint = new FeatureTreeItem(feature);
                item->appendChild(itemPoint);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all point clouds
    if(this->currentJob->getFeaturesByType(ePointCloudFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(ePointCloudElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(ePointCloudFeature)){
            if(!feature.isNull() && !feature->getPointCloud().isNull()){
                FeatureTreeItem *itemCloud = new FeatureTreeItem(feature);
                item->appendChild(itemCloud);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all angles
    if(this->currentJob->getFeaturesByType(eScalarEntityAngleFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eScalarEntityAngleElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eScalarEntityAngleFeature)){
            if(!feature.isNull() && !feature->getScalarEntityAngle().isNull()){
                FeatureTreeItem *itemAngle = new FeatureTreeItem(feature);
                item->appendChild(itemAngle);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all distances
    if(this->currentJob->getFeaturesByType(eScalarEntityDistanceFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eScalarEntityDistanceElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eScalarEntityDistanceFeature)){
            if(!feature.isNull() && !feature->getScalarEntityDistance().isNull()){
                FeatureTreeItem *itemDistance = new FeatureTreeItem(feature);
                item->appendChild(itemDistance);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all measurement series
    if(this->currentJob->getFeaturesByType(eScalarEntityMeasurementSeriesFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eScalarEntityMeasurementSeriesElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eScalarEntityMeasurementSeriesFeature)){
            if(!feature.isNull() && !feature->getScalarEntityMeasurementSeries().isNull()){
                FeatureTreeItem *itemSeries = new FeatureTreeItem(feature);
                item->appendChild(itemSeries);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all temperatures
    if(this->currentJob->getFeaturesByType(eScalarEntityTemperatureFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eScalarEntityTemperatureElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eScalarEntityTemperatureFeature)){
            if(!feature.isNull() && !feature->getScalarEntityTemperature().isNull()){
                FeatureTreeItem *itemTemperature = new FeatureTreeItem(feature);
                item->appendChild(itemTemperature);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all slotted holes
    if(this->currentJob->getFeaturesByType(eSlottedHoleFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eSlottedHoleElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eSlottedHoleFeature)){
            if(!feature.isNull() && !feature->getSlottedHole().isNull()){
                FeatureTreeItem *itemSlottedHole = new FeatureTreeItem(feature);
                item->appendChild(itemSlottedHole);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all spheres
    if(this->currentJob->getFeaturesByType(eSphereFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eSphereElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eSphereFeature)){
            if(!feature.isNull() && !feature->getSphere().isNull()){
                FeatureTreeItem *itemSphere = new FeatureTreeItem(feature);
                item->appendChild(itemSphere);
            }
        }
        this->rootItem->appendChild(item);
    }

    //add all tori
    if(this->currentJob->getFeaturesByType(eTorusFeature).size() > 0){
        FeatureTreeItem *item = new FeatureTreeItem(QString("%1:").arg(getElementTypePluralName(eTorusElement)));
        foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesByType(eTorusFeature)){
            if(!feature.isNull() && !feature->getTorus().isNull()){
                FeatureTreeItem *itemTorus = new FeatureTreeItem(feature);
                item->appendChild(itemTorus);
            }
        }
        this->rootItem->appendChild(item);
    }

}
