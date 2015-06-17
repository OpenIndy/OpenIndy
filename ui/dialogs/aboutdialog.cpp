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
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

/*!
 * \brief AboutDialog::showEvent
 * \param event
 */
void AboutDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    event->accept();

}
