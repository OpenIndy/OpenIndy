#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
    this->ui->lineEdit_distanceDigits->setText("2");
    this->ui->lineEdit_angleDigits->setText("6");
    this->ui->lineEdit_temperatureDigits->setText("1");
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

/*!
 * \brief ExportDialog::showEvent
 * \param event
 */
void ExportDialog::showEvent(QShowEvent *event)
{
    //initialize models
    this->initModels();

    event->accept();
}

/*!
 * \brief ExportDialog::initModels
 */
void ExportDialog::initModels()
{
    //assign unit types models
    this->ui->comboBox_distance_unit->setModel(&ModelManager::getUnitTypesModel(eMetric));
    this->ui->comboBox_angle_unit->setModel(&ModelManager::getUnitTypesModel(eAngular));
    this->ui->comboBox_temperature_unit->setModel(&ModelManager::getUnitTypesModel(eTemperature));

    //set unit type defaults
    this->ui->comboBox_distance_unit->setCurrentText(getUnitTypeName(eUnitMilliMeter));
    this->ui->comboBox_angle_unit->setCurrentText(getUnitTypeName(eUnitDecimalDegree));
    this->ui->comboBox_temperature_unit->setCurrentText(getUnitTypeName(eUnitGrad));

    //assign plugin model
    this->ui->comboBox_plugin->setModel(&ModelManager::getPluginNamesModel());
}

/*!
 * \brief ExportDialog::on_pushButton_export_clicked
 */
void ExportDialog::on_pushButton_export_clicked()
{
    //check if all necessary parameters have been specified
    if(this->ui->lineEdit_choosePath->text().compare("") == 0){
        QMessageBox::warning(NULL, "no file", "No export file has been selected.");
        return;
    }else if(this->ui->comboBox_plugin->currentText().compare("") == 0){
        QMessageBox::warning(NULL, "no plugin", "No plugin has been selected.");
        return;
    }else if(this->ui->comboBox_method->currentText().compare("") == 0){
        QMessageBox::warning(NULL, "no plugin", "No exchange method has been selected.");
        return;
    }

    //set params
    ExchangeParams params;
    params.exchangeFilePath = this->ui->lineEdit_choosePath->text();
    params.delimiter = "whitespace [ ]";
    params.exchangeName = this->ui->comboBox_method->currentText();
    params.pluginName = this->ui->comboBox_plugin->currentText();
    params.isDefinedFormat = false;
    params.units.insert(eMetric, getUnitTypeEnum(this->ui->comboBox_distance_unit->currentText()));
    params.units.insert(eAngular, getUnitTypeEnum(this->ui->comboBox_angle_unit->currentText()));
    params.units.insert(eTemperature, getUnitTypeEnum(this->ui->comboBox_temperature_unit->currentText()));
    params.distanceDigits = this->ui->lineEdit_distanceDigits->text().toInt();
    params.angleDigits = this->ui->lineEdit_angleDigits->text().toInt();
    params.temperatureDigits = this->ui->lineEdit_temperatureDigits->text().toInt();

    emit this->startExport(params);

    this->close();

}

/*!
 * \brief ExportDialog::on_pushButton_Path_clicked
 */
void ExportDialog::on_pushButton_Path_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.*)"));
    this->ui->lineEdit_choosePath->setText(path);
}

/*!
 * \brief ExportDialog::on_comboBox_plugin_currentIndexChanged
 * \param arg1
 */
void ExportDialog::on_comboBox_plugin_currentIndexChanged(const QString &arg1)
{
    //if no plugin is selected clear other gui elements
    if(arg1.compare("") == 0){
        this->ui->comboBox_method->clear();
    }
    //delete old model and set new one
    if(this->ui->comboBox_method->model() != NULL){
        delete this->ui->comboBox_method->model();
    }
    this->ui->comboBox_method->setModel(ModelManager::getExchangeSimpleAsciiNames(arg1).data());
}
