#ifndef PLUGINMETADATA_H
#define PLUGINMETADATA_H

#include <QObject>
#include <QJsonArray>

class PluginMetaData : public QObject
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
    QJsonArray dependeciesPath;
    QString description;
    QString path;
    bool isValid;
    QString pluginType;
    bool alreadyExists;

};

#endif // PLUGINMETADATA_H
