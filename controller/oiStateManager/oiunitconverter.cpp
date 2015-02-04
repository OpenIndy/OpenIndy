#include "oiunitconverter.h"

//initialize static variables
OiUnitConverter *OiUnitConverter::myUnitConverter = NULL;
OiUnitConverter::MetricUnitTypes OiUnitConverter::metricUnitType = OiUnitConverter::eMeter;
OiUnitConverter::AngularUnitTypes OiUnitConverter::angularUnitType = OiUnitConverter::eRadiant;
OiUnitConverter::TemperatureUnitTypes OiUnitConverter::temperatureUnitType = OiUnitConverter::eCelsius;
unsigned int OiUnitConverter::metricDigits = 2;
unsigned int OiUnitConverter::angularDigits = 4;
unsigned int OiUnitConverter::temperatureDigits = 1;
double OiUnitConverter::metricMultiplier = 1.0;
double OiUnitConverter::angularMultiplier = 1.0;
QStringList OiUnitConverter::metricUnitTypeNames;
QStringList OiUnitConverter::angularUnitTypeNames;
QStringList OiUnitConverter::temperatureUnitTypeNames;
QStringList OiUnitConverter::metricUnitTypeSymbols;
QStringList OiUnitConverter::angularUnitTypeSymbols;
QStringList OiUnitConverter::temperatureUnitTypeSymbols;
QString OiUnitConverter::metricUnitTypeName;
QString OiUnitConverter::angularUnitTypeName;
QString OiUnitConverter::temperatureUnitTypeName;
QString OiUnitConverter::metricUnitTypeSymbol;
QString OiUnitConverter::angularUnitTypeSymbol;
QString OiUnitConverter::temperatureUnitTypeSymbol;

/*!
 * \brief OiUnitConverter::OiUnitConverter
 * \param parent
 */
OiUnitConverter::OiUnitConverter(QObject *parent){

    //TODO fill strings and lists
    OiUnitConverter::metricUnitTypeNames.append("meter");

}

/*!
 * \brief OiUnitConverter::getInstance
 * \return
 */
const OiUnitConverter *OiUnitConverter::getInstance(){
    if(OiUnitConverter::myUnitConverter == NULL){
        OiUnitConverter::myUnitConverter = new OiUnitConverter();
    }
    return OiUnitConverter::myUnitConverter;
}

/*!
 * \brief OiUnitConverter::getMetricDigits
 * \return
 */
const unsigned int &OiUnitConverter::getMetricDigits(){
    return OiUnitConverter::metricDigits;
}

/*!
 * \brief OiUnitConverter::getAngularDigits
 * \return
 */
const unsigned int &OiUnitConverter::getAngularDigits(){
    return OiUnitConverter::angularDigits;
}

/*!
 * \brief OiUnitConverter::getTemperatureDigits
 * \return
 */
const unsigned int &OiUnitConverter::getTemperatureDigits(){
    return OiUnitConverter::temperatureDigits;
}

/*!
 * \brief OiUnitConverter::setMetricDigits
 * \param digits
 */
void OiUnitConverter::setMetricDigits(const unsigned int &digits){
    OiUnitConverter::metricDigits = digits;
    emit OiUnitConverter::myUnitConverter->digitsChanged();
    emit OiUnitConverter::myUnitConverter->metricDigitsChanged();
}

/*!
 * \brief OiUnitConverter::setAngularDigits
 * \param digits
 */
void OiUnitConverter::setAngularDigits(const unsigned int &digits){
    OiUnitConverter::angularDigits = digits;
    emit OiUnitConverter::myUnitConverter->digitsChanged();
    emit OiUnitConverter::myUnitConverter->angularDigitsChanged();
}

/*!
 * \brief OiUnitConverter::setTemperatureDigits
 * \param digits
 */
void OiUnitConverter::setTemperatureDigits(const unsigned int &digits){
    OiUnitConverter::temperatureDigits = digits;
    emit OiUnitConverter::myUnitConverter->digitsChanged();
    emit OiUnitConverter::myUnitConverter->temperatureDigitsChanged();
}

/*!
 * \brief OiUnitConverter::getMetricUnitType
 * \return
 */
const OiUnitConverter::MetricUnitTypes &OiUnitConverter::getMetricUnitType(){
    return OiUnitConverter::metricUnitType;
}

/*!
 * \brief OiUnitConverter::getAngularUnitType
 * \return
 */
const OiUnitConverter::AngularUnitTypes &OiUnitConverter::getAngularUnitType(){
    return OiUnitConverter::angularUnitType;
}

/*!
 * \brief OiUnitConverter::getTemperatureUnitType
 * \return
 */
const OiUnitConverter::TemperatureUnitTypes &OiUnitConverter::getTemperatureUnitType(){
    return OiUnitConverter::temperatureUnitType;
}

/*!
 * \brief OiUnitConverter::setMetricUnitType
 * \param unitType
 */
void OiUnitConverter::setMetricUnitType(const OiUnitConverter::MetricUnitTypes &unitType){

    //set unit type
    OiUnitConverter::metricUnitType = unitType;

    //set multiplier
    switch(unitType){
    case OiUnitConverter::eMeter:
        OiUnitConverter::metricMultiplier = 1.0;
        break;
    case OiUnitConverter::eMilliMeter:
        OiUnitConverter::metricMultiplier = 1000.0;
        break;
    case OiUnitConverter::eInch:
        OiUnitConverter::metricMultiplier = 39.37007874;
        break;
    }

    emit OiUnitConverter::myUnitConverter->unitTypeChanged();
    emit OiUnitConverter::myUnitConverter->metricUnitTypeChanged();

}

/*!
 * \brief OiUnitConverter::setAngularUnitType
 * \param unitType
 */
void OiUnitConverter::setAngularUnitType(const OiUnitConverter::AngularUnitTypes &unitType){

    //set unit type
    OiUnitConverter::angularUnitType = unitType;

    //set multiplier
    switch(unitType){
    case OiUnitConverter::eRadiant:
        OiUnitConverter::angularMultiplier = 1.0;
        break;
    case OiUnitConverter::eMilliRadians:
        OiUnitConverter::angularMultiplier = 1000.0;
        break;
    case OiUnitConverter::eGon:
        OiUnitConverter::angularMultiplier = 200.0 / PI;
        break;
    case OiUnitConverter::eGrad:
        OiUnitConverter::angularMultiplier = 180.0 / PI;
        break;
    case OiUnitConverter::eArcSeconds:
        OiUnitConverter::angularMultiplier = 206264.8062471;
        break;
    }

    emit OiUnitConverter::myUnitConverter->unitTypeChanged();
    emit OiUnitConverter::myUnitConverter->angularUnitTypeChanged();

}

/*!
 * \brief OiUnitConverter::setTemperatureUnitType
 * \param unitType
 */
void OiUnitConverter::setTemperatureUnitType(const OiUnitConverter::TemperatureUnitTypes &unitType){
    OiUnitConverter::temperatureUnitType = unitType;
    emit OiUnitConverter::myUnitConverter->unitTypeChanged();
    emit OiUnitConverter::myUnitConverter->temperatureUnitTypeChanged();
}

/*!
 * \brief OiUnitConverter::getMetricMultiplier
 * \return
 */
const double &OiUnitConverter::getMetricMultiplier(){
    return OiUnitConverter::metricMultiplier;
}

/*!
 * \brief OiUnitConverter::getAngularMultiplier
 * \return
 */
const double &OiUnitConverter::getAngularMultiplier(){
    return OiUnitConverter::angularMultiplier;
}

/*!
 * \brief OiUnitConverter::convertMetricValue
 * \param value
 */
void OiUnitConverter::convertMetricValue(double &value){
    value = value * OiUnitConverter::metricMultiplier;
}

/*!
 * \brief OiUnitConverter::convertAngularValue
 * \param value
 */
void OiUnitConverter::convertAngularValue(double &value){
    value = value * OiUnitConverter::angularMultiplier;
}

/*!
 * \brief OiUnitConverter::convertTemperatureValue
 * \param value
 */
void OiUnitConverter::convertTemperatureValue(double &value){
    switch(OiUnitConverter::temperatureUnitType){
    case OiUnitConverter::eCelsius:
        return;
    case OiUnitConverter::eKelvin:
        value = value + 273.15;
        return;
    case OiUnitConverter::eFahrenheit:
        value = (value * 1.8) + 32.0;
        return;
    }
}

QString OiUnitConverter::getMetricUnitTypeName()
{
return "";
}

QString OiUnitConverter::getAngularUnitTypeName()
{
    return "";
}

QString OiUnitConverter::getTemperatureUnitTypeName()
{
    return "";
}
