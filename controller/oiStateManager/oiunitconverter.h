#ifndef OIUNITCONVERTER_H
#define OIUNITCONVERTER_H

#include <QObject>
#include <QString>
#include <QStringList>

#define PI 3.141592653589793

/*!
 * \brief The OiUnitConverter class
 * A static manager class that holds all selected units and digits for each dimension type
 */
class OiUnitConverter : public QObject
{
    Q_OBJECT

private:
    explicit OiUnitConverter(QObject *parent = 0);

public:
    static const OiUnitConverter *getInstance();

    //############################################################
    //enums with all dimension types and their corresponding units
    //############################################################

    //possible dimensions
    enum DimensionType{
        eMetric,
        eAngular,
        eTemperature
    };

    //for each dimension the corresponding unit types
    enum MetricUnitTypes{
        eMeter, //standard
        eMilliMeter,
        eInch
    };
    enum AngularUnitTypes{
        eRadiant, //standard
        eMilliRadians,
        eGon,
        eGrad,
        eArcSeconds
    };
    enum TemperatureUnitTypes{
        eCelsius, //standard
        eKelvin,
        eFahrenheit
    };

    //############################################################
    //public getter and setter to get or set digits and unit types
    //############################################################

    //get digits
    static const unsigned int &getMetricDigits();
    static const unsigned int &getAngularDigits();
    static const unsigned int &getTemperatureDigits();

    //set digits
    static void setMetricDigits(const unsigned int &digits);
    static void setAngularDigits(const unsigned int &digits);
    static void setTemperatureDigits(const unsigned int &digits);

    //get unit types
    static const OiUnitConverter::MetricUnitTypes &getMetricUnitType();
    static const OiUnitConverter::AngularUnitTypes &getAngularUnitType();
    static const OiUnitConverter::TemperatureUnitTypes &getTemperatureUnitType();

    //set unit types
    static void setMetricUnitType(const OiUnitConverter::MetricUnitTypes &unitType);
    static void setAngularUnitType(const OiUnitConverter::AngularUnitTypes &unitType);
    static void setTemperatureUnitType(const OiUnitConverter::TemperatureUnitTypes &unitType);

    //get multiplier for each dimension (expect temperature where conversion is a bit more complicated)
    static const double &getMetricMultiplier();
    static const double &getAngularMultiplier();

    //convert from standard unit type to selected one
    static void convertMetricValue(double &value);
    static void convertAngularValue(double &value);
    static void convertTemperatureValue(double &value);


    //TODO possibly add to configuration
    static QString getMetricUnitTypeName();
    static QString getAngularUnitTypeName();
    static QString getTemperatureUnitTypeName();

signals:

    //####################################################
    //signals to inform about changed unit types or digits
    //####################################################

    //one or more unit types have changed (no matter which one)
    void unitTypeChanged();

    //a specific's dimension unit type has changed
    void metricUnitTypeChanged();
    void angularUnitTypeChanged();
    void temperatureUnitTypeChanged();

    //one or more digits have changed (no matter which one)
    void digitsChanged();

    //a specific's dimension digit has changed
    void metricDigitsChanged();
    void angularDigitsChanged();
    void temperatureDigitsChanged();

private:
    static OiUnitConverter *myUnitConverter;

    //########################################
    //saved digits, unit types and multipliers
    //########################################

    //unit types
    static OiUnitConverter::MetricUnitTypes metricUnitType;
    static OiUnitConverter::AngularUnitTypes angularUnitType;
    static OiUnitConverter::TemperatureUnitTypes temperatureUnitType;

    //digits
    static unsigned int metricDigits;
    static unsigned int angularDigits;
    static unsigned int temperatureDigits;

    //multipliers
    static double metricMultiplier;
    static double angularMultiplier;

    //TODO sowohl enums als auch diese funktionen evtl in configuration auslagern

    //unit type names
    static QStringList metricUnitTypeNames;
    static QStringList angularUnitTypeNames;
    static QStringList temperatureUnitTypeNames;

    //unit type symbols
    static QStringList metricUnitTypeSymbols;
    static QStringList angularUnitTypeSymbols;
    static QStringList temperatureUnitTypeSymbols;

    //selected unit type names
    static QString metricUnitTypeName;
    static QString angularUnitTypeName;
    static QString temperatureUnitTypeName;

    //selected unit type symbols
    static QString metricUnitTypeSymbol;
    static QString angularUnitTypeSymbol;
    static QString temperatureUnitTypeSymbol;

};

#endif // OIUNITCONVERTER_H
