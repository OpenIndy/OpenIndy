#include "exportnominaldialog.h"
#include "ui_exportnominaldialog.h"

ExportNominalDialog::ExportNominalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportNominalDialog)
{
    ui->setupUi(this);
}

ExportNominalDialog::~ExportNominalDialog()
{
    delete ui;
}
