#include "sensorplugindialog.h"
#include "ui_sensorplugindialog.h"
#include <QtSql/QSqlQueryModel>

/*!
 * \brief SensorPluginDialog::SensorPluginDialog
 * \param parent
 */
SensorPluginDialog::SensorPluginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SensorPluginDialog)
{
    ui->setupUi(this);

    qSqlModel = new QSqlQueryModel();
    masterAccuracyLayout = new QVBoxLayout();
    masterSensorConfigLayout = new QVBoxLayout();

    this->initModels();

    //----------------------


    //tmpSensor = NULL;
    //sensorConfig = NULL;



    connect(ui->tableView_sensorPlugins,SIGNAL(clicked(QModelIndex)),this,SLOT(handleTableClicked(QModelIndex)));
    disableConnectionSettings();
}

/*!
 * \brief SensorPluginDialog::~SensorPluginDialog
 */
SensorPluginDialog::~SensorPluginDialog()
{
    delete ui;
}

/*void SensorPluginDialog::receiveTempSensor(Sensor *s)
{
    this->tmpSensor = s;

    this->setLabelUnits();
    this->getConnectionType();
    this->getReadingType();
    this->getSensorParameters();
}*/

/*!
 * \brief SensorPluginDialog::on_pushButton_ok_clicked
 * Triggered whenever the user clicked the save button
 */
void SensorPluginDialog::on_pushButton_ok_clicked(){

    //check if sensor config name is empty
    if(this->ui->comboBox_sensorConfig->currentText().compare("") == 0){
        Console::addLine("No sensor config name selected.");
        return;
    }

    //check if the sensor plugin is available
    if(this->selectedSConfig.mySensor == NULL){
        SensorConfiguration sConfig = OiConfigState::createConfigFromSensor(this->selectedSConfig.pluginName,
                                                                            this->selectedSConfig.sensorName);
        this->selectedSConfig.mySensor = sConfig.mySensor;
        if(this->selectedSConfig.mySensor == NULL){
            Console::addLine("Selected sensor plugin not available.");
            return;
        }
    }

    //save or edit the sensor config
    this->setSensorConfigFromGUI();
    this->selectedSConfig.setName(this->ui->comboBox_sensorConfig->currentText());
    if(OiConfigState::addSensorConfig(this->selectedSConfig)){

        //ask for connection
        QMessageBox msgBox;
        msgBox.setText("Want to connect sensor?");
        msgBox.setInformativeText("");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Yes:
            emit sendSensorConfig(this->selectedSConfig, true);
            break;
        case QMessageBox::No:
            emit sendSensorConfig(this->selectedSConfig, false);
            break;
        default:
            break;
        }

        this->close();
        return;
    }
    Console::addLine("The sensor config cannot be set.");




    /*if(ui->comboBox_availableSensorTypes->currentIndex() != -1){
        TypeOfSensor = static_cast<Configuration::SensorTypes>(ui->comboBox_availableSensorTypes->itemData(ui->comboBox_availableSensorTypes->currentIndex()).toInt());
        emit selectedPlugin(selectedIndex);

        initSensorConfig();

        this->close();
    }else{
        Console::addLine("problem with selected sensor. please select one.");
    }*/
}

/*!
 * \brief SensorPluginDialog::on_pushButton_cancel_clicked
 */
void SensorPluginDialog::on_pushButton_cancel_clicked()
{
    this->close();
}

/*!
 * \brief SensorPluginDialog::receiveModel
 * Sets the model of the tableview with the received sql model including the sensor plugins.
 * \param sqlModel
 */
void SensorPluginDialog::receiveModel(QSqlQueryModel *sqlModel){

    if(ui->tableView_sensorPlugins->model() != sqlModel){
        ui->tableView_sensorPlugins->setModel(sqlModel);
    }

}

/*!
 * \brief SensorPluginDialog::handleTableClicked
 * Triggered whenever the user selected another sensor plugin
 * \param idx
 */
void SensorPluginDialog::handleTableClicked(const QModelIndex &idx){

    qDebug() << "sensor selected";

    if(!idx.isValid()){
        return;
    }

    //get plugin name and sensor name from table model
    QModelIndex modelIdxPlugin = this->ui->tableView_sensorPlugins->model()->index(idx.row(), 2);
    QModelIndex modelIdxSensor = this->ui->tableView_sensorPlugins->model()->index(idx.row(), 0);
    QString pluginName = this->ui->tableView_sensorPlugins->model()->data(modelIdxPlugin).toString();
    QString sensorName = this->ui->tableView_sensorPlugins->model()->data(modelIdxSensor).toString();

    //if a sensor plugin was selected that does not equal the previous selection
    if(this->selectedSConfig.pluginName.compare(pluginName) != 0
            || this->selectedSConfig.sensorName.compare(sensorName) != 0){

        //create a new sensor config from sensor's defaults
        this->setSelectedSensorConfig(OiConfigState::createConfigFromSensor(pluginName, sensorName));

    }

    //set description
    QModelIndex modelIdxDescription = ui->tableView_sensorPlugins->model()->index(idx.row(), 1);
    QString description = this->ui->tableView_sensorPlugins->model()->data(modelIdxDescription).toString();
    this->ui->textBrowser_description->append(description);

}

/*!
 * \brief SensorPluginDialog::initModels
 * Initially get the models (sensor types, ip adresses etc.) from OiModelManager
 */
void SensorPluginDialog::initModels(){

    this->ui->comboBox_availableSensorTypes->setModel(&OiModelManager::getSensorTypes());
    this->ui->comboBox_baudrate->setModel(&OiModelManager::getBaudRateTypes());
    this->ui->comboBox_comPort->setModel(&OiModelManager::getAvailableSerialPorts());
    this->ui->comboBox_databits->setModel(&OiModelManager::getDataBitTypes());
    this->ui->comboBox_flowcontrol->setModel(&OiModelManager::getFlowControlTypes());
    this->ui->comboBox_ip->setModel(&OiModelManager::getAvailableIpAdresses());
    this->ui->comboBox_parity->setModel(&OiModelManager::getParityTypes());
    this->ui->comboBox_stopbits->setModel(&OiModelManager::getStopBitTypes());

    this->ui->comboBox_sensorConfig->setModel(OiConfigState::getSensorConfigNames());

}

/*!
 * \brief SensorPluginDialog::setSelectedSensorConfig
 * Set the currently selected sensor config and update the dynamic GUI based on the selection
 * \param selectedSConfig
 */
void SensorPluginDialog::setSelectedSensorConfig(SensorConfiguration selectedSConfig){

    //set selected sensor config
    this->selectedSConfig = selectedSConfig;

    //update all the GUI elements to display the selected sensor config parameters
    this->setGUIFromSensorConfig();

}

/*!
 * \brief SensorPluginDialog::updateDynamicGUI
 * Update the dynamic GUI
 */
void SensorPluginDialog::setGUIFromSensorConfig(){

    //destroy the old GUI
    this->destructDynamicGUI();

    //set units (labels)
    this->setLabelUnits();

    //set accuracy values
    this->setAccuracyFromConfig();

    //set connection parameters
    this->setConnectionTypeFromConfig();
    this->setConnectionParametersFromConfig();

    //set sensor parameters
    this->setSensorParametersFromConfig();

}

/*!
 * \brief SensorPluginDialog::setSensorConfigFromGUI
 * Updates the selected sensor config from GUI elements
 */
void SensorPluginDialog::setSensorConfigFromGUI(){

    //set connection
    this->setConnectionFromGUI();

    //set accuracy
    this->setAccuracyFromGUI();

    //set sensor parameters
    this->setSensorParametersFromGUI();

}

/*void SensorPluginDialog::initSensorConfig()
{
    sensorConfig = new SensorConfiguration();

   //sensorConfig->name = ui->lineEdit_configName->text();
   sensorConfig->instrumentType = this->TypeOfSensor;

   sensorConfig->connConfig->baudRate = static_cast<QSerialPort::BaudRate>(ui->comboBox_baudrate->itemData(ui->comboBox_baudrate->currentIndex()).toInt());
   sensorConfig->connConfig->comPort = ui->comboBox_comPort->currentText();
   sensorConfig->connConfig->dataBits = static_cast<QSerialPort::DataBits>(ui->comboBox_databits->itemData(ui->comboBox_databits->currentIndex()).toInt());
   sensorConfig->connConfig->flowControl = static_cast<QSerialPort::FlowControl>(ui->comboBox_flowcontrol->itemData(ui->comboBox_flowcontrol->currentIndex()).toInt());
   if(ui->lineEdit_ip->text().compare("") == 0){
       sensorConfig->connConfig->ip = ui->comboBox_ip->currentText();
   }else{
       sensorConfig->connConfig->ip = ui->lineEdit_ip->text();
   }
   sensorConfig->connConfig->parity = static_cast<QSerialPort::Parity>(ui->comboBox_parity->itemData(ui->comboBox_parity->currentIndex()).toInt());
   sensorConfig->connConfig->port = ui->lineEdit_port->text();
   sensorConfig->connConfig->stopBits = static_cast<QSerialPort::StopBits>(ui->comboBox_stopbits->itemData(ui->comboBox_stopbits->currentIndex()).toInt());
   sensorConfig->connConfig->typeOfConnection = static_cast<Configuration::ConnectionTypes>(ui->comboBox_typeOfConnection->itemData(ui->comboBox_typeOfConnection->currentIndex()).toInt());

   sensorConfig->sigma.sigmaAngleXZ = ui->lineEdit_sigmaAngleXZ->text().toDouble()/UnitConverter::getAngleMultiplier();
   sensorConfig->sigma.sigmaAngleYZ = ui->lineEdit_sigmaAngleYZ->text().toDouble()/UnitConverter::getAngleMultiplier();
   sensorConfig->sigma.sigmaAzimuth = ui->lineEdit_sigmaAzimuth->text().toDouble()/UnitConverter::getAngleMultiplier();
   sensorConfig->sigma.sigmaDistance = ui->lineEdit_sigmaDistance->text().toDouble()/UnitConverter::getDistanceMultiplier();
   sensorConfig->sigma.sigmaTemp = UnitConverter::getReverseTemperature(ui->lineEdit_sigmaTemp->text().toDouble());
   sensorConfig->sigma.sigmaZenith = ui->lineEdit_sigmaZenith->text().toDouble()/UnitConverter::getAngleMultiplier();
   sensorConfig->sigma.sigmaXyz.setAt(0,ui->lineEdit_sigmaX->text().toDouble()/UnitConverter::getDistanceMultiplier());
   sensorConfig->sigma.sigmaXyz.setAt(1,ui->lineEdit_sigmaY->text().toDouble()/UnitConverter::getDistanceMultiplier());
   sensorConfig->sigma.sigmaXyz.setAt(2,ui->lineEdit_sigmaZ->text().toDouble()/UnitConverter::getDistanceMultiplier());

   QMapIterator<QString,QLineEdit*> j(undefinedSigma);
   while(j.hasNext()){
       j.next();
       sensorConfig->sigma.sigmaUndefined.insert(j.key(),j.value()->text().toDouble());
   }

   QMapIterator<QString, QLineEdit*> k(doubleParameter);
   while(k.hasNext()){
       k.next();
       sensorConfig->doubleParameter.insert(k.key(),k.value()->text().toDouble());
   }

   QMapIterator<QString, QLineEdit*> l(integerParameter);
   while(l.hasNext()){
       l.next();
       sensorConfig->integerParameter.insert(l.key(),l.value()->text().toInt());
   }

   QMapIterator<QString, QComboBox*> m(stringParameter);
   while(m.hasNext()){
       m.next();
       sensorConfig->stringParameter.insert(m.key(),m.value()->currentText());
   }

   QMessageBox msgBox;
   msgBox.setText("Want to connect sensor?");
   msgBox.setInformativeText("");
   msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
   msgBox.setDefaultButton(QMessageBox::Ok);
   int ret = msgBox.exec();

   switch (ret) {
   case QMessageBox::Yes:
       emit sendSensorConfig(sensorConfig, true);
       break;
   case QMessageBox::No:
       emit sendSensorConfig(sensorConfig, false);
       break;
   default:
       break;
   }

}*/

/*!
 * \brief SensorPluginDialog::getCode
 * \param cb
 * \param type
 * \return
 */
int SensorPluginDialog::getCode(QComboBox *cb, QString type){
    return cb->findText(type);
}

/*!
 * \brief SensorPluginDialog::getLabel
 * \param cb
 * \param cT
 * \return
 */
QString SensorPluginDialog::getLabel(QComboBox *cb, Configuration::ConnectionTypes cT){
    return cb->itemText(cb->findData(cT));
}

/*!
 * \brief initGUI
 * FIlls all comboboxes with connection information (e.g. baudrate, flow control, parity etc. Also displays all available iPv4 network addresses.
 */
void SensorPluginDialog::initGUI(){


    //clear all comboboxes
    ui->comboBox_availableSensorTypes->clear();
    ui->comboBox_baudrate->clear();
    ui->comboBox_databits->clear();
    ui->comboBox_flowcontrol->clear();
    ui->comboBox_parity->clear();
    ui->comboBox_stopbits->clear();

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
}

void SensorPluginDialog::on_comboBox_availableSensorTypes_currentIndexChanged(const QString &arg1)
{
    int currentIndex = ui->comboBox_availableSensorTypes->currentIndex();
    //ui->pushButton_ok->setEnabled(false);
    disableConnectionSettings();
    disableAccuracyElements();
    destructDynamicGUI();

    if(currentIndex != -1){

        Configuration::SensorTypes sensor = static_cast<Configuration::SensorTypes>(ui->comboBox_availableSensorTypes->itemData(ui->comboBox_availableSensorTypes->currentIndex()).toInt());

        switch (sensor) {
        case Configuration::eLaserTracker:
            emit sendSensorType(Configuration::eLaserTracker);
            break;
        case Configuration::eTotalStation:
            emit sendSensorType(Configuration::eTotalStation);
            break;
        case Configuration::eUndefinedSensor:
            emit sendSensorType(Configuration::eUndefinedSensor);
            break;
        default:
            break;
        }
    }


}

/*!
 * \brief SensorPluginDialog::setConnectionType
 * Sets the connection type from sensor config (network, serial)
 */
void SensorPluginDialog::setConnectionTypeFromConfig(){

    this->ui->comboBox_typeOfConnection->clear();

    if(this->selectedSConfig.mySensor == NULL || this->selectedSConfig.mySensor->getConnectionType() == NULL){
        return;
    }

    //get a list of supported connection types
    QList<Configuration::ConnectionTypes> connectionTypes = *(this->selectedSConfig.mySensor->getConnectionType());

    //add the connection types to the corresponding combo box
    for(int i = 0; i < connectionTypes.size(); i++){
        Configuration::ConnectionTypes connectionType = connectionTypes.at(i);
        switch (connectionType) {
        case Configuration::eNetwork:
            this->ui->comboBox_typeOfConnection->insertItem(this->ui->comboBox_typeOfConnection->count(),
                                                            "network connection", Configuration::eNetwork);
            break;
        case Configuration::eSerial:
            this->ui->comboBox_typeOfConnection->insertItem(this->ui->comboBox_typeOfConnection->count(),
                                                            "serial connection", Configuration::eSerial);
            break;
        }
    }

    //select the connection type that is defined in sensor config
    switch (this->selectedSConfig.connConfig->typeOfConnection) {
    case Configuration::eNetwork:
        this->ui->comboBox_typeOfConnection->setCurrentText("network connection");
        break;
    case Configuration::eSerial:
        this->ui->comboBox_typeOfConnection->setCurrentText("serial connection");
        break;
    }

}

/*!
 * \brief SensorPluginDialog::setConnectionParameters
 * Sets the connection parameters from sensor config
 */
void SensorPluginDialog::setConnectionParametersFromConfig(){

    int index = -1;

    //set selected baud rate
    index = this->ui->comboBox_baudrate->findData(this->selectedSConfig.connConfig->baudRate);
    if(index > -1){
        this->ui->comboBox_baudrate->setCurrentIndex(index);
    }

    //set selected data bits
    index = this->ui->comboBox_databits->findData(this->selectedSConfig.connConfig->dataBits);
    if(index > -1){
        this->ui->comboBox_databits->setCurrentIndex(index);
    }

    //set selected flow control
    index = this->ui->comboBox_flowcontrol->findData(this->selectedSConfig.connConfig->flowControl);
    if(index > -1){
        this->ui->comboBox_flowcontrol->setCurrentIndex(index);
    }

    //set selected parity
    index = this->ui->comboBox_parity->findData(this->selectedSConfig.connConfig->parity);
    if(index > -1){
        this->ui->comboBox_parity->setCurrentIndex(index);
    }

    //set selected stop bits
    index = this->ui->comboBox_stopbits->findData(this->selectedSConfig.connConfig->stopBits);
    if(index > -1){
        this->ui->comboBox_stopbits->setCurrentIndex(index);
    }

    //set selected com port
    this->ui->comboBox_comPort->setCurrentText(this->selectedSConfig.connConfig->comPort);

    //set selected ip
    index = this->ui->comboBox_ip->findData(this->selectedSConfig.connConfig->ip);
    if(index > -1){
        this->ui->comboBox_ip->setCurrentIndex(index);
    }else{
        this->ui->comboBox_ip->setCurrentText(this->selectedSConfig.connConfig->ip);
    }

    //set selected port
    this->ui->lineEdit_port->setText(this->selectedSConfig.connConfig->port);

}

void SensorPluginDialog::closeEvent(QCloseEvent *event){
    //this->tmpSensor = NULL;
    qSqlModel = NULL;
    disableConnectionSettings();
    disableAccuracyElements();
    destructDynamicGUI();
    event->accept();
}

void SensorPluginDialog::showEvent(QShowEvent *event)
{
    qSqlModel = NULL;
    qSqlModel = new QSqlQueryModel();

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
    //initGUI();
    event->accept();
}

void SensorPluginDialog::on_comboBox_typeOfConnection_currentIndexChanged(const QString &arg1)
{
    if(ui->comboBox_typeOfConnection->currentIndex() != -1){
        Configuration::ConnectionTypes conType = static_cast<Configuration::ConnectionTypes>(ui->comboBox_typeOfConnection->itemData(ui->comboBox_typeOfConnection->currentIndex()).toInt());

        switch (conType) {
        case Configuration::eNetwork:
            ui->comboBox_ip->setEnabled(true);
            ui->lineEdit_port->setEnabled(true);
            ui->comboBox_comPort->setEnabled(false);
            ui->comboBox_baudrate->setEnabled(false);
            ui->comboBox_databits->setEnabled(false);
            ui->comboBox_flowcontrol->setEnabled(false);
            ui->comboBox_parity->setEnabled(false);
            ui->comboBox_stopbits->setEnabled(false);
            break;
        case Configuration::eSerial:
            ui->comboBox_comPort->setEnabled(true);
            ui->comboBox_baudrate->setEnabled(true);
            ui->comboBox_databits->setEnabled(true);
            ui->comboBox_flowcontrol->setEnabled(true);
            ui->comboBox_parity->setEnabled(true);
            ui->comboBox_stopbits->setEnabled(true);
            ui->comboBox_ip->setEnabled(false);
            ui->lineEdit_port->setEnabled(false);
            break;
        default:
            break;
        }
    }
}

void SensorPluginDialog::disableConnectionSettings()
{
    ui->comboBox_baudrate->setEnabled(false);
    ui->comboBox_comPort->setEnabled(false);
    ui->comboBox_databits->setEnabled(false);
    ui->comboBox_flowcontrol->setEnabled(false);
    ui->comboBox_ip->setEnabled(false);
    ui->comboBox_parity->setEnabled(false);
    ui->comboBox_stopbits->setEnabled(false);
    ui->lineEdit_port->setEnabled(false);
}

void SensorPluginDialog::disableAccuracyElements()
{
    ui->label_sigmaAzimuth->setEnabled(false);
    ui->label_sigmaZenith->setEnabled(false);
    ui->label_sigmaDistance->setEnabled(false);
    ui->label_sigmaX->setEnabled(false);
    ui->label_sigmaY->setEnabled(false);
    ui->label_sigmaZ->setEnabled(false);
    ui->label_sigmaTemp->setEnabled(false);
    ui->label_sigmaAngleXZ->setEnabled(false);
    ui->label_sigmaAngleYZ->setEnabled(false);

    ui->lineEdit_sigmaAzimuth->setEnabled(false);
    ui->lineEdit_sigmaZenith->setEnabled(false);
    ui->lineEdit_sigmaDistance->setEnabled(false);
    ui->lineEdit_sigmaX->setEnabled(false);
    ui->lineEdit_sigmaY->setEnabled(false);
    ui->lineEdit_sigmaZ->setEnabled(false);
    ui->lineEdit_sigmaTemp->setEnabled(false);
    ui->lineEdit_sigmaAngleXZ->setEnabled(false);
    ui->lineEdit_sigmaAngleYZ->setEnabled(false);
}

/*!
 * \brief SensorPluginDialog::setAccuracy
 * Sets the accuracy values of the selected sensor config in the GUI
 */
void SensorPluginDialog::setAccuracyFromConfig(){
/*
    if(this->selectedSConfig.mySensor == NULL){
        return;
    }

    //get a list of reading types that the sensor supports
    QList<Configuration::ReadingTypes> readingTypes = *(this->selectedSConfig.mySensor->getSupportedReadingTypes());

    //for each reading type enable the corresponding GUI element and display the config value
    for(int i = 0; i < readingTypes.size(); i++){

        switch (readingTypes.at(i)) {
        case Configuration::ePolar:

            //enable GUI elements
            this->ui->label_sigmaAzimuth->setEnabled(true);
            this->ui->label_sigmaZenith->setEnabled(true);
            this->ui->label_sigmaDistance->setEnabled(true);
            this->ui->lineEdit_sigmaAzimuth->setEnabled(true);
            this->ui->lineEdit_sigmaZenith->setEnabled(true);
            this->ui->lineEdit_sigmaDistance->setEnabled(true);

            //display config values
            this->ui->lineEdit_sigmaAzimuth->setText(QString::number(this->selectedSConfig.sigma.sigmaAzimuth,
                                                                     'f', OiUnitConverter::angleDigits));
            this->ui->lineEdit_sigmaZenith->setText(QString::number(this->selectedSConfig.sigma.sigmaZenith,
                                                                    'f', OiUnitConverter::angleDigits));
            this->ui->lineEdit_sigmaDistance->setText(QString::number(this->selectedSConfig.sigma.sigmaDistance,
                                                                      'f', OiUnitConverter::distanceDigits));

            break;

        case Configuration::eCartesian:

            //enable GUI elements
            this->ui->label_sigmaX->setEnabled(true);
            this->ui->label_sigmaY->setEnabled(true);
            this->ui->label_sigmaZ->setEnabled(true);
            this->ui->lineEdit_sigmaX->setEnabled(true);
            this->ui->lineEdit_sigmaY->setEnabled(true);
            this->ui->lineEdit_sigmaZ->setEnabled(true);

            //display config values
            this->ui->lineEdit_sigmaX->setText(QString::number(this->selectedSConfig.sigma.sigmaXyz.getAt(0),
                                                                     'f', OiUnitConverter::distanceDigits));
            this->ui->lineEdit_sigmaY->setText(QString::number(this->selectedSConfig.sigma.sigmaXyz.getAt(1),
                                                                     'f', OiUnitConverter::distanceDigits));
            this->ui->lineEdit_sigmaZ->setText(QString::number(this->selectedSConfig.sigma.sigmaXyz.getAt(2),
                                                                     'f', OiUnitConverter::distanceDigits));

            break;

        case Configuration::eDirection:

            //enable GUI elements
            this->ui->label_sigmaAzimuth->setEnabled(true);
            this->ui->label_sigmaZenith->setEnabled(true);
            this->ui->lineEdit_sigmaAzimuth->setEnabled(true);
            this->ui->lineEdit_sigmaZenith->setEnabled(true);

            //display config values
            this->ui->lineEdit_sigmaAzimuth->setText(QString::number(this->selectedSConfig.sigma.sigmaAzimuth,
                                                                     'f', OiUnitConverter::angleDigits));
            this->ui->lineEdit_sigmaZenith->setText(QString::number(this->selectedSConfig.sigma.sigmaZenith,
                                                                     'f', OiUnitConverter::angleDigits));

            break;

        case Configuration::eDistance:

            //enable GUI elements
            this->ui->label_sigmaDistance->setEnabled(true);
            this->ui->lineEdit_sigmaDistance->setEnabled(true);

            //display config values
            this->ui->lineEdit_sigmaDistance->setText(QString::number(this->selectedSConfig.sigma.sigmaDistance,
                                                                     'f', OiUnitConverter::distanceDigits));

            break;

        case Configuration::eTemperatur:

            //enable GUI elements
            this->ui->label_sigmaTemp->setEnabled(true);
            this->ui->lineEdit_sigmaTemp->setEnabled(true);

            //display config values
            this->ui->lineEdit_sigmaTemp->setText(QString::number(this->selectedSConfig.sigma.sigmaTemp,
                                                                     'f', OiUnitConverter::temperatureDigits));

            break;

        case Configuration::eLevel:

            //enable GUI elements
            this->ui->label_sigmaAngleXZ->setEnabled(true);
            this->ui->label_sigmaAngleYZ->setEnabled(true);
            this->ui->lineEdit_sigmaAngleXZ->setEnabled(true);
            this->ui->lineEdit_sigmaAngleYZ->setEnabled(true);

            //display config values
            this->ui->lineEdit_sigmaAngleXZ->setText(QString::number(this->selectedSConfig.sigma.sigmaAngleXZ,
                                                                     'f', OiUnitConverter::angleDigits));
            this->ui->lineEdit_sigmaAngleYZ->setText(QString::number(this->selectedSConfig.sigma.sigmaAngleYZ,
                                                                     'f', OiUnitConverter::angleDigits));

            break;

        case Configuration::eUndefined:

            //for each undefined sigma value add corresponding GUI elements
            QMap<QString,double> undefinedSigmas = this->selectedSConfig.sigma.sigmaUndefined;
            QMapIterator<QString, double> sigmaIterator(undefinedSigmas);
            while(sigmaIterator.hasNext()){
                sigmaIterator.next();

                //create and fill GUI elements
                QLabel *l = new QLabel();
                l->setText(sigmaIterator.key());
                QLineEdit *le = new QLineEdit();
                le->setText(QString::number(sigmaIterator.value(), 'f', OiUnitConverter::dimensionLessDigits));

                //create layout for both GUI elements
                QHBoxLayout *layout = new QHBoxLayout();
                layout->addWidget(l);
                layout->addWidget(le);
                layout->setStretch(0,1);
                layout->setStretch(1,1);

                //add the created layout to the layout with all undefined sigma elements
                this->masterAccuracyLayout->addLayout(layout);

                //save the created GUI elements to be able to easily access them later
                this->undefinedSigmaLabel.insert(sigmaIterator.key(),l);
                this->undefinedSigma.insert(sigmaIterator.key(), le);
                this->accuracyLayouts.insert(sigmaIterator.key(),layout);

            }

            //set the layout with all undefined accuracies as used layout in the GUI
            this->ui->page_sigmaUndefined->setLayout(masterAccuracyLayout);

            break;

        }

    }
*/
}

/*!
 * \brief SensorPluginDialog::destructDynamicGUI
 */
void SensorPluginDialog::destructDynamicGUI(){

    //delete double parameter labels
    QMapIterator<QString, QLabel*> m(doubleParameterLabel);
    while(m.hasNext()){
        m.next();
        ui->tab_sensorConfiguration->layout()->removeWidget(m.value());
        delete m.value();
    }
    doubleParameterLabel.clear();

    //delete double parameter line edits
    QMapIterator<QString, QLineEdit*> n(doubleParameter);
    while(n.hasNext()){
        n.next();
        ui->tab_sensorConfiguration->layout()->removeWidget(n.value());
        delete n.value();
    }
    doubleParameter.clear();

    //delete integer parameter labels
    QMapIterator<QString, QLabel*> o(integerParameterLabel);
    while(o.hasNext()){
        o.next();
        ui->tab_sensorConfiguration->layout()->removeWidget(o.value());
        delete o.value();
    }
    integerParameterLabel.clear();

    //delete integer parameter line edits
    QMapIterator<QString, QLineEdit*> p(integerParameter);
    while(p.hasNext()){
        p.next();
        ui->tab_sensorConfiguration->layout()->removeWidget(p.value());
        delete p.value();
    }
    integerParameter.clear();

    //delete string parameter labels
    QMapIterator<QString, QLabel*> q(stringParameterLabel);
    while(q.hasNext()){
        q.next();
        ui->tab_sensorConfiguration->layout()->removeWidget(q.value());
        delete q.value();
    }
    stringParameterLabel.clear();

    //delete string parameter combo boxes
    QMapIterator<QString, QComboBox*> r(stringParameter);
    while(r.hasNext()){
        r.next();
        ui->tab_sensorConfiguration->layout()->removeWidget(r.value());
        delete r.value();
    }
    stringParameter.clear();

    //delete layouts for the sensor parameters (int, double and string parameter)
    QMapIterator<QString, QLayout*> s(sensorConfigLayouts);
    while(s.hasNext()){
        s.next();
        ui->tab_sensorConfiguration->layout()->removeItem(s.value());
        delete s.value();
    }
    sensorConfigLayouts.clear();





    //undefined accuracy labels, lineedits and layouts. Here they all get deleted and removed from GUI

    ui->toolBox_accuracy->setItemText(4,"others");

    QMapIterator<QString, QLabel*> k(undefinedSigmaLabel);
    while(k.hasNext()){
        k.next();
        ui->page_sigmaUndefined->layout()->removeWidget(k.value());
        delete k.value();
    }
    undefinedSigmaLabel.clear();

    QMapIterator<QString, QLineEdit*> l(undefinedSigma);
    while(l.hasNext()){
        l.next();
        ui->page_sigmaUndefined->layout()->removeWidget(l.value());
        delete l.value();
    }
    undefinedSigma.clear();

    QMapIterator<QString,QLayout*> j(accuracyLayouts);
    while(j.hasNext()){
        j.next();
        ui->page_sigmaUndefined->layout()->removeItem(j.value());
        delete j.value();
    }

    accuracyLayouts.clear();
}

/*!
 * \brief SensorPluginDialog::setSensorParameters
 * Set special sensor parameters from selected sensor config
 */
void SensorPluginDialog::setSensorParametersFromConfig(){
/*
    //set integer parameters
    QMap<QString, int> intParams = this->selectedSConfig.integerParameter;
    QMapIterator<QString, int> intParamsIterator(intParams);
    while(intParamsIterator.hasNext()){
        intParamsIterator.next();

        //create and fill GUI elements
        QLabel *l = new QLabel();
        l->setText(intParamsIterator.key());
        QLineEdit *le = new QLineEdit();
        le->setText(QString::number(intParamsIterator.value()));

        //create layout for GUI elements
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(l);
        layout->addWidget(le);
        layout->setStretch(0,1);
        layout->setStretch(1,1);

        //add the layout to the layout with all sensor parameters
        this->masterSensorConfigLayout->addLayout(layout);

        //save the created GUI elements to be able to easily access them later
        this->integerParameter.insert(intParamsIterator.key(),le);
        this->integerParameterLabel.insert(intParamsIterator.key(),l);
        this->sensorConfigLayouts.insert(intParamsIterator.key(),layout);

    }

    //set double parameters
    QMap<QString, double> doubleParams = this->selectedSConfig.doubleParameter;
    QMapIterator<QString, double> doubleParamsIterator(doubleParams);
    while(doubleParamsIterator.hasNext()){
        doubleParamsIterator.next();

        //create and fill GUI elements
        QLabel *l = new QLabel();
        l->setText(doubleParamsIterator.key());
        QLineEdit *le = new QLineEdit();
        le->setText(QString::number(doubleParamsIterator.value(), 'f', OiUnitConverter::dimensionLessDigits));

        //create layout for GUI elements
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(l);
        layout->addWidget(le);
        layout->setStretch(0,1);
        layout->setStretch(1,1);

        //add the layout to the layout with all sensor parameters
        this->masterSensorConfigLayout->addLayout(layout);

        //save the created GUI elements to be able to easily access them later
        QString test = doubleParamsIterator.key();
        this->doubleParameter.insert(doubleParamsIterator.key(),le);
        this->doubleParameterLabel.insert(doubleParamsIterator.key(),l);
        this->sensorConfigLayouts.insert(doubleParamsIterator.key(),layout);

    }

    //set string parameters
    QMap <QString, QStringList> sensorStringParams; //string parameter from sensor
    if(this->selectedSConfig.mySensor != NULL && this->selectedSConfig.mySensor->getStringParameter() != NULL){
        sensorStringParams = *(this->selectedSConfig.mySensor->getStringParameter());

        QMap<QString, QString> stringParams = this->selectedSConfig.stringParameter; //string parameter from config
        QMapIterator<QString, QString> stringParamsIterator(stringParams);
        while(stringParamsIterator.hasNext()){
            stringParamsIterator.next();

            //create and fill GUI elements
            QLabel *le = new QLabel();
            le->setText(stringParamsIterator.key());
            QComboBox *l = new QComboBox();
            if(sensorStringParams.contains(stringParamsIterator.key())){
                QStringList values = sensorStringParams.value(stringParamsIterator.key());
                for(int i = 0; i < values.size(); i++){
                    l->addItem(values.at(i));
                }
            }
            l->setCurrentText(stringParamsIterator.value());

            //create layout for GUI elements
            QHBoxLayout *layout = new QHBoxLayout();
            layout->addWidget(l);
            layout->addWidget(le);
            layout->setStretch(0,1);
            layout->setStretch(1,1);

            //add the layout to the layout with all sensor parameters
            this->masterSensorConfigLayout->addLayout(layout);

            //save the created GUI elements to be able to easily access them later
            this->stringParameter.insert(stringParamsIterator.key(),l);
            this->stringParameterLabel.insert(stringParamsIterator.key(),le);
            this->sensorConfigLayouts.insert(stringParamsIterator.key(),layout);

        }
    }

    //add the layout with all sensor parameters to the GUI
    this->ui->tab_sensorConfiguration->setLayout(this->masterSensorConfigLayout);
*/
}

/*!
 * \brief SensorPluginDialog::setSelectedSensorPlugin
 * Select the plugin in table view that the given config contains
 * \param sConfig
 */
void SensorPluginDialog::setSelectedSensorPlugin(SensorConfiguration &sConfig){

    //select the right instrument type in the combo box
    int index = this->ui->comboBox_availableSensorTypes->findData(sConfig.instrumentType);
    this->ui->comboBox_availableSensorTypes->setCurrentIndex(index);

    //select the right model index in table view that respresents the sensor plugin of sConfig
    if(this->ui->tableView_sensorPlugins->model() != NULL
            && this->ui->tableView_sensorPlugins->model()->hasIndex(0, 0)){
        QModelIndex startIndex = this->ui->tableView_sensorPlugins->model()->index(0, 0);
        if(!startIndex.isValid()){
            return;
        }
        QModelIndexList modelIndexes = this->ui->tableView_sensorPlugins->model()->match(startIndex, Qt::DisplayRole,
                                                                                         sConfig.sensorName);
        if(modelIndexes.size() == 0){
            return;
        }
        this->ui->tableView_sensorPlugins->selectionModel()->setCurrentIndex(modelIndexes.at(0), QItemSelectionModel::Select);
    }

}

/*!
 * \brief SensorPluginDialog::setAccuracyFromGUI
 */
void SensorPluginDialog::setAccuracyFromGUI(){

    this->selectedSConfig.sigma.sigmaAngleXZ = this->ui->lineEdit_sigmaAngleXZ->text().toDouble();
    this->selectedSConfig.sigma.sigmaAngleYZ = this->ui->lineEdit_sigmaAngleYZ->text().toDouble();
    this->selectedSConfig.sigma.sigmaAzimuth = this->ui->lineEdit_sigmaAzimuth->text().toDouble();
    this->selectedSConfig.sigma.sigmaDistance = this->ui->lineEdit_sigmaDistance->text().toDouble();
    this->selectedSConfig.sigma.sigmaTemp = this->ui->lineEdit_sigmaTemp->text().toDouble();
    this->selectedSConfig.sigma.sigmaXyz.setAt(0, this->ui->lineEdit_sigmaX->text().toDouble());
    this->selectedSConfig.sigma.sigmaXyz.setAt(1, this->ui->lineEdit_sigmaY->text().toDouble());
    this->selectedSConfig.sigma.sigmaXyz.setAt(2, this->ui->lineEdit_sigmaZ->text().toDouble());
    this->selectedSConfig.sigma.sigmaZenith = this->ui->lineEdit_sigmaZenith->text().toDouble();

    QMapIterator<QString,QLineEdit*> undefinedIterator(this->undefinedSigma);
    while(undefinedIterator.hasNext()){
        undefinedIterator.next();
        this->selectedSConfig.sigma.sigmaUndefined.insert(undefinedIterator.key(), undefinedIterator.value()->text().toDouble());
    }

}

/*!
 * \brief SensorPluginDialog::setConnectionFromGUI
 */
void SensorPluginDialog::setConnectionFromGUI(){

    this->selectedSConfig.connConfig->baudRate = static_cast<QSerialPort::BaudRate>(this->ui->comboBox_baudrate->itemData(this->ui->comboBox_baudrate->currentIndex()).toInt());
    this->selectedSConfig.connConfig->comPort = this->ui->comboBox_comPort->currentText();
    this->selectedSConfig.connConfig->dataBits = static_cast<QSerialPort::DataBits>(this->ui->comboBox_databits->itemData(this->ui->comboBox_databits->currentIndex()).toInt());
    this->selectedSConfig.connConfig->flowControl = static_cast<QSerialPort::FlowControl>(this->ui->comboBox_flowcontrol->itemData(this->ui->comboBox_flowcontrol->currentIndex()).toInt());
    this->selectedSConfig.connConfig->ip = this->ui->comboBox_ip->currentText();
    this->selectedSConfig.connConfig->parity = static_cast<QSerialPort::Parity>(this->ui->comboBox_parity->itemData(this->ui->comboBox_parity->currentIndex()).toInt());
    this->selectedSConfig.connConfig->port = this->ui->lineEdit_port->text();
    this->selectedSConfig.connConfig->stopBits = static_cast<QSerialPort::StopBits>(this->ui->comboBox_stopbits->itemData(this->ui->comboBox_stopbits->currentIndex()).toInt());
    this->selectedSConfig.connConfig->typeOfConnection = static_cast<Configuration::ConnectionTypes>(this->ui->comboBox_typeOfConnection->itemData(this->ui->comboBox_typeOfConnection->currentIndex()).toInt());

}

/*!
 * \brief SensorPluginDialog::setSensorParametersFromGUI
 */
void SensorPluginDialog::setSensorParametersFromGUI(){

    //set double parameter
   QMapIterator<QString, QLineEdit*> doubleIterator(this->doubleParameter);
   while(doubleIterator.hasNext()){
       doubleIterator.next();
       this->selectedSConfig.doubleParameter.insert(doubleIterator.key(), doubleIterator.value()->text().toDouble());
   }

   //set integer parameter
   QMapIterator<QString, QLineEdit*> intIterator(integerParameter);
   while(intIterator.hasNext()){
       intIterator.next();
       this->selectedSConfig.integerParameter.insert(intIterator.key(), intIterator.value()->text().toInt());
   }

   //set string parameter
   QMapIterator<QString, QComboBox*> stringIterator(stringParameter);
   while(stringIterator.hasNext()){
       stringIterator.next();
       this->selectedSConfig.stringParameter.insert(stringIterator.key(), stringIterator.value()->currentText());
   }

}

/*!
 * \brief SensorPluginDialog::setLabelUnits
 * Sets the labels for each accuracy value and its unit
 */
void SensorPluginDialog::setLabelUnits(){
/*
    ui->label_sigmaAngleXZ->setText(QString("sigma angle XZ "+ OiUnitConverter::getAngleUnitString()));
    ui->label_sigmaAngleYZ->setText(QString("sigma angle YZ " + OiUnitConverter::getAngleUnitString()));
    ui->label_sigmaAzimuth->setText(QString("sigma azimuth "+ OiUnitConverter::getAngleUnitString()));
    ui->label_sigmaDistance->setText(QString("sigma distance " + OiUnitConverter::getDistanceUnitString()));
    ui->label_sigmaTemp->setText(QString("sigma temperature " + OiUnitConverter::getTemperatureUnitString()));
    ui->label_sigmaX->setText(QString("sigma x " + OiUnitConverter::getDistanceUnitString()));
    ui->label_sigmaY->setText(QString("sigma y " + OiUnitConverter::getDistanceUnitString()));
    ui->label_sigmaZ->setText(QString("sigma z " + OiUnitConverter::getDistanceUnitString()));
    ui->label_sigmaZenith->setText(QString("sigma zenith " + OiUnitConverter::getAngleUnitString()));
*/
}

/*!
 * \brief SensorPluginDialog::on_comboBox_sensorConfig_currentIndexChanged
 * Triggered whenever the user selected another sensor config
 * \param text
 */
void SensorPluginDialog::on_comboBox_sensorConfig_currentIndexChanged(const QString &text){
    SensorConfiguration myConfig = OiConfigState::getSensorConfig(text);
    if(myConfig.getIsValid()){

        //select the right plugin in table view
        this->setSelectedSensorPlugin(myConfig);

        //set the GUI elements show the selected sensor config
        this->setSelectedSensorConfig(myConfig);

    }
}
