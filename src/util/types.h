#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <QStringList>

//#######################
//available element types
//#######################

//! all OpenIndy element types
enum ElementTypes{
    ePointElement,
    eLineElement,
    ePlaneElement,
    eSphereElement,
    eScalarEntityAngleElement,
    eScalarEntityDistanceElement,
    eScalarEntityTemperatureElement,
    eStationElement,
    eCoordinateSystemElement,
    eTrafoParamElement,
    ePointCloudElement,
    eCircleElement,
    eConeElement,
    eCylinderElement,
    eEllipsoidElement,
    eHyperboloidElement,
    eParaboloidElement,
    eNurbsElement,
    eObservationElement,
    eReadingCartesianElement,
    eReadingPolarElement,
    eReadingDistanceElement,
    eReadingDirectionElement,
    eReadingTemperatureElement,
    eReadingLevelElement,
    eScalarEntityMeasurementSeriesElement,
    eUndefinedElement
};

//! all OpenIndy feature types
enum FeatureTypes{
    ePointFeature,
    eLineFeature,
    ePlaneFeature,
    eSphereFeature,
    eStationFeature,
    eCoordinateSystemFeature,
    eTrafoParamFeature,
    ePointCloudFeature,
    eCircleFeature,
    eConeFeature,
    eCylinderFeature,
    eEllipsoidFeature,
    eHyperboloidFeature,
    eNurbsFeature,
    eParaboloidFeature,
    eScalarEntityAngleFeature,
    eScalarEntityDistanceFeature,
    eScalarEntityTemperatureFeature,
    eScalarEntityMeasurementSeriesFeature,
    eUndefinedFeature
};

//! all OpenIndy geometry types
enum GeometryTypes{
    ePointGeometry,
    eLineGeometry,
    ePlaneGeometry,
    eSphereGeometry,
    ePointCloudGeometry,
    eCircleGeometry,
    eConeGeometry,
    eCylinderGeometry,
    eEllipsoidGeometry,
    eHyperboloidGeometry,
    eNurbsGeometry,
    eParaboloidGeometry,
    eScalarEntityAngleGeometry,
    eScalarEntityDistanceGeometry,
    eScalarEntityTemperatureGeometry,
    eScalarEntityMeasurementSeriesGeometry,
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
