#include "settingsdialog.h"
#include "ui_settingsdialog.h"

/*!
 * \brief SettingsDialog::SettingsDialog
 * \param parent
 */
SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::SettingsDialog)
{
    this->ui->setupUi(this);

    this->ui->lineEdit_angleDigits->setValidator(new QRegExpValidator(QRegExp("[1-8]"), this));
    this->ui->lineEdit_distanceDigits->setValidator(new QRegExpValidator(QRegExp("[1-8]"), this));
    this->ui->lineEdit_temperatureDigits->setValidator(new QRegExpValidator(QRegExp("[1-3]"), this));
    // 0 = disable autosave, > 0 = interval in minutes
    this->ui->lineEdit_autoSaveInterval->setValidator(new QIntValidator(0, 1000, this));

    //init GUI elements and assign models
    this->initModels();
    this->initGUI();
}

/*!
 * \brief SettingsDialog::~SettingsDialog
 */
SettingsDialog::~SettingsDialog()
{
}


/*!
 * \brief SettingsDialog::showEvent
 * \param event
 */
void SettingsDialog::showEvent(QShowEvent *event){
    this->ui->tabWidget_settings->setTabEnabled(2,false);
    this->ui->tabWidget_settings->setTabEnabled(3,false);
    this->ui->pushButton_ok->setEnabled(true);

    this->initGUI();

    event->accept();
}

/*!
 * \brief SettingsDialog::initGUI
 */
void SettingsDialog::initGUI(){
    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    //do not trigger changes while settings defaults
    this->ui->comboBox_angleType->blockSignals(true);
    this->ui->comboBox_distanceType->blockSignals(true);
    this->ui->comboBox_temperatureType->blockSignals(true);
    this->ui->lineEdit_angleDigits->blockSignals(true);
    this->ui->lineEdit_distanceDigits->blockSignals(true);
    this->ui->lineEdit_temperatureDigits->blockSignals(true);
    this->ui->checkBox_sounds->blockSignals(true);
    this->ui->lineEdit_autoSaveInterval->blockSignals(true);

    //set default unit, see ProjectConfig::getParameterDisplayConfig()
    this->ui->comboBox_angleType->setCurrentText(getUnitTypeName(static_cast<oi::UnitType>(ProjectConfig::getAngularUnit())));
    this->ui->comboBox_distanceType->setCurrentText(getUnitTypeName(static_cast<oi::UnitType>(ProjectConfig::getMetricUnit())));
    this->ui->comboBox_temperatureType->setCurrentText(getUnitTypeName(static_cast<oi::UnitType>(ProjectConfig::getTemperatureUnit())));
    this->ui->lineEdit_angleDigits->setText(QString::number(ProjectConfig::getAngularDigits()));
    this->ui->lineEdit_distanceDigits->setText(QString::number(ProjectConfig::getDistanceDigits()));
    this->ui->lineEdit_temperatureDigits->setText(QString::number(ProjectConfig::getTemperatureDigits()));
    this->ui->checkBox_sounds->setChecked(ProjectConfig::getUseSounds());
    this->ui->lineEdit_autoSaveInterval->setText(QString::number(ProjectConfig::getAutoSaveInterval()));

    //from now on trigger changes
    this->ui->comboBox_angleType->blockSignals(false);
    this->ui->comboBox_distanceType->blockSignals(false);
    this->ui->comboBox_temperatureType->blockSignals(false);
    this->ui->lineEdit_angleDigits->blockSignals(false);
    this->ui->lineEdit_distanceDigits->blockSignals(false);
    this->ui->lineEdit_temperatureDigits->blockSignals(false);
    this->ui->checkBox_sounds->blockSignals(false);
    this->ui->lineEdit_autoSaveInterval->blockSignals(false);
}

/*!
 * \brief SettingsDialog::initModels
 */
void SettingsDialog::initModels(){

    //set unit type models
    this->ui->comboBox_angleType->setModel(&ModelManager::getUnitTypesModel(eAngular));
    this->ui->comboBox_distanceType->setModel(&ModelManager::getUnitTypesModel(eMetric));
    this->ui->comboBox_temperatureType->setModel(&ModelManager::getUnitTypesModel(eTemperature));
}

/*!
 * \brief SettingsDialog::updateDisplayConfigFromSelection
 */
ParameterDisplayConfig SettingsDialog::updateDisplayConfigFromSelection(){

    ParameterDisplayConfig config;
    config.setDisplayDigits(eAngular, this->ui->lineEdit_angleDigits->text().toInt());
    config.setDisplayDigits(eMetric, this->ui->lineEdit_distanceDigits->text().toInt());
    config.setDisplayDigits(eTemperature, this->ui->lineEdit_temperatureDigits->text().toInt());
    config.setDisplayUnitType(eAngular, getUnitTypeEnum(this->ui->comboBox_angleType->currentText()));
    config.setDisplayUnitType(eMetric, getUnitTypeEnum(this->ui->comboBox_distanceType->currentText()));
    config.setDisplayUnitType(eTemperature, getUnitTypeEnum(this->ui->comboBox_temperatureType->currentText()));
    config.setUseSounds(this->ui->checkBox_sounds->isChecked());

    return config;
}


/*!
 * \brief SettingsDialog::on_pushButton_ok_clicked
 */
void SettingsDialog::on_pushButton_ok_clicked()
{
    ParameterDisplayConfig config = this->updateDisplayConfigFromSelection();

    // update ProjectConfig
    ProjectConfig::setAngularDigits(this->ui->lineEdit_angleDigits->text().toInt());
    ProjectConfig::setDistanceDigits(this->ui->lineEdit_distanceDigits->text().toInt());
    ProjectConfig::setTemperatureDigits(this->ui->lineEdit_temperatureDigits->text().toInt());
    ProjectConfig::setAngularUnit(getUnitTypeEnum(this->ui->comboBox_angleType->currentText()));
    ProjectConfig::setMetricUnit(getUnitTypeEnum(this->ui->comboBox_distanceType->currentText()));
    ProjectConfig::setTemperatureUnit(getUnitTypeEnum(this->ui->comboBox_temperatureType->currentText()));
    ProjectConfig::setUseSounds(this->ui->checkBox_sounds->isChecked());
    ProjectConfig::setAutoSaveInterval(this->ui->lineEdit_autoSaveInterval->text().toInt());
    // and save ProejctConfig file
    ProjectConfig::saveProjectPathConfigFile();

    emit this->setDisplayConfig(config);

    this->close();
}

void SettingsDialog::on_pushButton_cancel_clicked()
{
     this->close();
}

void SettingsDialog::on_lineEdit_angleDigits_textChanged(const QString &text)
{
    ui->pushButton_ok->setEnabled(text.toInt() > 0);
}

void SettingsDialog::on_lineEdit_distanceDigits_textChanged(const QString &text)
{
    ui->pushButton_ok->setEnabled(text.toInt() > 0);
}

void SettingsDialog::on_lineEdit_temperatureDigits_textChanged(const QString &text)
{
    ui->pushButton_ok->setEnabled(text.toInt() > 0);
}
