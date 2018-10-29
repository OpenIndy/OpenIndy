#include "projectconfig.h"

#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>


QString ProjectConfig::importNominalPath;
QString ProjectConfig::projectPath;

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
    saveConfigFile();
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
    saveConfigFile();
}

/*!
 * \brief ProjectConfig::loadConfigFile
 * \return
 */
bool ProjectConfig::loadConfigFile()
{
    QFile file("/config/projectConfigs/projectConfig.xml");
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
        importNominalPath = importPathElem.nodeValue();

        QDomElement projectPathElem = root.firstChildElement("projectPath");
        if(projectPathElem.isNull()){
            return false;
        }
        projectPath = projectPathElem.nodeValue();

        return true;
    }
    return false;
}

/*!
 * \brief ProjectConfig::saveConfigFile
 * \return
 */
bool ProjectConfig::saveConfigFile()
{
    QDomDocument xmlDoc;
    QDomElement root = xmlDoc.createElement("projectConfig");

    QDomElement importPathElem = xmlDoc.createElement("importNominalPath");
    QDomText importPathElemText = xmlDoc.createTextNode(importNominalPath);
    importPathElem.appendChild(importPathElemText);
    root.appendChild(importPathElem);

    QDomElement projectPathElem = xmlDoc.createElement("projectPath");
    QDomText projectPathElemText = xmlDoc.createTextNode(projectPath);
    projectPathElem.appendChild(projectPathElemText);
    root.appendChild(projectPathElem);

    xmlDoc.appendChild(root);

    QFile file(QCoreApplication::applicationDirPath() + "/config/projectConfigs/projectConfig.xml");
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
