#include "movementdialog.h"
#include "ui_movementdialog.h"

MovementDialog::MovementDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MovementDialog)
{
    ui->setupUi(this);

    initGUI();
}

MovementDialog::~MovementDialog()
{
    delete ui;
}

void MovementDialog::on_pushButton_move_clicked()
{
    Reading *r = new Reading();
    switch (typeOfReading) {
    case Configuration::ePolar:
        r->typeofReading = this->typeOfReading;
        r->rPolar.azimuth = (ui->lineEdit_azimuth->text().toDouble()*3.141592653589793/(180.0));
        r->rPolar.zenith = (ui->lineEdit_zenith->text().toDouble()*3.141592653589793/(180.0));
        r->rPolar.distance = ui->lineEdit_distance->text().toDouble();
        break;
    case Configuration::eCartesian:
        r->typeofReading = this->typeOfReading;
        OiVec xyzReading(4);
        xyzReading.setAt(0, ui->lineEdit_x->text().toDouble());
        xyzReading.setAt(1, ui->lineEdit_y->text().toDouble());
        xyzReading.setAt(2, ui->lineEdit_z->text().toDouble());
        xyzReading.setAt(3, 1.0);
        r->rCartesian.xyz = xyzReading;
        break;
    }

    sendReading(r);

    this->close();

}

void MovementDialog::initGUI(){

    ui->comboBox_typeOfReading->insertItem(ui->comboBox_typeOfReading->count(),"polar",Configuration::ePolar);
    ui->comboBox_typeOfReading->insertItem(ui->comboBox_typeOfReading->count(),"cartesian",Configuration::eCartesian);

}

void MovementDialog::on_comboBox_typeOfReading_currentIndexChanged(const QString &arg1)
{

    typeOfReading = static_cast<Configuration::ReadingTypes>(ui->comboBox_typeOfReading->itemData(ui->comboBox_typeOfReading->currentIndex()).toInt());

    switch (typeOfReading) {
    case Configuration::ePolar:
            ui->lineEdit_azimuth->setEnabled(true);
            ui->lineEdit_zenith->setEnabled(true);
            ui->lineEdit_distance->setEnabled(true);

            ui->lineEdit_x->setEnabled(false);
            ui->lineEdit_y->setEnabled(false);
            ui->lineEdit_z->setEnabled(false);
        break;
    case Configuration::eCartesian:
        ui->lineEdit_azimuth->setEnabled(false);
        ui->lineEdit_zenith->setEnabled(false);
        ui->lineEdit_distance->setEnabled(false);

        ui->lineEdit_x->setEnabled(true);
        ui->lineEdit_y->setEnabled(true);
        ui->lineEdit_z->setEnabled(true);
        break;
    default:
        break;
    }
}
