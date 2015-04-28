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
    QJsonArray dependenciesPath;
    QString description;
    QString path;
    bool isValid;
    QString pluginType;

};

#endif // PLUGINMETADATA_H
