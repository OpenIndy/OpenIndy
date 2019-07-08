#include "simplepluginloader.h"

SimplePluginLoader::SimplePluginLoader(QString path)
{
    this->path = path;

    QObject::connect(&pluginCopier, &PluginCopier::sendMessage, this, &SimplePluginLoader::log, Qt::AutoConnection);
    QObject::connect(&pluginCopier, &PluginCopier::importFinished, this, &SimplePluginLoader::importFinished, Qt::AutoConnection);
}

void SimplePluginLoader::importFinished(bool success) {
    if(success) {
        this->log("step 1/2: plugin successfully copied");
        if(this->pluginCopier.savePlugin(this->path)) {
            this->log("step 2/2: plugin successfully saved in the database");
        }
    }

    this->log("could not import plugin.");
}

void SimplePluginLoader::log(const QString &msg) {
    qDebug() << msg;
    std::cout << msg.toUtf8().data() << "\n";
    std::cout.flush();
}

void SimplePluginLoader::importPlugin() {
    if(QFile::exists(this->path)){
        this->pluginCopier.importPlugin(this->path);
    }
    this->log(QString("file do not exists: %1").arg(this->path));
}
