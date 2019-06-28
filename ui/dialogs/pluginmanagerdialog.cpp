#include "pluginmanagerdialog.h"
#include "ui_pluginmanagerdialog.h"

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

void SettingsDialog::on_pushButton_ok_clicked()
{

    //first call generate, so they get the new unit string, if changed.
    saveSettings();
    //! generate lists for gui and view modification
    GUIConfiguration::generateLists();

    //then they get the boolean for display or not.
    getFeatureColumns();
    getTrafoParamColumns();
    this->close();
}

void SettingsDialog::on_pushButton_cancel_clicked()
{
    this->close();
}

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
 * \brief PluginManagerDialog::PluginManagerDialog
 * \param parent
 */
PluginManagerDialog::PluginManagerDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::PluginManagerDialog)
{
    this->ui->setupUi(this);

    //init GUI elements and assign models
    this->initGUI();
    this->initModels();

}

/*!
 * \brief PluginManagerDialog::~PluginManagerDialog
 */
PluginManagerDialog::~PluginManagerDialog(){
    delete this->ui;
}

/*!
 * \brief PluginManagerDialog::on_pushButton_add_clicked
 */
void PluginManagerDialog::on_pushButton_add_clicked(){
    emit this->loadPlugins();
}

/*!
 * \brief PluginManagerDialog::on_pushButton_remove_clicked
 */
void PluginManagerDialog::on_pushButton_remove_clicked(){

}

/*!
 * \brief PluginManagerDialog::on_treeView_plugins_clicked
 * \param index
 */
void PluginManagerDialog::on_treeView_plugins_clicked(const QModelIndex &index){

    //check index
    if(!index.isValid()){
        return;
    }

    //get selected tree item
    PluginTreeItem *item = static_cast<PluginTreeItem *>(index.internalPointer());
    if(item == NULL){
        return;
    }

    //get plugin index and item
    QModelIndex pluginIndex = index;
    while(pluginIndex.parent().isValid()){
        pluginIndex = pluginIndex.parent();
    }
    PluginTreeItem *pluginItem = static_cast<PluginTreeItem *>(pluginIndex.internalPointer());
    if(pluginItem == NULL){
        return;
    }

    //set general plugin information
    this->ui->txt_pluginDescription->setText(pluginItem->getPlugin().description);
    this->ui->txt_pluginAuthor->setText(pluginItem->getPlugin().author);
    this->ui->txt_pluginFilePath->setText(pluginItem->getPlugin().file_path);
    this->ui->txt_pluginVersion->setText(pluginItem->getPlugin().version);

    //set visibility
    this->ui->pushButton_remove->setVisible(true);
    this->ui->txt_pluginDescription->setVisible(true);
    this->ui->txt_pluginAuthor->setVisible(true);
    this->ui->txt_pluginFilePath->setVisible(true);
    this->ui->txt_pluginVersion->setVisible(true);
    this->ui->label_description->setVisible(true);
    this->ui->label_author->setVisible(true);
    this->ui->label_filePath->setVisible(true);
    this->ui->label_version->setVisible(true);

    //set special plugin information
    if(item->getIsExchange()){
        this->pluginInformation->displayExchange(item->getExchange());
        this->ui->widget_pluginInfo->setVisible(true);
        return;
    }
    if(item->getIsFunction()){
        this->pluginInformation->displayFunction(item->getFunction());
        this->ui->widget_pluginInfo->setVisible(true);
        return;
    }
    if(item->getIsNetworkAdjustment()){
        this->pluginInformation->displayNetworkAdjustment(item->getNetworkAdjustment());
        this->ui->widget_pluginInfo->setVisible(true);
        return;
    }
    if(item->getIsSensor()){
        this->pluginInformation->displaySensor(item->getSensor());
        this->ui->widget_pluginInfo->setVisible(true);
        return;
    }
    if(item->getIsSimulation()){
        this->pluginInformation->displaySimulation(item->getSimulation());
        this->ui->widget_pluginInfo->setVisible(true);
        return;
    }
    if(item->getIsTool()){
        this->pluginInformation->displayTool(item->getTool());

        this->ui->widget_pluginInfo->setVisible(true);
        return;
    }

    this->ui->widget_pluginInfo->setVisible(false);

}

/*!
 * \brief PluginManagerDialog::initGUI
 */
void PluginManagerDialog::initGUI(){

    //set plugin information widget
    QGridLayout *pluginInfoLayout = new QGridLayout();
    pluginInfoLayout->setContentsMargins(0,0,0,0);
    this->ui->widget_pluginInfo->setLayout(pluginInfoLayout);
    this->pluginInformation = new PluginInfoWidget();
    pluginInfoLayout->addWidget(this->pluginInformation);

    //set initial visibility
    this->ui->pushButton_remove->setVisible(false);
    this->ui->txt_pluginDescription->setVisible(false);
    this->ui->txt_pluginAuthor->setVisible(false);
    this->ui->txt_pluginFilePath->setVisible(false);
    this->ui->txt_pluginVersion->setVisible(false);
    this->ui->label_description->setVisible(false);
    this->ui->label_author->setVisible(false);
    this->ui->label_filePath->setVisible(false);
    this->ui->label_version->setVisible(false);

}

/*!
 * \brief PluginManagerDialog::initModels
 */
void PluginManagerDialog::initModels(){

    //set plugin tree view model
    this->ui->treeView_plugins->setModel(&ModelManager::getPluginTreeViewModel());

}
