#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    QGridLayout *pluginInfoLayout = new QGridLayout();
    this->ui->widget_pluginInfo->setLayout(pluginInfoLayout);
    this->myPluginInformation = new PluginInfoWidget();
    pluginInfoLayout->addWidget(this->myPluginInformation);

    initGUI();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

/*!
 * \brief setPluginsModel
 */
void SettingsDialog::setPluginsModel(PluginTreeViewModel *model){
    this->ui->treeView_plugins->setModel(model);
    this->ui->treeView_plugins->expandToDepth(1);
}

/*!
 * \brief on_pushButton_ok_clicked saves all settings.
 */
void SettingsDialog::on_pushButton_ok_clicked()
{

    //first call generate, so they get the new unit string, if changed.
    saveSettings();
    GUIConfiguration::generateAllAttributes();
    GUIConfiguration::generateFeatureAttributes();
    GUIConfiguration::generateTrafoParamAttributes();
    //then they get the boolean for display or not.
    getFeatureColumns();
    getTrafoParamColumns();
    this->close();
}

/*!
 * \brief on_pushButton_cancel_clicked
 */
void SettingsDialog::on_pushButton_cancel_clicked()
{
    this->close();
}

/*!
 * \brief initGUI
 */
void SettingsDialog::initGUI(){

    ui->comboBox_angleType->insertItem(ui->comboBox_angleType->count(),"decimal degree",UnitConverter::eDECIMALDEGREE);
    ui->comboBox_angleType->insertItem(ui->comboBox_angleType->count(),"gon",UnitConverter::eGON);
    ui->comboBox_angleType->insertItem(ui->comboBox_angleType->count(),"radiant",UnitConverter::eRADIANT);
    ui->comboBox_angleType->insertItem(ui->comboBox_angleType->count(),"arc seconds",UnitConverter::eArcSeconds);
    ui->comboBox_angleType->insertItem(ui->comboBox_angleType->count(),"mrad",UnitConverter::eMilliRadians);

    ui->comboBox_distanceType->insertItem(ui->comboBox_distanceType->count(),"meter",UnitConverter::eMETER);
    ui->comboBox_distanceType->insertItem(ui->comboBox_distanceType->count(),"millimeter",UnitConverter::eMILLIMETER);
    ui->comboBox_distanceType->insertItem(ui->comboBox_distanceType->count(),"inch",UnitConverter::eInch);

    ui->comboBox_temperatureType->insertItem(ui->comboBox_temperatureType->count(),"degree",UnitConverter::eGRAD);
    ui->comboBox_temperatureType->insertItem(ui->comboBox_temperatureType->count(),"Kelvin",UnitConverter::eKelvin);
    ui->comboBox_temperatureType->insertItem(ui->comboBox_temperatureType->count(),"Fahrenheit",UnitConverter::eFahrenheit);

    ui->lineEdit_angleDigits->setText(QString::number(UnitConverter::angleDigits));
    ui->lineEdit_distanceDigits->setText(QString::number(UnitConverter::distanceDigits));
    ui->lineEdit_temperatureDigits->setText(QString::number(UnitConverter::temperatureDigits));

}

/*!
 * \brief extended showEvent
 * \param event
 */
void SettingsDialog::showEvent(QShowEvent *event){
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

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

/*!
 * \brief extended closeEvent
 * \param event
 */
void SettingsDialog::closeEvent(QCloseEvent *event)
{
    emit modelChanged();
    event->accept();
}

/*!
 * \brief saveSettings
 */
void SettingsDialog::saveSettings(){

    UnitConverter::angleDigits = ui->lineEdit_angleDigits->text().toInt();
    UnitConverter::distanceDigits = ui->lineEdit_distanceDigits->text().toInt();
    UnitConverter::temperatureDigits = ui->lineEdit_temperatureDigits->text().toInt();
    UnitConverter::setAngleUnit(static_cast<UnitConverter::unitType>(ui->comboBox_angleType->itemData(ui->comboBox_angleType->currentIndex()).toInt()));
    UnitConverter::setDistanceUnit(static_cast<UnitConverter::unitType>(ui->comboBox_distanceType->itemData(ui->comboBox_distanceType->currentIndex()).toInt()));
    UnitConverter::setTemperatureUnit(static_cast<UnitConverter::unitType>(ui->comboBox_temperatureType->itemData(ui->comboBox_temperatureType->currentIndex()).toInt()));
}

/*!
 * \brief displayColumns shows all available attribute columns for feature view.
 */
void SettingsDialog::displayFeatureColumns()
{
    allFeatureAttributes.clear();
    displayedFeatureAttributes.clear();
    for(int i=0; i<GUIConfiguration::featureAttributes.size();i++){
        if(GUIConfiguration::featureAttributes.at(i)->displayState){
            displayedFeatureAttributes.append(GUIConfiguration::featureAttributes.at(i)->attrName);
        }else{
            allFeatureAttributes.append(GUIConfiguration::featureAttributes.at(i)->attrName);
        }

    }
    allFeatureAttributes.sort(Qt::CaseSensitive);
    displayedFeatureAttributes.sort(Qt::CaseSensitive);
    m_featureAttributes = new QStringListModel(allFeatureAttributes);
    ui->listView_allFeatureAttributes->setModel(m_featureAttributes);

    m_displayedFeatureAttributes = new QStringListModel(displayedFeatureAttributes);
    ui->listView_displayedFeatureAttributes->setModel(m_displayedFeatureAttributes);

}

/*!
 * \brief displayTrafoParamColumns shows all available attributes for trafoParam view.
 */
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
    allTrafoParamAttributes.sort(Qt::CaseSensitive);
    displayedTrafoParamAttributes.sort(Qt::CaseSensitive);
    m_TrafoParamAttributes = new QStringListModel(allTrafoParamAttributes);
    ui->listView_allTrafoParamAttributes->setModel(m_TrafoParamAttributes);

    m_displayedTrafoParamAttributes = new QStringListModel(displayedTrafoParamAttributes);
    ui->listView_displayedTrafoParamAttributes->setModel(m_displayedTrafoParamAttributes);
}

/*!
 * \brief getFeatureColumns checks if the attribute should be displayed or not. It saves the changes in the configuration class.
 */
void SettingsDialog::getFeatureColumns()
{
    for(int i=0; i<GUIConfiguration::featureAttributes.size();i++){
        if(allFeatureAttributes.contains(GUIConfiguration::featureAttributes.at(i)->attrName)){
            GUIConfiguration::featureAttributes.at(i)->displayState = false;
        }
    }
}

/*!
 * \brief getTrafoParamColumns checks if the attribute should be displayed or not. It saves the changes in the configuration class.
 */
void SettingsDialog::getTrafoParamColumns()
{
    for(int i=0;i<GUIConfiguration::trafoParamAttributes.size();i++){
        if(allTrafoParamAttributes.contains(GUIConfiguration::trafoParamAttributes.at(i)->attrName)){
            GUIConfiguration::trafoParamAttributes.at(i)->displayState = false;
        }
    }
}

/*!
 * \brief on_toolButton_addFeatureAttribute_clicked adds a new feature attribute to the list of displayed attributes.
 */
void SettingsDialog::on_toolButton_addFeatureAttribute_clicked()
{
    if(allFeatureAttributes.size()>0){
        if(!ui->listView_allFeatureAttributes->currentIndex().isValid()){
            return;
        }
        QModelIndex idx = ui->listView_allFeatureAttributes->currentIndex();
        displayedFeatureAttributes.append(allFeatureAttributes.at(idx.row()));
        allFeatureAttributes.removeAt(idx.row());
        allFeatureAttributes.sort(Qt::CaseSensitive);
        displayedFeatureAttributes.sort(Qt::CaseSensitive);
        m_featureAttributes = new QStringListModel(allFeatureAttributes);
        ui->listView_allFeatureAttributes->setModel(m_featureAttributes);

        m_displayedFeatureAttributes = new QStringListModel(displayedFeatureAttributes);
        ui->listView_displayedFeatureAttributes->setModel(m_displayedFeatureAttributes);
    }
}

/*!
 * \brief on_toolButton_removeFeatureAttribute_clicked removes a feature attribute from the list of displayed attributes.
 */
void SettingsDialog::on_toolButton_removeFeatureAttribute_clicked()
{
    if(displayedFeatureAttributes.size()>0){
        if(!ui->listView_displayedFeatureAttributes->currentIndex().isValid()){
            return;
        }
        QModelIndex idx = ui->listView_displayedFeatureAttributes->currentIndex();
        allFeatureAttributes.append(displayedFeatureAttributes.at(idx.row()));
        displayedFeatureAttributes.removeAt(idx.row());
        allFeatureAttributes.sort(Qt::CaseSensitive);
        displayedFeatureAttributes.sort(Qt::CaseSensitive);
        m_featureAttributes = new QStringListModel(allFeatureAttributes);
        ui->listView_allFeatureAttributes->setModel(m_featureAttributes);

        m_displayedFeatureAttributes = new QStringListModel(displayedFeatureAttributes);
        ui->listView_displayedFeatureAttributes->setModel(m_displayedFeatureAttributes);
    }
}

/*!
 * \brief on_toolButton_addTrafoParamAttribute_clicked adds a trafo parameter attribute to the list of displayed attributes.
 */
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
            allTrafoParamAttributes.sort(Qt::CaseSensitive);
            displayedTrafoParamAttributes.sort(Qt::CaseSensitive);
            m_TrafoParamAttributes = new QStringListModel(allTrafoParamAttributes);
            ui->listView_allTrafoParamAttributes->setModel(m_TrafoParamAttributes);

            m_displayedTrafoParamAttributes = new QStringListModel(displayedTrafoParamAttributes);
            ui->listView_displayedTrafoParamAttributes->setModel(m_displayedTrafoParamAttributes);
        }
    }
}

/*!
 * \brief on_toolButton_removeTrafoParamAttribute_clicked removes an trafo param attribute from the list of displayed attributes.
 */
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
            allTrafoParamAttributes.sort(Qt::CaseSensitive);
            displayedTrafoParamAttributes.sort(Qt::CaseSensitive);
            m_TrafoParamAttributes = new QStringListModel(allTrafoParamAttributes);
            ui->listView_allTrafoParamAttributes->setModel(m_TrafoParamAttributes);

            m_displayedTrafoParamAttributes = new QStringListModel(displayedTrafoParamAttributes);
            ui->listView_displayedTrafoParamAttributes->setModel(m_displayedTrafoParamAttributes);
        }
    }
}

/*!
 * \brief SettingsDialog::on_treeView_plugins_clicked
 * Is called whenever a treeview item of plugins treeview is clicked
 * \param index
 */
void SettingsDialog::on_treeView_plugins_clicked(const QModelIndex &index){
    PluginTreeItem *item = static_cast<PluginTreeItem*>(index.internalPointer());

    //display general plugin information
    Plugin selectedPlugin = item->getPlugin();
    this->ui->txt_pluginDescription->setText(selectedPlugin.description);
    this->ui->txt_pluginAuthor->setText(selectedPlugin.author);
    this->ui->txt_pluginVersion->setText(selectedPlugin.version);
    this->ui->txt_pluginFilePath->setText(selectedPlugin.file_path);

    if(item->getIsFunction()){
        this->myPluginInformation->displayFunction(item->getFunction());
    }

}
