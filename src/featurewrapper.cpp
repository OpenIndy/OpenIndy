#include "featurewrapper.h"

/*!
 * \brief FeatureWrapper::FeatureWrapper
 * \param parent
 */
FeatureWrapper::FeatureWrapper(QObject *parent) : QObject(parent){
    this->typeOfFeature = eUndefinedFeature;
}

/*!
 * \brief FeatureWrapper::operator ==
 * Compare 2 FeatureWrappers by their underlying feature id
 * \param other
 * \return
 */
bool FeatureWrapper::operator==(const FeatureWrapper &other){

    //check if both feature wrappers have got a valid feature
    if(this->feature.isNull() || other.getFeature().isNull()){
        return false;
    }

    //compare the feature ids
    if(this->feature->getId() == other.getFeature()->getId()){
        return true;
    }

    return false;

}

/*!
 * \brief FeatureWrapper::getFeatureTypeString
 * \return
 */
QString FeatureWrapper::getFeatureTypeString() const{
    return getFeatureTypeName(this->typeOfFeature);
}


/*!
 * \brief FeatureWrapper::getFeatureTypeEnum
 * \return
 */
const FeatureTypes &FeatureWrapper::getFeatureTypeEnum() const{
    return this->typeOfFeature;
}

/*!
 * \brief FeatureWrapper::getFeature
 * \return
 */
const QPointer<Feature> &FeatureWrapper::getFeature() const{
    return this->feature;
}

/*!
 * \brief FeatureWrapper::getGeometry
 * \return
 */
const QPointer<Geometry> &FeatureWrapper::getGeometry() const{
    return this->geometry;
}

/*!
 * \brief FeatureWrapper::getCoordinateSystem
 * \return
 */
const QPointer<CoordinateSystem> &FeatureWrapper::getCoordinateSystem() const{
    return this->coordinateSystem;
}

/*!
 * \brief FeatureWrapper::getTrafoParam
 * \return
 */
const QPointer<TrafoParam> &FeatureWrapper::getTrafoParam() const{
    return this->trafoParam;
}

/*!
 * \brief FeatureWrapper::getStation
 * \return
 */
const QPointer<Station> &FeatureWrapper::getStation() const{
    return this->station;
}

/*!
 * \brief FeatureWrapper::getPoint
 * \return
 */
const QPointer<Point> &FeatureWrapper::getPoint() const{
    return this->point;
}

/*!
 * \brief FeatureWrapper::getLine
 * \return
 */
const QPointer<Line> &FeatureWrapper::getLine() const{
    return this->line;
}

/*!
 * \brief FeatureWrapper::getPlane
 * \return
 */
const QPointer<Plane> &FeatureWrapper::getPlane() const{
    return this->plane;
}

/*!
 * \brief FeatureWrapper::getSphere
 * \return
 */
const QPointer<Sphere> &FeatureWrapper::getSphere() const{
    return this->sphere;
}

/*!
 * \brief FeatureWrapper::getCircle
 * \return
 */
const QPointer<Circle> &FeatureWrapper::getCircle() const{
    return this->circle;
}

/*!
 * \brief FeatureWrapper::getCone
 * \return
 */
const QPointer<Cone> &FeatureWrapper::getCone() const{
    return this->cone;
}

/*!
 * \brief FeatureWrapper::getCylinder
 * \return
 */
const QPointer<Cylinder> &FeatureWrapper::getCylinder() const{
    return this->cylinder;
}

/*!
 * \brief FeatureWrapper::getEllipsoid
 * \return
 */
const QPointer<Ellipsoid> &FeatureWrapper::getEllipsoid() const{
    return this->ellipsoid;
}

/*!
 * \brief FeatureWrapper::getHyperboloid
 * \return
 */
const QPointer<Hyperboloid> &FeatureWrapper::getHyperboloid() const{
    return this->hyperboloid;
}

/*!
 * \brief FeatureWrapper::getParaboloid
 * \return
 */
const QPointer<Paraboloid> &FeatureWrapper::getParaboloid() const{
    return this->paraboloid;
}

/*!
 * \brief FeatureWrapper::getPointCloud
 * \return
 */
const QPointer<PointCloud> &FeatureWrapper::getPointCloud() const{
    return this->pointCloud;
}

/*!
 * \brief FeatureWrapper::getNurbs
 * \return
 */
const QPointer<Nurbs> &FeatureWrapper::getNurbs() const{
    return this->nurbs;
}

/*!
 * \brief FeatureWrapper::getEllipse
 * \return
 */
const QPointer<Ellipse> &FeatureWrapper::getEllipse() const{
    return this->ellipse;
}

/*!
 * \brief FeatureWrapper::getSlottedHole
 * \return
 */
const QPointer<SlottedHole> &FeatureWrapper::getSlottedHole() const{
    return this->slottedHole;
}

/*!
 * \brief FeatureWrapper::getTorus
 * \return
 */
const QPointer<Torus> &FeatureWrapper::getTorus() const{
    return this->torus;
}

/*!
 * \brief FeatureWrapper::getScalarEntityAngle
 * \return
 */
const QPointer<ScalarEntityAngle> &FeatureWrapper::getScalarEntityAngle() const{
    return this->angle;
}

/*!
 * \brief FeatureWrapper::getScalarEntityDistance
 * \return
 */
const QPointer<ScalarEntityDistance> &FeatureWrapper::getScalarEntityDistance() const{
    return this->distance;
}

/*!
 * \brief FeatureWrapper::getScalarEntityTemperature
 * \return
 */
const QPointer<ScalarEntityTemperature> &FeatureWrapper::getScalarEntityTemperature() const{
    return this->temperature;
}

/*!
 * \brief FeatureWrapper::getScalarEntityMeasurementSeries
 * \return
 */
const QPointer<ScalarEntityMeasurementSeries> &FeatureWrapper::getScalarEntityMeasurementSeries() const{
    return this->measurementSeries;
}

/*!
 * \brief FeatureWrapper::setCoordinateSystem
 * \param coordSystem
 */
void FeatureWrapper::setCoordinateSystem(const QPointer<CoordinateSystem> &coordSystem){
    if(!coordSystem.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = coordSystem;
        this->coordinateSystem = coordSystem;
        this->typeOfFeature = eCoordinateSystemFeature;
    }
}

/*!
 * \brief FeatureWrapper::setTrafoParam
 * \param trafoParam
 */
void FeatureWrapper::setTrafoParam(const QPointer<TrafoParam> &trafoParam){
    if(!trafoParam.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = trafoParam;
        this->trafoParam = trafoParam;
        this->typeOfFeature = eTrafoParamFeature;
    }
}

/*!
 * \brief FeatureWrapper::setStation
 * \param station
 */
void FeatureWrapper::setStation(const QPointer<Station> &station){
    if(!station.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = station;
        this->station = station;
        this->typeOfFeature = eStationFeature;
    }
}

/*!
 * \brief FeatureWrapper::setPoint
 * \param point
 */
void FeatureWrapper::setPoint(const QPointer<Point> &point){
    if(!point.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = point;
        this->geometry = point;
        this->point = point;
        this->typeOfFeature = ePointFeature;
    }
}

/*!
 * \brief FeatureWrapper::setLine
 * \param line
 */
void FeatureWrapper::setLine(const QPointer<Line> &line){
    if(!line.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = line;
        this->geometry = line;
        this->line = line;
        this->typeOfFeature = eLineFeature;
    }
}

/*!
 * \brief FeatureWrapper::setPlane
 * \param plane
 */
void FeatureWrapper::setPlane(const QPointer<Plane> &plane){
    if(!plane.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = plane;
        this->geometry = plane;
        this->plane = plane;
        this->typeOfFeature = ePlaneFeature;
    }
}

/*!
 * \brief FeatureWrapper::setSphere
 * \param sphere
 */
void FeatureWrapper::setSphere(const QPointer<Sphere> &sphere){
    if(!sphere.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = sphere;
        this->geometry = sphere;
        this->sphere = sphere;
        this->typeOfFeature = eSphereFeature;
    }
}

/*!
 * \brief FeatureWrapper::setCircle
 * \param circle
 */
void FeatureWrapper::setCircle(const QPointer<Circle> &circle){
    if(!circle.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = circle;
        this->geometry = circle;
        this->circle = circle;
        this->typeOfFeature = eCircleFeature;
    }
}

/*!
 * \brief FeatureWrapper::setCone
 * \param cone
 */
void FeatureWrapper::setCone(const QPointer<Cone> &cone){
    if(!cone.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = cone;
        this->geometry = cone;
        this->cone = cone;
        this->typeOfFeature = eConeFeature;
    }
}

/*!
 * \brief FeatureWrapper::setCylinder
 * \param cylinder
 */
void FeatureWrapper::setCylinder(const QPointer<Cylinder> &cylinder){
    if(!cylinder.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = cylinder;
        this->geometry = cylinder;
        this->cylinder = cylinder;
        this->typeOfFeature = eCylinderFeature;
    }
}

/*!
 * \brief FeatureWrapper::setEllipsoid
 * \param ellipsoid
 */
void FeatureWrapper::setEllipsoid(const QPointer<Ellipsoid> &ellipsoid){
    if(!ellipsoid.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = ellipsoid;
        this->geometry = ellipsoid;
        this->ellipsoid = ellipsoid;
        this->typeOfFeature = eEllipsoidFeature;
    }
}

/*!
 * \brief FeatureWrapper::setHyperboloid
 * \param hyperboloid
 */
void FeatureWrapper::setHyperboloid(const QPointer<Hyperboloid> &hyperboloid){
    if(!hyperboloid.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = hyperboloid;
        this->geometry = hyperboloid;
        this->hyperboloid = hyperboloid;
        this->typeOfFeature = eHyperboloidFeature;
    }
}

/*!
 * \brief FeatureWrapper::setParaboloid
 * \param paraboloid
 */
void FeatureWrapper::setParaboloid(const QPointer<Paraboloid> &paraboloid){
    if(!paraboloid.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = paraboloid;
        this->geometry = paraboloid;
        this->paraboloid = paraboloid;
        this->typeOfFeature = eParaboloidFeature;
    }
}

/*!
 * \brief FeatureWrapper::setPointCloud
 * \param pointCloud
 */
void FeatureWrapper::setPointCloud(const QPointer<PointCloud> &pointCloud){
    if(!pointCloud.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = pointCloud;
        this->geometry = pointCloud;
        this->pointCloud = pointCloud;
        this->typeOfFeature = ePointCloudFeature;
    }
}

/*!
 * \brief FeatureWrapper::setNurbs
 * \param nurbs
 */
void FeatureWrapper::setNurbs(const QPointer<Nurbs> &nurbs){
    if(!nurbs.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = nurbs;
        this->geometry = nurbs;
        this->nurbs = nurbs;
        this->typeOfFeature = eNurbsFeature;
    }
}

/*!
 * \brief FeatureWrapper::setEllipse
 * \param ellipse
 */
void FeatureWrapper::setEllipse(const QPointer<Ellipse> &ellipse){
    if(!ellipse.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = ellipse;
        this->geometry = ellipse;
        this->ellipse = ellipse;
        this->typeOfFeature = eEllipseFeature;
    }
}

/*!
 * \brief FeatureWrapper::setSlottedHole
 * \param slottedHole
 */
void FeatureWrapper::setSlottedHole(const QPointer<SlottedHole> &slottedHole){
    if(!slottedHole.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = slottedHole;
        this->geometry = slottedHole;
        this->slottedHole = slottedHole;
        this->typeOfFeature = eSlottedHoleFeature;
    }
}

/*!
 * \brief FeatureWrapper::setTorus
 * \param torus
 */
void FeatureWrapper::setTorus(const QPointer<Torus> &torus){
    if(!torus.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = torus;
        this->geometry = torus;
        this->torus = torus;
        this->typeOfFeature = eTorusFeature;
    }
}

/*!
 * \brief FeatureWrapper::setScalarEntityAngle
 * \param angle
 */
void FeatureWrapper::setScalarEntityAngle(const QPointer<ScalarEntityAngle> &angle){
    if(!angle.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = angle;
        this->geometry = angle;
        this->angle = angle;
        this->typeOfFeature = eScalarEntityAngleFeature;
    }
}

/*!
 * \brief FeatureWrapper::setScalarEntityDistance
 * \param distance
 */
void FeatureWrapper::setScalarEntityDistance(const QPointer<ScalarEntityDistance> &distance){
    if(!distance.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = distance;
        this->geometry = distance;
        this->distance = distance;
        this->typeOfFeature = eScalarEntityDistanceFeature;
    }
}

/*!
 * \brief FeatureWrapper::setScalarEntityTemperature
 * \param temperature
 */
void FeatureWrapper::setScalarEntityTemperature(const QPointer<ScalarEntityTemperature> &temperature){
    if(!temperature.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = temperature;
        this->geometry = temperature;
        this->temperature = temperature;
        this->typeOfFeature = eScalarEntityTemperatureFeature;
    }
}

/*!
 * \brief FeatureWrapper::setScalarEntityMeasurementSeries
 * \param measurementSeries
 */
void FeatureWrapper::setScalarEntityMeasurementSeries(const QPointer<ScalarEntityMeasurementSeries> &measurementSeries){
    if(!measurementSeries.isNull() && this->typeOfFeature == eUndefinedFeature){
        this->feature = measurementSeries;
        this->geometry = measurementSeries;
        this->measurementSeries = measurementSeries;
        this->typeOfFeature = eScalarEntityMeasurementSeriesFeature;
    }
}
