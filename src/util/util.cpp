#include "util.h"

//#################################################
//maps with element enums and corresponding strings
//#################################################

namespace internal{

QMap<ElementTypes, QString> elementTypesMap;
QMap<FeatureTypes, QString> featureTypesMap;
QMap<GeometryTypes, QString> geometryTypesMap;
QMap<ReadingTypes, QString> readingTypesMap;

QString undefined = "";

bool isInit = false;

void initMaps(){

    //clear maps
    elementTypesMap.clear();
    featureTypesMap.clear();
    geometryTypesMap.clear();
    readingTypesMap.clear();

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
        internal::initMaps();
    }

    //get the corresponding element name
    if(internal::elementTypesMap.contains(type)){
        return internal::elementTypesMap[type];
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
        internal::initMaps();
    }

    //get the corresponding element enum value
    return internal::elementTypesMap.key(name, eUndefinedElement);

}

/*!
 * \brief getFeatureTypeName
 * \param type
 * \return
 */
const QString &getFeatureTypeName(const FeatureTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::initMaps();
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
        internal::initMaps();
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
        internal::initMaps();
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
        internal::initMaps();
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
 * \brief getReadingTypeName
 * \param type
 * \return
 */
const QString &getReadingTypeName(const ReadingTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::initMaps();
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
        internal::initMaps();
    }

    //get the corresponding reading enum value
    return internal::readingTypesMap.key(name, eUndefinedReading);

}
