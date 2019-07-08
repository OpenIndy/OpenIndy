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
            return;
        }
    }

    this->log("could not import plugin.");
}

void SimplePluginLoader::log(const QString &msg) {
    QTextStream ts( stdout );
    ts << msg << "\n";
}


void SimplePluginLoader::importPlugin() {
    if(QFile::exists(this->path)){
        this->pluginCopier.importPlugin(this->path);
    } else {
        this->log(QString("file do not exists: %1").arg(this->path));
    }
}
