#include "watchwindow.h"
#include "ui_watchwindow.h"

WatchWindow::WatchWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WatchWindow)
{
    ui->setupUi(this);

    streamActiv = false;

    ui->lcdNumber->setMode(QLCDNumber::Dec);
    ui->lcdNumber_2->setMode(QLCDNumber::Dec);
    ui->lcdNumber_3->setMode(QLCDNumber::Dec);

    ui->lcdNumber->setDigitCount(4);
    ui->lcdNumber_2->setDigitCount(4);
    ui->lcdNumber_3->setDigitCount(4);

    ui->lcdNumber->setSmallDecimalPoint(true);
    ui->lcdNumber_2->setSmallDecimalPoint(true);
    ui->lcdNumber_3->setSmallDecimalPoint(true);




}

WatchWindow::~WatchWindow()
{


    delete ui;
}

void WatchWindow::on_pushButton_clicked()
{

     connect(myStation->sensorPad->instrumentListener,SIGNAL(sendReadingMap(QVariantMap)),this,SLOT(setLCDNumber(QVariantMap)));

    if (streamActiv == false){

        int r = Configuration::ePolar;

        myStation->emitStartReadingStream(r);
        streamActiv = true;

    }else{
        myStation->emitStopReadingStream();
        streamActiv = false;
    }
}

void WatchWindow::setLCDNumber(QVariantMap m){

        QMap<QString,QVariant>::const_iterator i = m.constBegin();


        QString name1 = i.key();
        QVariant q1 = i.value();
        QString x =  q1.toString();

        ui->label->setText(name1);
        ui->lcdNumber->display(x);


        i++;
        QString name2 = i.key();
        QVariant q2 = i.value();
        QString y = q2.toString();

        ui->label_2->setText(name2);
        ui->lcdNumber_2->display(y);

        /*i++;
        QString name3 = i.key();
        QVariant q3 = i.value();
        QString z =  q3.toString();

        ui->label_3->setText(name3);
        ui->lcdNumber_3->display(z);*/

}
