#include "watchwindow.h"
#include "ui_watchwindow.h"

WatchWindow::WatchWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WatchWindow)
{
    ui->setupUi(this);

    isGUIReady = false;
    digitCount = 4;

    masterLayout = new QVBoxLayout();
    settingsLayout = new QVBoxLayout();

}

WatchWindow::~WatchWindow()
{

    delete ui;
}


void WatchWindow::setLCDNumber(QVariantMap m){

    this->iniGUI(m);

    QMapIterator<QString,QVariant> j(m);
    while (j.hasNext()) {
        j.next();

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
                streamData.value(name)->display(QString::number(dvalue*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));
            }
        }else{
            QLCDNumber *lcdn = streamData.value(name);
            if(lcdn != NULL){
                lcdn->display(QString::number(dvalue*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));
            }
        }
    }
}

void WatchWindow::iniGUI(QVariantMap m)
{
    streamData.clear();

    getAttributes(m);

    if(masterLayout == NULL){
        masterLayout = new QVBoxLayout();
    }

    if(OiFeatureState::getActiveFeature() != NULL){
        QLabel *featureName = new QLabel();
        QFont f( "Arial", 30, QFont::Bold);
        featureName->setFont(f);
        featureName->setText(OiFeatureState::getActiveFeature()->getFeature()->getFeatureName());
        masterLayout->addWidget(featureName);
    }


    QMapIterator<QString,QVariant> j(m);
    while (j.hasNext()) {
        j.next();

        QString name = j.key();
        QVariant qvalue = j.value();
        QString value =  qvalue.toString();

        QFont f( "Arial", 60, QFont::Bold);

        QLabel *l = new QLabel();
        l->setText(name);
        l->setFont(f);

        QLCDNumber *n = new QLCDNumber();
        n->display(value);
        n->setFont(f);

        n->setMode(QLCDNumber::Dec);
        n->setDigitCount(10);
        n->setSmallDecimalPoint(true);

        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(l);
        layout->addWidget(n);
        layout->setStretch(0,1);
        layout->setStretch(1,4);

        masterLayout->addLayout(layout);

        streamData.insert(name,n);

    }

    ui->pageWatchWindow->setLayout(masterLayout);

    isGUIReady = true;

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
    isGUIReady = false;
    streamData.clear();

    disconnect(OiFeatureState::getActiveStation()->sensorPad->instrumentListener,SIGNAL(sendReadingMap(QVariantMap)),this,SLOT(setLCDNumber(QVariantMap)));

    delete masterLayout;
    masterLayout = NULL;

    delete settingsLayout;
    settingsLayout = NULL;

    delete this;

    e->accept();

}

void WatchWindow::showEvent(QShowEvent *event)
{
    if(OiFeatureState::getActiveStation() != NULL && OiFeatureState::getActiveStation()->sensorPad->instrument != NULL){
        connect(OiFeatureState::getActiveStation()->sensorPad->instrumentListener,SIGNAL(sendReadingMap(QVariantMap)),this,SLOT(setLCDNumber(QVariantMap)));

        initSuppReadings();

        event->accept();
    }
}

/*!
 * \brief WatchWindow::initSuppReadings adds the supported reading types of the sensor in the combobox
 */
void WatchWindow::initSuppReadings()
{
    isSettingsReady = false;

    ui->comboBox_suppReadings->clear();

    QList<Configuration::ReadingTypes> *rTypes = OiFeatureState::getActiveStation()->sensorPad->instrument->getSupportedReadingTypes();

    if(rTypes == NULL){
        return;
    }

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
    if(ui->comboBox_suppReadings->findText(Configuration::sCartesian) != -1){
        ui->comboBox_suppReadings->setCurrentText(Configuration::sCartesian);
    }
    isSettingsReady = true;

    on_comboBox_suppReadings_currentIndexChanged(ui->comboBox_suppReadings->currentText());

}

void WatchWindow::getAttributes(QVariantMap m)
{
    if(isCheckboxReady){

    }else{
        if(settingsLayout == NULL){
            settingsLayout = new QVBoxLayout();
        }

        foreach (QCheckBox *cb, checkboxes) {
            delete cb;
        }
        checkboxes.clear();

        QMapIterator<QString, QVariant> j(m);
        while(j.hasNext()){
            j.next();

            QCheckBox *cb = new QCheckBox();
            cb->setText(j.key());
            cb->setChecked(true);

            settingsLayout->addWidget(cb);

            this->checkboxes.append(cb);
        }
        ui->groupBox_displayValues->setLayout(settingsLayout);

        isCheckboxReady = true;
    }
}


/*!
 * \brief WatchWindow::on_comboBox_suppReadings_currentIndexChanged
 * \param arg1
 */
void WatchWindow::on_comboBox_suppReadings_currentIndexChanged(const QString &arg1)
{
    isCheckboxReady = false;
    if(!isSettingsReady){
        return;
    }

    OiFeatureState::getActiveStation()->emitStopReadingStream();

    int r;

    if(arg1.compare(Configuration::sCartesian) == 0){
        r = Configuration::eCartesian;
    }else if(arg1.compare(Configuration::sPolar) == 0){
        r = Configuration::ePolar;
    }else if(arg1.compare(Configuration::sDistance) == 0){
        r = Configuration::eDistance;
    }else if(arg1.compare(Configuration::sDirection) == 0){
        r = Configuration::eDirection;
    }else if(arg1.compare(Configuration::sLevel) == 0){
        r = Configuration::eLevel;
    }else if(arg1.compare(Configuration::sTemperatur) == 0){
        r = Configuration::eTemperatur;
    }else if(arg1.compare("undefined") == 0){
        r = Configuration::eUndefined;
    }

    isGUIReady = false;

    OiFeatureState::getActiveStation()->emitStartReadingStream(r);
}
