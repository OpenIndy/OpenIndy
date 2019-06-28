#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    //set version information
    this->ui->label_oicoreversion->setText(OPENINDY_CORE_VERSION);
    this->ui->label_oimathversion->setText(OPENINDY_MATH_VERSION);
    this->ui->label_oiversion->setText(OPENINDY_VERSION);
    QString oicommit = OPENINDY_COMMIT;
    QString oiversion = OPENINDY_VERSION;
    QString oiBuild = QString::number(OPENINDY_BUILD);
    this->ui->label_BuildNumber->setText(oicommit + "_" + oiversion + "_" + oiBuild);

}

AboutDialog::~AboutDialog()
{
    delete ui;
}
