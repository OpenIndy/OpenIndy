#include "measurebehaviordialog.h"
#include "ui_measurebehaviordialog.h"

MeasureBehaviorDialog::MeasureBehaviorDialog(QWidget *parent) :
    QDialog(parent),
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
    emit setDecision(decision);

    accept(); // close dialog
}

