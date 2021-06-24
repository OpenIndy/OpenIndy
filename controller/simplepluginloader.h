#ifndef SIMPLEPLUGINLOADER_H
#define SIMPLEPLUGINLOADER_H

#include <QObject>
#include <iostream>
#include <QDebug>
#include "plugincopier.h"
#include "pluginloader.h"

class SimplePluginLoader : public QObject
{
    Q_OBJECT

public:
    SimplePluginLoader(QString path);

    void importPlugin();

private slots:
    void log(const QString &msg);
    void importFinished(bool);

private:
    QString path;
    PluginCopier pluginCopier;

};

#endif // SIMPLEPLUGINLOADER_H
