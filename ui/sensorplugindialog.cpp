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


    tmpSensor = NULL;
    sensorConfig = NULL;

    qSqlModel = new QSqlQueryModel();
    masterAccuracyLayout = new QVBoxLayout();
    masterSensorConfigLayout = new QVBoxLayout();

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

void SensorPluginDialog::receiveTempSensor(Sensor *s)
{
    this->tmpSensor = s;

    this->setLabelUnits();
    this->getConnectionType();
    this->getReadingType();
    this->getSensorParameters();
}

/*!
 * \brief SensorPluginDialog::on_pushButton_ok_clicked
 * Sends the index of the selected sensor plugin and the sensor type to the controller for making an instance of the plugin.
 */
void SensorPluginDialog::on_pushButton_ok_clicked()
{

    if(ui->comboBox_availableSensorTypes->currentIndex() != -1){
        TypeOfSensor = static_cast<Configuration::SensorTypes>(ui->comboBox_availableSensorTypes->itemData(ui->comboBox_availableSensorTypes->currentIndex()).toInt());
        emit selectedPlugin(selectedIndex);

        initSensorConfig();

        this->close();
    }else{
        Console::addLine("problem with selected sensor. please select one.");
    }
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
 * Calculates the actual selected index and shows the description of the selected plugin in the textbrowser.
 * \param idx
 */
void SensorPluginDialog::handleTableClicked(const QModelIndex &idx){
    if(!idx.isValid()){
        return;
    }
    destructDynamicGUI();
    selectedIndex = idx.row();
    ui->textBrowser_description->clear();
    QModelIndex modelIdx = ui->tableView_sensorPlugins->model()->index(idx.row(), 1);
    QString description = ui->tableView_sensorPlugins->model()->data(modelIdx).toString();
    ui->textBrowser_description->append(description);

    ui->pushButton_ok->setEnabled(true);

    emit selectedTempPlugin(selectedIndex);

}

void SensorPluginDialog::initSensorConfig()
{
    sensorConfig = new SensorConfiguration();

   sensorConfig->name = ui->lineEdit_configName->text();
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

}

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
    ui->pushButton_ok->setEnabled(false);
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

void SensorPluginDialog::getConnectionType()
{
    ui->comboBox_typeOfConnection->clear();

    if(this->tmpSensor != NULL && this->tmpSensor->getConnectionType() != NULL){

        QList<Configuration::ConnectionTypes> conType = *this->tmpSensor->getConnectionType();

        for(int i=0; i<conType.size();i++){

            Configuration::ConnectionTypes connectType = conType.at(i);

            switch (connectType) {
            case Configuration::eNetwork:
                ui->comboBox_typeOfConnection->insertItem(ui->comboBox_typeOfConnection->count(),"network connection",Configuration::eNetwork);
                break;
            case Configuration::eSerial:
                ui->comboBox_typeOfConnection->insertItem(ui->comboBox_typeOfConnection->count(),"serial connection",Configuration::eSerial);
                break;
            default:
                break;
            }
        }


    }
}

void SensorPluginDialog::closeEvent(QCloseEvent *event){
    this->tmpSensor = NULL;
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

    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
    initGUI();
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

void SensorPluginDialog::getReadingType()
{
    if(this->tmpSensor != NULL && this->tmpSensor->getSupportedReadingTypes() != NULL){

        QList<Configuration::ReadingTypes> readingTypes = *this->tmpSensor->getSupportedReadingTypes();

        for(int i=0; i<readingTypes.size();i++){

            Configuration::ReadingTypes readingType =readingTypes.at(i);

            switch (readingType) {
            case Configuration::ePolar:
                ui->label_sigmaAzimuth->setEnabled(true);
                ui->label_sigmaZenith->setEnabled(true);
                ui->label_sigmaDistance->setEnabled(true);
                ui->lineEdit_sigmaAzimuth->setEnabled(true);
                ui->lineEdit_sigmaZenith->setEnabled(true);
                ui->lineEdit_sigmaDistance->setEnabled(true);

                if(this->tmpSensor->getDefaultAccuracy() != NULL){
                     QMap<QString,double> *sigmaPolar = this->tmpSensor->getDefaultAccuracy();
                      ui->lineEdit_sigmaAzimuth->setText(QString::number(sigmaPolar->value("sigmaAzimuth")));
                      ui->lineEdit_sigmaZenith->setText(QString::number(sigmaPolar->value("sigmaZenith")));
                      ui->lineEdit_sigmaDistance->setText(QString::number(sigmaPolar->value("sigmaDistance")));
                 }
                break;
            case Configuration::eCartesian:
                ui->label_sigmaX->setEnabled(true);
                ui->label_sigmaY->setEnabled(true);
                ui->label_sigmaZ->setEnabled(true);
                ui->lineEdit_sigmaX->setEnabled(true);
                ui->lineEdit_sigmaY->setEnabled(true);
                ui->lineEdit_sigmaZ->setEnabled(true);

                if(this->tmpSensor->getDefaultAccuracy() != NULL){
                     QMap<QString,double> *sigmaPolar = this->tmpSensor->getDefaultAccuracy();
                      ui->lineEdit_sigmaX->setText(QString::number(sigmaPolar->value("sigmaX")));
                      ui->lineEdit_sigmaY->setText(QString::number(sigmaPolar->value("sigmaY")));
                      ui->lineEdit_sigmaZ->setText(QString::number(sigmaPolar->value("sigmaZ")));
                 }
                break;
            case Configuration::eDirection:
                ui->label_sigmaAzimuth->setEnabled(true);
                ui->label_sigmaZenith->setEnabled(true);
                ui->lineEdit_sigmaAzimuth->setEnabled(true);
                ui->lineEdit_sigmaZenith->setEnabled(true);

                if(this->tmpSensor->getDefaultAccuracy() != NULL){
                     QMap<QString,double> *sigmaDirection = this->tmpSensor->getDefaultAccuracy();
                      ui->lineEdit_sigmaAzimuth->setText(QString::number(sigmaDirection->value("sigmaAzimuth")));
                      ui->lineEdit_sigmaZenith->setText(QString::number(sigmaDirection->value("sigmaZenith")));
                 }
                break;
            case Configuration::eDistance:
                ui->label_sigmaDistance->setEnabled(true);
                ui->lineEdit_sigmaDistance->setEnabled(true);

                if(this->tmpSensor->getDefaultAccuracy() != NULL){
                     QMap<QString,double> *sigmaDistance = this->tmpSensor->getDefaultAccuracy();
                      ui->lineEdit_sigmaDistance->setText(QString::number(sigmaDistance->value("sigmaDistance")));
                 }
                break;
            case Configuration::eTemperatur:
                ui->label_sigmaTemp->setEnabled(true);
                ui->lineEdit_sigmaTemp->setEnabled(true);

                 if(this->tmpSensor->getDefaultAccuracy() != NULL){
                       QMap<QString,double> *sigmaPolar = this->tmpSensor->getDefaultAccuracy();
                        ui->lineEdit_sigmaTemp->setText(QString::number(sigmaPolar->value("sigmaTempDeg")));
                  }

                break;
            case Configuration::eLevel:
                ui->label_sigmaAngleXZ->setEnabled(true);
                ui->label_sigmaAngleYZ->setEnabled(true);
                ui->lineEdit_sigmaAngleXZ->setEnabled(true);
                ui->lineEdit_sigmaAngleYZ->setEnabled(true);

                if(this->tmpSensor->getDefaultAccuracy() != NULL){
                     QMap<QString,double> *sigmaLevel = this->tmpSensor->getDefaultAccuracy();
                      ui->lineEdit_sigmaAngleXZ->setText(QString::number(sigmaLevel->value("sigmaAngleXZ")));
                      ui->lineEdit_sigmaAngleYZ->setText(QString::number(sigmaLevel->value("sigmaAngleYZ")));
                 }
                break;
            case Configuration::eUndefined:

                if(this->tmpSensor->getDefaultAccuracy() != NULL){

                    ui->toolBox_accuracy->setItemText(4,this->tmpSensor->getUndefinedReadingName());

                    QMap<QString, double> undefSigma = *this->tmpSensor->getDefaultAccuracy();

                    QMapIterator<QString, double> j(undefSigma);
                    while(j.hasNext()){
                        j.next();

                            QLabel *l = new QLabel();
                            l->setText(j.key());
                            QLineEdit *le = new QLineEdit();
                            le->setText(QString::number(j.value()));

                            QHBoxLayout *layout = new QHBoxLayout();
                            layout->addWidget(l);
                            layout->addWidget(le);
                            layout->setStretch(0,1);
                            layout->setStretch(1,1);

                            masterAccuracyLayout->addLayout(layout);

                            undefinedSigmaLabel.insert(j.key(),l);
                            undefinedSigma.insert(j.key(), le);
                            accuracyLayouts.insert(j.key(),layout);

                    }
                    ui->page_sigmaUndefined->setLayout(masterAccuracyLayout);
                }

                break;
            default:
                break;
            }
        }
    }
}

void SensorPluginDialog::destructDynamicGUI()
{

    //sensor config labels, lineedits, comboboxes. Here they all get deleted and removed from GUI

    QMapIterator<QString, QLabel*> m(doubleParameterLabel);
    while(m.hasNext()){
        m.next();
        ui->tab_sensorConfiguration->layout()->removeWidget(m.value());
        delete m.value();
    }
    doubleParameterLabel.clear();

    QMapIterator<QString, QLineEdit*> n(doubleParameter);
    while(n.hasNext()){
        n.next();
        ui->tab_sensorConfiguration->layout()->removeWidget(n.value());
        delete n.value();
    }
    doubleParameter.clear();

    QMapIterator<QString, QLabel*> o(integerParameterLabel);
    while(o.hasNext()){
        o.next();
        ui->tab_sensorConfiguration->layout()->removeWidget(o.value());
        delete o.value();
    }
    integerParameterLabel.clear();

    QMapIterator<QString, QLineEdit*> p(integerParameter);
    while(p.hasNext()){
        p.next();
        ui->tab_sensorConfiguration->layout()->removeWidget(p.value());
        delete p.value();
    }
    integerParameter.clear();

    QMapIterator<QString, QLabel*> q(stringParameterLabel);
    while(q.hasNext()){
        q.next();
        ui->tab_sensorConfiguration->layout()->removeWidget(q.value());
        delete q.value();
    }
    stringParameterLabel.clear();

    QMapIterator<QString, QComboBox*> r(stringParameter);
    while(r.hasNext()){
        r.next();
        ui->tab_sensorConfiguration->layout()->removeWidget(r.value());
        delete r.value();
    }
    stringParameter.clear();

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

void SensorPluginDialog::getSensorParameters()
{
    if(this->tmpSensor != NULL && this->tmpSensor->getDoubleParameter() != NULL){

        QMap<QString, double> doubleparam = *this->tmpSensor->getDoubleParameter();

        QMapIterator<QString, double> j(doubleparam);
        while(j.hasNext()){
            j.next();

            QLabel *l = new QLabel();
            l->setText(j.key());
            QLineEdit *le = new QLineEdit();
            le->setText(QString::number(j.value()));

            QHBoxLayout *layout = new QHBoxLayout();
            layout->addWidget(l);
            layout->addWidget(le);
            layout->setStretch(0,1);
            layout->setStretch(1,1);

            masterSensorConfigLayout->addLayout(layout);

            doubleParameter.insert(j.key(),le);
            doubleParameterLabel.insert(j.key(),l);
            sensorConfigLayouts.insert(j.key(),layout);
        }
    }

    if(this->tmpSensor != NULL && this->tmpSensor->getIntegerParameter() != NULL){

        QMap<QString, int> intParameter = *this->tmpSensor->getIntegerParameter();

        QMapIterator<QString, int> k(intParameter);
        while(k.hasNext()){
            k.next();

            QLabel *l = new QLabel();
            l->setText(k.key());
            QLineEdit *le = new QLineEdit();
            le->setText(QString::number(k.value()));

            QHBoxLayout *layout = new QHBoxLayout();
            layout->addWidget(l);
            layout->addWidget(le);
            layout->setStretch(0,1);
            layout->setStretch(1,1);

            masterSensorConfigLayout->addLayout(layout);

            integerParameter.insert(k.key(),le);
            integerParameterLabel.insert(k.key(),l);
            sensorConfigLayouts.insert(k.key(),layout);
        }
    }

    if(this->tmpSensor != NULL && this->tmpSensor->getStringParameter() != NULL){

        QMap<QString,QStringList> strParameter = *this->tmpSensor->getStringParameter();

        QMapIterator<QString,QStringList> m(strParameter);
        while(m.hasNext()){
            m.next();

            QLabel *l = new QLabel();
            l->setText(m.key());
            QComboBox *cb = new QComboBox();
            for(int a=0;a< m.value().size();a++){
                cb->addItem(m.value().at(a));
            }

            QHBoxLayout *layout = new QHBoxLayout();
            layout->addWidget(l);
            layout->addWidget(cb);
            layout->setStretch(0,1);
            layout->setStretch(1,1);

            masterSensorConfigLayout->addLayout(layout);

            stringParameter.insert(m.key(),cb);
            stringParameterLabel.insert(m.key(),l);
            sensorConfigLayouts.insert(m.key(),layout);
        }
    }

    ui->tab_sensorConfiguration->setLayout(masterSensorConfigLayout);
}

void SensorPluginDialog::setLabelUnits()
{
    ui->label_sigmaAngleXZ->setText(QString("sigma angle XZ "+ UnitConverter::getAngleUnitString()));
    ui->label_sigmaAngleYZ->setText(QString("sigma angle YZ " + UnitConverter::getAngleUnitString()));
    ui->label_sigmaAzimuth->setText(QString("sigma azimuth "+ UnitConverter::getAngleUnitString()));
    ui->label_sigmaDistance->setText(QString("sigma distance " + UnitConverter::getDistanceUnitString()));
    ui->label_sigmaTemp->setText(QString("sigma temperature " + UnitConverter::getTemperatureUnitString()));
    ui->label_sigmaX->setText(QString("sigma x " + UnitConverter::getDistanceUnitString()));
    ui->label_sigmaY->setText(QString("sigma y " + UnitConverter::getDistanceUnitString()));
    ui->label_sigmaZ->setText(QString("sigma z " + UnitConverter::getDistanceUnitString()));
    ui->label_sigmaZenith->setText(QString("sigma zenith " + UnitConverter::getAngleUnitString()));
}
