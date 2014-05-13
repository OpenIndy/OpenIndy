#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    initGUI();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

/*!
 * \brief SettingsDialog::setPluginsModel
 */
void SettingsDialog::setPluginsModel(PluginTreeViewModel *model){
    this->ui->treeView_plugins->setModel(model);
}

void SettingsDialog::on_pushButton_ok_clicked()
{
    saveSettings();
    Configuration::generateAllAttributes();
    Configuration::generateFeatureAttributes();
    Configuration::generateTrafoParamAttributes();
    this->close();
}

void SettingsDialog::on_pushButton_cancel_clicked()
{
    this->close();
}

void SettingsDialog::initGUI(){

    ui->comboBox_angleType->insertItem(ui->comboBox_angleType->count(),"decimal degree",UnitConverter::eDECIMALDEGREE);
    ui->comboBox_angleType->insertItem(ui->comboBox_angleType->count(),"gon",UnitConverter::eGON);
    ui->comboBox_angleType->insertItem(ui->comboBox_angleType->count(),"radiant",UnitConverter::eRADIANT);

    ui->comboBox_distanceType->insertItem(ui->comboBox_distanceType->count(),"meter",UnitConverter::eMETER);
    ui->comboBox_distanceType->insertItem(ui->comboBox_distanceType->count(),"millimeter",UnitConverter::eMILLIMETER);

    ui->comboBox_temperatureType->insertItem(ui->comboBox_temperatureType->count(),"degree",UnitConverter::eGRAD);

    ui->lineEdit_angleDigits->setText(QString::number(UnitConverter::angleDigits));
    ui->lineEdit_distanceDigits->setText(QString::number(UnitConverter::distanceDigits));
    ui->lineEdit_temperatureDigits->setText(QString::number(UnitConverter::temperatureDigits));

}

void SettingsDialog::showEvent(QShowEvent *event){

    ui->lineEdit_angleDigits->setText(QString::number(UnitConverter::angleDigits));
    ui->lineEdit_distanceDigits->setText(QString::number(UnitConverter::distanceDigits));
    ui->lineEdit_temperatureDigits->setText(QString::number(UnitConverter::temperatureDigits));

    ui->comboBox_angleType->setCurrentIndex(ui->comboBox_angleType->findData(UnitConverter::angleType));
    ui->comboBox_distanceType->setCurrentIndex(ui->comboBox_distanceType->findData(UnitConverter::distanceType));
    ui->comboBox_temperatureType->setCurrentIndex(ui->comboBox_temperatureType->findData(UnitConverter::temperatureType));

    event->accept();
}

void SettingsDialog::saveSettings(){

    UnitConverter::angleDigits = ui->lineEdit_angleDigits->text().toInt();
    UnitConverter::distanceDigits = ui->lineEdit_distanceDigits->text().toInt();
    UnitConverter::temperatureDigits = ui->lineEdit_temperatureDigits->text().toInt();
    UnitConverter::setAngleUnit(static_cast<UnitConverter::unitType>(ui->comboBox_angleType->itemData(ui->comboBox_angleType->currentIndex()).toInt()));
    UnitConverter::setDistanceUnit(static_cast<UnitConverter::unitType>(ui->comboBox_distanceType->itemData(ui->comboBox_distanceType->currentIndex()).toInt()));
    UnitConverter::setTemperatureUnit(static_cast<UnitConverter::unitType>(ui->comboBox_temperatureType->itemData(ui->comboBox_temperatureType->currentIndex()).toInt()));
}
