#include "sensorinfodialog.h"
#include "ui_sensorinfodialog.h"

/*!
 * \brief SensorInfoDialog::SensorInfoDialog
 * \param parent
 */
SensorInfoDialog::SensorInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SensorInfoDialog)
{
    ui->setupUi(this);
}

/*!
 * \brief SensorInfoDialog::~SensorInfoDialog
 */
SensorInfoDialog::~SensorInfoDialog()
{
    delete ui;
}

/*!
 * \brief SensorInfoDialog::showInfo
 * \param info
 */
void SensorInfoDialog::showInfo(QString info){
    ui->label_info->setText(info);
    this->show();
}

/*!
 * \brief SensorInfoDialog::hideInfo
 * \param check
 */
void SensorInfoDialog::hideInfo(bool check){
    if(check){
        this->close();
    }else{
        ui->label_info->setText("sensor action failed");
    }
}

void SensorInfoDialog::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
    event->accept();
}

/*!
 * \brief SensorInfoDialog::on_pushButton_cancel_clicked
 */
void SensorInfoDialog::on_pushButton_cancel_clicked()
{
    this->close();
}
