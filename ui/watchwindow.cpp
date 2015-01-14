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

    int numberOfDigits = ui->spinBox_decimalDigits->text().toInt();

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
                QString lName = "<p align=\"left\">d3D</p>";
                QString lValue = "<p align=\"right\">" + QString::number(distR3*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
                QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

                streamData.value("d3D")->setText(table);
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
        QString lName = "<p align=\"left\">distance</p>";
        QString lValue = "<p align=\"right\">" + QString::number(dDist*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
        QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

        streamData.value("distance")->setText(table);

        double alpha = featureValues.getAt(0)-trackerValues.getAt(0);

        double dAcross =  alpha * trackerValues.getAt(2);

        if(qFabs(dAcross) >= qFabs(tolerance)){
            streamData.value("across")->setPalette(Qt::red);
        }else{
            streamData.value("across")->setPalette(Qt::green);
        }
        lName = "<p align=\"left\">across</p>";
        lValue = "<p align=\"right\">" + QString::number(dAcross*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
        table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

        streamData.value("across")->setText(table);

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
        lName = "<p align=\"left\">dH</p>";
        lValue = "<p align=\"right\">" + QString::number(dH*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
        table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table";

        streamData.value("dH")->setText(table);

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
                            QString lName = "<p align=\"left\">x</p>";
                            QString lValue = "<p align=\"right\">" + QString::number(dX*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
                            QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

                            streamData.value("x")->setText(table);

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
                            QString lName = "<p align=\"left\">y</p>";
                            QString lValue = "<p align=\"right\">" + QString::number(dY*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
                            QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

                            streamData.value("y")->setText(table);

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
                            QString lName = "<p align=\"left\">z</p>";
                            QString lValue = "<p align=\"right\">" + QString::number(dZ*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
                            QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

                            streamData.value("z")->setText(table);

                        }else{
                            double tolerance = this->attributeTolerance.value(j.key())->text().toDouble();

                            if(qFabs(j.value().toDouble()) >= qFabs(tolerance)){
                                streamData.value(j.key())->setPalette(Qt::red);
                            }else{
                                streamData.value(j.key())->setPalette(Qt::green);
                            }
                            QString lName = "<p align=\"left\">" + j.key() + "</p>";
                            QString lValue = "<p align=\"right\">" + QString::number(j.value().toDouble(),'f',numberOfDigits) + "</p>";
                            QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

                            streamData.value(j.key())->setText(table);
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
                                QString lName = "<p align=\"left\">azimuth</p>";
                                QString lValue = "<p align=\"right\">" + QString::number(dAZ*UnitConverter::getAngleMultiplier(),'f',numberOfDigits) + "</p>";
                                QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

                                streamData.value("azimuth")->setText(table);

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
                                QString lName = "<p align=\"left\">zenith</p>";
                                QString lValue = "<p align=\"right\">" + QString::number(dZE*UnitConverter::getAngleMultiplier(),'f',numberOfDigits) + "</p>";
                                QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

                                streamData.value("zenith")->setText(table);

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
                                QString lName = "<p align=\"left\">distance</p>";
                                QString lValue = "<p align=\"right\">" + QString::number(dDIS*UnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
                                QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

                                streamData.value("distance")->setText(table);

                            }else{
                                double tolerance = this->attributeTolerance.value(j.key())->text().toDouble();

                                if(qFabs(j.value().toDouble()) >= qFabs(tolerance)){
                                    streamData.value(j.key())->setPalette(Qt::red);
                                }else{
                                    streamData.value(j.key())->setPalette(Qt::green);
                                }
                                QString lName = "<p align=\"left\">" + j.key() + "</p>";
                                QString lValue = "<p align=\"right\">" + QString::number(j.value().toDouble(),'f',numberOfDigits) + "</p>";
                                QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

                                streamData.value(j.key())->setText(table);
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

    //QFont f( "Arial", 40, QFont::Bold);
    QFont f;
    f.setFamily("Arial");

    //represents the number (index) of displayed labels in the master layout
    int labelCount = 0;

    //display feature name
    if(OiFeatureState::getActiveFeature() != NULL){
        QLabel *featureName;
        featureName = new QLabel();
        featureName->setFont(f);

        if(OiFeatureState::getActiveFeature()->getGeometry() != NULL){

            QString name ="<p align=\"center\">" + OiFeatureState::getActiveFeature()->getFeature()->getFeatureName() + "</p>";
            QString coordsys = "<p align=\"center\"(>" + OiFeatureState::getActiveCoordinateSystem()->getFeatureName() + "<)/p>";
            QString obs = "<p align=\"center\">obs: " + QString::number(OiFeatureState::getActiveFeature()->getGeometry()->getObservations().size()) + "</p>";
            QString featName = "";

            featName ="<table width=\"100%\"> <tr> <td>" + name + "</td> <td>" + coordsys + "</td> <td>" + obs + "</td> </tr> </table>";
            featureName->setText(featName);

        }else{
            QString name ="<p align=\"center\">" + OiFeatureState::getActiveFeature()->getFeature()->getFeatureName() + "</p>";
            QString coordsys = "<p align=\"center\">" + OiFeatureState::getActiveCoordinateSystem()->getFeatureName() + "</p>";
            QString featName = "";

            featName ="<table width=\"100%\"> <tr> <td>" + name + "</td> <td>" + coordsys + "</td> </tr> </table>";
            featureName->setText(featName);
        }

        featureName->setAlignment(Qt::AlignVCenter);
        featureName->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
        featureName->setScaledContents(true);

        QHBoxLayout *layout;
        layout = new QHBoxLayout();
        layout->addWidget(featureName);
        layout->setStretch(0,1);

        masterLayout->addLayout(layout);
        masterLayout->setStretch(labelCount,1);

        labelCount++;

        widgets.append(featureName);
        layouts.append(layout);
    }

    QString value = "XX.XX";

    //adding labels and LCDs for cross and distance watch window
    if(activeReadingType == Configuration::ePolar && ui->comboBox_polarMode->currentText().compare("cross and distance") == 0){
        //special gui for cross and distance view at actice polar reading

        //cross distance
        QString name = "across";

        QLabel *lv;
        lv = new QLabel();
        QString lName = "<p align=\"left\">" + name + "</p>";
        QString lValue = "<p align=\"right\">" + value + "</p>";
        QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";
        lv->setText(table);
        lv->setFont(f);
        lv->setAutoFillBackground(true);
        lv->setAlignment(Qt::AlignVCenter);
        lv->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
        lv->setScaledContents(true);
        widgets.append(lv);

        QHBoxLayout *layout;
        layout = new QHBoxLayout();
        layout->addWidget(lv);
        layout->setStretch(0,1);
        layouts.append(layout);

        masterLayout->addLayout(layout);
        masterLayout->setStretch(labelCount,3);
        labelCount++;

        streamData.insert(name,lv);

        //distance
        name = "distance";

        QLabel *lv2;
        lv2 = new QLabel();
        lName = "<p align=\"left\">" + name + "</p>";
        lValue = "<p align=\"right\">" + value + "</p>";
        table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";
        lv2->setText(table);
        lv2->setFont(f);
        lv2->setAutoFillBackground(true);
        lv2->setAlignment(Qt::AlignVCenter);
        lv2->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
        lv2->setScaledContents(true);
        widgets.append(lv2);

        QHBoxLayout *layout2 = new QHBoxLayout();
        layout2->addWidget(lv2);
        layout2->setStretch(0,1);
        layouts.append(layout2);

        masterLayout->addLayout(layout2);
        masterLayout->setStretch(labelCount,3);
        labelCount++;

        streamData.insert(name,lv2);

        //delta height
        name = "dH";

        QLabel *lv3;
        lv3 = new QLabel();
        lName = "<p align=\"left\">" + name + "</p>";
        lValue = "<p align=\"right\">" + value + "</p>";
        table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";
        lv->setText(table);
        lv3->setFont(f);
        lv3->setAutoFillBackground(true);
        lv3->setAlignment(Qt::AlignVCenter);
        lv3->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
        lv3->setScaledContents(true);
        widgets.append(lv3);

        QHBoxLayout *layout3 = new QHBoxLayout();
        layout3->addWidget(lv3);
        layout3->setStretch(0,1);
        layouts.append(layout3);

        masterLayout->addLayout(layout3);
        masterLayout->setStretch(labelCount,3);
        labelCount++;

        streamData.insert(name,lv3);

        //dynamical for all other watch window modes
    }else{
        for(int i=0; i<this->checkboxes.size();i++){

            if(this->checkboxes.at(i)->isChecked()){
                QString name = this->checkboxes.at(i)->text();

                QLabel *lv;
                lv = new QLabel();
                QString lName = "<p align=\"left\">" + this->checkboxes.at(i)->text() + "</p>";
                QString lValue = "<p align=\"right\">" + value + "</p>";
                QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";
                lv->setText(table);
                lv->setFont(f);
                lv->setAutoFillBackground(true);
                lv->setAlignment(Qt::AlignVCenter);
                lv->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
                lv->setScaledContents(true);
                widgets.append(lv);

                QHBoxLayout *layout = new QHBoxLayout();
                layout->addWidget(lv);
                layout->setStretch(0,1);
                layouts.append(layout);

                masterLayout->addLayout(layout);
                masterLayout->setStretch((labelCount),3);
                labelCount++;

                streamData.insert(name,lv);
            }
        }
    }

    ui->pageWatchWindow->setLayout(masterLayout);

    for(int i=0; i<widgets.size();i++){

        //first show the label, so the correct size gets displayed and returned by function
        this->widgets.at(i)->show();

        //then get font of label
        QFont f = this->widgets.at(i)->font();
        //get label height
        double h = this->widgets.at(i)->height();
        double w = this->widgets.at(i)->width();
        QFontMetrics fm(f);
        //get scale factor of font height and label height
        double scale = h/fm.height();
        //set new font size to label
        f.setPointSizeF(f.pointSizeF()*scale);
        this->widgets.at(i)->setFont(f);
    }

    //emit sendGUIReady(true);
    listener->isGUIReady = true;
}

void WatchWindow::keyPressEvent(QKeyEvent *e){

    //emit measure command
    if(e->key() == Qt::Key_F3){
        emit startMeasure();
    }

    //emit self defined action
    emit this->doSelfDefinedAction(e->text());
    qDebug() << e->text();

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
    ui->label_polarMode->setVisible(false);

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
                lab->setText(QString("tolerance d3D %1").arg(UnitConverter::getDistanceUnitString()));
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
        ui->label_polarMode->setVisible(true);
    }else{
        ui->comboBox_polarMode->setVisible(false);
        ui->label_polarMode->setVisible(false);
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
