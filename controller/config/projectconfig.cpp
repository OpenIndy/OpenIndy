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

QList<oi::FeatureDisplayAttributes> ProjectConfig::displayColumns;

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
}

int ProjectConfig::getAutoSaveInterval()
{
    return autoSaveInterval;
}

void ProjectConfig::setAutoSaveInterval(int value)
{
    autoSaveInterval = value;
}

QList<oi::FeatureDisplayAttributes> ProjectConfig::getDisplayColumns() {
    return displayColumns;
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

        // TODO auf xpath umstellen
        QDomElement projectFeatureViewSettingsElem = root.firstChildElement("projectFeatureViewSettings");
        if(!projectFeatureViewSettingsElem.isNull()){
            QDomElement columnsElem = projectFeatureViewSettingsElem.firstChildElement("columns");
            if(!columnsElem.isNull()) {
                QDomNodeList columns = columnsElem.elementsByTagName("column");
                for (int i=0; i<columns.length(); i++) {
                    QDomElement colum = columns.at(i).toElement();

                    // TODO dynamisch bestimmen, ist z. Z. nicht mögliche, da die enums nicht als Q_ENUMS bzw. Q_ENUM deklariert sind
                    const QString name = colum.attribute("name");
                    if(QString::compare(name, "Type", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayType);
                    } else if(QString::compare(name, "Name", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayName);
                    } else if(QString::compare(name, "Comment", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayComment);
                    } else if(QString::compare(name, "Group", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayGroup);
                    } else if(QString::compare(name, "IsSolved", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayIsSolved);
                    } else if(QString::compare(name, "IsUpdated", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayIsUpdated);
                    } else if(QString::compare(name, "Functions", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayFunctions);
                    } else if(QString::compare(name, "UsedFor", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayUsedFor);
                    } else if(QString::compare(name, "PreviouslyNeeded", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayPreviouslyNeeded);
                    } else if(QString::compare(name, "StDev", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayStDev);
                    } else if(QString::compare(name, "MeasurementConfig", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayMeasurementConfig);
                    } else if(QString::compare(name, "Observations", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayObservations);
                    } else if(QString::compare(name, "IsCommon", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayIsCommon);
                    } else if(QString::compare(name, "IsActual", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayIsActual);
                    } else if(QString::compare(name, "X", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayX);
                    } else if(QString::compare(name, "Y", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayY);
                    } else if(QString::compare(name, "Z", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayZ);
                    } else if(QString::compare(name, "PrimaryI", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayPrimaryI);
                    } else if(QString::compare(name, "PrimaryJ", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayPrimaryJ);
                    } else if(QString::compare(name, "PrimaryK", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayPrimaryK);
                    } else if(QString::compare(name, "RadiusA", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayRadiusA);
                    } else if(QString::compare(name, "RadiusB", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayRadiusB);
                    } else if(QString::compare(name, "SecondaryI", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplaySecondaryI);
                    } else if(QString::compare(name, "SecondaryJ", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplaySecondaryJ);
                    } else if(QString::compare(name, "SecondaryK", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplaySecondaryK);
                    } else if(QString::compare(name, "Aperture", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayAperture);
                    } else if(QString::compare(name, "A", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayA);
                    } else if(QString::compare(name, "B", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayB);
                    } else if(QString::compare(name, "C", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayC);
                    } else if(QString::compare(name, "Angle", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayAngle);
                    } else if(QString::compare(name, "Distance", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayDistance);
                    } else if(QString::compare(name, "MeasurementSeries", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayMeasurementSeries);
                    } else if(QString::compare(name, "Temperature", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayTemperature);
                    } else if(QString::compare(name, "Length", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayLength);
                    } else if(QString::compare(name, "ExpansionOriginX", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayExpansionOriginX);
                    } else if(QString::compare(name, "ExpansionOriginY", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayExpansionOriginY);
                    } else if(QString::compare(name, "ExpansionOriginZ", Qt::CaseInsensitive) == 0) {
                         displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayExpansionOriginZ);
                    } else if(QString::compare(name, "FormError", Qt::CaseInsensitive) == 0) {
                        displayColumns.append(oi::FeatureDisplayAttributes::eFeatureDisplayFormError);
                    }
                }
            }
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

    QDomElement projectFeatureViewSettingsElem = xmlDoc.createElement("projectFeatureViewSettings");
    QDomElement columnsElem = xmlDoc.createElement("columns");
    projectFeatureViewSettingsElem.appendChild(columnsElem);

    QList<oi::FeatureDisplayAttributes>::iterator it;
    for(it = displayColumns.begin(); it != displayColumns.end(); ++it) {
        const oi::FeatureDisplayAttributes& fda = *it;
        QDomElement column = xmlDoc.createElement("column");
        switch(fda) {
        case (oi::FeatureDisplayAttributes::eFeatureDisplayType):
            column.setAttribute("name", "Type");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayName):
            column.setAttribute("name", "Name");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayComment):
            column.setAttribute("name", "Comment");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayGroup):
            column.setAttribute("name", "Group");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayIsSolved):
            column.setAttribute("name", "IsSolved");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayIsUpdated):
            column.setAttribute("name", "IsUpdated");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayFunctions):
            column.setAttribute("name", "Functions");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayUsedFor):
            column.setAttribute("name", "UsedFor");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayPreviouslyNeeded):
            column.setAttribute("name", "PreviouslyNeeded");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayStDev):
            column.setAttribute("name", "StDev");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayMeasurementConfig):
            column.setAttribute("name", "MeasurementConfig");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayObservations):
            column.setAttribute("name", "Observations");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayIsCommon):
            column.setAttribute("name", "IsCommon");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayIsActual):
            column.setAttribute("name", "IsActual");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayX):
            column.setAttribute("name", "X");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayY):
            column.setAttribute("name", "Y");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayZ):
            column.setAttribute("name", "Z");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayPrimaryI):
            column.setAttribute("name", "PrimaryI");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayPrimaryJ):
            column.setAttribute("name", "PrimaryJ");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayPrimaryK):
            column.setAttribute("name", "PrimaryK");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayRadiusA):
            column.setAttribute("name", "RadiusA");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayRadiusB):
            column.setAttribute("name", "RadiusB");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplaySecondaryI):
            column.setAttribute("name", "SecondaryI");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplaySecondaryJ):
            column.setAttribute("name", "SecondaryJ");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplaySecondaryK):
            column.setAttribute("name", "SecondaryK");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayAperture):
            column.setAttribute("name", "Aperture");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayA):
            column.setAttribute("name", "A");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayB):
            column.setAttribute("name", "B");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayC):
            column.setAttribute("name", "C");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayAngle):
            column.setAttribute("name", "Angle");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayDistance):
            column.setAttribute("name", "Distance");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayMeasurementSeries):
            column.setAttribute("name", "MeasurementSeries");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayTemperature):
            column.setAttribute("name", "Temperature");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayLength):
            column.setAttribute("name", "Length");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayExpansionOriginX):
            column.setAttribute("name", "ExpansionOriginX");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayExpansionOriginY):
            column.setAttribute("name", "ExpansionOriginY");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayExpansionOriginZ):
            column.setAttribute("name", "ExpansionOriginZ");
            break;
        case (oi::FeatureDisplayAttributes::eFeatureDisplayFormError):
            column.setAttribute("name", "FormError");
            break;
        }
        columnsElem.appendChild(column);

    }
    root.appendChild(projectFeatureViewSettingsElem);

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

