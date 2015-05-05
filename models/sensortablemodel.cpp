#include "sensortablemodel.h"

/*!
 * \brief SensorTableModel::SensorTableModel
 * \param parent
 */
SensorTableModel::SensorTableModel(QObject *parent) : QAbstractTableModel(parent){

}

/*!
 * \brief SensorTableModel::rowCount
 * \param parent
 * \return
 */
int SensorTableModel::rowCount(const QModelIndex &parent) const{

    int numSensorPlugins = 0;

    foreach(const sdb::Plugin &plugin, this->plugins){
        numSensorPlugins += plugin.sensors.size();
    }

    return numSensorPlugins;

}

/*!
 * \brief SensorTableModel::columnCount
 * \param parent
 * \return
 */
int SensorTableModel::columnCount(const QModelIndex &parent) const{
    return 3;
}

/*!
 * \brief SensorTableModel::data
 * \param index
 * \param role
 * \return
 */
QVariant SensorTableModel::data(const QModelIndex &index, int role) const{

    //check model index
    if(!index.isValid()){
        return QVariant();
    }

    //set background for selected index
    if(this->selectedIndex.row() == index.row() && role == Qt::BackgroundRole){
        return QColor(QColor::fromCmykF(0.59, 0.40, 0.10, 0.10).lighter());
    }

    //check role
    if(role != Qt::DisplayRole){
        return QVariant();
    }

    int numSensorPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all function of the current plugin
        foreach(const sdb::Sensor &sensor, plugin.sensors){

            //if the current sensor corresponds to the given row index
            if(index.row() == numSensorPlugins){

                if(index.column() == 0){
                    return sensor.name;
                }else if(index.column() == 1){
                    return plugin.name;
                }else if(index.column() == 2){
                    return sensor.iid;
                }

            }

            numSensorPlugins++;
        }

    }

    return QVariant();

}

/*!
 * \brief SensorTableModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant SensorTableModel::headerData(int section, Qt::Orientation orientation, int role) const{

    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        if(section == 0){
            return "sensor";
        }else if(section == 1){
            return "plugin";
        }else if(section == 2){
            return "iid";
        }

    }

    return QVariant();

}

/*!
 * \brief SensorTableModel::getSelectedIndex
 * \return
 */
QModelIndex SensorTableModel::getSelectedIndex() const{
    return this->selectedIndex;
}

/*!
 * \brief SensorTableModel::selectSensorPlugin
 * \param sensorName
 * \param pluginName
 */
void SensorTableModel::selectSensorPlugin(const QString &sensorName, const QString &pluginName){

    //get the right sensor
    int numSensorPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all sensor of the current plugin
        foreach(const sdb::Sensor &sensor, plugin.sensors){

            //if the current sensor corresponds to the given row index
            if(sensor.name.compare(sensorName) == 0 && plugin.name.compare(pluginName) == 0){

                this->selectedIndex = this->createIndex(numSensorPlugins, 0);
                this->updateModel();
                return;

            }

            numSensorPlugins++;
        }

    }

}

/*!
 * \brief SensorTableModel::getSensorDescription
 * \param index
 * \return
 */
QString SensorTableModel::getSensorDescription(const QModelIndex &index) const{

    //check index
    if(!index.isValid()){
        return "";
    }

    //get description of the right sensor
    int numSensorPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all sensor of the current plugin
        foreach(const sdb::Sensor &sensor, plugin.sensors){

            //if the current sensor corresponds to the given row index
            if(index.row() == numSensorPlugins){

                return sensor.description;

            }

            numSensorPlugins++;
        }

    }

    return "";

}

/*!
 * \brief SensorTableModel::getSensorName
 * \param index
 * \return
 */
QString SensorTableModel::getSensorName(const QModelIndex &index) const{

    //check index
    if(!index.isValid()){
        return "";
    }

    //get description of the right sensor
    int numSensorPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all sensor of the current plugin
        foreach(const sdb::Sensor &sensor, plugin.sensors){

            //if the current function corresponds to the given row index
            if(index.row() == numSensorPlugins){

                return sensor.name;

            }

            numSensorPlugins++;
        }

    }

    return "";

}

/*!
 * \brief SensorTableModel::getPluginName
 * \param index
 * \return
 */
QString SensorTableModel::getPluginName(const QModelIndex &index) const{

    //check index
    if(!index.isValid()){
        return "";
    }

    //get description of the right sensor
    int numSensorPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all sensor of the current plugin
        foreach(const sdb::Sensor &sensor, plugin.sensors){

            //if the current function corresponds to the given row index
            if(index.row() == numSensorPlugins){

                return plugin.name;

            }

            numSensorPlugins++;
        }

    }

    return "";

}

/*!
 * \brief SensorTableModel::getSensorType
 * \param index
 * \return
 */
SensorTypes SensorTableModel::getSensorType(const QModelIndex &index) const{

    //check index
    if(!index.isValid()){
        return eUndefinedSensor;
    }

    //get description of the right sensor
    int numSensorPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all sensor of the current plugin
        foreach(const sdb::Sensor &sensor, plugin.sensors){

            //if the current function corresponds to the given row index
            if(index.row() == numSensorPlugins){

                return OiMetaData::getSensorTypeEnum(sensor.iid);

            }

            numSensorPlugins++;
        }

    }

    return eUndefinedSensor;

}

/*!
 * \brief SensorTableModel::getPluginFilePath
 * \param index
 * \return
 */
QString SensorTableModel::getPluginFilePath(const QModelIndex &index) const{

    //check index
    if(!index.isValid()){
        return "";
    }

    //get description of the right sensor
    int numSensorPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all sensor of the current plugin
        foreach(const sdb::Sensor &sensor, plugin.sensors){

            //if the current sensor corresponds to the given row index
            if(index.row() == numSensorPlugins){

                return plugin.file_path;

            }

            numSensorPlugins++;
        }

    }

    return "";

}

/*!
 * \brief SensorTableModel::getSupportedConnectionTypes
 * \param index
 * \return
 */
QList<ConnectionTypes> SensorTableModel::getSupportedConnectionTypes(const QModelIndex &index){

    //get and check sensor name and plugin file path
    QString sensorName = this->getSensorName(index);
    QString filePath = this->getPluginFilePath(index);
    if(sensorName.compare("") == 0 || filePath.compare("") == 0){
        return QList<ConnectionTypes>();
    }

    //load and check sensor plugin
    QPointer<Sensor> sensor = PluginLoader::loadSensorPlugin(filePath, sensorName);
    if(sensor.isNull()){
        return QList<ConnectionTypes>();
    }

    QList<ConnectionTypes> types = sensor->getSupportedConnectionTypes();

    delete sensor;

    return types;

}

/*!
 * \brief SensorTableModel::getAccuracy
 * \param index
 * \return
 */
Accuracy SensorTableModel::getAccuracy(const QModelIndex &index){

    return Accuracy();

}

/*!
 * \brief SensorTableModel::getIntegerParameter
 * \return
 */
QMap<QString, int> SensorTableModel::getIntegerParameter(){

    //get and check sensor name and plugin file path
    QString sensorName = this->getSensorName(this->selectedIndex);
    QString filePath = this->getPluginFilePath(this->selectedIndex);
    if(sensorName.compare("") == 0 || filePath.compare("") == 0){
        return QMap<QString, int>();
    }

    //load and check sensor plugin
    QPointer<Sensor> sensor = PluginLoader::loadSensorPlugin(filePath, sensorName);
    if(sensor.isNull()){
        return QMap<QString, int>();
    }

    QMap<QString, int> params = sensor->getIntegerParameter();

    delete sensor;

    return params;

}

/*!
 * \brief SensorTableModel::getDoubleParameter
 * \return
 */
QMap<QString, double> SensorTableModel::getDoubleParameter(){

    //get and check sensor name and plugin file path
    QString sensorName = this->getSensorName(this->selectedIndex);
    QString filePath = this->getPluginFilePath(this->selectedIndex);
    if(sensorName.compare("") == 0 || filePath.compare("") == 0){
        return QMap<QString, double>();
    }

    //load and check sensor plugin
    QPointer<Sensor> sensor = PluginLoader::loadSensorPlugin(filePath, sensorName);
    if(sensor.isNull()){
        return QMap<QString, double>();
    }

    QMap<QString, double> params = sensor->getDoubleParameter();

    delete sensor;

    return params;

}

/*!
 * \brief SensorTableModel::getStringParameter
 * \return
 */
QMultiMap<QString, QString> SensorTableModel::getStringParameter(){

    //get and check sensor name and plugin file path
    QString sensorName = this->getSensorName(this->selectedIndex);
    QString filePath = this->getPluginFilePath(this->selectedIndex);
    if(sensorName.compare("") == 0 || filePath.compare("") == 0){
        return QMultiMap<QString, QString>();
    }

    //load and check sensor plugin
    QPointer<Sensor> sensor = PluginLoader::loadSensorPlugin(filePath, sensorName);
    if(sensor.isNull()){
        return QMultiMap<QString, QString>();
    }

    QMultiMap<QString, QString> params = sensor->getStringParameter();

    delete sensor;

    return params;

}

/*!
 * \brief SensorTableModel::getPlugins
 * \return
 */
const QList<sdb::Plugin> &SensorTableModel::getPlugins() const{
    return plugins;
}

/*!
 * \brief SensorTableModel::setPlugins
 * \param plugins
 */
void SensorTableModel::setPlugins(const QList<sdb::Plugin> &plugins){
    emit this->beginResetModel();
    this->plugins = plugins;
    emit this->endResetModel();
    this->updateModel();
}

/*!
 * \brief SensorTableModel::updateModel
 */
void SensorTableModel::updateModel(){
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}
