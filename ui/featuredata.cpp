#include "featuredata.h"
#include "ui_featuredata.h"

/*!
 * \brief FeatureData constructor
 * \param parent
 */
FeatureData::FeatureData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FeatureData)
{
    ui->setupUi(this);

    selectedFeature = NULL;
    ui->tab_sensorInformation->setEnabled(false);
    sensorConfig = NULL;
    connectionChanged = false;
    masterAccuracyLayout = new QVBoxLayout();
    masterSensorConfigLayout = new QVBoxLayout();
    qxxModel = NULL;
    sxxModel = NULL;
}

/*!
 * \brief FeatureData destructor
 */
FeatureData::~FeatureData()
{
    delete ui;
}

/*!
 * \brief cancels the dialog without saving anything.
 */
void FeatureData::on_pushButton_cancel_clicked()
{
    this->close();
}

/*!
 * \brief closes the dialog without saving anything
 */
void FeatureData::on_pushButton_ok_clicked()
{
    this->close();
}

/*!
 * \brief receives the active feature and initiates the models and gui.
 * If the received feature is a station with an active instrument, you can change the instrument config with this dialog
 * and connect the instrument new, if the connection settings were changed.
 * \param FeatureWrapper activeFeature
 */
void FeatureData::getActiveFeature(FeatureWrapper *activeFeature){

    this->selectedFeature = activeFeature;
    this->setWindowTitle(QString("information abaout " + this->selectedFeature->getFeature()->name));
    rModel = new ReadingModel(*this->selectedFeature,0);
    oModel = new ObservationModel(*this->selectedFeature,0);

    ui->tableView_observation->setModel(oModel);
    ui->tableView_readings->setModel(rModel);

    ui->tableView_displayedfunctionStatistic->setModel(NULL);
    ui->tableView_qxxAposteriori->setModel(NULL);
    ui->tableView_sxxApriori->setModel(NULL);
    ui->label_s0aposterioriValue->setText("");
    ui->label_s0aprioriValue->setText("");

    initGUI();

    //station specific
    if(this->selectedFeature->getStation() != NULL && this->selectedFeature->getStation()->sensorPad->instrument != NULL){
        ui->label_activeSensor->setText(this->selectedFeature->getStation()->sensorPad->instrument->getMetaData()->name);
        ui->lineEdit_configName->setText(this->selectedFeature->getStation()->getInstrumentConfig()->name);
        ui->tab_sensorInformation->setEnabled(true);
        getReadingType();
        getSensorConfiguration();
        getSensorParameters();
        initConnection();
    }
}

void FeatureData::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
    event->accept();
}

/*!
 * \brief initiates the gui and fills the combobox with the functions of the given feature.
 */
void FeatureData::initGUI(){

    ui->comboBox_displayedFunction->clear();

    for(int i=0; i<this->selectedFeature->getFeature()->functionList.size();i++){
        ui->comboBox_displayedFunction->addItem(this->selectedFeature->getFeature()->functionList.at(i)->getMetaData()->name);
    }
}

/*!
 * \brief handles the initialisation and actualisation of the models, if the selected function is changed.
 * \param QString arg1
 */
void FeatureData::on_comboBox_displayedFunction_currentIndexChanged(const QString &arg1)
{
    int currentFunction = ui->comboBox_displayedFunction->currentIndex();

    if(currentFunction != -1){

        fModel = new FunctionStatistic();
        //fModel->setFunction(NULL);
        //ui->tableView_displayedfunctionStatistic->setModel(fModel);
        //fModel->updateModel();
        ui->tableView_displayedfunctionStatistic->setModel(NULL);

        qxxModel = new MatrixModel();
        sxxModel = new MatrixModel();

        ui->tableView_qxxAposteriori->setModel(qxxModel);
        ui->tableView_sxxApriori->setModel(sxxModel);

        if(this->selectedFeature->getFeature()->functionList.size()>0){
            ui->textBrowser_protocol->clear();

            for(int i=0; i<this->selectedFeature->getFeature()->functionList.at(currentFunction)->getResultProtocol().size();i++){
                ui->textBrowser_protocol->append(this->selectedFeature->getFeature()->functionList.at(currentFunction)->getResultProtocol().at(i));
            }

            if(this->selectedFeature->getFeature()->functionList.at(currentFunction)->getStatistic().isValid){
                OiMat tmpQxxOriginal = this->selectedFeature->getFeature()->functionList.at(currentFunction)->getStatistic().qxx;
                double tmps0aposteriori = this->selectedFeature->getFeature()->functionList.at(currentFunction)->getStatistic().s0_aposteriori;
                double tmps0apriori = this->selectedFeature->getFeature()->functionList.at(currentFunction)->getStatistic().s0_apriori;


                OiMat tmpQxx = (tmps0aposteriori*tmps0aposteriori)*tmpQxxOriginal;

                OiMat tmpSxx = (tmps0apriori*tmps0apriori)*tmpQxxOriginal;

                qxxModel->updateModel(tmpQxx);
                sxxModel->updateModel(tmpSxx);
                ui->label_s0aposterioriValue->setText(QString::number(this->selectedFeature->getFeature()->functionList.at(currentFunction)->getStatistic().s0_aposteriori,'f',6));
                ui->label_s0aprioriValue->setText(QString::number(this->selectedFeature->getFeature()->functionList.at(currentFunction)->getStatistic().s0_apriori,'f',6));
            }
        }
        /*
        if(this->selectedFeature->getPoint() == NULL){
            return;
        }*/

        if(this->selectedFeature->getFeature()->functionList.at(currentFunction)->getFeatureOrder().size() > 0){

            fModel->setFunction(this->selectedFeature->getFeature()->functionList.at(currentFunction));
            ui->tableView_displayedfunctionStatistic->setModel(fModel);
            fModel->updateModel();
        }else{
            /*fModel->setFunction(NULL);
            ui->tableView_displayedfunctionStatistic->setModel(fModel);
            fModel->updateModel();*/
            ui->tableView_displayedfunctionStatistic->setModel(NULL);
        }
    }

}

/*!
 * \brief overwritten closeevent that destructs the dynamic gui.
 * if the selected feature is a station some parts of the gui are created dynamically.
 * \param event
 */
void FeatureData::closeEvent(QCloseEvent *event)
{
    ui->tab_sensorInformation->setEnabled(false);
    disableAccuracyElements();
    destructDynamicGUI();
    disableConnectionSettings();
    connectionChanged = false;
    event->accept();
}

/*!
 * \brief displays the values of the feature in the gui.
 */
void FeatureData::getSensorConfiguration()
{
    ui->label_sigmaAzimuth->setText(QString("sigma azimuth " + UnitConverter::getAngleUnitString()));
    ui->label_sigmaDistance->setText(QString("sigma distance " + UnitConverter::getDistanceUnitString()));
    ui->label_sigmaTemperature->setText(QString("sigma temperature " + UnitConverter::getTemperatureUnitString()));
    ui->label_sigmaX->setText(QString("sigma x " + UnitConverter::getDistanceUnitString()));
    ui->label_sigmaXZ->setText(QString("sigma xz " + UnitConverter::getAngleUnitString()));
    ui->label_sigmaY->setText(QString("sigma y " + UnitConverter::getDistanceUnitString()));
    ui->label_sigmaYZ->setText(QString("sigma yz " + UnitConverter::getDistanceUnitString()));
    ui->label_sigmaZ->setText(QString("sigma z " + UnitConverter::getDistanceUnitString()));
    ui->label_sigmaZenith->setText(QString("sigma zenith " + UnitConverter::getAngleUnitString()));

    ui->lineEdit_sigmaAzimuth->setText(QString::number(this->selectedFeature->getStation()->getInstrumentConfig()->sigma.sigmaAzimuth*UnitConverter::getAngleMultiplier()));
    ui->lineEdit_sigmaDistance->setText(QString::number(this->selectedFeature->getStation()->getInstrumentConfig()->sigma.sigmaDistance*UnitConverter::getDistanceMultiplier()));
    ui->lineEdit_sigmaZenith->setText(QString::number(this->selectedFeature->getStation()->getInstrumentConfig()->sigma.sigmaZenith*UnitConverter::getAngleMultiplier()));
    ui->lineEdit_sigmaTemperature->setText(QString::number(UnitConverter::getTemperature(this->selectedFeature->getStation()->getInstrumentConfig()->sigma.sigmaTemp)));
    ui->lineEdit_sigmaX->setText(QString::number(this->selectedFeature->getStation()->getInstrumentConfig()->sigma.sigmaXyz.getAt(0)*UnitConverter::getDistanceMultiplier()));
    ui->lineEdit_sigmaY->setText(QString::number(this->selectedFeature->getStation()->getInstrumentConfig()->sigma.sigmaXyz.getAt(1)*UnitConverter::getDistanceMultiplier()));
    ui->lineEdit_sigmaZ->setText(QString::number(this->selectedFeature->getStation()->getInstrumentConfig()->sigma.sigmaXyz.getAt(2)*UnitConverter::getDistanceMultiplier()));
    ui->lineEdit_sigmaXZ->setText(QString::number(this->selectedFeature->getStation()->getInstrumentConfig()->sigma.sigmaAngleXZ*UnitConverter::getAngleMultiplier()));
    ui->lineEdit_sigmaYZ->setText(QString::number(this->selectedFeature->getStation()->getInstrumentConfig()->sigma.sigmaAngleYZ*UnitConverter::getAngleMultiplier()));
}

/*!
 * \brief enables the supported reading types and creates dynamic the unknown reading types given by the QMap.
 */
void FeatureData::getReadingType()
{
    if(this->selectedFeature->getStation()->sensorPad->instrument != NULL && this->selectedFeature->getStation()->sensorPad->instrument->getSupportedReadingTypes() != NULL){

        QList<Configuration::ReadingTypes> readingTypes = *this->selectedFeature->getStation()->sensorPad->instrument->getSupportedReadingTypes();

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

                if(this->selectedFeature->getStation()->sensorPad->instrument->getDefaultAccuracy() != NULL){

                    ui->toolBox_accuracy->setItemText(4,this->selectedFeature->getStation()->sensorPad->instrument->getUndefinedReadingName());

                    QMap<QString, double> undefSigma = *this->selectedFeature->getStation()->sensorPad->instrument->getDefaultAccuracy();

                    QMapIterator<QString, double> j(undefSigma);
                    while(j.hasNext()){
                        j.next();

                        QLabel *l = new QLabel();
                        l->setText(j.key());
                        QLineEdit *le = new QLineEdit();
                        //le->setText(QString::number(j.value()));
                        le->setText(QString::number(this->selectedFeature->getStation()->getInstrumentConfig()->sigma.sigmaUndefined.value(j.key())));

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
void FeatureData::disableAccuracyElements()
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
 * \brief destructs the dynamic gui generated by a station and its sensor functions.
 */
void FeatureData::destructDynamicGUI()
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

/*!
 * \brief creates the new sensor configuration with the given values via gui and sends it to the station
 */
void FeatureData::initSensorConfiguration()
{
    sensorConfig = new SensorConfiguration();

    sensorConfig->name = ui->lineEdit_configName->text();
    sensorConfig->instrumentType = this->selectedFeature->getStation()->getInstrumentConfig()->instrumentType;

    sensorConfig->connConfig->baudRate = static_cast<QSerialPort::BaudRate>(ui->comboBox_baudrate->itemData(ui->comboBox_baudrate->currentIndex()).toInt());
    sensorConfig->connConfig->comPort = ui->comboBox_comport->currentText();
    sensorConfig->connConfig->dataBits = static_cast<QSerialPort::DataBits>(ui->comboBox_databits->itemData(ui->comboBox_databits->currentIndex()).toInt());
    sensorConfig->connConfig->flowControl = static_cast<QSerialPort::FlowControl>(ui->comboBox_flowcontrol->itemData(ui->comboBox_flowcontrol->currentIndex()).toInt());
    sensorConfig->connConfig->ip = ui->comboBox_ip->currentText();
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
    }
}

/*!
 * \brief displays specified sensor parameters in the gui.
 * The sepecial gui elements are constructed dynamically.
 */
void FeatureData::getSensorParameters()
{
    if(this->selectedFeature->getStation()->sensorPad->instrument != NULL && this->selectedFeature->getStation()->sensorPad->instrument->getDoubleParameter() != NULL){

        QMap<QString, double> doubleparam = *this->selectedFeature->getStation()->sensorPad->instrument->getDoubleParameter();

        QMapIterator<QString, double> j(doubleparam);
        while(j.hasNext()){
            j.next();

            QLabel *l = new QLabel();
            l->setText(j.key());
            QLineEdit *le = new QLineEdit();
            //le->setText(QString::number(j.value()));
            le->setText(QString::number(this->selectedFeature->getStation()->getInstrumentConfig()->doubleParameter.value(j.key())));

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

    if(this->selectedFeature->getStation()->sensorPad->instrument != NULL && this->selectedFeature->getStation()->sensorPad->instrument->getIntegerParameter() != NULL){

        QMap<QString, int> intParameter = *this->selectedFeature->getStation()->sensorPad->instrument->getIntegerParameter();

        QMapIterator<QString, int> k(intParameter);
        while(k.hasNext()){
            k.next();

            QLabel *l = new QLabel();
            l->setText(k.key());
            QLineEdit *le = new QLineEdit();
            //le->setText(QString::number(k.value()));
            le->setText(QString::number(this->selectedFeature->getStation()->getInstrumentConfig()->integerParameter.value(k.key())));

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

    if(this->selectedFeature->getStation()->sensorPad->instrument != NULL && this->selectedFeature->getStation()->sensorPad->instrument->getStringParameter() != NULL){

        QMap<QString,QStringList> strParameter = *this->selectedFeature->getStation()->sensorPad->instrument->getStringParameter();

        QMapIterator<QString,QStringList> m(strParameter);
        while(m.hasNext()){
            m.next();

            QLabel *l = new QLabel();
            l->setText(m.key());
            QComboBox *cb = new QComboBox();
            for(int a=0;a< m.value().size();a++){
                cb->addItem(m.value().at(a));
            }
            cb->setCurrentIndex(cb->findText(this->selectedFeature->getStation()->getInstrumentConfig()->stringParameter.value(m.key())));

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

/*!
 * \brief sets the sensor configuration and connects the sensor if specified.
 */
void FeatureData::on_pushButton_setSensorConfig_clicked()
{
    if(connectionChanged){
        this->selectedFeature->getStation()->startDisconnect();
        initSensorConfiguration();
        this->selectedFeature->getStation()->setInstrumentConfig(this->sensorConfig);
        this->selectedFeature->getStation()->startConnect(this->selectedFeature->getStation()->getInstrumentConfig()->connConfig);
    }else{
        initSensorConfiguration();
        this->selectedFeature->getStation()->setInstrumentConfig(this->sensorConfig);
    }

}

/*!
 * \brief initiates the supported connection types of the sensor and displays possible settings in the gui.
 */
void FeatureData::initConnection()
{
    if(this->selectedFeature->getStation()->sensorPad->instrument->getConnectionType() != NULL){

        QList<Configuration::ConnectionTypes> conTypes = *this->selectedFeature->getStation()->sensorPad->instrument->getConnectionType();
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
    ui->comboBox_connectiontype->setCurrentIndex(ui->comboBox_connectiontype->findData(this->selectedFeature->getStation()->getInstrumentConfig()->connConfig->typeOfConnection));

    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"1200",QSerialPort::Baud1200);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"2400",QSerialPort::Baud2400);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"4800",QSerialPort::Baud4800);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"9600",QSerialPort::Baud9600);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"19200",QSerialPort::Baud19200);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"38400",QSerialPort::Baud38400);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"57600",QSerialPort::Baud57600);
    ui->comboBox_baudrate->insertItem(ui->comboBox_baudrate->count(),"115200",QSerialPort::Baud115200);
    ui->comboBox_baudrate->setCurrentIndex(ui->comboBox_baudrate->findData(this->selectedFeature->getStation()->getInstrumentConfig()->connConfig->baudRate));

    ui->comboBox_databits->insertItem(ui->comboBox_databits->count(),"5",QSerialPort::Data5);
    ui->comboBox_databits->insertItem(ui->comboBox_databits->count(),"6",QSerialPort::Data6);
    ui->comboBox_databits->insertItem(ui->comboBox_databits->count(),"7",QSerialPort::Data7);
    ui->comboBox_databits->insertItem(ui->comboBox_databits->count(),"8",QSerialPort::Data8);
    ui->comboBox_databits->insertItem(ui->comboBox_databits->count(),"unknown",QSerialPort::UnknownDataBits);
    ui->comboBox_databits->setCurrentIndex(ui->comboBox_databits->findData(this->selectedFeature->getStation()->getInstrumentConfig()->connConfig->dataBits));

    ui->comboBox_flowcontrol->insertItem(ui->comboBox_flowcontrol->count(),"no flowcontrol",QSerialPort::NoFlowControl);
    ui->comboBox_flowcontrol->insertItem(ui->comboBox_flowcontrol->count(),"hardware flowcontrol",QSerialPort::HardwareControl);
    ui->comboBox_flowcontrol->insertItem(ui->comboBox_flowcontrol->count(),"software flowcontrol",QSerialPort::SoftwareControl);
    ui->comboBox_flowcontrol->insertItem(ui->comboBox_flowcontrol->count(),"unknown flowcontrol",QSerialPort::UnknownFlowControl);
    ui->comboBox_flowcontrol->setCurrentIndex(ui->comboBox_flowcontrol->findData(this->selectedFeature->getStation()->getInstrumentConfig()->connConfig->flowControl));

    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"no parity",QSerialPort::NoParity);
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"even parity",QSerialPort::EvenParity);
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"odd parity",QSerialPort::OddParity);
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"space parity",QSerialPort::SpaceParity);
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"mark parity",QSerialPort::MarkParity);
    ui->comboBox_parity->insertItem(ui->comboBox_parity->count(),"unknown parity",QSerialPort::UnknownParity);
    ui->comboBox_parity->setCurrentIndex(ui->comboBox_parity->findData(this->selectedFeature->getStation()->getInstrumentConfig()->connConfig->parity));

    ui->comboBox_stopbits->insertItem(ui->comboBox_stopbits->count(),"one stop",QSerialPort::OneStop);
    ui->comboBox_stopbits->insertItem(ui->comboBox_stopbits->count(),"one and half stop",QSerialPort::OneAndHalfStop);
    ui->comboBox_stopbits->insertItem(ui->comboBox_stopbits->count(),"two stop",QSerialPort::TwoStop);
    ui->comboBox_stopbits->insertItem(ui->comboBox_stopbits->count(),"unknown stopbits",QSerialPort::UnknownStopBits);
    ui->comboBox_stopbits->setCurrentIndex(ui->comboBox_stopbits->findData(this->selectedFeature->getStation()->getInstrumentConfig()->connConfig->stopBits));

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        ui->comboBox_comport->insertItem(ui->comboBox_comport->count(),info.portName());
    }
    ui->comboBox_comport->setCurrentIndex(ui->comboBox_comport->findText(this->selectedFeature->getStation()->getInstrumentConfig()->connConfig->port));

    QList<QHostAddress> ipAdresses = QNetworkInterface::allAddresses();
    foreach(const QHostAddress &adress, ipAdresses){
        ui->comboBox_ip->insertItem(ui->comboBox_ip->count(),adress.toString());
    }
    ui->comboBox_ip->setCurrentIndex(ui->comboBox_ip->findText(this->selectedFeature->getStation()->getInstrumentConfig()->connConfig->ip));

    ui->lineEdit_port->setText(this->selectedFeature->getStation()->getInstrumentConfig()->connConfig->port);
}

/*!
 * \brief disables ConnectionSettings gui elements.
 */
void FeatureData::disableConnectionSettings()
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
void FeatureData::on_comboBox_connectiontype_currentIndexChanged(const QString &arg1)
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
 * \brief if clicked the sensor will be (re-) connected by closing this dialog.
 */
void FeatureData::on_pushButton_connectionChanged_clicked()
{
    connectionChanged = true;
}
