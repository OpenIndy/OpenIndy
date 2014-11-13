#include "measurementconfigdialog.h"
#include "ui_measurementconfigdialog.h"

/*!
 * \brief MeasurementConfigDialog::MeasurementConfigDialog
 * \param parent
 */
MeasurementConfigDialog::MeasurementConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeasurementConfigDialog)
{
    ui->setupUi(this);

    initGUI();
}

/*!
 * \brief MeasurementConfigDialog::~MeasurementConfigDialog
 */
MeasurementConfigDialog::~MeasurementConfigDialog()
{
    delete ui;
}

/*!
 * \brief MeasurementConfigDialog::on_pushButton_ok_clicked
 *
 */
void MeasurementConfigDialog::on_pushButton_ok_clicked(){

    //check if dialog entries are valid
    if(this->ui->comboBox_existingConfigs->currentText().compare("") == 0
            || this->ui->lineEdit_count->text().compare("") == 0
            || this->ui->lineEdit_iterations->text().compare("") == 0
            || this->ui->lineEdit_distanceInterval->text().compare("") == 0
            || this->ui->lineEdit_timeInterval->text().compare("") == 0){
        QMessageBox::information(NULL, "Measurement config invalid",
                                 "One of the fields is empty. Please specify each of the available fields.");
        return;
    }

    //create a measurement config object from dialog entries
    MeasurementConfig mConfig;
    mConfig.setName(this->ui->comboBox_existingConfigs->currentText());
    mConfig.count = this->ui->lineEdit_count->text().toInt();
    mConfig.iterations = this->ui->lineEdit_iterations->text().toInt();
    mConfig.timeInterval = this->ui->lineEdit_timeInterval->text().toLong();
    mConfig.distanceInterval = this->ui->lineEdit_distanceInterval->text().toDouble();
    mConfig.distanceDependent = this->ui->checkBox_distanceDependent->isChecked();
    mConfig.timeDependent = this->ui->checkBox_timeDependent->isChecked();
    mConfig.measureTwoSides = this->ui->checkBox_fsbs->isChecked();
    mConfig.typeOfReading = (Configuration::ReadingTypes)this->ui->comboBox_typeOfReading->currentText().toInt();

    //send created config to OiConfigState
    if(!OiConfigState::setMeasurementConfig(OiFeatureState::getActiveFeature(), mConfig)){
        QMessageBox::information(NULL, "Measurement config invalid",
                                 "Unable to set the measurement config for the active feature. Maybe there is another "
                                 "measurement config with the same name but different parameters which has been used yet.");
    }

    this->close();


    //this->ui->comboBox_existingConfigs->insertItem(0, ui->comboBox_existingConfigs->currentText());

    /*mConfig.name = ui->lineEdit_configName->text();
    mConfig.count = ui->lineEdit_count->text().toInt();
    mConfig.iterations = ui->lineEdit_iterations->text().toInt();
    mConfig.measureTwoSides = ui->checkBox_fsbs->isChecked();
    mConfig.typeOfReading = static_cast<Configuration::ReadingTypes>(ui->comboBox_typeOfReading->itemData(ui->comboBox_typeOfReading->currentIndex()).toInt());
    mConfig.distanceDependent = ui->checkBox_distanceDependent->isChecked();
    mConfig.timeDependent = ui->checkBox_timeDependent->isChecked();
    mConfig.distanceInterval = ui->lineEdit_distanceInterval->text().toDouble();
    mConfig.timeInterval = ui->lineEdit_timeInterval->text().toLong();

    if(mConfig.timeDependent == true && mConfig.distanceDependent == true){

        QMessageBox msgBox;
        msgBox.setText("You can only select time dependent or distance dependent. If you choose nothing, a normal measurement will be done");
        msgBox.setInformativeText("Change your config to continue.");
        //msgBox.setStandardButtons(QMessageBox::Accepted);
        //msgBox.setDefaultButton(QMessageBox::Accepted);
        int ret = msgBox.exec();

        return;
    }

    emit sendConfig(OiFeatureState::getActiveFeature(), mConfig);
    this->close();*/
}

/*!
 * \brief MeasurementConfigDialog::on_pushButton_cancel_clicked
 */
void MeasurementConfigDialog::on_pushButton_cancel_clicked(){
    this->close();
}

/*!
 * \brief MeasurementConfigDialog::receiveConfig
 * Receives the last measurement configuration setup from the controller, and sets the values to the GUI elements.
 *\param mC
 */
void MeasurementConfigDialog::receiveConfig(MeasurementConfig mC){
    //TODO mit sensor funktionswerten abgleichen
    /*this->mConfig = mC;

    ui->lineEdit_configName->setText(mConfig.name);
    ui->lineEdit_count->setText(QString::number(mConfig.count));
    ui->lineEdit_iterations->setText(QString::number(mConfig.iterations));
    ui->checkBox_fsbs->setChecked(this->mConfig.measureTwoSides);
    initGUI();
    ui->checkBox_distanceDependent->setChecked(this->mConfig.distanceDependent);
    ui->checkBox_timeDependent->setChecked(this->mConfig.timeDependent);
    ui->lineEdit_distanceInterval->setText(QString::number(this->mConfig.distanceInterval));
    ui->lineEdit_timeInterval->setText(QString::number(this->mConfig.timeInterval));*/
}

/*!
 * \brief MeasurementConfigDialog::initGUI
 */
void MeasurementConfigDialog::initGUI(){

    //set validators for each dialog field
    QRegExp regExpInt("^[1-9][0-9]*$");
    QRegExp regExpDouble("^-?(0|[1-9]{1}[0-9]{0,9})(\\.[0-9]+)?$");
    this->ui->lineEdit_count->setValidator(new QRegExpValidator(regExpInt, this->ui->lineEdit_count));
    this->ui->lineEdit_iterations->setValidator(new QRegExpValidator(regExpInt, this->ui->lineEdit_iterations));
    this->ui->lineEdit_timeInterval->setValidator(new QRegExpValidator(regExpInt, this->ui->lineEdit_timeInterval));
    this->ui->lineEdit_distanceInterval->setValidator(new QRegExpValidator(regExpDouble, this->ui->lineEdit_distanceInterval));

    //set model of available measurement configs
    this->ui->comboBox_existingConfigs->setModel(OiConfigState::getMeasurementConfigNames());

    //set default measurement config

    //set available reading types



    if(OiFeatureState::getActiveStation() != NULL && OiFeatureState::getActiveStation()->sensorPad->instrument != NULL){
        if(OiFeatureState::getActiveStation()->sensorPad->instrument->getSupportedReadingTypes() != NULL){

            ui->comboBox_typeOfReading->clear();

            QList<Configuration::ReadingTypes> readingTypes = *OiFeatureState::getActiveStation()->sensorPad->instrument->getSupportedReadingTypes();
            for(int i=0; i< readingTypes.size();i++){
                switch (readingTypes.at(i)) {
                case Configuration::ePolar:
                    ui->comboBox_typeOfReading->insertItem(ui->comboBox_typeOfReading->count(),Configuration::sPolar,Configuration::ePolar);
                    break;
                case Configuration::eCartesian:
                    ui->comboBox_typeOfReading->insertItem(ui->comboBox_typeOfReading->count(),Configuration::sCartesian,Configuration::eCartesian);
                    break;
                case Configuration::eDirection:
                    ui->comboBox_typeOfReading->insertItem(ui->comboBox_typeOfReading->count(),Configuration::sDirection,Configuration::eDirection);
                    break;
                case Configuration::eDistance:
                    ui->comboBox_typeOfReading->insertItem(ui->comboBox_typeOfReading->count(),Configuration::sDistance,Configuration::eDistance);
                    break;
                case Configuration::eTemperatur:
                    ui->comboBox_typeOfReading->insertItem(ui->comboBox_typeOfReading->count(),Configuration::sTemperatur,Configuration::eTemperatur);
                    break;
                case Configuration::eLevel:
                    ui->comboBox_typeOfReading->insertItem(ui->comboBox_typeOfReading->count(),Configuration::sLevel,Configuration::eLevel);
                    break;
                case Configuration::eUndefined:
                    ui->comboBox_typeOfReading->insertItem(ui->comboBox_typeOfReading->count(),OiFeatureState::getActiveStation()->sensorPad->instrument->getUndefinedReadingName(),Configuration::eUndefined);
                    break;
                default:
                    break;
                }
            }
            if(this->mConfig.typeOfReading == -1){
                ui->comboBox_typeOfReading->setCurrentIndex(0);
            }else{
                ui->comboBox_typeOfReading->setCurrentIndex(ui->comboBox_typeOfReading->findData(mConfig.typeOfReading));
            }
        }else{
            Console::addLine("Sensor does not support any reading types.");
        }
    }else{
        QString currentText = ui->comboBox_typeOfReading->currentText();
        ui->comboBox_typeOfReading->clear();
        ui->comboBox_typeOfReading->insertItem(ui->comboBox_typeOfReading->count(),Configuration::sPolar,Configuration::ePolar);
        ui->comboBox_typeOfReading->insertItem(ui->comboBox_typeOfReading->count(),Configuration::sCartesian,Configuration::eCartesian);
        ui->comboBox_typeOfReading->insertItem(ui->comboBox_typeOfReading->count(),Configuration::sDirection,Configuration::eDirection);
        ui->comboBox_typeOfReading->insertItem(ui->comboBox_typeOfReading->count(),Configuration::sDistance,Configuration::eDistance);
        ui->comboBox_typeOfReading->insertItem(ui->comboBox_typeOfReading->count(),Configuration::sTemperatur,Configuration::eTemperatur);
        ui->comboBox_typeOfReading->insertItem(ui->comboBox_typeOfReading->count(),Configuration::sLevel,Configuration::eLevel);
        int idx = ui->comboBox_typeOfReading->findText(currentText);
        if(idx != -1){
            ui->comboBox_typeOfReading->setCurrentIndex(idx);
        }
    }
}

/*!
 * \brief MeasurementConfigDialog::getCode
 * function to search a String in a specified combobox. The return value is a integer.
 * \param cb
 * \param label
 * \return
 */
int MeasurementConfigDialog::getCode(QComboBox *cb, QString label){
    return cb->findText(label);
}

/*!
 * \brief MeasurementConfigDialog::getLabel
 * function to search a index in a specified combobox and returns the corresponding String.
 * \param cb
 * \param code
 * \return
 */
QString MeasurementConfigDialog::getLabel(QComboBox *cb, int code){
    return cb->itemText(cb->findData(code));
}

/*!
 * \brief MeasurementConfigDialog::showEvent
 * \param event
 */
void MeasurementConfigDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    //set default measurement config as selected config in the dialog

    event->accept();
}

/*!
 * \brief overwritten closeEvent
 * \param event
 */
void MeasurementConfigDialog::closeEvent(QCloseEvent *event){
    event->accept();
}

/*!
 * \brief MeasurementConfigDialog::on_comboBox_existingConfigs_currentIndexChanged
 * \param arg1
 */
void MeasurementConfigDialog::on_comboBox_existingConfigs_currentIndexChanged(const QString &arg1){

    qDebug() << arg1;

}
