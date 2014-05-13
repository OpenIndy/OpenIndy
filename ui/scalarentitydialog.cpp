#include "scalarentitydialog.h"
#include "ui_scalarentitydialog.h"

ScalarEntityDialog::ScalarEntityDialog(QList<FeatureWrapper*> &features, QWidget *parent) :
    QDialog(parent),featureList(features),
    ui(new Ui::ScalarEntityDialog)
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
    ui->comboBox_scalarEntityType->insertItem(ui->comboBox_scalarEntityType->count(),"scalar entity angle",Configuration::eScalarentityAngleFeature);
    ui->comboBox_scalarEntityType->insertItem(ui->comboBox_scalarEntityType->count(),"scalar entity temperature", Configuration::eScalarEntityTemperatureFeature);
    ui->comboBox_scalarEntityType->insertItem(ui->comboBox_scalarEntityType->count(),"scalar entity measurement series", Configuration::eScalarEntityMeasurementSeriesFeature);

    ui->comboBox_nominalSystem->clear();

    if(featureList.size() !=0){
        for(int i=0; i<featureList.size();i++){
            if(featureList.at(i)->getCoordinateSystem() != NULL){
                ui->comboBox_nominalSystem->addItem(featureList.at(i)->getCoordinateSystem()->name);
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
        int featureType = static_cast<Configuration::FeatureTypes>(ui->comboBox_scalarEntityType->itemData(ui->comboBox_scalarEntityType->currentIndex()).toInt());
        bool actual = ui->checkBox_actual->isChecked();
        bool nominal = ui->checkBox_nominal->isChecked();
        bool comPoint = ui->checkBox_commonPoint->isChecked();
        CoordinateSystem *nominalSystem = NULL;

        if(nominal){
            for(int k=0; k<this->featureList.size();k++){
                if(this->featureList.at(k)->getCoordinateSystem() != NULL &&
                        ui->comboBox_nominalSystem->currentText() == this->featureList.at(k)->getCoordinateSystem()->name){
                    nominalSystem = this->featureList.at(k)->getCoordinateSystem();
                }
                if(this->featureList.at(k)->getStation() != NULL &&
                        ui->comboBox_nominalSystem->currentText() == this->featureList.at(k)->getStation()->name){
                    nominalSystem = this->featureList.at(k)->getStation()->coordSys;
                }
            }
        }

        FeatureAttributesExchange featureAttributes(count,featureType,name,group,actual,nominal,comPoint,nominalSystem);

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
    emit createFeatureMConfig();
}


void ScalarEntityDialog::showEvent(QShowEvent *event)
{
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
