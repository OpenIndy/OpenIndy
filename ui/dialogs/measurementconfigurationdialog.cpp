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
    if(!mConfigModel->getIsSaved(mConfigProxyModel->mapToSource(index))){
        return;
    }

    //set selected index
    this->ui->listView_measurementConfigs->selectionModel()->select(index, QItemSelectionModel::Select);

    QMenu *menu = new QMenu();
    menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("delete config"), this, SLOT(removeSelectedMeasurementConfig()));
    menu->exec(ui->listView_measurementConfigs->mapToGlobal(point));

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

    mConfigModel->removeMeasurementConfig(mConfigProxyModel->mapToSource(index));

}

/*!
 * \brief MeasurementConfigurationDialog::on_pushButton_add_clicked
 */
void MeasurementConfigurationDialog::on_pushButton_add_clicked(){

    MeasurementConfig mConfig;
    mConfig.setName("new config");

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

}

/*!
 * \brief MeasurementConfigurationDialog::on_lineEdit_numMeas_textChanged
 * \param arg1
 */
void MeasurementConfigurationDialog::on_lineEdit_numMeas_textChanged(const QString &arg1){
    this->updateMeasurementConfigFromSelection();
}

/*!
 * \brief MeasurementConfigurationDialog::on_lineEdit_iterations_textChanged
 * \param arg1
 */
void MeasurementConfigurationDialog::on_lineEdit_iterations_textChanged(const QString &arg1){
    this->updateMeasurementConfigFromSelection();
}

/*!
 * \brief MeasurementConfigurationDialog::on_comboBox_readingType_currentIndexChanged
 * \param arg1
 */
void MeasurementConfigurationDialog::on_comboBox_readingType_currentIndexChanged(const QString &arg1){
    this->updateMeasurementConfigFromSelection();
}

/*!
 * \brief MeasurementConfigurationDialog::on_checkBox_twoFace_clicked
 */
void MeasurementConfigurationDialog::on_checkBox_twoFace_clicked(){
    this->updateMeasurementConfigFromSelection();
}

/*!
 * \brief MeasurementConfigurationDialog::on_checkBox_timeDependent_clicked
 */
void MeasurementConfigurationDialog::on_checkBox_timeDependent_clicked(){
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
 * \brief MeasurementConfigurationDialog::on_checkBox_distanceDependent_clicked
 */
void MeasurementConfigurationDialog::on_checkBox_distanceDependent_clicked(){
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

    //get selected measurement config name
    QModelIndexList selection = this->ui->listView_measurementConfigs->selectionModel()->selectedIndexes();
    if(selection.size() != 1){
        return;
    }
    QString name = selection.at(0).data().toString();

    //set the selected measurement config for the active feature
    emit this->setMeasurementConfiguration(name);

}

/*!
 * \brief MeasurementConfigurationDialog::updateGuiFromMeasurementConfig
 * \param mConfig
 */
void MeasurementConfigurationDialog::updateGuiFromMeasurementConfig(const MeasurementConfig &mConfig){

    //do not trigger edits while setting up measurement config
    this->ui->comboBox_readingType->blockSignals(true);
    this->ui->lineEdit_distancInterval->blockSignals(true);
    this->ui->lineEdit_iterations->blockSignals(true);
    this->ui->lineEdit_numMeas->blockSignals(true);
    this->ui->lineEdit_timeInterval->blockSignals(true);
    this->ui->checkBox_distanceDependent->blockSignals(true);
    this->ui->checkBox_timeDependent->blockSignals(true);
    this->ui->checkBox_twoFace->blockSignals(true);

    //set up GUI elements
    this->ui->comboBox_readingType->setCurrentText(getReadingTypeName(mConfig.getTypeOfReading()));
    this->ui->lineEdit_distancInterval->setText(QString::number(mConfig.getDistanceInterval(), 'f', 4));
    this->ui->lineEdit_iterations->setText(QString::number(mConfig.getIterations()));
    this->ui->lineEdit_numMeas->setText(QString::number(mConfig.getCount()));
    this->ui->lineEdit_timeInterval->setText(QString::number(mConfig.getTimeInterval()));
    this->ui->checkBox_distanceDependent->setChecked(mConfig.getDistanceDependent());
    this->ui->checkBox_timeDependent->setChecked(mConfig.getTimeDependent());
    this->ui->checkBox_twoFace->setChecked(mConfig.getMeasureTwoSides());

    //from now on trigger edits
    this->ui->comboBox_readingType->blockSignals(true);
    this->ui->lineEdit_distancInterval->blockSignals(true);
    this->ui->lineEdit_iterations->blockSignals(true);
    this->ui->lineEdit_numMeas->blockSignals(true);
    this->ui->lineEdit_timeInterval->blockSignals(true);
    this->ui->checkBox_distanceDependent->blockSignals(true);
    this->ui->checkBox_timeDependent->blockSignals(true);
    this->ui->checkBox_twoFace->blockSignals(true);

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

    //getmeasurement config from GUI selection
    MeasurementConfig mConfig;
    mConfig.setTypeOfReading(getReadingTypeEnum(this->ui->comboBox_readingType->currentText()));
    mConfig.setDistanceInterval(this->ui->lineEdit_distancInterval->text().toDouble());
    mConfig.setIterations(this->ui->lineEdit_iterations->text().toInt());
    mConfig.setCount(this->ui->lineEdit_numMeas->text().toInt());
    mConfig.setTimeInterval(this->ui->lineEdit_timeInterval->text().toLong());
    mConfig.setDistanceDependent(this->ui->checkBox_distanceDependent->isChecked());
    mConfig.setTimeDependent(this->ui->checkBox_timeDependent->isChecked());
    mConfig.setMeasureTwoSides(this->ui->checkBox_twoFace->isChecked());

    //replace the selected measurement config
    mConfigModel->replaceMeasurementConfig(name, mConfig);

}

/*!
 * \brief MeasurementConfigurationDialog::showEvent
 * \param event
 */
void MeasurementConfigurationDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    event->accept();

}

/*!
 * \brief MeasurementConfigurationDialog::initGUI
 */
void MeasurementConfigurationDialog::initGUI(){

    //set default reading type
    this->ui->comboBox_readingType->setCurrentText(getReadingTypeName(ePolarReading));

    //set visibility
    this->ui->widget_measurementConfigValues->setEnabled(false);

    //allow contect menu for sensor config model
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

    //init reading types model
    this->ui->comboBox_readingType->setModel(&ModelManager::getReadingTypeNamesModel());

    //init measurement config model
    this->ui->listView_measurementConfigs->setModel(&ModelManager::getMeasurementConfigurationProxyModel());

}
