#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QStringList>
#include <QMap>

#include "types.h"

//#######################################
//global functions to query element types
//#######################################

/*!
 * \brief getElementTypeName
 * \param type
 * \return
 */
QString getElementTypeName(const ElementTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::initMaps();
    }

    //get the corresponding element name
    if(internal::elementTypesMap.contains(type)){
        return internal::elementTypesMap.value(type);
    }
    return "undefined";

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
QString getFeatureTypeName(const FeatureTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::initMaps();
    }

    //get the corresponding feature name
    if(internal::featureTypesMap.contains(type)){
        return internal::featureTypesMap.value(type);
    }
    return "undefined";

}

/*!
 * \brief getElementTypeEnum
 * \param name
 * \return
 */
FeatureTypes getElementTypeEnum(const QString &name){

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
QString getGeometryTypeName(const GeometryTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::initMaps();
    }

    //get the corresponding geometry name
    if(internal::geometryTypesMap.contains(type)){
        return internal::geometryTypesMap.value(type);
    }
    return "undefined";

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

//#################################################
//maps with element enums and corresponding strings
//#################################################

namespace internal{

QMap<ElementTypes, QString> elementTypesMap;
QMap<FeatureTypes, QString> featureTypesMap;
QMap<GeometryTypes, QString> geometryTypesMap;

bool isInit = false;

/*!
 * \brief initMaps
 */
void initMaps(){

    //clear maps
    elementTypesMap.clear();
    featureTypesMap.clear();
    geometryTypesMap.clear();

    //fill elements map
    elementTypesMap.insert(ePointElement, "point");
    elementTypesMap.insert(eLineElement, "line");
    elementTypesMap.insert(ePlaneElement, "plane");
    elementTypesMap.insert(eSphereElement, "sphere");
    elementTypesMap.insert(eScalarEntityAngleElement, "angle");
    elementTypesMap.insert(eScalarEntityDistanceElement, "distance");
    elementTypesMap.insert(eScalarEntityTemperatureElement, "temperature");
    elementTypesMap.insert(eStationElement, "station");
    elementTypesMap.insert(eCoordinateSystemElement, "coordinate system");
    elementTypesMap.insert(eTrafoParamElement, "transformation parameters");
    elementTypesMap.insert(ePointCloudElement, "point cloud");
    elementTypesMap.insert(eCircleElement, "circle");
    elementTypesMap.insert(eConeElement, "cone");
    elementTypesMap.insert(eCylinderElement, "cylinder");
    elementTypesMap.insert(eEllipsoidElement, "ellipsoid");
    elementTypesMap.insert(eHyperboloidElement, "hyperboloid");
    elementTypesMap.insert(eParaboloidElement, "paraboloid");
    elementTypesMap.insert(eNurbsElement, "nurbs");
    elementTypesMap.insert(eObservationElement, "observation");
    elementTypesMap.insert(eReadingCartesianElement, "cartesian reading");
    elementTypesMap.insert(eReadingPolarElement, "polar reading");
    elementTypesMap.insert(eReadingDistanceElement, "distance reading");
    elementTypesMap.insert(eReadingDirectionElement, "direction reading");
    elementTypesMap.insert(eReadingTemperatureElement, "temperature reading");
    elementTypesMap.insert(eReadingLevelElement, "level reading");
    elementTypesMap.insert(eScalarEntityMeasurementSeriesElement, "measurement series");

    //fill features map
    featureTypesMap.insert(ePointFeature, "point");
    featureTypesMap.insert(eLineFeature, "line");
    featureTypesMap.insert(ePlaneFeature, "plane");
    featureTypesMap.insert(eSphereFeature, "sphere");
    featureTypesMap.insert(eScalarEntityAngleFeature, "angle");
    featureTypesMap.insert(eScalarEntityDistanceFeature, "distance");
    featureTypesMap.insert(eScalarEntityTemperatureFeature, "temperature");
    featureTypesMap.insert(eStationFeature, "station");
    featureTypesMap.insert(eCoordinateSystemFeature, "coordinate system");
    featureTypesMap.insert(eTrafoParamFeature, "transformation parameters");
    featureTypesMap.insert(ePointCloudFeature, "point cloud");
    featureTypesMap.insert(eCircleFeature, "circle");
    featureTypesMap.insert(eConeFeature, "cone");
    featureTypesMap.insert(eCylinderFeature, "cylinder");
    featureTypesMap.insert(eEllipsoidFeature, "ellipsoid");
    featureTypesMap.insert(eHyperboloidFeature, "hyperboloid");
    featureTypesMap.insert(eParaboloidFeature, "paraboloid");
    featureTypesMap.insert(eNurbsFeature, "nurbs");
    featureTypesMap.insert(eScalarEntityMeasurementSeriesFeature, "measurement series");

    //fill geometry map
    geometryTypesMap.insert(ePointGeometry, "point");
    geometryTypesMap.insert(eLineGeometry, "line");
    geometryTypesMap.insert(ePlaneGeometry, "plane");
    geometryTypesMap.insert(eSphereGeometry, "sphere");
    geometryTypesMap.insert(eScalarEntityAngleGeometry, "angle");
    geometryTypesMap.insert(eScalarEntityDistanceGeometry, "distance");
    geometryTypesMap.insert(eScalarEntityTemperatureGeometry, "temperature");
    geometryTypesMap.insert(ePointCloudGeometry, "point cloud");
    geometryTypesMap.insert(eCircleGeometry, "circle");
    geometryTypesMap.insert(eConeGeometry, "cone");
    geometryTypesMap.insert(eCylinderGeometry, "cylinder");
    geometryTypesMap.insert(eEllipsoidGeometry, "ellipsoid");
    geometryTypesMap.insert(eHyperboloidGeometry, "hyperboloid");
    geometryTypesMap.insert(eParaboloidGeometry, "paraboloid");
    geometryTypesMap.insert(eNurbsGeometry, "nurbs");
    geometryTypesMap.insert(eScalarEntityMeasurementSeriesGeometry, "measurement series");

    isInit = true;

}

}

#endif // UTIL_H
