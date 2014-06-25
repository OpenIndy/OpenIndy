#include "pluginloader.h"

PluginMetaData *PluginLoader::myMetaInfo = new PluginMetaData();

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
        Console::addLine(path);

        bool isPlugin = pluginLoader.metaData().value("MetaData").toObject().value("isOiPlugin").toBool();

        if (isPlugin){

            bool hasMetaData = PluginLoader::getMetaData(path);

            bool isValid;

            if (hasMetaData){
               isValid = PluginLoader::checkPlugin();
            }

            if(isValid){
                return path;
            }
        }
    }


    Console::addLine("no plugin found");
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
         Console::addLine("plugin meta data loaded");
         return true;
    }

    Console::addLine("can not load plugin meta data");
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
        Console::addLine("plugin not valid: wrong plugin interface version");
    }else if(metaInfo->operatingSystem != OiMetaData::getOperatingSys()){
        Console::addLine("plugin not valid: wrong operating system");
    }else if(metaInfo->compiler != OiMetaData::getCompiler()){
        Console::addLine("plugin not valid: different compiler");
    }else{
        Console::addLine("plugin meta data loaded");
        return metaInfo;
    }

    Console::addLine("can not load plugin meta data");
    return NULL;

}

/*!
 * \brief PluginLoader::checkPlugin
 * \return
 */
bool PluginLoader::checkPlugin(){


    if(myMetaInfo->operatingSystem != OiMetaData::getOperatingSys()){

        Console::addLine("plugin not valid: wrong operating system");
        return false;
    }

    if(myMetaInfo->compiler != OiMetaData::getCompiler()){

        Console::addLine("plugin not valid: different compiler");
        return false;
    }

    if(!OiMetaData::findIID(myMetaInfo->iid)){
        Console::addLine("plugin not valid: wrong plugin interface version");
        return false;
    }

    Console::addLine("plugin valid");
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
        Console::addLine("plugin not valid");
        return false;
    }

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

myMetaInfo->alreadyExists = false;

QFile plugin(filename);

QFileInfo fileInfo(filename);
QString pluginName(fileInfo.fileName());

QString copyString = NULL;

QDir desti(pluginsDir.absolutePath()+"/plugins/");
desti.mkpath(desti.absolutePath());

copyString = QString(desti.absolutePath()+"/"+pluginName);


    if (QFile::exists(copyString)){

        PluginMetaData *tmpMeta = PluginLoader::getPluginMetaData(copyString);

        if(myMetaInfo->pluginVersion != tmpMeta->pluginVersion){
            Console::addLine("old plugin version already exits");
            Console::addLine("removing old plugin version...");
            QFile::remove(copyString);
            Console::addLine("old plugin version removed");
        }else{
            Console::addLine("plugin already exits");
            Console::addLine("Adding plugin is terminated");
            myMetaInfo->alreadyExists = true;
            return true;
        }


    }

        Console::addLine("copy: "  + pluginName + " to " +copyString);

        //copy plugin to application
        bool success = QFile::copy(plugin.fileName(),copyString);

        //copy dependencies
        if(myMetaInfo->dependencies){
         bool copyCheck = PluginLoader::copyDependencies(fileInfo.absoluteDir().absolutePath());

         if (!copyCheck){
             Console::addLine("error: could not copy all dependencies");
         }
        }

        if (success){
            Console::addLine("plugin file successfully copied");
            myMetaInfo->path = copyString;
            myMetaInfo->emitSendMe();
        }else{
            Console::addLine("File not copied successfully");
            return false;
        }

  return true;
}


bool PluginLoader::copyDependencies(QString dirPath){

#ifdef Q_OS_MAC
QDir appDir(qApp->applicationDirPath());
#endif


#ifdef Q_OS_WIN
QDir appDir(qApp->applicationDirPath());
#endif

#ifdef Q_OS_LINUX
QDir appDir(qApp->applicationDirPath());
#endif

bool check = true;

    for(int i = 0; i < myMetaInfo->dependeciesPath.size();i++){

        //source path
        QString path(dirPath + "/" +  myMetaInfo->dependeciesPath.at(i).toObject().value("name").toString());
        QFileInfo fileInfo(path);

        //destination path
        QString destPath;

        if (myMetaInfo->dependeciesPath.at(i).toObject().value("path").toString()=="app"){
            destPath = QString(appDir.absolutePath()+"/"+fileInfo.fileName());
        }else{
            destPath = QString(myMetaInfo->dependeciesPath.at(i).toObject().value("path").toString()+"/"+fileInfo.fileName());;
        }

        //check if source is file or folder
        if (fileInfo.isFile()){

                QFile::copy(path,destPath);
                Console::addLine(QString("copy file: " + fileInfo.fileName()));

        }else if(fileInfo.isDir()){

                PluginLoader::copyDir(path,destPath);
                Console::addLine(QString("copy dir " + fileInfo.fileName()));

        }else{
            Console::addLine(QString("error (copy failed): "  + fileInfo.fileName() + " unknown file format"));
            check = false;
        }

    }

return check;


}

bool PluginLoader::copyDir(QString sourcePath, QString destinationPath){
    QDir desti;
    desti.mkpath(destinationPath);
    QDir source=sourcePath;

    QStringList flist=source.entryList(QDir::NoDotAndDotDot | QDir::Files);
    QStringList dlist = source.entryList(QDir::NoDotAndDotDot | QDir::Dirs);

    //emit fileCount(flist.size());
    //int val = 0;

    for(int i = 0; i<flist.size();i++){
        if(!QFile::copy(sourcePath+"/"+flist.at(i),destinationPath+"/"+flist.at(i))){
            return false;
        }

        //QFileInfo info(flist.at(i));
        //val +=1;
        //emit changeText("copy " + info.fileName());
        //emit changeFileCount(val);
    }

    foreach(QString dir,dlist){
        copyDir(sourcePath+"/"+dir,destinationPath+"/"+dir);
    }

    return true;
}

 QList<Sensor*> PluginLoader::loadSensorPlugins(QString path){

    QList<Sensor*> sensorList;

    QPluginLoader pluginLoader(path);

    if(PluginLoader::getMetaData(path)){

       Console::addLine("load plugin: " + path);

       QObject *plugin = pluginLoader.instance();


       if (plugin) {

           OiPlugin *SensorFactory = qobject_cast<OiPlugin *>(plugin);

            sensorList = SensorFactory->createSensors();

            Console::addLine(QString(myMetaInfo->name + " successfully created."));
            return sensorList;
       }else{
           Console::addLine(QString("create" + myMetaInfo->name + "failed"));
           return sensorList;
       }
   }else{
        Console::addLine("meta data not valid");
    }


}

 QList<Function*> PluginLoader::loadFunctionPlugins(QString path){

    QList<Function*> functionList;

    QPluginLoader pluginLoader(path);

    if(PluginLoader::getMetaData(path)){

       Console::addLine("load plugin: " + path);

       QObject *plugin = pluginLoader.instance();


       if (plugin) {

           OiPlugin *FunctionFactory = qobject_cast<OiPlugin *>(plugin);

            functionList = FunctionFactory->createFunctions();

            Console::addLine(QString(myMetaInfo->name + " successfully created."));
            return functionList;
       }else{
           Console::addLine(QString("create" + myMetaInfo->name + "failed"));
           return functionList;
       }
   }else{
        Console::addLine("meta data not valid");
   }

 }

 QList<SimulationModel *> PluginLoader::loadSimulationPlugins(QString path)
 {
     QList<SimulationModel*> simulationList;

     QPluginLoader pluginLoader(path);

     if(PluginLoader::getMetaData(path)){

        Console::addLine("load plugin: " + path);

        QObject *plugin = pluginLoader.instance();


        if (plugin) {

            OiPlugin *SimulationFactory = qobject_cast<OiPlugin *>(plugin);

             simulationList = SimulationFactory->createSimulations();

             Console::addLine(QString(myMetaInfo->name + " successfully created."));
             return simulationList;
        }else{
            Console::addLine(QString("create" + myMetaInfo->name + "failed"));
            return simulationList;
        }
    }else{
         Console::addLine("meta data not valid");
    }
 }

QList<NetworkAdjustment*> PluginLoader::loadNetworkAdjustmentPlugins(QString path){

    QList<NetworkAdjustment*> networkAdjustmentList;

    QPluginLoader pluginLoader(path);

    if(PluginLoader::getMetaData(path)){

       Console::addLine("load plugin: " + path);

       QObject *plugin = pluginLoader.instance();


       if (plugin) {

           OiPlugin *FunctionFactory = qobject_cast<OiPlugin *>(plugin);

            networkAdjustmentList = FunctionFactory->createNetworkAdjustments();

            Console::addLine(QString(myMetaInfo->name + " successfully created."));
            return networkAdjustmentList;
       }else{
           Console::addLine(QString("create" + myMetaInfo->name + "failed"));
           return networkAdjustmentList;
       }
   }else{
        Console::addLine("meta data not valid");
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
