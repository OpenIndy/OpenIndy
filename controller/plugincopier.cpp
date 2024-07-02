#include "plugincopier.h"

#include "pluginloader.h"

/*!
 * \brief PluginCopier::PluginCopier
 * \param parent
 */
PluginCopier::PluginCopier(QObject *parent) : QObject(parent){

}

/*!
 * \brief PluginCopier::importPlugin
 * \param path
 */
void PluginCopier::importPlugin(const QString &path){

    //get and check plugin meta data
    PluginMetaData metaData;
    if(!this->checkPluginMetaData(metaData, path)){
        emit this->importFinished(false);
        return;
    }

    //get application directory
#ifdef Q_OS_MAC
    QDir appDir(qApp->applicationDirPath());
    appDir.cdUp();
#endif
#ifdef Q_OS_WIN
    QDir appDir(qApp->applicationDirPath());
#endif
#ifdef Q_OS_LINUX
    QDir appDir(qApp->applicationDirPath());
#endif

    //get file info and check if plugin already exists
    QFileInfo pluginFileInfo(path);
    QDir pluginDir(appDir.absolutePath() + "/plugins");
    pluginDir.mkpath(pluginDir.absolutePath());

    if(QFile::exists(pluginDir.absoluteFilePath(pluginFileInfo.fileName()))){
        emit this->sendMessage(QString("Plugin %1 already exists").arg(pluginFileInfo.fileName()), eErrorMessage);
        emit this->importFinished(false);
        return;
    }

    pluginDir.cdUp();

    //copy dependencies
    if(metaData.dependencies){

        //check if the dependencies exist
        if(!this->checkDependencies(pluginFileInfo.absoluteDir().absolutePath(), metaData)){
            emit this->sendMessage(QString("Not all specified dependencies could be found for plugin %1").arg(pluginFileInfo.fileName()), eCriticalMessage);
            emit this->importFinished(false);
            return;
        }

        //copy all dependencies
        for(int i = 0; i < metaData.dependenciesPath.size(); i++){

            //get source path
            QString sourcePath(pluginFileInfo.absoluteDir().absolutePath() + "/" +  metaData.dependenciesPath.at(i).toObject().value("name").toString());
            QFileInfo sourcePathInfo(sourcePath);

            //get destination path
            QString destPath;
            if (metaData.dependenciesPath.at(i).toObject().value("path").toString().compare("app") == 0){
                destPath = QString(pluginDir.absolutePath() + "/" + metaData.dependenciesPath.at(i).toObject().value("name").toString());
            }else{
                destPath = QString(metaData.dependenciesPath.at(i).toObject().value("path").toString() + "/" + metaData.dependenciesPath.at(i).toObject().value("name").toString());
            }

            //copy from source to destination
            if(sourcePathInfo.isFile()){

                if(QFileInfo::exists(destPath)){
                    QFile::remove(destPath);
                }
                QFile::copy(sourcePath, destPath);

            }else if(sourcePathInfo.isDir()){

                this->copyDir(sourcePath, destPath);

            }else{

                emit this->sendMessage(QString("Copy failed for file %1 (unknown file format)").arg(sourcePathInfo.fileName()), eCriticalMessage);

            }

            emit this->updateProgress( (i * 100) / metaData.dependenciesPath.size() );

        }
    }

    pluginDir.cd("plugins");

    //copy plugin itself
    if(!QFile::copy(pluginFileInfo.absoluteFilePath(), pluginDir.absoluteFilePath(pluginFileInfo.fileName()))){
        emit this->sendMessage(QString("Copy failed for plugin %1").arg(pluginFileInfo.fileName()), eErrorMessage);
        emit this->importFinished(false);
        return;
    }

    //save plugin path to be able to add the plugin to database later
    this->importPluginPath = pluginDir.absoluteFilePath(pluginFileInfo.fileName());

    emit this->importFinished(true);

}

/*!
 * \brief PluginCopier::deletePlugin
 * \param name
 */
void PluginCopier::deletePlugin(const QString &name){

    //TODO delete plugins

    emit this->deletionFinished(false);

}

/*!
 * \brief PluginCopier::checkPluginMetaData
 * \param metaData
 * \param path
 * \return
 */
bool PluginCopier::checkPluginMetaData(PluginMetaData &metaData, const QString &path){

    QPluginLoader pluginLoader(path);
    metaData.iid = pluginLoader.metaData().value("IID").toString();
    metaData.name =  pluginLoader.metaData().value("MetaData").toObject().value("name").toString();
    metaData.pluginVersion =  pluginLoader.metaData().value("MetaData").toObject().value("pluginVersion").toString();
    metaData.author = pluginLoader.metaData().value("MetaData").toObject().value("author").toString();
    metaData.compiler = pluginLoader.metaData().value("MetaData").toObject().value("compiler").toString();
    metaData.operatingSystem = pluginLoader.metaData().value("MetaData").toObject().value("operatingSystem").toString();
    metaData.dependencies = pluginLoader.metaData().value("MetaData").toObject().value("dependencies").toBool();
    metaData.description = pluginLoader.metaData().value("MetaData").toObject().value("description").toString();
    metaData.dependenciesPath = pluginLoader.metaData().value("MetaData").toObject().value("libPaths").toArray();

    qDebug() << "plugin: " <<  metaData.iid;
    qDebug() << "oi: " << OiMetaData::iid_Plugin;

    //check OpenIndy version
    if(metaData.iid.compare(OiMetaData::iid_Plugin) != 0){
        emit this->sendMessage("Plugin version does not match this OpenIndy version", eCriticalMessage);
        return false;
    }

    //check operating system
    if(metaData.operatingSystem.compare(OiMetaData::getOperatingSys()) != 0){
        emit this->sendMessage("Plugin has been compiled on a different OS than this OpenIndy version", eCriticalMessage);
        return false;
    }

    //check compiler
    if(metaData.compiler.compare(OiMetaData::getCompiler()) != 0){
        emit this->sendMessage("Plugin has been compiled with a different compiler than this OpenIndy version", eCriticalMessage);
        return false;
    }

    return true;

}

/*!
 * \brief PluginCopier::copyDir
 * Copies all files and folders of sourcePath to destinationPath
 * \param sourcePath
 * \param destinationPath
 * \return
 */
bool PluginCopier::copyDir(const QString &sourcePath, const QString &destinationPath){

    QDir destDir;
    destDir.mkpath(destinationPath);
    QDir sourceDir(sourcePath);

    //get lists of files and folders in source folder
    QStringList destFiles = sourceDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
    QStringList destDirs = sourceDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);

    //remove files and folders in destination folder and add source files and folders
    for(int i = 0; i < destFiles.size(); i++){

        if(QFileInfo::exists(destinationPath + "/" + destFiles.at(i))){
            QFile::remove(destinationPath + "/" + destFiles.at(i));
        }

        if(!QFile::copy(sourcePath + "/" + destFiles.at(i), destinationPath + "/" + destFiles.at(i))){
            return false;
        }else{

        }

    }
    foreach(const QString &dir, destDirs){
        this->copyDir(sourcePath + "/" + dir, destinationPath + "/" + dir);
    }

    return true;

}

/*!
 * \brief PluginCopier::checkDependencies
 * Checks wether the dependencies at sourcePath exist or not
 * \param sourcePath
 * \param metaData
 * \return
 */
bool PluginCopier::checkDependencies(const QString &sourcePath, const PluginMetaData &metaData){

    for(int i = 0; i < metaData.dependenciesPath.size(); i++){

        //get source file path
        QString p(sourcePath + "/" + metaData.dependenciesPath.at(i).toObject().value("name").toString());
        QFileInfo fileInfo(p);

        if(!fileInfo.exists()){
            emit this->sendMessage(QString("dependency %1 does not exist").arg(fileInfo.fileName()), eCriticalMessage);
            return false;
        }
    }

    return true;

}

/*!
 * \brief PluginCopier::savePlugin
 * \param path
 * \return
 */
bool PluginCopier::savePlugin(const QString &path){

    //check if the file exists
    if(!QFile::exists(path)){
        emit this->sendMessage(QString("file not exists: %1").arg(path), eCriticalMessage);
        return false;
    }

    //create plugin for database
    sdb::Plugin plugin;
    QPluginLoader pluginLoader(path);
    plugin.iid = pluginLoader.metaData().value("IID").toString();
    plugin.name =  pluginLoader.metaData().value("MetaData").toObject().value("name").toString();
    plugin.version =  pluginLoader.metaData().value("MetaData").toObject().value("pluginVersion").toString();
    plugin.author = pluginLoader.metaData().value("MetaData").toObject().value("author").toString();
    plugin.compiler = pluginLoader.metaData().value("MetaData").toObject().value("compiler").toString();
    plugin.operating_sys = pluginLoader.metaData().value("MetaData").toObject().value("operatingSystem").toString();
    plugin.has_dependencies = pluginLoader.metaData().value("MetaData").toObject().value("dependencies").toBool();
    plugin.description = pluginLoader.metaData().value("MetaData").toObject().value("description").toString();
    plugin.file_path = path;
    plugin.is_active = true;

    //add sensors
    QList<QPointer<Sensor> > sensors = PluginLoader::loadSensorPlugins(path);
    foreach(const QPointer<Sensor> &sensor, sensors){
        if(!sensor.isNull()){
            sdb::Sensor pSensor;
            pSensor.iid = sensor->getMetaData().iid;
            pSensor.name = sensor->getMetaData().name;
            pSensor.description = sensor->getMetaData().description;
            plugin.sensors.append(pSensor);
        }
    }

    //add functions
    QList<QPointer<Function> > functions = PluginLoader::loadFunctionPlugins(path);
    foreach(const QPointer<Function> &function, functions){
        if(!function.isNull()){
            sdb::Function pFunction;
            pFunction.iid = function->getMetaData().iid;
            pFunction.name = function->getMetaData().name;
            pFunction.description = function->getMetaData().description;
            pFunction.applicableFor = function->getApplicableFor();
            foreach(const NeededElement &element, function->getNeededElements()){
                pFunction.neededElements.append(element.typeOfElement);
            }
            plugin.functions.append(pFunction);
        }
    }

    //add simulations
    QList<QPointer<SimulationModel> > simulations = PluginLoader::loadSimulationPlugins(path);
    foreach(const QPointer<SimulationModel> &simulation, simulations){
        if(!simulation.isNull()){
            sdb::Simulation pSimulation;
            pSimulation.iid = simulation->getMetaData().iid;
            pSimulation.name = simulation->getMetaData().name;
            pSimulation.description = simulation->getMetaData().description;
            plugin.simulations.append(pSimulation);
        }
    }

    //add tools
    QList<QPointer<Tool> > tools = PluginLoader::loadToolPlugins(path);
    foreach(const QPointer<Tool> &tool, tools){
        if(!tool.isNull()){
            sdb::Tool pTool;
            pTool.iid = tool->getMetaData().iid;
            pTool.name = tool->getMetaData().name;
            pTool.description = tool->getMetaData().description;
            plugin.tools.append(pTool);
        }
    }

    //add network adjustments
    /*QList<QPointer<NetworkAdjustment> > networkAdjustments = PluginLoader::loadNetworkAdjustmentPlugins(path);
    foreach(const QPointer<NetworkAdjustment> &networkAdjustment, networkAdjustments){
        if(!networkAdjustment.isNull()){
            sdb::NetworkAdjustment pNetworkAdjustment;
            pNetworkAdjustment.iid = networkAdjustment->getMetaData().iid;
            pNetworkAdjustment.name = networkAdjustment->getMetaData().name;
            pNetworkAdjustment.description = networkAdjustment->getMetaData().description;
            plugin.networkAdjustments.append(pNetworkAdjustment);
        }
    }*/

    //add simple ascii exchanges
    QList<QPointer<ExchangeSimpleAscii> > exchangesSimpleAscii = PluginLoader::loadExchangeSimpleAsciiPlugins(path);
    foreach(const QPointer<ExchangeSimpleAscii> &exchange, exchangesSimpleAscii){
        if(!exchange.isNull()){
            sdb::Exchange pExchange;
            pExchange.iid = exchange->getMetaData().iid;
            pExchange.name = exchange->getMetaData().name;
            pExchange.description = exchange->getMetaData().description;
            plugin.exchanges.append(pExchange);
        }
    }

    //add defined format exchanges
    QList<QPointer<ExchangeDefinedFormat> > exchangesDefinedFormat = PluginLoader::loadExchangeDefinedFormatPlugins(path);
    foreach(const QPointer<ExchangeDefinedFormat> &exchange, exchangesDefinedFormat){
        if(!exchange.isNull()){
            sdb::Exchange pExchange;
            pExchange.iid = exchange->getMetaData().iid;
            pExchange.name = exchange->getMetaData().name;
            pExchange.description = exchange->getMetaData().description;
            plugin.exchanges.append(pExchange);
        }
    }

    QString msg;
    const bool success = SystemDbManager::addPlugin(plugin, msg);
    if(!msg.isEmpty()) {
        emit this->sendMessage(msg, success ? eInformationMessage : eCriticalMessage);
    }
    return success;

}
