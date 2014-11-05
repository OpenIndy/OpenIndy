#ifndef PLUGINCOPIER_H
#define PLUGINCOPIER_H

#include <QThread>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileInfo>
#include "pluginmetadata.h"

class PluginCopier : public QThread
{
    Q_OBJECT
public:
    explicit PluginCopier(QObject *parent = 0);

    void run();


signals:
    void copyCount(int i);
    void sendErrorMsg(QString msg);
    void sendString(QString msg);
    void copyFinished(bool check);

public slots:
    void setPaths(PluginMetaData *pluginMeta, QString destinationPath,QString pluginDir);

private:
    PluginMetaData *pMetaData;
    QString dPath;
    QString pDir;

    bool copyDir(QString sourcePath, QString destinationPath);

};

#endif // PLUGINCOPIER_H
