#include "watchwindow.h"
#include "ui_watchwindow.h"

WatchWindow::WatchWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WatchWindow)
{
    ui->setupUi(this);

    //isGUIReady = false;
    digitCount = 4;

    masterLayout = new QVBoxLayout();
    settingsLayout = new QVBoxLayout();

    ui->groupBox_displayValues->setLayout(settingsLayout);

    listener = new WatchWindowListener();

    connect(this,SIGNAL(sendCheckBoxReady(bool)),this->listener,SLOT(setCheckBoxReady(bool)));
    connect(this,SIGNAL(sendGUIReady(bool)),this->listener,SLOT(setGUIReady(bool)));
    connect(this,SIGNAL(sendSettingsReady(bool)),this->listener,SLOT(setSettingsReady(bool)));
    connect(this->listener,SIGNAL(sendAttributeNames(QStringList)),this,SLOT(getAttributes(QStringList)));
    connect(this->listener,SIGNAL(sendData(QVariantMap)),this,SLOT(setLCDNumber(QVariantMap)));

    listener->moveToThread(&listenerThread);
    listenerThread.start();

}

WatchWindow::~WatchWindow()
{
    delete ui;
}


void WatchWindow::setLCDNumber(QVariantMap m){

    /*if(!listener->isGUIReady){
       this->iniGUI();
    }*/
/*
    //change display, depending on checked setting checkboxes.
    iniGUI();


    QMapIterator<QString,QVariant> j(m);
    while (j.hasNext()) {
        j.next();

        for(int i=0;i<this->checkboxes.size();i++){
            if(this->checkboxes.at(i)->text() == j.key() && this->checkboxes.at(i)->isChecked()){

                QString name = j.key();
                QVariant qvalue = j.value();
                double dvalue = qvalue.toDouble();

                if(!OiFeatureState::getActiveStation()->coordSys->getIsActiveCoordinateSystem()){

                    TrafoParam *tp = NULL;
                    QList<TrafoParam*> myTrafoParams = OiFeatureState::getActiveStation()->coordSys->getTransformationParameters(OiFeatureState::getActiveStation()->coordSys);
                    if(myTrafoParams.size() > 0){
                        tp = myTrafoParams.at(0);
                    }
                    if(tp != NULL){
                        OiMat t;
                        if(tp->getDestinationSystem() == OiFeatureState::getActiveCoordinateSystem()){
                            t = tp->getHomogenMatrix();
                        }else{
                            t = tp->getHomogenMatrix().inv();
                        }

                        OiVec trackerXYZ(4);
                        trackerXYZ.setAt(0,m.value("x").toDouble());
                        trackerXYZ.setAt(1,m.value("y").toDouble());
                        trackerXYZ.setAt(2,m.value("z").toDouble());
                        trackerXYZ.setAt(3,1.0);
                        trackerXYZ = t*trackerXYZ;

                        if(name == "x"){
                            dvalue =trackerXYZ.getAt(0);
                        }else if (name == "y"){
                            dvalue = trackerXYZ.getAt(1);
                        }else if (name == "z"){
                            dvalue =trackerXYZ.getAt(2);
                        }
                    }
                }

                if(OiFeatureState::getActiveFeature() != NULL){
                    if(name == "x"){

                        double featureX = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ().getAt(0);
                        double dx = featureX - dvalue;
                        streamData.value(name)->display(QString::number(dx*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));

                    }else if(name == "y"){

                        double featureY = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ().getAt(1);
                        double dy = featureY - dvalue;
                        streamData.value(name)->display(QString::number(dy*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));

                    }else if(name == "z"){

                        double featureZ = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ().getAt(2);
                        double dz = featureZ - dvalue;
                        streamData.value(name)->display(QString::number(dz*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));

                    }else{
                        streamData.value(name)->display(QString::number(dvalue,'f',6));
                    }
                }else{
                    QLCDNumber *lcdn = streamData.value(name);
                    if(lcdn != NULL){
                        lcdn->display(QString::number(dvalue*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));
                    }
                }
            }
        }
    }
*/
    //new implementation

    //change display, depending on checked setting checkboxes.
    iniGUI();

    QMapIterator<QString,QVariant> j(m);
    while (j.hasNext()) {
        j.next();

        OiMat trafo;

        for(int i=0;i<this->checkboxes.size();i++){

            if(this->checkboxes.at(i)->text() == j.key() && this->checkboxes.at(i)->isChecked()){

                if(!OiFeatureState::getActiveStation()->coordSys->getIsActiveCoordinateSystem()){

                    trafo = FeatureUpdater::trafoControl.getTransformationMatrix(OiFeatureState::getActiveStation()->coordSys);
                }

                OiVec trackerXYZ(4);
                OiVec trackerValues(4);

                switch (activeReadingType) {
                case Configuration::eCartesian:

                    trackerXYZ.setAt(0,m.value("x").toDouble());
                    trackerXYZ.setAt(1,m.value("y").toDouble());
                    trackerXYZ.setAt(2,m.value("z").toDouble());
                    trackerXYZ.setAt(3,1.0);

                    if(trafo.getRowCount() == 4 && trafo.getColCount() == 4){
                        trackerXYZ = trafo*trackerXYZ;
                    }

                    if(j.key() == "x"){
                        double featureX = 0.0;

                        if(this->checkFeatureValid()){
                            featureX = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ().getAt(0);
                        }
                        double dX = featureX - trackerXYZ.getAt(0);
                        streamData.value("x")->display(QString::number(dX*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));

                    }else if(j.key() == "y"){
                        double featureY = 0.0;

                        if(this->checkFeatureValid()){
                            featureY = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ().getAt(1);
                        }
                        double dY = featureY - trackerXYZ.getAt(1);
                        streamData.value("y")->display(QString::number(dY*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));

                    }else if(j.key() == "z"){
                        double featureZ = 0.0;

                        if(this->checkFeatureValid()){
                            featureZ = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ().getAt(1);
                        }
                        double dZ = featureZ - trackerXYZ.getAt(2);
                        streamData.value("z")->display(QString::number(dZ*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));

                    }else{
                        streamData.value(j.key())->display(QString::number(j.value().toDouble(),'f',6));
                    }

                    break;
                case Configuration::ePolar:

                    trackerValues.setAt(0,m.value("azimuth").toDouble());
                    trackerValues.setAt(1,m.value("zenith").toDouble());
                    trackerValues.setAt(2,m.value("distance").toDouble());
                    trackerValues.setAt(3,1.0);

                    trackerValues = Reading::toCartesian(trackerValues.getAt(0),trackerValues.getAt(1),trackerValues.getAt(2));


                    if(trafo.getRowCount() == 4 && trafo.getColCount() == 4){
                        trackerValues = trafo*trackerValues;
                    }

                    trackerValues = Reading::toPolar(trackerValues.getAt(0),trackerValues.getAt(1),trackerValues.getAt(2));

                    if(j.key() == "azimuth"){
                        double FeatureAZ = 0.0;

                        if(this->checkFeatureValid()){
                            OiVec xyz = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ();
                            OiVec featurePolar = Reading::toPolar(xyz.getAt(0),xyz.getAt(1),xyz.getAt(2));
                            FeatureAZ = featurePolar.getAt(0);
                        }
                        double dAZ = FeatureAZ - trackerValues.getAt(0);
                        streamData.value("azimuth")->display(QString::number(dAZ*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits));

                    }else if(j.key() == "zenith"){
                        double FeatureZE = 0.0;

                        if(this->checkFeatureValid()){
                            OiVec xyz = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ();
                            OiVec featurePolar = Reading::toPolar(xyz.getAt(0),xyz.getAt(1),xyz.getAt(2));
                            FeatureZE = featurePolar.getAt(1);
                        }
                        double dZE = FeatureZE - trackerValues.getAt(1);
                        streamData.value("zenith")->display(QString::number(dZE*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits));
                    }else if(j.key() == "distance"){
                        double FeatureDIS = 0.0;

                        if(this->checkFeatureValid()){
                            OiVec xyz = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ();
                            OiVec featurePolar = Reading::toPolar(xyz.getAt(0),xyz.getAt(1),xyz.getAt(2));
                            FeatureDIS = featurePolar.getAt(2);
                        }
                        double dDIS = FeatureDIS - trackerValues.getAt(2);
                        streamData.value("distance")->display(QString::number(dDIS*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));
                    }else{
                        streamData.value(j.key())->display(QString::number(j.value().toDouble(),'f',6));
                    }

                    break;
                case Configuration::eDirection:
                    break;
                case Configuration::eDistance:
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void WatchWindow::iniGUI()
{
    streamData.clear();

    for(int i=0; i<widgets.size();i++){
        delete widgets.at(i);
    }
    for(int i=0; i<layouts.size();i++){
        delete layouts.at(i);
    }
    layouts.clear();
    widgets.clear();

    delete masterLayout;
    masterLayout = NULL;

    if(masterLayout == NULL){
        masterLayout = new QVBoxLayout();
    }

    if(OiFeatureState::getActiveFeature() != NULL){
        QLabel *featureName = new QLabel();
        QFont f( "Arial", 30, QFont::Bold);
        featureName->setFont(f);
        featureName->setText(OiFeatureState::getActiveFeature()->getFeature()->getFeatureName());
        masterLayout->addWidget(featureName);
        widgets.append(featureName);
    }

    for(int i=0; i<this->checkboxes.size();i++){

        if(this->checkboxes.at(i)->isChecked()){
            QString name = this->checkboxes.at(i)->text();
            QString value = "0.0";
            QFont f( "Arial", 60, QFont::Bold);

            QLabel *l = new QLabel();
            l->setText(name);
            l->setFont(f);
            widgets.append(l);

            QLCDNumber *n = new QLCDNumber();
            n->display(value);
            n->setFont(f);
            widgets.append(n);

            n->setMode(QLCDNumber::Dec);
            n->setDigitCount(10);
            n->setSmallDecimalPoint(true);

            QHBoxLayout *layout = new QHBoxLayout();
            layout->addWidget(l);
            layout->addWidget(n);
            layout->setStretch(0,1);
            layout->setStretch(1,4);
            layouts.append(layout);

            masterLayout->addLayout(layout);

            streamData.insert(name,n);
        }
    }

    ui->pageWatchWindow->setLayout(masterLayout);

    //emit sendGUIReady(true);
    listener->isGUIReady = true;
}

void WatchWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_F3){
        emit startMeasure();
    }
}

void WatchWindow::closeEvent(QCloseEvent *e)
{
    OiFeatureState::getActiveStation()->emitStopReadingStream();
    streamData.clear();

    disconnect(OiFeatureState::getActiveStation()->sensorPad->instrumentListener,SIGNAL(sendReadingMap(QVariantMap)),this->listener,SLOT(setLCDNumber(QVariantMap)));

    delete masterLayout;
    masterLayout = NULL;

    delete settingsLayout;
    settingsLayout = NULL;

    listenerThread.quit();
    listenerThread.wait();

    delete this;

    e->accept();

}

void WatchWindow::showEvent(QShowEvent *event)
{
    listenerThread.start();

    if(OiFeatureState::getActiveStation() != NULL && OiFeatureState::getActiveStation()->sensorPad->instrument != NULL){
        connect(OiFeatureState::getActiveStation()->sensorPad->instrumentListener,SIGNAL(sendReadingMap(QVariantMap)),this->listener,SLOT(setLCDNumber(QVariantMap)));

        initSuppReadings();

        event->accept();
    }
}

/*!
 * \brief WatchWindow::initSuppReadings adds the supported reading types of the sensor in the combobox
 */
void WatchWindow::initSuppReadings()
{
    ui->comboBox_suppReadings->clear();

    QList<Configuration::ReadingTypes> *rTypes = OiFeatureState::getActiveStation()->sensorPad->instrument->getSupportedReadingTypes();

    if(rTypes == NULL){
        return;
    }
    listener->isSettingsReady = true;

    for(int i=0; i<rTypes->size();i++){
        switch (rTypes->at(i)) {
        case Configuration::ePolar:
            ui->comboBox_suppReadings->addItem(Configuration::sPolar);
            break;
        case Configuration::eCartesian:
            ui->comboBox_suppReadings->addItem(Configuration::sCartesian);
            break;
        case Configuration::eDistance:
            ui->comboBox_suppReadings->addItem(Configuration::sDistance);
            break;
        case Configuration::eDirection:
            ui->comboBox_suppReadings->addItem(Configuration::sDirection);
            break;
        case Configuration::eTemperatur:
            ui->comboBox_suppReadings->addItem(Configuration::sTemperatur);
            break;
        case Configuration::eLevel:
            ui->comboBox_suppReadings->addItem(Configuration::sLevel);
            break;
        case Configuration::eUndefined:
            ui->comboBox_suppReadings->addItem("undefined");
            break;
        default:
            break;
        }
    }
    /*if(ui->comboBox_suppReadings->findText(Configuration::sCartesian) != -1){
        ui->comboBox_suppReadings->setCurrentText(Configuration::sCartesian);
    }*/
    //emit sendSettingsReady(true);
    //listener->isSettingsReady = true;

    //on_comboBox_suppReadings_currentIndexChanged(ui->comboBox_suppReadings->currentText());
}

void WatchWindow::getAttributes(QStringList l)
{
    stopStream();


        if(listener->isCheckBoxReady){

        }else{
            if(settingsLayout == NULL){
                settingsLayout = new QVBoxLayout();
            }

            foreach (QCheckBox *cb, checkboxes) {
                ui->groupBox_displayValues->layout()->removeWidget(cb);
                delete cb;
            }
            checkboxes.clear();

            if(l.size() > 0){

                for(int i=0; i<l.size();i++){
                    QCheckBox *cb = new QCheckBox();
                    cb->setText(l.at(i));
                    cb->setChecked(true);

                    settingsLayout->addWidget(cb);

                    this->checkboxes.append(cb);
                }

                //emit sendCheckBoxReady(true);
                listener->isCheckBoxReady = true;
            }

        }

        iniGUI();


     startStream();
}


/*!
 * \brief WatchWindow::on_comboBox_suppReadings_currentIndexChanged
 * \param arg1
 */
void WatchWindow::on_comboBox_suppReadings_currentIndexChanged(const QString &arg1)
{

    stopStream();

    listener->isCheckBoxReady = false;


    if(!listener->isSettingsReady){
        return;
    }


    if(arg1.compare(Configuration::sCartesian) == 0){
        activeReadingType = Configuration::eCartesian;
    }else if(arg1.compare(Configuration::sPolar) == 0){
        activeReadingType = Configuration::ePolar;
    }else if(arg1.compare(Configuration::sDistance) == 0){
        activeReadingType = Configuration::eDistance;
    }else if(arg1.compare(Configuration::sDirection) == 0){
        activeReadingType = Configuration::eDirection;
    }else if(arg1.compare(Configuration::sLevel) == 0){
        activeReadingType = Configuration::eLevel;
    }else if(arg1.compare(Configuration::sTemperatur) == 0){
        activeReadingType = Configuration::eTemperatur;
    }else if(arg1.compare("undefined") == 0){
        activeReadingType = Configuration::eUndefined;
    }

    //emit sendGUIReady(false);
    listener->isGUIReady = false;

    startStream();

    //emit sendCheckBoxReady(false);

}

void WatchWindow::stopStream()
{
    OiFeatureState::getActiveStation()->emitStopReadingStream();

    listenerThread.quit();

    if(!listenerThread.wait()){

    }
}

void WatchWindow::startStream()
{
    OiFeatureState::getActiveStation()->emitStartReadingStream(activeReadingType);

    listenerThread.start();
}

bool WatchWindow::checkFeatureValid()
{
    if(OiFeatureState::getActiveFeature() != NULL){
        if(OiFeatureState::getActiveFeature()->getGeometry() != NULL){
            return true;
        }else{
            return false;
        }
    }
    return false;
}
