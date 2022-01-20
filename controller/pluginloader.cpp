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
    QObject::connect(&PluginLoader::pluginCopier, SIGNAL(sendMessage(const QString&, const MessageTypes&, const MessageDestinations&)),
                     this, SIGNAL(sendMessage(const QString&, const MessageTypes&, const MessageDestinations&)), Qt::AutoConnection);
    QObject::connect(&PluginLoader::pluginCopier, SIGNAL(deletionFinished(const bool&)),
                     this, SIGNAL(deletionFinished(const bool&)), Qt::AutoConnection);

    QObject::connect(&PluginLoader::pluginCopier, SIGNAL(importFinished(const bool&)),
                     this, SLOT(importTaskFinished(const bool&)), Qt::AutoConnection);

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

    foreach(const QString &fileName, pluginsDir.entryList(QStringList() << "p_*.dll" << "libp_*.so", QDir::Files)) {

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
            if(sensorFactory) {
            sensor = sensorFactory->createSensor(name);
            if(!sensor.isNull()){
                sensor->init();
            }
        } else {
            emit PluginLoader::getInstance()->sendMessage(QString("Cannot load selected sensor: %1: %2").arg(name).arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
        }
    }else{
        emit PluginLoader::getInstance()->sendMessage(QString("Cannot load selected sensor: %1: %2").arg(name).arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
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
            if(functionFactory){
            function = functionFactory->createFunction(name);
            if(!function.isNull()){
                function->init();
            }
        } else {
            emit PluginLoader::getInstance()->sendMessage(QString("Cannot load selected function: %1: %2").arg(name).arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
        }
    }else{
        emit PluginLoader::getInstance()->sendMessage(QString("Cannot load selected function: %1: %2").arg(name).arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
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
            if(simulationFactory) {
            simulation = simulationFactory->createSimulation(name);
            if(!simulation.isNull()){
                simulation->init();
            }
        } else {
            emit PluginLoader::getInstance()->sendMessage(QString("Cannot load selected simulation: %1: %2").arg(name).arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
        }
    }else{
        emit PluginLoader::getInstance()->sendMessage(QString("Cannot load selected simulation: %1: %2").arg(name).arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
    }

    return simulation;

}

/*!
 * \brief PluginLoader::loadBundleAdjustmentPlugin
 * \param path
 * \param name
 * \return
 */
QPointer<BundleAdjustment> PluginLoader::loadBundleAdjustmentPlugin(const QString &path, const QString &name){

    QPointer<BundleAdjustment> bundleAdjustment(NULL);

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *bundleAdjustmentFactory = qobject_cast<Plugin *>(plugin);
        if(bundleAdjustmentFactory) {
            bundleAdjustment = bundleAdjustmentFactory->createBundleAdjustment(name);
            if(!bundleAdjustment.isNull()){
                bundleAdjustment->init();
            }
        } else {
            emit PluginLoader::getInstance()->sendMessage(QString("Cannot load selected bundle adjustment: %1: %2").arg(name).arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
        }
    }else{
        emit PluginLoader::getInstance()->sendMessage(QString("Cannot load selected bundle adjustment: %1: %2").arg(name).arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
    }

    return bundleAdjustment;

}

/*!
 * \brief PluginLoader::loadToolPlugin
 * \param path
 * \param name
 * \return
 */
QPointer<Tool> PluginLoader::loadToolPlugin(const QString &path, const QString &name){

    QPointer<Tool> tool(NULL);

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *toolFactory = qobject_cast<Plugin *>(plugin);
        if(toolFactory) {
            tool = toolFactory->createTool(name);
            if(!tool.isNull()){
                tool->init();
            }
        } else {
            emit PluginLoader::getInstance()->sendMessage(QString("Cannot load selected tool: %1: %2").arg(name).arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
        }
    }else{
        emit PluginLoader::getInstance()->sendMessage(QString("Cannot load selected tool: %1: %2").arg(name).arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
    }

    return tool;

}

/*!
 * \brief PluginLoader::loadExchangeSimpleAsciiPlugin
 * \param path
 * \param name
 * \return
 */
QPointer<ExchangeSimpleAscii> PluginLoader::loadExchangeSimpleAsciiPlugin(const QString &path, const QString &name){

    QPointer<ExchangeSimpleAscii> exchange(NULL);

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *exchangeFactory = qobject_cast<Plugin *>(plugin);
        if(exchangeFactory) {
            exchange = exchangeFactory->createSimpleAsciiExchange(name);
            if(!exchange.isNull()){
                exchange->init();
            }
        } else {
            emit PluginLoader::getInstance()->sendMessage(QString("Cannot load selected simple ascii exchange: %1: %2").arg(name).arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
        }
    }else{
        emit PluginLoader::getInstance()->sendMessage(QString("Cannot load selected simple ascii exchange: %1: %2").arg(name).arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
    }

    return exchange;

}

/*!
 * \brief PluginLoader::loadExchangeDefinedFormatPlugin
 * \param path
 * \param name
 * \return
 */
QPointer<ExchangeDefinedFormat> PluginLoader::loadExchangeDefinedFormatPlugin(const QString &path, const QString &name){

    QPointer<ExchangeDefinedFormat> exchange(NULL);

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *exchangeFactory = qobject_cast<Plugin *>(plugin);
        if(exchangeFactory) {
            exchange = exchangeFactory->createDefinedFormatExchange(name);
            if(!exchange.isNull()){
                exchange->init();
            }
        } else {
            emit PluginLoader::getInstance()->sendMessage(QString("Cannot load selected defined format exchange: %1: %2").arg(name).arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
        }
    }else{
        emit PluginLoader::getInstance()->sendMessage(QString("Cannot load selected defined format exchange: %1: %2").arg(name).arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
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
        Plugin *sensorFactory = qobject_cast<Plugin *>(plugin);
        if(sensorFactory) {
            sensorList = sensorFactory->createSensors();
            foreach(const QPointer<Sensor> &sensor, sensorList){
                if(!sensor.isNull()){
                    sensor->init();
                }
            }
        } else {
            emit PluginLoader::getInstance()->sendMessage(QString("Cannot load sensors: %1").arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
        }
    }else{
        emit PluginLoader::getInstance()->sendMessage(QString("Cannot load sensors: %1").arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
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
        if(functionFactory) {
            functionList = functionFactory->createFunctions();
            foreach(const QPointer<Function> &function, functionList){
                if(!function.isNull()){
                    function->init();
                }
            }
        } else {
            emit PluginLoader::getInstance()->sendMessage(QString("Cannot load functions: %1").arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
        }
    }else{
        emit PluginLoader::getInstance()->sendMessage(QString("Cannot load functions: %1").arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
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
        if(simulationFactory) {
            simulationList = simulationFactory->createSimulations();
            foreach(const QPointer<SimulationModel> &simulation, simulationList){
                if(!simulation.isNull()){
                    simulation->init();
                }
            }
        } else {
            emit PluginLoader::getInstance()->sendMessage(QString("Cannot load simulations: %1").arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
        }
    }else{
        emit PluginLoader::getInstance()->sendMessage(QString("Cannot load simulations: %1").arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
    }

    return simulationList;

}

/*!
 * \brief PluginLoader::loadBundleAdjustmentPlugins
 * \param path
 * \return
 */
QList<QPointer<BundleAdjustment> > PluginLoader::loadBundleAdjustmentPlugins(const QString &path){

    QList<QPointer<BundleAdjustment> > bundleAdjustmentList;

    QPluginLoader pluginLoader(path);
    QObject *plugin = pluginLoader.instance();
    if(plugin){
        Plugin *bundleAdjustmentFactory = qobject_cast<Plugin *>(plugin);
        if(bundleAdjustmentFactory) {
            bundleAdjustmentList = bundleAdjustmentFactory->createBundleAdjustments();
            foreach(const QPointer<BundleAdjustment> &bundle, bundleAdjustmentList){
                if(!bundle.isNull()){
                    bundle->init();
                }
            }
        } else {
            emit PluginLoader::getInstance()->sendMessage(QString("Cannot load bundle adjustments: %1").arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
        }
    }else{
        emit PluginLoader::getInstance()->sendMessage(QString("Cannot load bundle adjustments: %1").arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
    }

    return bundleAdjustmentList;

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
        if(toolFactory) {
            toolList = toolFactory->createTools();
            foreach(const QPointer<Tool> &tool, toolList){
                if(!tool.isNull()){
                    tool->init();
                }
            }
        } else {
            emit PluginLoader::getInstance()->sendMessage(QString("Cannot load tools: %1").arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
        }
    }else{
        emit PluginLoader::getInstance()->sendMessage(QString("Cannot load tools: %1").arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
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
        if(exchangeFactory) {
            exchangeList = exchangeFactory->createSimpleAsciiExchanges();
            foreach(const QPointer<ExchangeSimpleAscii> &exchange, exchangeList){
                if(!exchange.isNull()){
                    exchange->init();
                }
            }
        } else {
            emit PluginLoader::getInstance()->sendMessage(QString("Cannot load simple ascii exchanges: %1").arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
        }
    }else{
        emit PluginLoader::getInstance()->sendMessage(QString("Cannot load simple ascii exchanges: %1").arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
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
        if(exchangeFactory) {
            exchangeList = exchangeFactory->createDefinedFormatExchanges();
            foreach(const QPointer<ExchangeDefinedFormat> &exchange, exchangeList){
                if(!exchange.isNull()){
                    exchange->init();
                }
            }
        } else {
            emit PluginLoader::getInstance()->sendMessage(QString("Cannot load defined format exchanges: %1").arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
        }
    }else{
        emit PluginLoader::getInstance()->sendMessage(QString("Cannot load defined format exchanges: %1").arg(pluginLoader.errorString()), eErrorMessage, eConsoleMessage);
    }

    return exchangeList;

}

/*!
 * \brief PluginLoader::importTaskFinished
 * Is called when the plugin copier has finished a task (import or deletion)
 * \param success
 */
void PluginLoader::importTaskFinished(const bool &success){

    PluginLoader::copierThread.quit();
    PluginLoader::copierThread.wait();

    //if the plugin has been successfully imported add it to database
    if(success){

        if(!this->pluginCopier.savePlugin(this->pluginCopier.importPluginPath)){
            emit this->sendMessage(QString("Plugin %1 has not been saved in system database").arg(this->pluginCopier.importPluginPath), eErrorMessage, eConsoleMessage);
            emit this->importFinished(false);
            return;
        }

        emit this->importFinished(true);
        return;

    }

    emit this->importFinished(false);

}
