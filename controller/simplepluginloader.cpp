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

        QFileInfo fileInfo(this->path);
        QString relPath(QString("plugins/%1").arg(fileInfo.fileName())); // save relative path
        if(this->pluginCopier.savePlugin(relPath)) {
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
        this->log(QString("step 0/2: try to import %1").arg(this->path));
        this->pluginCopier.importPlugin(this->path);
    } else {
        this->log(QString("file do not exists: %1").arg(this->path));
    }
}
