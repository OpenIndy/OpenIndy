#include "featurewrapper.h"

FeatureWrapper::FeatureWrapper()
{
    this->myFeature = NULL;
    this->myGeometry = NULL;

    this->myCoordinateSystem = NULL;
    this->myTrafoParam = NULL;

    this->myStation = NULL;

    this->myPoint = NULL;
    this->myLine = NULL;
    this->myPlane = NULL;
    this->mySphere = NULL;
    this->myCircle = NULL;
    this->myCone = NULL;
    this->myCylinder = NULL;
    this->myEllipsoid = NULL;
    this->myHyperboloid = NULL;
    this->myParaboloid = NULL;
    this->myNurbs = NULL;
    this->myPointCloud = NULL;
    this->myScalarEntityAngle = NULL;
    this->myScalarEntityDistance = NULL;
    this->myScalarEntityMeasurementSeries = NULL;
    this->myScalarEntityTemperature = NULL;
}

/*!
 * \brief FeatureWrapper::getTypeOfFeature
 * \return
 */
Configuration::FeatureTypes FeatureWrapper::getTypeOfFeature(){
    return this->typeOfFeature;
}

/*!
 * \brief FeatureWrapper::getFeature
 * \return
 */
Feature* FeatureWrapper::getFeature(){
    return this->myFeature;
}

/*!
 * \brief FeatureWrapper::getGeometry
 * \return
 */
Geometry* FeatureWrapper::getGeometry(){
    return this->myGeometry;
}

/*!
 * \brief FeatureWrapper::getCoordinateSystem
 * \return
 */
CoordinateSystem* FeatureWrapper::getCoordinateSystem(){
    return this->myCoordinateSystem;
}

/*!
 * \brief FeatureWrapper::getTrafoParam
 * \return
 */
TrafoParam* FeatureWrapper::getTrafoParam(){
    return this->myTrafoParam;
}

/*!
 * \brief FeatureWrapper::getStation
 * \return
 */
Station* FeatureWrapper::getStation(){
    return this->myStation;
}

/*!
 * \brief FeatureWrapper::getPoint
 * \return
 */
Point* FeatureWrapper::getPoint(){
    return this->myPoint;
}

/*!
 * \brief FeatureWrapper::getLine
 * \return
 */
Line* FeatureWrapper::getLine(){
    return this->myLine;
}

/*!
 * \brief FeatureWrapper::getPlane
 * \return
 */
Plane* FeatureWrapper::getPlane(){
    return this->myPlane;
}

/*!
 * \brief FeatureWrapper::getSphere
 * \return
 */
Sphere* FeatureWrapper::getSphere(){
    return this->mySphere;
}

/*!
 * \brief FeatureWrapper::getCircle
 * \return
 */
Circle* FeatureWrapper::getCircle(){
    return this->myCircle;
}

/*!
 * \brief FeatureWrapper::getCone
 * \return
 */
Cone* FeatureWrapper::getCone(){
    return this->myCone;
}

/*!
 * \brief FeatureWrapper::getCylinder
 * \return
 */
Cylinder* FeatureWrapper::getCylinder(){
    return this->myCylinder;
}

/*!
 * \brief FeatureWrapper::getEllipsoid
 * \return
 */
Ellipsoid* FeatureWrapper::getEllipsoid(){
    return this->myEllipsoid;
}

/*!
 * \brief FeatureWrapper::getHyperboloid
 * \return
 */
Hyperboloid* FeatureWrapper::getHyperboloid(){
    return this->myHyperboloid;
}

/*!
 * \brief FeatureWrapper::getParaboloid
 * \return
 */
Paraboloid* FeatureWrapper::getParaboloid(){
    return this->myParaboloid;
}

/*!
 * \brief FeatureWrapper::getPointCloud
 * \return
 */
PointCloud* FeatureWrapper::getPointCloud(){
    return this->myPointCloud;
}

/*!
 * \brief FeatureWrapper::getNurbs
 * \return
 */
Nurbs* FeatureWrapper::getNurbs(){
    return this->myNurbs;
}

/*!
 * \brief FeatureWrapper::getScalarEntityAngle
 * \return
 */
ScalarEntityAngle* FeatureWrapper::getScalarEntityAngle(){
    return this->myScalarEntityAngle;
}

/*!
 * \brief FeatureWrapper::getScalarEntityDistance
 * \return
 */
ScalarEntityDistance* FeatureWrapper::getScalarEntityDistance(){
    return this->myScalarEntityDistance;
}

/*!
 * \brief FeatureWrapper::getScalarEntityTemperature
 * \return
 */
ScalarEntityTemperature* FeatureWrapper::getScalarEntityTemperature(){
    return this->myScalarEntityTemperature;
}

/*!
 * \brief FeatureWrapper::getScalarEntityMeasurementSeries
 * \return
 */
ScalarEntityMeasurementSeries* FeatureWrapper::getScalarEntityMeasurementSeries(){
    return this->myScalarEntityMeasurementSeries;
}

/*!
 * \brief FeatureWrapper::setCoordinateSystem
 * \param coordSys
 */
void FeatureWrapper::setCoordinateSystem(CoordinateSystem *coordSys){
    if(coordSys != NULL){
        this->myFeature = coordSys;
        this->myCoordinateSystem= coordSys;
        this->typeOfFeature = Configuration::eCoordinateSystemFeature;
    }
}

/*!
 * \brief FeatureWrapper::setTrafoParam
 * \param trafoPara
 */
void FeatureWrapper::setTrafoParam(TrafoParam *trafoPara){
    if(trafoPara != NULL){
        this->myFeature = trafoPara;
        this->myTrafoParam = trafoPara;
        this->typeOfFeature = Configuration::eTrafoParamFeature;
    }
}

/*!
 * \brief FeatureWrapper::setStation
 * \param s
 */
void FeatureWrapper::setStation(Station *s){
    if(s != NULL){
        this->myFeature = s;
        this->myStation = s;
        this->typeOfFeature = Configuration::eStationFeature;
    }
}

/*!
 * \brief FeatureWrapper::setPoint
 * \param p
 */
void FeatureWrapper::setPoint(Point *p){
    if(p != NULL){
        this->myFeature = p;
        this->myGeometry = p;
        this->myPoint = p;
        this->typeOfFeature = Configuration::ePointFeature;
    }
}

/*!
 * \brief FeatureWrapper::setLine
 * \param l
 */
void FeatureWrapper::setLine(Line *l){
    if(l != NULL){
        this->myFeature = l;
        this->myGeometry = l;
        this->myLine = l;
        this->typeOfFeature = Configuration::eLineFeature;
    }
}

/*!
 * \brief FeatureWrapper::setPlane
 * \param p
 */
void FeatureWrapper::setPlane(Plane *p){
    if(p != NULL){
        this->myFeature = p;
        this->myGeometry = p;
        this->myPlane = p;
        this->typeOfFeature = Configuration::ePlaneFeature;
    }
}

/*!
 * \brief FeatureWrapper::setSphere
 * \param s
 */
void FeatureWrapper::setSphere(Sphere *s){
    if(s != NULL){
        this->myFeature = s;
        this->myGeometry = s;
        this->mySphere = s;
        this->typeOfFeature = Configuration::eSphereFeature;
    }
}

/*!
 * \brief FeatureWrapper::setCircle
 * \param c
 */
void FeatureWrapper::setCircle(Circle *c){
    if(c != NULL){
        this->myFeature = c;
        this->myGeometry = c;
        this->myCircle = c;
        this->typeOfFeature = Configuration::eCircleFeature;
    }
}

/*!
 * \brief FeatureWrapper::setCone
 * \param c
 */
void FeatureWrapper::setCone(Cone *c){
    if(c != NULL){
        this->myFeature = c;
        this->myGeometry = c;
        this->myCone = c;
        this->typeOfFeature = Configuration::eConeFeature;
    }
}

/*!
 * \brief FeatureWrapper::setCylinder
 * \param c
 */
void FeatureWrapper::setCylinder(Cylinder *c){
    if(c != NULL){
        this->myFeature = c;
        this->myGeometry = c;
        this->myCylinder = c;
        this->typeOfFeature = Configuration::eCylinderFeature;
    }
}

/*!
 * \brief FeatureWrapper::setEllipsoid
 * \param e
 */
void FeatureWrapper::setEllipsoid(Ellipsoid *e){
    if(e != NULL){
        this->myFeature = e;
        this->myGeometry = e;
        this->myEllipsoid = e;
        this->typeOfFeature = Configuration::eEllipsoidFeature;
    }
}

/*!
 * \brief FeatureWrapper::setHyperboloid
 * \param h
 */
void FeatureWrapper::setHyperboloid(Hyperboloid *h){
    if(h != NULL){
        this->myFeature = h;
        this->myGeometry = h;
        this->myHyperboloid = h;
        this->typeOfFeature = Configuration::eHyperboloidFeature;
    }
}

/*!
 * \brief FeatureWrapper::setParaboloid
 * \param p
 */
void FeatureWrapper::setParaboloid(Paraboloid *p){
    if(p != NULL){
        this->myFeature = p;
        this->myGeometry = p;
        this->myParaboloid = p;
        this->typeOfFeature = Configuration::eParaboloidFeature;
    }
}

/*!
 * \brief FeatureWrapper::setPointCloud
 * \param p
 */
void FeatureWrapper::setPointCloud(PointCloud *p){
    if(p != NULL){
        this->myFeature = p;
        this->myGeometry = p;
        this->myPointCloud = p;
        this->typeOfFeature = Configuration::ePointCloudFeature;
    }
}

/*!
 * \brief FeatureWrapper::setNurbs
 * \param n
 */
void FeatureWrapper::setNurbs(Nurbs *n){
    if(n != NULL){
        this->myFeature = n;
        this->myGeometry = n;
        this->myNurbs = n;
        this->typeOfFeature = Configuration::eNurbsFeature;
    }
}

/*!
 * \brief FeatureWrapper::setScalarEntityAngle
 * \param sEA
 */
void FeatureWrapper::setScalarEntityAngle(ScalarEntityAngle *sEA){
    if(sEA != NULL){
        this->myFeature = sEA;
        this->myGeometry = sEA;
        this->myScalarEntityAngle = sEA;
        this->typeOfFeature = Configuration::eScalarentityAngleFeature;
    }
}

/*!
 * \brief FeatureWrapper::setScalarEntityDistance
 * \param sED
 */
void FeatureWrapper::setScalarEntityDistance(ScalarEntityDistance *sED){
    if(sED != NULL){
        this->myFeature = sED;
        this->myGeometry = sED;
        this->myScalarEntityDistance = sED;
        this->typeOfFeature = Configuration::eScalarEntityDistanceFeature;
    }
}

/*!
 * \brief FeatureWrapper::setScalarEntityTemperature
 * \param SET
 */
void FeatureWrapper::setScalarEntityTemperature(ScalarEntityTemperature *SET){
    if(SET != NULL){
        this->myFeature = SET;
        this->myGeometry = SET;
        this->myScalarEntityTemperature = SET;
        this->typeOfFeature = Configuration::eScalarEntityTemperatureFeature;
    }
}

/*!
 * \brief FeatureWrapper::setScalarEntityMeasurementSeries
 * \param SEMS
 */
void FeatureWrapper::setScalarEntityMeasurementSeries(ScalarEntityMeasurementSeries *SEMS){
    if(SEMS != NULL){
        this->myFeature = SEMS;
        this->myGeometry = SEMS;
        this->myScalarEntityMeasurementSeries = SEMS;
        this->typeOfFeature = Configuration::eScalarEntityMeasurementSeriesFeature;
    }
}

/*!
 * \brief FeatureWrapper::returnFeatureType
 * \return
 */
QString FeatureWrapper::returnFeatureType(){
    switch (this->typeOfFeature) {
    case Configuration::ePlaneFeature:
        return Configuration::sPlane;
        break;
    case Configuration::ePointFeature:
        return Configuration::sPoint ;
        break;
    case Configuration::eLineFeature:
        return Configuration::sLine;
        break;
    case Configuration::eStationFeature:
        return Configuration::sStation;
        break;
    case Configuration::eCoordinateSystemFeature:
        return Configuration::sCoordinatesystem;
        break;
    case Configuration::eSphereFeature:
        return Configuration::sSphere;
        break;
    case Configuration::eScalarentityAngleFeature:
        return Configuration::sEntitiyAngle;
        break;
    case Configuration::eScalarEntityDistanceFeature:
        return Configuration::sEntityDistance;
         break;
    case Configuration::eTrafoParamFeature:
        return Configuration::sTrafoParam;
        break;
    case Configuration::eCircleFeature:
        return Configuration::sCircle;
        break;
    case Configuration::eConeFeature:
        return Configuration::sCone;
        break;
    case Configuration::eCylinderFeature:
        return Configuration::sCylinder;
        break;
    case Configuration::eEllipsoidFeature:
        return Configuration::sEllipsoid;
        break;
    case Configuration::eHyperboloidFeature:
        return Configuration::sHyperboloid;
        break;
    case Configuration::eParaboloidFeature:
        return Configuration::sParaboloid;
        break;
    case Configuration::eNurbsFeature:
        return Configuration::sNurbs;
        break;
    case Configuration::ePointCloudFeature:
        return Configuration::sPointCloud;
        break;
    case Configuration::eScalarEntityTemperatureFeature:
        return Configuration::sEntityTemperature;
        break;
    case Configuration::eScalarEntityMeasurementSeriesFeature:
        return Configuration::sEntityMeasurementSeries;
        break;
    default:
        return "";
        break;
    }
}
