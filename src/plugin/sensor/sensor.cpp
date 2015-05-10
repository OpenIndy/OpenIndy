#include "sensor.h"

#include "reading.h"
#include "measurementconfig.h"
#include "sensorcontrol.h"

/*!
 * \brief Sensor::Sensor
 * \param parent
 */
Sensor::Sensor(QObject *parent) : QObject(parent){

}

/*!
 * \brief Sensor::~Sensor
 */
Sensor::~Sensor(){

}

/*!
 * \brief Sensor::getSensorConfiguration
 * \return
 */
const SensorConfiguration &Sensor::getSensorConfiguration() const{
    return this->sensorConfiguration;
}

/*!
 * \brief Sensor::setSensorConfiguration
 * \param sConfig
 */
void Sensor::setSensorConfiguration(const SensorConfiguration &sConfig){
    this->sensorConfiguration = sConfig;
}

/*!
 * \brief Sensor::getLastReading
 * \return
 */
const QPair<ReadingTypes, QPointer<Reading> > &Sensor::getLastReading() const{
    return this->lastReading;
}

/*!
 * \brief Sensor::getSupportedReadingTypes
 * \return
 */
const QList<ReadingTypes> &Sensor::getSupportedReadingTypes() const{
    return this->supportedReadingTypes;
}

/*!
 * \brief Sensor::getSupportedSensorActions
 * \return
 */
const QList<SensorFunctions> &Sensor::getSupportedSensorActions() const{
    return this->supportedSensorActions;
}

/*!
 * \brief Sensor::getSupportedConnectionTypes
 * \return
 */
const QList<ConnectionTypes> &Sensor::getSupportedConnectionTypes() const{
    return this->supportedConnectionTypes;
}

/*!
 * \brief Sensor::getMetaData
 * \return
 */
const PluginMetaData &Sensor::getMetaData() const{
    return this->metaData;
}

/*!
 * \brief Sensor::getIntegerParameter
 * \return
 */
const QMap<QString, int> &Sensor::getIntegerParameter() const{
    return this->integerParameters;
}

/*!
 * \brief Sensor::getDoubleParameter
 * \return
 */
const QMap<QString, double> &Sensor::getDoubleParameter() const{
    return this->doubleParameters;
}

/*!
 * \brief Sensor::getStringParameter
 * \return
 */
const QMultiMap<QString, QString> &Sensor::getStringParameter() const{
    return this->stringParameters;
}

/*!
 * \brief Sensor::getSelfDefinedActions
 * \return
 */
const QStringList &Sensor::getSelfDefinedActions() const{
    return this->selfDefinedActions;
}

/*!
 * \brief Sensor::getDefaultAccuracy
 * \return
 */
const QMap<QString, double> &Sensor::getDefaultAccuracy() const{
    return this->defaultAccuracy;
}

/*!
 * \brief Sensor::accept
 * Calls the appropriate internal sensor method
 * \param method
 * \param sAttr
 * \return
 */
bool Sensor::accept(const SensorFunctions &method, const SensorAttributes &sAttr){
    return false;
}

/*!
 * \brief Sensor::abortAction
 * \return
 */
bool Sensor::abortAction(){
    return false;
}

/*!
 * \brief Sensor::connectSensor
 * \return
 */
bool Sensor::connectSensor(){
    emit this->sensorMessage(QString("Sensor not implemented correctly: connect method missing"));
    return false;
}

/*!
 * \brief Sensor::disconnectSensor
 * \return
 */
bool Sensor::disconnectSensor(){
    emit this->sensorMessage(QString("Sensor not implemented correctly: disconnect method missing"));
    return false;
}

/*!
 * \brief Sensor::measure
 * \param mConfig
 * \return
 */
QList<QPointer<Reading> > Sensor::measure(const MeasurementConfig &mConfig){
    emit this->sensorMessage(QString("Sensor not implemented correctly: measure method missing"));
    return QList<QPointer<Reading> >();
}

/*!
 * \brief Sensor::readingStream
 * \param streamFormat
 * \return
 */
QVariantMap Sensor::readingStream(const ReadingTypes &streamFormat){
    emit this->sensorMessage(QString("Sensor not implemented correctly: reading stream method missing"));
    return QVariantMap();
}

/*!
 * \brief Sensor::getConnectionState
 * \return
 */
bool Sensor::getConnectionState(){
    emit this->sensorMessage(QString("Sensor not implemented correctly: connection state method missing"));
    return false;
}

/*!
 * \brief Sensor::getIsReadyForMeasurement
 * \return
 */
bool Sensor::getIsReadyForMeasurement(){
    emit this->sensorMessage(QString("Sensor not implemented correctly: is ready method missing"));
    return false;
}

/*!
 * \brief Sensor::getIsBusy
 * \return
 */
bool Sensor::getIsBusy(){
    emit this->sensorMessage(QString("Sensor not implemented correctly: is busy method missing"));
    return false;
}

/*!
 * \brief Sensor::getSensorStatus
 * \return
 */
QMap<QString, QString> Sensor::getSensorStatus(){
    emit this->sensorMessage(QString("Sensor not implemented correctly: sensor status method missing"));
    return QMap<QString, QString>();
}

/*!
 * \brief Sensor::doSelfDefinedAction
 * \param action
 * \return
 */
bool Sensor::doSelfDefinedAction(const QString &action){
    return false;
}

/*!
 * \brief Sensor::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Sensor::toOpenIndyXML(QDomDocument &xmlDoc) const{

    if(xmlDoc.isNull()){
        return QDomElement();
    }

    QDomElement sensor = xmlDoc.createElement("sensor");

    //add sensor attributes
    sensor.setAttribute("name", this->getMetaData().name);
    sensor.setAttribute("plugin", this->getMetaData().pluginName);

    //add reading types
    /*QDomElement readingTypes = xmlDoc.createElement("readingTypes");
    if(this->getSupportedReadingTypes() != NULL){
        QList<ReadingTypes> *types = this->getSupportedReadingTypes();
        for(int i = 0; i < types->size(); i++){
            QDomElement readingType = xmlDoc.createElement("type");
            readingType.setAttribute("name", getReadingTypeName(types->at(i)));
            readingTypes.appendChild(readingType);
        }
    }
    sensor.appendChild(readingTypes);*/

    return sensor;

}

/*!
 * \brief Sensor::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Sensor::fromOpenIndyXML(QDomElement &xmlElem){
    return false;
}

/*!
 * \brief Sensor::init
 */
void Sensor::init(){

}
