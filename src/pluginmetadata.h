#ifndef PLUGINMETADATA_H
#define PLUGINMETADATA_H

#include <QObject>
#include <QJsonArray>

class PluginMetaData : public QObject
{
    Q_OBJECT
public:
    explicit PluginMetaData(QObject *parent = 0);

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

/*signals:
    void sendMe(PluginMetaData*);

public slots:
    void emitSendMe();*/

};

#endif // PLUGINMETADATA_H
