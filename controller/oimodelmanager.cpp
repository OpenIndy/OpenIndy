#include "oimodelmanager.h"

OiModelManager *OiModelManager::myInstance = NULL;

QStringListModel OiModelManager::pluginNamesModel;
QStringListModel OiModelManager::nominalSystemsModel;
QStringListModel OiModelManager::geometryTypes;
//QStringListModel *OiModelManager::simpleAsciiExchangePlugins;
//QStringListModel OiModelManager::definedFormatExchangePlugins;

OiModelManager::OiModelManager(QObject *parent) : QObject(parent)
{
}

/*!
 * \brief OiModelManager::getInstance
 * \return
 */
OiModelManager *OiModelManager::getInstance(){
    if(OiModelManager::myInstance == NULL){
        OiModelManager::myInstance = new OiModelManager();
        OiModelManager::myInstance->initModels();
    }
    return OiModelManager::myInstance;
}

/*!
 * \brief OiModelManager::getPluginNamesModel
 * \return
 */
QStringListModel &OiModelManager::getPluginNamesModel(){
    return OiModelManager::pluginNamesModel;
}

/*!
 * \brief OiModelManager::getNominalSystemsModel
 * \return
 */
QStringListModel &OiModelManager::getNominalSystemsModel(){
    return OiModelManager::nominalSystemsModel;
}

/*!
 * \brief OiModelManager::getGeometryTypesModel
 * \return
 */
QStringListModel &OiModelManager::getGeometryTypesModel(){
    return OiModelManager::geometryTypes;
}

/*!
 * \brief OiModelManager::getSimpleAsciiExchangePlugins
 * \param plugin
 * \return
 */
QStringListModel *OiModelManager::getSimpleAsciiExchangePlugins(QString plugin){

    QMultiMap<QString,QString> simpleAsciiExchangePlugins = SystemDbManager::getAvailableSimpleAsciiExchangePlugins();

    QStringListModel *pluginsModel = new QStringListModel();
    QStringList exchangePlugins = simpleAsciiExchangePlugins.values(plugin);
    pluginsModel->setStringList(exchangePlugins);

    return pluginsModel;
}

/*!
 * \brief OiModelManager::initModels
 * Initialize all models once when OpenIndy is opened
 */
void OiModelManager::initModels(){

    //initialize exchange plugin models
    QStringList pluginNames = SystemDbManager::getAvailablePluginNames();
    OiModelManager::pluginNamesModel.setStringList(pluginNames);

    //initialize coordinate system models
    QStringList nominalSystems;
    QList<CoordinateSystem *> mySystems = OiFeatureState::getCoordinateSystems();
    foreach(CoordinateSystem *c, mySystems){
        nominalSystems.append(c->getFeatureName());
    }
    OiModelManager::nominalSystemsModel.setStringList(nominalSystems);

    //initialize geometry types
    QStringList geometryTypes;
    geometryTypes.append(Configuration::sPoint);



    /*QStringList simpleAsciiExchangePlugins = SystemDbManager::getAvailableSimpleAsciiExchangePlugins();
    QStringList definedFormatExchangePlugins = SystemDbManager::getAvailableDefinedFormatExchangePlugins();
    OiModelManager::simpleAsciiExchangePlugins.setStringList(simpleAsciiExchangePlugins);
    OiModelManager::definedFormatExchangePlugins.setStringList(definedFormatExchangePlugins);*/

}
