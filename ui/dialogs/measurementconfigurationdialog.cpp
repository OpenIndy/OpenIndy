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
    if(!mConfig.getIsValid()){
        this->ui->listView_measurementConfigs->selectionModel()->clearSelection();
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
    this->ui->listView_measurementConfigs->selectionModel()->clearSelection();
    this->ui->listView_measurementConfigs->selectionModel()->select(index, QItemSelectionModel::Select);
    this->updateGuiFromMeasurementConfig(mConfig);
    this->ui->widget_measurementConfigValues->setEnabled(mConfig.getIsSaved()
                                                         ? !ModelManager::getMeasurementConfigManager()->isStandardConfig(mConfig.getName())
                                                         : false);
    this->selectedMeasurementConfig = mConfig;

}

/*!
 * \brief MeasurementConfigurationDialog::on_listView_measurementConfigs_clicked
 * \param index
 */
void MeasurementConfigurationDialog::on_listView_measurementConfigs_clicked(const QModelIndex &index){

    //check index
    if(!index.isValid()){
        return;
    }

    //set widgets visibility
    this->ui->widget_measurementConfigValues->setEnabled(true);

    //get and check model
    MeasurementConfigurationProxyModel *mConfigProxyModel = static_cast<MeasurementConfigurationProxyModel *>(this->ui->listView_measurementConfigs->model());
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

    //toggle enabled state depending on what mConfig has been selected (project vs. saved)
    this->ui->widget_measurementConfigValues->setEnabled(mConfig.getIsSaved()
                                                         ? !ModelManager::getMeasurementConfigManager()->isStandardConfig(mConfig.getName())
                                                         : false);

    this->selectedMeasurementConfig = mConfig;

}

/*!
 * \brief MeasurementConfigurationDialog::measurementConfigContextMenuRequested
 * \param point
 */
void MeasurementConfigurationDialog::measurementConfigContextMenuRequested(const QPoint &point){

    //get and check index
    QModelIndex index = this->ui->listView_measurementConfigs->indexAt(point);
    if(!index.isValid() || index.parent().isValid()){
        return;
    }

    //get and check measurement config proxy model
    MeasurementConfigurationProxyModel *mConfigProxyModel = static_cast<MeasurementConfigurationProxyModel *>(this->ui->listView_measurementConfigs->model());
    if(mConfigProxyModel == NULL){
        return;
    }

    //get and check measurement config model
    MeasurementConfigurationModel *mConfigModel = static_cast<MeasurementConfigurationModel *>(mConfigProxyModel->sourceModel());
    if(mConfigModel == NULL){
        return;
    }

    //check if the selected config is a saved config
    bool isSaved = mConfigModel->getIsSaved(mConfigProxyModel->mapToSource(index));

    //set selected index
    this->ui->listView_measurementConfigs->selectionModel()->select(index, QItemSelectionModel::Select);
    this->updateGuiFromMeasurementConfig(mConfigModel->getMeasurementConfig(mConfigProxyModel->mapToSource(index)));
    if(isSaved){
        this->ui->widget_measurementConfigValues->setEnabled(true);
    }else{
        this->ui->widget_measurementConfigValues->setEnabled(false);
    }
    this->selectedMeasurementConfig = mConfigModel->getMeasurementConfig(mConfigProxyModel->mapToSource(index));

    if(isSaved){
        QMenu *menu = new QMenu();
        menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("delete config"), this, SLOT(removeSelectedMeasurementConfig()));
        menu->exec(ui->listView_measurementConfigs->mapToGlobal(point));
    }else{
        QMenu *menu = new QMenu();
        menu->addAction(QIcon(":/Images/icons/edit_add.png"), QString("clone config"), this, SLOT(cloneSelectedMeasurementConfig()));
        menu->exec(ui->listView_measurementConfigs->mapToGlobal(point));
    }

}

/*!
 * \brief MeasurementConfigurationDialog::removeSelectedMeasurementConfig
 */
void MeasurementConfigurationDialog::removeSelectedMeasurementConfig(){

    //get and check index
    QModelIndexList selection = this->ui->listView_measurementConfigs->selectionModel()->selectedIndexes();
    if(selection.size() != 1){
        return;
    }
    QModelIndex index = selection.at(0);
    if(!index.isValid() || index.parent().isValid()){
        return;
    }

    //get and check measurement config proxy model
    MeasurementConfigurationProxyModel *mConfigProxyModel = static_cast<MeasurementConfigurationProxyModel *>(this->ui->listView_measurementConfigs->model());
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
    this->ui->listView_measurementConfigs->selectionModel()->clearSelection();
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
    MeasurementConfigurationProxyModel *mConfigProxyModel = static_cast<MeasurementConfigurationProxyModel *>(this->ui->listView_measurementConfigs->model());
    if(mConfigProxyModel == NULL){
        return;
    }

    //get and check measurement config model
    MeasurementConfigurationModel *mConfigModel = static_cast<MeasurementConfigurationModel *>(mConfigProxyModel->sourceModel());
    if(mConfigModel == NULL){
        return;
    }

    mConfigModel->cloneMeasurementConfig(mConfig);

    //select the new item
    QModelIndex index = mConfigModel->getIndex(mConfig);
    this->ui->listView_measurementConfigs->selectionModel()->clearSelection();
    this->ui->listView_measurementConfigs->selectionModel()->select(index, QItemSelectionModel::Select);

}

/*!
 * \brief MeasurementConfigurationDialog::on_pushButton_add_clicked
 */
void MeasurementConfigurationDialog::on_pushButton_add_clicked(){

    MeasurementConfig mConfig;
    mConfig.setName("new config");
    mConfig.setIsSaved(true);

    //get and check measurement config proxy model
    MeasurementConfigurationProxyModel *mConfigProxyModel = static_cast<MeasurementConfigurationProxyModel *>(this->ui->listView_measurementConfigs->model());
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
    this->ui->listView_measurementConfigs->selectionModel()->clearSelection();
    this->ui->listView_measurementConfigs->selectionModel()->select(index, QItemSelectionModel::Select);
    this->updateGuiFromMeasurementConfig(mConfig);
    this->ui->listView_measurementConfigs->edit(mConfigProxyModel->mapFromSource(index));
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
void MeasurementConfigurationDialog::on_pushButton_set_clicked(){

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
 * \brief MeasurementConfigurationDialog::on_checkBox_showAll_stateChanged
 * \param arg1
 */
void MeasurementConfigurationDialog::on_checkBox_showAll_stateChanged(int arg1){

    //get measurement config model
    const MeasurementConfigurationModel &model = ModelManager::getMeasurementConfigurationModel();

    //get selected measurement config
    MeasurementConfig mConfig = this->selectedMeasurementConfig;

    //set filter
    ModelManager::getMeasurementConfigurationProxyModel().setFilter(this->ui->checkBox_showAll->isChecked());

    //set selected index
    this->ui->listView_measurementConfigs->clearSelection();
    QModelIndex index = model.getIndex(mConfig);
    if(index.isValid()){
        this->ui->listView_measurementConfigs->selectionModel()->select(index, QItemSelectionModel::Select);
        this->updateGuiFromMeasurementConfig(mConfig);
        this->ui->widget_measurementConfigValues->setEnabled(mConfig.getIsSaved()
                                                             ? !ModelManager::getMeasurementConfigManager()->isStandardConfig(mConfig.getName())
                                                             : false);
    }else{
        this->updateGuiFromMeasurementConfig(MeasurementConfig());
        this->ui->widget_measurementConfigValues->setEnabled(false);
    }

}

/*!
 * \brief MeasurementConfigurationDialog::updateGuiFromMeasurementConfig
 * \param mConfig
 */
void MeasurementConfigurationDialog::updateGuiFromMeasurementConfig(const MeasurementConfig &mConfig){

    //update selected measurement config
    this->selectedMeasurementConfig = mConfig;

    //
    this->enableUIElements(mConfig.getMeasurementType());

    //do not trigger edits while setting up measurement config
    this->ui->lineEdit_distancInterval->blockSignals(true);
    this->ui->lineEdit_maxObservations->blockSignals(true);
    this->ui->lineEdit_timeInterval->blockSignals(true);
    this->ui->checkBox_twoFace->blockSignals(true);
    this->ui->comboBox_MeasurementMode->blockSignals(true);
    this->ui->comboBox_MeasurementType->blockSignals(true);

    //set up GUI elements
    this->ui->label_configName->setText(QString("%1%2")
                                        .arg((!mConfig.getIsValid() || mConfig.getIsSaved())?"":"*")
                                        .arg(mConfig.getName()));

    this->ui->comboBox_MeasurementType->setCurrentIndex(mConfig.getMeasurementType());
    this->ui->comboBox_MeasurementMode->setCurrentIndex(mConfig.getMeasurementMode());
    this->ui->checkBox_twoFace->setChecked(mConfig.getMeasureTwoSides());

    this->ui->lineEdit_maxObservations->setText(QString::number(mConfig.getMaxObservations()));
    this->ui->lineEdit_distancInterval->setText(QString::number(mConfig.getDistanceInterval(), 'f', 4));
    this->ui->lineEdit_timeInterval->setText(QString::number(mConfig.getTimeInterval()));

    //from now on trigger edits
    this->ui->lineEdit_distancInterval->blockSignals(false);
    this->ui->lineEdit_maxObservations->blockSignals(false);
    this->ui->lineEdit_timeInterval->blockSignals(false);
    this->ui->checkBox_twoFace->blockSignals(false);
    this->ui->comboBox_MeasurementMode->blockSignals(false);
    this->ui->comboBox_MeasurementType->blockSignals(false);

}

/*!
 * \brief MeasurementConfigurationDialog::updateMeasurementConfigFromSelection
 */
void MeasurementConfigurationDialog::updateMeasurementConfigFromSelection(){

    //get and check model
    MeasurementConfigurationProxyModel *mConfigProxyModel = static_cast<MeasurementConfigurationProxyModel *>(this->ui->listView_measurementConfigs->model());
    if(mConfigProxyModel == NULL){
        return;
    }

    //get and check source model
    MeasurementConfigurationModel *mConfigModel = static_cast<MeasurementConfigurationModel *>(mConfigProxyModel->sourceModel());
    if(mConfigModel == NULL){
        return;
    }

    //get selected measurement config name
    QModelIndexList selection = this->ui->listView_measurementConfigs->selectionModel()->selectedIndexes();
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


    mConfig.setIsSaved(true);

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

    MeasurementConfigurationProxyModel *mConfigProxyModel = static_cast<MeasurementConfigurationProxyModel *>(this->ui->listView_measurementConfigs->model());
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

    //set visibility
    this->ui->widget_measurementConfigValues->setEnabled(false);

    //allow context menu for measurement config model
    this->ui->listView_measurementConfigs->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this->ui->listView_measurementConfigs, &QListView::customContextMenuRequested, this, &MeasurementConfigurationDialog::measurementConfigContextMenuRequested, Qt::AutoConnection);

    //assign delegate to measurement configs list view
    MeasurementConfigurationListDelegate *delegate = new MeasurementConfigurationListDelegate();
    this->ui->listView_measurementConfigs->setItemDelegate(delegate);

}

/*!
 * \brief MeasurementConfigurationDialog::initModels
 */
void MeasurementConfigurationDialog::initModels(){

    //init measurement config model
    this->ui->listView_measurementConfigs->setModel(&ModelManager::getMeasurementConfigurationProxyModel());

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

void MeasurementConfigurationDialog::enableUIElements(const MeasurementTypes &type) {
    switch(type) {
    case eSinglePoint_MeasurementType:
        this->ui->groupBox_Single_Point->setEnabled(true);
        this->ui->groupBox_Scan->setEnabled(false);
        break;
    case eScanTimeDependent_MeasurementType:
        this->ui->groupBox_Single_Point->setEnabled(false);
        this->ui->groupBox_Scan->setEnabled(true);
        this->ui->lineEdit_timeInterval->setEnabled(true);
        this->ui->lineEdit_distancInterval->setEnabled(false);
        this->ui->lineEdit_maxObservations->setEnabled(true);
        break;
    case eScanDistanceDependent_MeasurementType:
        this->ui->groupBox_Single_Point->setEnabled(false);
        this->ui->groupBox_Scan->setEnabled(true);
        this->ui->lineEdit_timeInterval->setEnabled(false);
        this->ui->lineEdit_distancInterval->setEnabled(true);
        this->ui->lineEdit_maxObservations->setEnabled(true);
        break;
    case eLevel_MeasurementType:
    case eDistance_MeasurementType:
    case eDirection_MeasurementType:
    case eTemperature_MeasurementType:
        this->ui->groupBox_Scan->setEnabled(false);
        this->ui->groupBox_Single_Point->setEnabled(false);
        break;
    }
}

