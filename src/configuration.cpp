#include "configuration.h"

Configuration::Configuration(QObject *parent) :
    QObject(parent)
{

}
/*
Configuration::Configuration()
{
}
*/
Configuration::~Configuration()
{
}

QString Configuration::sPlane = "plane";
QString Configuration::sPoint = "point";
QString Configuration::sLine = "line";
QString Configuration::sStation = "station";
QString Configuration::sCoordinatesystem = "coordinatesystem";
QString Configuration::sSphere = "sphere";
QString Configuration::sEntitiyAngle = "sEntitiy angle";
QString Configuration::sEntityDistance = "sEntity distance";
QString Configuration::sTrafoParam = "trafoParam";
QString Configuration::sCircle = "circle";
QString Configuration::sCone = "cone";
QString Configuration::sCylinder = "cylinder";
QString Configuration::sEllipsoid = "ellipsoid";
QString Configuration::sHyperboloid = "hyperboloid";
QString Configuration::sParaboloid = "paraboloid";
QString Configuration::sPointCloud = "pointcloud";
QString Configuration::sNurbs = "nurbs";
QString Configuration::sEntityTemperature = "sEntity temperature";
QString Configuration::sEntityMeasurementSeries = "sEntity measurement series";

QString Configuration::sDistance = "distance";
QString Configuration::sCartesian = "cartesian";
QString Configuration::sPolar = "polar";
QString Configuration::sDirection = "direction";
QString Configuration::sTemperatur = "temperature";
QString Configuration::sLevel = "level";

QString Configuration::sObservation = "observation";

int Configuration::idCount = 0;

int Configuration::generateID(){

    Configuration::idCount  += 1;
    return Configuration::idCount;
}

/*!
 * \brief Configuration::getElementTypeEnum
 * Get corresponding enum value from element type name
 * \param name
 * \return
 */
Configuration::ElementTypes Configuration::getElementTypeEnum(QString name){
    if(name.compare(Configuration::sPlane) == 0){
        return Configuration::ePlaneElement;
    }else if(name.compare(Configuration::sPoint) == 0){
        return Configuration::ePointElement;
    }else if(name.compare(Configuration::sLine) == 0){
        return Configuration::eLineElement;
    }else if(name.compare(Configuration::sStation) == 0){
        return Configuration::eStationElement;
    }else if(name.compare(Configuration::sCoordinatesystem) == 0){
        return Configuration::eCoordinateSystemElement;
    }else if(name.compare(Configuration::sSphere) == 0){
        return Configuration::eSphereElement;
    }else if(name.compare(Configuration::sEntitiyAngle) == 0){
        return Configuration::eScalarEntityAngleElement;
    }else if(name.compare(Configuration::sEntityDistance) == 0){
        return Configuration::eScalarEntityDistanceElement;
    }else if(name.compare(Configuration::sTrafoParam) == 0){
        return Configuration::eTrafoParamElement;
    }else if(name.compare(Configuration::sCircle) == 0){
        return Configuration::eCircleElement;
    }else if(name.compare(Configuration::sCone) == 0){
        return Configuration::eConeElement;
    }else if(name.compare(Configuration::sCylinder) == 0){
        return Configuration::eCylinderElement;
    }else if(name.compare(Configuration::sEllipsoid) == 0){
        return Configuration::eEllipsoidElement;
    }else if(name.compare(Configuration::sHyperboloid) == 0){
        return Configuration::eHyperboloidElement;
    }else if(name.compare(Configuration::sParaboloid) == 0){
        return Configuration::eParaboloidElement;
    }else if(name.compare(Configuration::sNurbs) == 0){
        return Configuration::eNurbsElement;
    }else if(name.compare(Configuration::sPointCloud) == 0){
        return Configuration::ePointCloudElement;
    }else if(name.compare(Configuration::sObservation) == 0){
        return Configuration::eObservationElement;
    }else if(name.compare(Configuration::sEntityTemperature) == 0){
        return Configuration::eScalarEntityTemperatureElement;
    }else if(name.compare(Configuration::sDirection) == 0){
        return Configuration::eReadingDirectionElement;
    }else if(name.compare(Configuration::sDistance) == 0){
        return Configuration::eReadingDistanceElement;
    }else if(name.compare(Configuration::sCartesian) == 0){
        return Configuration::eReadingCartesianElement;
    }else if(name.compare(Configuration::sPolar) == 0){
        return Configuration::eReadingPolarElement;
    }else if(name.compare(Configuration::sTemperatur) == 0){
        return Configuration::eReadingTemperatureElement;
    }else if(name.compare(Configuration::sLevel) == 0){
        return Configuration::eReadingLevelElement;
    }else if(name.compare(Configuration::sEntityMeasurementSeries) == 0){
        return Configuration::eScalarEntityMeasurementSeriesElement;
    }
}

/*!
 * \brief Configuration::getFeatureTypeEnum
 * Get corresponding enum value from feature type name
 * \param s
 * \return
 */
Configuration::FeatureTypes Configuration::getFeatureTypeEnum(QString name){
    if(name.compare(Configuration::sPlane) == 0){
        return Configuration::ePlaneFeature;
    }else if(name.compare(Configuration::sPoint) == 0){
        return Configuration::ePointFeature;
    }else if(name.compare(Configuration::sLine) == 0){
        return Configuration::eLineFeature;
    }else if(name.compare(Configuration::sStation) == 0){
        return Configuration::eStationFeature;
    }else if(name.compare(Configuration::sCoordinatesystem) == 0){
        return Configuration::eCoordinateSystemFeature;
    }else if(name.compare(Configuration::sSphere) == 0){
        return Configuration::eSphereFeature;
    }else if(name.compare(Configuration::sEntitiyAngle) == 0){
        return Configuration::eScalarEntityAngleFeature;
    }else if(name.compare(Configuration::sEntityDistance) == 0){
        return Configuration::eScalarEntityDistanceFeature;
    }else if(name.compare(Configuration::sTrafoParam) == 0){
        return Configuration::eTrafoParamFeature;
    }else if(name.compare(Configuration::sCircle) == 0){
        return Configuration::eCircleFeature;
    }else if(name.compare(Configuration::sCone) == 0){
        return Configuration::eConeFeature;
    }else if(name.compare(Configuration::sCylinder) == 0){
        return Configuration::eCylinderFeature;
    }else if(name.compare(Configuration::sEllipsoid) == 0){
        return Configuration::eEllipsoidFeature;
    }else if(name.compare(Configuration::sHyperboloid) == 0){
        return Configuration::eHyperboloidFeature;
    }else if(name.compare(Configuration::sParaboloid) == 0){
        return Configuration::eParaboloidFeature;
    }else if(name.compare(Configuration::sNurbs) == 0){
        return Configuration::eNurbsFeature;
    }else if(name.compare(Configuration::sPointCloud) == 0){
        return Configuration::ePointCloudFeature;
    }else if(name.compare(Configuration::sEntityTemperature) == 0){
        return Configuration::eScalarEntityTemperatureFeature;
    }else if(name.compare(Configuration::sEntityMeasurementSeries) == 0){
        return Configuration::eScalarEntityMeasurementSeriesFeature;
    }
}

/*!
 * \brief Configuration::getReadingTypeEnum
 * \param name
 * \return
 */
Configuration::ReadingTypes Configuration::getReadingTypeEnum(QString name){
    if(name.compare(Configuration::sCartesian) == 0){
        return Configuration::eCartesian;
    }else if(name.compare(Configuration::sDistance) == 0){
        return Configuration::eDistance;
    }else if(name.compare(Configuration::sDirection) == 0){
        return Configuration::eDirection;
    }else if(name.compare(Configuration::sPolar) == 0){
        return Configuration::ePolar;
    }else if(name.compare(Configuration::sTemperatur) == 0){
        return Configuration::eTemperatur;
    }else if(name.compare(Configuration::sLevel) == 0){
        return Configuration::eLevel;
    }else if(name.compare("") == 0){
        return Configuration::eUndefined;
    }
}

/*!
 * \brief Configuration::getElementTypeString
 * Get element type name from corresponding enum value
 * \param type
 * \return
 */
QString Configuration::getElementTypeString(Configuration::ElementTypes type){
    switch (type) {
    case Configuration::ePlaneElement:
        return Configuration::sPlane;
    case Configuration::ePointElement:
        return Configuration::sPoint ;
    case Configuration::eLineElement:
        return Configuration::sLine;
    case Configuration::eStationElement:
        return Configuration::sStation;
    case Configuration::eCoordinateSystemElement:
        return Configuration::sCoordinatesystem;
    case Configuration::eSphereElement:
        return Configuration::sSphere;
    case Configuration::eScalarEntityAngleElement:
        return Configuration::sEntitiyAngle;
    case Configuration::eScalarEntityDistanceElement:
        return Configuration::sEntityDistance;
    case Configuration::eTrafoParamElement:
        return Configuration::sTrafoParam;
    case Configuration::eCircleElement:
        return Configuration::sCircle;
    case Configuration::eConeElement:
        return Configuration::sCone;
    case Configuration::eCylinderElement:
        return Configuration::sCylinder;
    case Configuration::eEllipsoidElement:
        return Configuration::sEllipsoid;
    case Configuration::eHyperboloidElement:
        return Configuration::sHyperboloid;
    case Configuration::eParaboloidElement:
        return Configuration::sParaboloid;
    case Configuration::eNurbsElement:
        return Configuration::sNurbs;
    case Configuration::ePointCloudElement:
        return Configuration::sPointCloud;
    case Configuration::eObservationElement:
        return Configuration::sObservation;
    case Configuration::eScalarEntityTemperatureElement:
        return Configuration::sEntityTemperature;
    case Configuration::eReadingDistanceElement:
        return Configuration::sDistance;
    case Configuration::eReadingCartesianElement:
        return Configuration::sCartesian;
    case Configuration::eReadingPolarElement:
        return Configuration::sPolar;
    case Configuration::eReadingDirectionElement:
        return Configuration::sDirection;
    case Configuration::eReadingLevelElement:
        return Configuration::sLevel;
    case Configuration::eReadingTemperatureElement:
        return Configuration::sTemperatur;
    case Configuration::eScalarEntityMeasurementSeriesElement:
        return Configuration::sEntityMeasurementSeries;
    default:
        return "";
    }
}

/*!
 * \brief Configuration::getFeatureTypeString
 * Get feature type name from corresponding enum value
 * \param type
 * \return
 */
QString Configuration::getFeatureTypeString(Configuration::FeatureTypes type){
    switch (type) {
    case Configuration::ePlaneFeature:
        return Configuration::sPlane;
    case Configuration::ePointFeature:
        return Configuration::sPoint ;
    case Configuration::eLineFeature:
        return Configuration::sLine;
    case Configuration::eStationFeature:
        return Configuration::sStation;
    case Configuration::eCoordinateSystemFeature:
        return Configuration::sCoordinatesystem;
    case Configuration::eSphereFeature:
        return Configuration::sSphere;
    case Configuration::eScalarEntityAngleFeature:
        return Configuration::sEntitiyAngle;
    case Configuration::eScalarEntityDistanceFeature:
        return Configuration::sEntityDistance;
    case Configuration::eTrafoParamFeature:
        return Configuration::sTrafoParam;
    case Configuration::eCircleFeature:
        return Configuration::sCircle;
    case Configuration::eConeFeature:
        return Configuration::sCone;
    case Configuration::eCylinderFeature:
        return Configuration::sCylinder;
    case Configuration::eEllipsoidFeature:
        return Configuration::sEllipsoid;
    case Configuration::eHyperboloidFeature:
        return Configuration::sHyperboloid;
    case Configuration::eParaboloidFeature:
        return Configuration::sParaboloid;
    case Configuration::eNurbsFeature:
        return Configuration::sNurbs;
    case Configuration::ePointCloudFeature:
        return Configuration::sPointCloud;
    case Configuration::eScalarEntityTemperatureFeature:
        return Configuration::sEntityTemperature;
    case Configuration::eScalarEntityMeasurementSeriesFeature:
        return Configuration::sEntityMeasurementSeries;
    default:
        return "";
    }
}

QString Configuration::getReadingTypeString(Configuration::ReadingTypes type){
    switch (type) {
    case Configuration::eCartesian:
        return Configuration::sCartesian;
    case Configuration::eDistance:
        return Configuration::sDistance;
    case Configuration::eDirection:
        return Configuration::sDirection;
    case Configuration::ePolar:
        return Configuration::sPolar;
    case Configuration::eLevel:
        return Configuration::sLevel;
    case Configuration::eUndefined:
        return "";
    case Configuration::eTemperatur:
        return Configuration::sTemperatur;
    default:
        return "";
    }
}

/*!
 * \brief Configuration::getFeatureIcon
 * \param featureType
 * \return
 */
QPixmap Configuration::getFeatureIcon(Configuration::FeatureTypes featureType){

    switch(featureType){
    case Configuration::ePointFeature:
        return QPixmap(":/Images/icons/point_5e8acf.png");
    case Configuration::ePlaneFeature:
        return QPixmap(":/Images/icons/plane_5e8acf.png");
    case Configuration::eLineFeature:
        return QPixmap(":/Images/icons/line_5e8acf.png");
    case Configuration::eSphereFeature:
        return QPixmap(":/Images/icons/sphere_5e8acf.png");
    case Configuration::eHyperboloidFeature:
        return QPixmap(":/Images/icons/hyperboloid_5e8acf.png");
    case Configuration::eParaboloidFeature:
        return QPixmap(":/Images/icons/paraboloid_5e8acf.png");
    case Configuration::eEllipsoidFeature:
        return QPixmap(":/Images/icons/ellipse_5e8acf.png");
    case Configuration::ePointCloudFeature:
        return QPixmap(":/Images/icons/pointCloud_5e8acf.png");
    case Configuration::eStationFeature:
        return QPixmap(":/Images/icons/station.png");
    case Configuration::eTrafoParamFeature:
        return QPixmap(":/Images/icons/trafoParam.png");
    case Configuration::eCoordinateSystemFeature:
        return QPixmap(":/Images/icons/coordinateSystem.png");
    case Configuration::eCircleFeature:
        return QPixmap(":/Images/icons/circle_5e8acf.png");
    case Configuration::eCylinderFeature:
        return QPixmap(":/Images/icons/cylinder_5e8acf.png");
    case Configuration::eConeFeature:
        return QPixmap(":/Images/icons/cone_5e8acf.png");
    case Configuration::eScalarEntityAngleFeature:
        return QPixmap(":/Images/icons/scalarEntities_5e8acf.png");
    case Configuration::eScalarEntityDistanceFeature:
        return QPixmap(":/Images/icons/scalarEntities_5e8acf.png");
    case Configuration::eScalarEntityMeasurementSeriesFeature:
        return QPixmap(":/Images/icons/scalarEntities_5e8acf.png");
    case Configuration::eScalarEntityTemperatureFeature:
        return QPixmap(":/Images/icons/scalarEntities_5e8acf.png");
    case Configuration::eNurbsFeature:
        return QPixmap(":/Images/icons/nurbs_5e8acf.png");
    }

    return QPixmap("");

}

/*!
 * \brief Configuration::getElementIcon
 * \param elementType
 * \return
 */
QPixmap Configuration::getElementIcon(Configuration::ElementTypes elementType){

    QPixmap result;

    switch(elementType){
    case Configuration::ePointElement:
        return QPixmap(":/Images/icons/point_5e8acf.png");
    case Configuration::ePlaneElement:
        return QPixmap(":/Images/icons/plane_5e8acf.png");
    case Configuration::eLineElement:
        return QPixmap(":/Images/icons/line_5e8acf.png");
    case Configuration::eSphereElement:
        return QPixmap(":/Images/icons/sphere_5e8acf.png");
    case Configuration::eHyperboloidElement:
        return QPixmap(":/Images/icons/hyperboloid_5e8acf.png");
    case Configuration::eParaboloidElement:
        return QPixmap(":/Images/icons/paraboloid_5e8acf.png");
    case Configuration::eEllipsoidElement:
        return QPixmap(":/Images/icons/ellipse_5e8acf.png");
    case Configuration::ePointCloudElement:
        return QPixmap(":/Images/icons/pointCloud_5e8acf.png");
    case Configuration::eStationElement:
        return QPixmap(":/Images/icons/station.png");
    case Configuration::eTrafoParamElement:
        return QPixmap(":/Images/icons/trafoParam.png");
    case Configuration::eCoordinateSystemElement:
        return QPixmap(":/Images/icons/coordinateSystem.png");
    case Configuration::eCircleElement:
        return QPixmap(":/Images/icons/circle_5e8acf.png");
    case Configuration::eCylinderElement:
        return QPixmap(":/Images/icons/cylinder_5e8acf.png");
    case Configuration::eConeElement:
        return QPixmap(":/Images/icons/cone_5e8acf.png");
    case Configuration::eScalarEntityAngleElement:
        return QPixmap(":/Images/icons/scalarEntities_5e8acf.png");
    case Configuration::eScalarEntityDistanceElement:
        return QPixmap(":/Images/icons/scalarEntities_5e8acf.png");
    case Configuration::eScalarEntityTemperatureElement:
        return QPixmap(":/Images/icons/scalarEntities_5e8acf.png");
    case Configuration::eNurbsElement:
        return QPixmap(":/Images/icons/nurbs_5e8acf.png");
    case Configuration::eObservationElement:
        return QPixmap("");
    case Configuration::eReadingCartesianElement:
        return QPixmap("");
    case Configuration::eReadingDirectionElement:
        return QPixmap("");
    case Configuration::eReadingDistanceElement:
        return QPixmap("");
    case Configuration::eReadingPolarElement:
        return QPixmap("");
    }

    return QPixmap("");

}
