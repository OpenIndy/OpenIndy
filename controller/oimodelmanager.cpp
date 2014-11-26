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

    //initialize sensor types model
    QStandardItem *laserTracker = new QStandardItem("lasertracker");
    laserTracker->setData(Configuration::eLaserTracker, Qt::UserRole);
    this->sensorTypes.appendRow(laserTracker);










/*
    //add sensor types
    ui->comboBox_availableSensorTypes->insertItem(ui->comboBox_availableSensorTypes->count(),"lasertracker",Configuration::eLaserTracker);
    ui->comboBox_availableSensorTypes->insertItem(ui->comboBox_availableSensorTypes->count(),"totalstation",Configuration::eTotalStation);
    ui->comboBox_availableSensorTypes->insertItem(ui->comboBox_availableSensorTypes->count(),"others",Configuration::eUndefinedSensor);

    //add baudrates
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"1200",QSerialPort::Baud1200);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"2400",QSerialPort::Baud2400);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"4800",QSerialPort::Baud4800);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"9600",QSerialPort::Baud9600);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"19200",QSerialPort::Baud19200);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"38400",QSerialPort::Baud38400);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"57600",QSerialPort::Baud57600);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"115200",QSerialPort::Baud115200);

    //add databits
    ui->comboBox_databits->insertItem(ui->comboBox_databits->count(),"5",QSerialPort::Data5);
    ui->comboBox_databits->insertItem(ui->comboBox_databits->count(),"6",QSerialPort::Data6);
    ui->comboBox_databits->insertItem(ui->comboBox_databits->count(),"7",QSerialPort::Data7);
    ui->comboBox_databits->insertItem(ui->comboBox_databits->count(),"8",QSerialPort::Data8);
    ui->comboBox_databits->insertItem(ui->comboBox_databits->count(),"unknown",QSerialPort::UnknownDataBits);

    //add flow control
    ui->comboBox_flowcontrol->insertItem(ui->comboBox_flowcontrol->count(),"no flowcontrol",QSerialPort::NoFlowControl);
    ui->comboBox_flowcontrol->insertItem(ui->comboBox_flowcontrol->count(),"hardware flowcontrol",QSerialPort::HardwareControl);
    ui->comboBox_flowcontrol->insertItem(ui->comboBox_flowcontrol->count(),"software flowcontrol",QSerialPort::SoftwareControl);
    ui->comboBox_flowcontrol->insertItem(ui->comboBox_flowcontrol->count(),"unknown flowcontrol",QSerialPort::UnknownFlowControl);

    //add parity
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"no parity",QSerialPort::NoParity);
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"even parity",QSerialPort::EvenParity);
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"odd parity",QSerialPort::OddParity);
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"space parity",QSerialPort::SpaceParity);
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"mark parity",QSerialPort::MarkParity);
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"unknown parity",QSerialPort::UnknownParity);

    //add stopbits
    ui->comboBox_stopbits->insertItem(ui->comboBox_stopbits->count(),"one stop",QSerialPort::OneStop);
    ui->comboBox_stopbits->insertItem(ui->comboBox_stopbits->count(),"one and half stop",QSerialPort::OneAndHalfStop);
    ui->comboBox_stopbits->insertItem(ui->comboBox_stopbits->count(),"two stop",QSerialPort::TwoStop);
    ui->comboBox_stopbits->insertItem(ui->comboBox_stopbits->count(),"unknown stopbits",QSerialPort::UnknownStopBits);

    //add available serial ports
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        ui->comboBox_comPort->insertItem(ui->comboBox_comPort->count(),info.portName());
    }

    //display all iPv4 addresses
    QList<QHostAddress> ipAdresses = QNetworkInterface::allAddresses();
    foreach(const QHostAddress &adress, ipAdresses){

        if(adress.protocol() == QAbstractSocket::IPv4Protocol){
            ui->comboBox_ip->insertItem(ui->comboBox_ip->count(),adress.toString());
        }
    }
    */






    /*QStringList simpleAsciiExchangePlugins = SystemDbManager::getAvailableSimpleAsciiExchangePlugins();
    QStringList definedFormatExchangePlugins = SystemDbManager::getAvailableDefinedFormatExchangePlugins();
    OiModelManager::simpleAsciiExchangePlugins.setStringList(simpleAsciiExchangePlugins);
    OiModelManager::definedFormatExchangePlugins.setStringList(definedFormatExchangePlugins);*/

}
