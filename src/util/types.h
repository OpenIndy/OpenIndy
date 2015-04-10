#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <QStringList>

//#######################
//available element types
//#######################

//! all OpenIndy element types
enum ElementTypes{

    //geometry types
    eCircleElement,
    eConeElement,
    eCylinderElement,
    eEllipseElement,
    eEllipsoidElement,
    eHyperboloidElement,
    eLineElement,
    eNurbsElement,
    eParaboloidElement,
    ePlaneElement,
    ePointElement,
    ePointCloudElement,
    eScalarEntityAngleElement,
    eScalarEntityDistanceElement,
    eScalarEntityMeasurementSeriesElement,
    eScalarEntityTemperatureElement,
    eSlottedHoleElement,
    eSphereElement,
    eTorusElement,

    //geometric element types
    eDirectionElement,
    ePositionElement,
    eRadiusElement,

    //general feature types
    eCoordinateSystemElement,
    eStationElement,
    eTrafoParamElement,

    //observation and reading types
    eObservationElement,
    eReadingCartesianElement,
    eReadingPolarElement,
    eReadingDistanceElement,
    eReadingDirectionElement,
    eReadingTemperatureElement,
    eReadingLevelElement,

    eUndefinedElement

};

//! all OpenIndy feature types
enum FeatureTypes{

    //geometry types
    eCircleFeature,
    eConeFeature,
    eCylinderFeature,
    eEllipseFeature,
    eEllipsoidFeature,
    eHyperboloidFeature,
    eLineFeature,
    eNurbsFeature,
    eParaboloidFeature,
    ePlaneFeature,
    ePointFeature,
    ePointCloudFeature,
    eScalarEntityAngleFeature,
    eScalarEntityDistanceFeature,
    eScalarEntityMeasurementSeriesFeature,
    eScalarEntityTemperatureFeature,
    eSlottedHoleFeature,
    eSphereFeature,
    eTorusFeature,

    //general feature types
    eCoordinateSystemFeature,
    eStationFeature,
    eTrafoParamFeature,

    eUndefinedFeature

};

//! all OpenIndy geometry types
enum GeometryTypes{

    eCircleGeometry,
    eConeGeometry,
    eCylinderGeometry,
    eEllipseGeometry,
    eEllipsoidGeometry,
    eHyperboloidGeometry,
    eLineGeometry,
    eNurbsGeometry,
    eParaboloidGeometry,
    ePlaneGeometry,
    ePointGeometry,
    ePointCloudGeometry,
    eScalarEntityAngleGeometry,
    eScalarEntityDistanceGeometry,
    eScalarEntityMeasurementSeriesGeometry,
    eScalarEntityTemperatureGeometry,
    eSlottedHoleGeometry,
    eSphereGeometry,
    eTorusGeometry,

    eUndefinedGeometry

};

//#######################
//available reading types
//#######################

enum ReadingTypes{

    eDistanceReading,
    eCartesianReading,
    ePolarReading,
    eDirectionReading,
    eTemperatureReading,
    eLevelReading,
    eUndefinedReading

};

//#####################
//sensor specific enums
//#####################

enum SensorTypes{

    eLaserTracker,
    eTotalStation,
    eUndefinedSensor

};

enum SensorFaces{

    eFrontSide,
    eBackSide

};

enum ConnectionTypes{

    eNetworkConnection,
    eSerialConnection

};

enum SensorFunctions{

    eMoveAngle,
    eMoveXYZ,
    eMotorState,
    eToggleSight,
    eInitialize,
    eHome,
    eCompensation

};

#endif // TYPES_H
