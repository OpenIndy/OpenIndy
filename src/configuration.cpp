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

int Configuration::idCount = 0;

int Configuration::generateID(){

    Configuration::idCount  += 1;
    return Configuration::idCount;
}

Configuration::ElementTypes Configuration::getElementTypeEnum(QString s){
    if(s == Configuration::sPlane){
        return Configuration::ePlaneElement;
    }else if(s == Configuration::sPoint){
        return Configuration::ePointElement;
    }else if(s == Configuration::sLine){
        return Configuration::eLineElement;
    }else if(s == Configuration::sStation){
        return Configuration::eStationElement;
    }else if(s == Configuration::sCoordinatesystem){
        return Configuration::eCoordinateSystemElement;
    }else if(s == Configuration::sSphere){
        return Configuration::eSphereElement;
    }else if(s == Configuration::sEntitiyAngle){
        return Configuration::eScalarEntityAngleElement;
    }else if(s == Configuration::sEntityDistance){
        return Configuration::eScalarEntityDistanceElement;
    }else if(s == Configuration::sTrafoParam){
        return Configuration::eTrafoParamElement;
    }else if(s == Configuration::sCircle){
        return Configuration::eCircleElement;
    }else if(s == Configuration::sCone){
        return Configuration::eConeElement;
    }else if(s == Configuration::sCylinder){
        return Configuration::eCylinderElement;
    }else if(s == Configuration::sEllipsoid){
        return Configuration::eEllipsoidElement;
    }else if(s == Configuration::sHyperboloid){
        return Configuration::eHyperboloidElement;
    }else if(s == Configuration::sParaboloid){
        return Configuration::eParaboloidElement;
    }else if(s == Configuration::sNurbs){
        return Configuration::eNurbsElement;
    }else if(s == Configuration::sPointCloud){
        return Configuration::ePointCloudElement;
    }else if(s == Configuration::sObservation){
        return Configuration::eObservationElement;
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
    default:
        return "";
        break;
    }
}