#ifndef PROJECTCONFIG_H
#define PROJECTCONFIG_H

#include <QString>
#include <QCoreApplication>
#include <QDebug>

class ProjectConfig
{
public:

    static QString getImportNominalPath();
    static void setImportNominalPath(const QString &value);

    static QString getProjectPath();
    static void setProjectPath(const QString &value);

    static bool loadConfigFile();
    static bool saveConfigFile();

private:

    static QString importNominalPath;
    static QString projectPath;
};

#endif // PROJECTCONFIG_H
