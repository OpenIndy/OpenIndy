#include "unitconverter.h"

double UnitConverter::angleMultiplier = RHO_DEGREE;
double UnitConverter::distanceMultiplier = 1.0;
double UnitConverter::temperatureMultiplier = 1.0;

int UnitConverter::angleDigits = 6;
int UnitConverter::distanceDigits = 4;
int UnitConverter::temperatureDigits = 4;
int UnitConverter::dimensionLessDigits = 6;

UnitConverter::unitType UnitConverter::angleType = UnitConverter::eDECIMALDEGREE;
UnitConverter::unitType UnitConverter::distanceType = UnitConverter::eMETER;
UnitConverter::unitType UnitConverter::temperatureType = UnitConverter::eGRAD;

UnitConverter::UnitConverter(QObject *parent) :
    QObject(parent)
{
}

void UnitConverter::setAngleUnit(unitType scalar){

    switch (scalar) {
    case UnitConverter::eDECIMALDEGREE:
        UnitConverter::angleMultiplier = RHO_DEGREE;
        UnitConverter::angleType = scalar;
        break;
    case UnitConverter::eGON:
        UnitConverter::angleMultiplier = RHO_GON;
        UnitConverter::angleType = scalar;
        break;
    case UnitConverter::eRADIANT:
        UnitConverter::angleMultiplier = 1.0;
        UnitConverter::angleType = scalar;
        break;
    case UnitConverter::eMilliRadians:
        UnitConverter::angleMultiplier = 1000;
        UnitConverter::angleType = scalar;
        break;
    case UnitConverter::eArcSeconds:
        UnitConverter::angleMultiplier = 206264.8062471;
        UnitConverter::angleType = scalar;
        break;
    default:
        break;
    }



}

void UnitConverter::setDistanceUnit(unitType scalar){

    switch (scalar) {
    case UnitConverter::eMETER:
        UnitConverter::distanceMultiplier = 1.0;
        UnitConverter::distanceType = scalar;
        break;
    case UnitConverter::eMILLIMETER:
        UnitConverter::distanceMultiplier = 1000.0;
        UnitConverter::distanceType = scalar;
        break;
    case UnitConverter::eInch:
        UnitConverter::distanceMultiplier = 39.37007874;
        UnitConverter::distanceType = scalar;
        break;
    default:
        break;
    }

}

void UnitConverter::setTemperatureUnit(unitType scalar){

    switch (scalar) {
    case UnitConverter::eGRAD:
        //UnitConverter::temperatureMultiplier = 1.0;
        UnitConverter::temperatureType = scalar;
        break;
    case UnitConverter::eKelvin:
        //UnitConverter::temperatureMultiplier = 1.0 + 273.15;
        UnitConverter::temperatureType = scalar;
        break;
    case UnitConverter::eFahrenheit:
        //UnitConverter::temperatureMultiplier = 1.8 + 32;
        UnitConverter::temperatureType = scalar;
        break;
    default:
        break;
    }


}

double UnitConverter::getAngleMultiplier(){

    return UnitConverter::angleMultiplier;
}

double UnitConverter::getDistanceMultiplier(){

    return UnitConverter::distanceMultiplier;
}

/*!
 * \brief UnitConverter::getTemperature converts the temperature value from °Celsius to the specified temperature unit.
 * \param temp
 * \return
 */
double UnitConverter::getTemperature(double temp){

    switch (temperatureType) {
    case eGRAD:
        return temp;
        break;
    case eKelvin:
        return (temp + 273.15);
        break;
    case eFahrenheit:
        return ((temp *1.8) + 32.0);
        break;
    default:
        break;
    }
    return UnitConverter::temperatureMultiplier;
}

/*!
 * \brief UnitConverter::getReverseTemperature converts the specified temperature in the active temperature unit to °Celsius
 * \param temp
 * \return
 */
double UnitConverter::getReverseTemperature(double temp)
{
    switch (temperatureType) {
    case eGRAD:
        return temp;
        break;
    case eKelvin:
        return (temp -273.15);
        break;
    case eFahrenheit:
        return ((temp -32.0) / 1.8);
        break;
    default:
        break;
    }

}

/*!
 * \brief UnitConverter::getAngleUnitString
 * Get unit character for the current angle unit
 * \return
 */
QString UnitConverter::getAngleUnitString(){
    switch(UnitConverter::angleType){
    case UnitConverter::eDECIMALDEGREE:
        return " [°]";
    case UnitConverter::eGON:
        return " [gon]";
    case UnitConverter::eRADIANT:
        return " [rad]";
    case UnitConverter::eMilliRadians:
        return " [mrad]";
    case UnitConverter::eArcSeconds:
        return " [arcsec]";
    default:
        return "";
    }
}

/*!
 * \brief UnitConverter::getDistanceUnitString
 * Get unit character for the current distance unit
 * \return
 */
QString UnitConverter::getDistanceUnitString(){
    switch(UnitConverter::distanceType){
    case UnitConverter::eMETER:
        return " [m]";
    case UnitConverter::eMILLIMETER:
        return " [mm]";
    case UnitConverter::eInch:
        return " [inch]";
    default:
        return "";
    }
}

/*!
 * \brief UnitConverter::getTemperatureUnitString
 * Get unit character for the current temperature unit
 * \return
 */
QString UnitConverter::getTemperatureUnitString(){
    switch(UnitConverter::temperatureType){
    case UnitConverter::eGRAD:
        return " [°C]";
    case UnitConverter::eKelvin:
        return " [°K]";
    case UnitConverter::eFahrenheit:
        return " [°F]";
    default:
        return "";
    }
}

QString UnitConverter::getUnitString(UnitConverter::unitType u)
{
    switch(u){
    case UnitConverter::eMETER:
        return " [m]";
    case UnitConverter::eMILLIMETER:
        return " [mm]";
    case UnitConverter::eGRAD:
        return " [°C]";
    case UnitConverter::eDECIMALDEGREE:
        return " [°]";
    case UnitConverter::eGON:
        return " [gon]";
    case UnitConverter::eRADIANT:
        return " [rad]";
    case UnitConverter::eInch:
        return " [inch]";
    case UnitConverter::eArcSeconds:
        return " [arcsec]";
    case UnitConverter::eMilliRadians:
        return " [mrad]";
    case UnitConverter::eKelvin:
        return " [°K]";
    case UnitConverter::eFahrenheit:
        return " [°F]";
    default:
        return "";
    }
}

UnitConverter::unitType UnitConverter::getUnitType(QString s)
{
    if(s == " [m]"){
        return UnitConverter::eMETER;
    }else if(s == " [mm]"){
        return UnitConverter::eMILLIMETER;
    }else if(s == " [°C]"){
        return UnitConverter::eGRAD;
    }else if(s == " [°]"){
        return UnitConverter::eDECIMALDEGREE;
    }else if(s == " [gon]"){
        return UnitConverter::eGON;
    }else if(s == " [rad]"){
        return UnitConverter::eRADIANT;
    }else if(s == " [arcsec]"){
        return UnitConverter::eArcSeconds;
    }else if(s == " [mrad]"){
        return UnitConverter::eMilliRadians;
    }else if(s == " [inch]"){
        return UnitConverter::eInch;
    }else if(s == " [°K]"){
        return UnitConverter::eKelvin;
    }else if(s == " [°F]"){
        return UnitConverter::eFahrenheit;
    }

}
