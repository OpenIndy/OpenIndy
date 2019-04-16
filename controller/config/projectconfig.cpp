#include "projectconfig.h"

#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

QString ProjectConfig::importNominalPath;
QString ProjectConfig::projectPath;

int ProjectConfig::metricUnit;
int ProjectConfig::angularUnit;
int ProjectConfig::temperatureUnit;

int ProjectConfig::distanceDigits;
int ProjectConfig::angularDigits;
int ProjectConfig::temperatureDigits;

bool ProjectConfig::useSounds;
int ProjectConfig::autoSaveInterval;

/*!
 * \brief ProjectConfig::getImportNominalPath
 * \return
 */
QString ProjectConfig::getImportNominalPath()
{
    return importNominalPath;
}

/*!
 * \brief ProjectConfig::setImportNominalPath
 * \param value
 */
void ProjectConfig::setImportNominalPath(const QString &value)
{
    importNominalPath = value;
    saveProjectPathConfigFile();
}

/*!
 * \brief ProjectConfig::getProjectPath
 * \return
 */
QString ProjectConfig::getProjectPath()
{
    return projectPath;
}

/*!
 * \brief ProjectConfig::setProjectPath
 * \param value
 */
void ProjectConfig::setProjectPath(const QString &value)
{
    projectPath = value;
    saveProjectPathConfigFile();
}

/*!
 * \brief ProjectConfig::getMetricUnit
 * \return
 */
int ProjectConfig::getMetricUnit()
{
    return metricUnit;
}

/*!
 * \brief ProjectConfig::setMetricUnit
 * \param value
 */
void ProjectConfig::setMetricUnit(int value)
{
    metricUnit = value;
    saveprojectSettingsConfigFile();
}

/*!
 * \brief ProjectConfig::getAngularUnit
 * \return
 */
int ProjectConfig::getAngularUnit()
{
    return angularUnit;
}

/*!
 * \brief ProjectConfig::setAngularUnit
 * \param value
 */
void ProjectConfig::setAngularUnit(int value)
{
    angularUnit = value;
    saveprojectSettingsConfigFile();
}

/*!
 * \brief ProjectConfig::getTemperatureUnit
 * \return
 */
int ProjectConfig::getTemperatureUnit()
{
    return temperatureUnit;
}

/*!
 * \brief ProjectConfig::setTemperatureUnit
 * \param value
 */
void ProjectConfig::setTemperatureUnit(int value)
{
    temperatureUnit = value;
    saveprojectSettingsConfigFile();
}

/*!
 * \brief ProjectConfig::getDistanceDigits
 * \return
 */
int ProjectConfig::getDistanceDigits()
{
    return distanceDigits;
}

/*!
 * \brief ProjectConfig::setDistanceDigits
 * \param value
 */
void ProjectConfig::setDistanceDigits(int value)
{
    distanceDigits = value;
    saveprojectSettingsConfigFile();
}

/*!
 * \brief ProjectConfig::getAngularDigits
 * \return
 */
int ProjectConfig::getAngularDigits()
{
    return angularDigits;
}

/*!
 * \brief ProjectConfig::setAngularDigits
 * \param value
 */
void ProjectConfig::setAngularDigits(int value)
{
    angularDigits = value;
    saveprojectSettingsConfigFile();
}

/*!
 * \brief ProjectConfig::getTemperatureDigits
 * \return
 */
int ProjectConfig::getTemperatureDigits()
{
    return temperatureDigits;
}

/*!
 * \brief ProjectConfig::setTemperatureDigits
 * \param value
 */
void ProjectConfig::setTemperatureDigits(int value)
{
    temperatureDigits = value;
    saveprojectSettingsConfigFile();
}

/*!
 * \brief ProjectConfig::getUseSounds
 * \return
 */
bool ProjectConfig::getUseSounds()
{
    return useSounds;
}

/*!
 * \brief ProjectConfig::setUseSounds
 * \param value
 */
void ProjectConfig::setUseSounds(bool value)
{
    useSounds = value;
    saveprojectSettingsConfigFile();
}

int ProjectConfig::getAutoSaveInterval()
{
    return autoSaveInterval;
}

void ProjectConfig::setAutoSaveInterval(int value)
{
    autoSaveInterval = value;
    saveprojectSettingsConfigFile();
}

/*!
 * \brief ProjectConfig::loadProjectPathConfigFile
 * \return
 */
bool ProjectConfig::loadProjectPathConfigFile()
{
    QFile file(QCoreApplication::applicationDirPath() + "/config/projectConfigs/projectPathConfig.xml");
    QFileInfo info(file);

    if(info.exists()){

        QDomDocument xmlDoc;
        try{
            file.open(QIODevice::ReadOnly);
            xmlDoc.setContent(&file);
            file.close();
        }catch(const std::exception &e){
            return false;
        }

        QDomElement root = xmlDoc.documentElement();

        QDomElement importPathElem = root.firstChildElement("importNominalPath");
        if(importPathElem.isNull()){
            return false;
        }
        importNominalPath = importPathElem.text();

        QDomElement projectPathElem = root.firstChildElement("projectPath");
        if(projectPathElem.isNull()){
            return false;
        }
        projectPath = projectPathElem.text();

        return true;
    }
    return false;
}

/*!
 * \brief ProjectConfig::saveProjectPathConfigFile
 * \return
 */
bool ProjectConfig::saveProjectPathConfigFile()
{
    QDomDocument xmlDoc;
    QDomElement root = xmlDoc.createElement("projectPathConfig");

    QDomElement importPathElem = xmlDoc.createElement("importNominalPath");
    QDomText importPathElemText = xmlDoc.createTextNode(importNominalPath);
    importPathElem.appendChild(importPathElemText);
    root.appendChild(importPathElem);

    QDomElement projectPathElem = xmlDoc.createElement("projectPath");
    QDomText projectPathElemText = xmlDoc.createTextNode(projectPath);
    projectPathElem.appendChild(projectPathElemText);
    root.appendChild(projectPathElem);

    xmlDoc.appendChild(root);

    QFile file(QCoreApplication::applicationDirPath() + "/config/projectConfigs/projectPathConfig.xml");
    QFileInfo info(file);

    if(info.exists()){
        file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);

        if(file.isOpen()){
            QTextStream stream(&file);
            xmlDoc.save(stream, 4);
            file.close();

            return true;
        }
    }
    return false;
}

/*!
 * \brief ProjectConfig::loadProjectSettingsConfigFile
 * \return
 */
bool ProjectConfig::loadProjectSettingsConfigFile()
{
    QFile file(QCoreApplication::applicationDirPath() + "/config/projectConfigs/projectSettingsConfig.xml");
    QFileInfo info(file);

    if(info.exists()){

        QDomDocument xmlDoc;
        try{
            file.open(QIODevice::ReadOnly);
            xmlDoc.setContent(&file);
            file.close();
        }catch(const std::exception &e){
            return false;
        }

        QDomElement root = xmlDoc.documentElement();

        QDomElement projectDistanceUnitElem = root.firstChildElement("projectDistanceUnit");
        if(projectDistanceUnitElem.isNull()){
            return false;
        }
        metricUnit = projectDistanceUnitElem.text().toInt();

        QDomElement projectAngularUnitElem = root.firstChildElement("projectAngularUnit");
        if(projectAngularUnitElem.isNull()){
            return false;
        }
        angularUnit = projectAngularUnitElem.text().toInt();

        QDomElement projectTemperatureUnitElem = root.firstChildElement("projectTemperatureUnit");
        if(projectTemperatureUnitElem.isNull()){
            return false;
        }
        temperatureUnit = projectTemperatureUnitElem.text().toInt();

        QDomElement projectDistanceDigitsElem = root.firstChildElement("projectDistanceDigits");
        if(projectDistanceDigitsElem.isNull()){
            return false;
        }
        distanceDigits = projectDistanceDigitsElem.text().toInt();

        QDomElement projectAngularDigitsElem = root.firstChildElement("projectAngularDigits");
        if(projectAngularDigitsElem.isNull()){
            return false;
        }
        angularDigits = projectAngularDigitsElem.text().toInt();

        QDomElement projectTemperatureDigitsElem = root.firstChildElement("projectTemperatureDigits");
        if(projectTemperatureDigitsElem.isNull()){
            return false;
        }
        temperatureDigits = projectTemperatureDigitsElem.text().toInt();

        QDomElement projectSoundSettingsElem = root.firstChildElement("projectSoundSettings");
        if(projectSoundSettingsElem.isNull()){
            return false;
        }
        useSounds = projectSoundSettingsElem.text().toInt();

        QDomElement projectAutoSaveIntervalElem = root.firstChildElement("projectAutoSaveInterval");
        if(projectAutoSaveIntervalElem.isNull()){
            autoSaveInterval = 10; // default minutes
        } else {
            autoSaveInterval = projectAutoSaveIntervalElem.text().toInt();
        }

        return true;
    }
    return false;
}

/*!
 * \brief ProjectConfig::saveprojectSettingsConfigFile
 * \return
 */
bool ProjectConfig::saveprojectSettingsConfigFile()
{
    QDomDocument xmlDoc;
    QDomElement root = xmlDoc.createElement("projectSettingsConfig");

    QDomElement projectDistanceUnitElem = xmlDoc.createElement("projectDistanceUnit");
    QDomText projectDistanceUnitElemText = xmlDoc.createTextNode(QString::number(metricUnit));
    projectDistanceUnitElem.appendChild(projectDistanceUnitElemText);
    root.appendChild(projectDistanceUnitElem);

    QDomElement projectAngularUnitElem = xmlDoc.createElement("projectAngularUnit");
    QDomText projectAngularUnitElemText = xmlDoc.createTextNode(QString::number(angularUnit));
    projectAngularUnitElem.appendChild(projectAngularUnitElemText);
    root.appendChild(projectAngularUnitElem);

    QDomElement projectTemperatureUnitElem = xmlDoc.createElement("projectTemperatureUnit");
    QDomText projectTemperatureUnitElemText = xmlDoc.createTextNode(QString::number(temperatureUnit));
    projectTemperatureUnitElem.appendChild(projectTemperatureUnitElemText);
    root.appendChild(projectTemperatureUnitElem);

    QDomElement projectDistanceDigitsElem = xmlDoc.createElement("projectDistanceDigits");
    QDomText projectDistanceDigitsElemText = xmlDoc.createTextNode(QString::number(distanceDigits));
    projectDistanceDigitsElem.appendChild(projectDistanceDigitsElemText);
    root.appendChild(projectDistanceDigitsElem);

    QDomElement projectAngularDigitsElem = xmlDoc.createElement("projectAngularDigits");
    QDomText projectAngularDigitsElemText = xmlDoc.createTextNode(QString::number(angularDigits));
    projectAngularDigitsElem.appendChild(projectAngularDigitsElemText);
    root.appendChild(projectAngularDigitsElem);

    QDomElement projectTemperatureDigitsElem = xmlDoc.createElement("projectTemperatureDigits");
    QDomText projectTemperatureDigitsElemText = xmlDoc.createTextNode(QString::number(temperatureDigits));
    projectTemperatureDigitsElem.appendChild(projectTemperatureDigitsElemText);
    root.appendChild(projectTemperatureDigitsElem);

    QDomElement projectSoundSettingsElem = xmlDoc.createElement("projectSoundSettings");
    QDomText projectSoundSettingsElemText = xmlDoc.createTextNode(QString::number(useSounds));
    projectSoundSettingsElem.appendChild(projectSoundSettingsElemText);
    root.appendChild(projectSoundSettingsElem);

    QDomElement projectAutoSaveIntervalElem = xmlDoc.createElement("projectAutoSaveInterval");
    projectAutoSaveIntervalElem.appendChild(xmlDoc.createTextNode(QString::number(autoSaveInterval)));
    root.appendChild(projectAutoSaveIntervalElem);

    xmlDoc.appendChild(root);

    QFile file(QCoreApplication::applicationDirPath() + "/config/projectConfigs/projectSettingsConfig.xml");
    QFileInfo info(file);

    if(info.exists()){
        file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);

        if(file.isOpen()){
            QTextStream stream(&file);
            xmlDoc.save(stream, 4);
            file.close();

            return true;
        }
    }
    return false;
}

