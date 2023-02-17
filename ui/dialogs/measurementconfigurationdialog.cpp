#include "measurementconfigurationdialog.h"
#include "ui_measurementconfigurationdialog.h"

/*!
 * \brief MeasurementConfigurationDialog::MeasurementConfigurationDialog
 * \param parent
 */
MeasurementConfigurationDialog::MeasurementConfigurationDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::MeasurementConfigurationDialog)
{
    ui->setupUi(this);

    //init models
    this->initModels();

    //init GUI elements
    this->initGUI();

}

/*!
 * \brief MeasurementConfigurationDialog::~MeasurementConfigurationDialog
 */
MeasurementConfigurationDialog::~MeasurementConfigurationDialog(){
    delete this->ui;
}

/*!
 * \brief MeasurementConfigurationDialog::setMeasurementConfiguration
 * \param mConfig
 */
void MeasurementConfigurationDialog::setMeasurementConfiguration(const MeasurementConfig &mConfig){

    //check if name is empty
    if(!mConfig.isValid()){
        currentListView()->selectionModel()->clearSelection();
        this->updateGuiFromMeasurementConfig(MeasurementConfig());
        this->ui->widget_measurementConfigValues->setEnabled(false);
    }

    //get measurement config model
    const MeasurementConfigurationModel &model = ModelManager::getMeasurementConfigurationModel();

    //get model index of mConfig
    QModelIndex index = model.getIndex(mConfig);
    if(!index.isValid()){
        return;
    }

    //update selected measurement config
    currentListView()->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
    this->updateGuiFromMeasurementConfig(mConfig);
    this->ui->widget_measurementConfigValues->setEnabled(mConfig.isUserConfig()
                                                         ? !mConfig.isStandardConfig()
                                                         : false);
    this->selectedMeasurementConfig = mConfig;

}

/*!
 * \brief MeasurementConfigurationDialog::on_listView_userConfigs_clicked
 * \param index
 */
void MeasurementConfigurationDialog::on_listView_userConfigs_clicked(const QModelIndex &index){
    on_listView_configs_clicked(index);
}
void MeasurementConfigurationDialog::on_listView_projectConfigs_clicked(const QModelIndex &index){
    on_listView_configs_clicked(index);
}
void MeasurementConfigurationDialog::on_listView_configs_clicked(const QModelIndex &index) {
    //check index
    if(!index.isValid()){
        return;
    }

    //set widgets visibility
    this->ui->widget_measurementConfigValues->setEnabled(true);

    //get and check model
    MeasurementConfigurationProxyModel *mConfigProxyModel = static_cast<MeasurementConfigurationProxyModel *>(currentListView()->model());
    if(mConfigProxyModel == NULL){
        return;
    }

    //get and check source model
    MeasurementConfigurationModel *mConfigModel = static_cast<MeasurementConfigurationModel *>(mConfigProxyModel->sourceModel());
    if(mConfigModel == NULL){
        return;
    }

    //get selected measurement config
    MeasurementConfig mConfig = mConfigModel->getMeasurementConfig(mConfigProxyModel->mapToSource(index));

    //update GUI from selected measurement config
    this->updateGuiFromMeasurementConfig(mConfig);

    //toggle enabled state depending on what mConfig has been selected (project vs. user)
    this->ui->widget_measurementConfigValues->setEnabled(mConfig.isUserConfig()
                                                         ? !mConfig.isStandardConfig()
                                                         : false);

    this->selectedMeasurementConfig = mConfig;

}

/*!
 * \brief MeasurementConfigurationDialog::measurementConfigContextMenuRequested
 * \param point
 */
void MeasurementConfigurationDialog::measurementConfigContextMenuRequested(const QPoint &point){

    //get and check index
    QModelIndex index = currentListView()->indexAt(point);
    if(!index.isValid() || index.parent().isValid()){
        return;
    }

    //get and check measurement config proxy model
    MeasurementConfigurationProxyModel *mConfigProxyModel = static_cast<MeasurementConfigurationProxyModel *>(currentListView()->model());
    if(mConfigProxyModel == NULL){
        return;
    }

    //get and check measurement config model
    MeasurementConfigurationModel *mConfigModel = static_cast<MeasurementConfigurationModel *>(mConfigProxyModel->sourceModel());
    if(mConfigModel == NULL){
        return;
    }

    //set selected index
    currentListView()->selectionModel()->select(index, QItemSelectionModel::Select);
    this->selectedMeasurementConfig = mConfigModel->getMeasurementConfig(mConfigProxyModel->mapToSource(index));

    this->updateGuiFromMeasurementConfig(mConfigModel->getMeasurementConfig(mConfigProxyModel->mapToSource(index)));
    if(this->selectedMeasurementConfig.isUserConfig()){
        this->ui->widget_measurementConfigValues->setEnabled(true);
    }else{
        this->ui->widget_measurementConfigValues->setEnabled(false);
    }

    if(this->selectedMeasurementConfig.isUserConfig() && this->selectedMeasurementConfig.isEditable()) {
        QMenu *menu = new QMenu();
        menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("delete config"), this, SLOT(removeSelectedMeasurementConfig()));
        menu->exec(ui->listView_userConfigs->mapToGlobal(point));
    }else if(this->selectedMeasurementConfig.isProjectConfig()){
        QMenu *menu = new QMenu();
        menu->addAction(QIcon(":/Images/icons/edit_add.png"), QString("clone config"), this, SLOT(cloneSelectedMeasurementConfig()));
        menu->exec(ui->listView_projectConfigs->mapToGlobal(point));
    }

}

/*!
 * \brief MeasurementConfigurationDialog::removeSelectedMeasurementConfig
 */
void MeasurementConfigurationDialog::removeSelectedMeasurementConfig(){

    //get and check index
    QModelIndexList selection = currentListView()->selectionModel()->selectedIndexes();
    if(selection.size() != 1){
        return;
    }
    QModelIndex index = selection.at(0);
    if(!index.isValid() || index.parent().isValid()){
        return;
    }

    //get and check measurement config proxy model
    MeasurementConfigurationProxyModel *mConfigProxyModel = static_cast<MeasurementConfigurationProxyModel *>(currentListView()->model());
    if(mConfigProxyModel == NULL){
        return;
    }

    //get and check measurement config model
    MeasurementConfigurationModel *mConfigModel = static_cast<MeasurementConfigurationModel *>(mConfigProxyModel->sourceModel());
    if(mConfigModel == NULL){
        return;
    }

    QMessageBox msgBox;
    msgBox.setText("Please check if the deleted measurement config is still in use and replace it by an existing measurement config.");
    msgBox.setInformativeText("");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

    mConfigModel->removeMeasurementConfig(mConfigProxyModel->mapToSource(index));

    //deselect measurement configs and disable widget
    currentListView()->selectionModel()->clearSelection();
    this->updateGuiFromMeasurementConfig(MeasurementConfig());
    this->ui->widget_measurementConfigValues->setEnabled(false);
    this->selectedMeasurementConfig = MeasurementConfig();

}

/*!
 * \brief MeasurementConfigurationDialog::cloneSelectedMeasurementConfig
 */
void MeasurementConfigurationDialog::cloneSelectedMeasurementConfig(){

    //get selected measurement config
    MeasurementConfig mConfig = this->selectedMeasurementConfig;

    //get and check measurement config proxy model
    MeasurementConfigurationProxyModel *mConfigProxyModel = static_cast<MeasurementConfigurationProxyModel *>(currentListView()->model());
    if(mConfigProxyModel == NULL){
        return;
    }

    //get and check measurement config model
    MeasurementConfigurationModel *mConfigModel = static_cast<MeasurementConfigurationModel *>(mConfigProxyModel->sourceModel());
    if(mConfigModel == NULL){
        return;
    }

    const MeasurementConfig clone = mConfigModel->cloneMeasurementConfig(mConfig);
    if(clone.isValid()) {
        //select the new item
        QModelIndex index = mConfigModel->getIndex(clone);
        this->ui->tabWidget->setCurrentIndex(1);
        currentListView()->selectionModel()->select(mConfigProxyModel->mapFromSource(index), QItemSelectionModel::ClearAndSelect);
        this->on_listView_configs_clicked(mConfigProxyModel->mapFromSource(index));
    }
}

/*!
 * \brief MeasurementConfigurationDialog::on_pushButton_add_clicked
 */
void MeasurementConfigurationDialog::on_pushButton_add_clicked(){

    this->ui->tabWidget->setCurrentIndex(1);

    MeasurementConfig mConfig;
    mConfig.setName("new config");
    mConfig.makeUserConfig();

    //get and check measurement config proxy model
    MeasurementConfigurationProxyModel *mConfigProxyModel = static_cast<MeasurementConfigurationProxyModel *>(currentListView()->model());
    if(mConfigProxyModel == NULL){
        return;
    }

    //get and check measurement config model
    MeasurementConfigurationModel *mConfigModel = static_cast<MeasurementConfigurationModel *>(mConfigProxyModel->sourceModel());
    if(mConfigModel == NULL){
        return;
    }

    //add new measurement config
    QModelIndex index = mConfigModel->addMeasurementConfig(mConfig);

    //check index
    if(!index.isValid() || !mConfigModel->hasIndex(index.row(), index.column())){
        return;
    }

    //select the new item
    currentListView()->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
    this->updateGuiFromMeasurementConfig(mConfig);
    currentListView()->edit(mConfigProxyModel->mapFromSource(index));
    this->selectedMeasurementConfig = mConfig;

}

/*!
 * \brief MeasurementConfigurationDialog::on_lineEdit_maxObservations_textChanged
 * \param arg1
 */
void MeasurementConfigurationDialog::on_lineEdit_maxObservations_textChanged(const QString &arg1){
    this->updateMeasurementConfigFromSelection();
}

/*!
 * \brief MeasurementConfigurationDialog::on_checkBox_twoFace_clicked
 */
void MeasurementConfigurationDialog::on_checkBox_twoFace_clicked(){
    this->updateMeasurementConfigFromSelection();
}

/*!
 * \brief MeasurementConfigurationDialog::on_lineEdit_timeInterval_textChanged
 * \param arg1
 */
void MeasurementConfigurationDialog::on_lineEdit_timeInterval_textChanged(const QString &arg1){
    this->updateMeasurementConfigFromSelection();
}

/*!
 * \brief MeasurementConfigurationDialog::on_lineEdit_distancInterval_textChanged
 * \param arg1
 */
void MeasurementConfigurationDialog::on_lineEdit_distancInterval_textChanged(const QString &arg1){
    this->updateMeasurementConfigFromSelection();
}

/*!
 * \brief MeasurementConfigurationDialog::on_pushButton_set_clicked
 */
void MeasurementConfigurationDialog::on_pushButton_set_to_feature_clicked(){

    //get selected measurement config
    MeasurementConfig mConfig = this->selectedMeasurementConfig;

    //set the selected measurement config for the active feature
    emit this->measurementConfigurationChanged(mConfig);

    this->close();

}

/*!
 * \brief MeasurementConfigurationDialog::measurementConfigNameChanged
 * \param mConfig
 */
void MeasurementConfigurationDialog::measurementConfigNameChanged(const MeasurementConfig &mConfig){
    this->updateGuiFromMeasurementConfig(mConfig);
}

/*!
 * \brief MeasurementConfigurationDialog::updateGuiFromMeasurementConfig
 * \param mConfig
 */
void MeasurementConfigurationDialog::updateGuiFromMeasurementConfig(const MeasurementConfig &mConfig){

    //update selected measurement config
    this->selectedMeasurementConfig = mConfig;

    //do not trigger edits while setting up measurement config
    this->ui->lineEdit_distancInterval->blockSignals(true);
    this->ui->lineEdit_maxObservations->blockSignals(true);
    this->ui->lineEdit_timeInterval->blockSignals(true);
    this->ui->checkBox_twoFace->blockSignals(true);
    this->ui->comboBox_MeasurementMode->blockSignals(true);
    this->ui->comboBox_MeasurementType->blockSignals(true);

    //set up GUI elements
    this->ui->label_configName->setText(QString("%1%2")
                                        .arg(mConfig.getName())
                                        .arg((!mConfig.isValid() || mConfig.isUserConfig())?" [user]":"")
                                        );

    this->ui->comboBox_MeasurementType->setCurrentIndex(mConfig.getMeasurementType());
    this->ui->comboBox_MeasurementMode->setCurrentIndex(mConfig.getMeasurementMode());
    this->ui->checkBox_twoFace->setChecked(mConfig.getMeasureTwoSides());

    this->ui->lineEdit_maxObservations->setText(QString::number(mConfig.getMaxObservations()));
    this->ui->lineEdit_distancInterval->setText(QString::number(mConfig.getDistanceInterval(), 'f', 4));
    this->ui->lineEdit_timeInterval->setText(QString::number(mConfig.getTimeInterval()));

    this->enableUIElements(this->selectedMeasurementConfig.isEditable()
                           ? mConfig.getMeasurementType() : eUnknown_MeasurementType);

    //from now on trigger edits
    this->ui->lineEdit_distancInterval->blockSignals(false);
    this->ui->lineEdit_maxObservations->blockSignals(false);
    this->ui->lineEdit_timeInterval->blockSignals(false);
    this->ui->checkBox_twoFace->blockSignals(false);
    this->ui->comboBox_MeasurementMode->blockSignals(false);
    this->ui->comboBox_MeasurementType->blockSignals(false);

   this->enableUIElements(mConfig);

}

/*!
 * \brief MeasurementConfigurationDialog::updateMeasurementConfigFromSelection
 */
void MeasurementConfigurationDialog::updateMeasurementConfigFromSelection(){

    //get and check model
    MeasurementConfigurationProxyModel *mConfigProxyModel = static_cast<MeasurementConfigurationProxyModel *>(currentListView()->model());
    if(mConfigProxyModel == NULL){
        return;
    }

    //get and check source model
    MeasurementConfigurationModel *mConfigModel = static_cast<MeasurementConfigurationModel *>(mConfigProxyModel->sourceModel());
    if(mConfigModel == NULL){
        return;
    }

    //get selected measurement config name
    QModelIndexList selection = currentListView()->selectionModel()->selectedIndexes();
    if(selection.size() != 1){
        return;
    }
    QString name = selection.at(0).data().toString();

    //get measurement config from GUI selection
    MeasurementConfig mConfig;
    mConfig.setName(name);

    // set measurement type
    mConfig.setMeasurementType((MeasurementTypes)this->ui->comboBox_MeasurementType->currentIndex());

    // single point
    mConfig.setMeasureTwoSides(this->ui->checkBox_twoFace->isChecked());

    // scan
    mConfig.setMaxObservations(this->ui->lineEdit_maxObservations->text().toInt());
    mConfig.setDistanceInterval(this->ui->lineEdit_distancInterval->text().toDouble()); // [mm]
    mConfig.setTimeInterval(this->ui->lineEdit_timeInterval->text().toLong());

    mConfig.makeUserConfig();

    //replace the selected measurement config
    mConfigModel->replaceMeasurementConfig(name, mConfig);
    this->selectedMeasurementConfig = mConfig;

}

/*!
 * \brief MeasurementConfigurationDialog::showEvent
 * \param event
 */
void MeasurementConfigurationDialog::showEvent(QShowEvent *event){

    QObject::connect(&ModelManager::getMeasurementConfigurationModel(), &MeasurementConfigurationModel::measurementConfigNameChanged,
                        this, &MeasurementConfigurationDialog::measurementConfigNameChanged, Qt::AutoConnection);

    MeasurementConfigurationProxyModel *mConfigProxyModel = static_cast<MeasurementConfigurationProxyModel *>(currentListView()->model());
    if(mConfigProxyModel != NULL){
        mConfigProxyModel->resetFilter();
    }

    emit initialized();

    event->accept();

}

/*!
 * \brief MeasurementConfigurationDialog::closeEvent
 * \param event
 */
void MeasurementConfigurationDialog::closeEvent(QCloseEvent *event){

    QObject::disconnect(&ModelManager::getMeasurementConfigurationModel(), &MeasurementConfigurationModel::measurementConfigNameChanged,
                        this, &MeasurementConfigurationDialog::measurementConfigNameChanged);

    event->accept();

}

/*!
 * \brief MeasurementConfigurationDialog::initGUI
 */
void MeasurementConfigurationDialog::initGUI(){
    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    //set visibility
    this->ui->widget_measurementConfigValues->setEnabled(false);

    MeasurementConfigurationListDelegate *delegate = new MeasurementConfigurationListDelegate();

    //allow context menu for measurement config model
    this->ui->listView_userConfigs->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this->ui->listView_userConfigs, &QListView::customContextMenuRequested, this, &MeasurementConfigurationDialog::measurementConfigContextMenuRequested, Qt::AutoConnection);
    //assign delegate to measurement configs list view
    this->ui->listView_userConfigs->setItemDelegate(delegate);

    this->ui->listView_projectConfigs->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this->ui->listView_projectConfigs, &QListView::customContextMenuRequested, this, &MeasurementConfigurationDialog::measurementConfigContextMenuRequested, Qt::AutoConnection);
    //assign delegate to measurement configs list view
    this->ui->listView_projectConfigs->setItemDelegate(delegate);


}

/*!
 * \brief MeasurementConfigurationDialog::initModels
 */
void MeasurementConfigurationDialog::initModels(){

    MeasurementConfigurationProxyModel *model = &ModelManager::getMeasurementConfigurationProxyModel();

    //init measurement config model
    this->ui->listView_projectConfigs->setModel(model);
    this->ui->listView_userConfigs->setModel(model);

    this->ui->tabWidget->setCurrentIndex(0);

    this->isProjectConfigSelected = true;
    model->setFilterProjectConfig();
    this->ui->pushButton_save_user_config->setEnabled( ! this->isProjectConfigSelected);

}

void MeasurementConfigurationDialog::on_lineEdit_stablePoint_min_distance_textChanged(const QString &arg1)
{
    this->updateMeasurementConfigFromSelection();
}

void MeasurementConfigurationDialog::on_lineEdit_stablePoint_threshold_textChanged(const QString &arg1)
{
    this->updateMeasurementConfigFromSelection();
}

void MeasurementConfigurationDialog::on_checkBox_isStablePoint_clicked()
{
    this->updateMeasurementConfigFromSelection();
}

void MeasurementConfigurationDialog::on_lineEdit_stablePoint_thresholdTime_textChanged(const QString &arg1)
{
    this->updateMeasurementConfigFromSelection();
}


void MeasurementConfigurationDialog::on_comboBox_MeasurementMode_currentIndexChanged(int index)
{
    this->updateMeasurementConfigFromSelection();
}

void MeasurementConfigurationDialog::on_comboBox_MeasurementType_currentIndexChanged(int index)
{
    this->enableUIElements((MeasurementTypes)index);

    this->updateMeasurementConfigFromSelection();
}

void MeasurementConfigurationDialog::enableUIElements(MeasurementTypes type) {
    type = this->isProjectConfigSelected ? eUnknown_MeasurementType : type;

    // enable all
    this->ui->groupBox_Single_Point->setEnabled(true);
    this->ui->groupBox_Scan->setEnabled(true);
    this->ui->checkBox_twoFace->setEnabled(true);
    this->ui->lineEdit_timeInterval->setEnabled(true);
    this->ui->lineEdit_distancInterval->setEnabled(true);
    this->ui->lineEdit_maxObservations->setEnabled(true);

    switch(type) {
    case eSinglePoint_MeasurementType:
        this->ui->groupBox_Scan->setEnabled(false);
        break;
    case eScanTimeDependent_MeasurementType:
        this->ui->groupBox_Single_Point->setEnabled(false);
        this->ui->lineEdit_distancInterval->setEnabled(false);
        break;
    case eScanDistanceDependent_MeasurementType:
        this->ui->groupBox_Single_Point->setEnabled(false);
        this->ui->lineEdit_timeInterval->setEnabled(false);
        break;
    case eLevel_MeasurementType:
    case eDistance_MeasurementType:
    case eDirection_MeasurementType:
    case eTemperature_MeasurementType:
        this->ui->groupBox_Scan->setEnabled(false);
        this->ui->groupBox_Single_Point->setEnabled(false);
        break;
    case eUnknown_MeasurementType: // disable all
        this->ui->groupBox_Single_Point->setEnabled(false);
        this->ui->groupBox_Scan->setEnabled(false);
        this->ui->checkBox_twoFace->setEnabled(false);
        this->ui->lineEdit_timeInterval->setEnabled(false);
        this->ui->lineEdit_distancInterval->setEnabled(false);
        this->ui->lineEdit_maxObservations->setEnabled(false);
        break;
    }

}

void MeasurementConfigurationDialog::enableUIElements(const MeasurementConfig &mConfig) {
    this->ui->pushButton_save_user_config->setEnabled(
            mConfig.isUserConfig()
            && !mConfig.isStandardConfig()
        );
}

QPointer<QListView> MeasurementConfigurationDialog::currentListView() {

    MeasurementConfigurationProxyModel *model = static_cast<MeasurementConfigurationProxyModel *>(this->ui->listView_projectConfigs->model());

    if(this->ui->tabWidget->currentIndex() == 0) {
        model->setFilterProjectConfig(); // TODO wird zu häufig aufgerufen
        return this->ui->listView_projectConfigs;
    } else {
        model->setFilterUserConfig(); // TODO wird zu häufig aufgerufen
        return this->ui->listView_userConfigs;
    }
}

void MeasurementConfigurationDialog::on_tabWidget_currentChanged(int index)
{
    MeasurementConfigurationProxyModel *model = static_cast<MeasurementConfigurationProxyModel *>(this->ui->listView_projectConfigs->model());

    if(index == 0) {    // project
        model->setFilterProjectConfig();
        this->isProjectConfigSelected = true;
    } else {            // user
        model->setFilterUserConfig();
        this->isProjectConfigSelected = false;
    }

    this->ui->pushButton_save_user_config->setEnabled( ! this->isProjectConfigSelected);
}

void MeasurementConfigurationDialog::on_pushButton_save_user_config_clicked()
{
    //get selected measurement config
    MeasurementConfig mConfig = this->selectedMeasurementConfig;

    emit this->saveUserConfig(mConfig);

}
