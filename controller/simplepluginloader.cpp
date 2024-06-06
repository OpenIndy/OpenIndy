#include "simplepluginloader.h"

SimplePluginLoader::SimplePluginLoader(QString path)
{
    this->path = path;

    QObject::connect(&pluginCopier, &PluginCopier::sendMessage, this, &SimplePluginLoader::log);
    QObject::connect(&pluginCopier, &PluginCopier::importFinished, this, &SimplePluginLoader::importFinished);

    logfile = std::ofstream("SimplePluginLoader.log", ios::app);
}
SimplePluginLoader::~SimplePluginLoader() {
    if(logfile.is_open()) {
        logfile.close();
    }
}

void SimplePluginLoader::importFinished(bool success) {
    if(success) {
        this->log("step 1/2: plugin successfully copied");

        QFileInfo fileInfo(this->path);
        QString relPath(QString("plugins/%1").arg(fileInfo.fileName())); // save relative path
        if(this->pluginCopier.savePlugin(relPath)) {
            this->log("step 2/2: plugin successfully saved in the database");
        } else {
            this->log("could not save plugin to database.", eErrorMessage);
        }
    } else {
        this->log("could not import plugin.", eErrorMessage);
    }

    exit(success ? 0 : 1);
}

void SimplePluginLoader::log(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest) {
    logfile << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().data() << "] "
            << "[" << getMessageTypeName(msgType).toLocal8Bit().data() << "] "
            << msg.toLocal8Bit().data() << "\n";
}


void SimplePluginLoader::importPlugin() {
    if(QFile::exists(this->path)){
        this->log(QString("step 0/2: try to import %1").arg(this->path));
        this->pluginCopier.importPlugin(this->path);
    } else {
        this->log(QString("file do not exists: %1").arg(this->path));
    }
}
