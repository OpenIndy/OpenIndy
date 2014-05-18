#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QMetaEnum>
#include <QMetaObject>
#include <QStringList>
#include <QDebug>
#include "unitconverter.h"

/*!
 * \brief The Configuration class
 * This class includes all enums used in the openIndy project. So the enum decleration ist
 * central in this class. For using one, you only have to call the namespace of the class and the
 * enum you want.
 */

class Configuration : public QObject
{
    Q_OBJECT
    Q_ENUMS(Faces)
    Q_ENUMS(ReadingTypes)
    Q_ENUMS(FeatureTypes)
    Q_ENUMS(SensorFunctionalities)
    Q_ENUMS(SensorTypes)
    Q_ENUMS(ConnectionTypes)

public:
    explicit Configuration(QObject *parent = 0);
    //Configuration();
    ~Configuration();

    static QString sPlane;
    static QString sPoint;
    static QString sLine;
    static QString sStation;
    static QString sCoordinatesystem;
    static QString sSphere;
    static QString sEntitiyAngle;
    static QString sEntityDistance;
    static QString sTrafoParam;
    static QString sCircle;
    static QString sCone;
    static QString sCylinder;
    static QString sEllipsoid;
    static QString sHyperboloid;
    static QString sParaboloid;
    static QString sNurbs;
    static QString sPointCloud;
    static QString sEntityTemperature;
    static QString sEntityMeasurementSeries;

    static QString sDistance;
    static QString sCartesian;
    static QString sPolar;
    static QString sDirection;
    static QString sTemperatur;
    static QString sLevel;

    static QString sObservation;

    static QString sSteel;
    static QString sAluminum;

    static double dSteel;
    static double dAluminum;

    enum eColor{
        red,
        yellow,
        green
    };

    enum Faces{
        eFrontside,
        eBackside
    };

    enum ReadingTypes{
        eDistance,
        eCartesian,
        ePolar,
        eDirection,
        eTemperatur,
        eLevel,
        eUndefined
    };

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
        eScalarentityAngleFeature,
        eScalarEntityDistanceFeature,
        eScalarEntityTemperatureFeature,
        eScalarEntityMeasurementSeriesFeature
    };

    enum ElementTypes{
        eUndefinedElement,
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
        eReadingDirectionElement
    };

    enum SensorFunctionalities{
         eMoveAngle,
         eMoveXYZ,
         eMotorState,
         eToggleSight,
         eInitialize,
         eHome,
         eCompensation
     };

    enum SensorTypes{
        eLaserTracker,
        eTotalStation,
        eUndefinedSensor
    };

    enum ConnectionTypes{
        eNetwork,
        eSerial
    };

    enum Materials{
        eSteel,
        eAluminum
    };

    static int idCount;

    static int generateID();

    static Configuration::ElementTypes getElementTypeEnum(QString s);
    static QString getElementTypeString(Configuration::ElementTypes e);
    static double getExpansionCoefficient(QString material);
    static double getExpansionCoefficient(Configuration::Materials material);
};

#endif // CONFIGURATION_H
