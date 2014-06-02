#include "featuretreeviewmodel.h"

FeatureTreeViewModel::FeatureTreeViewModel(QObject *parent) : QAbstractItemModel(parent){
    this->rootItem = new FeatureTreeItem("available elements");
    this->refreshModel();
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
    this->rootItem->deleteChildren();

    QList<FeatureWrapper*> points, lines, planes, spheres, stations, coordinateSystems, trafoParams, angles, distances,
            circles, cones, cylinders, ellipsoids, hyperboloids, nurbs, paraboloids, pointClouds, temperatures;
    foreach(FeatureWrapper *wrapper, OiFeatureState::getFeatures()){
        if(wrapper->getGeometry() != NULL){ //if current feature wrapper contains a geometry
            if(wrapper->getPoint() != NULL){
                points.append(wrapper);
            }else if(wrapper->getLine() != NULL){
                lines.append(wrapper);
            }else if(wrapper->getPlane() != NULL){
                planes.append(wrapper);
            }else if(wrapper->getSphere() != NULL){
                spheres.append(wrapper);
            }else if(wrapper->getScalarEntityAngle() != NULL){
                angles.append(wrapper);
            }else if(wrapper->getScalarEntityDistance() != NULL){
                distances.append(wrapper);
            }else if(wrapper->getCircle() != NULL){
                circles.append(wrapper);
            }else if(wrapper->getCone() != NULL){
                cones.append(wrapper);
            }else if(wrapper->getCylinder() != NULL){
                cylinders.append(wrapper);
            }else if(wrapper->getEllipsoid() != NULL){
                ellipsoids.append(wrapper);
            }else if(wrapper->getHyperboloid() != NULL){
                hyperboloids.append(wrapper);
            }else if(wrapper->getNurbs() != NULL){
                nurbs.append(wrapper);
            }else if(wrapper->getParaboloid() != NULL){
                paraboloids.append(wrapper);
            }else if(wrapper->getPointCloud() != NULL){
                pointClouds.append(wrapper);
            }else if(wrapper->getScalarEntityTemperature() != NULL){
                temperatures.append(wrapper);
            }
        }else if(wrapper->getFeature() != NULL){ //if current feature wrapper contains a feature
            if(wrapper->getCoordinateSystem() != NULL){
                coordinateSystems.append(wrapper);
            }else if(wrapper->getStation() != NULL){
                stations.append(wrapper);
            }else if(wrapper->getTrafoParam() != NULL){
                trafoParams.append(wrapper);
            }
        }
    }

    if(points.size() > 0){ this->appendGeometries(this->rootItem, points, "points:"); }
    if(lines.size() > 0){ this->appendGeometries(this->rootItem, lines, "lines:"); }
    if(spheres.size() > 0){ this->appendGeometries(this->rootItem, spheres, "spheres:"); }
    if(planes.size() > 0){ this->appendGeometries(this->rootItem, planes, "planes:"); }
    if(angles.size() > 0){ this->appendGeometries(this->rootItem, angles, "angles:"); }
    if(distances.size() > 0){ this->appendGeometries(this->rootItem, distances, "distances:"); }
    if(temperatures.size() > 0){ this->appendGeometries(this->rootItem, temperatures, "temperatures:"); }
    if(circles.size() > 0){ this->appendGeometries(this->rootItem, circles, "circles:"); }
    if(cylinders.size() > 0){ this->appendGeometries(this->rootItem, cylinders, "cylinders:"); }
    if(cones.size() > 0){ this->appendGeometries(this->rootItem, cones, "cones:"); }
    if(ellipsoids.size() > 0){ this->appendGeometries(this->rootItem, ellipsoids, "ellipsoids:"); }
    if(hyperboloids.size() > 0){ this->appendGeometries(this->rootItem, hyperboloids, "hyperboloids:"); }
    if(nurbs.size() > 0){ this->appendGeometries(this->rootItem, nurbs, "nurbs:"); }
    if(paraboloids.size() > 0){ this->appendGeometries(this->rootItem, paraboloids, "paraboloids:"); }
    if(pointClouds.size() > 0){ this->appendGeometries(this->rootItem, pointClouds, "point clouds:"); }
    if(coordinateSystems.size() > 0){ this->appendCoordinateSystems(this->rootItem, coordinateSystems); }
    if(stations.size() > 0){ this->appendStations(this->rootItem, stations); }
    if(trafoParams.size() > 0){ this->appendTrafoParams(this->rootItem, trafoParams); }

    emit this->beginResetModel();
    emit this->endResetModel();

    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

/*!
 * \brief FeatureTreeViewModel::appendGeometries
 * \param root
 * \param geometries
 * \param geomType
 */
void FeatureTreeViewModel::appendGeometries(FeatureTreeItem *root, QList<FeatureWrapper*> geometries, QString geomType){
    FeatureTreeItem *item = new FeatureTreeItem(geomType);
    root->appendChild(item);
    for(int i = 0; i < geometries.size(); i++){
        FeatureTreeItem *geom = new FeatureTreeItem(geometries.at(i)->getGeometry()->getFeatureName());
        item->appendChild(geom);
        FeatureTreeItem *geom_observations = new FeatureTreeItem("observations:");
        FeatureTreeItem *geom_readings = new FeatureTreeItem("readings:");
        geom->appendChild(geom_observations);
        geom->appendChild(geom_readings);
        for(int j = 0; j < geometries.at(i)->getGeometry()->myObservations.size(); j++){
            Observation *obs = geometries.at(i)->getGeometry()->myObservations.at(j);
            if(obs != NULL && obs->myReading != NULL){
                if(obs->myStation != NULL){
                    this->appendObservation(geom_observations, obs);
                    this->appendReading(geom_readings, obs);
                }
            }
        }
        geom->setFeature(geometries.at(i));
    }
}

/*!
 * \brief FeatureTreeViewModel::appendCoordinateSystem
 * \param root
 * \param coordinateSystems
 */
void FeatureTreeViewModel::appendCoordinateSystems(FeatureTreeItem *root, QList<FeatureWrapper*> coordinateSystems){
    FeatureTreeItem *item = new FeatureTreeItem("coordinate systems:");
    root->appendChild(item);
    for(int i = 0; i < coordinateSystems.size(); i++){
        FeatureTreeItem *coordSys = new FeatureTreeItem(coordinateSystems.at(i)->getCoordinateSystem()->getFeatureName());
        item->appendChild(coordSys);
        coordSys->setFeature(coordinateSystems.at(i));
    }
}

/*!
 * \brief FeatureTreeViewModel::appendStations
 * \param root
 * \param stations
 */
void FeatureTreeViewModel::appendStations(FeatureTreeItem *root, QList<FeatureWrapper *> stations){
    FeatureTreeItem *item = new FeatureTreeItem("stations:");
    root->appendChild(item);
    for(int i = 0; i < stations.size(); i++){ //go through all stations
        FeatureTreeItem *station = new FeatureTreeItem(stations.at(i)->getStation()->getFeatureName());
        item->appendChild(station);

        //check wether there are observations to the current station
        FeatureTreeItem *station_observations = new FeatureTreeItem("observations:");
        FeatureTreeItem *station_readings = new FeatureTreeItem("readings:");
        station->appendChild(station_observations);
        station->appendChild(station_readings);
        if(stations.at(i)->getStation()->position != NULL){
            for(int j = 0; j < stations.at(i)->getStation()->position->myObservations.size(); j++){
                Observation *obs = stations.at(i)->getStation()->position->myObservations.at(j);
                if(obs != NULL && obs->myReading != NULL){
                    if(obs->myStation != NULL){
                        this->appendObservation(station_observations, obs);
                        this->appendReading(station_readings, obs);
                    }
                }
            }
        }

        station->setFeature(stations.at(i));
    }

}

/*!
 * \brief FeatureTreeViewModel::appendTrafoParam
 * \param root
 * \param trafoparams
 */
void FeatureTreeViewModel::appendTrafoParams(FeatureTreeItem *root, QList<FeatureWrapper*> trafoParams){
    FeatureTreeItem *item = new FeatureTreeItem("transformation parameter sets:");
    root->appendChild(item);
    for(int i = 0; i < trafoParams.size(); i++){
        FeatureTreeItem *trafoParam = new FeatureTreeItem(trafoParams.at(i)->getTrafoParam()->getFeatureName());
        item->appendChild(trafoParam);
        if(trafoParams.at(i)->getTrafoParam()->getStartSystem() != NULL && trafoParams.at(i)->getTrafoParam()->getDestinationSystem() != NULL){
            FeatureTreeItem *from = new FeatureTreeItem( QString("from: %1").arg(trafoParams.at(i)->getTrafoParam()->getStartSystem()->getFeatureName()) );
            FeatureTreeItem *to = new FeatureTreeItem( QString("to: %1").arg(trafoParams.at(i)->getTrafoParam()->getDestinationSystem()->getFeatureName()) );
            trafoParam->appendChild(from);
            trafoParam->appendChild(to);
        }
        trafoParam->setFeature(trafoParams.at(i));
    }

}

/*!
 * \brief FeatureTreeViewModel::appendObservation
 * \param root
 * \param obs
 */
void FeatureTreeViewModel::appendObservation(FeatureTreeItem *root, Observation *obs){
    FeatureTreeItem *geom_observation = new FeatureTreeItem(obs->myReading->measuredAt.toString());
    root->appendChild(geom_observation);
    geom_observation->appendChild( new FeatureTreeItem(obs->myStation->getFeatureName()) );
    geom_observation->appendChild( new FeatureTreeItem(obs->isValid?"valid: true":"valid: false") );
    if(obs->isValid){
        FeatureTreeItem *x = new FeatureTreeItem( QString("x: %1 %2").arg( QString::number(obs->myXyz.getAt(0)*UnitConverter::getDistanceMultiplier(),
                                                                 'f',UnitConverter::distanceDigits) )
                                                    .arg( UnitConverter::getDistanceUnitString() ) );
        FeatureTreeItem *y = new FeatureTreeItem( QString("y: %1 %2").arg( QString::number(obs->myXyz.getAt(1)*UnitConverter::getDistanceMultiplier(),
                                                                 'f',UnitConverter::distanceDigits) )
                                                    .arg( UnitConverter::getDistanceUnitString() ) );
        FeatureTreeItem *z = new FeatureTreeItem( QString("z: %1 %2").arg( QString::number(obs->myXyz.getAt(2)*UnitConverter::getDistanceMultiplier(),
                                                                 'f',UnitConverter::distanceDigits) )
                                                    .arg( UnitConverter::getDistanceUnitString() ) );
        geom_observation->appendChild(x);
        geom_observation->appendChild(y);
        geom_observation->appendChild(z);
    }
    geom_observation->setObservation(obs);
}

/*!
 * \brief FeatureTreeViewModel::appendReading
 * \param root
 * \param obs
 */
void FeatureTreeViewModel::appendReading(FeatureTreeItem *root, Observation *obs){
    FeatureTreeItem *geom_reading = new FeatureTreeItem(obs->myReading->measuredAt.toString());
    root->appendChild(geom_reading);
    geom_reading->appendChild( new FeatureTreeItem(obs->myStation->getFeatureName()) );
    geom_reading->appendChild( new FeatureTreeItem(obs->isValid?"valid: true":"valid: false") );
    if(obs->isValid){
        if(obs->myReading->typeofReading == Configuration::eDistance){
            FeatureTreeItem *distance = new FeatureTreeItem( QString("distance: %1 %2")
                                                         .arg( QString::number(obs->myReading->rDistance.distance*UnitConverter::getDistanceMultiplier(),
                                                                     'f',UnitConverter::distanceDigits) )
                                                         .arg( UnitConverter::getDistanceUnitString() ) );
            geom_reading->appendChild(distance);
        }else if(obs->myReading->typeofReading == Configuration::eDirection){
            FeatureTreeItem *azimuth = new FeatureTreeItem( QString("azimuth: %1 %2")
                                                         .arg( QString::number(obs->myReading->rDirection.azimuth*UnitConverter::getAngleMultiplier(),
                                                                     'f',UnitConverter::angleDigits) )
                                                         .arg( UnitConverter::getAngleUnitString() ) );
            FeatureTreeItem *zenith = new FeatureTreeItem( QString("zenith: %1 %2")
                                                         .arg( QString::number(obs->myReading->rDirection.zenith*UnitConverter::getAngleMultiplier(),
                                                                     'f',UnitConverter::angleDigits) )
                                                         .arg( UnitConverter::getAngleUnitString() ) );
            geom_reading->appendChild(azimuth);
            geom_reading->appendChild(zenith);
        }else if(obs->myReading->typeofReading == Configuration::ePolar){
            FeatureTreeItem *azimuth = new FeatureTreeItem( QString("azimuth: %1 %2")
                                                         .arg( QString::number(obs->myReading->rPolar.azimuth*UnitConverter::getAngleMultiplier(),
                                                                     'f',UnitConverter::angleDigits) )
                                                         .arg( UnitConverter::getAngleUnitString() ) );
            FeatureTreeItem *zenith = new FeatureTreeItem( QString("zenith: %1 %2")
                                                         .arg( QString::number(obs->myReading->rPolar.zenith*UnitConverter::getAngleMultiplier(),
                                                                     'f',UnitConverter::angleDigits) )
                                                         .arg( UnitConverter::getAngleUnitString() ) );
            FeatureTreeItem *distance = new FeatureTreeItem( QString("distance: %1 %2")
                                                         .arg( QString::number(obs->myReading->rPolar.distance*UnitConverter::getDistanceMultiplier(),
                                                                     'f',UnitConverter::distanceDigits) )
                                                         .arg( UnitConverter::getDistanceUnitString() ) );
            geom_reading->appendChild(azimuth);
            geom_reading->appendChild(zenith);
            geom_reading->appendChild(distance);
        }else if(obs->myReading->typeofReading == Configuration::eCartesian){
            FeatureTreeItem *x = new FeatureTreeItem( QString("x: %1 %2")
                                                         .arg( QString::number(obs->myReading->rCartesian.xyz.getAt(0)*UnitConverter::getDistanceMultiplier(),
                                                                     'f',UnitConverter::distanceDigits) )
                                                         .arg( UnitConverter::getDistanceUnitString() ) );
            FeatureTreeItem *y = new FeatureTreeItem( QString("y: %1 %2")
                                                         .arg( QString::number(obs->myReading->rCartesian.xyz.getAt(0)*UnitConverter::getDistanceMultiplier(),
                                                                     'f',UnitConverter::distanceDigits) )
                                                         .arg( UnitConverter::getDistanceUnitString() ) );
            FeatureTreeItem *z = new FeatureTreeItem( QString("z: %1 %2")
                                                         .arg( QString::number(obs->myReading->rCartesian.xyz.getAt(0)*UnitConverter::getDistanceMultiplier(),
                                                                     'f',UnitConverter::distanceDigits) )
                                                         .arg( UnitConverter::getDistanceUnitString() ) );
            geom_reading->appendChild(x);
            geom_reading->appendChild(y);
            geom_reading->appendChild(z);
        }else if(obs->myReading->typeofReading == Configuration::eTemperatur){
            FeatureTreeItem *temperature = new FeatureTreeItem( QString("temperature: %1 %2")
                                                         .arg( QString::number(obs->myReading->rTemperature.tempDeg*UnitConverter::getDistanceMultiplier(),
                                                                     'f',UnitConverter::temperatureDigits) )
                                                         .arg( UnitConverter::getTemperatureUnitString() ) );
            geom_reading->appendChild(temperature);
        }
    }
    geom_reading->setReading(obs->myReading);
}
