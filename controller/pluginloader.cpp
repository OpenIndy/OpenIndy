#include "pluginloader.h"

//initialize static variables
QPointer<PluginLoader> PluginLoader::myInstance(NULL);
PluginCopier PluginLoader::pluginCopier;
QThread PluginLoader::copierThread;

/*!
 * \brief PluginLoader::importPlugin
 * \param path
 */
PluginLoader::PluginLoader(QObject *parent) : QObject(parent){

    //connect plugin copier
    QObject::connect(this, SIGNAL(startImport(const QString&)),
                     &PluginLoader::pluginCopier, SLOT(importPlugin(const QString&)), Qt::AutoConnection);
    QObject::connect(this, SIGNAL(startDeletion(const QString&)),
                     &PluginLoader::pluginCopier, SLOT(deletePlugin(const QString&)), Qt::AutoConnection);

    QObject::connect(&PluginLoader::pluginCopier, SIGNAL(updateProgress(const int&)),
                     this, SIGNAL(updateProgress(const int&)), Qt::AutoConnection);
    QObject::connect(&PluginLoader::pluginCopier, SIGNAL(sendMessage(const QString&)),
                     this, SIGNAL(sendMessage(const QString&)), Qt::AutoConnection);
    QObject::connect(&PluginLoader::pluginCopier, SIGNAL(sendError(const QString&)),
                     this, SIGNAL(sendError(const QString&)), Qt::AutoConnection);
    QObject::connect(&PluginLoader::pluginCopier, SIGNAL(importFinished(const bool&)),
                     this, SIGNAL(importFinished(const bool&)), Qt::AutoConnection);
    QObject::connect(&PluginLoader::pluginCopier, SIGNAL(deletionFinished(const bool&)),
                     this, SIGNAL(deletionFinished(const bool&)), Qt::AutoConnection);

    QObject::connect(&PluginLoader::pluginCopier, SIGNAL(importFinished(const bool&)),
                     this, SLOT(taskFinished()), Qt::AutoConnection);

}

/*!
 * \brief PluginLoader::getInstance
 * \return
 */
QPointer<PluginLoader> PluginLoader::getInstance(){
    if(PluginLoader::myInstance.isNull()){
        PluginLoader::myInstance = new PluginLoader();
    }
    return PluginLoader::myInstance;
}

/*!
 * \brief PluginLoader::getPluginMetaData
 * Get plugin meta data as a preview
 * \param path
 * \return
 */
PluginMetaData PluginLoader::getPluginMetaData(const QString &path){

    PluginMetaData metaData;

    //search in the given folder for a valid plugin
    QDir pluginsDir(path);

    foreach(const QString &fileName, pluginsDir.entryList(QDir::Files)) {

        //get file path and check if the file is a valied plugin
        QString filePath = pluginsDir.absoluteFilePath(fileName);
        QPluginLoader pluginLoader(filePath);
        bool isPlugin = pluginLoader.metaData().value("MetaData").toObject().value("isOiPlugin").toBool();
        if(isPlugin){

            //get plugin meta data
            metaData.iid = pluginLoader.metaData().value("IID").toString();
            metaData.name =  pluginLoader.metaData().value("MetaData").toObject().value("name").toString();
            metaData.pluginVersion =  pluginLoader.metaData().value("MetaData").toObject().value("pluginVersion").toString();
            metaData.author = pluginLoader.metaData().value("MetaData").toObject().value("author").toString();
            metaData.compiler = pluginLoader.metaData().value("MetaData").toObject().value("compiler").toString();
            metaData.operatingSystem = pluginLoader.metaData().value("MetaData").toObject().value("operatingSystem").toString();
            metaData.dependencies = pluginLoader.metaData().value("MetaData").toObject().value("dependencies").toBool();
            metaData.description = pluginLoader.metaData().value("MetaData").toObject().value("description").toString();
            metaData.dependenciesPath = pluginLoader.metaData().value("MetaData").toObject().value("libPaths").toArray();
            metaData.path = filePath;

            break;

        }

    }

    return metaData;

}

/*!
 * \brief PluginLoader::importPlugin
 * Imports the plugin at file path path
 * \param path
 */
void PluginLoader::importPlugin(const QString &path){

    QPointer<PluginLoader> pluginLoader = PluginLoader::getInstance();

    //move copier to thread
    PluginLoader::pluginCopier.moveToThread(&PluginLoader::copierThread);
    PluginLoader::copierThread.start();

    emit pluginLoader->startImport(path);

}

/*!
 * \brief PluginLoader::deletePlugin
 * Deletes the plugin with the name name
 * \param name
 */
void PluginLoader::deletePlugin(const QString &name){

    QPointer<PluginLoader> pluginLoader = PluginLoader::getInstance();

    //move copier to thread
    PluginLoader::pluginCopier.moveToThread(&PluginLoader::copierThread);
    PluginLoader::copierThread.start();

    emit pluginLoader->startDeletion(name);

}

/*!
 * \brief PluginLoader::loadSensorPlugin
 * \param path
 * \param name
 * \return
 */
QPointer<Sensor> PluginLoader::loadSensorPlugin(const QString &path, const QString &name){

    QPointer<Sensor> sensor(NULL);

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *sensorFactory = qobject_cast<Plugin *>(plugin);
        sensor = sensorFactory->createSensor(name);
    }else{
        Console::getInstance()->addLine(QString("Cannot load selected sensor"));
    }

    return sensor;

}

/*!
 * \brief PluginLoader::loadFunctionPlugin
 * \param path
 * \param name
 * \return
 */
QPointer<Function> PluginLoader::loadFunctionPlugin(const QString &path, const QString &name){

    QPointer<Function> function(NULL);

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *functionFactory = qobject_cast<Plugin *>(plugin);
        function = functionFactory->createFunction(name);
    }else{
        Console::getInstance()->addLine(QString("Cannot load selected function"));
    }

    return function;

}

/*!
 * \brief PluginLoader::loadSimulationPlugin
 * \param path
 * \param name
 * \return
 */
QPointer<SimulationModel> PluginLoader::loadSimulationPlugin(const QString &path, const QString &name){

    QPointer<SimulationModel> simulation(NULL);

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *simulationFactory = qobject_cast<Plugin *>(plugin);
        simulation = simulationFactory->createSimulation(name);
    }else{
        Console::getInstance()->addLine(QString("Cannot load selected simulation"));
    }

    return simulation;

}

/*!
 * \brief PluginLoader::loadNetworkAdjustmentPlugin
 * \param path
 * \param name
 * \return
 */
QPointer<NetworkAdjustment> PluginLoader::loadNetworkAdjustmentPlugin(const QString &path, const QString &name){

    QPointer<NetworkAdjustment> networkAdjustment(NULL);

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *networkAdjustmentFactory = qobject_cast<Plugin *>(plugin);
        networkAdjustment = networkAdjustmentFactory->createNetworkAdjustment(name);
    }else{
        Console::getInstance()->addLine(QString("Cannot load selected network adjustment"));
    }

    return networkAdjustment;

}

/*!
 * \brief PluginLoader::loadOiToolPlugin
 * \param path
 * \param name
 * \return
 */
QPointer<Tool> PluginLoader::loadOiToolPlugin(const QString &path, const QString &name){

    QPointer<Tool> tool(NULL);

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *toolFactory = qobject_cast<Plugin *>(plugin);
        tool = toolFactory->createTool(name);
    }else{
        Console::getInstance()->addLine(QString("Cannot load selected tool"));
    }

    return tool;

}

/*!
 * \brief PluginLoader::loadOiExchangeSimpleAsciiPlugin
 * \param path
 * \param name
 * \return
 */
QPointer<ExchangeSimpleAscii> PluginLoader::loadOiExchangeSimpleAsciiPlugin(const QString &path, const QString &name){

    QPointer<ExchangeSimpleAscii> exchange(NULL);

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *exchangeFactory = qobject_cast<Plugin *>(plugin);
        exchange = exchangeFactory->createSimpleAsciiExchange(name);
    }else{
        Console::getInstance()->addLine(QString("Cannot load selected exchange"));
    }

    return exchange;

}

/*!
 * \brief PluginLoader::loadOiExchangeDefinedFormatPlugin
 * \param path
 * \param name
 * \return
 */
QPointer<ExchangeDefinedFormat> PluginLoader::loadOiExchangeDefinedFormatPlugin(const QString &path, const QString &name){

    QPointer<ExchangeDefinedFormat> exchange(NULL);

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *exchangeFactory = qobject_cast<Plugin *>(plugin);
        exchange = exchangeFactory->createDefinedFormatExchange(name);
    }else{
        Console::getInstance()->addLine(QString("Cannot load selected exchange"));
    }

    return exchange;

}

/*!
 * \brief PluginLoader::loadSensorPlugins
 * \param path
 * \return
 */
QList<QPointer<Sensor> > PluginLoader::loadSensorPlugins(const QString &path){

    QList<QPointer<Sensor> > sensorList;

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *SensorFactory = qobject_cast<Plugin *>(plugin);
        sensorList = SensorFactory->createSensors();
    }else{
        Console::getInstance()->addLine(QString("Cannot load sensors"));
    }

    return sensorList;

}

/*!
 * \brief PluginLoader::loadFunctionPlugins
 * \param path
 * \return
 */
QList<QPointer<Function> > PluginLoader::loadFunctionPlugins(const QString &path){

    QList<QPointer<Function> > functionList;

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *functionFactory = qobject_cast<Plugin *>(plugin);
        functionList = functionFactory->createFunctions();
    }else{
        Console::getInstance()->addLine(QString("Cannot load functions"));
    }

    return functionList;

}

/*!
 * \brief PluginLoader::loadSimulationPlugins
 * \param path
 * \return
 */
QList<QPointer<SimulationModel> > PluginLoader::loadSimulationPlugins(const QString &path){

    QList<QPointer<SimulationModel> > simulationList;

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *simulationFactory = qobject_cast<Plugin *>(plugin);
        simulationList = simulationFactory->createSimulations();
    }else{
        Console::getInstance()->addLine(QString("Cannot load simulations"));
    }

    return simulationList;

}

/*!
 * \brief PluginLoader::loadNetworkAdjustmentPlugins
 * \param path
 * \return
 */
QList<QPointer<NetworkAdjustment> > PluginLoader::loadNetworkAdjustmentPlugins(const QString &path){

    QList<QPointer<NetworkAdjustment> > networkAdjustmentList;

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *networkAdjustmenFactory = qobject_cast<Plugin *>(plugin);
        networkAdjustmentList = networkAdjustmenFactory->createNetworkAdjustments();
    }else{
        Console::getInstance()->addLine(QString("Cannot load network adjustments"));
    }

    return networkAdjustmentList;

}

/*!
 * \brief PluginLoader::loadToolPlugins
 * \param path
 * \return
 */
QList<QPointer<Tool> > PluginLoader::loadToolPlugins(const QString &path){

    QList<QPointer<Tool> > toolList;

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *toolFactory = qobject_cast<Plugin *>(plugin);
        toolList = toolFactory->createTools();
    }else{
        Console::getInstance()->addLine(QString("Cannot load tools"));
    }

    return toolList;

}

/*!
 * \brief PluginLoader::loadExchangeSimpleAsciiPlugins
 * \param path
 * \return
 */
QList<QPointer<ExchangeSimpleAscii> > PluginLoader::loadExchangeSimpleAsciiPlugins(const QString &path){

    QList<QPointer<ExchangeSimpleAscii> > exchangeList;

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *exchangeFactory = qobject_cast<Plugin *>(plugin);
        exchangeList = exchangeFactory->createSimpleAsciiExchanges();
    }else{
        Console::getInstance()->addLine(QString("Cannot load exchanges"));
    }

    return exchangeList;

}

/*!
 * \brief PluginLoader::loadExchangeDefinedFormatPlugins
 * \param path
 * \return
 */
QList<QPointer<ExchangeDefinedFormat> > PluginLoader::loadExchangeDefinedFormatPlugins(const QString &path){

    QList<QPointer<ExchangeDefinedFormat> > exchangeList;

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *exchangeFactory = qobject_cast<Plugin *>(plugin);
        exchangeList = exchangeFactory->createDefinedFormatExchanges();
    }else{
        Console::getInstance()->addLine(QString("Cannot load exchanges"));
    }

    return exchangeList;

}

/*!
 * \brief PluginLoader::taskFinished
 * Is called when the plugin copier has finished a task (import or deletion)
 */
void PluginLoader::taskFinished(){

    PluginLoader::copierThread.quit();
    PluginLoader::copierThread.wait();

}
