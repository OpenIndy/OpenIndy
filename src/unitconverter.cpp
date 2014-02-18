#include "unitconverter.h"

double UnitConverter::angleMultiplier = RHO_DEGREE;
double UnitConverter::distanceMultiplier = 1.0;
double UnitConverter::temperatureMultiplier = 1.0;

int UnitConverter::angleDigits = 5;
int UnitConverter::distanceDigits = 4;
int UnitConverter::temperatureDigits = 4;

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
        UnitConverter::distanceMultiplier = 1000;
        UnitConverter::distanceType = scalar;
        break;
    default:
        break;
    }

}

void UnitConverter::setTemperatureUnit(unitType scalar){

    switch (scalar) {
    case UnitConverter::eGRAD:
        UnitConverter::temperatureMultiplier = 1.0;
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

double UnitConverter::getTemperatureMultiplier(){

    return UnitConverter::temperatureMultiplier;
}

/*!
 * \brief UnitConverter::getAngleUnitString
 * Get unit character for the current angle unit
 * \return
 */
QString UnitConverter::getAngleUnitString(){
    switch(UnitConverter::angleType){
    case UnitConverter::eDECIMALDEGREE:
        return "°";
    case UnitConverter::eGON:
        return "gon";
    case UnitConverter::eRADIANT:
        return "rad";
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
        return "m";
    case UnitConverter::eMILLIMETER:
        return "mm";
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
        return "°C";
    default:
        return "";
    }
}
