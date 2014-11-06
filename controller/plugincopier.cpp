#include "plugincopier.h"

PluginCopier::PluginCopier(QObject *parent) :
    QThread(parent)
{
}

void PluginCopier::run()
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

    QFileInfo pluginFile(pluginPath);
    QString pluginName(pluginFile.fileName());


    if(pMetaData->dependencies){
        int count = 1;
        for(int i = 0; i < pMetaData->dependeciesPath.size();i++){

            //source path
            QString path(pluginFile.absoluteDir().absolutePath() + "/" +  pMetaData->dependeciesPath.at(i).toObject().value("name").toString());
            QFileInfo fileInfo(path);

            if(!this->checkDependenciesExistens(pluginFile.absoluteDir().absolutePath(),pMetaData)){
                return;
            }

            QString destPath;

            if (pMetaData->dependeciesPath.at(i).toObject().value("path").toString()=="app"){
                destPath = QString(pluginsDir.absolutePath() +"/"+fileInfo.fileName());
            }else{
                destPath = QString(pMetaData->dependeciesPath.at(i).toObject().value("path").toString()+"/"+fileInfo.fileName());;
            }

            //check if source is file or folder
            if (fileInfo.isFile()){

                    if(QFileInfo::exists(destPath)){
                        QFile::remove(destPath);
                    }
                    QFile::copy(path,destPath);

                    emit sendString(destPath);
                    count += 1;

            }else if(fileInfo.isDir()){

                    this->copyDir(path,destPath);
                    count += 1;

            }else{
                    this->makeErrorMsg(QString("error (copy failed): "  + fileInfo.fileName() + " unknown file format"));
                    emit copyFinished(false);
                    return;
            }


            emit copyCount(count);

        }
    }

    QDir desti(pluginsDir.absolutePath()+"/plugins/");
    desti.mkpath(desti.absolutePath());

    QString copyString = QString(desti.absolutePath()+"/"+pluginName);

    emit sendMsg("copy: "  + pluginName + " to " +copyString,false);

    bool success = QFile::copy(pluginFile.absoluteFilePath(),copyString);

    if (success){
        emit sendString("plugin file successfully copied");

        pMetaData->path = copyString;
        pMetaData->emitSendMe();
    }else{
        emit sendErrorMsg("File not copied successfully");
        emit copyFinished(false);
    }



    emit copyFinished(true);
}

void PluginCopier::setPaths(PluginMetaData *pluginMeta, QString filename)
{
    pMetaData = pluginMeta;
    pluginPath = filename;
}

void PluginCopier::sendMsg(QString msg, bool error)
{
    if(error){
       this->makeErrorMsg(msg);
    }else{
       emit sendString(msg);
    }

}

void PluginCopier::sendCopyFinished(bool check)
{
    emit copyFinished(check);
}

bool PluginCopier::copyDir(QString sourcePath, QString destinationPath)
{
    QDir desti;
    desti.mkpath(destinationPath);
    QDir source=sourcePath;

    QStringList flist=source.entryList(QDir::NoDotAndDotDot | QDir::Files);
    QStringList dlist = source.entryList(QDir::NoDotAndDotDot | QDir::Dirs);


    for(int i = 0; i<flist.size();i++){

        if(QFileInfo::exists(destinationPath+"/"+flist.at(i))){
            QFile::remove(destinationPath+"/"+flist.at(i));
        }

        if(!QFile::copy(sourcePath+"/"+flist.at(i),destinationPath+"/"+flist.at(i))){
            return false;
        }else{
            emit sendString(destinationPath+"/"+flist.at(i));
        }

    }

    foreach(QString dir,dlist){
        copyDir(sourcePath+"/"+dir,destinationPath+"/"+dir);
    }

    return true;
}

void PluginCopier::makeErrorMsg(QString msg)
{
    emit sendErrorMsg("<font color=red>"+ msg +"</font>");
}

bool PluginCopier::checkDependenciesExistens(QString path, PluginMetaData *pluginMeta)
{
    for(int i = 0; i < pluginMeta->dependeciesPath.size();i++){

        //source path
        QString p(path + "/" +  pluginMeta->dependeciesPath.at(i).toObject().value("name").toString());
        QFileInfo fileInfo(p);

        if(!fileInfo.exists()){
            this->makeErrorMsg(QString("dependence "+ fileInfo.fileName() + " does not exist"));
            return false;
        }
    }

    return true;

}
