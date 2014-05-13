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

QStringList Configuration::allAttributes;
QList<DisplayAttribute*> Configuration::featureAttributes;
QList<DisplayAttribute> Configuration::trafoParamAttributes;

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

/*!
 * \brief generateAllAttributes fills the QStringList with all existing attribute names.
 * This list is later used for the tableview model.
 * Call this function at project beginning and after every change in unit setting!
 */
void Configuration::generateAllAttributes()
{
    Configuration::allAttributes.clear();

    Configuration::allAttributes.append("feature type");
    Configuration::allAttributes.append("feature name");
    Configuration::allAttributes.append("group");
    Configuration::allAttributes.append(QString("x" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append(QString("y" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append(QString("z" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append("observations");
    Configuration::allAttributes.append("solved");
    Configuration::allAttributes.append("measurement config");
    Configuration::allAttributes.append("functions");
    Configuration::allAttributes.append("common point");
    Configuration::allAttributes.append("nominal");
    Configuration::allAttributes.append(QString("stddev" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append("I");
    Configuration::allAttributes.append("J");
    Configuration::allAttributes.append("K");
    Configuration::allAttributes.append(QString("radius" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append("scalar value [m]");
    Configuration::allAttributes.append("scalar value [dec. degree]");
    Configuration::allAttributes.append("start");
    Configuration::allAttributes.append("destination");
    Configuration::allAttributes.append(QString("translation x" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append(QString("translation y" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append(QString("translation z" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append(QString("rotation x" + UnitConverter::getAngleUnitString()));
    Configuration::allAttributes.append(QString("rotation y" + UnitConverter::getAngleUnitString()));
    Configuration::allAttributes.append(QString("rotation z" + UnitConverter::getAngleUnitString()));
    Configuration::allAttributes.append("scale x");
    Configuration::allAttributes.append("scale y");
    Configuration::allAttributes.append("scale z");
    Configuration::allAttributes.append(QString("temperature" + UnitConverter::getTemperatureUnitString()));
    Configuration::allAttributes.append("measurement series");
    Configuration::allAttributes.append("comment");
}

/*!
 * \brief generateFeatureAttributes fills the QMap with all available attributes for features except trafo parameter.
 * Also includes a boolean that handles if the attributes is displayed or not.
 */
void Configuration::generateFeatureAttributes()
{
    Configuration::featureAttributes.clear();

    DisplayAttribute *ftype = new DisplayAttribute("feature type",true);
    Configuration::featureAttributes.append(ftype);
    DisplayAttribute *fname = new DisplayAttribute("feature name",true);
    Configuration::featureAttributes.append(fname);
    DisplayAttribute *fgroup = new DisplayAttribute("group",true);
    Configuration::featureAttributes.append(fgroup);
    DisplayAttribute *fX = new DisplayAttribute(QString("x"+UnitConverter::getDistanceUnitString()),true);
    Configuration::featureAttributes.append(fX);
    DisplayAttribute *fY = new DisplayAttribute(QString("y"+UnitConverter::getDistanceUnitString()),true);
    Configuration::featureAttributes.append(fY);
    DisplayAttribute *fZ = new DisplayAttribute(QString("z"+UnitConverter::getDistanceUnitString()),true);
    Configuration::featureAttributes.append(fZ);
    DisplayAttribute *fobs = new DisplayAttribute("observation",true);
    Configuration::featureAttributes.append(fobs);
    DisplayAttribute *fSolved = new DisplayAttribute("solved",true);
    Configuration::featureAttributes.append(fSolved);
    DisplayAttribute *fMConfig = new DisplayAttribute("measurement config",true);
    Configuration::featureAttributes.append(fMConfig);
    DisplayAttribute *fFunctions = new DisplayAttribute("functions",true);
    Configuration::featureAttributes.append(fFunctions);
    DisplayAttribute *fcomPoint = new DisplayAttribute("common point",true);
    Configuration::featureAttributes.append(fcomPoint);
    DisplayAttribute *fNominal = new DisplayAttribute("nominal",true);
    Configuration::featureAttributes.append(fNominal);
    DisplayAttribute *fStddev = new DisplayAttribute(QString("stddev"+UnitConverter::getDistanceUnitString()),true);
    Configuration::featureAttributes.append(fStddev);
    DisplayAttribute *fI = new DisplayAttribute("I",true);
    Configuration::featureAttributes.append(fI);
    DisplayAttribute *fJ = new DisplayAttribute("J",true);
    Configuration::featureAttributes.append(fJ);
    DisplayAttribute *fK = new DisplayAttribute("K",true);
    Configuration::featureAttributes.append(fK);
    DisplayAttribute *fRadius = new DisplayAttribute(QString("radius"+UnitConverter::getDistanceUnitString()),true);
    Configuration::featureAttributes.append(fRadius);
    DisplayAttribute *fscalarD = new DisplayAttribute("scalar value [m]",true);
    Configuration::featureAttributes.append(fscalarD);
    DisplayAttribute *fscalarA = new DisplayAttribute("scalar value [dec.degree]",true);
    Configuration::featureAttributes.append(fscalarA);
    DisplayAttribute *fTemperature = new DisplayAttribute(QString("temperature"+UnitConverter::getTemperatureUnitString()),true);
    Configuration::featureAttributes.append(fTemperature);
    DisplayAttribute *fMeasSeries = new DisplayAttribute("measurement series",true);
    Configuration::featureAttributes.append(fMeasSeries);
    DisplayAttribute *fcomment = new DisplayAttribute("comment",true);
    Configuration::featureAttributes.append(fcomment);
}

/*!
 * \brief generateTrafoParamAttributes fills the QMap with all available attributes for trafo parameter.
 * Also includes a boolean that handles if the attributes is displayed or not.
 */
void Configuration::generateTrafoParamAttributes()
{/*
    Configuration::trafoParamAttributes.append("feature type",true);
    Configuration::trafoParamAttributes.append("feature name",true);
    Configuration::trafoParamAttributes.append("group",true);
    Configuration::trafoParamAttributes.append("solved",true);
    Configuration::trafoParamAttributes.append("functions",true);
    Configuration::trafoParamAttributes.append("start",true);
    Configuration::trafoParamAttributes.append("destination",true);
    Configuration::trafoParamAttributes.append("translation x",true);
    Configuration::trafoParamAttributes.append("translation y",true);
    Configuration::trafoParamAttributes.append("translation z",true);
    Configuration::trafoParamAttributes.append("rotation x",true);
    Configuration::trafoParamAttributes.append("rotation y",true);
    Configuration::trafoParamAttributes.append("rotation z",true);
    Configuration::trafoParamAttributes.append("scale x",true);
    Configuration::trafoParamAttributes.append("scale y",true);
    Configuration::trafoParamAttributes.append("scale z",true);
    Configuration::trafoParamAttributes.append("comment",true);
*/}

/*!
 * \brief displayAttributes checks which attributes should be displayed. returns the column indices for this attributes.
 * \param attributes
 * \return
 */
QList<int> Configuration::displayAttributes(QList<DisplayAttribute*> attributes)
{
    QList<int> display;

    for(int i =0; i<attributes.size();i++){
        if(attributes.at(i)->displayState == true){
            int res = Configuration::allAttributes.indexOf(attributes.at(i)->attrName);
            if(res != -1){
                display.append(res);
            }
        }
    }
    return display;
}
