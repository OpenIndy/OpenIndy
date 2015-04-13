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
    if(this->myFeature.isNull() || other.getFeature().isNull()){
        return false;
    }

    //compare the feature ids
    if(this->myFeature->getId() == other.getFeature()->getId()){
        return true;
    }

    return false;

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
    return this->myFeature;
}

/*!
 * \brief FeatureWrapper::getGeometry
 * \return
 */
const QPointer<Geometry> &FeatureWrapper::getGeometry() const{
    return this->myGeometry;
}

/*!
 * \brief FeatureWrapper::getCoordinateSystem
 * \return
 */
const QPointer<CoordinateSystem> &FeatureWrapper::getCoordinateSystem() const{
    return this->myCoordinateSystem;
}

/*!
 * \brief FeatureWrapper::getTrafoParam
 * \return
 */
const QPointer<TrafoParam> &FeatureWrapper::getTrafoParam() const{
    return this->myTrafoParam;
}

/*!
 * \brief FeatureWrapper::getStation
 * \return
 */
const QPointer<Station> &FeatureWrapper::getStation() const{
    return this->myStation;
}

/*!
 * \brief FeatureWrapper::getPoint
 * \return
 */
const QPointer<Point> &FeatureWrapper::getPoint() const{
    return this->myPoint;
}

/*!
 * \brief FeatureWrapper::getLine
 * \return
 */
const QPointer<Line> &FeatureWrapper::getLine() const{
    return this->myLine;
}

/*!
 * \brief FeatureWrapper::getPlane
 * \return
 */
const QPointer<Plane> &FeatureWrapper::getPlane() const{
    return this->myPlane;
}

/*!
 * \brief FeatureWrapper::getSphere
 * \return
 */
const QPointer<Sphere> &FeatureWrapper::getSphere() const{
    return this->mySphere;
}

/*!
 * \brief FeatureWrapper::getCircle
 * \return
 */
const QPointer<Circle> &FeatureWrapper::getCircle() const{
    return this->myCircle;
}

/*!
 * \brief FeatureWrapper::getCone
 * \return
 */
const QPointer<Cone> &FeatureWrapper::getCone() const{
    return this->myCone;
}

/*!
 * \brief FeatureWrapper::getCylinder
 * \return
 */
const QPointer<Cylinder> &FeatureWrapper::getCylinder() const{
    return this->myCylinder;
}

/*!
 * \brief FeatureWrapper::getEllipsoid
 * \return
 */
const QPointer<Ellipsoid> &FeatureWrapper::getEllipsoid() const{
    return this->myEllipsoid;
}

/*!
 * \brief FeatureWrapper::getHyperboloid
 * \return
 */
const QPointer<Hyperboloid> &FeatureWrapper::getHyperboloid() const{
    return this->myHyperboloid;
}

/*!
 * \brief FeatureWrapper::getParaboloid
 * \return
 */
const QPointer<Paraboloid> &FeatureWrapper::getParaboloid() const{
    return this->myParaboloid;
}

/*!
 * \brief FeatureWrapper::getPointCloud
 * \return
 */
const QPointer<PointCloud> &FeatureWrapper::getPointCloud() const{
    return this->myPointCloud;
}

/*!
 * \brief FeatureWrapper::getNurbs
 * \return
 */
const QPointer<Nurbs> &FeatureWrapper::getNurbs() const{
    return this->myNurbs;
}

/*!
 * \brief FeatureWrapper::getScalarEntityAngle
 * \return
 */
const QPointer<ScalarEntityAngle> &FeatureWrapper::getScalarEntityAngle() const{
    return this->myScalarEntityAngle;
}

/*!
 * \brief FeatureWrapper::getScalarEntityDistance
 * \return
 */
const QPointer<ScalarEntityDistance> &FeatureWrapper::getScalarEntityDistance() const{
    return this->myScalarEntityDistance;
}

/*!
 * \brief FeatureWrapper::getScalarEntityTemperature
 * \return
 */
const QPointer<ScalarEntityTemperature> &FeatureWrapper::getScalarEntityTemperature() const{
    return this->myScalarEntityTemperature;
}

/*!
 * \brief FeatureWrapper::getScalarEntityMeasurementSeries
 * \return
 */
const QPointer<ScalarEntityMeasurementSeries> &FeatureWrapper::getScalarEntityMeasurementSeries() const{
    return this->myScalarEntityMeasurementSeries;
}

/*!
 * \brief FeatureWrapper::setCoordinateSystem
 * \param coordSys
 */
void FeatureWrapper::setCoordinateSystem(const QPointer<CoordinateSystem> &coordSystem){
    if(!coordSystem.isNull()){
        this->myFeature = coordSystem;
        this->myCoordinateSystem = coordSystem;
        this->typeOfFeature = eCoordinateSystemFeature;
    }
}

/*!
 * \brief FeatureWrapper::setTrafoParam
 * \param trafoPara
 */
void FeatureWrapper::setTrafoParam(const QPointer<TrafoParam> &trafoPara){
    if(!trafoPara.isNull()){
        this->myFeature = trafoPara;
        this->myTrafoParam = trafoPara;
        this->typeOfFeature = eTrafoParamFeature;
    }
}

/*!
 * \brief FeatureWrapper::setStation
 * \param s
 */
void FeatureWrapper::setStation(const QPointer<Station> &station){
    if(!station.isNull()){
        this->myFeature = station;
        this->myStation = station;
        this->typeOfFeature = eStationFeature;
    }
}

/*!
 * \brief FeatureWrapper::setPoint
 * \param p
 */
void FeatureWrapper::setPoint(const QPointer<Point> &point){
    if(!point.isNull()){
        this->myFeature = point;
        this->myGeometry = point;
        this->myPoint = point;
        this->typeOfFeature = ePointFeature;
    }
}

/*!
 * \brief FeatureWrapper::setLine
 * \param l
 */
void FeatureWrapper::setLine(const QPointer<Line> &line){
    if(!line.isNull()){
        this->myFeature = line;
        this->myGeometry = line;
        this->myLine = line;
        this->typeOfFeature = eLineFeature;
    }
}

/*!
 * \brief FeatureWrapper::setPlane
 * \param p
 */
void FeatureWrapper::setPlane(const QPointer<Plane> &plane){
    if(!plane.isNull()){
        this->myFeature = plane;
        this->myGeometry = plane;
        this->myPlane = plane;
        this->typeOfFeature = ePlaneFeature;
    }
}

/*!
 * \brief FeatureWrapper::setSphere
 * \param s
 */
void FeatureWrapper::setSphere(const QPointer<Sphere> &sphere){
    if(!sphere.isNull()){
        this->myFeature = sphere;
        this->myGeometry = sphere;
        this->mySphere = sphere;
        this->typeOfFeature = eSphereFeature;
    }
}

/*!
 * \brief FeatureWrapper::setCircle
 * \param c
 */
void FeatureWrapper::setCircle(const QPointer<Circle> &circle){
    if(!circle.isNull()){
        this->myFeature = circle;
        this->myGeometry = circle;
        this->myCircle = circle;
        this->typeOfFeature = eCircleFeature;
    }
}

/*!
 * \brief FeatureWrapper::setCone
 * \param c
 */
void FeatureWrapper::setCone(const QPointer<Cone> &cone){
    if(!cone.isNull()){
        this->myFeature = cone;
        this->myGeometry = cone;
        this->myCone = cone;
        this->typeOfFeature = eConeFeature;
    }
}

/*!
 * \brief FeatureWrapper::setCylinder
 * \param c
 */
void FeatureWrapper::setCylinder(const QPointer<Cylinder> &cylinder){
    if(!cylinder.isNull()){
        this->myFeature = cylinder;
        this->myGeometry = cylinder;
        this->myCylinder = cylinder;
        this->typeOfFeature = eCylinderFeature;
    }
}

/*!
 * \brief FeatureWrapper::setEllipsoid
 * \param e
 */
void FeatureWrapper::setEllipsoid(const QPointer<Ellipsoid> &ellipsoid){
    if(!ellipsoid.isNull()){
        this->myFeature = ellipsoid;
        this->myGeometry = ellipsoid;
        this->myEllipsoid = ellipsoid;
        this->typeOfFeature = eEllipsoidFeature;
    }
}

/*!
 * \brief FeatureWrapper::setHyperboloid
 * \param h
 */
void FeatureWrapper::setHyperboloid(const QPointer<Hyperboloid> &hyperboloid){
    if(!hyperboloid.isNull()){
        this->myFeature = hyperboloid;
        this->myGeometry = hyperboloid;
        this->myHyperboloid = hyperboloid;
        this->typeOfFeature = eHyperboloidFeature;
    }
}

/*!
 * \brief FeatureWrapper::setParaboloid
 * \param p
 */
void FeatureWrapper::setParaboloid(const QPointer<Paraboloid> &paraboloid){
    if(!paraboloid.isNull()){
        this->myFeature = paraboloid;
        this->myGeometry = paraboloid;
        this->myParaboloid = paraboloid;
        this->typeOfFeature = eParaboloidFeature;
    }
}

/*!
 * \brief FeatureWrapper::setPointCloud
 * \param p
 */
void FeatureWrapper::setPointCloud(const QPointer<PointCloud> &pointCloud){
    if(!pointCloud.isNull()){
        this->myFeature = pointCloud;
        this->myGeometry = pointCloud;
        this->myPointCloud = pointCloud;
        this->typeOfFeature = ePointCloudFeature;
    }
}

/*!
 * \brief FeatureWrapper::setNurbs
 * \param n
 */
void FeatureWrapper::setNurbs(const QPointer<Nurbs> &nurbs){
    if(!nurbs.isNull()){
        this->myFeature = nurbs;
        this->myGeometry = nurbs;
        this->myNurbs = nurbs;
        this->typeOfFeature = eNurbsFeature;
    }
}

/*!
 * \brief FeatureWrapper::setScalarEntityAngle
 * \param sEA
 */
void FeatureWrapper::setScalarEntityAngle(const QPointer<ScalarEntityAngle> &angle){
    if(!angle.isNull()){
        this->myFeature = angle;
        this->myGeometry = angle;
        this->myScalarEntityAngle = angle;
        this->typeOfFeature = eScalarEntityAngleFeature;
    }
}

/*!
 * \brief FeatureWrapper::setScalarEntityDistance
 * \param sED
 */
void FeatureWrapper::setScalarEntityDistance(const QPointer<ScalarEntityDistance> &distance){
    if(!distance.isNull()){
        this->myFeature = distance;
        this->myGeometry = distance;
        this->myScalarEntityDistance = distance;
        this->typeOfFeature = eScalarEntityDistanceFeature;
    }
}

/*!
 * \brief FeatureWrapper::setScalarEntityTemperature
 * \param SET
 */
void FeatureWrapper::setScalarEntityTemperature(const QPointer<ScalarEntityTemperature> &temperature){
    if(!temperature.isNull()){
        this->myFeature = temperature;
        this->myGeometry = temperature;
        this->myScalarEntityTemperature = temperature;
        this->typeOfFeature = eScalarEntityTemperatureFeature;
    }
}

/*!
 * \brief FeatureWrapper::setScalarEntityMeasurementSeries
 * \param SEMS
 */
void FeatureWrapper::setScalarEntityMeasurementSeries(const QPointer<ScalarEntityMeasurementSeries> &measurementSeries){
    if(!measurementSeries.isNull()){
        this->myFeature = measurementSeries;
        this->myGeometry = measurementSeries;
        this->myScalarEntityMeasurementSeries = measurementSeries;
        this->typeOfFeature = eScalarEntityMeasurementSeriesFeature;
    }
}

/*!
 * \brief FeatureWrapper::getFeatureTypeString
 * \return
 */
QString FeatureWrapper::getFeatureTypeString() const{
    return getFeatureTypeName(this->typeOfFeature);
}
