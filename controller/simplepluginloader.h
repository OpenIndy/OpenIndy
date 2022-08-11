#ifndef SIMPLEPLUGINLOADER_H
#define SIMPLEPLUGINLOADER_H

#include <QObject>
#include <iostream>
#include <QDebug>
#include "plugincopier.h"

#if defined(OI_MAIN_LIB)
#  define OI_MAIN_EXPORT Q_DECL_EXPORT
#else
#  define OI_MAIN_EXPORT Q_DECL_IMPORT
#endif

class OI_MAIN_EXPORT SimplePluginLoader : public QObject
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
