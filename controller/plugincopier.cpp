#include "plugincopier.h"

PluginCopier::PluginCopier(QObject *parent) :
    QThread(parent)
{
}

void PluginCopier::run()
{

    int count = 1;

    for(int i = 0; i < pMetaData->dependeciesPath.size();i++){

        //source path
        QString path(pDir + "/" +  pMetaData->dependeciesPath.at(i).toObject().value("name").toString());
        QFileInfo fileInfo(path);


        QString destPath;

        if (pMetaData->dependeciesPath.at(i).toObject().value("path").toString()=="app"){
            destPath = QString(dPath +"/"+fileInfo.fileName());
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
            emit sendErrorMsg(QString("error (copy failed): "  + fileInfo.fileName() + " unknown file format"));
            emit copyFinished(false);
        }



        emit copyCount(count);

    }

    emit copyFinished(true);
}

void PluginCopier::setPaths(PluginMetaData *pluginMeta, QString destinationPath, QString pluginDir)
{
    pMetaData = pluginMeta;
    dPath = destinationPath;
    pDir = pluginDir;
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
