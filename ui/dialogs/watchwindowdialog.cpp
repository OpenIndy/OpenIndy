#include "watchwindowdialog.h"

#include "ui_watchwindowdialog.h"

/*
WatchWindow::WatchWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WatchWindow), isVisible(false)
{
    ui->setupUi(this);

    //isGUIReady = false;
    digitCount = 4;

    listener = new WatchWindowListener();

    connect(this,SIGNAL(sendCheckBoxReady(bool)),this->listener,SLOT(setCheckBoxReady(bool)));
    connect(this,SIGNAL(sendGUIReady(bool)),this->listener,SLOT(setGUIReady(bool)));
    connect(this,SIGNAL(sendSettingsReady(bool)),this->listener,SLOT(setSettingsReady(bool)));
    connect(this->listener,SIGNAL(sendAttributeNames(QStringList)),this,SLOT(getAttributes(QStringList)));
    connect(this->listener,SIGNAL(sendData(QVariantMap)),this,SLOT(setLCDNumber(QVariantMap)));

    listener->moveToThread(&listenerThread);
    listenerThread.start();
}

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

                double tolerance = this->attributeTolerance.value("d3D")->text().toDouble()/OiUnitConverter::getDistanceMultiplier();

                if(qFabs(distR3) >= qFabs(tolerance)){
                    streamData.value("d3D")->setPalette(Qt::red);
                }else{
                    streamData.value("d3D")->setPalette(Qt::green);
                }
                QString lName = "<p align=\"left\">d3D</p>";
                QString lValue = "<p align=\"right\">" + QString::number(distR3*OiUnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
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

        double dDist;
        if(ui->radioButton_actnom->isChecked()){
            dDist = trackerValues.getAt(2) - featureValues.getAt(2);
        }else{
            dDist = featureValues.getAt(2) - trackerValues.getAt(2);
        }

        double tolerance = this->attributeTolerance.value("distance")->text().toDouble()/OiUnitConverter::getDistanceMultiplier();

        if(qFabs(dDist) >= qFabs(tolerance)){
            streamData.value("distance")->setPalette(Qt::red);
        }else{
            streamData.value("distance")->setPalette(Qt::green);
        }
        QString lName = "<p align=\"left\">distance</p>";
        QString lValue = "<p align=\"right\">" + QString::number(dDist*OiUnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
        QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

        streamData.value("distance")->setText(table);

        double alpha;
        if(ui->radioButton_actnom->isChecked()){
            alpha = trackerValues.getAt(0) - featureValues.getAt(0);
        }else{
            alpha = featureValues.getAt(0)-trackerValues.getAt(0);
        }

        double dAcross =  alpha * trackerValues.getAt(2);

        if(qFabs(dAcross) >= qFabs(tolerance)){
            streamData.value("across")->setPalette(Qt::red);
        }else{
            streamData.value("across")->setPalette(Qt::green);
        }
        lName = "<p align=\"left\">across</p>";
        lValue = "<p align=\"right\">" + QString::number(dAcross*OiUnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
        table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

        streamData.value("across")->setText(table);

        //nominal
        double h1 = qSin(((PI/2) - featureValues.getAt(1)))*featureValues.getAt(2);
        //actual
        double h2 = qSin(((PI/2) - trackerValues.getAt(1)))*trackerValues.getAt(2);

        double dH;
        if(ui->radioButton_actnom->isChecked()){
            dH = h2 - h1;
        }else{
            dH = h1 - h2;
        }

        if(qFabs(dH) >= qFabs(tolerance)){
            streamData.value("dH")->setPalette(Qt::red);
        }else{
            streamData.value("dH")->setPalette(Qt::green);
        }
        lName = "<p align=\"left\">dH</p>";
        lValue = "<p align=\"right\">" + QString::number(dH*OiUnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
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

                            double dX;
                            if(ui->radioButton_actnom->isChecked()){
                                dX = trackerXYZ.getAt(0) - featureX;
                            }else{
                                dX = featureX - trackerXYZ.getAt(0);
                            }

                            double tolerance = this->attributeTolerance.value("x")->text().toDouble()/OiUnitConverter::getDistanceMultiplier();

                            if(qFabs(dX) >= qFabs(tolerance)){
                                streamData.value("x")->setPalette(Qt::red);
                            }else{
                                streamData.value("x")->setPalette(Qt::green);
                            }
                            QString lName = "<p align=\"left\">x</p>";
                            QString lValue = "<p align=\"right\">" + QString::number(dX*OiUnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
                            QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

                            streamData.value("x")->setText(table);

                        }else if(j.key() == "y"){
                            double featureY = 0.0;

                            if(this->checkFeatureValid()){
                                featureY = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ().getAt(1);
                            }

                            double dY;
                            if(ui->radioButton_actnom->isChecked()){
                                dY = trackerXYZ.getAt(1) - featureY;
                            }else{
                                dY = featureY - trackerXYZ.getAt(1);
                            }

                            double tolerance = this->attributeTolerance.value("y")->text().toDouble()/OiUnitConverter::getDistanceMultiplier();

                            if(qFabs(dY) >= qFabs(tolerance)){
                                streamData.value("y")->setPalette(Qt::red);
                            }else{
                                streamData.value("y")->setPalette(Qt::green);
                            }
                            QString lName = "<p align=\"left\">y</p>";
                            QString lValue = "<p align=\"right\">" + QString::number(dY*OiUnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
                            QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

                            streamData.value("y")->setText(table);

                        }else if(j.key() == "z"){
                            double featureZ = 0.0;

                            if(this->checkFeatureValid()){
                                featureZ = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ().getAt(2);
                            }

                            double dZ;
                            if(ui->radioButton_actnom->isChecked()){
                                dZ = trackerXYZ.getAt(2) - featureZ;
                            }else{
                                dZ = featureZ - trackerXYZ.getAt(2);
                            }

                            double tolerance = this->attributeTolerance.value("z")->text().toDouble()/OiUnitConverter::getDistanceMultiplier();

                            if(qFabs(dZ) >= qFabs(tolerance)){
                                streamData.value("z")->setPalette(Qt::red);
                            }else{
                                streamData.value("z")->setPalette(Qt::green);
                            }
                            QString lName = "<p align=\"left\">z</p>";
                            QString lValue = "<p align=\"right\">" + QString::number(dZ*OiUnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
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

                                double dAZ;
                                if(ui->radioButton_actnom->isChecked()){
                                    dAZ = trackerValues.getAt(0) - FeatureAZ;
                                }else{
                                    dAZ = FeatureAZ - trackerValues.getAt(0);
                                }

                                double tolerance = this->attributeTolerance.value("azimuth")->text().toDouble()/OiUnitConverter::getAngleMultiplier();

                                if(qFabs(dAZ) >= qFabs(tolerance)){
                                    streamData.value("azimuth")->setPalette(Qt::red);
                                }else{
                                    streamData.value("azimuth")->setPalette(Qt::green);
                                }
                                QString lName = "<p align=\"left\">azimuth</p>";
                                QString lValue = "<p align=\"right\">" + QString::number(dAZ*OiUnitConverter::getAngleMultiplier(),'f',numberOfDigits) + "</p>";
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
                                if(ui->radioButton_actnom->isChecked()){
                                    dZE = trackerValues.getAt(1) - FeatureZE;
                                }else{
                                    dZE = FeatureZE - trackerValues.getAt(1);
                                }

                                double tolerance = this->attributeTolerance.value("zenith")->text().toDouble()/OiUnitConverter::getAngleMultiplier();

                                if(qFabs(dZE) >= qFabs(tolerance)){
                                    streamData.value("zenith")->setPalette(Qt::red);
                                }else{
                                    streamData.value("zenith")->setPalette(Qt::green);
                                }
                                QString lName = "<p align=\"left\">zenith</p>";
                                QString lValue = "<p align=\"right\">" + QString::number(dZE*OiUnitConverter::getAngleMultiplier(),'f',numberOfDigits) + "</p>";
                                QString table = "<table width=\"100%\"> <tr> <td width=\"50%\">" + lName + "</td> <td width=\"50%\">" + lValue + "</td> </tr></table>";

                                streamData.value("zenith")->setText(table);

                            }else if(j.key() == "distance"){
                                double FeatureDIS = 0.0;

                                if(this->checkFeatureValid()){
                                    OiVec xyz = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ();
                                    OiVec featurePolar = Reading::toPolar(xyz.getAt(0),xyz.getAt(1),xyz.getAt(2));
                                    FeatureDIS = featurePolar.getAt(2);
                                }

                                double dDIS;
                                if(ui->radioButton_actnom->isChecked()){
                                    dDIS = trackerValues.getAt(2) - FeatureDIS;
                                }else{
                                    dDIS = FeatureDIS - trackerValues.getAt(2);
                                }

                                double tolerance = this->attributeTolerance.value("distance")->text().toDouble()/OiUnitConverter::getDistanceMultiplier();

                                if(qFabs(dDIS) >= qFabs(tolerance)){
                                    streamData.value("distance")->setPalette(Qt::red);
                                }else{
                                    streamData.value("distance")->setPalette(Qt::green);
                                }
                                QString lName = "<p align=\"left\">distance</p>";
                                QString lValue = "<p align=\"right\">" + QString::number(dDIS*OiUnitConverter::getDistanceMultiplier(),'f',numberOfDigits) + "</p>";
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
    if(OiJob::getActiveFeature() != NULL){
        QLabel *featureName;
        featureName = new QLabel();
        featureName->setFont(f);

        if(OiJob::getActiveFeature()->getGeometry() != NULL){

            QString name ="<p align=\"center\">" + OiJob::getActiveFeature()->getFeature()->getFeatureName() + "</p>";
            QString actNom = "<p align=\"center\"(>" + OiJob::getActiveFeature()->getFeature()->getDisplayIsNominal() + "<)/p>";
            QString obs = "<p align=\"center\">obs: " + QString::number(OiJob::getActiveFeature()->getGeometry()->getObservations().size()) + "</p>";
            QString featName = "";

            featName ="<table width=\"100%\"> <tr> <td>" + name + "</td> <td>" + actNom + "</td> <td>" + obs + "</td> </tr> </table>";
            featureName->setText(featName);

        }else{
            QString name ="<p align=\"center\">" + OiJob::getActiveFeature()->getFeature()->getFeatureName() + "</p>";
            QString actNom = "<p align=\"center\">" + OiJob::getActiveFeature()->getFeature()->getDisplayIsNominal() + "</p>";
            QString featName = "";

            featName ="<table width=\"100%\"> <tr> <td>" + name + "</td> <td>" + actNom + "</td> </tr> </table>";
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


    if(e->key() == Qt::Key_F3){

        //emit measure command
        emit startMeasure();

    }else if(e->key() == Qt::Key_F4){

        emit this->addMeasurement();

    }else{ //emit key pressed event to be able to react on such event (e.g. in OiTools)
        emit this->keyPressed((Qt::Key)e->key());
    }


     * else if(e->key() == Qt::Key_F && e->modifiers() == Qt::ALT){ //Leica Tracker smartfind after aim
        emit this->doSelfDefinedAction("find target");
    }
    else if(e->text().compare("") != 0){ //emit self defined action
        emit this->doSelfDefinedAction(e->text());
    }

}

void WatchWindow::closeEvent(QCloseEvent *e)
{

    if(this->isVisible == false){
        return;
    }
    this->isVisible = false;

    OiJob::getActiveStation()->emitStopReadingStream();
    streamData.clear();

    disconnect(OiJob::getActiveStation()->sensorPad->instrumentListener,SIGNAL(sendReadingMap(QVariantMap)),this->listener,SLOT(setLCDNumber(QVariantMap)));

    delete masterLayout;
    masterLayout = NULL;

    delete settingsLayout;
    settingsLayout = NULL;

    listenerThread.quit();
    listenerThread.wait();

    //delete this;

    emit closeWatchWindow();

    e->accept();

}

void WatchWindow::showEvent(QShowEvent *event)
{
    this->isVisible = true;

    listenerThread.start();

    ui->comboBox_polarMode->clear();
    ui->comboBox_polarMode->addItem("normale mode");
    ui->comboBox_polarMode->addItem("cross and distance");
    ui->comboBox_polarMode->setVisible(false);
    ui->label_polarMode->setVisible(false);

    if(OiJob::getActiveStation() != NULL && OiJob::getActiveStation()->sensorPad->instrument != NULL){
        connect(OiJob::getActiveStation()->sensorPad->instrumentListener,SIGNAL(sendReadingMap(QVariantMap)),this->listener,SLOT(setLCDNumber(QVariantMap)));

        initSuppReadings();

        event->accept();
    }
}

void WatchWindow::initSuppReadings()
{
    ui->comboBox_suppReadings->clear();

    QList<Configuration::ReadingTypes> *rTypes = OiJob::getActiveStation()->sensorPad->instrument->getSupportedReadingTypes();

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
    /*stopStream();


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
                lab->setText(QString("tolerance d3D %1").arg(OiUnitConverter::getDistanceUnitString()));
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
    OiJob::getActiveStation()->emitStopReadingStream();

    listenerThread.quit();

    if(!listenerThread.wait()){

    }
}

void WatchWindow::startStream()
{
    OiJob::getActiveStation()->emitStartReadingStream(activeReadingType);

    listenerThread.start();
}

bool WatchWindow::checkFeatureValid()
{
    if(OiJob::getActiveFeature() != NULL){
        if(OiJob::getActiveFeature()->getGeometry() != NULL){
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
        return OiUnitConverter::getDistanceUnitString();
    }else if(attribute == "y"){
        return OiUnitConverter::getDistanceUnitString();
    }else if(attribute == "z"){
        return OiUnitConverter::getDistanceUnitString();
    }else if(attribute == "azimuth"){
        return OiUnitConverter::getAngleUnitString();
    }else if(attribute == "zenith"){
        return OiUnitConverter::getAngleUnitString();
    }else if(attribute == "distance"){
        return OiUnitConverter::getDistanceUnitString();
    }else if(attribute == "temperature"){
        return OiUnitConverter::getTemperatureUnitString();
    }
    return "";
}
*/


/*!
 * \brief WatchWindowDialog::WatchWindowDialog
 * \param parent
 */
WatchWindowDialog::WatchWindowDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::WatchWindowDialog)
{
    ui->setupUi(this);

    //initialize dynamic GUI elements
    this->initGUI();
    this->initModels();

    //set up default settings
    this->getDefaultSettings();

}

/*!
 * \brief WatchWindowDialog::~WatchWindowDialog
 */
WatchWindowDialog::~WatchWindowDialog(){
    delete this->ui;
}

/*!
 * \brief WatchWindowDialog::getCurrentJob
 * \return
 */
const QPointer<OiJob> &WatchWindowDialog::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief WatchWindowDialog::setCurrentJob
 * \param job
 */
void WatchWindowDialog::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){
        this->currentJob = job;
    }
}

void WatchWindowDialog::keyPressEvent(QKeyEvent *e)
{

}

/*!
 * \brief WatchWindowDialog::on_spinBox_decimalDigits_valueChanged
 * \param arg1
 */
void WatchWindowDialog::on_spinBox_decimalDigits_valueChanged(int arg1){
    this->settings.digits = this->ui->spinBox_decimalDigits->value();
}

/*!
 * \brief WatchWindowDialog::on_comboBox_readingTypes_currentIndexChanged
 * \param arg1
 */
void WatchWindowDialog::on_comboBox_readingTypes_currentIndexChanged(const QString &arg1){
    this->settings.readingType = getReadingTypeEnum(this->ui->comboBox_readingTypes->currentText());
    if(this->settings.readingType == ePolarReading){
        this->ui->comboBox_polarMode->setVisible(true);
        this->ui->label_polarMode->setVisible(true);
    }else{
        this->ui->comboBox_polarMode->setVisible(false);
        this->ui->label_polarMode->setVisible(false);
    }
}

/*!
 * \brief WatchWindowDialog::on_comboBox_polarMode_currentIndexChanged
 * \param arg1
 */
void WatchWindowDialog::on_comboBox_polarMode_currentIndexChanged(const QString &arg1){
    if(this->ui->comboBox_polarMode->currentText().compare("cross and distance") == 0){
        this->settings.polarType = 1;
    }else{
        this->settings.polarType = 0;
    }
}

/*!
 * \brief WatchWindowDialog::on_radioButton_actnom_clicked
 */
void WatchWindowDialog::on_radioButton_actnom_clicked(){
    this->settings.reference = this->ui->radioButton_actnom->isChecked()?0:1;
}

/*!
 * \brief WatchWindowDialog::on_radioButton_nomact_clicked
 */
void WatchWindowDialog::on_radioButton_nomact_clicked(){
    this->settings.reference = this->ui->radioButton_actnom->isChecked()?0:1;
}

/*!
 * \brief WatchWindowDialog::on_checkBox_x_clicked
 */
void WatchWindowDialog::on_checkBox_x_clicked(){
    if(this->ui->checkBox_x->isChecked()){
        this->settings.displayValues.insert("x", this->ui->lineEdit_x->text().toDouble());
    }else{
        this->settings.displayValues.remove("x");
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_y_clicked
 */
void WatchWindowDialog::on_checkBox_y_clicked(){
    if(this->ui->checkBox_y->isChecked()){
        this->settings.displayValues.insert("y", this->ui->lineEdit_y->text().toDouble());
    }else{
        this->settings.displayValues.remove("y");
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_z_clicked
 */
void WatchWindowDialog::on_checkBox_z_clicked(){
    if(this->ui->checkBox_z->isChecked()){
        this->settings.displayValues.insert("z", this->ui->lineEdit_z->text().toDouble());
    }else{
        this->settings.displayValues.remove("z");
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_d3d_clicked
 */
void WatchWindowDialog::on_checkBox_d3d_clicked(){
    if(this->ui->checkBox_d3d->isChecked()){
        this->settings.displayValues.insert("d3D", this->ui->lineEdit_d3d->text().toDouble());
    }else{
        this->settings.displayValues.remove("d3D");
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_azimuth_clicked
 */
void WatchWindowDialog::on_checkBox_azimuth_clicked(){
    if(this->ui->checkBox_azimuth->isChecked()){
        this->settings.displayValues.insert("azimuth", this->ui->lineEdit_azimuth->text().toDouble());
    }else{
        this->settings.displayValues.remove("azimuth");
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_zenith_clicked
 */
void WatchWindowDialog::on_checkBox_zenith_clicked(){
    if(this->ui->checkBox_zenith->isChecked()){
        this->settings.displayValues.insert("zenith", this->ui->lineEdit_zenith->text().toDouble());
    }else{
        this->settings.displayValues.remove("zenith");
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_distance_clicked
 */
void WatchWindowDialog::on_checkBox_distance_clicked(){
    if(this->ui->checkBox_distance->isChecked()){
        this->settings.displayValues.insert("distance", this->ui->lineEdit_distance->text().toDouble());
    }else{
        this->settings.displayValues.remove("distance");
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_x_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_x_textChanged(const QString &arg1){
    if(this->ui->checkBox_x->isChecked()){
        this->settings.displayValues.insert("x", this->ui->lineEdit_x->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_y_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_y_textChanged(const QString &arg1){
    if(this->ui->checkBox_y->isChecked()){
        this->settings.displayValues.insert("y", this->ui->lineEdit_y->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_z_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_z_textChanged(const QString &arg1){
    if(this->ui->checkBox_z->isChecked()){
        this->settings.displayValues.insert("z", this->ui->lineEdit_z->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_d3d_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_d3d_textChanged(const QString &arg1){
    if(this->ui->checkBox_d3d->isChecked()){
        this->settings.displayValues.insert("d3D", this->ui->lineEdit_d3d->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_azimuth_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_azimuth_textChanged(const QString &arg1){
    if(this->ui->checkBox_azimuth->isChecked()){
        this->settings.displayValues.insert("azimuth", this->ui->lineEdit_azimuth->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_zenith_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_zenith_textChanged(const QString &arg1){
    if(this->ui->checkBox_zenith->isChecked()){
        this->settings.displayValues.insert("zenith", this->ui->lineEdit_zenith->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_distance_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_distance_textChanged(const QString &arg1){
    if(this->ui->checkBox_distance->isChecked()){
        this->settings.displayValues.insert("distance", this->ui->lineEdit_distance->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::realTimeReading
 * \param reading
 */
void WatchWindowDialog::realTimeReading(const QVariantMap &reading){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //check active station
    if(this->activeStation.isNull() || this->activeStation->getCoordinateSystem().isNull()){
        return;
    }

    //set up watch winddow values
    if(this->settings.readingType == eCartesianReading){
        this->setUpCartesianWatchWindow(reading);
    }



}

/*!
 * \brief WatchWindowDialog::showEvent
 * \param event
 */
void WatchWindowDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    //disconnect old sensor and connect new one
    this->disconnectSensor();
    this->connectActiveSensor();

    event->accept();

}

/*!
 * \brief WatchWindowDialog::closeEvent
 * \param event
 */
void WatchWindowDialog::closeEvent(QCloseEvent *event){

    //disconnect old sensor
    this->disconnectSensor();

    event->accept();

}

/*!
 * \brief WatchWindowDialog::initGUI
 */
void WatchWindowDialog::initGUI(){

    //init layout for watch window values
    this->masterLayout = new QVBoxLayout();

    //set initial visibility
    this->ui->comboBox_polarMode->setVisible(false);
    this->ui->label_polarMode->setVisible(false);

    //set polar modes
    this->ui->comboBox_polarMode->addItem("normal mode");
    this->ui->comboBox_polarMode->addItem("cross and distance");

    //###########################
    //set up dynamic watch window
    //###########################

    //create font for watch window
    QFont f;
    f.setFamily("Arial");

    //feature name
    QLabel *featureName = new QLabel();
    featureName->setAlignment(Qt::AlignVCenter);
    featureName->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    featureName->setScaledContents(true);
    QHBoxLayout *featureNameLayout = new QHBoxLayout();
    featureNameLayout->addWidget(featureName);
    featureNameLayout->setStretch(0,1);
    this->masterLayout->addLayout(featureNameLayout);
    this->masterLayout->setStretch(0, 1);
    streamData.insert("name", featureName);
    //this->masterLayout->addWidget(featureName);
    //streamData.insert("name", featureName);

    //x
    QLabel *x = new QLabel();
    x->setFont(f);
    x->setAutoFillBackground(true);
    x->setAlignment(Qt::AlignVCenter);
    x->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    x->setScaledContents(true);
    QHBoxLayout *xLayout = new QHBoxLayout();
    xLayout->addWidget(x);
    xLayout->setStretch(0,1);
    this->masterLayout->addLayout(xLayout);
    this->masterLayout->setStretch(1, 3);
    //this->masterLayout->addWidget(x);
    streamData.insert("x", x);

    //y
    QLabel *y = new QLabel();
    y->setFont(f);
    y->setAutoFillBackground(true);
    y->setAlignment(Qt::AlignVCenter);
    y->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    y->setScaledContents(true);
    QHBoxLayout *yLayout = new QHBoxLayout();
    yLayout->addWidget(y);
    yLayout->setStretch(0,1);
    masterLayout->addLayout(yLayout);
    //this->masterLayout->addWidget(y);
    this->masterLayout->setStretch(2, 3);
    streamData.insert("y", y);

    //z
    QLabel *z = new QLabel();
    z->setFont(f);
    z->setAutoFillBackground(true);
    z->setAlignment(Qt::AlignVCenter);
    z->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    z->setScaledContents(true);
    QHBoxLayout *zLayout = new QHBoxLayout();
    zLayout->addWidget(z);
    zLayout->setStretch(0,1);
    masterLayout->addLayout(zLayout);
    //this->masterLayout->addWidget(z);
    this->masterLayout->setStretch(3, 3);
    streamData.insert("z", z);

    //d3D
    QLabel *d3D = new QLabel();
    d3D->setFont(f);
    d3D->setAutoFillBackground(true);
    d3D->setAlignment(Qt::AlignVCenter);
    d3D->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    d3D->setScaledContents(true);
    QHBoxLayout *d3DLayout = new QHBoxLayout();
    d3DLayout->addWidget(d3D);
    d3DLayout->setStretch(0,1);
    masterLayout->addLayout(d3DLayout);
    //this->masterLayout->addWidget(d3D);
    this->masterLayout->setStretch(4, 3);
    streamData.insert("d3D", d3D);
/*
    //azimuth
    QLabel *azimuth = new QLabel();
    azimuth->setFont(f);
    azimuth->setAutoFillBackground(true);
    azimuth->setAlignment(Qt::AlignVCenter);
    azimuth->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    azimuth->setScaledContents(true);
    QHBoxLayout *azimuthLayout = new QHBoxLayout();
    azimuthLayout->addWidget(azimuth);
    azimuthLayout->setStretch(0,1);
    masterLayout->addLayout(azimuthLayout);
    streamData.insert("azimuth", azimuth);

    //zenith
    QLabel *zenith = new QLabel();
    zenith->setFont(f);
    zenith->setAutoFillBackground(true);
    zenith->setAlignment(Qt::AlignVCenter);
    zenith->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    zenith->setScaledContents(true);
    QHBoxLayout *zenithLayout = new QHBoxLayout();
    zenithLayout->addWidget(zenith);
    zenithLayout->setStretch(0,1);
    masterLayout->addLayout(zenithLayout);
    streamData.insert("zenith", zenith);

    //distance
    QLabel *distance = new QLabel();
    distance->setFont(f);
    distance->setAutoFillBackground(true);
    distance->setAlignment(Qt::AlignVCenter);
    distance->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    distance->setScaledContents(true);
    QHBoxLayout *distanceLayout = new QHBoxLayout();
    distanceLayout->addWidget(distance);
    distanceLayout->setStretch(0,1);
    masterLayout->addLayout(distanceLayout);
    streamData.insert("distance", distance);
/*
    //cross
    QLabel *x = new QLabel();
    x->setFont(f);
    x->setAutoFillBackground(true);
    x->setAlignment(Qt::AlignVCenter);
    x->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    x->setScaledContents(true);
    QHBoxLayout *xLayout = new QHBoxLayout();
    xLayout->addWidget(lv);
    xLayout->setStretch(0,1);
    masterLayout->addLayout(xLayout);

    //distance
    QLabel *x = new QLabel();
    x->setFont(f);
    x->setAutoFillBackground(true);
    x->setAlignment(Qt::AlignVCenter);
    x->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    x->setScaledContents(true);
    QHBoxLayout *xLayout = new QHBoxLayout();
    xLayout->addWidget(lv);
    xLayout->setStretch(0,1);
    masterLayout->addLayout(xLayout);
*/

    //assign master layout
    this->ui->pageWatchWindow->setLayout(this->masterLayout);

}

/*!
 * \brief WatchWindowDialog::initModels
 * Is called each time the active sensor has changed to update settings
 */
void WatchWindowDialog::initModels(){

    //update supported reading types
    this->ui->comboBox_readingTypes->clear();
    if(!this->activeStation.isNull()){
        QList<ReadingTypes> readingTypes = this->activeStation->getSupportedReadingTypes();
        foreach(const ReadingTypes &type, readingTypes){
            this->ui->comboBox_readingTypes->addItem(getReadingTypeName(type));
        }
    }

}

/*!
 * \brief WatchWindowDialog::connectActiveSensor
 * Connects the active sensor so that real time readings are recognized by the watch window
 */
void WatchWindowDialog::connectActiveSensor(){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //check the active station
    if(this->currentJob->getActiveStation().isNull()){ //no active station available
        return;
    }else if(this->activeStation.isNull()){ //the active station has not been set yet

        //save and connect the active station
        this->activeStation = this->currentJob->getActiveStation();
        QObject::connect(this->activeStation.data(), &Station::sensorChanged, this, &WatchWindowDialog::connectActiveSensor, Qt::AutoConnection);
        if(!this->activeStation->getSensorListener().isNull()){
            QObject::connect(this->activeStation->getSensorListener().data(), &SensorListener::realTimeReading, this, &WatchWindowDialog::realTimeReading, Qt::AutoConnection);
        }

        //update settings based on the new sensor
        this->initModels();

    }else if(this->activeStation != this->currentJob->getActiveStation()){ //there is a new active station

        //disconnect the old station
        this->disconnectSensor();

        //save and connect the active station
        this->activeStation = this->currentJob->getActiveStation();
        QObject::connect(this->activeStation.data(), &Station::sensorChanged, this, &WatchWindowDialog::connectActiveSensor, Qt::AutoConnection);
        if(!this->activeStation->getSensorListener().isNull()){
            QObject::connect(this->activeStation->getSensorListener().data(), &SensorListener::realTimeReading, this, &WatchWindowDialog::realTimeReading, Qt::AutoConnection);
        }

        //update settings based on the new sensor
        this->initModels();

    }else{ //there is a new active sensor

        //disconnect the old station
        this->disconnectSensor();

        //save and connect the active station
        this->activeStation = this->currentJob->getActiveStation();
        QObject::connect(this->activeStation.data(), &Station::sensorChanged, this, &WatchWindowDialog::connectActiveSensor, Qt::AutoConnection);
        if(!this->activeStation->getSensorListener().isNull()){
            QObject::connect(this->activeStation->getSensorListener().data(), &SensorListener::realTimeReading, this, &WatchWindowDialog::realTimeReading, Qt::AutoConnection);
        }

        //update settings based on the new sensor
        this->initModels();

    }

}

/*!
 * \brief WatchWindowDialog::disconnectSensor
 * Disconnects the current sensor so that relatime readings are not recognized by the watch window anymore
 */
void WatchWindowDialog::disconnectSensor(){

    //check the active station
    if(this->activeStation.isNull()){
        return;
    }

    //disconnect the active station
    QObject::disconnect(this->activeStation.data(), &Station::sensorChanged, this, &WatchWindowDialog::connectActiveSensor);
    if(!this->activeStation->getSensorListener().isNull()){
        QObject::disconnect(this->activeStation->getSensorListener().data(), &SensorListener::realTimeReading, this, &WatchWindowDialog::realTimeReading);
    }

    //reset active station
    this->activeStation = QPointer<Station>(NULL);

}

/*!
 * \brief WatchWindowDialog::connectJob
 */
void WatchWindowDialog::connectJob(){

    QObject::connect(this->currentJob.data(), &OiJob::activeStationChanged, this, &WatchWindowDialog::connectActiveSensor, Qt::AutoConnection);

}

/*!
 * \brief WatchWindowDialog::setUpCartesianWatchWindow
 * \param reading
 */
void WatchWindowDialog::setUpCartesianWatchWindow(const QVariantMap &reading){

    //init variables
    QString name, actNom, obs, value, displayValue;

    //check and get active coordinate system
    QPointer<CoordinateSystem> activeSystem = this->currentJob->getActiveCoordinateSystem();
    if(activeSystem.isNull()){
        return;
    }

    //check and get active geometry
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getGeometry().isNull()
            || !this->currentJob->getActiveFeature()->getGeometry()->getIsSolved()
            || !this->currentJob->getActiveFeature()->getGeometry()->hasPosition()){
        return;
    }
    QPointer<Geometry> activeGeometry = this->currentJob->getActiveFeature()->getGeometry();

    //get transformation parameters to transform readings
    OiMat trafo(4,4);
    if(!this->trafoController.getTransformationMatrix(trafo, this->activeStation->getCoordinateSystem(), activeSystem)){
        return;
    }

    //get and transform tracker position
    if(!reading.contains("x") || !reading.contains("y") || !reading.contains("z")){
        return;
    }
    OiVec trackerXYZ(4);
    trackerXYZ.setAt(0, reading.value("x").toDouble());
    trackerXYZ.setAt(1, reading.value("y").toDouble());
    trackerXYZ.setAt(2, reading.value("z").toDouble());
    trackerXYZ.setAt(3, 1.0);
    trackerXYZ = trafo * trackerXYZ;

    //number of visible elements
    int numVisibleElements = 1;

    //set feature name
    name ="<p align=\"center\">" + activeGeometry->getFeatureName() + "</p>";
    actNom = "<p align=\"center\"(>" + QString(activeGeometry->getIsNominal()?"nominal":"actual") + "<)/p>";
    obs = "<p align=\"center\">obs: " + QString::number(activeGeometry->getObservations().size()) + "</p>";
    displayValue = "<table width=\"100%\"> <tr> <td>" + name + "</td> <td>" + actNom + "</td> <td>" + obs + "</td> </tr> </table>";
    this->streamData["name"]->setText(displayValue);

    //set x
    if(this->settings.displayValues.contains("x")){

        //get display value
        double displayX;
        if(this->settings.reference == 0){
            displayX = trackerXYZ.getAt(0) - activeGeometry->getPosition().getVector().getAt(0);
        }else{
            displayX = activeGeometry->getPosition().getVector().getAt(0) - trackerXYZ.getAt(0);
        }

        //set color depending on tolerance
        if(qFabs(displayX) >= qFabs(this->settings.displayValues.value("x"))){
            streamData.value("x")->setPalette(Qt::red);
        }else{
            streamData.value("x")->setPalette(Qt::green);
        }

        //format display value
        name = "<p align=\"left\">x</p>";
        value = "<p align=\"right\">" + QString::number(displayX, 'f', this->settings.digits) + "</p>";
        displayValue = "<table width=\"100%\"> <tr> <td width=\"50%\">" + name + "</td> <td width=\"50%\">" + value + "</td> </tr></table>";
        streamData.value("x")->setText(displayValue);

        numVisibleElements++;

    }

    //set y
    if(this->settings.displayValues.contains("y")){

        //get display value
        double displayY;
        if(this->settings.reference == 0){
            displayY = trackerXYZ.getAt(1) - activeGeometry->getPosition().getVector().getAt(1);
        }else{
            displayY = activeGeometry->getPosition().getVector().getAt(1) - trackerXYZ.getAt(1);
        }

        //set color depending on tolerance
        if(qFabs(displayY) >= qFabs(this->settings.displayValues.value("x"))){
            streamData.value("y")->setPalette(Qt::red);
        }else{
            streamData.value("y")->setPalette(Qt::green);
        }

        //format display value
        name = "<p align=\"left\">y</p>";
        value = "<p align=\"right\">" + QString::number(displayY, 'f', this->settings.digits) + "</p>";
        displayValue = "<table width=\"100%\"> <tr> <td width=\"50%\">" + name + "</td> <td width=\"50%\">" + value + "</td> </tr></table>";
        streamData.value("y")->setText(displayValue);

        numVisibleElements++;

    }

    //set z
    if(this->settings.displayValues.contains("z")){

        //get display value
        double displayZ;
        if(this->settings.reference == 0){
            displayZ = trackerXYZ.getAt(2) - activeGeometry->getPosition().getVector().getAt(2);
        }else{
            displayZ = activeGeometry->getPosition().getVector().getAt(2) - trackerXYZ.getAt(2);
        }

        //set color depending on tolerance
        if(qFabs(displayZ) >= qFabs(this->settings.displayValues.value("z"))){
            streamData.value("z")->setPalette(Qt::red);
        }else{
            streamData.value("z")->setPalette(Qt::green);
        }

        //format display value
        name = "<p align=\"left\">z</p>";
        value = "<p align=\"right\">" + QString::number(displayZ, 'f', this->settings.digits) + "</p>";
        displayValue = "<table width=\"100%\"> <tr> <td width=\"50%\">" + name + "</td> <td width=\"50%\">" + value + "</td> </tr></table>";
        streamData.value("z")->setText(displayValue);

        numVisibleElements++;

    }

    //set d3D
    if(this->settings.displayValues.contains("d3D")){

        //get display value
        OiVec d = activeGeometry->getPosition().getVectorH() - trackerXYZ;
        double displayD3D = qSqrt(d.getAt(0)*d.getAt(0)+d.getAt(1)*d.getAt(1)+d.getAt(2)*d.getAt(2));

        //set color depending on tolerance
        if(qFabs(displayD3D) >= qFabs(this->settings.displayValues.value("d3D"))){
            streamData.value("d3D")->setPalette(Qt::red);
        }else{
            streamData.value("d3D")->setPalette(Qt::green);
        }

        //format display value
        name = "<p align=\"left\">d3D</p>";
        value = "<p align=\"right\">" + QString::number(displayD3D, 'f', this->settings.digits) + "</p>";
        displayValue = "<table width=\"100%\"> <tr> <td width=\"50%\">" + name + "</td> <td width=\"50%\">" + value + "</td> </tr></table>";
        streamData.value("d3D")->setText(displayValue);

        numVisibleElements++;

    }

    //set visibility
    if(this->settings.displayValues.contains("x")){
        this->streamData["x"]->setVisible(true);
    }else{
        this->streamData["x"]->setVisible(false);
    }
    if(this->settings.displayValues.contains("y")){
        this->streamData["y"]->setVisible(true);
    }else{
        this->streamData["y"]->setVisible(false);
    }
    if(this->settings.displayValues.contains("z")){
        this->streamData["z"]->setVisible(true);
    }else{
        this->streamData["z"]->setVisible(false);
    }
    if(this->settings.displayValues.contains("d3D")){
        this->streamData["d3D"]->setVisible(true);
    }else{
        this->streamData["d3D"]->setVisible(false);
    }
    //this->streamData["name"]->setVisible(true);
    //this->streamData["x"]->setVisible(true);
    //this->streamData["y"]->setVisible(true);
    //this->streamData["z"]->setVisible(true);
    //this->streamData["d3D"]->setVisible(true);
    //this->streamData["azimuth"]->setVisible(false);
    //this->streamData["zenith"]->setVisible(false);
    //this->streamData["distance"]->setVisible(false);

    //stretch visible elements
    masterLayout->setStretch(numVisibleElements,1);

    //resize labels (maximum font size that is possible)
    this->resizeWatchWindowValues();

}

/*!
 * \brief WatchWindowDialog::getDefaultSettings
 */
void WatchWindowDialog::getDefaultSettings(){

    if(this->ui->checkBox_x->isChecked()){
        this->settings.displayValues.insert("x", this->ui->lineEdit_x->text().toDouble());
    }else{
        this->settings.displayValues.remove("x");
    }

    if(this->ui->checkBox_y->isChecked()){
        this->settings.displayValues.insert("y", this->ui->lineEdit_y->text().toDouble());
    }else{
        this->settings.displayValues.remove("y");
    }

    if(this->ui->checkBox_z->isChecked()){
        this->settings.displayValues.insert("z", this->ui->lineEdit_z->text().toDouble());
    }else{
        this->settings.displayValues.remove("z");
    }

    if(this->ui->checkBox_d3d->isChecked()){
        this->settings.displayValues.insert("d3D", this->ui->lineEdit_d3d->text().toDouble());
    }else{
        this->settings.displayValues.remove("d3D");
    }

    if(this->ui->checkBox_azimuth->isChecked()){
        this->settings.displayValues.insert("azimuth", this->ui->lineEdit_azimuth->text().toDouble());
    }else{
        this->settings.displayValues.remove("azimuth");
    }

    if(this->ui->checkBox_zenith->isChecked()){
        this->settings.displayValues.insert("zenith", this->ui->lineEdit_zenith->text().toDouble());
    }else{
        this->settings.displayValues.remove("zenith");
    }

    if(this->ui->checkBox_distance->isChecked()){
        this->settings.displayValues.insert("distance", this->ui->lineEdit_distance->text().toDouble());
    }else{
        this->settings.displayValues.remove("distance");
    }

}

/*!
 * \brief WatchWindowDialog::resizeWatchWindowValues
 * Resizes the watch window values so that the font is at maximum
 */
void WatchWindowDialog::resizeWatchWindowValues(){

    //init variables
    double w = 0.0, h = 0.0, scale = 0.0;

    this->ui->pageWatchWindow->setLayout(this->masterLayout);

    //show the labels first so the right font is returned
    this->streamData["name"]->show();
    if(this->settings.displayValues.contains("x")){
        this->streamData["x"]->show();
    }
    if(this->settings.displayValues.contains("y")){
        this->streamData["y"]->show();
    }
    if(this->settings.displayValues.contains("z")){
        this->streamData["z"]->show();
    }
    if(this->settings.displayValues.contains("d3D")){
        this->streamData["d3D"]->show();
    }

    //get current fonts
    QFont fName = this->streamData["name"]->font();
    QFont fX = this->streamData["x"]->font();
    QFont fY = this->streamData["y"]->font();
    QFont fZ = this->streamData["z"]->font();
    QFont fd3D = this->streamData["d3D"]->font();

    //calculate new fonts
    h = this->streamData["name"]->height();
    w = this->streamData["name"]->width();
    QFontMetrics fmName(fName);
    scale = h/fmName.height();
    fName.setPointSizeF(fName.pointSizeF()*scale);
    h = this->streamData["x"]->height();
    w = this->streamData["x"]->width();
    QFontMetrics fmX(fX);
    scale = h/fmX.height();
    fX.setPointSizeF(fX.pointSizeF()*scale);
    h = this->streamData["y"]->height();
    w = this->streamData["y"]->width();
    QFontMetrics fmY(fY);
    scale = h/fmY.height();
    fY.setPointSizeF(fY.pointSizeF()*scale);
    h = this->streamData["z"]->height();
    w = this->streamData["z"]->width();
    QFontMetrics fmZ(fZ);
    scale = h/fmZ.height();
    fZ.setPointSizeF(fZ.pointSizeF()*scale);
    h = this->streamData["d3D"]->height();
    w = this->streamData["d3D"]->width();
    QFontMetrics fmd3D(fd3D);
    scale = h/fmd3D.height();
    fd3D.setPointSizeF(fd3D.pointSizeF()*scale);

    //set new fonts
    this->streamData["name"]->setFont(fName);
    this->streamData["x"]->setFont(fX);
    this->streamData["y"]->setFont(fX);
    this->streamData["z"]->setFont(fX);
    this->streamData["d3D"]->setFont(fX);

}
