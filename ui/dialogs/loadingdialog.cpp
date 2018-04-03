#include "loadingdialog.h"
#include "ui_loadingdialog.h"

/*!
 * \brief LoadingDialog::LoadingDialog
 * \param parent
 */
LoadingDialog::LoadingDialog(QWidget *parent) :
      QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint ),
    ui(new Ui::LoadingDialog)
{
    ui->setupUi(this);
    this->setModal(true);
}

/*!
 * \brief LoadingDialog::~LoadingDialog
 */
LoadingDialog::~LoadingDialog(){
    delete ui;
}

/*!
 * \brief LoadingDialog::updateProgress
 * \param progress
 * \param msg
 */
void LoadingDialog::updateProgress(const int &progress, const QString &msg) const{

    //update display text and progress bar
    this->ui->label_description->setText(msg);
    this->ui->progressBar->setValue(progress);

}

/*!
 * \brief LoadingDialog::showEvent
 * \param event
 */
void LoadingDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    event->accept();

}

/*!
 * \brief LoadingDialog::closeEvent
 * \param event
 */
void LoadingDialog::closeEvent(QCloseEvent *event){

    //reset display text and progress bar
    this->ui->label_description->setText("");
    this->ui->progressBar->setValue(0);

    event->accept();

}
