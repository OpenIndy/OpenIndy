#include "defaultsconfiginit.h"
#include <QJsonDocument>
#include "systemdbmanager.h"

DefaultsConfigInit::DefaultsConfigInit(QObject *parent) : QObject(parent)
{

}

void DefaultsConfigInit::init(QString path) {
    QFile jsonFile(path);
    if(!jsonFile.open(QFile::ReadOnly | QFile::Text)) {
        emit sendMessage("can not open config/defaults.json", MessageTypes::eWarningMessage);
        return;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonFile.readAll(), &parseError);
    if(parseError.error != QJsonParseError::NoError){
        emit sendMessage("can not parse config/defaults.json", MessageTypes::eWarningMessage);
        return;
    }

    QMap<QString, QString> functionFilepath;
    foreach(sdb::Function f, SystemDbManager::getFunctions()) {
        functionFilepath.insert(f.name, f.plugin.file_path);
    }

    QJsonObject config = doc.object();
    foreach(QJsonValue v, config.value("createFeature").toArray()) {
        QJsonObject c = v.toObject();
        QString featureType = c.value("featureType").toString();
        QString function = c.value("function").toString();
        QString measurementConfig = c.value("measurementConfig").toString();
        // invalid parameters will ignored by SystemDbManager
        SystemDbManager::setDefaultFunction(getFeatureTypeEnum(featureType), function, functionFilepath.value(function));
        SystemDbManager::setDefaultMeasurementConfig(measurementConfig, featureType);
    }
}
