#include "util.h"

//#################################################
//maps with element enums and corresponding strings
//#################################################

namespace internal{

QMap<ElementTypes, QString> elementTypesMap;
QMap<ElementTypes, QString> elementTypesPluralMap;
QMap<FeatureTypes, QString> featureTypesMap;
QMap<GeometryTypes, QString> geometryTypesMap;

QMap<ReadingTypes, QString> readingTypesMap;

QMap<FeatureDisplayAttributes, QString> featureDisplayAttributesMap;
QMap<TrafoParamDisplayAttributes, QString> trafoParamDisplayAttributesMap;

QList<int> displayAttributes;

QMap<UnitType, QString> unitTypesMap;

QString undefined = "";

bool isInit = false;

void init(){

    //clear maps
    elementTypesMap.clear();
    elementTypesPluralMap.clear();
    featureTypesMap.clear();
    geometryTypesMap.clear();
    readingTypesMap.clear();
    featureDisplayAttributesMap.clear();
    trafoParamDisplayAttributesMap.clear();
    displayAttributes.clear();
    unitTypesMap.clear();

    //fill element map
    elementTypesMap.insert(eCircleElement, "circle");
    elementTypesMap.insert(eConeElement, "cone");
    elementTypesMap.insert(eCylinderElement, "cylinder");
    elementTypesMap.insert(eEllipseElement, "ellipse");
    elementTypesMap.insert(eEllipsoidElement, "ellipsoid");
    elementTypesMap.insert(eHyperboloidElement, "hyperboloid");
    elementTypesMap.insert(eLineElement, "line");
    elementTypesMap.insert(eNurbsElement, "nurbs");
    elementTypesMap.insert(eParaboloidElement, "paraboloid");
    elementTypesMap.insert(ePlaneElement, "plane");
    elementTypesMap.insert(ePointElement, "point");
    elementTypesMap.insert(ePointCloudElement, "point cloud");
    elementTypesMap.insert(eScalarEntityAngleElement, "angle");
    elementTypesMap.insert(eScalarEntityDistanceElement, "distance");
    elementTypesMap.insert(eScalarEntityMeasurementSeriesElement, "measurement series");
    elementTypesMap.insert(eScalarEntityTemperatureElement, "temperature");
    elementTypesMap.insert(eSlottedHoleElement, "slotted hole");
    elementTypesMap.insert(eSphereElement, "sphere");
    elementTypesMap.insert(eTorusElement, "torus");
    elementTypesMap.insert(eDirectionElement, "direction");
    elementTypesMap.insert(ePositionElement, "position");
    elementTypesMap.insert(eRadiusElement, "radius");
    elementTypesMap.insert(eCoordinateSystemElement, "coordinate system");
    elementTypesMap.insert(eStationElement, "station");
    elementTypesMap.insert(eTrafoParamElement, "transformation parameters");
    elementTypesMap.insert(eObservationElement, "observation");
    elementTypesMap.insert(eReadingCartesianElement, "cartesian reading");
    elementTypesMap.insert(eReadingPolarElement, "polar reading");
    elementTypesMap.insert(eReadingDistanceElement, "distance reading");
    elementTypesMap.insert(eReadingDirectionElement, "direction reading");
    elementTypesMap.insert(eReadingTemperatureElement, "temperature reading");
    elementTypesMap.insert(eReadingLevelElement, "level reading");

    //fill element plural map
    elementTypesPluralMap.insert(eCircleElement, "circles");
    elementTypesPluralMap.insert(eConeElement, "cones");
    elementTypesPluralMap.insert(eCylinderElement, "cylinders");
    elementTypesPluralMap.insert(eEllipseElement, "ellipses");
    elementTypesPluralMap.insert(eEllipsoidElement, "ellipsoids");
    elementTypesPluralMap.insert(eHyperboloidElement, "hyperboloids");
    elementTypesPluralMap.insert(eLineElement, "lines");
    elementTypesPluralMap.insert(eNurbsElement, "nurbs");
    elementTypesPluralMap.insert(eParaboloidElement, "paraboloids");
    elementTypesPluralMap.insert(ePlaneElement, "planes");
    elementTypesPluralMap.insert(ePointElement, "points");
    elementTypesPluralMap.insert(ePointCloudElement, "point clouds");
    elementTypesPluralMap.insert(eScalarEntityAngleElement, "angles");
    elementTypesPluralMap.insert(eScalarEntityDistanceElement, "distances");
    elementTypesPluralMap.insert(eScalarEntityMeasurementSeriesElement, "measurement series");
    elementTypesPluralMap.insert(eScalarEntityTemperatureElement, "temperatures");
    elementTypesPluralMap.insert(eSlottedHoleElement, "slotted holes");
    elementTypesPluralMap.insert(eSphereElement, "spheres");
    elementTypesPluralMap.insert(eTorusElement, "tori");
    elementTypesPluralMap.insert(eDirectionElement, "directions");
    elementTypesPluralMap.insert(ePositionElement, "positions");
    elementTypesPluralMap.insert(eRadiusElement, "radii");
    elementTypesPluralMap.insert(eCoordinateSystemElement, "coordinate systems");
    elementTypesPluralMap.insert(eStationElement, "stations");
    elementTypesPluralMap.insert(eTrafoParamElement, "transformation parameters");
    elementTypesPluralMap.insert(eObservationElement, "observations");
    elementTypesPluralMap.insert(eReadingCartesianElement, "cartesian readings");
    elementTypesPluralMap.insert(eReadingPolarElement, "polar readings");
    elementTypesPluralMap.insert(eReadingDistanceElement, "distance readings");
    elementTypesPluralMap.insert(eReadingDirectionElement, "direction readings");
    elementTypesPluralMap.insert(eReadingTemperatureElement, "temperature readings");
    elementTypesPluralMap.insert(eReadingLevelElement, "level readings");

    //fill feature map
    featureTypesMap.insert(eCircleFeature, "circle");
    featureTypesMap.insert(eConeFeature, "cone");
    featureTypesMap.insert(eCylinderFeature, "cylinder");
    featureTypesMap.insert(eEllipseFeature, "ellipse");
    featureTypesMap.insert(eEllipsoidFeature, "ellipsoid");
    featureTypesMap.insert(eHyperboloidFeature, "hyperboloid");
    featureTypesMap.insert(eLineFeature, "line");
    featureTypesMap.insert(eNurbsFeature, "nurbs");
    featureTypesMap.insert(eParaboloidFeature, "paraboloid");
    featureTypesMap.insert(ePlaneFeature, "plane");
    featureTypesMap.insert(ePointFeature, "point");
    featureTypesMap.insert(ePointCloudFeature, "point cloud");
    featureTypesMap.insert(eScalarEntityAngleFeature, "angle");
    featureTypesMap.insert(eScalarEntityDistanceFeature, "distance");
    featureTypesMap.insert(eScalarEntityMeasurementSeriesFeature, "measurement series");
    featureTypesMap.insert(eScalarEntityTemperatureFeature, "temperature");
    featureTypesMap.insert(eSlottedHoleFeature, "slotted hole");
    featureTypesMap.insert(eSphereFeature, "sphere");
    featureTypesMap.insert(eTorusFeature, "torus");
    featureTypesMap.insert(eCoordinateSystemFeature, "coordinate system");
    featureTypesMap.insert(eStationFeature, "station");
    featureTypesMap.insert(eTrafoParamFeature, "transformation parameters");

    //fill geometry map
    geometryTypesMap.insert(eCircleGeometry, "circle");
    geometryTypesMap.insert(eConeGeometry, "cone");
    geometryTypesMap.insert(eCylinderGeometry, "cylinder");
    geometryTypesMap.insert(eEllipseGeometry, "ellipse");
    geometryTypesMap.insert(eEllipsoidGeometry, "ellipsoid");
    geometryTypesMap.insert(eHyperboloidGeometry, "hyperboloid");
    geometryTypesMap.insert(eLineGeometry, "line");
    geometryTypesMap.insert(eNurbsGeometry, "nurbs");
    geometryTypesMap.insert(eParaboloidGeometry, "paraboloid");
    geometryTypesMap.insert(ePlaneGeometry, "plane");
    geometryTypesMap.insert(ePointGeometry, "point");
    geometryTypesMap.insert(ePointCloudGeometry, "point cloud");
    geometryTypesMap.insert(eScalarEntityAngleGeometry, "angle");
    geometryTypesMap.insert(eScalarEntityDistanceGeometry, "distance");
    geometryTypesMap.insert(eScalarEntityMeasurementSeriesGeometry, "measurement series");
    geometryTypesMap.insert(eScalarEntityTemperatureGeometry, "temperature");
    geometryTypesMap.insert(eSlottedHoleGeometry, "slotted hole");
    geometryTypesMap.insert(eSphereGeometry, "sphere");
    geometryTypesMap.insert(eTorusGeometry, "torus");

    //fill reading map
    readingTypesMap.insert(eDistanceReading, "distance");
    readingTypesMap.insert(eCartesianReading, "cartesian");
    readingTypesMap.insert(ePolarReading, "polar");
    readingTypesMap.insert(eDirectionReading, "direction");
    readingTypesMap.insert(eTemperatureReading, "temperature");
    readingTypesMap.insert(eLevelReading, "level");

    //fill feature display attributes map
    featureDisplayAttributesMap.insert(eFeatureDisplayType, "type");
    featureDisplayAttributesMap.insert(eFeatureDisplayName, "name");
    featureDisplayAttributesMap.insert(eFeatureDisplayComment, "comment");
    featureDisplayAttributesMap.insert(eFeatureDisplayGroup, "group");
    featureDisplayAttributesMap.insert(eFeatureDisplayIsSolved, "solved");
    featureDisplayAttributesMap.insert(eFeatureDisplayIsUpdated, "updated");
    featureDisplayAttributesMap.insert(eFeatureDisplayFunctions, "functions");
    featureDisplayAttributesMap.insert(eFeatureDisplayUsedFor, "used for");
    featureDisplayAttributesMap.insert(eFeatureDisplayPreviouslyNeeded, "previously needed");
    featureDisplayAttributesMap.insert(eFeatureDisplayStDev, "stdev");
    featureDisplayAttributesMap.insert(eFeatureDisplayMeasurementConfig, "measurement config");
    featureDisplayAttributesMap.insert(eFeatureDisplayObservations, "observations");
    featureDisplayAttributesMap.insert(eFeatureDisplayX, "x");
    featureDisplayAttributesMap.insert(eFeatureDisplayY, "y");
    featureDisplayAttributesMap.insert(eFeatureDisplayZ, "z");
    featureDisplayAttributesMap.insert(eFeatureDisplayPrimaryI, "i");
    featureDisplayAttributesMap.insert(eFeatureDisplayPrimaryJ, "j");
    featureDisplayAttributesMap.insert(eFeatureDisplayPrimaryK, "k");
    featureDisplayAttributesMap.insert(eFeatureDisplayRadiusA, "radius");
    featureDisplayAttributesMap.insert(eFeatureDisplayRadiusB, "radius 2");
    featureDisplayAttributesMap.insert(eFeatureDisplaySecondaryI, "i 2");
    featureDisplayAttributesMap.insert(eFeatureDisplaySecondaryJ, "j 2");
    featureDisplayAttributesMap.insert(eFeatureDisplaySecondaryK, "k 2");
    featureDisplayAttributesMap.insert(eFeatureDisplayAperture, "aperture");
    featureDisplayAttributesMap.insert(eFeatureDisplayA, "a");
    featureDisplayAttributesMap.insert(eFeatureDisplayB, "b");
    featureDisplayAttributesMap.insert(eFeatureDisplayC, "c");
    featureDisplayAttributesMap.insert(eFeatureDisplayAngle, "angle");
    featureDisplayAttributesMap.insert(eFeatureDisplayDistance, "distance");
    featureDisplayAttributesMap.insert(eFeatureDisplayMeasurementSeries, "measurement series");
    featureDisplayAttributesMap.insert(eFeatureDisplayTemperature, "temperature");
    featureDisplayAttributesMap.insert(eFeatureDisplayLength, "length");
    featureDisplayAttributesMap.insert(eFeatureDisplayExpansionOriginX, "expansion x");
    featureDisplayAttributesMap.insert(eFeatureDisplayExpansionOriginY, "expansion y");
    featureDisplayAttributesMap.insert(eFeatureDisplayExpansionOriginZ, "expansion z");

    //fill trafo param display attributes map
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayType, "type");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayName, "name");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayComment, "comment");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayGroup, "group");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayIsSolved, "solved");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayIsUpdated, "updated");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayFunctions, "functions");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayUsedFor, "used for");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayPreviouslyNeeded, "previously needed");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayStDev, "stdev");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayStartSystem, "start");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayDestinationSystem, "destination");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayTranslationX, "tx");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayTranslationY, "ty");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayTranslationZ, "tz");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayRotationX, "rx");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayRotationY, "ry");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayRotationZ, "rz");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayScaleX, "sx");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayScaleY, "sy");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayScaleZ, "sz");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayIsUsed, "used");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayValidTime, "time");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayIsMovement, "movement");

    //fill available display attributes
    for(int i = 0; i < 10; i++){ //general feature attributes
        displayAttributes.append(i);
    }
    for(int i = 100; i < 102; i++){ //geometry specific attributes
        displayAttributes.append(i);
    }
    for(int i = 200; i < 220; i++){ //unknown geometry parameters
        displayAttributes.append(i);
    }
    for(int i = 300; i < 303; i++){ //coordinate system specific
        displayAttributes.append(i);
    }
    for(int i = 400; i < 414; i++){ //trafo param specific
        displayAttributes.append(i);
    }

    //fill unit types map
    unitTypesMap.insert(eUnitArcSeconds, "[arcsec]");
    unitTypesMap.insert(eUnitDecimalDegree, "[°]");
    unitTypesMap.insert(eUnitFahrenheit, "[°F]");
    unitTypesMap.insert(eUnitGon, "[gon]");
    unitTypesMap.insert(eUnitGrad, "[°C]");
    unitTypesMap.insert(eUnitInch, "[inch]");
    unitTypesMap.insert(eUnitKelvin, "[°K]");
    unitTypesMap.insert(eUnitMeter, "[m]");
    unitTypesMap.insert(eUnitMilliMeter, "[mm]");
    unitTypesMap.insert(eUnitMilliRadians, "[mrad]");
    unitTypesMap.insert(eUnitRadiant, "[rad]");

    isInit = true;

}

}

//#######################################
//global functions to query element types
//#######################################

/*!
 * \brief getElementTypeName
 * \param type
 * \return
 */
const QString &getElementTypeName(const ElementTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding element name
    if(internal::elementTypesMap.contains(type)){
        return internal::elementTypesMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getElementTypePluralName
 * \param type
 * \return
 */
const QString &getElementTypePluralName(const ElementTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding element name
    if(internal::elementTypesPluralMap.contains(type)){
        return internal::elementTypesPluralMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getElementTypeEnum
 * \param name
 * \return
 */
ElementTypes getElementTypeEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding element enum value
    ElementTypes type = internal::elementTypesMap.key(name, eUndefinedElement);
    if(type == eUndefinedElement){
        type = internal::elementTypesPluralMap.key(name, eUndefinedElement);;
    }

    return type;

}

/*!
 * \brief getElementTypeEnum
 * Converts FeatureTypes to ElementTypes
 * \param type
 * \return
 */
ElementTypes getElementTypeEnum(const FeatureTypes &type){

    switch(type){
    case eCircleFeature:
        return eCircleElement;
    case eConeFeature:
        return eConeElement;
    case eCylinderFeature:
        return eCylinderElement;
    case eEllipseFeature:
        return eEllipseElement;
    case eEllipsoidFeature:
        return eEllipsoidElement;
    case eHyperboloidFeature:
        return eHyperboloidElement;
    case eLineFeature:
        return eLineElement;
    case eNurbsFeature:
        return eNurbsElement;
    case eParaboloidFeature:
        return eParaboloidElement;
    case ePlaneFeature:
        return ePlaneElement;
    case ePointFeature:
        return ePointElement;
    case ePointCloudFeature:
        return ePointCloudElement;
    case eScalarEntityAngleFeature:
        return eScalarEntityAngleElement;
    case eScalarEntityDistanceFeature:
        return eScalarEntityDistanceElement;
    case eScalarEntityMeasurementSeriesFeature:
        return eScalarEntityMeasurementSeriesElement;
    case eScalarEntityTemperatureFeature:
        return eScalarEntityTemperatureElement;
    case eSlottedHoleFeature:
        return eSlottedHoleElement;
    case eSphereFeature:
        return eSphereElement;
    case eTorusFeature:
        return eTorusElement;
    case eCoordinateSystemFeature:
        return eCoordinateSystemElement;
    case eStationFeature:
        return eStationElement;
    case eTrafoParamFeature:
        return eTrafoParamElement;
    default:
        return eUndefinedElement;
    }

}

/*!
 * \brief getElementTypeEnum
 * convert ReadingTypes to ElementTypes
 * \param type
 * \return
 */
ElementTypes getElementTypeEnum(const ReadingTypes &type){

    switch(type){
    case eCartesianReading:
        return eReadingCartesianElement;
    case eDistanceReading:
        return eReadingDistanceElement;
    case ePolarReading:
        return eReadingPolarElement;
    case eDirectionReading:
        return eReadingDirectionElement;
    case eTemperatureReading:
        return eReadingTemperatureElement;
    case eLevelReading:
        return eReadingLevelElement;
    }

}

/*!
 * \brief getFeatureTypeName
 * \param type
 * \return
 */
const QString &getFeatureTypeName(const FeatureTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding feature name
    if(internal::featureTypesMap.contains(type)){
        return internal::featureTypesMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getFeatureTypeEnum
 * \param name
 * \return
 */
FeatureTypes getFeatureTypeEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding feature enum value
    return internal::featureTypesMap.key(name, eUndefinedFeature);

}

/*!
 * \brief getGeometryTypeName
 * \param type
 * \return
 */
const QString &getGeometryTypeName(const GeometryTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding geometry name
    if(internal::geometryTypesMap.contains(type)){
        return internal::geometryTypesMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getGeometryTypeEnum
 * \param name
 * \return
 */
GeometryTypes getGeometryTypeEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding geometry enum value
    return internal::geometryTypesMap.key(name, eUndefinedGeometry);

}

/*!
 * \brief getIsGeometry
 * \param type
 * \return
 */
bool getIsGeometry(const FeatureTypes &type){
    if(type == eCoordinateSystemFeature
            || type == eStationFeature
            || type == eTrafoParamFeature){
        return false;
    }
    return true;
}

/*!
 * \brief getIsScalarEntity
 * \param type
 * \return
 */
bool getIsScalarEntity(const FeatureTypes &type){
    if(type == eScalarEntityAngleFeature
            || type == eScalarEntityDistanceFeature
            || type == eScalarEntityMeasurementSeriesFeature
            || type == eScalarEntityTemperatureFeature){
        return true;
    }
    return false;
}

/*!
 * \brief getReadingTypeName
 * \param type
 * \return
 */
const QString &getReadingTypeName(const ReadingTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding reading name
    if(internal::readingTypesMap.contains(type)){
        return internal::readingTypesMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getReadingTypeEnum
 * \param name
 * \return
 */
ReadingTypes getReadingTypeEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding reading enum value
    return internal::readingTypesMap.key(name, eUndefinedReading);

}

/*!
 * \brief getDisplayAttributes
 * Returns a list of enum values (feature attributes and trafo param attributes)
 * \return
 */
const QList<int> &getDisplayAttributes(){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::displayAttributes;

}

/*!
 * \brief getIsFeatureDisplayAttribute
 * Returns true if the given attribute is a feature attribute
 * \param attr
 * \return
 */
bool getIsFeatureDisplayAttribute(const int &attr){
    if( (attr >= 0 && attr < 10) || (attr >= 100 && attr < 102) || (attr >= 200 && attr < 220) || (attr >= 300 && attr < 303) ){
        return true;
    }
    return false;
}

/*!
 * \brief getIsTrafoParamDisplayAttribute
 * Returns true if the given attribute is a trafo param attribute
 * \param attr
 * \return
 */
bool getIsTrafoParamDisplayAttribute(const int &attr){
    if( (attr >= 0 && attr < 10) || (attr >= 400 && attr < 414) ){
        return true;
    }
    return false;
}

/*!
 * \brief getDisplayAttributeName
 * Returns the name of the attribute represented by the given enum value
 * \param attr
 * \return
 */
const QString &getDisplayAttributeName(const int &attr){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    if(getIsFeatureDisplayAttribute(attr)){ //feature display attribute
        return internal::featureDisplayAttributesMap.value((FeatureDisplayAttributes)attr);
    }else if(getIsTrafoParamDisplayAttribute(attr)){ //trafo param display attribute
        return internal::trafoParamDisplayAttributesMap.value((TrafoParamDisplayAttributes)attr);
    }

    return internal::undefined;

}

/*!
 * \brief getDisplayAttributeName
 * \param attr
 * \return
 */
const QString &getDisplayAttributeName(const FeatureDisplayAttributes &attr){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::featureDisplayAttributesMap.value(attr);

}

/*!
 * \brief getDisplayAttributeName
 * \param attr
 * \return
 */
const QString &getDisplayAttributeName(const TrafoParamDisplayAttributes &attr){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::trafoParamDisplayAttributesMap.value(attr);

}

/*!
 * \brief convertToDefault
 * Convert the given value of unit type to the default unit
 * \param value
 * \param type
 * \return
 */
double convertToDefault(const double &value, const UnitType &type){

    switch(type){
    case eUnitMilliMeter:
        return value * (1.0 / 1000.0);
    case eUnitInch:
        return value * (1.0 / 39.37007874);
    case eUnitMilliRadians:
        return value * (1.0 / 1000.0);
    case eUnitGon:
        return value * (1.0 / RHO_GON);
    case eUnitArcSeconds:
        return value * (1.0 / 206264.8062471);
    case eUnitDecimalDegree:
        return value * (1.0 / RHO_DEGREE);
    case eUnitFahrenheit:
        return (value - 32.0) / 1.8;
    case eUnitKelvin:
        return value - 273.15;
    default:
        return value;
    }

}

/*!
 * \brief getUnitTypeName
 * \param type
 * \return
 */
const QString &getUnitTypeName(const UnitType &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding reading name
    if(internal::unitTypesMap.contains(type)){
        return internal::unitTypesMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getUnitTypeEnum
 * \param name
 * \return
 */
UnitType getUnitTypeEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding reading enum value
    return internal::unitTypesMap.key(name, eNoUnit);

}
