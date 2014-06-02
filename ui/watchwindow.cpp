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


}

WatchWindow::~WatchWindow()
{

    delete ui;
}


void WatchWindow::setLCDNumber(QVariantMap m){

    if(!isGUIReady){
        this->iniGUI(m);
    }

    QMapIterator<QString,QVariant> j(m);
    while (j.hasNext()) {
        j.next();

        QString name = j.key();
        QVariant qvalue = j.value();
        double dvalue = qvalue.toDouble();

        if(myStation->coordSys != activeCoordinateSystem){

            TrafoParam *tp = myStation->coordSys->findTrafoParam(activeCoordinateSystem);
            if(tp != NULL){
                OiMat t;
                if(tp->to == activeCoordinateSystem){
                    t = tp->homogenMatrix;
                }else{
                    t = tp->homogenMatrix.inv();
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

        if(OiFeatureState::getActiveCoordinateSystem() == OiFeatureState::getActiveStation()->coordSys && OiFeatureState::getActiveFeature() != NULL){
            if(name == "x"){

                double featureX = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ()->getAt(0);
                double dx = featureX - dvalue;
                streamData.value(name)->display(QString::number(dx*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));

            }else if(name == "y"){

                double featureY = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ()->getAt(1);
                double dy = featureY - dvalue;
                streamData.value(name)->display(QString::number(dy*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));

            }else if(name == "z"){

                double featureZ = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ()->getAt(2);
                double dz = featureZ - dvalue;
                streamData.value(name)->display(QString::number(dz*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));
				
            }else{
                streamData.value(name)->display(QString::number(dvalue*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));
            }
        }else{
            streamData.value(name)->display(QString::number(dvalue*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));
        }




    }
}

void WatchWindow::iniGUI(QVariantMap m)
{
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

    this->close();

    delete this;

    e->accept();

}

void WatchWindow::showEvent(QShowEvent *event)
{
    if(OiFeatureState::getActiveStation() != NULL && OiFeatureState::getActiveStation()->sensorPad->instrument != NULL){
        connect(OiFeatureState::getActiveStation()->sensorPad->instrumentListener,SIGNAL(sendReadingMap(QVariantMap)),this,SLOT(setLCDNumber(QVariantMap)));

        int r = Configuration::ePolar;

        OiFeatureState::getActiveStation()->emitStartReadingStream(r);

        event->accept();
    }
}





