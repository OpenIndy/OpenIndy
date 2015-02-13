#include "simulationtreeviewmodel.h"

SimulationTreeViewModel::SimulationTreeViewModel(QObject *parent) : QAbstractItemModel(parent){
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
QModelIndex SimulationTreeViewModel::index(int row, int column, const QModelIndex &parent) const{
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
QModelIndex SimulationTreeViewModel::parent(const QModelIndex &index) const{
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
int SimulationTreeViewModel::rowCount(const QModelIndex &parent) const{

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
int SimulationTreeViewModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

/*!
 * \brief FeatureTreeViewModel::data
 * \param index
 * \param role
 * \return
 */
QVariant SimulationTreeViewModel::data(const QModelIndex &index, int role) const{
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

FeatureTreeItem *SimulationTreeViewModel::getSelectedItem(const QModelIndex &index)
{

    FeatureTreeItem *item = static_cast<FeatureTreeItem*>(index.internalPointer());

    return item;
}

/*!
 * \brief FeatureTreeViewModel::getRootItem
 * \return
 */
FeatureTreeItem* SimulationTreeViewModel::getRootItem() const{
    return this->rootItem;
}

/*!
 * \brief FeatureTreeViewModel::refreshModel
 * Set up the tree view hierarchy
 */
void SimulationTreeViewModel::refreshModel(){
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
    //if(coordinateSystems.size() > 0){ this->appendCoordinateSystems(this->rootItem, coordinateSystems); }
    //if(stations.size() > 0){ this->appendStations(this->rootItem, stations); }
    //if(trafoParams.size() > 0){ this->appendTrafoParams(this->rootItem, trafoParams); }

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
void SimulationTreeViewModel::appendGeometries(FeatureTreeItem *root, QList<FeatureWrapper*> geometries, QString geomType){
    FeatureTreeItem *item = new FeatureTreeItem(geomType);
    root->appendChild(item);
    for(int it = 0; it < geometries.size(); it++){
        QString geomHeader = QString("%1 - %2").arg(geometries.at(it)->getGeometry()->getFeatureName())
                .arg(geometries.at(it)->getGeometry()->getIsNominal()?QString("nominal (%1)")
                                                                   .arg(geometries.at(it)->getGeometry()->getNominalSystem()->getFeatureName()):"actual");
        FeatureTreeItem *geom = new FeatureTreeItem(geomHeader);
        item->appendChild(geom);

        FeatureTreeItem *x, *y, *z, *i, *j, *k, *r, *angle, *distance, *temp;

        switch(geometries.at(it)->getTypeOfFeature()){
        case(Configuration::ePointFeature):
            x = new FeatureTreeItem(QString("X"));
            y = new FeatureTreeItem(QString("Y"));
            z = new FeatureTreeItem(QString("Z"));
            geom->appendChild(x);
            geom->appendChild(y);
            geom->appendChild(z);
            break;
        case(Configuration::eLineFeature):
            x = new FeatureTreeItem(QString("X"));
            y = new FeatureTreeItem(QString("Y"));
            z = new FeatureTreeItem(QString("Z"));
            i = new FeatureTreeItem(QString("I"));
            j = new FeatureTreeItem(QString("J"));
            k = new FeatureTreeItem(QString("K"));
            geom->appendChild(x);
            geom->appendChild(y);
            geom->appendChild(z);
            geom->appendChild(i);
            geom->appendChild(j);
            geom->appendChild(k);
            break;
        case(Configuration::ePlaneFeature):
            x = new FeatureTreeItem(QString("X"));
            y = new FeatureTreeItem(QString("Y"));
            z = new FeatureTreeItem(QString("Z"));
            i = new FeatureTreeItem(QString("I"));
            j = new FeatureTreeItem(QString("J"));
            k = new FeatureTreeItem(QString("K"));
            geom->appendChild(x);
            geom->appendChild(y);
            geom->appendChild(z);
            geom->appendChild(i);
            geom->appendChild(j);
            geom->appendChild(k);
            break;
        case(Configuration::eSphereFeature):
            x = new FeatureTreeItem(QString("X"));
            y = new FeatureTreeItem(QString("Y"));
            z = new FeatureTreeItem(QString("Z"));
            r = new FeatureTreeItem(QString("Radius"));
            geom->appendChild(x);
            geom->appendChild(y);
            geom->appendChild(z);
            geom->appendChild(r);
            break;
        case(Configuration::eCircleFeature):
            x = new FeatureTreeItem(QString("X"));
            y = new FeatureTreeItem(QString("Y"));
            z = new FeatureTreeItem(QString("Z"));
            i = new FeatureTreeItem(QString("I"));
            j = new FeatureTreeItem(QString("J"));
            k = new FeatureTreeItem(QString("K"));
            r = new FeatureTreeItem(QString("Radius"));
            geom->appendChild(x);
            geom->appendChild(y);
            geom->appendChild(z);
            geom->appendChild(i);
            geom->appendChild(j);
            geom->appendChild(k);
            geom->appendChild(r);
            break;
        case(Configuration::eConeFeature):
            break;
        case(Configuration::eCylinderFeature):
            break;
        case(Configuration::eParaboloidFeature):
            break;
        case(Configuration::eHyperboloidFeature):
            break;
        case(Configuration::eEllipsoidFeature):
            break;
        case(Configuration::eScalarEntityAngleFeature):
            angle = new FeatureTreeItem(QString("angle"));
            geom->appendChild(angle);
            break;
        case(Configuration::eScalarEntityDistanceFeature):
            distance = new FeatureTreeItem(QString("distance"));
            geom->appendChild(distance);
            break;
        case(Configuration::eScalarEntityTemperatureFeature):
            temp = new FeatureTreeItem(QString("temperature"));
            geom->appendChild(temp);
            break;
        case(Configuration::ePointCloudFeature):
            break;
        }

        geom->setFeature(geometries.at(it));
    }
}

/*!
 * \brief FeatureTreeViewModel::appendCoordinateSystem
 * \param root
 * \param coordinateSystems
 */
void SimulationTreeViewModel::appendCoordinateSystems(FeatureTreeItem *root, QList<FeatureWrapper*> coordinateSystems){
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
void SimulationTreeViewModel::appendStations(FeatureTreeItem *root, QList<FeatureWrapper *> stations){
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
            for(int j = 0; j < stations.at(i)->getStation()->position->getObservations().size(); j++){
                Observation *obs = stations.at(i)->getStation()->position->getObservations().at(j);
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
void SimulationTreeViewModel::appendTrafoParams(FeatureTreeItem *root, QList<FeatureWrapper*> trafoParams){
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
void SimulationTreeViewModel::appendObservation(FeatureTreeItem *root, Observation *obs){
    FeatureTreeItem *geom_observation = new FeatureTreeItem(QString::number(obs->myReading->id));
    root->appendChild(geom_observation);
    geom_observation->appendChild( new FeatureTreeItem(obs->myReading->measuredAt.toString()) );
    geom_observation->appendChild( new FeatureTreeItem(obs->myStation->getFeatureName()) );
    geom_observation->appendChild( new FeatureTreeItem(obs->getUseState()?"valid: true":"valid: false") );
    if(obs->getUseState()){
        /*FeatureTreeItem *x = new FeatureTreeItem( QString("x: %1 %2").arg( QString::number(obs->myXyz.getAt(0)*OiUnitConverter::getDistanceMultiplier(),
                                                                 'f',OiUnitConverter::distanceDigits) )
                                                    .arg( OiUnitConverter::getDistanceUnitString() ) );
        FeatureTreeItem *y = new FeatureTreeItem( QString("y: %1 %2").arg( QString::number(obs->myXyz.getAt(1)*OiUnitConverter::getDistanceMultiplier(),
                                                                 'f',OiUnitConverter::distanceDigits) )
                                                    .arg( OiUnitConverter::getDistanceUnitString() ) );
        FeatureTreeItem *z = new FeatureTreeItem( QString("z: %1 %2").arg( QString::number(obs->myXyz.getAt(2)*OiUnitConverter::getDistanceMultiplier(),
                                                                 'f',OiUnitConverter::distanceDigits) )
                                                    .arg( OiUnitConverter::getDistanceUnitString() ) );
        geom_observation->appendChild(x);
        geom_observation->appendChild(y);
        geom_observation->appendChild(z);*/
    }
    geom_observation->setObservation(obs);
}

/*!
 * \brief FeatureTreeViewModel::appendReading
 * \param root
 * \param obs
 */
void SimulationTreeViewModel::appendReading(FeatureTreeItem *root, Observation *obs){
    FeatureTreeItem *geom_reading = new FeatureTreeItem(QString::number(obs->myReading->id));
    root->appendChild(geom_reading);
    geom_reading->appendChild( new FeatureTreeItem(obs->myReading->measuredAt.toString()) );
    geom_reading->appendChild( new FeatureTreeItem(obs->myStation->getFeatureName()) );
    geom_reading->appendChild( new FeatureTreeItem(obs->getUseState()?"valid: true":"valid: false") );
    if(obs->getUseState()){
        /*if(obs->myReading->typeofReading == Configuration::eDistance){
            FeatureTreeItem *distance = new FeatureTreeItem( QString("distance: %1 %2")
                                                         .arg( QString::number(obs->myReading->rDistance.distance*OiUnitConverter::getDistanceMultiplier(),
                                                                     'f',OiUnitConverter::distanceDigits) )
                                                         .arg( OiUnitConverter::getDistanceUnitString() ) );
            geom_reading->appendChild(distance);
        }else if(obs->myReading->typeofReading == Configuration::eDirection){
            FeatureTreeItem *azimuth = new FeatureTreeItem( QString("azimuth: %1 %2")
                                                         .arg( QString::number(obs->myReading->rDirection.azimuth*OiUnitConverter::getAngleMultiplier(),
                                                                     'f',OiUnitConverter::angleDigits) )
                                                         .arg( OiUnitConverter::getAngleUnitString() ) );
            FeatureTreeItem *zenith = new FeatureTreeItem( QString("zenith: %1 %2")
                                                         .arg( QString::number(obs->myReading->rDirection.zenith*OiUnitConverter::getAngleMultiplier(),
                                                                     'f',OiUnitConverter::angleDigits) )
                                                         .arg( OiUnitConverter::getAngleUnitString() ) );
            geom_reading->appendChild(azimuth);
            geom_reading->appendChild(zenith);
        }else if(obs->myReading->typeofReading == Configuration::ePolar){
            FeatureTreeItem *azimuth = new FeatureTreeItem( QString("azimuth: %1 %2")
                                                         .arg( QString::number(obs->myReading->rPolar.azimuth*OiUnitConverter::getAngleMultiplier(),
                                                                     'f',OiUnitConverter::angleDigits) )
                                                         .arg( OiUnitConverter::getAngleUnitString() ) );
            FeatureTreeItem *zenith = new FeatureTreeItem( QString("zenith: %1 %2")
                                                         .arg( QString::number(obs->myReading->rPolar.zenith*OiUnitConverter::getAngleMultiplier(),
                                                                     'f',OiUnitConverter::angleDigits) )
                                                         .arg( OiUnitConverter::getAngleUnitString() ) );
            FeatureTreeItem *distance = new FeatureTreeItem( QString("distance: %1 %2")
                                                         .arg( QString::number(obs->myReading->rPolar.distance*OiUnitConverter::getDistanceMultiplier(),
                                                                     'f',OiUnitConverter::distanceDigits) )
                                                         .arg( OiUnitConverter::getDistanceUnitString() ) );
            geom_reading->appendChild(azimuth);
            geom_reading->appendChild(zenith);
            geom_reading->appendChild(distance);
        }else if(obs->myReading->typeofReading == Configuration::eCartesian){
            FeatureTreeItem *x = new FeatureTreeItem( QString("x: %1 %2")
                                                         .arg( QString::number(obs->myReading->rCartesian.xyz.getAt(0)*OiUnitConverter::getDistanceMultiplier(),
                                                                     'f',OiUnitConverter::distanceDigits) )
                                                         .arg( OiUnitConverter::getDistanceUnitString() ) );
            FeatureTreeItem *y = new FeatureTreeItem( QString("y: %1 %2")
                                                         .arg( QString::number(obs->myReading->rCartesian.xyz.getAt(0)*OiUnitConverter::getDistanceMultiplier(),
                                                                     'f',OiUnitConverter::distanceDigits) )
                                                         .arg( OiUnitConverter::getDistanceUnitString() ) );
            FeatureTreeItem *z = new FeatureTreeItem( QString("z: %1 %2")
                                                         .arg( QString::number(obs->myReading->rCartesian.xyz.getAt(0)*OiUnitConverter::getDistanceMultiplier(),
                                                                     'f',OiUnitConverter::distanceDigits) )
                                                         .arg( OiUnitConverter::getDistanceUnitString() ) );
            geom_reading->appendChild(x);
            geom_reading->appendChild(y);
            geom_reading->appendChild(z);
        }else if(obs->myReading->typeofReading == Configuration::eTemperatur){
            FeatureTreeItem *temperature = new FeatureTreeItem( QString("temperature: %1 %2")
                                                         .arg( QString::number(obs->myReading->rTemperature.tempDeg*OiUnitConverter::getDistanceMultiplier(),
                                                                     'f',OiUnitConverter::temperatureDigits) )
                                                         .arg( OiUnitConverter::getTemperatureUnitString() ) );
            geom_reading->appendChild(temperature);
        }*/
    }
    geom_reading->setReading(obs->myReading);
}
