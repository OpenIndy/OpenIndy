#include "pluginloader.h"

PluginMetaData *PluginLoader::myMetaInfo = new PluginMetaData();
PluginCopier *PluginLoader::pCopier = new PluginCopier();

PluginLoader::PluginLoader(QObject *parent) :
    QObject(parent)
{

}

/*!
 * \brief PluginLoader::findOiPlugin
 * \param dirPath
 * \return path
 *
 * try to find a openIndy plugin at given path.
 */
QString PluginLoader::findOiPlugin(QString dirPath){

    QDir pluginsDir(dirPath);

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {

        QString path = pluginsDir.absoluteFilePath(fileName);

        QPluginLoader pluginLoader(path);

        bool isPlugin = pluginLoader.metaData().value("MetaData").toObject().value("isOiPlugin").toBool();

        if (isPlugin){

            bool hasMetaData = PluginLoader::getMetaData(path);

            bool isValid;

            if (hasMetaData){
               pCopier->sendMsg("plugin meta data loaded",false);
               isValid = PluginLoader::checkPlugin();
            }else{
                pCopier->sendMsg("can not load plugin meta data",true);
            }

            if(isValid){
                return path;
            }
        }
    }


    pCopier->sendMsg("no plugin found",true);
    return NULL;

}

/*!
 * \brief PluginLoader::getMetaData
 * \param path
 * \return bool
 */
bool PluginLoader::getMetaData(QString path){

    QPluginLoader pluginLoader(path);

    myMetaInfo->iid =  pluginLoader.metaData().value("IID").toString();;

    myMetaInfo->name =  pluginLoader.metaData().value("MetaData").toObject().value("name").toString();
    myMetaInfo->pluginVersion =  pluginLoader.metaData().value("MetaData").toObject().value("pluginVersion").toString();
    myMetaInfo->author = pluginLoader.metaData().value("MetaData").toObject().value("author").toString();
    myMetaInfo->compiler = pluginLoader.metaData().value("MetaData").toObject().value("compiler").toString();
    myMetaInfo->operatingSystem = pluginLoader.metaData().value("MetaData").toObject().value("operatingSystem").toString();
    myMetaInfo->dependencies = pluginLoader.metaData().value("MetaData").toObject().value("dependencies").toBool();
    myMetaInfo->description= pluginLoader.metaData().value("MetaData").toObject().value("description").toString();
    myMetaInfo->dependeciesPath= pluginLoader.metaData().value("MetaData").toObject().value("libPaths").toArray();



    if (myMetaInfo->iid != NULL){  
         return true;
    }

    return false;

}

PluginMetaData* PluginLoader::getPluginMetaData(QString path){


    PluginMetaData* metaInfo = new PluginMetaData();

    QPluginLoader pluginLoader(path);

    metaInfo->iid =  pluginLoader.metaData().value("IID").toString();

    metaInfo->name =  pluginLoader.metaData().value("MetaData").toObject().value("name").toString();
    metaInfo->pluginVersion =  pluginLoader.metaData().value("MetaData").toObject().value("pluginVersion").toString();
    metaInfo->author = pluginLoader.metaData().value("MetaData").toObject().value("author").toString();
    metaInfo->compiler = pluginLoader.metaData().value("MetaData").toObject().value("compiler").toString();
    metaInfo->operatingSystem = pluginLoader.metaData().value("MetaData").toObject().value("operatingSystem").toString();
    metaInfo->dependencies = pluginLoader.metaData().value("MetaData").toObject().value("dependencies").toBool();
    metaInfo->description= pluginLoader.metaData().value("MetaData").toObject().value("description").toString();
    metaInfo->dependeciesPath= pluginLoader.metaData().value("MetaData").toObject().value("libPaths").toArray();


    if (!OiMetaData::findIID(metaInfo->iid)){
        pCopier->sendMsg("plugin not valid: wrong plugin interface version",true);
    }else if(metaInfo->operatingSystem != OiMetaData::getOperatingSys()){
        pCopier->sendMsg("plugin not valid: wrong operating system",true);
    }else if(metaInfo->compiler != OiMetaData::getCompiler()){
        pCopier->sendMsg("plugin not valid: different compiler",true);
    }else{
        pCopier->sendMsg("plugin meta data loaded",false);
        return metaInfo;
    }

    pCopier->sendMsg("can not load plugin meta data",true);
    return NULL;

}

/*!
 * \brief PluginLoader::checkPlugin
 * \return
 */
bool PluginLoader::checkPlugin(){


    if(myMetaInfo->operatingSystem != OiMetaData::getOperatingSys()){

        pCopier->sendMsg("plugin not valid: wrong operating system",true);
        return false;
    }

    if(myMetaInfo->compiler != OiMetaData::getCompiler()){

        pCopier->sendMsg("plugin not valid: wrong compiler",true);
        return false;
    }

    if(!OiMetaData::findIID(myMetaInfo->iid)){
        pCopier->sendMsg("plugin not valid: wrong plugin interface version",true);
        return false;
    }

    pCopier->sendMsg("plugin valid",true);
    myMetaInfo->isValid = true;
    return true;

}

/*!
 * \brief PluginLoader::copyPlugin
 * \param filename
 * \return
 */
bool PluginLoader::copyPlugin(QString filename){

    if (!myMetaInfo->isValid){
        pCopier->sendMsg("plugin not valid",true);
        return false;
    }

    myMetaInfo->alreadyExists = false;


#ifdef Q_OS_MAC
    QDir pluginsDir(qApp->applicationDirPath());
    pluginsDir.cdUp();
#endif


#ifdef Q_OS_WIN
    QDir pluginsDir(qApp->applicationDirPath());
#endif

#ifdef Q_OS_LINUX
    QDir pluginsDir(qApp->applicationDirPath());
#endif


    QFileInfo fileInfo(filename);
    QString pluginName(fileInfo.fileName());

    QString copyString = NULL;

    QDir desti(pluginsDir.absolutePath()+"/plugins/");
    desti.mkpath(desti.absolutePath());

    copyString = QString(desti.absolutePath()+"/"+pluginName);


    if (QFile::exists(copyString)){

        PluginMetaData *tmpMeta = PluginLoader::getPluginMetaData(copyString);

        if(myMetaInfo->pluginVersion != tmpMeta->pluginVersion){
            pCopier->sendMsg("old plugin version already exits",false);
            pCopier->sendMsg("removing old plugin version...",false);
            QFile::remove(copyString);
            pCopier->sendMsg("old plugin version removed",false);
        }else{
            pCopier->sendMsg("plugin already exits",true);
            pCopier->sendMsg("Adding plugin is terminated",true);
            myMetaInfo->alreadyExists = true;
            return true;
        }


    }

    pCopier->setPaths(myMetaInfo,filename);
    pCopier->start();


  return true;
}




PluginCopier *PluginLoader::getCopier()
{
    return pCopier;
}

bool PluginLoader::deletePlugin(PluginMetaData *metaData)
{
#ifdef Q_OS_MAC
    QDir pluginsDir(qApp->applicationDirPath());
    pluginsDir.cdUp();
#endif


#ifdef Q_OS_WIN
    QDir pluginsDir(qApp->applicationDirPath());
#endif

#ifdef Q_OS_LINUX
    QDir pluginsDir(qApp->applicationDirPath());
#endif

    QDir desti(pluginsDir.absolutePath()+"/plugins/");
    //TODO find plugin
    QString pluginToDelete = QString(desti.absolutePath()+"/"+metaData->name);

    if(QFile::exists(pluginToDelete)){
        return QFile::remove(pluginToDelete);
    }else{
        return false;
    }
}

 QList<Sensor*> PluginLoader::loadSensorPlugins(QString path){

    QList<Sensor*> sensorList;

    QPluginLoader pluginLoader(path);

    if(PluginLoader::getMetaData(path)){

       pCopier->sendMsg("load sensor plugins",false);

       QObject *plugin = pluginLoader.instance();


       if (plugin) {

           OiPlugin *SensorFactory = qobject_cast<OiPlugin *>(plugin);

            sensorList = SensorFactory->createSensors();

            pCopier->sendMsg(QString(QString::number(sensorList.size())+ " sensors successfully created."),false);
            return sensorList;
       }else{
           pCopier->sendMsg(QString("create sensor plugins failed"),true);
           return sensorList;
       }
   }else{
        pCopier->sendMsg("meta data not valid",true);
    }


}

 QList<Function*> PluginLoader::loadFunctionPlugins(QString path){

    QList<Function*> functionList;

    QPluginLoader pluginLoader(path);

    if(PluginLoader::getMetaData(path)){

       pCopier->sendMsg("load function plugins",false);

       QObject *plugin = pluginLoader.instance();


       if (plugin) {

           OiPlugin *FunctionFactory = qobject_cast<OiPlugin *>(plugin);

            functionList = FunctionFactory->createFunctions();

            pCopier->sendMsg(QString(QString::number(functionList.size())+ " functions successfully created."),false);
            return functionList;
       }else{
           pCopier->sendMsg(QString("create function plugins failed"),true);
           return functionList;
       }
   }else{
        pCopier->sendMsg("meta data not valid",true);
   }

 }

 QList<SimulationModel *> PluginLoader::loadSimulationPlugins(QString path)
 {
     QList<SimulationModel*> simulationList;

     QPluginLoader pluginLoader(path);

     if(PluginLoader::getMetaData(path)){

        pCopier->sendMsg("load simulation plugins",false);

        QObject *plugin = pluginLoader.instance();


        if (plugin) {

            OiPlugin *SimulationFactory = qobject_cast<OiPlugin *>(plugin);

             simulationList = SimulationFactory->createSimulations();

             pCopier->sendMsg(QString(QString::number(simulationList.size())+ " simulations successfully created."),false);
             return simulationList;
        }else{
            pCopier->sendMsg(QString("create simulation plugins failed"),true);
            return simulationList;
        }
    }else{
         pCopier->sendMsg("meta data not valid",true);
    }
 }

QList<NetworkAdjustment*> PluginLoader::loadNetworkAdjustmentPlugins(QString path){

    QList<NetworkAdjustment*> networkAdjustmentList;

    QPluginLoader pluginLoader(path);

    if(PluginLoader::getMetaData(path)){

       pCopier->sendMsg("load network adjustment plugins",false);

       QObject *plugin = pluginLoader.instance();


       if (plugin) {

           OiPlugin *FunctionFactory = qobject_cast<OiPlugin *>(plugin);

            networkAdjustmentList = FunctionFactory->createNetworkAdjustments();

            pCopier->sendMsg(QString(QString::number(networkAdjustmentList.size())+ " networkadjustments successfully created."),false);
            return networkAdjustmentList;
       }else{
           pCopier->sendMsg(QString("create networkadjustment plugins failed"),true);
           return networkAdjustmentList;
       }
   }else{
        pCopier->sendMsg("meta data not valid",true);
    }


}

/*!
 * \brief PluginLoader::loadSensorPlugin
 * Load the sensor with the specified name which is located in the plugin at the specified path
 * \param path
 * \param name
 * \return
 */
Sensor* PluginLoader::loadSensorPlugin(QString path, QString name){
    Sensor* s = NULL;

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        OiPlugin *sensorFactory = qobject_cast<OiPlugin *>(plugin);
        s = sensorFactory->createSensor(name);
    }else{
        Console::addLine(QString("Cannot load selected sensor"));
    }

    return s;
}

/*!
 * \brief PluginLoader::loadFunctionPlugin
 * Load the function with the specified name which is located in the plugin at the specified path
 * \param path
 * \param name
 * \return
 */
Function* PluginLoader::loadFunctionPlugin(QString path, QString name){
    Function* f = NULL;

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        OiPlugin *functionFactory = qobject_cast<OiPlugin *>(plugin);
        f = functionFactory->createFunction(name);
    }else{
        Console::addLine(QString("Cannot load selected function"));
    }

    return f;
}

SimulationModel *PluginLoader::loadSimulationPlugin(QString path, QString name)
{
    SimulationModel* s = NULL;

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        OiPlugin *simulationFactory = qobject_cast<OiPlugin *>(plugin);
        s = simulationFactory->createSimulation(name);
    }else{
        Console::addLine(QString("Cannot load selected function"));
    }

    return s;
}

/*!
 * \brief PluginLoader::loadNetworkAdjustmentPlugin
 * Load the network adjustment with the specified name which is located in the plugin at the specified path
 * \param path
 * \param name
 * \return
 */
NetworkAdjustment* PluginLoader::loadNetworkAdjustmentPlugin(QString path, QString name){
    NetworkAdjustment* n = NULL;

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        OiPlugin *networkAdjustmentFactory = qobject_cast<OiPlugin *>(plugin);
        n = networkAdjustmentFactory->createNetworkAdjustment(name);
    }else{
        Console::addLine(QString("Cannot load selected network adjustment"));
    }

    return n;
}
