#include "settingsdialog.h"
#include "ui_settingsdialog.h"
/*!
 * \brief SettingsDialog::on_pushButton_ok_clicked
 */
void SettingsDialog::on_pushButton_ok_clicked()
{
    this->close();
}

/*!
 * \brief SettingsDialog::SettingsDialog
 * \param parent
 */
SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::SettingsDialog)
{
    this->ui->setupUi(this);

    //init GUI elements and assign models
    this->initModels();
    this->initGUI();
    this->updateDisplayConfigFromSelection(); //no signal emit in constructor call!!!
}

/*!
 * \brief SettingsDialog::~SettingsDialog
 */
SettingsDialog::~SettingsDialog()
{
}

/*!
 * \brief SettingsDialog::on_comboBox_angleType_currentIndexChanged
 * \param arg1
 */
void SettingsDialog::on_comboBox_angleType_currentIndexChanged(const QString &arg1){
    this->updateDisplayConfigFromSelection();
}

/*!
 * \brief SettingsDialog::on_comboBox_distanceType_currentIndexChanged
 * \param arg1
 */
void SettingsDialog::on_comboBox_distanceType_currentIndexChanged(const QString &arg1){
    this->updateDisplayConfigFromSelection();
}

/*!
 * \brief SettingsDialog::on_comboBox_temperatureType_currentIndexChanged
 * \param arg1
 */
void SettingsDialog::on_comboBox_temperatureType_currentIndexChanged(const QString &arg1){
    this->updateDisplayConfigFromSelection();
}

/*!
 * \brief SettingsDialog::on_lineEdit_angleDigits_textChanged
 * \param arg1
 */
void SettingsDialog::on_lineEdit_angleDigits_textChanged(const QString &arg1){
    this->updateDisplayConfigFromSelection();
}

/*!
 * \brief SettingsDialog::on_lineEdit_distanceDigits_textChanged
 * \param arg1
 */
void SettingsDialog::on_lineEdit_distanceDigits_textChanged(const QString &arg1){
    this->updateDisplayConfigFromSelection();
}

/*!
 * \brief SettingsDialog::on_lineEdit_temperatureDigits_textChanged
 * \param arg1
 */
void SettingsDialog::on_lineEdit_temperatureDigits_textChanged(const QString &arg1){
    this->updateDisplayConfigFromSelection();
}

void SettingsDialog::on_lineEdit_autoSaveInterval_textChanged(const QString &arg1){
    this->updateDisplayConfigFromSelection();
}

/*!
 * \brief SettingsDialog::showEvent
 * \param event
 */
void SettingsDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
    this->ui->tabWidget_settings->setTabEnabled(2,false);
    this->ui->tabWidget_settings->setTabEnabled(3,false);
    this->initGUI();
    this->updateDisplayConfigFromSelection();
}

/*!
 * \brief SettingsDialog::initGUI
 */
void SettingsDialog::initGUI(){

    //do not trigger changes while settings defaults
    this->ui->comboBox_angleType->blockSignals(true);
    this->ui->comboBox_distanceType->blockSignals(true);
    this->ui->comboBox_temperatureType->blockSignals(true);
    this->ui->lineEdit_angleDigits->blockSignals(true);
    this->ui->lineEdit_distanceDigits->blockSignals(true);
    this->ui->lineEdit_temperatureDigits->blockSignals(true);
    this->ui->checkBox_sounds->blockSignals(true);
    this->ui->lineEdit_autoSaveInterval->blockSignals(true);

    //set default unit
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
void SettingsDialog::updateDisplayConfigFromSelection(){

    ParameterDisplayConfig config;
    config.setDisplayDigits(eAngular, this->ui->lineEdit_angleDigits->text().toInt());
    config.setDisplayDigits(eMetric, this->ui->lineEdit_distanceDigits->text().toInt());
    config.setDisplayDigits(eTemperature, this->ui->lineEdit_temperatureDigits->text().toInt());
    config.setDisplayUnitType(eAngular, getUnitTypeEnum(this->ui->comboBox_angleType->currentText()));
    config.setDisplayUnitType(eMetric, getUnitTypeEnum(this->ui->comboBox_distanceType->currentText()));
    config.setDisplayUnitType(eTemperature, getUnitTypeEnum(this->ui->comboBox_temperatureType->currentText()));
    config.setUseSounds(this->ui->checkBox_sounds->isChecked());

    if(this->isVisible()){//online save the changes after editing in GUI

        ProjectConfig::setAngularDigits(this->ui->lineEdit_angleDigits->text().toInt());
        ProjectConfig::setDistanceDigits(this->ui->lineEdit_distanceDigits->text().toInt());
        ProjectConfig::setTemperatureDigits(this->ui->lineEdit_temperatureDigits->text().toInt());
        ProjectConfig::setAngularUnit(getUnitTypeEnum(this->ui->comboBox_angleType->currentText()));
        ProjectConfig::setMetricUnit(getUnitTypeEnum(this->ui->comboBox_distanceType->currentText()));
        ProjectConfig::setTemperatureUnit(getUnitTypeEnum(this->ui->comboBox_temperatureType->currentText()));
        ProjectConfig::setUseSounds(this->ui->checkBox_sounds->isChecked());
        ProjectConfig::setAutoSaveInterval(this->ui->lineEdit_autoSaveInterval->text().toInt());
    }

    emit this->setDisplayConfig(config);
}

/*!
 * \brief SettingsDialog::on_checkBox_sounds_toggled
 * \param checked
 */
void SettingsDialog::on_checkBox_sounds_toggled(bool checked)
{
    this->updateDisplayConfigFromSelection();
}
