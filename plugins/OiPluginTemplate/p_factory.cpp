#include "p_factory.h"

/*!
 * \brief OiTemplatePlugin::OiTemplatePlugin
 * \param parent
 */
OiTemplatePlugin::OiTemplatePlugin(QObject *parent):
    QObject(parent){

}

/*!
 * \brief OiTemplatePlugin::createSensors
 * Returns a list with all sensors
 * \return
 */
QList<Sensor*> OiTemplatePlugin::createSensors(){
    QList<Sensor*> resultSet;
    return resultSet;
}

/*!
 * \brief OiTemplatePlugin::createFunctions
 * Returns a list with all functions
 * \return
 */
QList<Function*> OiTemplatePlugin::createFunctions(){
    QList<Function*> resultSet;
    return resultSet;
}

/*!
 * \brief OiTemplatePlugin::createNetworkAdjustments
 * Returns a list with all network adjustments
 * \return
 */
QList<NetworkAdjustment*> OiTemplatePlugin::createNetworkAdjustments(){
    QList<NetworkAdjustment*> resultSet;
    return resultSet;
}

/*!
 * \brief OiTemplatePlugin::createSensor
 * Returns the sensor with the specified name
 * \param name
 * \return
 */
Sensor* OiTemplatePlugin::createSensor(QString name){
    Sensor *result = NULL;
    return result;
}

/*!
 * \brief OiTemplatePlugin::createFunction
 * Returns the function with the specified name
 * \param name
 * \return
 */
Function* OiTemplatePlugin::createFunction(QString name){
    Function *result = NULL;
    return result;
}

/*!
 * \brief OiTemplatePlugin::createNetworkAdjustment
 * Returns the network adjustment with the specified name
 * \param name
 * \return
 */
NetworkAdjustment* OiTemplatePlugin::createNetworkAdjustment(QString name){
    NetworkAdjustment *result = NULL;
    return result;
}
