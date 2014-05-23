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
        double dvalue = qvalue.toDouble()*UnitConverter::getDistanceMultiplier();
        QString value =  QString::number(dvalue);

        if(activeCoordinateSystem == myStation->coordSys && activeFeature != NULL){
            if(name == "x"){

                double featureX = activeFeature->getFeature()->getDisplayX().toDouble();
                double dx = featureX - value.toDouble();
                streamData.value(name)->display(QString::number(dx,'f',UnitConverter::distanceDigits));

            }else if(name == "y"){

                double featureY = activeFeature->getFeature()->getDisplayY().toDouble();
                double dy = featureY - value.toDouble();
                streamData.value(name)->display(QString::number(dy,'f',UnitConverter::distanceDigits));

            }else if(name == "z"){

                double featureZ = activeFeature->getFeature()->getDisplayZ().toDouble();
                double dz = featureZ - value.toDouble();
                streamData.value(name)->display(QString::number(dz,'f',UnitConverter::distanceDigits));

            }else{
                streamData.value(name)->display(value);
            }
        }else{
            streamData.value(name)->display(value);
        }




    }

}

void WatchWindow::iniGUI(QVariantMap m)
{
    if(masterLayout == NULL){
        masterLayout = new QVBoxLayout();
    }

    if(activeFeature != NULL){
        QLabel *featureName = new QLabel();
        QFont f( "Arial", 30, QFont::Bold);
        featureName->setFont(f);
        featureName->setText(activeFeature->getFeature()->name);
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
    myStation->emitStopReadingStream();
    isGUIReady = false;
    streamData.clear();

    disconnect(myStation->sensorPad->instrumentListener,SIGNAL(sendReadingMap(QVariantMap)),this,SLOT(setLCDNumber(QVariantMap)));

    delete masterLayout;
    masterLayout = NULL;

    this->close();

    delete this;

    e->accept();

}

void WatchWindow::showEvent(QShowEvent *event)
{
    if(myStation != NULL && myStation->sensorPad->instrument != NULL){
        connect(myStation->sensorPad->instrumentListener,SIGNAL(sendReadingMap(QVariantMap)),this,SLOT(setLCDNumber(QVariantMap)));

        int r = Configuration::ePolar;

        myStation->emitStartReadingStream(r);

        event->accept();
    }
}





