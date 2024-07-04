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
    this->ui->temperature->setText(QString("%1%2").arg(temperature).arg(" °C"));
}

void StationStatus::setBundled(bool bundled) {
    this->ui->bundled->setText(QString( bundled
                                       ? "bundled<span style=\"color: lightgreen;\">&#x2705;</span>"
                                       : "not bundled<span style=\"color: red;\">&#x274C;</span>"
                                    )
                               );
}

void StationStatus::reset() {
    this->ui->name->clear();
    this->setTemperature("-/-");
    this->ui->bundled->clear();
}
