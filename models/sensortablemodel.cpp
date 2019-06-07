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

    //get row and column index
    int rowIndex = index.row();
    int columnIndex = index.column();

    //get plugin and sensor
    sdb::Plugin currentPlugin;
    sdb::Sensor currentSensor;
    int numSensorPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all function of the current plugin
        bool sensorFound = false;
        foreach(const sdb::Sensor &sensor, plugin.sensors){

            //if the current sensor corresponds to the given row index
            if(rowIndex == numSensorPlugins){
                currentPlugin = plugin;
                currentSensor = sensor;
                sensorFound = true;
                break;
            }
            numSensorPlugins++;

        }

        if(sensorFound){
            break;
        }

    }

    if(role == Qt::DisplayRole){

        switch(columnIndex){
        case 0:
            return currentSensor.name;
        case 1:
            return currentPlugin.name;
        case 2:
            return currentSensor.iid;
        }

    }else if(role == Qt::BackgroundRole){

        //active sensor
        if(this->selectedSensor.first.compare(currentPlugin.name) == 0
                && this->selectedSensor.second.compare(currentSensor.name) == 0){
            return QColor(QColor::fromCmykF(0.59, 0.40, 0.10, 0.10).lighter());
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
QPair<QString, QString> SensorTableModel::getSelectedSensorPlugin() const{
    return this->selectedSensor;
}

/*!
 * \brief SensorTableModel::selectSensorPlugin
 * \param index
 */
void SensorTableModel::selectSensorPlugin(const QModelIndex &index){

    //check index
    if(!index.isValid()){
        return;
    }

    //get row index
    int rowIndex = index.row();

    //get type of the right sensor
    int numSensorPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all sensor of the current plugin
        foreach(const sdb::Sensor &sensor, plugin.sensors){

            //if the current function corresponds to the given row index
            if(numSensorPlugins == rowIndex){

                //check if sConfig is different from the current selection
                if(this->selectedSensor.first.compare(plugin.name) == 0
                        && this->selectedSensor.second.compare(sensor.name) == 0){
                    return;
                }

                this->selectedSensor.first = plugin.name;
                this->selectedSensor.second = sensor.name;

                this->updateModel();

                emit this->selectedSensorPluginChanged();

                return;

            }

            numSensorPlugins++;
        }

    }

}

/*!
 * \brief SensorTableModel::selectSensorPlugin
 * \param sConfig
 */
void SensorTableModel::selectSensorPlugin(const SensorConfiguration &sConfig){

    //check if sConfig is different from the current selection
    if(this->selectedSensor.first.compare(sConfig.getPluginName()) == 0
            && this->selectedSensor.second.compare(sConfig.getSensorName()) == 0){
        return;
    }

    this->selectedSensor.first = sConfig.getPluginName();
    this->selectedSensor.second = sConfig.getSensorName();

    this->updateModel();

    emit this->selectedSensorPluginChanged();

}

/*!
 * \brief SensorTableModel::selectSensorPlugin
 * \param sensorName
 * \param pluginName
 */
void SensorTableModel::selectSensorPlugin(const QString &sensorName, const QString &pluginName){

    //check if sConfig is different from the current selection
    if(this->selectedSensor.first.compare(pluginName) == 0
            && this->selectedSensor.second.compare(sensorName) == 0){
        return;
    }

    this->selectedSensor.first = pluginName;
    this->selectedSensor.second = sensorName;

    this->updateModel();

    emit this->selectedSensorPluginChanged();

}

/*!
 * \brief SensorTableModel::getSensorType
 * \return
 */
SensorTypes SensorTableModel::getSensorType() const{

    //get type of the right sensor
    int numSensorPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all sensor of the current plugin
        foreach(const sdb::Sensor &sensor, plugin.sensors){

            //if the current function corresponds to the given row index
            if(plugin.name.compare(this->selectedSensor.first) == 0
                    && sensor.name.compare(this->selectedSensor.second) == 0){

                return OiMetaData::getSensorTypeEnum(sensor.iid);

            }

            numSensorPlugins++;
        }

    }

    return eUndefinedSensor;

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

    //get rowIndex
    int rowIndex = index.row();

    //get plugin and sensor index
    QModelIndex pluginIndex = this->index(rowIndex, 1);
    QModelIndex sensorIndex = this->index(rowIndex, 0);
    if(!pluginIndex.isValid() || !sensorIndex.isValid()){
        return eUndefinedSensor;
    }

    //get plugin and sensor name
    QString pluginName = this->data(pluginIndex).toString();
    QString sensorName = this->data(sensorIndex).toString();

    //get type of the right sensor
    int numSensorPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all sensor of the current plugin
        foreach(const sdb::Sensor &sensor, plugin.sensors){

            //if the current function corresponds to the given row index
            if(plugin.name.compare(pluginName) == 0
                    && sensor.name.compare(sensorName) == 0){

                return OiMetaData::getSensorTypeEnum(sensor.iid);

            }

            numSensorPlugins++;
        }

    }

    return eUndefinedSensor;

}

/*!
 * \brief SensorTableModel::getSensorDescription
 * \return
 */
QString SensorTableModel::getSensorDescription() const{

    //get description of the right sensor
    int numSensorPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all sensor of the current plugin
        foreach(const sdb::Sensor &sensor, plugin.sensors){

            //if the current function corresponds to the given row index
            if(plugin.name.compare(this->selectedSensor.first) == 0
                    && sensor.name.compare(this->selectedSensor.second) == 0){

                return sensor.description;

            }

            numSensorPlugins++;
        }

    }

    return "";

}

/*!
 * \brief SensorTableModel::getSensorDescription
 * \param index
 * \return
 */
QString SensorTableModel::getSensorDescription(const QModelIndex &index) const{

    //check index
    if(!index.isValid()){
        return "UndefinedSensor";
    }

    //get rowIndex
    int rowIndex = index.row();

    //get plugin and sensor index
    QModelIndex pluginIndex = this->index(rowIndex, 1);
    QModelIndex sensorIndex = this->index(rowIndex, 0);
    if(!pluginIndex.isValid() || !sensorIndex.isValid()){
        return "UndefinedSensor";
    }

    //get plugin and sensor name
    QString pluginName = this->data(pluginIndex).toString();
    QString sensorName = this->data(sensorIndex).toString();

    //get description of the right sensor
    int numSensorPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all sensor of the current plugin
        foreach(const sdb::Sensor &sensor, plugin.sensors){

            //if the current function corresponds to the given row index
            if(plugin.name.compare(pluginName) == 0
                    && sensor.name.compare(sensorName) == 0){

                return sensor.description;

            }

            numSensorPlugins++;
        }

    }

    return "";

}

/*!
 * \brief SensorTableModel::getPluginFilePath
 * \return
 */
QString SensorTableModel::getPluginFilePath() const{

    //get plugin file path of the right sensor
    int numSensorPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all sensor of the current plugin
        foreach(const sdb::Sensor &sensor, plugin.sensors){

            //if the current function corresponds to the given row index
            if(plugin.name.compare(this->selectedSensor.first) == 0
                    && sensor.name.compare(this->selectedSensor.second) == 0){

                return plugin.file_path;

            }

            numSensorPlugins++;
        }

    }

    return "";

}

/*!
 * \brief SensorTableModel::getPluginFilePath
 * \param index
 * \return
 */
QString SensorTableModel::getPluginFilePath(const QModelIndex &index) const{

    //check index
    if(!index.isValid()){
        return "UndefinedSensor";
    }

    //get rowIndex
    int rowIndex = index.row();

    //get plugin and sensor index
    QModelIndex pluginIndex = this->index(rowIndex, 1);
    QModelIndex sensorIndex = this->index(rowIndex, 0);
    if(!pluginIndex.isValid() || !sensorIndex.isValid()){
        return "UndefinedSensor";
    }

    //get plugin and sensor name
    QString pluginName = this->data(pluginIndex).toString();
    QString sensorName = this->data(sensorIndex).toString();

    //get description of the right sensor
    int numSensorPlugins = 0;
    foreach(const sdb::Plugin &plugin, this->plugins){

        //run through all sensor of the current plugin
        foreach(const sdb::Sensor &sensor, plugin.sensors){

            //if the current function corresponds to the given row index
            if(plugin.name.compare(pluginName) == 0
                    && sensor.name.compare(sensorName) == 0){

                return plugin.file_path;

            }

            numSensorPlugins++;
        }

    }

    return "";

}

/*!
 * \brief SensorTableModel::getSupportedReadingTypes
 * \return
 */
QList<ReadingTypes> SensorTableModel::getSupportedReadingTypes() const{

    QList<ReadingTypes> types;

    //get and check sensor name and plugin file path
    QString sensorName = this->selectedSensor.second;
    QString filePath = this->getPluginFilePath();
    if(sensorName.compare("") == 0 || filePath.compare("") == 0){
        return types;
    }

    //load and check sensor plugin
    QPointer<Sensor> sensor = PluginLoader::loadSensorPlugin(filePath, sensorName);
    if(sensor.isNull()){
        return types;
    }

    //get reading types and delete sensor
    types = sensor->getSupportedReadingTypes();
    delete sensor;

    return types;

}

/*!
 * \brief SensorTableModel::getSupportedSensorActions
 * \return
 */
QList<SensorFunctions> SensorTableModel::getSupportedSensorActions() const{

    QList<SensorFunctions> functions;

    //get and check sensor name and plugin file path
    QString sensorName = this->selectedSensor.second;
    QString filePath = this->getPluginFilePath();
    if(sensorName.compare("") == 0 || filePath.compare("") == 0){
        return functions;
    }

    //load and check sensor plugin
    QPointer<Sensor> sensor = PluginLoader::loadSensorPlugin(filePath, sensorName);
    if(sensor.isNull()){
        return functions;
    }

    //get functions and delete sensor
    functions =  sensor->getSupportedSensorActions();
    delete sensor;

    return functions;

}

/*!
 * \brief SensorTableModel::getSupportedConnectionTypes
 * \return
 */
QList<ConnectionTypes> SensorTableModel::getSupportedConnectionTypes() const{

    QList<ConnectionTypes> types;

    //get and check sensor name and plugin file path
    QString sensorName = this->selectedSensor.second;
    QString filePath = this->getPluginFilePath();
    if(sensorName.compare("") == 0 || filePath.compare("") == 0){
        return types;
    }

    //load and check sensor plugin
    QPointer<Sensor> sensor = PluginLoader::loadSensorPlugin(filePath, sensorName);
    if(sensor.isNull()){
        return types;
    }

    //get connection types and delete sensor
    types =  sensor->getSupportedConnectionTypes();
    delete sensor;

    return types;

}

/*!
 * \brief SensorTableModel::getDefaultSensorConfig
 * Sets up
 * \return
 */
SensorConfiguration SensorTableModel::getDefaultSensorConfig() const{

    SensorConfiguration sConfig;

    //get and check sensor name and plugin file path
    QString sensorName = this->selectedSensor.second;
    QString filePath = this->getPluginFilePath();
    if(sensorName.compare("") == 0 || filePath.compare("") == 0){
        return sConfig;
    }

    //load and check sensor plugin
    QPointer<Sensor> sensor = PluginLoader::loadSensorPlugin(filePath, sensorName);
    if(sensor.isNull()){
        return sConfig;
    }

    //set general attributes
    sConfig.setName(sensor->getMetaData().name);
    sConfig.setTypeOfSensor(getSensorType());
    sConfig.setPluginName(sensor->getMetaData().pluginName);
    sConfig.setSensorName(sensor->getMetaData().name);

    //set accuracy
    sConfig.setAccuracy(sensor->getDefaultAccuracy());

    //set sensor parameters
    sConfig.setIntegerParameter(sensor->getIntegerParameter());
    sConfig.setDoubleParameter(sensor->getDoubleParameter());
    sConfig.setAvailableStringParameter(sensor->getStringParameter());
    QStringList keys = sensor->getStringParameter().keys();
    QMap<QString, QString> stringParams;
    foreach(const QString &key, keys){
        stringParams.insert(key, sensor->getStringParameter().value(key));
    }
    sConfig.setStringParameter(stringParams);

    //set connection type
    ConnectionConfig cConfig;
    if(sensor->getSupportedConnectionTypes().size() > 0){
        cConfig.typeOfConnection = sensor->getSupportedConnectionTypes().first();
    }
    sConfig.setConnectionConfig(cConfig);

    delete sensor;

    return sConfig;

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
