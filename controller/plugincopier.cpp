#include "plugincopier.h"

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
    QDir pluginDir(appDir.absolutePath() + "/plugins/" + pluginFileInfo.fileName());
    pluginDir.mkpath(pluginDir.absolutePath());
    if(QFile::exists(pluginDir.absoluteFilePath(pluginFileInfo.fileName()))){
        emit this->sendError(QString("Plugin %1 already exists").arg(pluginFileInfo.fileName()));
        emit this->importFinished(false);
        return;
    }

    //copy dependencies
    if(metaData.dependencies){

        //check if the dependencies exist
        if(!this->checkDependencies(pluginFileInfo.absoluteDir().absolutePath(), metaData)){
            emit this->sendError(QString("Not all specified dependencies could be found for plugin %1").arg(pluginFileInfo.fileName()));
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
                destPath = QString(pluginDir.absolutePath() + "/" + pluginFileInfo.fileName());
            }else{
                destPath = QString(metaData.dependenciesPath.at(i).toObject().value("path").toString() + "/" + pluginFileInfo.fileName());
            }

            //copy from source to destination
            if(sourcePathInfo.isFile()){

                if(QFileInfo::exists(destPath)){
                    QFile::remove(destPath);
                }
                QFile::copy(sourcePath, destPath);
                emit this->sendMessage(destPath);

            }else if(sourcePathInfo.isDir()){

                this->copyDir(sourcePath, destPath);

            }else{

                emit this->sendError(QString("Copy failed for file %1 (unknown file format)").arg(sourcePathInfo.fileName()));

            }

            emit this->updateProgress( (i * 100) / metaData.dependenciesPath.size() );

        }
    }

    //copy plugin itself
    if(!QFile::copy(pluginFileInfo.absoluteFilePath(), pluginDir.absolutePath() + "/" + pluginFileInfo.fileName())){
        emit this->sendError(QString("Copy failed for plugin %1").arg(pluginFileInfo.fileName()));
        emit this->importFinished(false);
        return;
    }

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

    //check OpenIndy version
    if(metaData.iid.compare(OiMetaData::iid_Plugin) != 0){
        emit this->sendError("Plugin version does not match this OpenIndy version");
        return false;
    }

    //check operating system
    if(metaData.operatingSystem.compare(OiMetaData::getOperatingSys()) != 0){
        emit this->sendError("Plugin has been compiled on a different OS than this OpenIndy version");
        return false;
    }

    //check compiler
    if(metaData.compiler.compare(OiMetaData::getCompiler()) != 0){
        emit this->sendError("Plugin has been compiled with a different compiler than this OpenIndy version");
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
            emit sendMessage(destinationPath + "/" + destFiles.at(i));
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
            this->sendError(QString("dependency " + fileInfo.fileName() + " does not exist"));
            return false;
        }
    }

    return true;

}
