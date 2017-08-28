#include "settingsdialog.h"
#include "ui_settingsdialog.h"

/*

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    QGridLayout *pluginInfoLayout = new QGridLayout();
    pluginInfoLayout->setContentsMargins(0,0,0,0);
    this->ui->widget_pluginInfo->setLayout(pluginInfoLayout);
    this->myPluginInformation = new PluginInfoWidget();
    pluginInfoLayout->addWidget(this->myPluginInformation);

    initGUI();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setPluginsModel(PluginTreeViewModel *model){
    this->ui->treeView_plugins->setModel(model);
    this->ui->treeView_plugins->expandToDepth(1);
}
*/
void SettingsDialog::on_pushButton_ok_clicked()
{
/*
    //first call generate, so they get the new unit string, if changed.
    saveSettings();
    //! generate lists for gui and view modification
    GUIConfiguration::generateLists();

    //then they get the boolean for display or not.
    getFeatureColumns();
    getTrafoParamColumns();*/
    this->close();
}
/*
void SettingsDialog::initGUI(){

    this->ui->checkBox_differences->setChecked(false);
    this->ui->checkBox_differences->setToolTip("show differences between actual and nominal feature");

    ui->comboBox_angleType->insertItem(ui->comboBox_angleType->count(),"decimal degree",OiUnitConverter::eDecimalDegree);
    ui->comboBox_angleType->insertItem(ui->comboBox_angleType->count(),"gon",OiUnitConverter::eGon);
    ui->comboBox_angleType->insertItem(ui->comboBox_angleType->count(),"radiant",OiUnitConverter::eRadiant);
    ui->comboBox_angleType->insertItem(ui->comboBox_angleType->count(),"arc seconds",OiUnitConverter::eArcSeconds);
    ui->comboBox_angleType->insertItem(ui->comboBox_angleType->count(),"mrad",OiUnitConverter::eMilliRadians);

    ui->comboBox_distanceType->insertItem(ui->comboBox_distanceType->count(),"meter",OiUnitConverter::eMeter);
    ui->comboBox_distanceType->insertItem(ui->comboBox_distanceType->count(),"millimeter",OiUnitConverter::eMillimeter);
    ui->comboBox_distanceType->insertItem(ui->comboBox_distanceType->count(),"inch",OiUnitConverter::eInch);

    ui->comboBox_temperatureType->insertItem(ui->comboBox_temperatureType->count(),"degree",OiUnitConverter::eGRAD);
    ui->comboBox_temperatureType->insertItem(ui->comboBox_temperatureType->count(),"Kelvin",OiUnitConverter::eKelvin);
    ui->comboBox_temperatureType->insertItem(ui->comboBox_temperatureType->count(),"Fahrenheit",OiUnitConverter::eFahrenheit);

    ui->lineEdit_angleDigits->setText(QString::number(OiUnitConverter::angleDigits));
    ui->lineEdit_distanceDigits->setText(QString::number(OiUnitConverter::distanceDigits));
    ui->lineEdit_temperatureDigits->setText(QString::number(OiUnitConverter::temperatureDigits));

}

void SettingsDialog::showEvent(QShowEvent *event){
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    ui->lineEdit_angleDigits->setText(QString::number(OiUnitConverter::angleDigits));
    ui->lineEdit_distanceDigits->setText(QString::number(OiUnitConverter::distanceDigits));
    ui->lineEdit_temperatureDigits->setText(QString::number(OiUnitConverter::temperatureDigits));

    ui->comboBox_angleType->setCurrentIndex(ui->comboBox_angleType->findData(OiUnitConverter::angleType));
    ui->comboBox_distanceType->setCurrentIndex(ui->comboBox_distanceType->findData(OiUnitConverter::distanceType));
    ui->comboBox_temperatureType->setCurrentIndex(ui->comboBox_temperatureType->findData(OiUnitConverter::temperatureType));

    ui->checkBox_differences->setChecked(GUIConfiguration::getShowDifferences());

    displayFeatureColumns();
    displayTrafoParamColumns();
    event->accept();
}

void SettingsDialog::closeEvent(QCloseEvent *event)
{
    emit modelChanged();

    emit changedColumnOrder();

    event->accept();
}

void SettingsDialog::saveSettings(){

    OiUnitConverter::angleDigits = ui->lineEdit_angleDigits->text().toInt();
    OiUnitConverter::distanceDigits = ui->lineEdit_distanceDigits->text().toInt();
    OiUnitConverter::temperatureDigits = ui->lineEdit_temperatureDigits->text().toInt();
    OiUnitConverter::setAngleUnit(static_cast<OiUnitConverter::UnitType>(ui->comboBox_angleType->itemData(ui->comboBox_angleType->currentIndex()).toInt()));
    OiUnitConverter::setDistanceUnit(static_cast<OiUnitConverter::UnitType>(ui->comboBox_distanceType->itemData(ui->comboBox_distanceType->currentIndex()).toInt()));
    OiUnitConverter::setTemperatureUnit(static_cast<OiUnitConverter::UnitType>(ui->comboBox_temperatureType->itemData(ui->comboBox_temperatureType->currentIndex()).toInt()));

    GUIConfiguration::setShowDifferences(this->ui->checkBox_differences->isChecked());
}

void SettingsDialog::displayFeatureColumns()
{
    allFeatureAttributes.clear();
    displayedFeatureAttributes.clear();

    if(GUIConfiguration::userDefFeatOrder.size() > 0){
        displayedFeatureAttributes = GUIConfiguration::userDefFeatOrder;
    }

    for(int i=0; i<GUIConfiguration::featureAttributes.size();i++){
        if(GUIConfiguration::featureAttributes.at(i)->displayState){

            if(!(GUIConfiguration::userDefFeatOrder.size() > 0)){
                displayedFeatureAttributes.append(GUIConfiguration::featureAttributes.at(i)->attrName);
            }
        }else{
            allFeatureAttributes.append(GUIConfiguration::featureAttributes.at(i)->attrName);
        }
    }

    for(int i=0; i<GUIConfiguration::featureAttributes.size();i++){
        if(GUIConfiguration::featureAttributes.at(i)->displayState){
            displayedFeatureAttributes.append(GUIConfiguration::featureAttributes.at(i)->attrName);
        }else{
            allFeatureAttributes.append(GUIConfiguration::featureAttributes.at(i)->attrName);
        }

    }
    //allFeatureAttributes.sort(Qt::CaseSensitive);
    //displayedFeatureAttributes.sort(Qt::CaseSensitive);
    m_featureAttributes = new QStringListModel(allFeatureAttributes);
    ui->listView_allFeatureAttributes->setModel(m_featureAttributes);

    m_displayedFeatureAttributes = new QStringListModel(displayedFeatureAttributes);
    ui->listView_displayedFeatureAttributes->setModel(m_displayedFeatureAttributes);

}

void SettingsDialog::displayTrafoParamColumns()
{
    allTrafoParamAttributes.clear();
    displayedTrafoParamAttributes.clear();
    for(int i=0; i<GUIConfiguration::trafoParamAttributes.size();i++){
        if(GUIConfiguration::trafoParamAttributes.at(i)->displayState){
            displayedTrafoParamAttributes.append(GUIConfiguration::trafoParamAttributes.at(i)->attrName);
        }else{
            allTrafoParamAttributes.append(GUIConfiguration::trafoParamAttributes.at(i)->attrName);
        }

    }
    //allTrafoParamAttributes.sort(Qt::CaseSensitive);
    //displayedTrafoParamAttributes.sort(Qt::CaseSensitive);
    m_TrafoParamAttributes = new QStringListModel(allTrafoParamAttributes);
    ui->listView_allTrafoParamAttributes->setModel(m_TrafoParamAttributes);

    m_displayedTrafoParamAttributes = new QStringListModel(displayedTrafoParamAttributes);
    ui->listView_displayedTrafoParamAttributes->setModel(m_displayedTrafoParamAttributes);
}

void SettingsDialog::getFeatureColumns()
{
    for(int i=0; i<GUIConfiguration::featureAttributes.size();i++){
        if(allFeatureAttributes.contains(GUIConfiguration::featureAttributes.at(i)->attrName)){
            GUIConfiguration::featureAttributes.at(i)->displayState = false;
        }else if(displayedFeatureAttributes.contains(GUIConfiguration::featureAttributes.at(i)->attrName)){
            GUIConfiguration::featureAttributes.at(i)->displayState = true;
        }
    }


    //get user defined feature order
    //GUIConfiguration::userDefFeatOrder.clear();
    //GUIConfiguration::userDefFeatOrder = displayedFeatureAttributes;
}

void SettingsDialog::getTrafoParamColumns()
{
    for(int i=0;i<GUIConfiguration::trafoParamAttributes.size();i++){
        if(allTrafoParamAttributes.contains(GUIConfiguration::trafoParamAttributes.at(i)->attrName)){
            GUIConfiguration::trafoParamAttributes.at(i)->displayState = false;
        }else if(displayedTrafoParamAttributes.contains(GUIConfiguration::trafoParamAttributes.at(i)->attrName)){
            GUIConfiguration::trafoParamAttributes.at(i)->displayState = true;
        }
    }

    //get user defined trafo param order
    //GUIConfiguration::userDefTrafoOrder.clear();
    //GUIConfiguration::userDefTrafoOrder = displayedTrafoParamAttributes;
}

void SettingsDialog::on_toolButton_addFeatureAttribute_clicked()
{
    if(allFeatureAttributes.size()>0){
        if(!ui->listView_allFeatureAttributes->currentIndex().isValid()){
            return;
        }
        QModelIndex idx = ui->listView_allFeatureAttributes->currentIndex();
        displayedFeatureAttributes.append(allFeatureAttributes.at(idx.row()));
        allFeatureAttributes.removeAt(idx.row());
        //allFeatureAttributes.sort(Qt::CaseSensitive);
        //displayedFeatureAttributes.sort(Qt::CaseSensitive);
        m_featureAttributes = new QStringListModel(allFeatureAttributes);
        ui->listView_allFeatureAttributes->setModel(m_featureAttributes);

        m_displayedFeatureAttributes = new QStringListModel(displayedFeatureAttributes);
        ui->listView_displayedFeatureAttributes->setModel(m_displayedFeatureAttributes);
    }
}

void SettingsDialog::on_toolButton_removeFeatureAttribute_clicked()
{
    if(displayedFeatureAttributes.size()>0){
        if(!ui->listView_displayedFeatureAttributes->currentIndex().isValid()){
            return;
        }
        QModelIndex idx = ui->listView_displayedFeatureAttributes->currentIndex();
        allFeatureAttributes.append(displayedFeatureAttributes.at(idx.row()));
        displayedFeatureAttributes.removeAt(idx.row());
        //allFeatureAttributes.sort(Qt::CaseSensitive);
        //displayedFeatureAttributes.sort(Qt::CaseSensitive);
        m_featureAttributes = new QStringListModel(allFeatureAttributes);
        ui->listView_allFeatureAttributes->setModel(m_featureAttributes);

        m_displayedFeatureAttributes = new QStringListModel(displayedFeatureAttributes);
        ui->listView_displayedFeatureAttributes->setModel(m_displayedFeatureAttributes);
    }
}

void SettingsDialog::on_toolButton_addTrafoParamAttribute_clicked()
{
    if(allTrafoParamAttributes.size()>0){
        if(!ui->listView_allTrafoParamAttributes->currentIndex().isValid()){
            return;
        }
        QModelIndex idx = ui->listView_allTrafoParamAttributes->currentIndex();
        if(idx.isValid()){
            displayedTrafoParamAttributes.append(allTrafoParamAttributes.at(idx.row()));
            allTrafoParamAttributes.removeAt(idx.row());
            //allTrafoParamAttributes.sort(Qt::CaseSensitive);
            //displayedTrafoParamAttributes.sort(Qt::CaseSensitive);
            m_TrafoParamAttributes = new QStringListModel(allTrafoParamAttributes);
            ui->listView_allTrafoParamAttributes->setModel(m_TrafoParamAttributes);

            m_displayedTrafoParamAttributes = new QStringListModel(displayedTrafoParamAttributes);
            ui->listView_displayedTrafoParamAttributes->setModel(m_displayedTrafoParamAttributes);
        }
    }
}

void SettingsDialog::on_toolButton_removeTrafoParamAttribute_clicked()
{
    if(displayedTrafoParamAttributes.size()>0){
        if(!ui->listView_displayedTrafoParamAttributes->currentIndex().isValid()){
            return;
        }
        QModelIndex idx = ui->listView_displayedTrafoParamAttributes->currentIndex();
        if(idx.isValid()){
            allTrafoParamAttributes.append(displayedTrafoParamAttributes.at(idx.row()));
            displayedTrafoParamAttributes.removeAt(idx.row());
            //allTrafoParamAttributes.sort(Qt::CaseSensitive);
            //displayedTrafoParamAttributes.sort(Qt::CaseSensitive);
            m_TrafoParamAttributes = new QStringListModel(allTrafoParamAttributes);
            ui->listView_allTrafoParamAttributes->setModel(m_TrafoParamAttributes);

            m_displayedTrafoParamAttributes = new QStringListModel(displayedTrafoParamAttributes);
            ui->listView_displayedTrafoParamAttributes->setModel(m_displayedTrafoParamAttributes);
        }
    }
}

void SettingsDialog::on_treeView_plugins_clicked(const QModelIndex &index){
    PluginTreeItem *item = static_cast<PluginTreeItem*>(index.internalPointer());

    //display general plugin information
    Plugin selectedPlugin = item->getPlugin();
    this->ui->txt_pluginDescription->setText(selectedPlugin.description);
    this->ui->txt_pluginAuthor->setText(selectedPlugin.author);
    this->ui->txt_pluginVersion->setText(selectedPlugin.version);
    this->ui->txt_pluginFilePath->setText(selectedPlugin.file_path);

    //display special sensor or function information
    if(item->getIsFunction()){
        this->myPluginInformation->displayFunction(item->getFunction());
    }else if(item->getIsSensor()){
        this->myPluginInformation->displaySensor(item->getSensor());
    }else{
        this->myPluginInformation->reset();
    }

}

void SettingsDialog::on_toolButton_up_clicked()
{
    if(!ui->listView_displayedFeatureAttributes->currentIndex().isValid()){
        return;
    }
    QModelIndex idx = ui->listView_displayedFeatureAttributes->currentIndex();
    if(idx.row()>0 && idx.row()<displayedFeatureAttributes.size()){
        displayedFeatureAttributes.swap(idx.row(),idx.row()-1);
        m_displayedFeatureAttributes = new QStringListModel(displayedFeatureAttributes);
        ui->listView_displayedFeatureAttributes->setModel(m_displayedFeatureAttributes);
    }
}

void SettingsDialog::on_toolButton_down_clicked()
{
    if(!ui->listView_displayedFeatureAttributes->currentIndex().isValid()){
        return;
    }
    QModelIndex idx = ui->listView_displayedFeatureAttributes->currentIndex();
    if(idx.row()<displayedFeatureAttributes.size()-1){
        displayedFeatureAttributes.swap(idx.row(),idx.row()+1);
        m_displayedFeatureAttributes = new QStringListModel(displayedFeatureAttributes);
        ui->listView_displayedFeatureAttributes->setModel(m_displayedFeatureAttributes);
    }
}
*/

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

    //set default unit
    this->ui->comboBox_angleType->setCurrentText(getUnitTypeName(ModelManager::getParameterDisplayConfig().getDisplayUnit(eAngular)));
    this->ui->comboBox_distanceType->setCurrentText(getUnitTypeName(ModelManager::getParameterDisplayConfig().getDisplayUnit(eMetric)));
    this->ui->comboBox_temperatureType->setCurrentText(getUnitTypeName(ModelManager::getParameterDisplayConfig().getDisplayUnit(eTemperature)));
    this->ui->lineEdit_angleDigits->setText(QString::number(ModelManager::getParameterDisplayConfig().getDisplayDigits(eAngular)));
    this->ui->lineEdit_distanceDigits->setText(QString::number(ModelManager::getParameterDisplayConfig().getDisplayDigits(eMetric)));
    this->ui->lineEdit_temperatureDigits->setText(QString::number(ModelManager::getParameterDisplayConfig().getDisplayDigits(eTemperature)));

    //from now on trigger changes
    this->ui->comboBox_angleType->blockSignals(false);
    this->ui->comboBox_distanceType->blockSignals(false);
    this->ui->comboBox_temperatureType->blockSignals(false);
    this->ui->lineEdit_angleDigits->blockSignals(false);
    this->ui->lineEdit_distanceDigits->blockSignals(false);
    this->ui->lineEdit_temperatureDigits->blockSignals(false);

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

    emit this->setDisplayConfig(config);

}
