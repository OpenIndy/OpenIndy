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
QString Configuration::sTemperatur = "temperatur";
QString Configuration::sLevel = "level";

QString Configuration::sObservation = "observation";

QString Configuration::sSteel = "steel";
QString Configuration::sAluminum = "aluminum";
QString Configuration::sPlumb = "plumb";
QString Configuration::sIron = "iron";
QString Configuration::sGrayCastIron = "gray cast iron";
QString Configuration::sCopper = "copper";
QString Configuration::sBrass = "brass";
QString Configuration::sZinc = "zinc";
QString Configuration::sPlatinum = "platinum";

/*!
 * \brief expansion coefficients in meter unit.
 */
double Configuration::dSteel = 0.000016;
double Configuration::dAluminum = 0.000024;
double Configuration::dPlumb = 0.000030;
double Configuration::dIron = 0.000012;
double Configuration::dGrayCastIron = 0.000009;
double Configuration::dCopper = 0.000017;
double Configuration::dBrass = 0.000018;
double Configuration::dZinc = 0.000027;
double Configuration::dPlatinum = 0.000009;

int Configuration::idCount = 0;

int Configuration::generateID(){

    Configuration::idCount  += 1;
    return Configuration::idCount;
}

Configuration::ElementTypes Configuration::getElementTypeEnum(QString s){
    if(s.compare(Configuration::sPlane) == 0){
        return Configuration::ePlaneElement;
    }else if(s.compare(Configuration::sPoint) == 0){
        return Configuration::ePointElement;
    }else if(s.compare(Configuration::sLine) == 0){
        return Configuration::eLineElement;
    }else if(s.compare(Configuration::sStation) == 0){
        return Configuration::eStationElement;
    }else if(s.compare(Configuration::sCoordinatesystem) == 0){
        return Configuration::eCoordinateSystemElement;
    }else if(s.compare(Configuration::sSphere) == 0){
        return Configuration::eSphereElement;
    }else if(s.compare(Configuration::sEntitiyAngle) == 0){
        return Configuration::eScalarEntityAngleElement;
    }else if(s.compare(Configuration::sEntityDistance) == 0){
        return Configuration::eScalarEntityDistanceElement;
    }else if(s.compare(Configuration::sTrafoParam) == 0){
        return Configuration::eTrafoParamElement;
    }else if(s.compare(Configuration::sCircle) == 0){
        return Configuration::eCircleElement;
    }else if(s.compare(Configuration::sCone) == 0){
        return Configuration::eConeElement;
    }else if(s.compare(Configuration::sCylinder) == 0){
        return Configuration::eCylinderElement;
    }else if(s.compare(Configuration::sEllipsoid) == 0){
        return Configuration::eEllipsoidElement;
    }else if(s.compare(Configuration::sHyperboloid) == 0){
        return Configuration::eHyperboloidElement;
    }else if(s.compare(Configuration::sParaboloid) == 0){
        return Configuration::eParaboloidElement;
    }else if(s.compare(Configuration::sNurbs) == 0){
        return Configuration::eNurbsElement;
    }else if(s.compare(Configuration::sPointCloud) == 0){
        return Configuration::ePointCloudElement;
    }else if(s.compare(Configuration::sObservation) == 0){
        return Configuration::eObservationElement;
    }else if(s.compare(Configuration::sEntityTemperature) == 0){
        return Configuration::eScalarEntityTemperatureElement;
    }
}

/*!
 * \brief Configuration::getFeatureTypeEnum
 * \param s
 * \return
 */
Configuration::FeatureTypes Configuration::getFeatureTypeEnum(QString s){
    if(s.compare(Configuration::sPlane) == 0){
        return Configuration::ePlaneFeature;
    }else if(s.compare(Configuration::sPoint) == 0){
        return Configuration::ePointFeature;
    }else if(s.compare(Configuration::sLine) == 0){
        return Configuration::eLineFeature;
    }else if(s.compare(Configuration::sStation) == 0){
        return Configuration::eStationFeature;
    }else if(s.compare(Configuration::sCoordinatesystem) == 0){
        return Configuration::eCoordinateSystemFeature;
    }else if(s.compare(Configuration::sSphere) == 0){
        return Configuration::eSphereFeature;
    }else if(s.compare(Configuration::sEntitiyAngle) == 0){
        return Configuration::eScalarentityAngleFeature;
    }else if(s.compare(Configuration::sEntityDistance) == 0){
        return Configuration::eScalarEntityDistanceFeature;
    }else if(s.compare(Configuration::sTrafoParam) == 0){
        return Configuration::eTrafoParamFeature;
    }else if(s.compare(Configuration::sCircle) == 0){
        return Configuration::eCircleFeature;
    }else if(s.compare(Configuration::sCone) == 0){
        return Configuration::eConeFeature;
    }else if(s.compare(Configuration::sCylinder) == 0){
        return Configuration::eCylinderFeature;
    }else if(s.compare(Configuration::sEllipsoid) == 0){
        return Configuration::eEllipsoidFeature;
    }else if(s.compare(Configuration::sHyperboloid) == 0){
        return Configuration::eHyperboloidFeature;
    }else if(s.compare(Configuration::sParaboloid) == 0){
        return Configuration::eParaboloidFeature;
    }else if(s.compare(Configuration::sNurbs) == 0){
        return Configuration::eNurbsFeature;
    }else if(s.compare(Configuration::sPointCloud) == 0){
        return Configuration::ePointCloudFeature;
    }else if(s.compare(Configuration::sEntityTemperature) == 0){
        return Configuration::eScalarEntityTemperatureFeature;
    }
}

QString Configuration::getElementTypeString(Configuration::ElementTypes e){
    switch (e) {
    case Configuration::ePlaneElement:
        return Configuration::sPlane;
        break;
    case Configuration::ePointElement:
        return Configuration::sPoint ;
        break;
    case Configuration::eLineElement:
        return Configuration::sLine;
        break;
    case Configuration::eStationElement:
        return Configuration::sStation;
        break;
    case Configuration::eCoordinateSystemElement:
        return Configuration::sCoordinatesystem;
        break;
    case Configuration::eSphereElement:
        return Configuration::sSphere;
        break;
    case Configuration::eScalarEntityAngleElement:
        return Configuration::sEntitiyAngle;
        break;
    case Configuration::eScalarEntityDistanceElement:
        return Configuration::sEntityDistance;
         break;
    case Configuration::eTrafoParamElement:
        return Configuration::sTrafoParam;
        break;
    case Configuration::eCircleElement:
        return Configuration::sCircle;
        break;
    case Configuration::eConeElement:
        return Configuration::sCone;
        break;
    case Configuration::eCylinderElement:
        return Configuration::sCylinder;
        break;
    case Configuration::eEllipsoidElement:
        return Configuration::sEllipsoid;
        break;
    case Configuration::eHyperboloidElement:
        return Configuration::sHyperboloid;
        break;
    case Configuration::eParaboloidElement:
        return Configuration::sParaboloid;
        break;
    case Configuration::eNurbsElement:
        return Configuration::sNurbs;
        break;
    case Configuration::ePointCloudElement:
        return Configuration::sPointCloud;
        break;
    case Configuration::eObservationElement:
        return Configuration::sObservation;
        break;
    case Configuration::eScalarEntityTemperatureElement:
        return Configuration::sEntityTemperature;
        break;
    default:
        return "";
        break;
    }
}

/*!
 * \brief getExpansionCoefficient returns the expansion coefficient of the material.
 * \param material
 * \return
 */
double Configuration::getExpansionCoefficient(QString material)
{
    if(material.compare(Configuration::sSteel) == 0){
        return Configuration::dSteel;
    }
    if(material.compare(Configuration::sAluminum) == 0){
        return Configuration::dAluminum;
    }
    if(material.compare(Configuration::sPlumb) == 0){
        return Configuration::dPlumb;
    }
    if(material.compare(Configuration::sIron) == 0){
        return Configuration::dIron;
    }
    if(material.compare(Configuration::sGrayCastIron) == 0){
        return Configuration::dGrayCastIron;
    }
    if(material.compare(Configuration::sCopper) == 0){
        return Configuration::dCopper;
    }
    if(material.compare(Configuration::sBrass) == 0){
        return Configuration::dBrass;
    }
    if(material.compare(Configuration::sZinc) == 0){
        return Configuration::dZinc;
    }
    if(material.compare(Configuration::sPlatinum) == 0){
        return Configuration::dPlatinum;
    }
}

/*!
 * \brief getExpansionCoefficient returns the expansion coefficient of the material.
 * \return
 */
double Configuration::getExpansionCoefficient(Configuration::Materials material)
{
    switch (material) {
    case Configuration::eSteel:
        return Configuration::dSteel;
        break;
    case Configuration::eAluminum:
        return Configuration::dAluminum;
        break;
    case Configuration::ePlumb:
        return Configuration::dPlumb;
        break;
    case Configuration::eIron:
        return Configuration::dIron;
        break;
    case Configuration::eGrayCastIron:
        return Configuration::dGrayCastIron;
        break;
    case Configuration::eCopper:
        return Configuration::dCopper;
        break;
    case Configuration::eBrass:
        return Configuration::dBrass;
        break;
    case Configuration::eZinc:
        return Configuration::dZinc;
        break;
    case Configuration::ePlatinum:
        return Configuration::dPlatinum;
        break;
    default:
        break;
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
    case Configuration::eScalarentityAngleFeature:
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
