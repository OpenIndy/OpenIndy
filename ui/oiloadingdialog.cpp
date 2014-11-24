#include "oiloadingdialog.h"
#include "ui_oiloadingdialog.h"

OiLoadingDialog *OiLoadingDialog::myDialog = NULL;

OiLoadingDialog::OiLoadingDialog(QWidget *parent) :
      QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint ),
    ui(new Ui::OiLoadingDialog)
{
    ui->setupUi(this);
}

OiLoadingDialog::~OiLoadingDialog()
{
    delete ui;
}

OiLoadingDialog *OiLoadingDialog::getInstance(){
    return OiLoadingDialog::myDialog;
}

/*!
 * \brief OiLoadingDialog::showLoadingDialog
 * If not yet opened this function opens and returns a LoadingDialog
 * \return
 */
const OiLoadingDialog *OiLoadingDialog::showLoadingDialog(){

    if(OiLoadingDialog::myDialog == NULL){

        OiLoadingDialog::myDialog = new OiLoadingDialog();
        OiLoadingDialog::myDialog->show();
        return OiLoadingDialog::myDialog;

    }
    return NULL;

}

/*!
 * \brief OiLoadingDialog::closeLoadingDialog
 * If opened this method closes that LoadingDialog
 */
void OiLoadingDialog::closeLoadingDialog(){

    if(OiLoadingDialog::myDialog != NULL){

        OiLoadingDialog::myDialog->ui->progressBar->setValue(100);

        OiLoadingDialog::myDialog->close();
        delete OiLoadingDialog::myDialog;
        OiLoadingDialog::myDialog = NULL;

    }

}

/*!
 * \brief OiLoadingDialog::updateProgress
 * Update progress bar
 * \param progress
 */
void OiLoadingDialog::updateProgress(int progress, QString msg) const{

    if(progress < 0){
        progress = 0;
    }else if(progress > 100){
        progress = 100;
    }

    this->ui->progressBar->setValue(progress);
    this->ui->label_description->setText(msg);

}
