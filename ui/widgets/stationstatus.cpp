#include "stationstatus.h"
#include "ui_stationstatus.h"

StationStatus::StationStatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StationStatus)
{
    ui->setupUi(this);
}

StationStatus::~StationStatus()
{
    delete ui;
}

void StationStatus::setName(QString name) {
    this->ui->name->setText(name);
}

void StationStatus::setTemperature(QString temperature) {
    this->ui->temperature->setText(temperature);
}
