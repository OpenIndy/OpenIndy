#include "measurebehaviordialog.h"
#include "ui_measurebehaviordialog.h"

MeasureBehaviorDialog::MeasureBehaviorDialog(QWidget *parent) :
    QDialog(parent),
    decision(Decision::eNotSet),
    ui(new Ui::MeasureBehaviorDialog)
{
    ui->setupUi(this);
}

MeasureBehaviorDialog::~MeasureBehaviorDialog()
{
    delete ui;
}

void MeasureBehaviorDialog::on_pushButton_searchSMR_clicked()
{
    this->clicked(Decision::eSearchSMR);
}

void MeasureBehaviorDialog::on_pushButton_tryAgain_clicked()
{
    this->clicked(Decision::eTryAgain);
}

void MeasureBehaviorDialog::on_pushButton_skip_clicked()
{
    this->clicked(Decision::eSkip);
}

void MeasureBehaviorDialog::clicked(Decision decision) {
    this->decision = decision;

    accept(); // close dialog
}

void MeasureBehaviorDialog::resetDecision() {
    this->decision = Decision::eNotSet;
}

bool MeasureBehaviorDialog::skip() {
    return this->decision == Decision::eSkip;
}

bool MeasureBehaviorDialog::searchSMR() {
    return this->decision == Decision::eSearchSMR;
}

bool MeasureBehaviorDialog::tryAgain() {
    return this->decision == Decision::eTryAgain;
}
