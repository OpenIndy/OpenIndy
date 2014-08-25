#include "ps_loadingdialog.h"
#include "ui_ps_loadingdialog.h"

PS_LoadingDialog::PS_LoadingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PS_LoadingDialog)
{
    ui->setupUi(this);
}

PS_LoadingDialog::~PS_LoadingDialog()
{
    delete ui;
}

void PS_LoadingDialog::reset(){

    this->ui->progressBar_status->setValue(0);
    this->ui->listwidget_msg->clear();

}

void PS_LoadingDialog::setStatus(QString msg, int status){

    this->ui->listwidget_msg->addItem(msg);
    this->ui->progressBar_status->setValue(status);

}
