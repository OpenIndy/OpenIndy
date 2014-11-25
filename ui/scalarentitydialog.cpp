#include "scalarentitydialog.h"
#include "ui_scalarentitydialog.h"

ScalarEntityDialog::ScalarEntityDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::ScalarEntityDialog)
{
    ui->setupUi(this);
    initGUI();
}

ScalarEntityDialog::~ScalarEntityDialog()
{
    delete ui;
}

void ScalarEntityDialog::initGUI(){

    ui->lineEdit_name->setText("");
    ui->checkBox_actual->setChecked(true);
    ui->checkBox_nominal->setChecked(false);
    ui->comboBox_nominalSystem->setVisible(false);
    ui->label_nominalSystem->setVisible(false);
    ui->spinBox_count->setValue(1);

    ui->comboBox_scalarEntityType->clear();

    ui->comboBox_scalarEntityType->insertItem(ui->comboBox_scalarEntityType->count(),"scalar entity distance",Configuration::eScalarEntityDistanceFeature);
    ui->comboBox_scalarEntityType->insertItem(ui->comboBox_scalarEntityType->count(),"scalar entity angle",Configuration::eScalarEntityAngleFeature);
    ui->comboBox_scalarEntityType->insertItem(ui->comboBox_scalarEntityType->count(),"scalar entity temperature", Configuration::eScalarEntityTemperatureFeature);
    ui->comboBox_scalarEntityType->insertItem(ui->comboBox_scalarEntityType->count(),"scalar entity measurement series", Configuration::eScalarEntityMeasurementSeriesFeature);

    ui->comboBox_nominalSystem->clear();

    if(OiFeatureState::getFeatureCount() !=0){
        for(int i=0; i<OiFeatureState::getFeatures().size();i++){
            if(OiFeatureState::getFeatures().at(i)->getCoordinateSystem() != NULL){
                ui->comboBox_nominalSystem->addItem(OiFeatureState::getFeatures().at(i)->getCoordinateSystem()->getFeatureName());
            }

        }
    }
}

void ScalarEntityDialog::on_pushButton_cancel_clicked()
{
    this->close();
}

void ScalarEntityDialog::on_pushButton_ok_clicked()
{
    if(ui->spinBox_count->value()>0){
        QString name = ui->lineEdit_name->text();
        QString group = this->ui->comboBox_group->currentText();
        int count = ui->spinBox_count->value();
        Configuration::FeatureTypes featureType = static_cast<Configuration::FeatureTypes>(ui->comboBox_scalarEntityType->itemData(ui->comboBox_scalarEntityType->currentIndex()).toInt());
        bool actual = ui->checkBox_actual->isChecked();
        bool nominal = ui->checkBox_nominal->isChecked();
        bool comPoint = ui->checkBox_commonPoint->isChecked();
        CoordinateSystem *nominalSystem = NULL;
        QString function = this->ui->comboBox_function->currentText();

        if(nominal){
            for(int k=0; k<OiFeatureState::getFeatureCount();k++){
                if(OiFeatureState::getFeatures().at(k)->getCoordinateSystem() != NULL &&
                        ui->comboBox_nominalSystem->currentText() == OiFeatureState::getFeatures().at(k)->getCoordinateSystem()->getFeatureName()){
                    nominalSystem = OiFeatureState::getFeatures().at(k)->getCoordinateSystem();
                }
                if(OiFeatureState::getFeatures().at(k)->getStation() != NULL &&
                        ui->comboBox_nominalSystem->currentText() == OiFeatureState::getFeatures().at(k)->getStation()->getFeatureName()){
                    nominalSystem = OiFeatureState::getFeatures().at(k)->getStation()->coordSys;
                }
            }
        }

        FeatureAttributesExchange featureAttributes;
        featureAttributes.count = count;
        featureAttributes.featureType = featureType;
        featureAttributes.name = name;
        featureAttributes.group = group;
        featureAttributes.function = function;
        featureAttributes.actual = actual;
        featureAttributes.nominal = nominal;
        featureAttributes.common = comPoint;
        featureAttributes.nominalSystem = nominalSystem;

        emit createFeature(featureAttributes);

        this->close();
    }
}

void ScalarEntityDialog::on_checkBox_nominal_toggled(bool checked)
{
    if(checked){
        ui->comboBox_nominalSystem->setVisible(true);
        ui->label_nominalSystem->setVisible(true);
    }else{
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_nominalSystem->setVisible(false);
    }
}

void ScalarEntityDialog::on_comboBox_scalarEntityType_currentTextChanged(const QString &arg1)
{
    /*Configuration::FeatureTypes scalarEntityType = static_cast<Configuration::FeatureTypes>(ui->comboBox_scalarEntityType->itemData(ui->comboBox_scalarEntityType->currentIndex()).toInt());

    switch (scalarEntityType) {
    case Configuration::eScalarentityAngleFeature:
        ui->label_value->setText(QString("value " + UnitConverter::getAngleUnitString()));
        break;
    case Configuration::eScalarEntityDistanceFeature:
        ui->label_value->setText(QString("value " + UnitConverter::getDistanceUnitString()));
        break;
    case Configuration::eScalarEntityMeasurementSeriesFeature:
        ui->label_value->setText(QString("value"));
        break;
    case Configuration::eScalarEntityTemperatureFeature:
        ui->label_value->setText(QString("value " + UnitConverter::getTemperatureUnitString()));
        break;
    default:
        ui->label_value->setText("value");
        break;
    }*/
}

void ScalarEntityDialog::on_toolButton_mConfig_clicked()
{
    Configuration::FeatureTypes featureType = static_cast<Configuration::FeatureTypes>(ui->comboBox_scalarEntityType->itemData(ui->comboBox_scalarEntityType->currentIndex()).toInt());
    emit createFeatureMConfig(featureType);
}


void ScalarEntityDialog::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
    initGUI();
    event->accept();
}

/*!
 * \brief ScalarEntityDialog::availableGroupsChanged
 * Update group combobox
 * \param myGroups
 */
void ScalarEntityDialog::availableGroupsChanged(QStringList myGroups){
    this->ui->comboBox_group->clear();
    this->ui->comboBox_group->clearEditText();
    this->ui->comboBox_group->addItems(myGroups);
}

/*!
 * \brief ScalarEntityDialog::on_checkBox_actual_toggled
 * \param checked
 */
void ScalarEntityDialog::on_checkBox_actual_toggled(bool checked)
{
    if(checked){
        this->ui->comboBox_function->setEnabled(true);
    }else{
        this->ui->comboBox_function->setEnabled(false);
    }
}

/*!
 * \brief ScalarEntityDialog::setAvailableFunctions
 * Assign all available create functions to the function combo box and select default value
 * \param functions
 * \param defaultFunction
 */
void ScalarEntityDialog::setAvailableFunctions(QStringList functions, QString defaultFunction){
    this->ui->comboBox_function->clear();
    this->ui->comboBox_function->addItems(functions);
    this->ui->comboBox_function->setCurrentText(defaultFunction);
}
