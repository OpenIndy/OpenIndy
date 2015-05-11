#include "modelmanager.h"

//single model manager instance
QPointer<ModelManager> ModelManager::myInstance = new ModelManager();

//init static variables
QPointer<OiJob> ModelManager::currentJob;
FeatureTableModel ModelManager::featureTableModel;
FeatureTableProxyModel ModelManager::featureTableProxyModel;
FeatureTableColumnConfig ModelManager::featureTableColumnConfig;
TrafoParamTableColumnConfig ModelManager::trafoParamTableColumnConfig;
ParameterDisplayConfig ModelManager::parameterDisplayConfig;
QStringListModel ModelManager::coordinateSystemsModel;
QStringListModel ModelManager::nominalSystemsModel;
QStringListModel ModelManager::metricUnitTypesModel;
QStringListModel ModelManager::angularUnitTypesModel;
QStringListModel ModelManager::temperatureUnitTypesModel;
QStringListModel ModelManager::dummyStringListModel;
QStringListModel ModelManager::pluginNamesModel;
QStringListModel ModelManager::groupNamesModel;
FeatureTreeViewModel ModelManager::featureTreeViewModel;
AvailableElementsTreeViewProxyModel ModelManager::availableElementsTreeViewProxyModel;
UsedElementsModel ModelManager::usedElementsModel;
FunctionTableModel ModelManager::functionTableModel;
FunctionTableProxyModel ModelManager::functionTableProxyModel;
ActiveFeatureFunctionsModel ModelManager::activeFeatureFunctionsModel;
QPointer<SensorConfigurationManager> ModelManager::sensorConfigManager;
SensorTableModel ModelManager::sensorTableModel;
SensorTableProxyModel ModelManager::sensorTableProxyModel;
SensorConfigurationModel ModelManager::sensorConfigurationModel;
SensorConfigurationProxyModel ModelManager::sensorConfigurationProxyModel;
QStringListModel ModelManager::sensorTypeNamesModel;
QList<sdb::Plugin> ModelManager::plugins;
QStandardItemModel ModelManager::baudRateTypesModel;
QStandardItemModel ModelManager::dataBitTypesModel;
QStandardItemModel ModelManager::flowControlTypesModel;
QStandardItemModel ModelManager::parityTypesModel;
QStandardItemModel ModelManager::stopBitTypesModel;
QStandardItemModel ModelManager::availableSerialPortsModel;
QStandardItemModel ModelManager::availableIpAdressesModel;
QStringListModel ModelManager::readingTypeNamesModel;
PluginTreeViewModel ModelManager::pluginTreeViewModel;
TrafoParamTableProxyModel ModelManager::trafoParamTableProxyModel;

/*!
 * \brief ModelManager::ModelManager
 */
ModelManager::ModelManager(QObject *parent) : QObject(parent){

}

/*!
 * \brief ModelManager::init
 */
void ModelManager::init(){

    //init models
    ModelManager::initFeatureTableModels();
    ModelManager::initFeatureTreeViewModels();
    ModelManager::initFunctionTableModels();
    ModelManager::initUnitTypesModels();
    ModelManager::initPluginModels();
    ModelManager::initSensorTableModels();
    ModelManager::initSensorListViewModels();
    ModelManager::initSensorTypeNamesModel();
    ModelManager::initSensorConnectionModels();
    ModelManager::initReadingTypesModels();
    ModelManager::initGroupsModel();

}

/*!
 * \brief ModelManager::getCurrentJob
 * \return
 */
const QPointer<OiJob> &ModelManager::getCurrentJob(){
    return ModelManager::currentJob;
}

/*!
 * \brief ModelManager::setCurrentJob
 * \param job
 */
void ModelManager::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){
        ModelManager::currentJob = job;
        ModelManager::updateJob();
    }
}

/*!
 * \brief ModelManager::getSensorConfigManager
 * \return
 */
const QPointer<SensorConfigurationManager> &ModelManager::getSensorConfigManager(){
    return ModelManager::sensorConfigManager;
}

/*!
 * \brief ModelManager::setSensorConfigManager
 * \param sensorConfigManager
 */
void ModelManager::setSensorConfigManager(const QPointer<SensorConfigurationManager> &sensorConfigManager){
    ModelManager::sensorConfigManager = sensorConfigManager;
    ModelManager::updateSensorConfigManager();
}

/*!
 * \brief ModelManager::getFeatureTableColumnConfig
 * \return
 */
const FeatureTableColumnConfig &ModelManager::getFeatureTableColumnConfig(){
    return ModelManager::featureTableColumnConfig;
}

/*!
 * \brief ModelManager::setFeatureTableColumnConfig
 * \param config
 */
void ModelManager::setFeatureTableColumnConfig(const FeatureTableColumnConfig &config){
    ModelManager::featureTableColumnConfig = config;
    ModelManager::updateFeatureTableColumnConfig();
}

/*!
 * \brief ModelManager::getTrafoParamTableColumnConfig
 * \return
 */
const TrafoParamTableColumnConfig &ModelManager::getTrafoParamTableColumnConfig(){
    return ModelManager::trafoParamTableColumnConfig;
}

/*!
 * \brief ModelManager::setTrafoParamColumnConfig
 * \param config
 */
void ModelManager::setTrafoParamColumnConfig(const TrafoParamTableColumnConfig &config){
    ModelManager::trafoParamTableColumnConfig = config;
    ModelManager::updateTrafoParamTableColumnConfig();
}

/*!
 * \brief ModelManager::getParameterDisplayConfig
 * \return
 */
const ParameterDisplayConfig &ModelManager::getParameterDisplayConfig(){
    return ModelManager::parameterDisplayConfig;
}

/*!
 * \brief ModelManager::setParameterDisplayConfig
 * \param config
 */
void ModelManager::setParameterDisplayConfig(const ParameterDisplayConfig &config){
    ModelManager::parameterDisplayConfig = config;
    ModelManager::updateParameterDisplayConfig();
}

/*!
 * \brief ModelManager::getFeatureTableModel
 * \return
 */
FeatureTableModel &ModelManager::getFeatureTableModel(){
    return ModelManager::featureTableModel;
}

/*!
 * \brief ModelManager::getFeatureTableProxyModel
 * \return
 */
FeatureTableProxyModel &ModelManager::getFeatureTableProxyModel(){
    return ModelManager::featureTableProxyModel;
}

/*!
 * \brief ModelManager::getTrafoParamTableProxyModel
 * \return
 */
TrafoParamTableProxyModel &ModelManager::getTrafoParamTableProxyModel(){
    return ModelManager::trafoParamTableProxyModel;
}

/*!
 * \brief ModelManager::getFeatureTreeViewModel
 * \return
 */
FeatureTreeViewModel &ModelManager::getFeatureTreeViewModel(){
    return ModelManager::featureTreeViewModel;
}

/*!
 * \brief ModelManager::getAvailableElementsTreeViewProxyModel
 * \return
 */
AvailableElementsTreeViewProxyModel &ModelManager::getAvailableElementsTreeViewProxyModel(){
    return ModelManager::availableElementsTreeViewProxyModel;
}

/*!
 * \brief ModelManager::getUsedElementsModel
 * \return
 */
UsedElementsModel &ModelManager::getUsedElementsModel(){
    return ModelManager::usedElementsModel;
}

/*!
 * \brief ModelManager::getCoordinateSystemsModel
 * \return
 */
QStringListModel &ModelManager::getCoordinateSystemsModel(){
    return ModelManager::coordinateSystemsModel;
}

/*!
 * \brief ModelManager::getNominalSystemsModel
 * \return
 */
QStringListModel &ModelManager::getNominalSystemsModel(){
    return ModelManager::nominalSystemsModel;
}

/*!
 * \brief ModelManager::getGroupNamesModel
 * \return
 */
QStringListModel &ModelManager::getGroupNamesModel(){
    return ModelManager::groupNamesModel;
}

/*!
 * \brief ModelManager::getUnitTypesModel
 * \param dimension
 * \return
 */
QStringListModel &ModelManager::getUnitTypesModel(const DimensionType &dimension){
    switch(dimension){
    case eMetric:
        return ModelManager::metricUnitTypesModel;
    case eAngular:
        return ModelManager::angularUnitTypesModel;
    case eTemperature:
        return ModelManager::temperatureUnitTypesModel;
    default:
        return ModelManager::dummyStringListModel;
    }
}

/*!
 * \brief ModelManager::getPluginNamesModel
 * \return
 */
QStringListModel &ModelManager::getPluginNamesModel(){
    return ModelManager::pluginNamesModel;
}

/*!
 * \brief ModelManager::getPluginTreeViewModel
 * \return
 */
PluginTreeViewModel &ModelManager::getPluginTreeViewModel(){
    return ModelManager::pluginTreeViewModel;
}

/*!
 * \brief ModelManager::getReadingTypeNamesModel
 * \return
 */
QStringListModel &ModelManager::getReadingTypeNamesModel(){
    return ModelManager::readingTypeNamesModel;
}

/*!
 * \brief ModelManager::getFunctionTableModel
 * \return
 */
FunctionTableModel &ModelManager::getFunctionTableModel(){
    return ModelManager::functionTableModel;
}

/*!
 * \brief ModelManager::getFunctionTableProxyModel
 * \return
 */
FunctionTableProxyModel &ModelManager::getFunctionTableProxyModel(){
    return ModelManager::functionTableProxyModel;
}

/*!
 * \brief ModelManager::getActiveFeatureFunctionsModel
 * \return
 */
ActiveFeatureFunctionsModel &ModelManager::getActiveFeatureFunctionsModel(){
    return ModelManager::activeFeatureFunctionsModel;
}

/*!
 * \brief ModelManager::getSensorTypeNamesModel
 * \return
 */
QStringListModel &ModelManager::getSensorTypeNamesModel(){
    return ModelManager::sensorTypeNamesModel;
}

/*!
 * \brief ModelManager::getSensorTableModel
 * \return
 */
SensorTableModel &ModelManager::getSensorTableModel(){
    return ModelManager::sensorTableModel;
}

/*!
 * \brief ModelManager::getSensorTableProxyModel
 * \return
 */
SensorTableProxyModel &ModelManager::getSensorTableProxyModel(){
    return ModelManager::sensorTableProxyModel;
}

/*!
 * \brief ModelManager::getSensorConfigurationModel
 * \return
 */
SensorConfigurationModel &ModelManager::getSensorConfigurationModel(){
    return ModelManager::sensorConfigurationModel;
}

/*!
 * \brief ModelManager::getSensorConfigurationProxyModel
 * \return
 */
SensorConfigurationProxyModel &ModelManager::getSensorConfigurationProxyModel(){
    return ModelManager::sensorConfigurationProxyModel;
}

/*!
 * \brief ModelManager::getBaudRateTypesModel
 * \return
 */
QStandardItemModel &ModelManager::getBaudRateTypesModel(){
    return ModelManager::baudRateTypesModel;
}

/*!
 * \brief ModelManager::getDataBitTypesModel
 * \return
 */
QStandardItemModel &ModelManager::getDataBitTypesModel(){
    return ModelManager::dataBitTypesModel;
}

/*!
 * \brief ModelManager::getFlowControlTypesModel
 * \return
 */
QStandardItemModel &ModelManager::getFlowControlTypesModel(){
    return ModelManager::flowControlTypesModel;
}

/*!
 * \brief ModelManager::getParityTypesModel
 * \return
 */
QStandardItemModel &ModelManager::getParityTypesModel(){
    return ModelManager::parityTypesModel;
}

/*!
 * \brief ModelManager::getStopBitTypesModel
 * \return
 */
QStandardItemModel &ModelManager::getStopBitTypesModel(){
    return ModelManager::stopBitTypesModel;
}

/*!
 * \brief ModelManager::getAvailableSerialPortsModel
 * \return
 */
QStandardItemModel &ModelManager::getAvailableSerialPortsModel(){
    return ModelManager::availableSerialPortsModel;
}

/*!
 * \brief ModelManager::getAvailableIpAdressesModel
 * \return
 */
QStandardItemModel &ModelManager::getAvailableIpAdressesModel(){
    return ModelManager::availableIpAdressesModel;
}

/*!
 * \brief ModelManager::getExchangeSimpleAsciiNames
 * \param pluginName
 * \return
 */
QPointer<QStringListModel> ModelManager::getExchangeSimpleAsciiNames(const QString &pluginName){

    QPointer<QStringListModel> model(NULL);

    QStringList exchanges;
    foreach(const sdb::Plugin &plugin, ModelManager::plugins){

        //check if the plugin is the one that was queried and add all simple ascii exchanges
        if(plugin.name.compare(pluginName) == 0){
            foreach(const sdb::Exchange &exchange, plugin.exchanges){
                if(exchange.iid.compare(OiMetaData::iid_ExchangeSimpleAscii) == 0){
                    exchanges.append(exchange.name);
                }
            }
        }

    }
    model = new QStringListModel(exchanges);

    return model;

}

/*!
 * \brief ModelManager::getExchangeSimpleAsciiDelimiters
 * \param pluginName
 * \param exchangeName
 * \return
 */
QPointer<QStringListModel> ModelManager::getExchangeSimpleAsciiDelimiters(const QString &pluginName, const QString &exchangeName){

    QPointer<QStringListModel> model(NULL);

    QStringList delimiters;

    //get plugin file path
    QString path;
    foreach(const sdb::Plugin &plugin, ModelManager::plugins){
        if(plugin.name.compare(pluginName) == 0){
            path = plugin.file_path;
            break;
        }
    }

    //get and check exchange plugin
    QPointer<ExchangeSimpleAscii> exchange = PluginLoader::loadExchangeSimpleAsciiPlugin(path, exchangeName);
    if(!exchange.isNull()){
        delimiters = exchange->getSupportedDelimiters();
    }

    model = new QStringListModel(delimiters);

    return model;

}

/*!
 * \brief ModelManager::getExchangeSimpleAsciiSupportedGeometries
 * \param pluginName
 * \param exchangeName
 * \return
 */
QPointer<QStringListModel> ModelManager::getExchangeSimpleAsciiSupportedGeometries(const QString &pluginName, const QString &exchangeName){

    QPointer<QStringListModel> model(NULL);

    QStringList supportedGeometries;

    //get plugin file path
    QString path;
    foreach(const sdb::Plugin &plugin, ModelManager::plugins){
        if(plugin.name.compare(pluginName) == 0){
            path = plugin.file_path;
            break;
        }
    }

    //get and check exchange plugin
    QPointer<ExchangeSimpleAscii> exchange = PluginLoader::loadExchangeSimpleAsciiPlugin(path, exchangeName);
    if(!exchange.isNull()){
        foreach(const GeometryTypes &type, exchange->getSupportedGeometries()){
            supportedGeometries.append(getGeometryTypeName(type));
        }
    }

    model = new QStringListModel(supportedGeometries);

    return model;

}

/*!
 * \brief ModelManager::featureSetChanged
 */
void ModelManager::featureSetChanged(){

    //check the current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //resort table model
    ModelManager::featureTableProxyModel.sort(0);

}

/*!
 * \brief ModelManager::coordSystemSetChanged
 */
void ModelManager::coordSystemSetChanged(){

    //check the current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //update coordinate systems model
    ModelManager::updateCoordinateSystemsModel();

    //update nominal systems model
    ModelManager::updateNominalSystemsModel();

}

/*!
 * \brief ModelManager::stationSetChanged
 */
void ModelManager::stationSetChanged(){

    //check the current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //update coordinate systems model
    ModelManager::updateCoordinateSystemsModel();

}

/*!
 * \brief ModelManager::availableGroupsChanged
 */
void ModelManager::availableGroupsChanged(){

    //check the current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //update groups model
    ModelManager::updateGroupsModel();

}

/*!
 * \brief ModelManager::featureNameChanged
 * \param featureId
 * \param oldName
 */
void ModelManager::featureNameChanged(const int &featureId, const QString &oldName){

    //check the current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //resort table model
    ModelManager::featureTableProxyModel.sort(0);

}

/*!
 * \brief ModelManager::sensorConfigurationsChanged
 */
/*void ModelManager::sensorConfigurationsChanged(){

    ModelManager::updateSensorConfigNamesModel();

}*/

/*!
 * \brief ModelManager::updateJob
 * Passes the new job to all static models
 */
void ModelManager::updateJob(){

    //check the current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //pass the job to all static models that need it
    ModelManager::featureTableModel.setCurrentJob(ModelManager::currentJob);
    ModelManager::featureTreeViewModel.setCurrentJob(ModelManager::currentJob);
    ModelManager::activeFeatureFunctionsModel.setCurrentJob(ModelManager::currentJob);
    ModelManager::functionTableProxyModel.setCurrentJob(ModelManager::currentJob);
    ModelManager::usedElementsModel.setCurrentJob(ModelManager::currentJob);
    ModelManager::availableElementsTreeViewProxyModel.setCurrentJob(ModelManager::currentJob);

    //connect the job to slots in model manager
    QObject::connect(ModelManager::currentJob.data(), &OiJob::coordSystemSetChanged, ModelManager::myInstance.data(), &ModelManager::coordSystemSetChanged, Qt::AutoConnection);
    QObject::connect(ModelManager::currentJob.data(), &OiJob::stationSetChanged, ModelManager::myInstance.data(), &ModelManager::stationSetChanged, Qt::AutoConnection);
    QObject::connect(ModelManager::currentJob.data(), &OiJob::availableGroupsChanged, ModelManager::myInstance.data(), &ModelManager::availableGroupsChanged, Qt::AutoConnection);
    QObject::connect(ModelManager::currentJob.data(), &OiJob::featureSetChanged, ModelManager::myInstance.data(), &ModelManager::featureSetChanged, Qt::AutoConnection);
    QObject::connect(ModelManager::currentJob.data(), &OiJob::featureNameChanged, ModelManager::myInstance.data(), &ModelManager::featureNameChanged, Qt::AutoConnection);

}

/*!
 * \brief ModelManager::updateFeatureTableColumnConfig
 */
void ModelManager::updateFeatureTableColumnConfig(){

}

/*!
 * \brief ModelManager::updateTrafoParamTableColumnConfig
 */
void ModelManager::updateTrafoParamTableColumnConfig(){

}

/*!
 * \brief ModelManager::updateParameterDisplayConfig
 * Passes the new parameter display config to all static models
 */
void ModelManager::updateParameterDisplayConfig(){

    //pass the parameter display config to all static models that need it
    ModelManager::featureTableModel.setParameterDisplayConfig(ModelManager::parameterDisplayConfig);

}

/*!
 * \brief ModelManager::updateSensorConfigManager
 */
void ModelManager::updateSensorConfigManager(){

    //pass the sensor config manager to all static models that need it
    ModelManager::sensorConfigurationModel.setSensorConfigurationManager(ModelManager::sensorConfigManager);

    //connect the config manager to slots in ModelManager
    //QObject::connect(&ModelManager::sensorConfigManager, &SensorConfigurationManager::sensorConfigurationsChanged,
    //                 ModelManager::myInstance.data(), &ModelManager::sensorConfigurationsChanged, Qt::AutoConnection);

}

/*!
 * \brief ModelManager::updateCoordinateSystemsModel
 */
void ModelManager::updateCoordinateSystemsModel(){

    QStringList coordSystems;

    //check current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //add all nominal systems
    foreach(const QPointer<CoordinateSystem> &system, ModelManager::currentJob->getCoordinateSystemsList()){

        //check system
        if(system.isNull()){
            continue;
        }

        //add system's name
        coordSystems.append(system->getFeatureName());

    }

    //add all station systems
    foreach(const QPointer<CoordinateSystem> &system, ModelManager::currentJob->getStationSystemsList()){

        //check system
        if(system.isNull()){
            continue;
        }

        //add system's name
        coordSystems.append(system->getFeatureName());

    }

    ModelManager::coordinateSystemsModel.setStringList(coordSystems);

}

/*!
 * \brief ModelManager::updateNominalSystemsModel
 */
void ModelManager::updateNominalSystemsModel(){

    QStringList coordSystems;

    //check current job
    if(ModelManager::currentJob.isNull()){
        return;
    }

    //add all nominal systems
    foreach(const QPointer<CoordinateSystem> &system, ModelManager::currentJob->getCoordinateSystemsList()){

        //check system
        if(system.isNull()){
            continue;
        }

        //add system's name
        coordSystems.append(system->getFeatureName());

    }

    ModelManager::nominalSystemsModel.setStringList(coordSystems);

}

/*!
 * \brief ModelManager::updateGroupsModel
 */
void ModelManager::updateGroupsModel(){

    QStringList groups;

    //get a list of all available groups
    if(!ModelManager::currentJob.isNull()){
        groups = ModelManager::currentJob->getFeatureGroupList();
    }

    //add default entry (all groups)
    groups.push_front("All Groups");

    ModelManager::groupNamesModel.setStringList(groups);

}

/*!
 * \brief ModelManager::initFeatureTableModels
 */
void ModelManager::initFeatureTableModels(){

    //assign source models
    ModelManager::featureTableProxyModel.setSourceModel(&ModelManager::featureTableModel);
    ModelManager::trafoParamTableProxyModel.setSourceModel(&ModelManager::featureTableModel);

}

/*!
 * \brief ModelManager::initFeatureTreeViewModels
 */
void ModelManager::initFeatureTreeViewModels(){

    //assign source models
    ModelManager::availableElementsTreeViewProxyModel.setSourceModel(&ModelManager::featureTreeViewModel);

    //set header data
    //ModelManager::usedElementsModel.setHeaderData(1, Qt::Horizontal, "used elements");

}

/*!
 * \brief ModelManager::initFunctionTableModels
 */
void ModelManager::initFunctionTableModels(){

    //assign source models
    ModelManager::functionTableProxyModel.setSourceModel(&ModelManager::functionTableModel);

}

/*!
 * \brief ModelManager::initSensorTypeNamesModel
 */
void ModelManager::initSensorTypeNamesModel(){

    QStringList sensorTypes;

    foreach(const SensorTypes &type, getAvailableSensorTypes()){
        sensorTypes.append(getSensorTypeName(type));
    }

    ModelManager::sensorTypeNamesModel.setStringList(sensorTypes);

}

/*!
 * \brief ModelManager::initSensorTableModels
 */
void ModelManager::initSensorTableModels(){

    //assign source models
    ModelManager::sensorTableProxyModel.setSourceModel(&ModelManager::sensorTableModel);

}

/*!
 * \brief ModelManager::initSensorListViewModels
 */
void ModelManager::initSensorListViewModels(){

    //assign source models
    ModelManager::sensorConfigurationProxyModel.setSourceModel(&ModelManager::sensorConfigurationModel);

}

/*!
 * \brief ModelManager::initSensorConnectionModels
 */
void ModelManager::initSensorConnectionModels(){

    //initialize baud rate model
    QStandardItem *baud1200 = new QStandardItem("1200");
    baud1200->setData(QSerialPort::Baud1200, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud1200);
    QStandardItem *baud2400 = new QStandardItem("2400");
    baud2400->setData(QSerialPort::Baud2400, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud2400);
    QStandardItem *baud4800 = new QStandardItem("4800");
    baud4800->setData(QSerialPort::Baud4800, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud4800);
    QStandardItem *baud9600 = new QStandardItem("9600");
    baud9600->setData(QSerialPort::Baud9600, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud9600);
    QStandardItem *baud19200 = new QStandardItem("19200");
    baud19200->setData(QSerialPort::Baud19200, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud19200);
    QStandardItem *baud38400 = new QStandardItem("38400");
    baud38400->setData(QSerialPort::Baud38400, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud38400);
    QStandardItem *baud57600 = new QStandardItem("57600");
    baud57600->setData(QSerialPort::Baud57600, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud57600);
    QStandardItem *baud115200 = new QStandardItem("115200");
    baud115200->setData(QSerialPort::Baud115200, Qt::UserRole);
    ModelManager::baudRateTypesModel.appendRow(baud115200);

    //initialize databits model
    QStandardItem *data5 = new QStandardItem("5");
    data5->setData(QSerialPort::Data5, Qt::UserRole);
    ModelManager::dataBitTypesModel.appendRow(data5);
    QStandardItem *data6 = new QStandardItem("6");
    data6->setData(QSerialPort::Data6, Qt::UserRole);
    ModelManager::dataBitTypesModel.appendRow(data6);
    QStandardItem *data7 = new QStandardItem("7");
    data7->setData(QSerialPort::Data7, Qt::UserRole);
    ModelManager::dataBitTypesModel.appendRow(data7);
    QStandardItem *data8 = new QStandardItem("8");
    data8->setData(QSerialPort::Data8, Qt::UserRole);
    ModelManager::dataBitTypesModel.appendRow(data8);

    //initialize flow control model
    QStandardItem *noControl = new QStandardItem("no flowcontrol");
    noControl->setData(QSerialPort::NoFlowControl, Qt::UserRole);
    ModelManager::flowControlTypesModel.appendRow(noControl);
    QStandardItem *hardwareControl = new QStandardItem("hardware flowcontrol");
    hardwareControl->setData(QSerialPort::HardwareControl, Qt::UserRole);
    ModelManager::flowControlTypesModel.appendRow(hardwareControl);
    QStandardItem *softwareControl = new QStandardItem("software flowcontrol");
    softwareControl->setData(QSerialPort::SoftwareControl, Qt::UserRole);
    ModelManager::flowControlTypesModel.appendRow(softwareControl);
    QStandardItem *unknownControl = new QStandardItem("unknown flowcontrol");
    unknownControl->setData(QSerialPort::UnknownFlowControl, Qt::UserRole);
    ModelManager::flowControlTypesModel.appendRow(unknownControl);

    //initialize parity model
    QStandardItem *noParity = new QStandardItem("no parity");
    noParity->setData(QSerialPort::NoParity, Qt::UserRole);
    ModelManager::parityTypesModel.appendRow(noParity);
    QStandardItem *evenParity = new QStandardItem("even parity");
    evenParity->setData(QSerialPort::EvenParity, Qt::UserRole);
    ModelManager::parityTypesModel.appendRow(evenParity);
    QStandardItem *oddParity = new QStandardItem("odd parity");
    oddParity->setData(QSerialPort::OddParity, Qt::UserRole);
    ModelManager::parityTypesModel.appendRow(oddParity);
    QStandardItem *spaceParity = new QStandardItem("space parity");
    spaceParity->setData(QSerialPort::SpaceParity, Qt::UserRole);
    ModelManager::parityTypesModel.appendRow(spaceParity);
    QStandardItem *markParity = new QStandardItem("mark parity");
    markParity->setData(QSerialPort::MarkParity, Qt::UserRole);
    ModelManager::parityTypesModel.appendRow(markParity);
    QStandardItem *unknownParity = new QStandardItem("unknown parity");
    unknownParity->setData(QSerialPort::UnknownParity, Qt::UserRole);
    ModelManager::parityTypesModel.appendRow(unknownParity);

    //initialize stop bits model
    QStandardItem *oneStop = new QStandardItem("one stop");
    oneStop->setData(QSerialPort::OneStop, Qt::UserRole);
    ModelManager::stopBitTypesModel.appendRow(oneStop);
    QStandardItem *oneHalfStop = new QStandardItem("one and half");
    oneHalfStop->setData(QSerialPort::OneAndHalfStop, Qt::UserRole);
    ModelManager::stopBitTypesModel.appendRow(oneHalfStop);
    QStandardItem *twoStop = new QStandardItem("two stop");
    twoStop->setData(QSerialPort::TwoStop, Qt::UserRole);
    ModelManager::stopBitTypesModel.appendRow(twoStop);
    QStandardItem *unknownStop = new QStandardItem("unknown stopbits");
    unknownStop->setData(QSerialPort::UnknownStopBits, Qt::UserRole);
    ModelManager::stopBitTypesModel.appendRow(unknownStop);

    //initialize serial ports model
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        QStandardItem *serialPort = new QStandardItem(info.portName());
        ModelManager::availableSerialPortsModel.appendRow(serialPort);
    }

    //initialize ip4 addresses model
    QList<QHostAddress> ipAdresses = QNetworkInterface::allAddresses();
    foreach(const QHostAddress &adress, ipAdresses){
        if(adress.protocol() == QAbstractSocket::IPv4Protocol){
            QStandardItem *ip = new QStandardItem(adress.toString());
            ModelManager::availableIpAdressesModel.appendRow(ip);
        }
    }

}

/*!
 * \brief ModelManager::initUnitTypesModels
 */
void ModelManager::initUnitTypesModels(){

    QStringList metricUnitTypes;
    metricUnitTypes.append(getUnitTypeName(eUnitInch));
    metricUnitTypes.append(getUnitTypeName(eUnitMeter));
    metricUnitTypes.append(getUnitTypeName(eUnitMilliMeter));
    ModelManager::metricUnitTypesModel.setStringList(metricUnitTypes);

    QStringList angularUnitTypes;
    angularUnitTypes.append(getUnitTypeName(eUnitArcSeconds));
    angularUnitTypes.append(getUnitTypeName(eUnitDecimalDegree));
    angularUnitTypes.append(getUnitTypeName(eUnitGon));
    angularUnitTypes.append(getUnitTypeName(eUnitMilliRadians));
    angularUnitTypes.append(getUnitTypeName(eUnitRadiant));
    ModelManager::angularUnitTypesModel.setStringList(angularUnitTypes);

    QStringList temperatureUnitTypes;
    temperatureUnitTypes.append(getUnitTypeName(eUnitFahrenheit));
    temperatureUnitTypes.append(getUnitTypeName(eUnitGrad));
    temperatureUnitTypes.append(getUnitTypeName(eUnitKelvin));
    ModelManager::temperatureUnitTypesModel.setStringList(temperatureUnitTypes);

}

/*!
 * \brief ModelManager::initReadingTypesModels
 */
void ModelManager::initReadingTypesModels(){

    QStringList readingTypes;
    readingTypes.append(getReadingTypeName(eCartesianReading));
    readingTypes.append(getReadingTypeName(eDistanceReading));
    readingTypes.append(getReadingTypeName(ePolarReading));
    readingTypes.append(getReadingTypeName(eDirectionReading));
    readingTypes.append(getReadingTypeName(eTemperatureReading));
    readingTypes.append(getReadingTypeName(eLevelReading));
    ModelManager::readingTypeNamesModel.setStringList(readingTypes);

}

/*!
 * \brief ModelManager::initPluginModels
 */
void ModelManager::initPluginModels(){

    //get plugins from database
    ModelManager::plugins = SystemDbManager::getPlugins();

    //pass the plugins list to all static models that need it
    ModelManager::functionTableModel.setPlugins(ModelManager::plugins);
    ModelManager::sensorTableModel.setPlugins(ModelManager::plugins);
    ModelManager::pluginTreeViewModel.setPlugins(ModelManager::plugins);

    //update plugin names model
    QStringList pluginNames;
    foreach(const sdb::Plugin &plugin, plugins){
        pluginNames.append(plugin.name);
    }
    ModelManager::pluginNamesModel.setStringList(pluginNames);

}

/*!
 * \brief ModelManager::initGroupsModel
 */
void ModelManager::initGroupsModel(){

    ModelManager::updateGroupsModel();

}
