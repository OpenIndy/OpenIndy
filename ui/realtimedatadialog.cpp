#include "realtimedatadialog.h"
#include "ui_realtimedatadialog.h"

RealTimeDataDialog::RealTimeDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RealTimeDataDialog)
{
    ui->setupUi(this);
}

RealTimeDataDialog::~RealTimeDataDialog()
{
    delete ui;
}

void RealTimeDataDialog::closeEvent(QCloseEvent *event)
{
    OiJob::getActiveStation()->emitStopSensorStatsStream();

    qRegisterMetaType<streamData>("streamData");
    disconnect(OiJob::getActiveStation()->sensorPad->instrumentListener,SIGNAL(sendSensorStats(streamData)),
            this,SLOT(getRTData(streamData)));
    event->accept();
}

void RealTimeDataDialog::showEvent(QShowEvent *event)
{

    ui->label_sensorName->setText("no active sensor");
    ui->listView_RTData->setModel(NULL);
    if(checkSensorValid()){
        ui->label_sensorName->setText(OiJob::getActiveStation()->sensorPad->instrument->getMetaData()->name);


        qRegisterMetaType<streamData>("streamData");
        connect(OiJob::getActiveStation()->sensorPad->instrumentListener,SIGNAL(sendSensorStats(streamData)),
                this,SLOT(getRTData(streamData)));
        OiJob::getActiveStation()->emitStartSensorStatsStream();
    }
    event->accept();
}

bool RealTimeDataDialog::checkSensorValid()
{
    if(OiJob::getActiveStation() != NULL && OiJob::getActiveStation()->sensorPad != NULL &&
            OiJob::getActiveStation()->sensorPad->instrument != NULL){
        return true;
    }else{
        return false;
    }
}

void RealTimeDataDialog::getRTData(QMap<QString, QString> rtData)
{
    rtDataList.clear();
    QMapIterator<QString,QString>i(rtData);
    while(i.hasNext()){
        i.next();
        rtDataList.append(QString(i.key() +": "+ i.value()));
    }
    rtDataModel = new QStringListModel(rtDataList);
    ui->listView_RTData->setModel(rtDataModel);
}
