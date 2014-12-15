#include "oimodelmanager.h"

OiModelManager *OiModelManager::myInstance = NULL;

QStringListModel OiModelManager::pluginNamesModel;
QStringListModel OiModelManager::nominalSystemsModel;
QStringListModel OiModelManager::geometryTypes;
QStandardItemModel OiModelManager::sensorTypes;
QStandardItemModel OiModelManager::baudRateTypes;
QStandardItemModel OiModelManager::dataBitTypes;
QStandardItemModel OiModelManager::flowControlTypes;
QStandardItemModel OiModelManager::parityTypes;
QStandardItemModel OiModelManager::stopBitTypes;
QStandardItemModel OiModelManager::availableSerialPorts;
QStandardItemModel OiModelManager::availableIpAdresses;
//QStringListModel *OiModelManager::simpleAsciiExchangePlugins;
//QStringListModel OiModelManager::definedFormatExchangePlugins;
QStringListModel OiModelManager::distanceUnitsModel;
QStringListModel OiModelManager::angleUnitsModel;
QStringListModel OiModelManager::temperatureUnitsModel;

OiModelManager::OiModelManager(QObject *parent) : QObject(parent)
{
}

/*!
 * \brief OiModelManager::getInstance
 * \return
 */
OiModelManager *OiModelManager::getInstance(){
    if(OiModelManager::myInstance == NULL){
        OiModelManager::myInstance = new OiModelManager();
        OiModelManager::myInstance->initModels();
    }
    return OiModelManager::myInstance;
}

/*!
 * \brief OiModelManager::getPluginNamesModel
 * \return
 */
QStringListModel &OiModelManager::getPluginNamesModel(){
    return OiModelManager::pluginNamesModel;
}

/*!
 * \brief OiModelManager::getNominalSystemsModel
 * \return
 */
QStringListModel &OiModelManager::getNominalSystemsModel(){
    return OiModelManager::nominalSystemsModel;
}

/*!
 * \brief OiModelManager::getGeometryTypesModel
 * \return
 */
QStringListModel &OiModelManager::getGeometryTypesModel(){
    return OiModelManager::geometryTypes;
}

/*!
 * \brief OiModelManager::getSimpleAsciiExchangePlugins
 * \param plugin
 * \return
 */
QStringListModel *OiModelManager::getSimpleAsciiExchangePlugins(QString plugin){

    QMultiMap<QString,QString> simpleAsciiExchangePlugins = SystemDbManager::getAvailableSimpleAsciiExchangePlugins();

    QStringListModel *pluginsModel = new QStringListModel();
    QStringList exchangePlugins = simpleAsciiExchangePlugins.values(plugin);
    pluginsModel->setStringList(exchangePlugins);

    return pluginsModel;
}

/*!
 * \brief OiModelManager::getSensorTypes
 * \return
 */
QStandardItemModel &OiModelManager::getSensorTypes(){
    return OiModelManager::sensorTypes;
}

/*!
 * \brief OiModelManager::getBaudRateTypes
 * \return
 */
QStandardItemModel &OiModelManager::getBaudRateTypes(){
    return OiModelManager::baudRateTypes;
}

/*!
 * \brief OiModelManager::getDataBitTypes
 * \return
 */
QStandardItemModel &OiModelManager::getDataBitTypes(){
    return OiModelManager::dataBitTypes;
}

/*!
 * \brief OiModelManager::getFlowControlTypes
 * \return
 */
QStandardItemModel &OiModelManager::getFlowControlTypes(){
    return OiModelManager::flowControlTypes;
}

/*!
 * \brief OiModelManager::getParityTypes
 * \return
 */
QStandardItemModel &OiModelManager::getParityTypes(){
    return OiModelManager::parityTypes;
}

/*!
 * \brief OiModelManager::getStopBitTypes
 * \return
 */
QStandardItemModel &OiModelManager::getStopBitTypes(){
    return OiModelManager::stopBitTypes;
}

/*!
 * \brief OiModelManager::getAvailableSerialPorts
 * \return
 */
QStandardItemModel &OiModelManager::getAvailableSerialPorts(){
    return OiModelManager::availableSerialPorts;
}

/*!
 * \brief OiModelManager::getAvailableIpAdresses
 * \return
 */
QStandardItemModel &OiModelManager::getAvailableIpAdresses(){
    return OiModelManager::availableIpAdresses;
}

/*!
 * \brief OiModelManager::getDistanceUnitsModel
 * \return
 */
QStringListModel &OiModelManager::getDistanceUnitsModel(){
    return OiModelManager::distanceUnitsModel;
}

/*!
 * \brief OiModelManager::getAngleUnitsModel
 * \return
 */
QStringListModel &OiModelManager::getAngleUnitsModel(){
    return OiModelManager::angleUnitsModel;
}

/*!
 * \brief OiModelManager::getTemperatureUnitsModel
 * \return
 */
QStringListModel &OiModelManager::getTemperatureUnitsModel(){
    return OiModelManager::temperatureUnitsModel;
}

/*!
 * \brief OiModelManager::initModels
 * Initialize all models once when OpenIndy is opened
 */
void OiModelManager::initModels(){

    //initialize exchange plugin models
    QStringList pluginNames = SystemDbManager::getAvailablePluginNames();
    OiModelManager::pluginNamesModel.setStringList(pluginNames);

    //initialize coordinate system models
    QStringList nominalSystems;
    QList<CoordinateSystem *> mySystems = OiFeatureState::getCoordinateSystems();
    foreach(CoordinateSystem *c, mySystems){
        nominalSystems.append(c->getFeatureName());
    }
    OiModelManager::nominalSystemsModel.setStringList(nominalSystems);

    //initialize geometry types
    QStringList geometryTypes;
    geometryTypes.append(Configuration::sPoint);

    //initialize sensor specific models
    this->initSensorModels();

    //initialize unit models
    this->initUnitModels();

















    /*QStringList simpleAsciiExchangePlugins = SystemDbManager::getAvailableSimpleAsciiExchangePlugins();
    QStringList definedFormatExchangePlugins = SystemDbManager::getAvailableDefinedFormatExchangePlugins();
    OiModelManager::simpleAsciiExchangePlugins.setStringList(simpleAsciiExchangePlugins);
    OiModelManager::definedFormatExchangePlugins.setStringList(definedFormatExchangePlugins);*/

}

/*!
 * \brief OiModelManager::initSensorModels
 */
void OiModelManager::initSensorModels(){

    //initialize sensor types model
    QStandardItem *laserTracker = new QStandardItem("lasertracker");
    laserTracker->setData(Configuration::eLaserTracker, Qt::UserRole);
    this->sensorTypes.appendRow(laserTracker);
    QStandardItem *totalStation = new QStandardItem("totalstation");
    totalStation->setData(Configuration::eTotalStation, Qt::UserRole);
    this->sensorTypes.appendRow(totalStation);
    QStandardItem *undefinedSensor = new QStandardItem("undefined sensor");
    undefinedSensor->setData(Configuration::eUndefinedSensor, Qt::UserRole);
    this->sensorTypes.appendRow(undefinedSensor);

    //initialize baud rate model
    QStandardItem *baud1200 = new QStandardItem("1200");
    baud1200->setData(QSerialPort::Baud1200, Qt::UserRole);
    this->baudRateTypes.appendRow(baud1200);
    QStandardItem *baud2400 = new QStandardItem("2400");
    baud2400->setData(QSerialPort::Baud2400, Qt::UserRole);
    this->baudRateTypes.appendRow(baud2400);
    QStandardItem *baud4800 = new QStandardItem("4800");
    baud4800->setData(QSerialPort::Baud4800, Qt::UserRole);
    this->baudRateTypes.appendRow(baud4800);
    QStandardItem *baud9600 = new QStandardItem("9600");
    baud9600->setData(QSerialPort::Baud9600, Qt::UserRole);
    this->baudRateTypes.appendRow(baud9600);
    QStandardItem *baud19200 = new QStandardItem("19200");
    baud19200->setData(QSerialPort::Baud19200, Qt::UserRole);
    this->baudRateTypes.appendRow(baud19200);
    QStandardItem *baud38400 = new QStandardItem("38400");
    baud38400->setData(QSerialPort::Baud38400, Qt::UserRole);
    this->baudRateTypes.appendRow(baud38400);
    QStandardItem *baud57600 = new QStandardItem("57600");
    baud57600->setData(QSerialPort::Baud57600, Qt::UserRole);
    this->baudRateTypes.appendRow(baud57600);
    QStandardItem *baud115200 = new QStandardItem("115200");
    baud115200->setData(QSerialPort::Baud115200, Qt::UserRole);
    this->baudRateTypes.appendRow(baud115200);

    //initialize databits model
    QStandardItem *data5 = new QStandardItem("5");
    data5->setData(QSerialPort::Data5, Qt::UserRole);
    this->dataBitTypes.appendRow(data5);
    QStandardItem *data6 = new QStandardItem("6");
    data6->setData(QSerialPort::Data6, Qt::UserRole);
    this->dataBitTypes.appendRow(data6);
    QStandardItem *data7 = new QStandardItem("7");
    data7->setData(QSerialPort::Data7, Qt::UserRole);
    this->dataBitTypes.appendRow(data7);
    QStandardItem *data8 = new QStandardItem("8");
    data8->setData(QSerialPort::Data8, Qt::UserRole);
    this->dataBitTypes.appendRow(data8);

    //initialize flow control model
    QStandardItem *noControl = new QStandardItem("no flowcontrol");
    noControl->setData(QSerialPort::NoFlowControl, Qt::UserRole);
    this->flowControlTypes.appendRow(noControl);
    QStandardItem *hardwareControl = new QStandardItem("hardware flowcontrol");
    hardwareControl->setData(QSerialPort::HardwareControl, Qt::UserRole);
    this->flowControlTypes.appendRow(hardwareControl);
    QStandardItem *softwareControl = new QStandardItem("software flowcontrol");
    softwareControl->setData(QSerialPort::SoftwareControl, Qt::UserRole);
    this->flowControlTypes.appendRow(softwareControl);
    QStandardItem *unknownControl = new QStandardItem("unknown flowcontrol");
    unknownControl->setData(QSerialPort::UnknownFlowControl, Qt::UserRole);
    this->flowControlTypes.appendRow(unknownControl);

    //initialize parity model
    QStandardItem *noParity = new QStandardItem("no parity");
    noParity->setData(QSerialPort::NoParity, Qt::UserRole);
    this->parityTypes.appendRow(noParity);
    QStandardItem *evenParity = new QStandardItem("even parity");
    evenParity->setData(QSerialPort::EvenParity, Qt::UserRole);
    this->parityTypes.appendRow(evenParity);
    QStandardItem *oddParity = new QStandardItem("odd parity");
    oddParity->setData(QSerialPort::OddParity, Qt::UserRole);
    this->parityTypes.appendRow(oddParity);
    QStandardItem *spaceParity = new QStandardItem("space parity");
    spaceParity->setData(QSerialPort::SpaceParity, Qt::UserRole);
    this->parityTypes.appendRow(spaceParity);
    QStandardItem *markParity = new QStandardItem("mark parity");
    markParity->setData(QSerialPort::MarkParity, Qt::UserRole);
    this->parityTypes.appendRow(markParity);
    QStandardItem *unknownParity = new QStandardItem("unknown parity");
    unknownParity->setData(QSerialPort::UnknownParity, Qt::UserRole);
    this->parityTypes.appendRow(unknownParity);

    //initialize stop bits model
    QStandardItem *oneStop = new QStandardItem("one stop");
    oneStop->setData(QSerialPort::OneStop, Qt::UserRole);
    this->stopBitTypes.appendRow(oneStop);
    QStandardItem *oneHalfStop = new QStandardItem("one and half");
    oneHalfStop->setData(QSerialPort::OneAndHalfStop, Qt::UserRole);
    this->stopBitTypes.appendRow(oneHalfStop);
    QStandardItem *twoStop = new QStandardItem("two stop");
    twoStop->setData(QSerialPort::TwoStop, Qt::UserRole);
    this->stopBitTypes.appendRow(twoStop);
    QStandardItem *unknownStop = new QStandardItem("unknown stopbits");
    unknownStop->setData(QSerialPort::UnknownStopBits, Qt::UserRole);
    this->stopBitTypes.appendRow(unknownStop);

    //initialize serial ports model
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        QStandardItem *serialPort = new QStandardItem(info.portName());
        this->availableSerialPorts.appendRow(serialPort);
    }

    //initialize ip4 addresses model
    QList<QHostAddress> ipAdresses = QNetworkInterface::allAddresses();
    foreach(const QHostAddress &adress, ipAdresses){
        if(adress.protocol() == QAbstractSocket::IPv4Protocol){
            QStandardItem *ip = new QStandardItem(adress.toString());
            this->availableIpAdresses.appendRow(ip);
        }
    }

}

/*!
 * \brief OiModelManager::initUnitModels
 */
void OiModelManager::initUnitModels(){

    //distance units
    QStringList distanceUnits;
    distanceUnits.append(UnitConverter::getUnitString(UnitConverter::eMETER));
    distanceUnits.append(UnitConverter::getUnitString(UnitConverter::eMILLIMETER));
    distanceUnits.append(UnitConverter::getUnitString(UnitConverter::eInch));
    OiModelManager::distanceUnitsModel.setStringList(distanceUnits);

    //angle units
    QStringList angleUnits;
    angleUnits.append(UnitConverter::getUnitString(UnitConverter::eDECIMALDEGREE));
    angleUnits.append(UnitConverter::getUnitString(UnitConverter::eGON));
    angleUnits.append(UnitConverter::getUnitString(UnitConverter::eRADIANT));
    angleUnits.append(UnitConverter::getUnitString(UnitConverter::eMilliRadians));
    angleUnits.append(UnitConverter::getUnitString(UnitConverter::eArcSeconds));
    OiModelManager::angleUnitsModel.setStringList(angleUnits);

    //temperature units
    QStringList temperatureUnits;
    temperatureUnits.append(UnitConverter::getUnitString(UnitConverter::eGRAD));
    temperatureUnits.append(UnitConverter::getUnitString(UnitConverter::eKelvin));
    temperatureUnits.append(UnitConverter::getUnitString(UnitConverter::eFahrenheit));
    OiModelManager::temperatureUnitsModel.setStringList(temperatureUnits);

}
