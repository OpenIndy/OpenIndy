#include "featuretreeviewmodel.h"

FeatureTreeViewModel::FeatureTreeViewModel(QPointer<OiJob> job, QObject *parent) : QAbstractItemModel(parent){

    this->rootItem = new FeatureTreeItem("features");
    this->currentJob = job;

}

FeatureTreeViewModel::FeatureTreeViewModel(QObject *parent){

    this->rootItem = new FeatureTreeItem("features");

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

            if (parentItem == NULL || parentItem == rootItem) //|| parentItem == rootItem
                return QModelIndex();

            return createIndex(parentItem->getIndex(), 0, parentItem);
        }else{
            return QModelIndex();
        }

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
 * \brief FeatureTreeViewModel::getRootItem
 * \return
 */
FeatureTreeItem* FeatureTreeViewModel::getRootItem() const{
    return this->rootItem;
}

/*!
 * \brief FeatureTreeViewModel::refreshModel
 * Set up the tree view hierarchy
 */
void FeatureTreeViewModel::refreshModel(){

    //check if a job is set and the root (header) item exists
    if(this->currentJob.isNull() || this->rootItem.isNull()){
        return;
    }

    //delete the old tree hierarchy
    this->rootItem->deleteChildren();

    //add all stations, coordinate systems and trafo params to the tree hierarchy
    QList<FeatureWrapper *> stations = this->currentJob->getFeaturesByFeatureType(eStationFeature);
    this->appendStations(stations);
    QList<FeatureWrapper *> coordinateSystems = this->currentJob->getFeaturesByFeatureType(eStationFeature);
    this->appendStations(coordinateSystems);
    QList<FeatureWrapper *> trafoParams = this->currentJob->getFeaturesByFeatureType(eStationFeature);
    this->appendStations(trafoParams);

    //add all geometries to the tree hierarchy
    QList<FeatureWrapper *> points = this->currentJob->getFeaturesByFeatureType(ePointFeature);
    this->appendGeometries(points, ePointFeature);
    QList<FeatureWrapper *> lines = this->currentJob->getFeaturesByFeatureType(eLineFeature);
    this->appendGeometries(lines, ePointFeature);
    QList<FeatureWrapper *> planes = this->currentJob->getFeaturesByFeatureType(ePlaneFeature);
    this->appendGeometries(planes, ePointFeature);
    QList<FeatureWrapper *> spheres = this->currentJob->getFeaturesByFeatureType(eSphereFeature);
    this->appendGeometries(spheres, ePointFeature);
    QList<FeatureWrapper *> circles = this->currentJob->getFeaturesByFeatureType(eCircleFeature);
    this->appendGeometries(circles, ePointFeature);
    QList<FeatureWrapper *> ellipsoids = this->currentJob->getFeaturesByFeatureType(eEllipsoidFeature);
    this->appendGeometries(ellipsoids, ePointFeature);
    QList<FeatureWrapper *> paraboloids = this->currentJob->getFeaturesByFeatureType(eParaboloidFeature);
    this->appendGeometries(paraboloids, ePointFeature);
    QList<FeatureWrapper *> hyperboloids = this->currentJob->getFeaturesByFeatureType(eHyperboloidFeature);
    this->appendGeometries(hyperboloids, ePointFeature);
    QList<FeatureWrapper *> angles = this->currentJob->getFeaturesByFeatureType(eScalarEntityAngleFeature);
    this->appendGeometries(angles, ePointFeature);
    QList<FeatureWrapper *> distances = this->currentJob->getFeaturesByFeatureType(eScalarEntityDistanceFeature);
    this->appendGeometries(distances, ePointFeature);
    QList<FeatureWrapper *> temperatures = this->currentJob->getFeaturesByFeatureType(eScalarEntityTemperatureFeature);
    this->appendGeometries(temperatures, ePointFeature);
    QList<FeatureWrapper *> pointclouds = this->currentJob->getFeaturesByFeatureType(ePointCloudFeature);
    this->appendGeometries(pointclouds, ePointFeature);
    QList<FeatureWrapper *> nurbs = this->currentJob->getFeaturesByFeatureType(eNurbsFeature);
    this->appendGeometries(nurbs, ePointFeature);
    QList<FeatureWrapper *> cones = this->currentJob->getFeaturesByFeatureType(eConeFeature);
    this->appendGeometries(cones, ePointFeature);
    QList<FeatureWrapper *> cylinders = this->currentJob->getFeaturesByFeatureType(eCylinderFeature);
    this->appendGeometries(cylinders, ePointFeature);

    emit this->beginResetModel();
    emit this->endResetModel();

    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

/*!
 * \brief FeatureTreeViewModel::appendGeometries
 * \param geometries
 * \param type
 */
void FeatureTreeViewModel::appendGeometries(const QList<FeatureWrapper*> &geometries, const FeatureTypes &type){

    //check if root item is valid
    if(this->rootItem.isNull()){
        return;
    }

    //create header item
    QPointer<FeatureTreeItem> item = new FeatureTreeItem(getFeatureTypeName(type));
    this->rootItem->appendChild(item);

    //add all geometries
    foreach(FeatureWrapper *geom, geometries){

        //check the geometry
        if(geom == NULL || geom->getGeometry() == NULL || (geom->getGeometry()->getIsNominal() && geom->getGeometry()->getNominalSystem() == NULL)){
            continue;
        }

        //create item for the geometry
        QString geomHeader = QString("%1 - %2").arg(geom->getGeometry()->getFeatureName())
                .arg(geom->getGeometry()->getIsNominal()?QString("nominal (%1)")
                                                                   .arg(geometries.at(i)->getGeometry()->getNominalSystem()->getFeatureName()):"actual");
        QPointer<FeatureTreeItem> geomItem = new FeatureTreeItem(geomHeader);
        item->appendChild(geomItem);

        //add observations and readings
        QPointer<FeatureTreeItem> observations = new FeatureTreeItem("observations");
        QPointer<FeatureTreeItem> readings = new FeatureTreeItem("readings");
        geomItem->appendChild(observations);
        geomItem->appendChild(readings);
        for(int j = 0; j < geom->getGeometry()->getObservations().size(); j++){
            Observation *obs = geom->getGeometry()->getObservations().at(j);
            if(obs != NULL && obs->myReading != NULL){
                if(obs->myStation != NULL){
                    this->appendObservation(observations, obs);
                    this->appendReading(readings, obs);
                }
            }
        }

        //add the feature to the tree item
        geomItem->setFeature(geom);

    }

}

/*!
 * \brief FeatureTreeViewModel::appendCoordinateSystems
 * \param coordinateSystems
 */
void FeatureTreeViewModel::appendCoordinateSystems(const QList<FeatureWrapper *> &coordinateSystems){

    //check if root item is valid
    if(this->rootItem.isNull()){
        return;
    }

    //create header item
    QPointer<FeatureTreeItem> item = new FeatureTreeItem(eCoordinateSystemFeature);
    this->rootItem->appendChild(item);

    //add all coordinate systems
    foreach(FeatureWrapper *system, coordinateSystems){

        //check the coordinate system
        if(system == NULL || system->getCoordinateSystem() == NULL){
            continue;
        }

        //create item for the coordinate system
        QPointer<FeatureTreeItem> systemItem = new FeatureTreeItem(system->getCoordinateSystem()->getFeatureName());
        item->appendChild(systemItem);

        //add the feature to the tree item
        systemItem->setFeature(system);

    }

}

/*!
 * \brief FeatureTreeViewModel::appendStations
 * \param stations
 */
void FeatureTreeViewModel::appendStations(const QList<FeatureWrapper *> &stations){

    //check if root item is valid
    if(this->rootItem.isNull()){
        return;
    }

    //create header item
    QPointer<FeatureTreeItem> item = new FeatureTreeItem(eStationFeature);
    this->rootItem->appendChild(item);

    //add all stations
    foreach(FeatureWrapper *station, stations){

        //check the station
        if(station == NULL || station->getStation() == NULL){
            continue;
        }

        //create item for the coordinate system
        QPointer<FeatureTreeItem> stationItem = new FeatureTreeItem(station->getStation()->getFeatureName());
        item->appendChild(stationItem);

        //check wether there are observations to the current station
        QPointer<FeatureTreeItem> station_observations = new FeatureTreeItem("observations:");
        QPointer<FeatureTreeItem> station_readings = new FeatureTreeItem("readings:");
        station->appendChild(station_observations);
        station->appendChild(station_readings);
        if(station->getStation()->position != NULL){
            for(int j = 0; j < station->getStation()->position->getObservations().size(); j++){
                Observation *obs = station->getStation()->position->getObservations().at(j);
                if(obs != NULL && obs->myReading != NULL){
                    if(obs->myStation != NULL){
                        this->appendObservation(station_observations, obs);
                        this->appendReading(station_readings, obs);
                    }
                }
            }
        }

        //add the feature to the tree item
        systemItem->setFeature(station);

    }

}

/*!
 * \brief FeatureTreeViewModel::appendTrafoParams
 * \param trafoParams
 */
void FeatureTreeViewModel::appendTrafoParams(const QList<FeatureWrapper *> &trafoParams){

    //check if root item is valid
    if(this->rootItem.isNull()){
        return;
    }

    //create header item
    QPointer<FeatureTreeItem> item = new FeatureTreeItem(eTrafoParamFeature);
    this->rootItem->appendChild(item);

    //add all trafo params
    foreach(FeatureWrapper *trafoParam, trafoParams){

        //check the trafo param
        if(trafoParam == NULL || trafoParam->getTrafoParam() == NULL){
            continue;
        }

        //create item for the trafo param
        QPointer<FeatureTreeItem> trafoParamItem = new FeatureTreeItem(trafoParam->getTrafoParam()->getFeatureName());
        item->appendChild(trafoParamItem);

        //add start and destination system
        if(trafoParam->getTrafoParam()->getStartSystem() != NULL && trafoParam->getTrafoParam()->getDestinationSystem() != NULL){
            QPointer<FeatureTreeItem> from = new FeatureTreeItem( QString("from: %1").arg(trafoParam->getTrafoParam()->getStartSystem()->getFeatureName()) );
            QPointer<FeatureTreeItem> to = new FeatureTreeItem( QString("to: %1").arg(trafoParam->getTrafoParam()->getDestinationSystem()->getFeatureName()) );
            trafoParam->appendChild(from);
            trafoParam->appendChild(to);
        }

        //add the feature to the tree item
        trafoParamItem->setFeature(trafoParam);

    }

}

/*!
 * \brief FeatureTreeViewModel::appendObservation
 * \param parent
 * \param obs
 */
void FeatureTreeViewModel::appendObservation(const QPointer<FeatureTreeItem> &parent, Observation *obs){

    //check parent and observation
    if(parent.isNull() || obs == NULL || obs->myReading == NULL || obs->myStation == NULL){
        return;
    }

    //create observation item
    QPointer<FeatureTreeItem> observation = new FeatureTreeItem(QString::number(obs->getId()));
    parent->appendChild(observation);

    //create meta information items
    QPointer<FeatureTreeItem> measuredAt = new FeatureTreeItem(obs->myReading->measuredAt.toString());
    observation->appendChild(measuredAt);
    QPointer<FeatureTreeItem> station = new FeatureTreeItem(obs->myStation->getFeatureName());
    observation->appendChild(measuredAt);
    QPointer<FeatureTreeItem> isSolved = new FeatureTreeItem(obs->getIsSolved()?"valid: true":"valid: false");
    observation->appendChild(measuredAt);
    observation->appendChild(measuredAt);
    observation->appendChild(station);
    observation->appendChild(isSolved);

    //add the observation to the parent item
    observation->setObservation(obs);

}

/*!
 * \brief FeatureTreeViewModel::appendReading
 * \param parent
 * \param obs
 */
void FeatureTreeViewModel::appendReading(const QPointer<FeatureTreeItem> &parent, Observation *obs){

    //check parent and observation
    if(parent.isNull() || obs == NULL || obs->myReading == NULL || obs->myStation == NULL){
        return;
    }

    //create observation item
    QPointer<FeatureTreeItem> reading = new FeatureTreeItem(QString::number(obs->myReading->id));
    parent->appendChild(reading);

    //create meta information items
    QPointer<FeatureTreeItem> measuredAt = new FeatureTreeItem(obs->myReading->measuredAt.toString());
    observation->appendChild(measuredAt);
    QPointer<FeatureTreeItem> station = new FeatureTreeItem(obs->myStation->getFeatureName());
    observation->appendChild(measuredAt);
    QPointer<FeatureTreeItem> isSolved = new FeatureTreeItem(obs->getIsSolved()?"valid: true":"valid: false");
    observation->appendChild(measuredAt);
    observation->appendChild(measuredAt);
    observation->appendChild(station);
    observation->appendChild(isSolved);

    //add the reading to the parent item
    reading->setReading(obs->myReading);

}
