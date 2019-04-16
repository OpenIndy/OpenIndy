#ifndef PROJECTCONFIG_H
#define PROJECTCONFIG_H

#include <QString>
#include <QCoreApplication>
#include "types.h"

class ProjectConfig
{
public:

    static QString getImportNominalPath();
    static void setImportNominalPath(const QString &value);

    static QString getProjectPath();
    static void setProjectPath(const QString &value);

    static int getMetricUnit();
    static void setMetricUnit(int value);

    static int getAngularUnit();
    static void setAngularUnit(int value);

    static int getTemperatureUnit();
    static void setTemperatureUnit(int value);

    static int getDistanceDigits();
    static void setDistanceDigits(int value);

    static int getAngularDigits();
    static void setAngularDigits(int value);

    static int getTemperatureDigits();
    static void setTemperatureDigits(int value);

    static bool getUseSounds();
    static void setUseSounds(bool value);

    static int getAutoSaveInterval();
    static void setAutoSaveInterval(int value);

    //********************************************
    //save and load functions for all config files
    //********************************************

    static bool loadProjectPathConfigFile();
    static bool saveProjectPathConfigFile();

    static bool loadProjectSettingsConfigFile();
    static bool saveprojectSettingsConfigFile();

private:

    static QString importNominalPath;
    static QString projectPath;

    static int metricUnit;
    static int angularUnit;
    static int temperatureUnit;

    static int distanceDigits;
    static int angularDigits;
    static int temperatureDigits;

    static bool useSounds;

    static int autoSaveInterval;
};

#endif // PROJECTCONFIG_H
