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
}

/*!
 * \brief MeasurementConfigDialog::~MeasurementConfigDialog
 */
MeasurementConfigDialog::~MeasurementConfigDialog()
{
    delete ui;
}

/*!
 * \brief MeasurementConfigDialog::setMeasurementConfig
 * \param mConfig
 */
void MeasurementConfigDialog::setMeasurementConfig(MeasurementConfig mConfig){
    this->mConfig = mConfig;
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
            || (this->ui->lineEdit_distanceInterval->text().compare("") == 0 && this->ui->checkBox_distanceDependent->isChecked())
            || (this->ui->lineEdit_timeInterval->text().compare("") == 0 && this->ui->checkBox_timeDependent->isChecked())){
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
    mConfig.typeOfReading = Configuration::getReadingTypeEnum(this->ui->comboBox_typeOfReading->currentText());

    //send created config to OiConfigState
    if(!OiConfigState::addMeasurementConfig(mConfig)){
        QMessageBox::information(NULL, "Measurement config invalid",
                                 "The selected measurement config is invalid.");
    }

    emit this->measurementConfigSelected(mConfig);

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
    emit this->measurementConfigSelected(MeasurementConfig());
    this->close();
}

/*!
 * \brief MeasurementConfigDialog::receiveConfig
 * Receives the last measurement configuration setup from the controller, and sets the values to the GUI elements.
 *\param mC
 */
/*void MeasurementConfigDialog::receiveConfig(MeasurementConfig mC){
    //TODO mit sensor funktionswerten abgleichen
    this->mConfig = mC;

    ui->lineEdit_configName->setText(mConfig.name);
    ui->lineEdit_count->setText(QString::number(mConfig.count));
    ui->lineEdit_iterations->setText(QString::number(mConfig.iterations));
    ui->checkBox_fsbs->setChecked(this->mConfig.measureTwoSides);
    initGUI();
    ui->checkBox_distanceDependent->setChecked(this->mConfig.distanceDependent);
    ui->checkBox_timeDependent->setChecked(this->mConfig.timeDependent);
    ui->lineEdit_distanceInterval->setText(QString::number(this->mConfig.distanceInterval));
    ui->lineEdit_timeInterval->setText(QString::number(this->mConfig.timeInterval));
}*/

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

    //check if active feature exists and is a geometry
    /*if(OiFeatureState::getActiveFeature() == NULL
            || OiFeatureState::getActiveFeature()->getGeometry() == NULL){
        return;
    }*/

    //set default measurement config
    /*MeasurementConfig mConfig;
    if(OiFeatureState::getActiveFeature()->getGeometry()->getMeasurementConfig().getIsValid()){
        mConfig = OiFeatureState::getActiveFeature()->getGeometry()->getMeasurementConfig();
    }else{
        switch(OiFeatureState::getActiveFeature()->getTypeOfFeature()){
        case Configuration::ePointFeature:
            qDebug() << Point::defaultMeasurementConfig.getName();
            mConfig = Point::defaultMeasurementConfig;
            break;
        case Configuration::eLineFeature:
            mConfig = Line::defaultMeasurementConfig;
            break;
        case Configuration::ePlaneFeature:
            mConfig = Plane::defaultMeasurementConfig;
            break;
        case Configuration::ePointCloudFeature:
            mConfig = PointCloud::defaultMeasurementConfig;
            break;
        case Configuration::eCircleFeature:
            mConfig = Circle::defaultMeasurementConfig;
            break;
        case Configuration::eConeFeature:
            mConfig = Cone::defaultMeasurementConfig;
            break;
        case Configuration::eHyperboloidFeature:
            mConfig = Hyperboloid::defaultMeasurementConfig;
            break;
        case Configuration::eParaboloidFeature:
            mConfig = Paraboloid::defaultMeasurementConfig;
            break;
        case Configuration::eEllipsoidFeature:
            mConfig = Ellipsoid::defaultMeasurementConfig;
            break;
        case Configuration::eScalarEntityAngleFeature:
            mConfig = ScalarEntityAngle::defaultMeasurementConfig;
            break;
        case Configuration::eScalarEntityDistanceFeature:
            mConfig = ScalarEntityDistance::defaultMeasurementConfig;
            break;
        case Configuration::eScalarEntityMeasurementSeriesFeature:
            mConfig = ScalarEntityMeasurementSeries::defaultMeasurementConfig;
            break;
        case Configuration::eScalarEntityTemperatureFeature:
            mConfig = ScalarEntityTemperature::defaultMeasurementConfig;
            break;
        case Configuration::eNurbsFeature:
            mConfig = Nurbs::defaultMeasurementConfig;
            break;
        case Configuration::eSphereFeature:
            mConfig = Sphere::defaultMeasurementConfig;
            break;
        case Configuration::eCylinderFeature:
            mConfig = Cylinder::defaultMeasurementConfig;
            break;
        case Configuration::eStationFeature:
            //mConfig = Station::defaultMeasurementConfig;
            break;
        }
    }*/

    //set default values for GUI elements
    if(mConfig.getIsValid()){
        this->ui->comboBox_existingConfigs->setCurrentText(mConfig.getDisplayName());
        this->ui->lineEdit_count->setText(QString::number(mConfig.count));
        this->ui->lineEdit_iterations->setText(QString::number(mConfig.iterations));
        this->ui->checkBox_distanceDependent->setChecked(mConfig.distanceDependent);
        this->ui->checkBox_timeDependent->setChecked(mConfig.timeDependent);
        this->ui->lineEdit_distanceInterval->setText(QString::number(mConfig.distanceInterval));
        this->ui->lineEdit_timeInterval->setText(QString::number(mConfig.timeInterval));
    }else{
        this->ui->comboBox_existingConfigs->setCurrentText("");
        this->ui->lineEdit_count->setText("");
        this->ui->lineEdit_iterations->setText("");
        this->ui->checkBox_distanceDependent->setChecked(false);
        this->ui->checkBox_timeDependent->setChecked(false);
        this->ui->lineEdit_distanceInterval->setText("");
        this->ui->lineEdit_timeInterval->setText("");
    }

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
/*int MeasurementConfigDialog::getCode(QComboBox *cb, QString label){
    return cb->findText(label);
}*/

/*!
 * \brief MeasurementConfigDialog::getLabel
 * function to search a index in a specified combobox and returns the corresponding String.
 * \param cb
 * \param code
 * \return
 */
/*QString MeasurementConfigDialog::getLabel(QComboBox *cb, int code){
    return cb->itemText(cb->findData(code));
}*/

/*!
 * \brief MeasurementConfigDialog::showEvent
 * \param event
 */
void MeasurementConfigDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    this->initGUI();

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
 * \param configName
 */
void MeasurementConfigDialog::on_comboBox_existingConfigs_currentIndexChanged(const QString &configName){

    //get selected measurement config
    MeasurementConfig mConfig = OiConfigState::getMeasurementConfig(configName);

    //set GUI elements to hold the selected measurement config
    this->ui->comboBox_existingConfigs->setCurrentText(mConfig.getDisplayName());
    this->ui->lineEdit_count->setText(QString::number(mConfig.count));
    this->ui->lineEdit_iterations->setText(QString::number(mConfig.iterations));
    this->ui->checkBox_distanceDependent->setChecked(mConfig.distanceDependent);
    this->ui->checkBox_timeDependent->setChecked(mConfig.timeDependent);
    this->ui->lineEdit_distanceInterval->setText(QString::number(mConfig.distanceInterval));
    this->ui->lineEdit_timeInterval->setText(QString::number(mConfig.timeInterval));

}
