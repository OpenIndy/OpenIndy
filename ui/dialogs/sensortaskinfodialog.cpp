#include "sensortaskinfodialog.h"
#include "ui_sensortaskinfodialog.h"

/*!
 * \brief SensorTaskInfoDialog::SensorTaskInfoDialog
 * \param parent
 */
SensorTaskInfoDialog::SensorTaskInfoDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::SensorTaskInfoDialog)
{
    ui->setupUi(this);
}

/*!
 * \brief SensorTaskInfoDialog::~SensorTaskInfoDialog
 */
SensorTaskInfoDialog::~SensorTaskInfoDialog(){
    delete ui;
}

/*!
 * \brief SensorTaskInfoDialog::setDisplayMessage
 * \param msg
 */
void SensorTaskInfoDialog::setDisplayMessage(const QString &msg){
    this->ui->label_info->setText(msg);
}

/*!
 * \brief SensorTaskInfoDialog::on_pushButton_cancel_clicked
 */
void SensorTaskInfoDialog::on_pushButton_cancel_clicked(){
    this->close();
}

