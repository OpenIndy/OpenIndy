#ifndef PLUGINMETADATA_H
#define PLUGINMETADATA_H

#include <QJsonArray>

class PluginMetaData
{
public:
    QString iid;
    QString name;
    QString pluginName;
    QString pluginVersion;
    QString author;
    QString compiler;
    QString operatingSystem;
    bool dependencies;
    QJsonArray dependenciesPath;
    QString description;
    QString path;
    bool isValid;
    QString pluginType;

};

#endif // PLUGINMETADATA_H
