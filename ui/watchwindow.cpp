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

    ui->lineEdit_decimalDigits->setText("3");
    ui->lineEdit_fNameFontSize->setText("30");
    ui->lineEdit_fAttributeFontSize->setText("60");

}

WatchWindow::~WatchWindow()
{
    delete ui;
}


void WatchWindow::setLCDNumber(QVariantMap m){

    //change display, depending on checked setting checkboxes.
    iniGUI();

    int numberOfDigits = ui->lineEdit_decimalDigits->text().toInt();

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

                        double tolerance = this->attributeTolerance.value("x")->text().toDouble()/UnitConverter::getDistanceMultiplier();

                        if(qFabs(dX) >= qFabs(tolerance)){
                            streamData.value("x")->setPalette(Qt::red);
                        }else{
                            streamData.value("x")->setPalette(Qt::green);
                        }

                        streamData.value("x")->display(QString::number(dX*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits));

                    }else if(j.key() == "y"){
                        double featureY = 0.0;

                        if(this->checkFeatureValid()){
                            featureY = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ().getAt(1);
                        }
                        double dY = featureY - trackerXYZ.getAt(1);

                        double tolerance = this->attributeTolerance.value("y")->text().toDouble()/UnitConverter::getDistanceMultiplier();

                        if(qFabs(dY) >= qFabs(tolerance)){
                            streamData.value("y")->setPalette(Qt::red);
                        }else{
                            streamData.value("y")->setPalette(Qt::green);
                        }

                        streamData.value("y")->display(QString::number(dY*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits));

                    }else if(j.key() == "z"){
                        double featureZ = 0.0;

                        if(this->checkFeatureValid()){
                            featureZ = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ().getAt(1);
                        }
                        double dZ = featureZ - trackerXYZ.getAt(2);

                        double tolerance = this->attributeTolerance.value("z")->text().toDouble()/UnitConverter::getDistanceMultiplier();

                        if(qFabs(dZ) >= qFabs(tolerance)){
                            streamData.value("z")->setPalette(Qt::red);
                        }else{
                            streamData.value("z")->setPalette(Qt::green);
                        }

                        streamData.value("z")->display(QString::number(dZ*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits));

                    }else{
                        double tolerance = this->attributeTolerance.value(j.key())->text().toDouble();

                        if(qFabs(j.value().toDouble()) >= qFabs(tolerance)){
                            streamData.value(j.key())->setPalette(Qt::red);
                        }else{
                            streamData.value(j.key())->setPalette(Qt::green);
                        }
                        streamData.value(j.key())->display(QString::number(j.value().toDouble(),'f',numberOfDigits));
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

                        double tolerance = this->attributeTolerance.value("azimuth")->text().toDouble()/UnitConverter::getAngleMultiplier();

                        if(qFabs(dAZ) >= qFabs(tolerance)){
                            streamData.value("azimuth")->setPalette(Qt::red);
                        }else{
                            streamData.value("azimuth")->setPalette(Qt::green);
                        }

                        streamData.value("azimuth")->display(QString::number(dAZ*UnitConverter::getAngleMultiplier(),'f',numberOfDigits));

                    }else if(j.key() == "zenith"){
                        double FeatureZE = 0.0;

                        if(this->checkFeatureValid()){
                            OiVec xyz = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ();
                            OiVec featurePolar = Reading::toPolar(xyz.getAt(0),xyz.getAt(1),xyz.getAt(2));
                            FeatureZE = featurePolar.getAt(1);
                        }
                        double dZE = FeatureZE - trackerValues.getAt(1);

                        double tolerance = this->attributeTolerance.value("zenith")->text().toDouble()/UnitConverter::getAngleMultiplier();

                        if(qFabs(dZE) >= qFabs(tolerance)){
                            streamData.value("zenith")->setPalette(Qt::red);
                        }else{
                            streamData.value("zenith")->setPalette(Qt::green);
                        }

                        streamData.value("zenith")->display(QString::number(dZE*UnitConverter::getAngleMultiplier(),'f',numberOfDigits));

                    }else if(j.key() == "distance"){
                        double FeatureDIS = 0.0;

                        if(this->checkFeatureValid()){
                            OiVec xyz = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ();
                            OiVec featurePolar = Reading::toPolar(xyz.getAt(0),xyz.getAt(1),xyz.getAt(2));
                            FeatureDIS = featurePolar.getAt(2);
                        }
                        double dDIS = FeatureDIS - trackerValues.getAt(2);

                        double tolerance = this->attributeTolerance.value("distance")->text().toDouble()/UnitConverter::getDistanceMultiplier();

                        if(qFabs(dDIS) >= qFabs(tolerance)){
                            streamData.value("distance")->setPalette(Qt::red);
                        }else{
                            streamData.value("distance")->setPalette(Qt::green);
                        }

                        streamData.value("distance")->display(QString::number(dDIS*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits));

                    }else{
                        double tolerance = this->attributeTolerance.value(j.key())->text().toDouble();

                        if(qFabs(j.value().toDouble()) >= qFabs(tolerance)){
                            streamData.value(j.key())->setPalette(Qt::red);
                        }else{
                            streamData.value(j.key())->setPalette(Qt::green);
                        }

                        streamData.value(j.key())->display(QString::number(j.value().toDouble(),'f',numberOfDigits));
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
    int nameSize = ui->lineEdit_fNameFontSize->text().toInt();
    int attributeSize = ui->lineEdit_fAttributeFontSize->text().toInt();

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
        QFont f( "Arial", nameSize, QFont::Bold);
        featureName->setFont(f);
        featureName->setText(OiFeatureState::getActiveFeature()->getFeature()->getFeatureName());
        masterLayout->addWidget(featureName);
        widgets.append(featureName);
    }

    for(int i=0; i<this->checkboxes.size();i++){

        if(this->checkboxes.at(i)->isChecked()){
            QString name = this->checkboxes.at(i)->text();
            QString value = "0.0";
            QFont f( "Arial", attributeSize, QFont::Bold);

            QLabel *l = new QLabel();
            l->setText(name);
            l->setFont(f);
            widgets.append(l);

            QLCDNumber *n = new QLCDNumber();
            n->display(value);
            n->setFont(f);
            n->setAutoFillBackground(true);
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

            foreach (QLabel *l, attributeLabels) {
                ui->groupBox_displayValues->layout()->removeWidget(l);
                delete l;
            }
            attributeLabels.clear();

            foreach (QLineEdit *le, attributeTolerance.values()) {
                ui->groupBox_displayValues->layout()->removeWidget(le);
                delete le;
            }
            attributeTolerance.clear();

            foreach (QLayout *l, attributeLayout) {
                ui->groupBox_displayValues->layout()->removeItem(l);
                delete l;
            }
            attributeLayout.clear();

            if(l.size() > 0){

                for(int i=0; i<l.size();i++){

                    QHBoxLayout *layout = new QHBoxLayout();
                    attributeLayout.append(layout);

                    QCheckBox *cb = new QCheckBox();
                    cb->setText(l.at(i));
                    cb->setChecked(true);
                    this->checkboxes.append(cb);

                    QLabel *lab = new QLabel();
                    lab->setText(QString("tolerance " + l.at(i) + " " + this->getUnitString(l.at(i))));
                    attributeLabels.append(lab);

                    QLineEdit *le = new QLineEdit();
                    le->setText("0.0");
                    attributeTolerance.insert(l.at(i),le);

                    layout->addWidget(cb);
                    layout->addWidget(lab);
                    layout->addWidget(le);

                    //set streching equal for all 3 elements
                    layout->setStretch(0,1);
                    layout->setStretch(1,1);
                    layout->setStretch(2,1);

                    settingsLayout->addLayout(layout);
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

QString WatchWindow::getUnitString(QString attribute)
{
    if(attribute == "x"){
        return UnitConverter::getDistanceUnitString();
    }else if(attribute == "y"){
        return UnitConverter::getDistanceUnitString();
    }else if(attribute == "z"){
        return UnitConverter::getDistanceUnitString();
    }else if(attribute == "azimuth"){
        return UnitConverter::getAngleUnitString();
    }else if(attribute == "zenith"){
        return UnitConverter::getAngleUnitString();
    }else if(attribute == "distance"){
        return UnitConverter::getDistanceUnitString();
    }else if(attribute == "temperature"){
        return UnitConverter::getTemperatureUnitString();
    }
    return "";
}
