#include "stationinfodialog.h"
#include "ui_stationinfodialog.h"

StationInfoDialog::StationInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StationInfoDialog)
{
    ui->setupUi(this);
    masterAccuracyLayout = new QVBoxLayout();
    masterSensorConfigLayout = new QVBoxLayout();
}

StationInfoDialog::~StationInfoDialog()
{
    delete ui;
}

void StationInfoDialog::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
    ui->checkBox_connectionChanged->setChecked(false);

    ui->label_activeSensor->setText("no sensor connected");
    this->setWindowTitle(QString("information abaout " + OiFeatureState::getActiveFeature()->getFeature()->getFeatureName()));
    if(OiFeatureState::getActiveFeature()->getStation() != NULL && OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument != NULL){
        ui->label_activeSensor->setText(OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument->getMetaData()->name);
        ui->lineEdit_configName->setText(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig().getName());
        getReadingType();
        getSensorConfiguration();
        getSensorParameters();
        initConnection();
    }

    event->accept();
}

void StationInfoDialog::closeEvent(QCloseEvent *event)
{
    this->destructDynamicGUI();
    event->accept();
}

/*!
 * \brief sets the sensor configuration and connects the sensor if specified.
 */
void StationInfoDialog::on_toolButton_ok_clicked()
{
    if(ui->checkBox_connectionChanged->isChecked()){
        if(OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument->getConnectionState()){
            emit disconnectSensor();
        }
        //OiFeatureState::getActiveFeature()->getStation()->startDisconnect();
        initSensorConfiguration();
        OiFeatureState::getActiveFeature()->getStation()->setInstrumentConfig(this->sensorConfig);

        QMessageBox msgBox;
        msgBox.setText("Sensor disconnected, because connection config changed.");
        msgBox.setInformativeText("Want to reconnect?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Ok:
            emit connectSensor();
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
        }
            //OiFeatureState::getActiveFeature()->getStation()->startConnect(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig()->connConfig);

    }else{
        initSensorConfiguration();
        OiFeatureState::getActiveFeature()->getStation()->setInstrumentConfig(this->sensorConfig);
    }
    this->close();
}

void StationInfoDialog::on_toolButton_cancel_clicked()
{
    this->close();
}

void StationInfoDialog::initGUI()
{
}

/*!
 * \brief displays the values of the feature in the gui.
 */
void StationInfoDialog::getSensorConfiguration()
{
    /*ui->label_sigmaAzimuth->setText(QString("sigma azimuth " + OiUnitConverter::getAngleUnitString()));
    ui->label_sigmaDistance->setText(QString("sigma distance " + OiUnitConverter::getDistanceUnitString()));
    ui->label_sigmaTemperature->setText(QString("sigma temperature " + OiUnitConverter::getTemperatureUnitString()));
    ui->label_sigmaX->setText(QString("sigma x " + OiUnitConverter::getDistanceUnitString()));
    ui->label_sigmaXZ->setText(QString("sigma xz " + OiUnitConverter::getAngleUnitString()));
    ui->label_sigmaY->setText(QString("sigma y " + OiUnitConverter::getDistanceUnitString()));
    ui->label_sigmaYZ->setText(QString("sigma yz " + OiUnitConverter::getDistanceUnitString()));
    ui->label_sigmaZ->setText(QString("sigma z " + OiUnitConverter::getDistanceUnitString()));
    ui->label_sigmaZenith->setText(QString("sigma zenith " + OiUnitConverter::getAngleUnitString()));

    ui->lineEdit_sigmaAzimuth->setText(QString::number(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig().sigma.sigmaAzimuth*OiUnitConverter::getAngleMultiplier()));
    ui->lineEdit_sigmaDistance->setText(QString::number(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig().sigma.sigmaDistance*OiUnitConverter::getDistanceMultiplier()));
    ui->lineEdit_sigmaZenith->setText(QString::number(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig().sigma.sigmaZenith*OiUnitConverter::getAngleMultiplier()));
    ui->lineEdit_sigmaTemperature->setText(QString::number(OiUnitConverter::getTemperature(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig().sigma.sigmaTemp)));
    ui->lineEdit_sigmaX->setText(QString::number(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig().sigma.sigmaXyz.getAt(0)*OiUnitConverter::getDistanceMultiplier()));
    ui->lineEdit_sigmaY->setText(QString::number(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig().sigma.sigmaXyz.getAt(1)*OiUnitConverter::getDistanceMultiplier()));
    ui->lineEdit_sigmaZ->setText(QString::number(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig().sigma.sigmaXyz.getAt(2)*OiUnitConverter::getDistanceMultiplier()));
    ui->lineEdit_sigmaXZ->setText(QString::number(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig().sigma.sigmaAngleXZ*OiUnitConverter::getAngleMultiplier()));
    ui->lineEdit_sigmaYZ->setText(QString::number(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig().sigma.sigmaAngleYZ*OiUnitConverter::getAngleMultiplier()));*/
}

/*!
 * \brief enables the supported reading types and creates dynamic the unknown reading types given by the QMap.
 */
void StationInfoDialog::getReadingType()
{
    if(OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument != NULL
            && OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument->getSupportedReadingTypes() != NULL){

        QList<Configuration::ReadingTypes> readingTypes = *OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument->getSupportedReadingTypes();

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
                break;
            case Configuration::eCartesian:
                ui->label_sigmaX->setEnabled(true);
                ui->label_sigmaY->setEnabled(true);
                ui->label_sigmaZ->setEnabled(true);
                ui->lineEdit_sigmaX->setEnabled(true);
                ui->lineEdit_sigmaY->setEnabled(true);
                ui->lineEdit_sigmaZ->setEnabled(true);
                break;
            case Configuration::eDirection:
                ui->label_sigmaAzimuth->setEnabled(true);
                ui->label_sigmaZenith->setEnabled(true);
                ui->lineEdit_sigmaAzimuth->setEnabled(true);
                ui->lineEdit_sigmaZenith->setEnabled(true);
                break;
            case Configuration::eDistance:
                ui->label_sigmaDistance->setEnabled(true);
                ui->lineEdit_sigmaDistance->setEnabled(true);
                break;
            case Configuration::eTemperatur:
                ui->label_sigmaTemperature->setEnabled(true);
                ui->lineEdit_sigmaTemperature->setEnabled(true);
                break;
            case Configuration::eLevel:
                ui->label_sigmaXZ->setEnabled(true);
                ui->label_sigmaYZ->setEnabled(true);
                ui->lineEdit_sigmaXZ->setEnabled(true);
                ui->lineEdit_sigmaYZ->setEnabled(true);
                break;
            case Configuration::eUndefined:

                if(OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument->getDefaultAccuracy() != NULL){

                    ui->toolBox_accuracy->setItemText(4,OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument->getUndefinedReadingName());

                    QMap<QString, double> undefSigma = *OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument->getDefaultAccuracy();

                    QMapIterator<QString, double> j(undefSigma);
                    while(j.hasNext()){
                        j.next();

                        QLabel *l = new QLabel();
                        l->setText(j.key());
                        QLineEdit *le = new QLineEdit();
                        //le->setText(QString::number(j.value()));
                        le->setText(QString::number(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig().sigma.sigmaUndefined.value(j.key())));

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

/*!
 * \brief disables all gui elements at the end of using the dialog.
 */
void StationInfoDialog::disableAccuracyElements()
{
    ui->label_sigmaAzimuth->setEnabled(false);
    ui->label_sigmaZenith->setEnabled(false);
    ui->label_sigmaDistance->setEnabled(false);
    ui->label_sigmaX->setEnabled(false);
    ui->label_sigmaY->setEnabled(false);
    ui->label_sigmaZ->setEnabled(false);
    ui->label_sigmaTemperature->setEnabled(false);
    ui->label_sigmaXZ->setEnabled(false);
    ui->label_sigmaYZ->setEnabled(false);

    ui->lineEdit_sigmaAzimuth->setEnabled(false);
    ui->lineEdit_sigmaZenith->setEnabled(false);
    ui->lineEdit_sigmaDistance->setEnabled(false);
    ui->lineEdit_sigmaX->setEnabled(false);
    ui->lineEdit_sigmaY->setEnabled(false);
    ui->lineEdit_sigmaZ->setEnabled(false);
    ui->lineEdit_sigmaTemperature->setEnabled(false);
    ui->lineEdit_sigmaXZ->setEnabled(false);
    ui->lineEdit_sigmaYZ->setEnabled(false);
}

/*!
 * \brief displays specified sensor parameters in the gui.
 * The sepecial gui elements are constructed dynamically.
 */
void StationInfoDialog::getSensorParameters()
{
    if(OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument != NULL && OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument->getDoubleParameter() != NULL){

        QMap<QString, double> doubleparam = *OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument->getDoubleParameter();

        QMapIterator<QString, double> j(doubleparam);
        while(j.hasNext()){
            j.next();

            QLabel *l = new QLabel();
            l->setText(j.key());
            QLineEdit *le = new QLineEdit();
            //le->setText(QString::number(j.value()));
            le->setText(QString::number(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig().doubleParameter.value(j.key())));

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

    if(OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument != NULL && OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument->getIntegerParameter() != NULL){

        QMap<QString, int> intParameter = *OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument->getIntegerParameter();

        QMapIterator<QString, int> k(intParameter);
        while(k.hasNext()){
            k.next();

            QLabel *l = new QLabel();
            l->setText(k.key());
            QLineEdit *le = new QLineEdit();
            //le->setText(QString::number(k.value()));
            le->setText(QString::number(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig().integerParameter.value(k.key())));

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

    if(OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument != NULL && OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument->getStringParameter() != NULL){

        QMap<QString,QStringList> strParameter = *OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument->getStringParameter();

        QMapIterator<QString,QStringList> m(strParameter);
        while(m.hasNext()){
            m.next();

            QLabel *l = new QLabel();
            l->setText(m.key());
            QComboBox *cb = new QComboBox();
            for(int a=0;a< m.value().size();a++){
                cb->addItem(m.value().at(a));
            }
            cb->setCurrentIndex(cb->findText(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig().stringParameter.value(m.key())));

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

    ui->tab_sensorConfig->setLayout(masterSensorConfigLayout);
}

/*!
 * \brief destructs the dynamic gui generated by a station and its sensor functions.
 */
void StationInfoDialog::destructDynamicGUI()
{
    //sensor config labels, lineedits, comboboxes. Here they all get deleted and removed from GUI

    QMapIterator<QString, QLabel*> m(doubleParameterLabel);
    while(m.hasNext()){
        m.next();
        ui->tab_sensorConfig->layout()->removeWidget(m.value());
        delete m.value();
    }
    doubleParameterLabel.clear();

    QMapIterator<QString, QLineEdit*> n(doubleParameter);
    while(n.hasNext()){
        n.next();
        ui->tab_sensorConfig->layout()->removeWidget(n.value());
        delete n.value();
    }
    doubleParameter.clear();

    QMapIterator<QString, QLabel*> o(integerParameterLabel);
    while(o.hasNext()){
        o.next();
        ui->tab_sensorConfig->layout()->removeWidget(o.value());
        delete o.value();
    }
    integerParameterLabel.clear();

    QMapIterator<QString, QLineEdit*> p(integerParameter);
    while(p.hasNext()){
        p.next();
        ui->tab_sensorConfig->layout()->removeWidget(p.value());
        delete p.value();
    }
    integerParameter.clear();

    QMapIterator<QString, QLabel*> q(stringParameterLabel);
    while(q.hasNext()){
        q.next();
        ui->tab_sensorConfig->layout()->removeWidget(q.value());
        delete q.value();
    }
    stringParameterLabel.clear();

    QMapIterator<QString, QComboBox*> r(stringParameter);
    while(r.hasNext()){
        r.next();
        ui->tab_sensorConfig->layout()->removeWidget(r.value());
        delete r.value();
    }
    stringParameter.clear();

    QMapIterator<QString, QLayout*> s(sensorConfigLayouts);
    while(s.hasNext()){
        s.next();
        ui->tab_sensorConfig->layout()->removeItem(s.value());
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
 * \brief creates the new sensor configuration with the given values via gui and sends it to the station
 */
void StationInfoDialog::initSensorConfiguration()
{
    /*sensorConfig = new SensorConfiguration();

    //sensorConfig->name = ui->lineEdit_configName->text();
    sensorConfig->instrumentType = OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig()->instrumentType;

    sensorConfig->connConfig->baudRate = static_cast<QSerialPort::BaudRate>(ui->comboBox_baudrate->itemData(ui->comboBox_baudrate->currentIndex()).toInt());
    sensorConfig->connConfig->comPort = ui->comboBox_comport->currentText();
    sensorConfig->connConfig->dataBits = static_cast<QSerialPort::DataBits>(ui->comboBox_databits->itemData(ui->comboBox_databits->currentIndex()).toInt());
    sensorConfig->connConfig->flowControl = static_cast<QSerialPort::FlowControl>(ui->comboBox_flowcontrol->itemData(ui->comboBox_flowcontrol->currentIndex()).toInt());
    if(ui->lineEdit_ip->text().size() == 0){
        sensorConfig->connConfig->ip = ui->comboBox_ip->currentText();
    }else{
        sensorConfig->connConfig->ip = ui->lineEdit_ip->text();
    }
    sensorConfig->connConfig->parity = static_cast<QSerialPort::Parity>(ui->comboBox_parity->itemData(ui->comboBox_parity->currentIndex()).toInt());
    sensorConfig->connConfig->port = ui->lineEdit_port->text();
    sensorConfig->connConfig->stopBits = static_cast<QSerialPort::StopBits>(ui->comboBox_stopbits->itemData(ui->comboBox_stopbits->currentIndex()).toInt());
    sensorConfig->connConfig->typeOfConnection = static_cast<Configuration::ConnectionTypes>(ui->comboBox_connectiontype->itemData(ui->comboBox_connectiontype->currentIndex()).toInt());

    sensorConfig->sigma.sigmaAngleXZ = ui->lineEdit_sigmaXZ->text().toDouble()/UnitConverter::getAngleMultiplier();
    sensorConfig->sigma.sigmaAngleYZ = ui->lineEdit_sigmaYZ->text().toDouble()/UnitConverter::getAngleMultiplier();
    sensorConfig->sigma.sigmaAzimuth = ui->lineEdit_sigmaAzimuth->text().toDouble()/UnitConverter::getAngleMultiplier();
    sensorConfig->sigma.sigmaDistance = ui->lineEdit_sigmaDistance->text().toDouble()/UnitConverter::getDistanceMultiplier();
    sensorConfig->sigma.sigmaTemp = UnitConverter::getReverseTemperature(ui->lineEdit_sigmaTemperature->text().toDouble());
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
    }*/
}

/*!
 * \brief initiates the supported connection types of the sensor and displays possible settings in the gui.
 */
void StationInfoDialog::initConnection()
{
    /*if(OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument->getConnectionType() != NULL){

        QList<Configuration::ConnectionTypes> conTypes = *OiFeatureState::getActiveFeature()->getStation()->sensorPad->instrument->getConnectionType();
        for(int i=0; i<conTypes.size();i++){
            switch (conTypes.at(i)) {
            case Configuration::eNetwork:
                ui->comboBox_connectiontype->insertItem(ui->comboBox_connectiontype->count(),"network",Configuration::eNetwork);
                break;
            case Configuration::eSerial:
                ui->comboBox_connectiontype->insertItem(ui->comboBox_connectiontype->count(),"serial", Configuration::eSerial);
                break;
            default:
                break;
            }
        }

    }
    ui->comboBox_connectiontype->setCurrentIndex(ui->comboBox_connectiontype->findData(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig()->connConfig->typeOfConnection));

    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"1200",QSerialPort::Baud1200);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"2400",QSerialPort::Baud2400);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"4800",QSerialPort::Baud4800);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"9600",QSerialPort::Baud9600);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"19200",QSerialPort::Baud19200);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"38400",QSerialPort::Baud38400);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"57600",QSerialPort::Baud57600);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"115200",QSerialPort::Baud115200);
    ui->comboBox_baudrate->setCurrentIndex(ui->comboBox_baudrate->findData(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig()->connConfig->baudRate));

    ui->comboBox_databits->insertItem(ui->comboBox_databits->count(),"5",QSerialPort::Data5);
    ui->comboBox_databits->insertItem(ui->comboBox_databits->count(),"6",QSerialPort::Data6);
    ui->comboBox_databits->insertItem(ui->comboBox_databits->count(),"7",QSerialPort::Data7);
    ui->comboBox_databits->insertItem(ui->comboBox_databits->count(),"8",QSerialPort::Data8);
    ui->comboBox_databits->insertItem(ui->comboBox_databits->count(),"unknown",QSerialPort::UnknownDataBits);
    ui->comboBox_databits->setCurrentIndex(ui->comboBox_databits->findData(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig()->connConfig->dataBits));

    ui->comboBox_flowcontrol->insertItem(ui->comboBox_flowcontrol->count(),"no flowcontrol",QSerialPort::NoFlowControl);
    ui->comboBox_flowcontrol->insertItem(ui->comboBox_flowcontrol->count(),"hardware flowcontrol",QSerialPort::HardwareControl);
    ui->comboBox_flowcontrol->insertItem(ui->comboBox_flowcontrol->count(),"software flowcontrol",QSerialPort::SoftwareControl);
    ui->comboBox_flowcontrol->insertItem(ui->comboBox_flowcontrol->count(),"unknown flowcontrol",QSerialPort::UnknownFlowControl);
    ui->comboBox_flowcontrol->setCurrentIndex(ui->comboBox_flowcontrol->findData(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig()->connConfig->flowControl));

    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"no parity",QSerialPort::NoParity);
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"even parity",QSerialPort::EvenParity);
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"odd parity",QSerialPort::OddParity);
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"space parity",QSerialPort::SpaceParity);
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"mark parity",QSerialPort::MarkParity);
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"unknown parity",QSerialPort::UnknownParity);
    ui->comboBox_parity->setCurrentIndex(ui->comboBox_parity->findData(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig()->connConfig->parity));

    ui->comboBox_stopbits->insertItem(ui->comboBox_stopbits->count(),"one stop",QSerialPort::OneStop);
    ui->comboBox_stopbits->insertItem(ui->comboBox_stopbits->count(),"one and half stop",QSerialPort::OneAndHalfStop);
    ui->comboBox_stopbits->insertItem(ui->comboBox_stopbits->count(),"two stop",QSerialPort::TwoStop);
    ui->comboBox_stopbits->insertItem(ui->comboBox_stopbits->count(),"unknown stopbits",QSerialPort::UnknownStopBits);
    ui->comboBox_stopbits->setCurrentIndex(ui->comboBox_stopbits->findData(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig()->connConfig->stopBits));

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        ui->comboBox_comport->insertItem(ui->comboBox_comport->count(),info.portName());
    }
    ui->comboBox_comport->setCurrentIndex(ui->comboBox_comport->findText(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig()->connConfig->port));

    QList<QHostAddress> ipAdresses = QNetworkInterface::allAddresses();
    foreach(const QHostAddress &adress, ipAdresses){
        ui->comboBox_ip->insertItem(ui->comboBox_ip->count(),adress.toString());
    }
    ui->comboBox_ip->setCurrentIndex(ui->comboBox_ip->findText(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig()->connConfig->ip));
    ui->lineEdit_ip->setText(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig()->connConfig->ip);
    ui->lineEdit_port->setText(OiFeatureState::getActiveFeature()->getStation()->getInstrumentConfig()->connConfig->port);
*/
}

/*!
 * \brief disables ConnectionSettings gui elements.
 */
void StationInfoDialog::disableConnectionSettings()
{
    ui->comboBox_connectiontype->clear();
    ui->comboBox_baudrate->setEnabled(false);
    ui->comboBox_comport->setEnabled(false);
    ui->comboBox_databits->setEnabled(false);
    ui->comboBox_flowcontrol->setEnabled(false);
    ui->comboBox_ip->setEnabled(false);
    ui->comboBox_parity->setEnabled(false);
    ui->comboBox_stopbits->setEnabled(false);
    ui->lineEdit_port->setEnabled(false);
}

/*!
 * \brief enables or disables possible gui elements for connection types depending on the selected connection type.
 * \param arg1
 */
void StationInfoDialog::on_comboBox_connectiontype_currentIndexChanged(const QString &arg1)
{
    if(ui->comboBox_connectiontype->currentIndex() != -1){
        Configuration::ConnectionTypes conType = static_cast<Configuration::ConnectionTypes>(ui->comboBox_connectiontype->itemData(ui->comboBox_connectiontype->currentIndex()).toInt());

        switch (conType) {
        case Configuration::eNetwork:
            ui->comboBox_ip->setEnabled(true);
            ui->lineEdit_port->setEnabled(true);
            ui->comboBox_comport->setEnabled(false);
            ui->comboBox_baudrate->setEnabled(false);
            ui->comboBox_databits->setEnabled(false);
            ui->comboBox_flowcontrol->setEnabled(false);
            ui->comboBox_parity->setEnabled(false);
            ui->comboBox_stopbits->setEnabled(false);
            break;
        case Configuration::eSerial:
            ui->comboBox_comport->setEnabled(true);
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

/*!
 * \brief opens the propertie dialog of the station point.
 */
void StationInfoDialog::on_toolButton_pointProperties_clicked()
{
    emit showStationGeomProperties(OiFeatureState::getActiveFeature());
}
