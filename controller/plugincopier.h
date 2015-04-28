#ifndef PLUGINCOPIER_H
#define PLUGINCOPIER_H

#include <QObject>
#include <QDir>
#include <QApplication>
#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileInfo>
#include <QPluginLoader>

#include "pluginmetadata.h"
#include "oimetadata.h"
#include "systemdbmanager.h"

class PluginCopier : public QObject
{
    Q_OBJECT

public:
    explicit PluginCopier(QObject *parent = 0);

public slots:

    //########################
    //import or remove plugins
    //########################

    void importPlugin(const QString &path);
    void deletePlugin(const QString &name);

signals:

    //#####################
    //inform about progress
    //#####################

    void updateProgress(const int &progress);
    void sendMessage(const QString &msg);
    void sendError(const QString &error);
    void importFinished(const bool &success);
    void deletionFinished(const bool &success);

private:

    //##############
    //helper methods
    //##############

    bool checkPluginMetaData(PluginMetaData &metaData, const QString &path);

    bool copyDir(const QString &sourcePath, const QString &destinationPath);
    bool checkDependencies(const QString &sourcePath, const PluginMetaData &metaData);

    bool savePlugin(const QString &path);

};

#endif // PLUGINCOPIER_H
