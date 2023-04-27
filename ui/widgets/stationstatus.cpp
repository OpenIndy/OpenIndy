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

void StationStatus:: setBundled(bool bundled) {
    this->ui->bundled->setText(QString("<span style=\"color: %1;\">&#x26AB;</span>").arg(bundled ? "lightgreen" : "red"));
}
