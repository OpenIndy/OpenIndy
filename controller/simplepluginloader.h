#ifndef SIMPLEPLUGINLOADER_H
#define SIMPLEPLUGINLOADER_H

#include <QObject>
#include <iostream>
#include <fstream>
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
    ~SimplePluginLoader();

    void importPlugin();

private slots:
    void log(const QString &msg, const MessageTypes &msgType = eInformationMessage, const MessageDestinations &msgDest = eConsoleMessage);
    void importFinished(bool);

private:
    QString path;
    PluginCopier pluginCopier;
    std::ofstream logfile;

};

#endif // SIMPLEPLUGINLOADER_H
