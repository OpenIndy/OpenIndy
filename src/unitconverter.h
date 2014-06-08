#ifndef UNITCONVERTER_H
#define UNITCONVERTER_H

#include <QObject>

#define PI 3.141592653589793
#define RHO_DEGREE  (180.0/PI)
#define RHO_GON (200.0/PI)

class UnitConverter : public QObject
{
    Q_OBJECT
public:
    explicit UnitConverter(QObject *parent = 0);

    enum unitType{
        eMILLIMETER,
        eMETER,
        eRADIANT,
        eGON,
        eDECIMALDEGREE,
        eGRAD,
        eInch,
        eArcSeconds,
        eMilliRadians,
        eKelvin,
        eFahrenheit
    };

    enum dimensionType{
        eMetric,
        eAngular,
        eTemperature,
        eUndefined
    };

    static int angleDigits;
    static int distanceDigits;
    static int temperatureDigits;

    static unitType angleType;
    static unitType distanceType;
    static unitType temperatureType;

signals:

public slots:

private:
    static double angleMultiplier;
    static double distanceMultiplier;
    static double temperatureMultiplier;

public:
    static void setAngleUnit(unitType scalar);
    static void setDistanceUnit(unitType scalar);
    static void setTemperatureUnit(unitType scalar);

    static double getAngleMultiplier();
    static double getDistanceMultiplier();
    static double getTemperature(double temp);
    static double getReverseTemperature(double temp);

    static QString getAngleUnitString();
    static QString getDistanceUnitString();
    static QString getTemperatureUnitString();

    static QString getUnitString(UnitConverter::unitType u);
    static UnitConverter::unitType getUnitType(QString s);

};

#endif // UNITCONVERTER_H
