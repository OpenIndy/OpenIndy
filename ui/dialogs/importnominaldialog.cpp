#include "importnominaldialog.h"
#include "ui_importnominaldialog.h"

/*!
 * \brief ImportNominalDialog::ImportNominalDialog
 * \param parent
 */
ImportNominalDialog::ImportNominalDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::ImportNominalDialog)
{
    this->ui->setupUi(this);
}

/*!
 * \brief ImportNominalDialog::~ImportNominalDialog
 */
ImportNominalDialog::~ImportNominalDialog(){

}

/*!
 * \brief ImportNominalDialog::on_comboBox_plugin_sa_currentIndexChanged
 * Triggered whenever the user selects another plugin
 * \param arg1
 */
void ImportNominalDialog::on_comboBox_plugin_sa_currentIndexChanged(const QString &arg1){

    //if no plugin is selected clear other gui elements
    if(arg1.compare("") == 0){
        this->ui->comboBox_exchange_sa->clear();
        this->ui->comboBox_separator_sa->clear();
        this->ui->comboBox_geometry_sa->clear();
    }

    //delete old model and set new one
    if(this->ui->comboBox_exchange_sa->model() != NULL){
        delete this->ui->comboBox_exchange_sa->model();
    }
    this->ui->comboBox_exchange_sa->setModel(ModelManager::getExchangeSimpleAsciiNames(arg1).data());

}

/*!
 * \brief ImportNominalDialog::on_comboBox_exchange_sa_currentIndexChanged
 * Triggered whenever the user selects another exchange method
 * \param arg1
 */
void ImportNominalDialog::on_comboBox_exchange_sa_currentIndexChanged(const QString &arg1){

    //if no exchange method is selected clear other gui elements
    if(arg1.compare("") == 0){
        this->ui->comboBox_separator_sa->clear();
        this->ui->comboBox_geometry_sa->clear();
    }

    //delete old models and set new models for other gui elements
    if(this->ui->comboBox_separator_sa->model() != NULL){
        delete this->ui->comboBox_separator_sa->model();
    }
    this->ui->comboBox_separator_sa->setModel(ModelManager::getExchangeSimpleAsciiDelimiters(this->ui->comboBox_plugin_sa->currentText(), arg1).data());
    if(this->ui->comboBox_geometry_sa->model() != NULL){
        delete this->ui->comboBox_geometry_sa->model();
    }
    this->ui->comboBox_geometry_sa->setModel(ModelManager::getExchangeSimpleAsciiSupportedGeometries(this->ui->comboBox_plugin_sa->currentText(), arg1).data());

}

/*!
 * \brief ImportNominalDialog::on_pushButton_file_sa_clicked
 * Triggered whenever the user wants to edit the file name
 */
void ImportNominalDialog::on_pushButton_file_sa_clicked(){

    QString path = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.*)"));
    this->ui->lineEdit_file_sa->setText(path);

}

/*!
 * \brief ImportNominalDialog::on_pushButton_cancel_sa_clicked
 */
void ImportNominalDialog::on_pushButton_cancel_sa_clicked(){
    this->close();
}

/*!
 * \brief ImportNominalDialog::showEvent
 * \param event
 */
void ImportNominalDialog::showEvent(QShowEvent *event){

    //initialize models for GUI elements
    this->initModels();

    event->accept();

}

/*!
 * \brief ImportNominalDialog::initModels
 */
void ImportNominalDialog::initModels(){

    //assign unit types models
    this->ui->comboBox_distance_sa->setModel(&ModelManager::getUnitTypesModel(eMetric));
    this->ui->comboBox_angle_sa->setModel(&ModelManager::getUnitTypesModel(eAngular));
    this->ui->comboBox_temperature_sa->setModel(&ModelManager::getUnitTypesModel(eTemperature));

    //set unit type defaults
    this->ui->comboBox_distance_sa->setCurrentText(getUnitTypeName(eUnitMilliMeter));
    this->ui->comboBox_angle_sa->setCurrentText(getUnitTypeName(eUnitDecimalDegree));
    this->ui->comboBox_temperature_sa->setCurrentText(getUnitTypeName(eUnitGrad));

    //assign plugin model
    this->ui->comboBox_plugin_sa->setModel(&ModelManager::getPluginNamesModel());

    if(ModelManager::getPluginNamesModel().stringList().contains("OpenIndy Default Plugin")){
        this->ui->comboBox_plugin_sa->setCurrentText("OpenIndy Default Plugin");
    }

    //assign nominal systems model
    this->ui->comboBox_system_sa->setModel(&ModelManager::getNominalSystemsModel());

    /*//set combobox size
    int sizeSystem = oi::getDropDownMenuSize(ModelManager::getNominalSystemsModel().stringList(),this->ui->comboBox_system_sa->width());
    this->ui->comboBox_system_sa->view()->setMinimumWidth(sizeSystem);*/

    //assign group names model
    this->ui->comboBox_groupNames->setModel(&ModelManager::getGroupNamesModel());
    //Set group01 as default for import
    this->ui->comboBox_groupNames->setCurrentText("Group01");

    /*int sizeGroup = oi::getDropDownMenuSize(ModelManager::getGroupNamesModel().stringList(), this->ui->comboBox_groupNames->width());
    this->ui->comboBox_groupNames->view()->setMinimumWidth(sizeGroup);*/

}

/*!
 * \brief ImportNominalDialog::on_pushButton_import_sa_clicked
 * Triggered whenever the user wants to start the import
 */
void ImportNominalDialog::on_pushButton_import_sa_clicked(){

    //check if all necessary parameters have been specified
    if(this->ui->lineEdit_file_sa->text().compare("") == 0){
        QMessageBox::warning(NULL, "no file", "No import file has been selected.");
        return;
    }else if(this->ui->comboBox_plugin_sa->currentText().compare("") == 0){
        QMessageBox::warning(NULL, "no plugin", "No plugin has been selected.");
        return;
    }else if(this->ui->comboBox_exchange_sa->currentText().compare("") == 0){
        QMessageBox::warning(NULL, "no plugin", "No exchange method has been selected.");
        return;
    }

    //set params
    ExchangeParams params;
    params.exchangeFilePath = this->ui->lineEdit_file_sa->text();
    params.delimiter = this->ui->comboBox_separator_sa->currentText();
    params.exchangeName = this->ui->comboBox_exchange_sa->currentText();
    if(this->ui->comboBox_groupNames->currentText().compare("All Groups") != 0){
        params.groupName = this->ui->comboBox_groupNames->currentText();
    }
    params.isDefinedFormat = false;
    params.nominalSystem = this->ui->comboBox_system_sa->currentText();
    params.pluginName = this->ui->comboBox_plugin_sa->currentText();
    params.skipFirstLine = this->ui->checkBox_skipFirst_sa->isChecked();
    params.typeOfGeometry = getGeometryTypeEnum(this->ui->comboBox_geometry_sa->currentText());
    params.units.insert(eMetric, getUnitTypeEnum(this->ui->comboBox_distance_sa->currentText()));
    params.units.insert(eAngular, getUnitTypeEnum(this->ui->comboBox_angle_sa->currentText()));
    params.units.insert(eTemperature, getUnitTypeEnum(this->ui->comboBox_temperature_sa->currentText()));
    params.createActual = this->ui->checkBox_createActual->isChecked();

    //start the import
    emit this->startImport(params);

    this->close();

}
