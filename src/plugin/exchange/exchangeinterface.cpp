#include "exchangeinterface.h"

/*!
 * \brief ExchangeInterface::ExchangeInterface
 * \param parent
 */
ExchangeInterface::ExchangeInterface(QObject *parent) : QObject(parent){

    //init units
    this->units.insert(eMetric, eUnitMeter);
    this->units.insert(eAngular, eUnitDecimalDegree);
    this->units.insert(eTemperature, eUnitGrad);

}

/*!
 * \brief ExchangeInterface::~ExchangeInterface
 */
ExchangeInterface::~ExchangeInterface(){

    //delete device if not deleted yet
    if(!this->device.isNull()){
        delete this->device;
    }

}

/*!
 * \brief ExchangeInterface::getMetaData
 * \return
 */
const PluginMetaData &ExchangeInterface::getMetaData() const{
    return this->metaData;
}

/*!
 * \brief ExchangeInterface::getSupportedGeometries
 * \return
 */
const QList<GeometryTypes> &ExchangeInterface::getSupportedGeometries(){
    return this->supportedGeometries;
}

/*!
 * \brief ExchangeInterface::getDevice
 * \return
 */
const QPointer<QIODevice> &ExchangeInterface::getDevice() const{
    return this->device;
}

/*!
 * \brief ExchangeInterface::setDevice
 * \param device
 */
void ExchangeInterface::setDevice(const QPointer<QIODevice> &device){
    this->device = device;
}

/*!
 * \brief ExchangeInterface::getFeatures
 * \return
 */
const QList<QPointer<FeatureWrapper> > &ExchangeInterface::getFeatures() const{
    return this->features;
}

/*!
 * \brief ExchangeInterface::setFeatures
 * \param features
 */
void ExchangeInterface::setFeatures(const QList<QPointer<FeatureWrapper> > &features){
    this->features = features;
}

/*!
 * \brief ExchangeInterface::getExportObservations
 * \return
 */
const bool &ExchangeInterface::getExportObservations() const{
    return this->exportObservations;
}

/*!
 * \brief ExchangeInterface::setExportObservations
 * \param exportObservations
 */
void ExchangeInterface::setExportObservations(const bool &exportObservations){
    this->exportObservations = exportObservations;
}

/*!
 * \brief ExchangeInterface::getGroupName
 * \return
 */
const QString &ExchangeInterface::getGroupName() const{
    return this->groupName;
}

/*!
 * \brief ExchangeInterface::setGroupName
 * \param groupName
 */
void ExchangeInterface::setGroupName(const QString &groupName){
    this->groupName = groupName;
}

/*!
 * \brief ExchangeInterface::getNominalSystem
 * \return
 */
const QPointer<CoordinateSystem> &ExchangeInterface::getNominalSystem() const{
    return this->nominalSystem;
}

/*!
 * \brief ExchangeInterface::setNominalSystem
 * \param nominalSystem
 */
void ExchangeInterface::setNominalSystem(const QPointer<CoordinateSystem> &nominalSystem){
    this->nominalSystem = nominalSystem;
}

/*!
 * \brief ExchangeInterface::getUnits
 * \return
 */
const QMap<DimensionType, UnitType> &ExchangeInterface::getUnits() const{
    return this->units;
}

/*!
 * \brief ExchangeInterface::setUnit
 * \param dimension
 * \param unit
 */
void ExchangeInterface::setUnit(const DimensionType &dimension, const UnitType &unit){
    this->units.insert(dimension, unit);
}

/*!
 * \brief ExchangeInterface::importOiData
 */
void ExchangeInterface::importOiData(){
    emit this->sendMessage(QString("Exchange Plugin not implemented correctly: import method missing"));
}

/*!
 * \brief ExchangeInterface::exportOiData
 */
void ExchangeInterface::exportOiData(){
    emit this->sendMessage(QString("Exchange Plugin not implemented correctly: export method missing"));
}

/*!
 * \brief ExchangeInterface::init
 */
void ExchangeInterface::init(){

}
