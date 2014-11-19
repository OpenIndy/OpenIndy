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

/*!
 * \brief setLCDNumber displays the differences between actual and nominal. Only displays values for geometries.
 * \param m
 * m includes the display attributes and values.
 */
void WatchWindow::setLCDNumber(QVariantMap m){

    //change display, depending on checked setting checkboxes.
    iniGUI();

    //only stream differences for geometries.
    //return if no feature selected or station / coord system
    if(!this->checkFeatureValid()){
        return;
    }

    int numberOfDigits = ui->lineEdit_decimalDigits->text().toInt();

    OiMat trafo;

    if(!OiFeatureState::getActiveStation()->coordSys->getIsActiveCoordinateSystem()){

        trafo = FeatureUpdater::trafoControl.getTransformationMatrix(OiFeatureState::getActiveStation()->coordSys);
    }

    if(activeReadingType == Configuration::eCartesian){
        for(int i=0; i< this->checkboxes.size();i++){
            if(checkboxes.at(i)->text() == "d3D" && checkboxes.at(i)->isChecked()){

                OiVec trackerXYZ(4);

                trackerXYZ.setAt(0,m.value("x").toDouble());
                trackerXYZ.setAt(1,m.value("y").toDouble());
                trackerXYZ.setAt(2,m.value("z").toDouble());
                trackerXYZ.setAt(3,1.0);

                if(trafo.getRowCount() == 4 && trafo.getColCount() == 4){
                    trackerXYZ = trafo*trackerXYZ;
                }

                OiVec featureXYZ(4);
                if(this->checkFeatureValid()){
                    featureXYZ = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ();
                }
                OiVec d = featureXYZ - trackerXYZ;

                double distR3 = qSqrt(d.getAt(0)*d.getAt(0)+d.getAt(1)*d.getAt(1)+d.getAt(2)*d.getAt(2));

                double tolerance = this->attributeTolerance.value("d3D")->text().toDouble()/UnitConverter::getDistanceMultiplier();

                if(qFabs(distR3) >= qFabs(tolerance)){
                    streamData.value("d3D")->setPalette(Qt::red);
                }else{
                    streamData.value("d3D")->setPalette(Qt::green);
                }

                streamData.value("d3D")->display(QString::number(distR3*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits));
            }
        }
    }

    if(activeReadingType == Configuration::ePolar && ui->comboBox_polarMode->currentText().compare("cross and distance") == 0){

        OiVec trackerValues(4);
        trackerValues.setAt(0,m.value("azimuth").toDouble());
        trackerValues.setAt(1,m.value("zenith").toDouble());
        trackerValues.setAt(2,m.value("distance").toDouble());
        trackerValues.setAt(3,1.0);

        OiVec featureValues(4);
        if(this->checkFeatureValid()){
            featureValues = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ();
        }
        featureValues.setAt(3,1.0);

        if(trafo.getRowCount() == 4 && trafo.getColCount() == 4){
            featureValues = trafo.inv()*featureValues;
        }

        featureValues = Reading::toPolar(featureValues.getAt(0),featureValues.getAt(1),featureValues.getAt(2));

        //check for negative values
        if(featureValues.getAt(0) <= 0.0){
            featureValues.setAt(0, 2*PI + featureValues.getAt(0));
        }

        double dDist = featureValues.getAt(2) - trackerValues.getAt(2);

        double tolerance = this->attributeTolerance.value("distance")->text().toDouble()/UnitConverter::getDistanceMultiplier();

        if(qFabs(dDist) >= qFabs(tolerance)){
            streamData.value("distance")->setPalette(Qt::red);
        }else{
            streamData.value("distance")->setPalette(Qt::green);
        }

        streamData.value("distance")->display(QString::number(dDist*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits));

        double alpha = featureValues.getAt(0)-trackerValues.getAt(0);

        double dAcross =  alpha * trackerValues.getAt(2);

        if(qFabs(dAcross) >= qFabs(tolerance)){
            streamData.value("across")->setPalette(Qt::red);
        }else{
            streamData.value("across")->setPalette(Qt::green);
        }

        streamData.value("across")->display(QString::number(dAcross*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits));

        //nominal
        double h1 = qSin(((PI/2) - featureValues.getAt(1)))*featureValues.getAt(2);
        //actual
        double h2 = qSin(((PI/2) - trackerValues.getAt(1)))*trackerValues.getAt(2);

        double dH = h1 - h2;

        if(qFabs(dH) >= qFabs(tolerance)){
            streamData.value("dH")->setPalette(Qt::red);
        }else{
            streamData.value("dH")->setPalette(Qt::green);
        }

        streamData.value("dH")->display(QString::number(dH*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits));

    }else{

        QMapIterator<QString,QVariant> j(m);
        while (j.hasNext()) {
            j.next();

            for(int i=0;i<this->checkboxes.size();i++){



                if(this->checkboxes.at(i)->text() == j.key() && this->checkboxes.at(i)->isChecked()){

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
                                featureZ = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ().getAt(2);
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

                        if(ui->comboBox_polarMode->currentText().compare("normale mode") == 0){
                            //if normal mode show the differences in angles and distance

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
}

/*!
 * \brief iniGUI initiates the gui and adds all needed labels and LCD labels for displaying the differences.
 */
void WatchWindow::iniGUI()
{
    int nameSize = ui->lineEdit_fNameFontSize->text().toInt();
    int attributeSize = ui->lineEdit_fAttributeFontSize->text().toInt();

    streamData.clear();

    //delete old gui elements

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

    //display feature name
    if(OiFeatureState::getActiveFeature() != NULL){
        QLabel *featureName = new QLabel();
        QFont f( "Arial", nameSize, QFont::Bold);
        featureName->setFont(f);
        featureName->setText(OiFeatureState::getActiveFeature()->getFeature()->getFeatureName());
        masterLayout->addWidget(featureName);
        widgets.append(featureName);
    }

    QFont fAttr( "Arial", attributeSize, QFont::Bold);

    QString value = "XX.XX";

    //adding labels and LCDs for cross and distance watch window
    if(activeReadingType == Configuration::ePolar && ui->comboBox_polarMode->currentText().compare("cross and distance") == 0){
        //special gui for cross and distance view at actice polar reading

        //cross distance
        QString name = "across";


        QLabel *l = new QLabel();
        l->setText(name);
        l->setFont(fAttr);
        widgets.append(l);

        QLCDNumber *n = new QLCDNumber();
        n->display(value);
        n->setFont(fAttr);
        n->setAutoFillBackground(true);
        widgets.append(n);

        n->setMode(QLCDNumber::Dec);
        n->setDigitCount(10);
        n->setSmallDecimalPoint(true);

        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(l);
        layout->addWidget(n);
        layout->setStretch(0,2);
        layout->setStretch(1,3);
        layouts.append(layout);

        masterLayout->addLayout(layout);

        streamData.insert(name,n);

        //distance
        name = "distance";

        QLabel *l2 = new QLabel();
        l2->setText(name);
        l2->setFont(fAttr);
        widgets.append(l2);

        QLCDNumber *n2 = new QLCDNumber();
        n2->display(value);
        n2->setFont(fAttr);
        n2->setAutoFillBackground(true);
        widgets.append(n2);

        n2->setMode(QLCDNumber::Dec);
        n2->setDigitCount(10);
        n2->setSmallDecimalPoint(true);

        QHBoxLayout *layout2 = new QHBoxLayout();
        layout2->addWidget(l2);
        layout2->addWidget(n2);
        layout2->setStretch(0,2);
        layout2->setStretch(1,3);
        layouts.append(layout2);

        masterLayout->addLayout(layout2);

        streamData.insert(name,n2);

        //delta height
        name = "dH";

        QLabel *l3 = new QLabel();
        l3->setText(name);
        l3->setFont(fAttr);
        widgets.append(l3);

        QLCDNumber *n3 = new QLCDNumber();
        n3->display(value);
        n3->setFont(fAttr);
        n3->setAutoFillBackground(true);
        widgets.append(n3);

        n3->setMode(QLCDNumber::Dec);
        n3->setDigitCount(10);
        n3->setSmallDecimalPoint(true);

        QHBoxLayout *layout3 = new QHBoxLayout();
        layout3->addWidget(l3);
        layout3->addWidget(n3);
        layout3->setStretch(0,2);
        layout3->setStretch(1,3);
        layouts.append(layout3);

        masterLayout->addLayout(layout3);

        streamData.insert(name,n3);

        //dynamical for all other watch window modes
    }else{
        for(int i=0; i<this->checkboxes.size();i++){

            if(this->checkboxes.at(i)->isChecked()){
                QString name = this->checkboxes.at(i)->text();
                //QString value = "0.0";

                QLabel *l = new QLabel();
                l->setText(name);
                l->setFont(fAttr);
                widgets.append(l);

                QLCDNumber *n = new QLCDNumber();
                n->display(value);
                n->setFont(fAttr);
                n->setAutoFillBackground(true);
                widgets.append(n);

                n->setMode(QLCDNumber::Dec);
                n->setDigitCount(10);
                n->setSmallDecimalPoint(true);

                QHBoxLayout *layout = new QHBoxLayout();
                layout->addWidget(l);
                layout->addWidget(n);
                layout->setStretch(0,2);
                layout->setStretch(1,3);
                layouts.append(layout);

                masterLayout->addLayout(layout);

                streamData.insert(name,n);
            }
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

    //delete this;

    e->accept();

}

void WatchWindow::showEvent(QShowEvent *event)
{
    listenerThread.start();

    ui->comboBox_polarMode->clear();
    ui->comboBox_polarMode->addItem("normale mode");
    ui->comboBox_polarMode->addItem("cross and distance");
    ui->comboBox_polarMode->setVisible(false);

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

    bool containPolar = false;

    //only add direction and distance if there is no polar available
    if(rTypes->contains(Configuration::ePolar)){
        containPolar = true;
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
            if(!containPolar){
                ui->comboBox_suppReadings->addItem(Configuration::sDistance);
            }
            break;
        case Configuration::eDirection:
            if(!containPolar){
                ui->comboBox_suppReadings->addItem(Configuration::sDirection);
            }
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
                le->setText("1.0");
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

            if(activeReadingType == Configuration::eCartesian){

                QHBoxLayout *layout = new QHBoxLayout();
                attributeLayout.append(layout);

                QCheckBox *cb = new QCheckBox();
                cb->setText("d3D");
                cb->setChecked(true);
                this->checkboxes.append(cb);

                QLabel *lab = new QLabel();
                lab->setText(QString("tolerance d3D"));
                attributeLabels.append(lab);

                QLineEdit *le = new QLineEdit();
                le->setText("1.0");
                attributeTolerance.insert("d3D",le);

                layout->addWidget(cb);
                layout->addWidget(lab);
                layout->addWidget(le);

                //set streching equal for all 3 elements
                layout->setStretch(0,1);
                layout->setStretch(1,1);
                layout->setStretch(2,1);

                settingsLayout->addLayout(layout);
            }
        }
    }

    //emit sendCheckBoxReady(true);
    listener->isCheckBoxReady = true;

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

    if(arg1.compare(Configuration::sPolar) == 0){
        ui->comboBox_polarMode->setVisible(true);
    }else{
        ui->comboBox_polarMode->setVisible(false);
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
