#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    initGUI();
    featureAttrLayout = new QVBoxLayout();
    trafoParamAttrLayout = new QVBoxLayout();
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
    this->ui->treeView_plugins->expandToDepth(1);
}

void SettingsDialog::on_pushButton_ok_clicked()
{

    //first call generate, so they get the new unit string, if changed.
    saveSettings();
    GUIConfiguration::generateAllAttributes();
    GUIConfiguration::generateFeatureAttributes();
    GUIConfiguration::generateTrafoParamAttributes();
    //then they get the boolean for display or not.
    getFeatureColumns();
    destructFeatureColumns();
    getTrafoParamColumns();
    destructTrafoParamColumns();
    this->close();
}

void SettingsDialog::on_pushButton_cancel_clicked()
{
    destructFeatureColumns();
    destructTrafoParamColumns();
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

    displayFeatureColumns();
    displayTrafoParamColumns();
    event->accept();
}

void SettingsDialog::closeEvent(QCloseEvent *event)
{
    destructFeatureColumns();
    destructTrafoParamColumns();
    emit modelChanged();
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

/*!
 * \brief displayColumns shows all available attribute columns for feature view as a checkbox.
 */
void SettingsDialog::displayFeatureColumns()
{
    for(int i=0;i<GUIConfiguration::featureAttributes.size();i++){
        QCheckBox *cb = new QCheckBox();
        cb->setText(GUIConfiguration::featureAttributes.at(i)->attrName);
        cb->setChecked(GUIConfiguration::featureAttributes.at(i)->displayState);
        featureCheckbox.append(cb);
    }

    int layoutCount = 0;
    if(featureCheckbox.size()%3 == 0){
        layoutCount = featureCheckbox.size()/3;
    }else{
        layoutCount = (featureCheckbox.size()/3)+1;
    }

    for(int i=0;i<layoutCount;i++){

        QHBoxLayout *layout = new QHBoxLayout();
        //layout->setStretch(1,1,1);
        featureLayouts.append(layout);
        featureAttrLayout->addLayout(layout);
    }

    int j=-1;
    for(int k=0;k<featureCheckbox.size();k++){

        if(k%3==0){
            j+=1;
        }
        featureLayouts.at(j)->addWidget(featureCheckbox.at(k));

    }

    ui->tab_featureAttributes->setLayout(featureAttrLayout);
}

/*!
 * \brief displayTrafoParamColumns shows all available attributes for trafoParam view.
 */
void SettingsDialog::displayTrafoParamColumns()
{
    for(int i=0;i<GUIConfiguration::trafoParamAttributes.size();i++){
        QCheckBox *cb = new QCheckBox();
        cb->setText(GUIConfiguration::trafoParamAttributes.at(i)->attrName);
        cb->setChecked(GUIConfiguration::trafoParamAttributes.at(i)->displayState);
        trafoParamCheckbox.append(cb);
    }

    int layoutCount = 0;
    if(trafoParamCheckbox.size()%3 == 0){
        layoutCount = trafoParamCheckbox.size()/3;
    }else{
        layoutCount = (trafoParamCheckbox.size()/3)+1;
    }

    for(int i=0;i<layoutCount;i++){

        QHBoxLayout *layout = new QHBoxLayout();
        //layout->setStretch(1,1,1);
        trafoParamLayouts.append(layout);
        trafoParamAttrLayout->addLayout(layout);
    }

    int j=-1;
    for(int k=0;k<trafoParamCheckbox.size();k++){

        if(k%3==0){
            j+=1;
        }
        trafoParamLayouts.at(j)->addWidget(trafoParamCheckbox.at(k));

    }

    ui->tab_TrafoParamAttributes->setLayout(trafoParamAttrLayout);
}

/*!
 * \brief getFeatureColumns checks if the attribute should be displayed or not. It saves the changes in the configuration class.
 * Order of both lists is the same, so you can directly set the boolean without checking for a equal name.
 */
void SettingsDialog::getFeatureColumns()
{
    for(int i =0; i<featureCheckbox.size();i++){
        GUIConfiguration::featureAttributes.at(i)->displayState = featureCheckbox.at(i)->isChecked();
    }
}

/*!
 * \brief getTrafoParamColumns checks if the attribute should be displayed or not. It saves the changes in the configuration class.
 * Order of both lists is the same, so you can directly set the boolean without checking for a equal name.
 */
void SettingsDialog::getTrafoParamColumns()
{
    for(int i=0;i<trafoParamCheckbox.size();i++){
        GUIConfiguration::trafoParamAttributes.at(i)->displayState = trafoParamCheckbox.at(i)->isChecked();
    }
}

/*!
 * \brief destructFeatureColumns destructs the dynamic gui for displaying the available feature attribute columns.
 */
void SettingsDialog::destructFeatureColumns()
{
    //delete all checkboxes
   for(int k=0; k<featureCheckbox.size();k++){
        ui->tab_featureAttributes->layout()->removeWidget(featureCheckbox.at(k));
        delete featureCheckbox.at(k);
    }
    featureCheckbox.clear();

    //delete all layouts
    for(int i=0; i<featureLayouts.size();i++){
        ui->tab_featureAttributes->layout()->removeItem(featureLayouts.at(i));
        delete featureLayouts.at(i);
    }
    featureLayouts.clear();
}

/*!
 * \brief destructTrafoParamColumns destructs the dynamic gui for displaying the available trafo parameter attribute columns.
 */
void SettingsDialog::destructTrafoParamColumns()
{
    //delete all checkboxes
   for(int k=0; k<trafoParamCheckbox.size();k++){
        ui->tab_TrafoParamAttributes->layout()->removeWidget(trafoParamCheckbox.at(k));
        delete trafoParamCheckbox.at(k);
    }
    trafoParamCheckbox.clear();

    //delete all layouts
    for(int i=0; i<trafoParamLayouts.size();i++){
        ui->tab_TrafoParamAttributes->layout()->removeItem(trafoParamLayouts.at(i));
        delete trafoParamLayouts.at(i);
    }
    trafoParamLayouts.clear();
}
