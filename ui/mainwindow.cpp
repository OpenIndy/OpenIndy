#include "mainwindow.h"
#include "ui_mainwindow.h"

/*!
 * \brief MainWindow::MainWindow
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);

    //connect controller, dialogs and status bar
    this->connectDialogs();
    this->connectController();
    this->connectStatusBar();

    //create default job
    QPointer<OiJob> job = this->control.createDefaultJob();
    this->createFeatureDialog.setCurrentJob(job);

    //assign models of ModelManager to GUI-elements
    this->assignModels();

    //init GUI elements
    this->initFeatureTableViews();
    this->initSensorPad();
    this->initToolMenus();
    this->initFilterComboBoxes();
    this->initStatusBar();
    this->initBundleView();
    this->initFilterToolBar();

    //connect bundle view
    this->connectBundleView();

    QObject::connect(&this->clipBoardUtil, &ClipBoardUtil::sendMessage, this, &MainWindow::log, Qt::AutoConnection);

    QObject::connect(&this->control, &Controller::sensorActionFinished, &this->measureBehaviorLogic, &MeasureBehaviorLogic::sensorActionFinished, Qt::AutoConnection);
    QObject::connect(&this->measureBehaviorLogic, &MeasureBehaviorLogic::measurementsFinished, this, &MainWindow::measureBehaviorLogicFinished, Qt::AutoConnection);

    //initially resize table view to fit the default job
    this->resizeTableView();

    tabifyDockWidget(this->ui->dockWidget_Console, this->ui->dockWidget_magnify);

    //set default tolerance to 0.2 and commit to the model
    this->ui->lineEdit_tolerance->setText("0.2");
    ModelManager::getFeatureDifferenceTableModel().setTolerance(0.2);

    this->ui->dockWidget_differences->setMaximumWidth(600);

    this->resizeTableView();

    //load default bundle plugin
    if(job->getBundleSystemList().size() >0){
        int bundleID = job->getBundleSystemList().at(0)->getId();
        this->loadDefaultBundlePlugIn(bundleID);
    }

    this->startAutoSave();

    this->updateCompleter();

}

/*!
 * \brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow(){
    delete ui;
}

/*!
 * \brief MainWindow::importNominalsStarted
 * Triggered by Controller whenever an nominal import task has been started
 */
void MainWindow::importNominalsStarted(){
    showCentered(this->loadingDialog);
}

/*!
 * \brief MainWindow::importNominalsFinished
 * Triggered by Controller whenever an import nominals task has been finished
 * \param success
 */
void MainWindow::importNominalsFinished(const bool &success){

    //print import success to console
    if(success){
        emit this->log("Nominals successfully imported", eInformationMessage, eMessageBoxMessage);
    }else{
        emit this->log("Nominals not imported successfully", eErrorMessage, eMessageBoxMessage);
    }
    this->resizeTableView();
    this->loadingDialog.close();
}

/*!
 * \brief MainWindow::importObservationsStarted
 */
void MainWindow::importObservationsStarted(){
    showCentered(this->loadingDialog);
}

/*!
 * \brief MainWindow::importObservationsFinished
 * \param success
 */
void MainWindow::importObservationsFinished(const bool &success){

    //print import success to console
    if(success){
        emit this->log("Observations successfully imported", eInformationMessage, eMessageBoxMessage);
    }else{
        emit this->log("Observations not imported successfully", eErrorMessage, eMessageBoxMessage);
    }

    this->loadingDialog.close();

}

/*!
 * \brief MainWindow::activeFeatureChanged
 */
void MainWindow::activeFeatureChanged(){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> feature = sourceModel->getActiveFeature();
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    //update magnify window
    this->updateMagnifyWindow(feature);

}

/*!
 * \brief MainWindow::activeStationChanged
 */
void MainWindow::activeStationChanged(){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get and check active station
    QPointer<Station> station = sourceModel->getActiveStation();
    if(station.isNull() || !station->getIsSensorSet()){
        return;
    }

    //ask the user wether to take the sensor from the last station
    QMessageBox msgBox;
    msgBox.setText("Do you want to take the sensor from the last station?");
    msgBox.setInformativeText("");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Yes:
        break;
    case QMessageBox::No:
        emit this->removeActiveStationSensor();
        break;
    }

}

/*!
 * \brief MainWindow::activeCoordinateSystemChanged
 */
void MainWindow::activeCoordinateSystemChanged(){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get and check current job
    QPointer<OiJob> job = sourceModel->getCurrentJob();
    if(job.isNull()){
        return;
    }

    //update combo box with active coordinate system
    if(!job->getActiveCoordinateSystem().isNull()){
        this->ui->comboBox_activeCoordSystem->setCurrentText(job->getActiveCoordinateSystem()->getFeatureName());
    }

}

/*!
 * \brief MainWindow::coordSystemSetChanged
 */
void MainWindow::coordSystemSetChanged(){
    this->updateSystemFilterSize();
}

/*!
 * \brief MainWindow::stationSetChanged
 */
void MainWindow::stationSetChanged(){

}

/*!
 * \brief MainWindow::trafoParamSetChanged
 */
void MainWindow::trafoParamSetChanged(){

    //activate trafo param tab
    this->ui->tabWidget_views->setCurrentWidget(this->ui->tab_trafoParam);

}

/*!
 * \brief MainWindow::availableGroupsChanged
 */
void MainWindow::availableGroupsChanged(){
    this->updateGroupFilterSize();
}

void MainWindow::activeGroupChanged()
{

}

/*!
 * \brief MainWindow::featureNameChanged
 * \param featureId
 * \param oldName
 */
void MainWindow::featureNameChanged(const int &featureId, const QString &oldName){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get and check current job
    QPointer<OiJob> job = sourceModel->getCurrentJob();
    if(job.isNull()){
        return;
    }

    //get the feature with the given id
    QPointer<FeatureWrapper> feature = job->getFeatureById(featureId);
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    if(feature->getFeatureTypeEnum() == eStationFeature || feature->getFeatureTypeEnum() == eCoordinateSystemFeature){
        this->updateSystemFilterSize();
    }

}

/*!
 * \brief MainWindow::stationSensorChanged
 * Triggered whenever the sensor of the active station has changed
 * \param featureId
 */
void MainWindow::stationSensorChanged(const int &featureId){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get type of sensor
    QPointer<Station> activeStation = sourceModel->getActiveStation();
    if(activeStation.isNull()){
        return;
    }

    //set sensor action's visibility depending on the sensor type
    SensorTypes type = activeStation->getActiveSensorType();
    QList<SensorFunctions> sensorActions = activeStation->getSupportedSensorActions();
    QStringList selfDefinedActions = activeStation->getSelfDefinedActions();
    this->activeSensorTypeChanged(type, sensorActions, selfDefinedActions);

    //TODO Absturz

}

/*!
 * \brief MainWindow::currentJobChanged
 */
void MainWindow::currentJobChanged(){

    //get current job and pass it to watch window
    QPointer<OiJob> job = ModelManager::getCurrentJob();

    for (auto it = watchWindowDialogs.begin(); it != watchWindowDialogs.end();) {
        if(it.value().isNull()) {
            delete it.value().data();
        }
        it = watchWindowDialogs.erase(it);
    }

    //set window title
    if(!job.isNull()){
        this->setWindowTitle(job->getJobName());
    }else{
        this->setWindowTitle("");
    }

    //pass job to models
    if(!this->bundleStationsModel.isNull()){
        this->bundleStationsModel->setCurrentJob(ModelManager::getCurrentJob());
    }
    if(!this->bundleGeometriesModel.isNull()){
        this->bundleGeometriesModel->setCurrentJob(ModelManager::getCurrentJob());
    }

}

/*!
 * \brief MainWindow::sensorActionStarted
 * \param name
 */
void MainWindow::sensorActionStarted(const QString &name, const bool enableFinishButton){
    this->showStatusSensor(SensorStatus::eSensorActionInProgress, name);

    this->sensorTaskInfoDialog.setDisplayMessage(name);
    this->sensorTaskInfoDialog.enableFinishButton(enableFinishButton);
    if(this->sensorTaskInfoDialog.isVisible()){
        this->sensorTaskInfoDialog.repaint();
    } else {
        showCentered(this->sensorTaskInfoDialog);
    }
}

/*!
 * \brief MainWindow::sensorActionFinished
 * \param success
 * \param msg
 */
void MainWindow::sensorActionFinished(const bool &success, const QString &msg){
    this->showStatusSensor(SensorStatus::eClearStatus, "");

    this->sensorTaskInfoDialog.enableFinishButton(false);
    this->sensorTaskInfoDialog.close();
    emit this->log(msg,
                   success ? eInformationMessage : eErrorMessage,
                   success ? eConsoleMessage     : eMessageBoxMessage);
}

/*!
 * \brief MainWindow::measurementCompleted
 * Called whenever a measurement task was successfully completed
 */
void MainWindow::measurementCompleted(){

}

/*!
 * \brief MainWindow::measurementDone plays a sound for successful and failed measurement if checked in settings
 * \param success
 */
void MainWindow::measurementDone(bool success)
{
    #if ENABLE_SOUND
    if(ModelManager::getParameterDisplayConfig().getUseSounds()){
        if(success){
            QSound::play(":/sounds/measure_success.wav");
        }else{
            QSound::play(":/sounds/measure_fail.wav");
        }
    }
    #endif
}

/*!
 * \brief MainWindow::showMessageBox
 * \param msg
 * \param msgType
 */
int MainWindow::showMessageBox(const QString &msg, const MessageTypes &msgType){

    commonMessageBox.setModal(false);
    commonMessageBox.setStandardButtons(QMessageBox::Ok);
    commonMessageBox.setText(msg);

    switch(msgType){
    case eInformationMessage:
        commonMessageBox.setIcon(QMessageBox::Information);
        break;
    case eWarningMessage:
        commonMessageBox.setIcon(QMessageBox::Warning);
        break;
    case eErrorMessage:
        commonMessageBox.setIcon(QMessageBox::Critical);
        break;
    case eCriticalMessage:
        commonMessageBox.setIcon(QMessageBox::Critical);
        break;
    case eQuestionMessage:
        commonMessageBox.setIcon(QMessageBox::Question);
        commonMessageBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        commonMessageBox.setDefaultButton(QMessageBox::No);
        commonMessageBox.setModal(true);
        break;
    }

    return commonMessageBox.exec();

}

/*!
 * \brief MainWindow::showStatusMessage
 * \param msg
 * \param msgType
 */
void MainWindow::showStatusMessage(const QString &msg, const MessageTypes &msgType){

    this->ui->statusBar->showMessage(QString("[%1] {%2} : %3")
                                     .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"))
                                     .arg(getMessageTypeName(msgType))
                                     .arg(msg));

}

/*!
 * \brief MainWindow::keyPressEvent
 * Triggered whenever the user has pressed a key
 * \param e
 */
void MainWindow::keyPressEvent(QKeyEvent *e){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //check triggered keys
    switch(e->key()){
    case Qt::Key_F3: //measure
        if(e->modifiers() == Qt::AltModifier){ //aim and measure one or more features
            this->aimAndMeasureFeatures();
        }else if(control.activeFeatureUseStablePointMeasurement()) {
            this->label_statusStablePointMeasurement->setText("stable point");
            this->control.startStablePointMeasurement();
        }else{ //normal measurement
            this->control._startMeasurement(false);
        }
        break;
    case Qt::Key_F5: // measure as dummy point
            this->control._startMeasurement(true);
        break;
    case Qt::Key_Escape: // stop or terminate all running actions
        this->label_statusStablePointMeasurement->setText("");
        this->control.stopStablePointMeasurement();
        break;

    case Qt::Key_A: //aim

        if(e->modifiers() == Qt::AltModifier){
            this->control.startAim();
        }
        break;

    case Qt::Key_F7:{ //delete observations

        //get and check the active feature
        QPointer<FeatureWrapper> feature = sourceModel->getActiveFeature();
        if(feature.isNull() || feature->getFeature().isNull()){
            return;
        }
        this->removeObservationOfActiveFeature();
        break;
    }case Qt::Key_C: //copy to clipboard

        if(e->modifiers() == Qt::CTRL){
            this->copyToClipboard();
        }
        break;
    case Qt::Key_V: //paste from clipboard

        if(e->modifiers() == Qt::CTRL){
            this->pasteFromClipboard();
        }
        break;
    case Qt::Key_F1: //open properties dialog

        //get and check the active feature
        QPointer<FeatureWrapper> feature = sourceModel->getActiveFeature();
        if(feature.isNull() || feature->getFeature().isNull()){
            return;
        }
        this->showFeatureProperties(true);
        break;
    }
}

/*!
 * \brief MainWindow::on_actionCreate_point_triggered
 */
void MainWindow::on_actionCreate_point_triggered(){
    this->createFeatureDialog.setFeatureType(ePointFeature);
    showCentered(this->createFeatureDialog);
}

/*!
 * \brief MainWindow::on_actionCreate_line_triggered
 */
void MainWindow::on_actionCreate_line_triggered(){
    this->createFeatureDialog.setFeatureType(eLineFeature);
    showCentered(this->createFeatureDialog);
}

/*!
 * \brief MainWindow::on_actionCreate_plane_triggered
 */
void MainWindow::on_actionCreate_plane_triggered(){
    this->createFeatureDialog.setFeatureType(ePlaneFeature);
    showCentered(this->createFeatureDialog);
}

/*!
 * \brief MainWindow::on_actionCreate_sphere_triggered
 */
void MainWindow::on_actionCreate_sphere_triggered(){
    this->createFeatureDialog.setFeatureType(eSphereFeature);
    showCentered(this->createFeatureDialog);
}

/*!
 * \brief MainWindow::on_actionCreate_station_triggered
 */
void MainWindow::on_actionCreate_station_triggered(){
    this->createFeatureDialog.setFeatureType(eStationFeature);
    showCentered(this->createFeatureDialog);
}

/*!
 * \brief MainWindow::on_actionCreate_coordinatesystem_triggered
 */
void MainWindow::on_actionCreate_coordinatesystem_triggered(){
    this->createFeatureDialog.setFeatureType(eCoordinateSystemFeature);
    showCentered(this->createFeatureDialog);
}

/*!
 * \brief MainWindow::on_actionCreate_scalar_entity_triggered
 */
void MainWindow::on_actionCreate_scalar_entity_triggered(){
    this->createFeatureDialog.setFeatureType(eScalarEntityAngleFeature);
    showCentered(this->createFeatureDialog);
}

/*!
 * \brief MainWindow::on_actionCreate_trafoParam_triggered
 */
void MainWindow::on_actionCreate_trafoParam_triggered(){
    this->createFeatureDialog.setFeatureType(eTrafoParamFeature);
    showCentered(this->createFeatureDialog);
}

/*!
 * \brief MainWindow::on_actionCreate_cylinder_triggered
 */
void MainWindow::on_actionCreate_cylinder_triggered(){
    this->createFeatureDialog.setFeatureType(eCylinderFeature);
    showCentered(this->createFeatureDialog);
}

/*!
 * \brief MainWindow::on_actionCreate_pointcloud_triggered
 */
/*void MainWindow::on_actionCreate_pointcloud_triggered(){
    this->createFeatureDialog.setFeatureType(ePointCloudFeature);
    showCentered(this->createFeatureDialog);
}*/

/*!
 * \brief MainWindow::on_actionCreate_circle_triggered
 */
void MainWindow::on_actionCreate_circle_triggered(){
    this->createFeatureDialog.setFeatureType(eCircleFeature);
    showCentered(this->createFeatureDialog);
}

/*!
 * \brief MainWindow::on_actionLoad_plugins_triggered
 */
void MainWindow::on_actionLoad_plugins_triggered(){
    showCentered(this->pluginLoaderDialog);
}

/*!
 * \brief MainWindow::on_actionPlugin_manager_triggered
 */
void MainWindow::on_actionPlugin_manager_triggered(){
    showCentered(this->pluginManagerDialog);
}

/*!
 * \brief MainWindow::on_actionimport_triggered
 */
void MainWindow::on_actionimport_triggered()
{
    this->importNominalDialog.show();
}

/*!
 * \brief MainWindow::on_actionexport_triggered
 */
void MainWindow::on_actionexport_triggered()
{
    this->exportDialog.show();
}

/*!
 * \brief MainWindow::on_tableView_features_clicked
 * \param index
 */
void MainWindow::on_tableView_features_clicked(const QModelIndex &index){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //set active feature
    sourceModel->setActiveFeature(model->mapToSource(index));

}

/*!
 * \brief MainWindow::on_tableView_features_doubleClicked
 * \param index
 */
void MainWindow::on_tableView_features_doubleClicked(const QModelIndex &index)
{
    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //set active feature
    sourceModel->setActiveFeature(model->mapToSource(index));

    FeatureTableColumnConfig ftc = model->getFeatureTableColumnConfig();

    if(model->getFeatureTableColumnConfig().getDisplayAttributeAt(index.column()) == eFeatureDisplayFunctions){

        QPointer<FeatureWrapper> selectedFeature = sourceModel->getCurrentJob()->getActiveFeature();

        if(selectedFeature->getStation().isNull() && selectedFeature->getCoordinateSystem().isNull()){
            this->on_actionSet_function_triggered();
        }
    }
}

/*!
 * \brief MainWindow::tableViewFeaturesSelectionChangedByKeyboard
 * Triggered whenever the user selects a feature by using keyboard
 * \param selected
 * \param deselected
 */
void MainWindow::tableViewFeaturesSelectionChangedByKeyboard(const QModelIndex &selected, const QModelIndex &deselected){
    if(selected.isValid() && deselected.isValid()){
        this->on_tableView_features_clicked(selected);
    }
}

/*!
 * \brief MainWindow::on_tableView_features_customContextMenuRequested
 * \param pos
 */
void MainWindow::on_tableView_features_customContextMenuRequested(const QPoint &pos){
    //create menu and add delete action
    QMenu *menu = new QMenu();

    //get feature table models
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel*>(this->ui->tableView_features->model());
    if(model == NULL){
        delete menu;
        return;
    }
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel*>(model->sourceModel());
    if(sourceModel == NULL){
        delete menu;
        return;
    }

    QItemSelectionModel *selectionModel = this->ui->tableView_features->selectionModel();
    QModelIndexList selection = selectionModel->selectedIndexes();;
    const bool multipleFeaturesSelected = selection.size() > 1;

    //get the selected index (where the right click was done)
    QModelIndex selectedIndex = this->ui->tableView_features->indexAt(pos);

    //get and check the feature at that index
    int id = sourceModel->getFeatureIdAtIndex(model->mapToSource(selectedIndex));
    QPointer<OiJob> job = sourceModel->getCurrentJob();
    if(job.isNull() || id < 0){
        delete menu;
        return;
    }
    QPointer<FeatureWrapper> selectedFeature = job->getFeatureById(id);
    if(selectedFeature.isNull() || selectedFeature->getFeature().isNull()){
        delete menu;
        return;
    }

    QString featureName = selectedFeature->getFeature()->getFeatureName();
    QString labelSuffix = multipleFeaturesSelected ? "ALL selected features" : featureName;

    const bool isStation = !selectedFeature->getStation().isNull();
    const bool isGeometry = !selectedFeature->getGeometry().isNull();

    //if the selected feature is the active feature
    if(selectedFeature->getFeature()->getIsActiveFeature()){

        if(isGeometry){
            menu->addAction(QIcon(":/icons/icons/toolbars/standard/function.png"),
                            QString("set function for %1").arg(featureName),
                            this, SLOT(on_actionSet_function_triggered()));
            menu->addAction(QIcon(":/icons/icons/toolbars/standard/Measurement Config.png"),
                            QString("edit measurement config for %1").arg(featureName),
                            this, SLOT(on_actionMeasurement_Configuration_triggered()));
        }

        menu->addAction(QIcon(":/Images/icons/info.png"),
                        QString("show properties of %1").arg(featureName),
                        this, SLOT(showFeatureProperties(bool)));

        menu->addAction(QIcon(":/Images/icons/button_ok.png"),
                        QString("recalc %1").arg(featureName),
                        &this->control, SLOT(recalcActiveFeature()));

        //if the active feature is a geometry
        if(isGeometry){

            if(!selectedFeature->getGeometry()->getObservations().isEmpty()) {
                menu->addAction(QString("enable all observations of %1").arg(featureName),
                                this, SLOT(enableObservationsOfActiveFeature()));

                menu->addAction(QString("disable all observations of %1").arg(featureName),
                                this, SLOT(disableObservationsOfActiveFeature()));

                menu->addAction(QIcon(":/Images/icons/cancel.png"),
                                QString("remove observations of %1").arg(featureName),
                                this, SLOT(removeObservationOfActiveFeature()));
            }

            menu->addAction(QString("aim to feature %1").arg(featureName),
                            &this->control, SLOT(startAim()));
        } else if(isStation){

            menu->addAction(QString("activate station %1").arg(featureName),
                            this, SLOT(on_actionActivate_station_triggered()));
        }

    }

    QList<int> ids;
    if(multipleFeaturesSelected) {
        for(QModelIndex idx : selection) {
            ids.append(sourceModel->getFeatureIdAtIndex(model->mapToSource(idx)));
        }

        menu->addSeparator();

        QAction *enableObservationssAction = menu->addAction(
                    QString("enable all observations of %1").arg(labelSuffix));
        connect(enableObservationssAction, &QAction::triggered,
                        this, [=]() {
                            for(int id : ids) {
                                emit this->enableObservations(id);
                            }
                        });

        QAction *disableObservationsAction = menu->addAction(
                    QString("disable all observations of %1").arg(labelSuffix));
        connect(disableObservationsAction, &QAction::triggered,
                        this, [=]() {
                            for(int id : ids) {
                                emit this->disableObservations(id);
                            }
                        });

        QAction *removeObservationsAction = menu->addAction(QIcon(":/Images/icons/cancel.png"),
                        QString("remove observations of %1").arg(labelSuffix));
        connect(removeObservationsAction, &QAction::triggered,
                        this, [=]() {
                            QMessageBox msgBox;
                            msgBox.setText(QString("Remove observations of %1?").arg(labelSuffix));
                            msgBox.setInformativeText("");
                            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                            msgBox.setDefaultButton(QMessageBox::Yes);
                            switch (msgBox.exec()) {
                            case QMessageBox::Yes:
                                for(int id : ids) {
                                    emit this->removeObservations(id);
                                }
                              break;
                            }
                        });
    }

    menu->addSeparator();
    menu->addAction(QIcon(":/Images/icons/edit_remove.png"),
                    QString("delete %1").arg(labelSuffix),
                    this, SLOT(deleteFeatures(bool)));

    menu->exec(this->ui->tableView_features->mapToGlobal(pos));

    delete menu;
}

/*!
 * \brief MainWindow::on_tableView_trafoParams_clicked
 * \param index
 */
void MainWindow::on_tableView_trafoParams_clicked(const QModelIndex &index){

    //get and check model
    TrafoParamTableProxyModel *model = static_cast<TrafoParamTableProxyModel *>(this->ui->tableView_trafoParams->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //set active feature
    sourceModel->setActiveFeature(model->mapToSource(index));

}

/*!
 * \brief MainWindow::on_tableView_trafoParams_doubleClicked
 * \param index
 */
void MainWindow::on_tableView_trafoParams_doubleClicked(const QModelIndex &index)
{
    //get and check model
    TrafoParamTableProxyModel *model = static_cast<TrafoParamTableProxyModel *>(this->ui->tableView_trafoParams->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //set active feature
    sourceModel->setActiveFeature(model->mapToSource(index));

    TrafoParamTableColumnConfig ftc = model->getTrafoParamTableColumnConfig();
    qDebug() << "col " << index.column();
    qDebug() << ftc.getDisplayAttributeAt(index.column());


    if(model->getTrafoParamTableColumnConfig().getDisplayAttributeAt(index.column()) == eTrafoParamDisplayFunctions){

        QPointer<FeatureWrapper> selectedFeature = sourceModel->getCurrentJob()->getActiveFeature();

        if(selectedFeature->getStation().isNull() && selectedFeature->getCoordinateSystem().isNull()){
            this->on_actionSet_function_triggered();
        }
    }
}

/*!
 * \brief MainWindow::tableViewTrafoParamsSelectionChangedByKeyboard
 * \param selected
 * \param deselected
 */
void MainWindow::tableViewTrafoParamsSelectionChangedByKeyboard(const QModelIndex &selected, const QModelIndex &deselected){
    if(selected.isValid() && deselected.isValid()){
        this->on_tableView_trafoParams_clicked(selected);
    }
}

/*!
 * \brief MainWindow::on_tableView_trafoParams_customContextMenuRequested
 * Show context menu for trafo param features
 * \param pos
 */
void MainWindow::on_tableView_trafoParams_customContextMenuRequested(const QPoint &pos){

    //create menu
    QMenu *menu = new QMenu();
    menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("delete selected feature(s)"), this, SLOT(deleteFeatures(bool)));

    //get trafo param table models
    TrafoParamTableProxyModel *model = static_cast<TrafoParamTableProxyModel*>(this->ui->tableView_trafoParams->model());
    if(model == NULL){
        delete menu;
        return;
    }
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel*>(model->sourceModel());
    if(sourceModel == NULL){
        delete menu;
        return;
    }

    //get the selected index (where the right click was done)
    QModelIndex selectedIndex = this->ui->tableView_trafoParams->indexAt(pos);

    //get and check the feature at that index
    int id = sourceModel->getFeatureIdAtIndex(model->mapToSource(selectedIndex));
    QPointer<OiJob> job = sourceModel->getCurrentJob();
    if(job.isNull() || id < 0){
        delete menu;
        return;
    }
    QPointer<FeatureWrapper> selectedFeature = job->getFeatureById(id);
    if(selectedFeature.isNull() || selectedFeature->getFeature().isNull()){
        delete menu;
        return;
    }

    //if the selected feature is the active feature
    if(selectedFeature->getFeature()->getIsActiveFeature()){

        if(selectedFeature->getStation().isNull() && selectedFeature->getCoordinateSystem().isNull()){
            menu->addAction(QIcon(":/icons/icons/toolbars/standard/function.png"), QString("set function for %1").arg(selectedFeature->getFeature()->getFeatureName()),
                            this, SLOT(on_actionSet_function_triggered()));
        }
        menu->addAction(QIcon(":/Images/icons/info.png"), QString("show properties of feature %1").arg(selectedFeature->getFeature()->getFeatureName()),
                        this, SLOT(showFeatureProperties(bool)));
        menu->addAction(QIcon(":/Images/icons/button_ok.png"), QString("recalc %1").arg(selectedFeature->getFeature()->getFeatureName()),
                        &this->control, SLOT(recalcActiveFeature()));

    }

    menu->exec(this->ui->tableView_features->mapToGlobal(pos));

}

/*!
 * \brief MainWindow::tableViewBundleParamsSelectionChangedByKeyboard
 * \param selected
 * \param deselected
 */
void MainWindow::tableViewBundleParamsSelectionChangedByKeyboard(const QModelIndex &selected, const QModelIndex &deselected)
{
    if(selected.isValid() && deselected.isValid()){
        this->on_tableView_bundleParameter_clicked(selected);
    }
}

/*!
 * \brief MainWindow::on_actionSet_function_triggered
 */
void MainWindow::on_actionSet_function_triggered(){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    if(!sourceModel->getActiveFeature().isNull()){
        showCentered(this->featureFunctionsDialog);
    }

}

/*!
 * \brief MainWindow::on_actionSet_sensor_triggered
 */
void MainWindow::on_actionSet_sensor_triggered(){
    showCentered(this->sensorConfigurationDialog);
}

/*!
 * \brief MainWindow::setSensorConfiguration
 * \param sConfig
 */
void MainWindow::setSensorConfiguration(const SensorConfiguration &sConfig){

    //check name
    if(!sConfig.getIsValid()){
        emit this->log("Invalid configuration name", eErrorMessage, eMessageBoxMessage);
        return;
    }

    //ask the user wether only to set the sensor config or to also connect to the sensor
    QMessageBox msgBox;
    msgBox.setText("Do you want to connect the sensor?");
    msgBox.setInformativeText("");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Yes:
        emit this->sensorConfigurationChanged(sConfig, true);
        break;
    case QMessageBox::No:
        emit this->sensorConfigurationChanged(sConfig, false);
        break;
    }

}

/*!
 * \brief MainWindow::showMoveSensorDialog
 */
void MainWindow::showMoveSensorDialog(){
    showCentered(this->moveSensorDialog);
}

/*!
 * \brief MainWindow::on_actionStationProperties_triggered
 */
void MainWindow::on_actionStationProperties_triggered(){

    //get feature table models
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel*>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel*>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> feature = sourceModel->getActiveFeature();
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    //display properties dialog for stations
    if(!feature->getStation().isNull()){

        //pass information to the station properties dialog
        this->stationPropertiesDialog.setIsActiveStation(feature->getStation()->getIsActiveStation());
        this->stationPropertiesDialog.setSensorConfiguration(feature->getStation()->getSensorConfiguration());

        showCentered(this->stationPropertiesDialog);

    }

}

/*!
 * \brief MainWindow::on_actionControl_pad_triggered
 */
void MainWindow::on_actionControl_pad_triggered(){
    if(this->ui->toolBar_controlPad->isVisible()){
        this->ui->toolBar_controlPad->setVisible(false);
    }else{
        this->ui->toolBar_controlPad->setVisible(true);
    }
}

/*!
 * \brief MainWindow::on_actionConsole_triggered
 */
void MainWindow::on_actionConsole_triggered(){
    if(this->ui->dockWidget_Console->isVisible()){
        this->ui->dockWidget_Console->setVisible(false);
    }else{
        this->ui->dockWidget_Console->setVisible(true);
    }
}

/*!
 * \brief MainWindow::on_actionMagnify_triggered
 */
void MainWindow::on_actionMagnify_triggered(){
    if(this->ui->dockWidget_magnify->isVisible()){
        this->ui->dockWidget_magnify->setVisible(false);
    }else{
        this->ui->dockWidget_magnify->setVisible(true);
    }
}

/*!
 * \brief MainWindow::on_comboBox_groups_currentIndexChanged
 * \param arg1
 */
void MainWindow::on_comboBox_groups_currentIndexChanged(const QString &arg1){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //check group name
    if(arg1.compare("") == 0){
        this->ui->comboBox_groups->setCurrentText(sourceModel->getActiveGroupName());
    }else{
        sourceModel->setActiveGroupName(arg1);
    }

    updateCompleter();
}

/*!
 * \brief MainWindow::on_comboBox_activeCoordSystem_currentIndexChanged
 * \param arg1
 */
void MainWindow::on_comboBox_activeCoordSystem_currentIndexChanged(const QString &arg1){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //check system name
    if(arg1.compare("") == 0){
        QPointer<CoordinateSystem> activeSystem = sourceModel->getActiveCoordinateSystem();
        if(!activeSystem.isNull()){
            this->ui->comboBox_activeCoordSystem->setCurrentText(activeSystem->getFeatureName());
        }
    }else{
        sourceModel->setActiveCoordinateSystem(arg1);
    }

}

/*!
 * \brief MainWindow::on_comboBox_actualNominal_currentIndexChanged
 * \param arg1
 */
void MainWindow::on_comboBox_actualNominal_currentIndexChanged(const QString &arg1){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //update actual nominal filter
    sourceModel->setActualNominalFilter(getActualNominalFilterEnum(arg1));

    updateCompleter();
}

/*!
 * \brief MainWindow::on_actionWatch_window_triggered
 */
void MainWindow::on_actionWatch_window_triggered(){
    openWatchWindow(WatchWindowBehavior::eShowAlwaysActiveFeature);
}

/*!
 * \brief MainWindow::on_actionOpen_triggered
 */
void MainWindow::on_actionOpen_triggered(){

    saveProjectIfDigestChanged();

    QString filename = QFileDialog::getOpenFileName(this, "Choose a file", ProjectConfig::getProjectPath(), "xml (*.xml)");

    this->loadProjectFile(filename);
}

void MainWindow::loadProjectFile(QString file) {
    if(file.isEmpty()){
        return;
    }

    if(!file.toLower().endsWith(".xml")) {
        return;
    }

    QFileInfo info(file);
    if(!info.exists()) {
        return;
    }

    QPointer<QFileDevice> device = new QFile(file);
    QString projectName = info.fileName();

    ProjectConfig::setProjectPath(info.absolutePath());

    //clear current selection in table view
    this->ui->tableView_features->clearSelection();
    this->ui->tableView_trafoParams->clearSelection();

    emit this->loadProject(projectName, device);
}

/*!
 * \brief MainWindow::on_actionSave_triggered
 */
void MainWindow::on_actionSave_triggered(){
    emit this->saveProject();
}

/*!
 * \brief MainWindow::on_actionSave_as_triggered
 */
void MainWindow::on_actionSave_as_triggered(){
    this->saveProjectAs();
}

void MainWindow::on_actionSave_as_template_triggered(){
    this->saveProjectAs(true);
}

/*!
 * \brief MainWindow::on_actionClose_triggered
 */
void MainWindow::on_actionClose_triggered(){
    this->close();
}

int MainWindow::saveProjectIfDigestChanged()
{
    // direct call (no "emit" use), therefore is no need to handle the event in the controller
    if(this->control.hasProjectDigestChanged()) {

        QMessageBox msgBox;
        msgBox.setWindowTitle("close project");
        msgBox.setText("Do you want to save changes?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);

        int ret = msgBox.exec();

        if(ret == QMessageBox::Yes){
            emit this->saveProject();
        }
        return ret;
    } else {
        return -1;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
   if(saveProjectIfDigestChanged() == QMessageBox::Cancel) {
       event->ignore();
   } else {
       QApplication::closeAllWindows(); // close all windows on exit e.g. watch window
       event->accept();
   }
}

/*!
 * \brief MainWindow::on_actionMeasurement_Configuration_triggered
 */
void MainWindow::on_actionMeasurement_Configuration_triggered(){

    //get feature table model
    const FeatureTableModel &model = ModelManager::getFeatureTableModel();

    //check if there is an active feature and pass its config to the measurement config dialog
    QPointer<FeatureWrapper> activeFeature = model.getActiveFeature();
    if(!activeFeature.isNull() && !activeFeature->getGeometry().isNull()){
        this->measurementConfigDialog.setMeasurementConfiguration(activeFeature->getGeometry()->getMeasurementConfig());
    }else{
        this->measurementConfigDialog.setMeasurementConfiguration(MeasurementConfig());
    }

    showCentered(this->measurementConfigDialog);

}

/*!
 * \brief MainWindow::on_actionActivate_station_triggered
 */
void MainWindow::on_actionActivate_station_triggered(){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get and check selected index
    QModelIndexList selection = this->ui->tableView_features->selectionModel()->selectedIndexes();
    if(selection.size() != 1){
        emit this->log("No station selected", eErrorMessage, eMessageBoxMessage);
        return;
    }
    QModelIndex index = selection.at(0);

    //get active feature to check station name
    QPointer<FeatureWrapper> actStation = sourceModel->getActiveFeature();
    if(actStation.isNull() || actStation->getFeature().isNull()){
        return;
    }

    //let the user confirm the task
    QMessageBox msgBox;
    msgBox.setText(QString("Do you really want to activate station %1?").arg(actStation->getFeature()->getFeatureName()));
    msgBox.setInformativeText("");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if(ret == QMessageBox::No){
        return;
    }

    //set selected station
    sourceModel->setActiveStation(model->mapToSource(index));

}

/*!
 * \brief MainWindow::on_actionView_settings_triggered
 */
void MainWindow::on_actionView_settings_triggered(){
    showCentered(this->settingsDialog);
}

/*!
 * \brief MainWindow::resizeTableView
 * Triggered whenever the user double clicks a header item of a table view
 */
void MainWindow::resizeTableView(){
    this->ui->tableView_features->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_trafoParams->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_bundleParameter->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_FeatureDifferences->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_features->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_trafoParams->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_bundleParameter->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_FeatureDifferences->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

/*!
 * \brief MainWindow::removeObservationOfActiveFeature
 */
void MainWindow::removeObservationOfActiveFeature(){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get and check the active feature
    QPointer<FeatureWrapper> feature = sourceModel->getActiveFeature();
    if(!feature.isNull() && !feature->getFeature().isNull()){

        QMessageBox msgBox;
        msgBox.setText(QString("Delete observations of feature %1?").arg(feature->getFeature()->getFeatureName()));
        msgBox.setInformativeText("");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Yes:
            emit this->removeObservations(feature->getFeature()->getId());
            break;
        }
    }
}

/*!
 * \brief MainWindow::on_actionAbout_OpenIndy_triggered
 */
void MainWindow::on_actionAbout_OpenIndy_triggered(){
    showCentered(this->aboutDialog);
}
/*!
 * \brief MainWindow::on_actionAbout_OpenIndy_triggered
 */
void MainWindow::on_actionShow_Licenses_triggered(){
    showCentered(this->showLicensesDialog);
}
/*!
 * \brief MainWindow::on_pushButton_addBundle_clicked
 */
void MainWindow::on_pushButton_addBundle_clicked(){
    emit this->addBundleSystem();
}

/*!
 * \brief MainWindow::on_pushButton_removeBundle_clicked
 */
void MainWindow::on_pushButton_removeBundle_clicked(){

    //get selected bundle system
    QModelIndexList selection = this->ui->listView_bundle->selectionModel()->selectedIndexes();
    if(selection.size() != 1){
        return;
    }
    QModelIndex index = selection.at(0);

    //get system id
    int id = ModelManager::getBundleSystemsModel().getSelectedBundleSystem(index);
    if(id < 0){
        return;
    }

    //remove bundle system
    emit this->removeBundleSystem(id);

}

/*!
 * \brief MainWindow::on_action_RunBundle_triggered
 */
void MainWindow::on_action_RunBundle_triggered(){

    //get selected bundle system
    QModelIndexList selection = this->ui->listView_bundle->selectionModel()->selectedIndexes();
    if(selection.size() != 1){
        return;
    }
    QModelIndex index = selection.at(0);

    //get system id
    int id = ModelManager::getBundleSystemsModel().getSelectedBundleSystem(index);
    if(id < 0){
        return;
    }

    //get selected bundle parameters
    //TODO zu verwendende stations ermitteln
    //TODO zu schätzende Parameter je station ermitteln

    //calculate bundle
    emit this->runBundle(id);

}

/*!
 * \brief MainWindow::on_pushButton_loadBundleTemplate_clicked
 */
void MainWindow::on_pushButton_loadBundleTemplate_clicked(){

    //get selected bundle system
    QModelIndexList bundleSelection = this->ui->listView_bundle->selectionModel()->selectedIndexes();
    if(bundleSelection.size() != 1){
        return;
    }
    QModelIndex index = bundleSelection.at(0);

    //get system id
    int id = ModelManager::getBundleSystemsModel().getSelectedBundleSystem(index);
    if(id < 0){
        return;
    }

    //get selected bundle template
    int templateIndex = this->ui->comboBox_bundleTemplate->currentIndex();
    QJsonObject bundleTemplate = ModelManager::getBundleTemplatesModel().getBundleTemplate(templateIndex);
    if(bundleTemplate.isEmpty()){
        return;
    }

    //load template
    emit this->loadBundleTemplate(id, bundleTemplate);
    this->bundleSelectionChanged();

}

/*!
 * \brief MainWindow::showFeatureProperties
 * \param checked
 */
void MainWindow::showFeatureProperties(bool checked){

    //get feature table models
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel*>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel*>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> feature = sourceModel->getActiveFeature();
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    //get parameter display config
    const ParameterDisplayConfig &dConfig = sourceModel->getParameterDisplayConfig();

    //display properties dialog for actuals
    if(!feature->getGeometry().isNull() && !feature->getGeometry()->getIsNominal()){
        showCentered(this->actualPropertiesDialog);
        return;
    }

    //display properties dialog for nominals
    if(!feature->getGeometry().isNull() && feature->getGeometry()->getIsNominal() && feature->getGeometry()->getIsSolved()){

        this->nominalPropertiesDialog.setCurrentNominal(feature->getFeature()->getId(), feature->getFeature()->getFeatureName(),
                                                        feature->getFeatureTypeEnum());

        QMap<DimensionType, UnitType> displayUnits = dConfig.getDisplayUnits();
        QMap<DimensionType, int> displayDigits = dConfig.getDisplayDigits();
        QMap<GeometryParameters, QString> parameters = feature->getGeometry()->getUnknownParameters(displayUnits, displayDigits);
        this->nominalPropertiesDialog.setUnknownNominalParameters(parameters);

        showCentered(this->nominalPropertiesDialog);

        return;

    }else if(!feature->getGeometry().isNull() && feature->getGeometry()->getIsNominal() && !feature->getGeometry()->getIsSolved()){
        emit this->log(QString("Switch to the nominal system of %1 to edit its properties").arg(feature->getGeometry()->getFeatureName()),
                       eInformationMessage, eMessageBoxMessage);
        return;
    }

    //display properties dialog for trafo params
    if(!feature->getTrafoParam().isNull()){

        this->trafoParamPropertiesDialog.setCurrentTrafoParam(feature->getFeature()->getId(), feature->getFeature()->getFeatureName());

        QMap<DimensionType, UnitType> displayUnits = dConfig.getDisplayUnits();
        QMap<DimensionType, int> displayDigits = dConfig.getDisplayDigits();
        QMap<TrafoParamParameters, QString> parameters = feature->getTrafoParam()->getUnknownParameters(displayUnits, displayDigits);
        this->trafoParamPropertiesDialog.setUnknownTrafoParamParameters(parameters);

        showCentered(this->trafoParamPropertiesDialog);

        return;

    }

    //display properties dialog for stations
    if(!feature->getStation().isNull()){

        //pass information to the station properties dialog
        this->stationPropertiesDialog.setIsActiveStation(feature->getStation()->getIsActiveStation());
        this->stationPropertiesDialog.setSensorConfiguration(feature->getStation()->getSensorConfiguration());

        showCentered(this->stationPropertiesDialog);

    }

}

/*!
 * \brief MainWindow::aimAndMeasureFeatures
 */
void MainWindow::aimAndMeasureFeatures(){

    //get feature table models
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel*>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel*>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get current job
    QPointer<OiJob> job = sourceModel->getCurrentJob();
    if(job.isNull()){
        return;
    }

    //ordered list of feature id's that are currently aimed and measured (ALT + F3)
    QList<int> measureFeatures;
    QModelIndexList selection = this->ui->tableView_features->selectionModel()->selectedIndexes();
    foreach(const QModelIndex &index, selection){
        int id = sourceModel->getFeatureIdAtIndex(model->mapToSource(index));
        if(id >= 0 && !measureFeatures.contains(id)){
            measureFeatures.append(id);
        }
    }

    this->measureBehaviorLogicStarted();
    QList<QPointer<QDialog>> dialogsToClose;
    dialogsToClose.append(&this->sensorTaskInfoDialog);
    dialogsToClose.append(&this->commonMessageBox);
    this->measureBehaviorLogic.init(&control, measureFeatures, sourceModel, dialogsToClose);
    if(this->measureBehaviorLogic.next()) {
        this->measureBehaviorLogic.measure();
    }
}

/*!
 * \brief MainWindow::deleteFeatures
 * \param checked
 */
void MainWindow::deleteFeatures(bool checked){

    //security check, if you really want to delete
    QMessageBox msgBox;
    msgBox.setWindowTitle("delete selected features");
    msgBox.setText("Do you want to delete the selected features?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    int ret = msgBox.exec();

    if(ret == QMessageBox::No){
        return;
    }

    //init variables
    QSortFilterProxyModel *model = NULL;
    QItemSelectionModel *selectionModel = NULL;
    QModelIndexList selection;

    //get models depending on the current tab view
    if(this->ui->tabWidget_views->currentWidget() == this->ui->tab_features){ //feature table view

        model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
        if(model == NULL){
            return;
        }

        //get selection
        selectionModel = this->ui->tableView_features->selectionModel();

    }else if(this->ui->tabWidget_views->currentWidget() == this->ui->tab_trafoParam){ //trafo param table view

        model = static_cast<TrafoParamTableProxyModel *>(this->ui->tableView_trafoParams->model());
        if(model == NULL){
            return;
        }

        //get selection
        selectionModel = this->ui->tableView_trafoParams->selectionModel();

    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get selected indexes
    selection = selectionModel->selectedIndexes();
    if(selection.size() <= 0){
        emit this->log("No features selected", eErrorMessage, eMessageBoxMessage);
        return;
    }
    qSort(selection);

    //get the id's of the selected features
    QSet<int> featureIds;
    foreach(const QModelIndex &index, selection){
        int id = sourceModel->getFeatureIdAtIndex(model->mapToSource(index));
        if(id >= 0){
            featureIds.insert(id);
        }
    }

    //remove the selected features
    if(featureIds.size() > 0){
        emit this->removeFeatures(featureIds);
    }

}

/*!
 * \brief MainWindow::copyToClipboard
 * Copies the current selection into the clipboard
 */
void MainWindow::copyToClipboard(){

    //init variables
    QPointer<QAbstractItemModel> model = NULL;
    QPointer<QItemSelectionModel> selectionModel = NULL;
    bool isFunctionColumnSelected = false;

    //get selection of the active table view
    // keep order of if statments
    if(this->ui->dockWidget_differences->underMouse()){ //check if the differences dock widget is under the mouse cursor to copy from this table view
        model = this->ui->tableView_FeatureDifferences->model();
        selectionModel = this->ui->tableView_FeatureDifferences->selectionModel();
    }else if(this->ui->tabWidget_views->currentWidget() == this->ui->tab_features){ //feature table view: copy displayed values or feature id !
        model = this->ui->tableView_features->model();
        selectionModel = this->ui->tableView_features->selectionModel();
        isFunctionColumnSelected = !selectionModel.isNull()
                && selectionModel->selectedIndexes().size() == 1
                && eFeatureDisplayFunctions == ModelManager::getFeatureTableColumnConfig().getDisplayAttributeAt(selectionModel->selectedIndexes().first().column());
    }else if(this->ui->tabWidget_views->currentWidget() == this->ui->tab_trafoParam){ //trafo param table view
        model = this->ui->tableView_trafoParams->model();
        selectionModel = this->ui->tableView_trafoParams->selectionModel();
        isFunctionColumnSelected = !selectionModel.isNull()
                && selectionModel->selectedIndexes().size() == 1
                && eTrafoParamDisplayFunctions == ModelManager::getTrafoParamTableColumnConfig().getDisplayAttributeAt(selectionModel->selectedIndexes().first().column());
    }else if(this->ui->tabWidget_views->currentWidget() == this->ui->tab_bundle){ // bundle param table view
        model = this->ui->tableView_bundleParameter->model();
        selectionModel = this->ui->tableView_bundleParameter->selectionModel();
    } else if(this->ui->dockWidget_differences->isActiveWindow()){
        model = this->ui->tableView_FeatureDifferences->model();
        selectionModel = this->ui->tableView_FeatureDifferences->selectionModel();
    }

    if(isFunctionColumnSelected) {
        // copy JSON to clipboard make this copy and paste action clearer: {"action":"copy function","id":8,"type":"feature"}
        QJsonObject object;
        object.insert("type", "feature");
        object.insert("id", this->control.getActiveFeature()->getFeature()->getId());
        object.insert("action", "copy function");
        clipBoardUtil.copyToClipBoard(QJsonDocument(object).toJson(QJsonDocument::Compact));

    } else { // common case: copy displayed values
        clipBoardUtil.copySelectionAsCsvToClipBoard(model, selectionModel);
    }

}

/*!
 * \brief MainWindow::copyDifferencesToClipboard
 */
void MainWindow::copyDifferencesToClipboard()
{
    //init variables
    QPointer<QAbstractItemModel> model = NULL;
    QPointer<QItemSelectionModel> selectionModel = NULL;

    model = this->ui->tableView_FeatureDifferences->model();
    selectionModel = this->ui->tableView_FeatureDifferences->selectionModel();

    clipBoardUtil.copySelectionAsCsvToClipBoard(model, selectionModel);
}

/*!
 * \brief MainWindow::pasteFromClipboard
 */
void MainWindow::pasteFromClipboard(){
    enum ProxyModelType {
        eFeatureTable,
        eTrafoParamTable,
        eBundleParameterTable
    };

    //init variables
    QPointer<QSortFilterProxyModel> model = NULL;
    QPointer<QItemSelectionModel> selectionModel = NULL;
    bool isFunctionColumnSelected = false;

    ProxyModelType proxyModelType;
    //get models depending on the current tab view
    if(this->ui->tabWidget_views->currentWidget() == this->ui->tab_features){ //feature table view
        model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
        selectionModel = this->ui->tableView_features->selectionModel();
        proxyModelType = ProxyModelType::eFeatureTable;
    }else if(this->ui->tabWidget_views->currentWidget() == this->ui->tab_trafoParam){ //trafo param table view
        model = static_cast<TrafoParamTableProxyModel *>(this->ui->tableView_trafoParams->model());
        selectionModel = this->ui->tableView_trafoParams->selectionModel();
        proxyModelType = ProxyModelType::eTrafoParamTable;
    }else if(this->ui->tabWidget_views->currentWidget() == this->ui->tab_bundle){ //bundle param table view
        model = static_cast<BundleParameterTableProxyModel *>(this->ui->tableView_bundleParameter->model());
        selectionModel = this->ui->tableView_bundleParameter->selectionModel();
        proxyModelType = ProxyModelType::eBundleParameterTable;
    }

    if(model.isNull()){
        qDebug() << "no model selected";
        return;
    }

    //get and check destination model (in the sense of copy target) the sourceModel of all upper proxy models is FeatureTableModel
    QPointer<FeatureTableModel> featureTabelModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(featureTabelModel.isNull()){
        qDebug() << "no destination model avialable";
        return;
    }

    //get selected indexes
    QModelIndexList selection = selectionModel->selectedIndexes();
    if(selection.size() <= 0){
        emit this->log("No features selected", eErrorMessage, eMessageBoxMessage);
        return;
    }

    if(isFunctionColumnSelected && selection.size() > 1) {
        emit this->log("Only select one feature to paste functions to.", eErrorMessage, eMessageBoxMessage);
        return;
    }

    qSort(selection);

    //get values from clipboard, so you can copy them
    QStringList rows;
    QClipboard *clipboard = QApplication::clipboard();
    QString content = clipboard->text();

    QJsonDocument document = QJsonDocument::fromJson(content.toUtf8());
    if(document.isObject()) { // check if content is JSON
        QJsonObject object = document.object();
        isFunctionColumnSelected = object.value("action").toString() == "copy function";
        rows.append(QString::number(object.value("id").toInt()));

    } else { // otherwise the content will be accepted as CSV data
        //seperate copy table into columns: only one column is allowed
        QStringList columns = content.split('\t');
        if(columns.size() != 1){
            return;
        }

        //seperate copy table into rows: either one or selection.size rows are allowed
        rows = content.split('\n');
        if(rows.size() != 2 && rows.size() != selection.size()+1){
            return;
        }
        rows.removeLast();
    }

    //edit entries at selected indexes
    if(rows.size() == 1){

        if( proxyModelType == ProxyModelType::eTrafoParamTable
                && isFunctionColumnSelected) {

            QMessageBox msgBox;
            msgBox.setText("Do you really want to replace existing functions?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            if(QMessageBox::Yes != msgBox.exec()) {
                return;
            }
        }

        foreach(const QModelIndex &index, selection){
            QModelIndex currentIndex = model->index(index.row(), index.column());
            featureTabelModel->setData( model->mapToSource(currentIndex),
                                rows.at(0),
                                Qt::EditRole,
                                isFunctionColumnSelected
                                    ? FeatureTableModel::EditMode::eFunctionCopyScalarInputParams | FeatureTableModel::EditMode::eFunctionCopyUsedElements
                                    : FeatureTableModel::EditMode::eDefault);
        }
    }else{
        int i = 0;
        foreach(const QModelIndex &index, selection){
            QModelIndex currentIndex = model->index(index.row(), index.column());
            featureTabelModel->setData(model->mapToSource(currentIndex), rows.at(i));
            i++;
        }
    }

}

/*!
 * \brief MainWindow::updateStatusBar
 */
void MainWindow::updateStatusBar(){

    const ParameterDisplayConfig &pConfig = ModelManager::getParameterDisplayConfig();

    //update units
    this->label_statusUnitMetric->setText(getUnitTypeName(pConfig.getDisplayUnit(eMetric)));
    this->label_statusUnitAngular->setText(getUnitTypeName(pConfig.getDisplayUnit(eAngular)));
    this->label_statusUnitTemperature->setText(getUnitTypeName(pConfig.getDisplayUnit(eTemperature)));

    //update sensor status

}

/*!
 * \brief MainWindow::bundleSelectionChanged
 * Is called whenever another bundle is selected
 */
void MainWindow::bundleSelectionChanged(){

    //check job
    if(ModelManager::getCurrentJob().isNull()){
        return;
    }

    //get selection
    QModelIndexList selection = this->ui->listView_bundle->selectionModel()->selectedIndexes();

    //update visibility depending on current selection
    if(selection.size() != 1){
        this->ui->tabWidget_bundle->setEnabled(false);
        this->ui->pushButton_removeBundle->setEnabled(false);
        this->ui->pushButton_runBundle->setEnabled(false);
        return;
    }else{
        this->ui->tabWidget_bundle->setEnabled(true);
        this->ui->pushButton_removeBundle->setEnabled(true);
        this->ui->pushButton_runBundle->setEnabled(true);
    }

    //get system id
    QModelIndex index = selection.at(0);
    int id = ModelManager::getBundleSystemsModel().getSelectedBundleSystem(index);
    if(id < 0){
        return;
    }

    //reset old parameters
    this->resetBundleView();

    //get and check bundle plugin and template
    QJsonObject bundleTemplate = this->control.getBundleTemplate(id);
    QPointer<BundleAdjustment> bundlePlugin = this->control.getBundleAdjustment(id);
    if(bundlePlugin.isNull()){
        return;
    }

    //set up scalar parameters
    ScalarInputParams scalarParams = bundlePlugin->getScalarInputParams();
    this->bundleParameterWidget->setEnabled(true);
    this->bundleParameterWidget->setIntParameter(scalarParams.intParameter);
    this->bundleParameterWidget->setDoubleParameter(scalarParams.doubleParameter);
    this->bundleParameterWidget->setStringParameter(bundlePlugin->getStringParameter(), scalarParams.stringParameter);

    //set up input stations
    QJsonArray inputStations;
    const QList<BundleStation> &stations = bundlePlugin->getInputStations();
    foreach(const BundleStation &station, stations){

        //get and check station
        QPointer<FeatureWrapper> feature = ModelManager::getCurrentJob()->getFeatureById(station.id);
        if(feature.isNull() || feature->getStation().isNull()){
            continue;
        }
        QString name = feature->getStation()->getFeatureName();

        //create json object
        QJsonObject inputStation;
        inputStation.insert("name", QJsonValue(name));
        inputStation.insert("used", QJsonValue(true));
        inputStation.insert("id", QJsonValue(station.id));
        inputStation.insert("tx", QJsonValue(station.tx));
        inputStation.insert("ty", QJsonValue(station.ty));
        inputStation.insert("tz", QJsonValue(station.tz));
        inputStation.insert("rx", QJsonValue(station.rx));
        inputStation.insert("ry", QJsonValue(station.ry));
        inputStation.insert("rz", QJsonValue(station.rz));
        inputStation.insert("m", QJsonValue(station.m));
        inputStations.append(inputStation);

    }
    this->ui->treeView_inputStations->setEnabled(true);
    this->bundleStationsModel->setBundleTemplate(bundleTemplate);
    this->bundleStationsModel->setStations(inputStations);

    //set up input geometries
    inputStations = this->bundleStationsModel->getStations();
    this->ui->treeView_inputGeometries->setEnabled(true);
    this->bundleGeometriesModel->setStations(inputStations);

    //set up result

}

/*!
 * \brief MainWindow::bundleSettingsChanged
 * Is called whenever the settings of a bundle have changed
 */
void MainWindow::bundleSettingsChanged(){

    //get selection
    QModelIndexList selection = this->ui->listView_bundle->selectionModel()->selectedIndexes();

    //get system id
    if(selection.size() != 1){
        return;
    }
    QModelIndex index = selection.at(0);
    int id = ModelManager::getBundleSystemsModel().getSelectedBundleSystem(index);
    if(id < 0){
        return;
    }

    //create parameter object
    QJsonObject param;

    //set up scalar parameters
    const QMap<QString, int> &intParams = this->bundleParameterWidget->getIntParameter();
    const QMap<QString, double> &doubleParams = this->bundleParameterWidget->getDoubleParameter();
    const QMap<QString, QString> &stringParams = this->bundleParameterWidget->getStringParameter();
    QJsonArray integerParameter, doubleParameter, stringParameter;
    QMap<QString, int>::ConstIterator intIterator;
    for(intIterator = intParams.constBegin(); intIterator != intParams.constEnd(); intIterator++){
        QJsonObject parameter;
        parameter.insert("name", intIterator.key());
        parameter.insert("value", intIterator.value());
        integerParameter.append(parameter);
    }
    QMap<QString, double>::ConstIterator doubleIterator;
    for(doubleIterator = doubleParams.constBegin(); doubleIterator != doubleParams.constEnd(); intIterator++){
        QJsonObject parameter;
        parameter.insert("name", doubleIterator.key());
        parameter.insert("value", doubleIterator.value());
        doubleParameter.append(parameter);
    }
    QMap<QString, QString>::ConstIterator stringIterator;
    for(stringIterator = stringParams.constBegin(); stringIterator != stringParams.constEnd(); intIterator++){
        QJsonObject parameter;
        parameter.insert("name", stringIterator.key());
        parameter.insert("value", stringIterator.value());
        stringParameter.append(parameter);
    }
    param.insert("integerParameter", integerParameter);
    param.insert("doubleParameter", doubleParameter);
    param.insert("stringParameter", stringParameter);

    //set up input stations
    QJsonArray stations = this->bundleStationsModel->getStations();
    param.insert("inputStations", stations);

    //update bundle geometries model
    this->bundleGeometriesModel->setStations(stations);

    emit this->updateBundleAdjustment(id, param);

}

/*!
 * \brief MainWindow::connectController
 */
void MainWindow::connectController(){

    //connect actions triggered by user to slots in controller
    QObject::connect(this, &MainWindow::log, &this->control, &Controller::log, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::addFeatures, &this->control, &Controller::addFeatures, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::importNominals, &this->control, &Controller::importNominals, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::exportFeatures, &this->control, &Controller::exportFeatures, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::sensorConfigurationChanged, &this->control, &Controller::setSensorConfig, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::sensorConfigurationsEdited, &this->control, &Controller::sensorConfigurationsEdited, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::measurementConfigurationChanged, &this->control, &Controller::measurementConfigurationChanged, Qt::AutoConnection);
    QObject::connect(this, static_cast<void (MainWindow::*)()>(&MainWindow::saveProject),
                     &this->control, static_cast<void (Controller::*)()>(&Controller::saveProject), Qt::AutoConnection);
    QObject::connect(this, static_cast<void (MainWindow::*)(const QString&)>(&MainWindow::saveProject),
                     &this->control, static_cast<void (Controller::*)(const QString&)>(&Controller::saveProject), Qt::AutoConnection);
    QObject::connect(this, &MainWindow::loadProject, &this->control, &Controller::loadProject, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::removeObservations, &this->control, &Controller::removeObservations, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::removeAllObservations, &this->control, &Controller::removeAllObservations, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::enableObservations, &this->control, &Controller::enableObservations, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::disableObservations, &this->control, &Controller::disableObservations, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::removeFeatures, &this->control, &Controller::removeFeatures, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::removeActiveStationSensor, &this->control, &Controller::removeActiveStationSensor, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::addBundleSystem, &this->control, &Controller::addBundleSystem, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::removeBundleSystem, &this->control, &Controller::removeBundleSystem, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::loadBundleTemplate, &this->control, &Controller::loadBundleTemplate, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::runBundle, &this->control, &Controller::runBundle, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::runBundle, ModelManager::getBundleGeometriesModel(), &BundleGeometriesModel::updateModel, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::updateBundleAdjustment, &this->control, &Controller::updateBundleAdjustment, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::loadAndSaveConfigs, &this->control, &Controller::initConfigs, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::createTemplateFromJob, &this->control, &Controller::createTemplateFromJob, Qt::AutoConnection);

    //connect actions triggered by controller to slots in main window
    QObject::connect(&this->control, &Controller::nominalImportStarted, this, &MainWindow::importNominalsStarted, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::nominalImportFinished, this, &MainWindow::importNominalsFinished, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::observationImportStarted, this, &MainWindow::importObservationsStarted, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::observationImportFinished, this, &MainWindow::importObservationsFinished, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::activeFeatureChanged, this, &MainWindow::activeFeatureChanged, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::activeCoordinateSystemChanged, this, &MainWindow::activeCoordinateSystemChanged, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::stationSensorChanged, this, &MainWindow::stationSensorChanged, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::sensorActionStarted, this, &MainWindow::sensorActionStarted, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::sensorActionFinished, this, &MainWindow::sensorActionFinished, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::measurementCompleted, this, &MainWindow::measurementCompleted, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::measurementDone, this, &MainWindow::measurementDone, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::measurementDone, this, &MainWindow::autoSwitchToNextFeature, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::showMessageBox, this, &MainWindow::showMessageBox, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::showStatusMessage, this, &MainWindow::showStatusMessage, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::sensorStatus, this, &MainWindow::showStatusSensor, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::availableGroupsChanged, this, &MainWindow::availableGroupsChanged, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::coordSystemSetChanged, this, &MainWindow::coordSystemSetChanged, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::featureNameChanged, this, &MainWindow::featureNameChanged, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::currentJobChanged, this, &MainWindow::currentJobChanged, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::trafoParamSetChanged, this, &MainWindow::trafoParamSetChanged, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::saveAsTriggered, this, &MainWindow::on_actionSave_as_triggered, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::activeStationChanged, this, &MainWindow::activeStationChanged, Qt::AutoConnection);

    QObject::connect(&this->control, &Controller::featureCreated, this, &MainWindow::featureCreated, Qt::AutoConnection);

    QObject::connect(&this->control, &Controller::requestMessageBoxTrafoParam, this, &MainWindow::createMessageBoxTrafoParamWarning, Qt::AutoConnection);

    QObject::connect(&this->control, &Controller::featureSetChanged, this, &MainWindow::updateCompleter, Qt::AutoConnection);   // create / remove feature
    QObject::connect(&this->control, &Controller::featureNameChanged, this, &MainWindow::updateCompleter, Qt::AutoConnection);  // update
    QObject::connect(&this->control, &Controller::currentJobChanged, this, &MainWindow::updateCompleter, Qt::AutoConnection);   // load job
}

/*!
 * \brief MainWindow::connectDialogs
 */
void MainWindow::connectDialogs(){

    //connect create feature dialog
    QObject::connect(&this->createFeatureDialog, &CreateFeatureDialog::addFeatures, this, &MainWindow::addFeatures, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::featureCreated, &this->createFeatureDialog, &CreateFeatureDialog::featureCreated, Qt::AutoConnection);

    //connect console
    QObject::connect(Console::getInstance().data(), &Console::appendMessageToConsole, this->ui->plainTextEdit, &QPlainTextEdit::appendPlainText, Qt::QueuedConnection);

    //connect import / export dialogs
    QObject::connect(&this->importNominalDialog, &ImportNominalDialog::startImport, this, &MainWindow::importNominals, Qt::AutoConnection);
    QObject::connect(&this->exportDialog, &ExportDialog::startExport, this, &MainWindow::exportFeatures, Qt::AutoConnection);
    QObject::connect(&this->importNominalDialog, &ImportNominalDialog::startImport, &this->control, &Controller::setExchangeParams, Qt::AutoConnection);

    //connect loading dialog
    QObject::connect(&this->control, &Controller::nominalImportProgressUpdated, &this->loadingDialog, &LoadingDialog::updateProgress, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::observationImportProgressUpdated, &this->loadingDialog, &LoadingDialog::updateProgress, Qt::AutoConnection);

    //connect sensor config dialog
    QObject::connect(&this->sensorConfigurationDialog, &SensorConfigurationDialog::setSensorConfig, this, &MainWindow::setSensorConfiguration, Qt::AutoConnection);
    QObject::connect(&this->sensorConfigurationDialog, &SensorConfigurationDialog::sensorConfigurationsEdited, this, &MainWindow::sensorConfigurationsEdited, Qt::AutoConnection);

    //connect measurement config dialog
    QObject::connect(&this->measurementConfigDialog, &MeasurementConfigurationDialog::measurementConfigurationChanged, this, &MainWindow::measurementConfigurationChanged, Qt::AutoConnection);

    //connect move sensor dialog
    QObject::connect(&this->moveSensorDialog, &MoveSensorDialog::moveSensor, &this->control, &Controller::startMove, Qt::AutoConnection);

    //connect plugin manager dialog
    QObject::connect(&this->pluginManagerDialog, &PluginManagerDialog::loadPlugins, &this->pluginLoaderDialog, &PluginLoaderDialog::show, Qt::AutoConnection);

    //connect feature functions dialog
    QObject::connect(&this->featureFunctionsDialog, &FeatureFunctionsDialog::recalculateActiveFeature, &this->control, &Controller::recalcActiveFeature, Qt::AutoConnection);

    //connect settings dialog
    QObject::connect(&this->settingsDialog, &SettingsDialog::setDisplayConfig, &this->control, &Controller::setParameterDisplayConfig, Qt::AutoConnection);

    //connect actual properties dialog
    QObject::connect(&this->actualPropertiesDialog, &ActualPropertiesDialog::importObservations, &this->control, &Controller::importObservations, Qt::AutoConnection);
    QObject::connect(&this->actualPropertiesDialog, &ActualPropertiesDialog::removeObservationsById, &this->control, &Controller::removeObservationsById, Qt::AutoConnection);
    QObject::connect(&this->actualPropertiesDialog, &ActualPropertiesDialog::sendMessage, this, &MainWindow::log, Qt::AutoConnection);

    //connect nominal properties dialog
    QObject::connect(&this->nominalPropertiesDialog, &NominalPropertiesDialog::nominalParametersChanged, &this->control, &Controller::setNominalParameters, Qt::AutoConnection);

    //connect trafo param properties dialog
    QObject::connect(&this->trafoParamPropertiesDialog, &TrafoParamPropertiesDialog::trafoParamParametersChanged, &this->control, &Controller::setTrafoParamParameters, Qt::AutoConnection);

    //connect station properties dialog
    QObject::connect(&this->stationPropertiesDialog, &StationPropertiesDialog::openSensorConfigurationDialog, this, &MainWindow::on_actionSet_sensor_triggered, Qt::AutoConnection);
    QObject::connect(&this->stationPropertiesDialog, &StationPropertiesDialog::sensorConfigurationChanged, &this->control, &Controller::sensorConfigurationUpdated, Qt::AutoConnection);
    QObject::connect(&this->stationPropertiesDialog, &StationPropertiesDialog::sendMessage, this, &MainWindow::log, Qt::AutoConnection);

    // connect SensorTaskInfo dialog
    QObject::connect(&this->sensorTaskInfoDialog, &SensorTaskInfoDialog::finishMeasurement, &this->control, &Controller::finishMeasurement, Qt::AutoConnection);

}

/*!
 * \brief MainWindow::connectStatusBar
 */
void MainWindow::connectStatusBar(){

    //connect unit updates
    QObject::connect(&this->control, &Controller::updateStatusBar, this, &MainWindow::updateStatusBar, Qt::AutoConnection);

}

/*!
 * \brief MainWindow::connectBundleView
 */
void MainWindow::connectBundleView(){

    //connect bundle selection
    QObject::connect(this->ui->listView_bundle->selectionModel(), &QItemSelectionModel::selectionChanged,
                     this, &MainWindow::bundleSelectionChanged, Qt::AutoConnection);
    QObject::connect(&ModelManager::getBundleSystemsModel(), &BundleSystemsModel::layoutChanged,
                     this, &MainWindow::bundleSelectionChanged, Qt::AutoConnection);

    //connect scalar parameters widget
    QObject::connect(this->bundleParameterWidget, &ScalarParameterWidget::scalarParametersChanged,
                     this, &MainWindow::bundleSettingsChanged, Qt::AutoConnection);

    //connect input stations model
    QObject::connect(this->bundleStationsModel, &BundleStationsModel::stationsChanged,
                     this, &MainWindow::bundleSettingsChanged, Qt::AutoConnection);

}

/*!
 * \brief MainWindow::assignModels
 * Assign the models of ModelManager to GUI-elements in MainWindow
 */
void MainWindow::assignModels(){

    //assign feature table views and delegates
    this->ui->tableView_features->setModel(&ModelManager::getFeatureTableProxyModel());
    FeatureTableDelegate *featureTableDelegate = new FeatureTableDelegate();
    this->ui->tableView_features->setItemDelegate(featureTableDelegate);
    this->ui->tableView_trafoParams->setModel(&ModelManager::getTrafoParamTableProxyModel());
    TrafoParamDelegate *trafoParamTableDelegate = new TrafoParamDelegate();
    this->ui->tableView_trafoParams->setItemDelegate(trafoParamTableDelegate);
    this->ui->tableView_bundleParameter->setModel(&ModelManager::getBundleParameterTableProxyModel());
    TrafoParamDelegate *bundleParamTableDelegate = new TrafoParamDelegate();
    this->ui->tableView_bundleParameter->setItemDelegate(bundleParamTableDelegate);
    this->ui->tableView_FeatureDifferences->setModel(&ModelManager::getFeatureDifferenceProxyModel());

    //assign groups model
    this->ui->comboBox_groups->setModel(&ModelManager::getGroupNamesModel());

    //assign coordinate system model
    this->ui->comboBox_activeCoordSystem->setModel(&ModelManager::getCoordinateSystemsModel());

    //assign actual nominal filter model
    this->ui->comboBox_actualNominal->setModel(&ModelManager::getActualNominalFilterModel());

    //assign bundle models
    this->ui->listView_bundle->setModel(&ModelManager::getBundleSystemsModel());
    this->ui->comboBox_bundleTemplate->setModel(&ModelManager::getBundleTemplatesModel());
    this->bundleStationsModel = ModelManager::getBundleStationsModel(this);
    this->bundleStationsModel->setCurrentJob(ModelManager::getCurrentJob());
    this->ui->treeView_inputStations->setModel(this->bundleStationsModel);
    this->bundleGeometriesModel = ModelManager::getBundleGeometriesModel(this);
    this->bundleGeometriesModel->setCurrentJob(ModelManager::getCurrentJob());
    this->ui->treeView_inputGeometries->setModel(this->bundleGeometriesModel);

    QObject::connect(&ModelManager::getFeatureTableModel(),&FeatureTableModel::recalcActiveFeature, &this->control, &Controller::recalcActiveFeature, Qt::AutoConnection);

}

/*!
 * \brief MainWindow::initFeatureTableViews
 */
void MainWindow::initFeatureTableViews(){

    //resize rows and columns to table view contents on double click
    QObject::connect(this->ui->tableView_features->horizontalHeader(), &QHeaderView::sectionDoubleClicked, this, &MainWindow::resizeTableView, Qt::AutoConnection);
    //this->ui->tableView_features->verticalHeader()->setDefaultSectionSize(22);
    QObject::connect(this->ui->tableView_features->verticalHeader(), &QHeaderView::sectionDoubleClicked, this, &MainWindow::resizeTableView, Qt::AutoConnection);
    QObject::connect(this->ui->tableView_trafoParams->horizontalHeader(), &QHeaderView::sectionDoubleClicked, this, &MainWindow::resizeTableView, Qt::AutoConnection);
    //this->ui->tableView_trafoParams->verticalHeader()->setDefaultSectionSize(22);
    QObject::connect(this->ui->tableView_trafoParams->verticalHeader(), &QHeaderView::sectionDoubleClicked, this, &MainWindow::resizeTableView, Qt::AutoConnection);
    QObject::connect(this->ui->tableView_FeatureDifferences->horizontalHeader(), &QHeaderView::sectionDoubleClicked, this, &MainWindow::resizeTableView, Qt::AutoConnection);
    QObject::connect(this->ui->tableView_FeatureDifferences->verticalHeader(), &QHeaderView::sectionDoubleClicked, this, &MainWindow::resizeTableView, Qt::AutoConnection);
    QObject::connect(this->ui->tableView_bundleParameter->horizontalHeader(), &QHeaderView::sectionDoubleClicked, this, &MainWindow::resizeTableView, Qt::AutoConnection);
    QObject::connect(this->ui->tableView_bundleParameter->verticalHeader(), &QHeaderView::sectionDoubleClicked, this, &MainWindow::resizeTableView, Qt::AutoConnection);

    //enable context menu
    this->ui->tableView_features->setContextMenuPolicy(Qt::CustomContextMenu);
    this->ui->tableView_trafoParams->setContextMenuPolicy(Qt::CustomContextMenu);
    this->ui->tableView_bundleParameter->setContextMenuPolicy(Qt::CustomContextMenu);
    this->ui->tableView_FeatureDifferences->setContextMenuPolicy(Qt::CustomContextMenu);

    //change active feature by using up and down keys
    QObject::connect(this->ui->tableView_features->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::tableViewFeaturesSelectionChangedByKeyboard, Qt::AutoConnection);
    QObject::connect(this->ui->tableView_trafoParams->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::tableViewTrafoParamsSelectionChangedByKeyboard, Qt::AutoConnection);
    QObject::connect(this->ui->tableView_bundleParameter->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::tableViewBundleParamsSelectionChangedByKeyboard, Qt::AutoConnection);

}

/*!
 * \brief MainWindow::initSensorPad
 */
void MainWindow::initSensorPad(){

    //clear sensor pad and hide it as default
    this->ui->toolBar_controlPad->clear();
    this->ui->toolBar_controlPad->setVisible(false);

    //initialize sensor actions
    this->actionConnect = new QAction(0);
    this->actionConnect->setText("connect");
    this->actionDisconnect = new QAction(0);
    this->actionDisconnect->setText("disconnect");
    this->actionInitialize = new QAction(0);
    this->actionInitialize->setText("initialize");
    this->actionMeasure = new QAction(0);
    this->actionMeasure->setShortcut(Qt::Key_F3);
    this->actionMeasure->setText("measure (F3)");
    this->actionAim = new QAction(0);
    this->actionAim->setShortcut(QKeySequence(Qt::ALT + Qt::Key_A));
    this->actionAim->setText("aim (ALT + A)");
    this->actionMove = new QAction(0);
    this->actionMove->setText("move");
    this->actionHome = new QAction(0);
    this->actionHome->setShortcut(Qt::Key_F9);
    this->actionHome->setText("home (F9)");
    this->actionChangeMotorState = new QAction(0);
    this->actionChangeMotorState->setText("change motor state");
    this->actionToggleSightOrientation = new QAction(0);
    this->actionToggleSightOrientation->setText("toggle sight orientation");
    this->actionCompensation = new QAction(0);
    this->actionCompensation->setText("compensation");

    this->actionSearch = new QAction(0);
    this->actionSearch->setShortcut(QKeySequence(Qt::ALT + Qt::Key_S));
    this->actionSearch->setText("search (Alt + S)");

    //add the actions to the sensor pad
    this->ui->toolBar_controlPad->addAction(this->actionConnect);
    this->ui->toolBar_controlPad->addAction(this->actionDisconnect);
    this->ui->toolBar_controlPad->addAction(this->actionInitialize);
    this->ui->toolBar_controlPad->addAction(this->actionMeasure);
    this->ui->toolBar_controlPad->addAction(this->actionAim);
    this->ui->toolBar_controlPad->addAction(this->actionMove);
    this->ui->toolBar_controlPad->addAction(this->actionHome);
    this->ui->toolBar_controlPad->addAction(this->actionChangeMotorState);
    this->ui->toolBar_controlPad->addAction(this->actionToggleSightOrientation);
    this->ui->toolBar_controlPad->addAction(this->actionCompensation);
    this->ui->toolBar_controlPad->addAction(this->actionSearch);

    //disable and hide actions as default
    this->actionConnect->setVisible(false);
    this->actionConnect->setEnabled(false);
    this->actionDisconnect->setVisible(false);
    this->actionDisconnect->setEnabled(false);
    this->actionInitialize->setVisible(false);
    this->actionInitialize->setEnabled(false);
    this->actionMeasure->setVisible(false);
    this->actionMeasure->setEnabled(false);
    this->actionAim->setVisible(false);
    this->actionAim->setEnabled(false);
    this->actionMove->setVisible(false);
    this->actionMove->setEnabled(false);
    this->actionHome->setVisible(false);
    this->actionHome->setEnabled(false);
    this->actionChangeMotorState->setVisible(false);
    this->actionChangeMotorState->setEnabled(false);
    this->actionToggleSightOrientation->setVisible(false);
    this->actionToggleSightOrientation->setEnabled(false);
    this->actionCompensation->setVisible(false);
    this->actionCompensation->setEnabled(false);
    this->actionSearch->setVisible(false);
    this->actionSearch->setEnabled(false);

    //connect actions
    QObject::connect(this->actionConnect, &QAction::triggered, &this->control, &Controller::startConnect, Qt::AutoConnection);
    QObject::connect(this->actionDisconnect, &QAction::triggered, &this->control, &Controller::startDisconnect, Qt::AutoConnection);
    QObject::connect(this->actionInitialize, &QAction::triggered, &this->control, &Controller::startInitialize, Qt::AutoConnection);
    QObject::connect(this->actionMeasure, &QAction::triggered, &this->control, &Controller::startMeasurement, Qt::AutoConnection);
    QObject::connect(this->actionAim, &QAction::triggered, &this->control, &Controller::startAim, Qt::AutoConnection);
    QObject::connect(this->actionHome, &QAction::triggered, &this->control, &Controller::startHome, Qt::AutoConnection);
    QObject::connect(this->actionChangeMotorState, &QAction::triggered, &this->control, &Controller::startChangeMotorState, Qt::AutoConnection);
    QObject::connect(this->actionToggleSightOrientation, &QAction::triggered, &this->control, &Controller::startToggleSight, Qt::AutoConnection);
    QObject::connect(this->actionCompensation, &QAction::triggered, &this->control, &Controller::startCompensation, Qt::AutoConnection);
    QObject::connect(this->actionMove, &QAction::triggered, this, &MainWindow::showMoveSensorDialog, Qt::AutoConnection);
    QObject::connect(this->actionSearch, &QAction::triggered, &this->control, &Controller::startSearch, Qt::AutoConnection);

}

/*!
 * \brief MainWindow::initToolMenus
 */
void MainWindow::initToolMenus(){

    //get all available tool plugins
    const QList<QPointer<Tool> > &tools = this->control.getAvailableTools();

    //add each tool as menu entry
    foreach(const QPointer<Tool> &tool, tools){

        if(tool.isNull()){
            continue;
        }

        //create an action and add it to the corresponding menu item
        ToolAction *action;
        action = new ToolAction();
        action->setToolName(tool->getMetaData().name);
        action->setPluginName(tool->getMetaData().pluginName);
        action->setText(tool->getMetaData().name);
        switch(tool->getToolType()){
        case eReportTool:
            this->ui->menuReport->addAction(action);
            break;
        case eUnknownTool:
            this->ui->menuTools->addAction(action);
            break;
        }

        //connect the triggered event of the action
        QObject::connect(action, &ToolAction::openToolWidget, &this->control, &Controller::showToolWidget);

    }

}

/*!
 * \brief MainWindow::initFilterComboBoxes
 * Initialize the size of filter combo boxes (group names, systems etc.)
 */
void MainWindow::initFilterComboBoxes(){
    this->updateGroupFilterSize();
    this->updateSystemFilterSize();
    this->updateActualNominalFilterSize();
}

void MainWindow::initFilterToolBar() {
    this->ui->toolBar_filter->addWidget(this->ui->groupBox_orderBy);
    this->ui->toolBar_filter->addWidget(this->ui->groupBox_activeCoordSystem);
    this->ui->toolBar_filter->addWidget(this->ui->groupBox_filter);
    this->ui->toolBar_filter->addWidget(this->ui->groupBox_search);
}

/*!
 * \brief MainWindow::initStatusBar
 */
void MainWindow::initStatusBar(){

    //create GUI elements
    this->label_statusSensor = new QLabel();
    // "blink" animation
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    this->label_statusSensor->setGraphicsEffect(opacityEffect);
    this->animation_label_statusSensor = new QPropertyAnimation(opacityEffect, "opacity", this);
    this->animation_label_statusSensor->setStartValue(-1.0);
    this->animation_label_statusSensor->setEndValue(2.0);
    this->animation_label_statusSensor->setLoopCount(-1);
    this->animation_label_statusSensor->setDuration(500);
    this->animation_label_statusSensor->setEasingCurve(QEasingCurve::OutQuad);


    this->label_statusUnitMetric = new QLabel();
    this->label_statusUnitAngular = new QLabel();
    this->label_statusUnitTemperature = new QLabel();
    this->label_statusStablePointMeasurement = new QLabel();

    //format GUI elements
    this->label_statusUnitMetric->setMinimumWidth(50);
    this->label_statusUnitMetric->setAlignment(Qt::AlignHCenter);
    this->label_statusUnitAngular->setMinimumWidth(50);
    this->label_statusUnitAngular->setAlignment(Qt::AlignHCenter);
    this->label_statusUnitTemperature->setMinimumWidth(50);
    this->label_statusUnitTemperature->setAlignment(Qt::AlignHCenter);
    this->label_statusStablePointMeasurement->setStyleSheet("QLabel { color : orangered;  font-weight: bold;}");

    //add GUI elements to status bar
    this->ui->statusBar->addPermanentWidget(this->label_statusStablePointMeasurement);
    this->ui->statusBar->addPermanentWidget(this->label_statusSensor);
    this->ui->statusBar->addPermanentWidget(this->label_statusUnitMetric);
    this->ui->statusBar->addPermanentWidget(this->label_statusUnitAngular);
    this->ui->statusBar->addPermanentWidget(this->label_statusUnitTemperature);

    //show initial status
    this->updateStatusBar();

}

/*!
 * \brief MainWindow::initBundleView
 */
void MainWindow::initBundleView(){

    //load bundle templates
    ModelManager::getBundleTemplatesModel().loadTemplates();
    this->ui->comboBox_bundleTemplate->setCurrentIndex(0);

    //set initial visibility
    this->ui->tabWidget_bundle->setEnabled(false);
    this->ui->pushButton_removeBundle->setEnabled(false);
    this->ui->pushButton_runBundle->setEnabled(false);

    //init bundle parameter widget
    QGridLayout *extraParameterLayout = new QGridLayout();
    this->ui->widget_bundleParameters->setLayout(extraParameterLayout);
    this->bundleParameterWidget = new ScalarParameterWidget();
    extraParameterLayout->addWidget(this->bundleParameterWidget);

}

/*!
 * \brief MainWindow::activeSensorTypeChanged
 * Depending on the active stations's sensor set visibility of sensor pad actions
 * \param type
 * \param supportedActions
 * \param selfDefinedActions
 */
void MainWindow::activeSensorTypeChanged(const SensorTypes &type, const QList<SensorFunctions> &supportedActions, const QStringList &selfDefinedActions){

    //set visibility of standard actions
    switch(type){
    case eLaserTracker:

        this->actionConnect->setVisible(true);
        this->actionConnect->setEnabled(true);
        this->actionDisconnect->setVisible(true);
        this->actionDisconnect->setEnabled(true);
        this->actionInitialize->setVisible(true);
        this->actionInitialize->setEnabled(true);
        this->actionMeasure->setVisible(true);
        this->actionMeasure->setEnabled(true);
        this->actionAim->setVisible(true);
        this->actionAim->setEnabled(true);
        this->actionMove->setVisible(true);
        this->actionMove->setEnabled(true);
        this->actionHome->setVisible(true);
        this->actionHome->setEnabled(true);
        this->actionChangeMotorState->setVisible(true);
        this->actionChangeMotorState->setEnabled(true);
        this->actionToggleSightOrientation->setVisible(true);
        this->actionToggleSightOrientation->setEnabled(true);
        this->actionCompensation->setVisible(true);
        this->actionCompensation->setEnabled(true);
        this->actionSearch->setVisible(supportedActions.contains(SensorFunctions::eSearch));
        this->actionSearch->setEnabled(supportedActions.contains(SensorFunctions::eSearch));

        break;

    case eTotalStation:

        this->actionConnect->setVisible(true);
        this->actionConnect->setEnabled(true);
        this->actionDisconnect->setVisible(true);
        this->actionDisconnect->setEnabled(true);
        this->actionInitialize->setVisible(false);
        this->actionInitialize->setEnabled(false);
        this->actionMeasure->setVisible(true);
        this->actionMeasure->setEnabled(true);
        this->actionAim->setVisible(true);
        this->actionAim->setEnabled(true);
        this->actionMove->setVisible(true);
        this->actionMove->setEnabled(true);
        this->actionHome->setVisible(false);
        this->actionHome->setEnabled(false);
        this->actionChangeMotorState->setVisible(false);
        this->actionChangeMotorState->setEnabled(false);
        this->actionToggleSightOrientation->setVisible(true);
        this->actionToggleSightOrientation->setEnabled(true);
        this->actionCompensation->setVisible(false);
        this->actionCompensation->setEnabled(false);

        break;

    default:

        this->actionConnect->setVisible(false);
        this->actionConnect->setEnabled(false);
        this->actionDisconnect->setVisible(false);
        this->actionDisconnect->setEnabled(false);
        this->actionInitialize->setVisible(false);
        this->actionInitialize->setEnabled(false);
        this->actionMeasure->setVisible(false);
        this->actionMeasure->setEnabled(false);
        this->actionAim->setVisible(false);
        this->actionAim->setEnabled(false);
        this->actionMove->setVisible(false);
        this->actionMove->setEnabled(false);
        this->actionHome->setVisible(false);
        this->actionHome->setEnabled(false);
        this->actionChangeMotorState->setVisible(false);
        this->actionChangeMotorState->setEnabled(false);
        this->actionToggleSightOrientation->setVisible(false);
        this->actionToggleSightOrientation->setEnabled(false);
        this->actionCompensation->setVisible(false);
        this->actionCompensation->setEnabled(false);

        break;

    }

    //delete old self defined actions
    foreach(const QPointer<QAction> &action, this->selfDefinedActions){
        if(!action.isNull()){
            this->ui->toolBar_controlPad->removeAction(action.data());
            delete action;
        }
    }
    this->selfDefinedActions.clear();

    //delete old signal mapper
    if(!this->customActionMapper.isNull()){
        delete this->customActionMapper;
    }
    this->customActionMapper = new QSignalMapper();

    //add new self defined actions
    foreach(const QString &action, selfDefinedActions){
        QRegExp rx("([\\w ]+)(\\(([\\w]+\\+[\\w]+)\\))?"); // extract label and shortcut from e.g. "searchSMR(Alt+S)"
        rx.setPatternSyntax(QRegExp::RegExp);
        rx.indexIn(action);

        QKeySequence actionShortCut = QKeySequence::fromString(rx.cap(3));
        QString actionLabel = QString("%1%2").arg(rx.cap(1)).arg(actionShortCut.isEmpty() || rx.cap(3).isEmpty() ? "" : " " + rx.cap(2));
        QString actionCommand = rx.cap(1);

        QPointer<QAction> customAction = new QAction(0);
        customAction->setShortcut(actionShortCut);
        customAction->setText(actionLabel);
        this->selfDefinedActions.append(customAction);
        this->ui->toolBar_controlPad->addAction(customAction.data());
        QObject::connect(customAction.data(), SIGNAL(triggered()), this->customActionMapper.data(), SLOT(map()), Qt::AutoConnection);
        this->customActionMapper->setMapping(customAction.data(), actionCommand);
    }
    QObject::connect(this->customActionMapper.data(), SIGNAL(mapped(const QString&)), &this->control, SLOT(startCustomAction(const QString&)), Qt::AutoConnection);

}

/*!
 * \brief MainWindow::updateMagnifyWindow
 * \param feature
 */
void MainWindow::updateMagnifyWindow(const QPointer<FeatureWrapper> &feature){

    //update contents
    this->ui->label_magnifyName->setText(feature->getFeature()->getFeatureName());
    if(!feature->getGeometry().isNull()){
        this->ui->label_magnifyActualNominal->setText(feature->getGeometry()->getIsNominal()?"nom":"act");
    }else{
        this->ui->label_magnifyActualNominal->setText("-/-");
    }

    //resize labels to maximum
    QFont fontActualNominal = this->ui->label_magnifyActualNominal->font();
    double h = this->ui->label_magnifyActualNominal->height();
    double w = this->ui->label_magnifyActualNominal->width();
    QFontMetrics fmActualNominal(fontActualNominal);
    const double tw = fmActualNominal.width(this->ui->label_magnifyActualNominal->text());
    double scaleW = w/tw;
    double scaleH = h/fmActualNominal.height();
    fontActualNominal.setPointSizeF(fontActualNominal.pointSizeF() * std::min(scaleH, scaleW));
    this->ui->label_magnifyActualNominal->setFont(fontActualNominal);

    // set label witdh
    this->ui->label_magnifyActualNominal->setFixedWidth(tw * std::min(scaleH, scaleW));

    QFont fontName = this->ui->label_magnifyName->font();
    h = this->ui->label_magnifyName->height();
    w = this->ui->label_magnifyName->width() + (w - tw * std::min(scaleH, scaleW)); // with delta size of label_magnifyActualNominal
    QFontMetrics fmName(fontName);
    scaleW = w/fmName.width(this->ui->label_magnifyName->text());
    scaleH = h/fmName.height();
    fontName.setPointSizeF(fontName.pointSizeF() * std::min(scaleH, scaleW));
    this->ui->label_magnifyName->setFont(fontName);

}

/*!
 * \brief MainWindow::updateGroupFilterSize
 */
void MainWindow::updateGroupFilterSize(){

    //get and check current job
    QPointer<OiJob> job = ModelManager::getCurrentJob();
    if(job.isNull()){
        return;
    }
/*
    //set combobox size
    int sizeGroup = oi::getDropDownMenuSize(ModelManager::getGroupNamesModel().stringList(),this->ui->comboBox_groups->width());
    this->ui->comboBox_groups->view()->setMinimumWidth(sizeGroup);*/
}

/*!
 * \brief MainWindow::updateSystemFilterSize
 */
void MainWindow::updateSystemFilterSize(){

    //get and check current job
    QPointer<OiJob> job = ModelManager::getCurrentJob();
    if(job.isNull()){
        return;
    }

    /*//get the largest system name
    const QList<QPointer<CoordinateSystem> > &nominalSystems = job->getCoordinateSystemsList();
    QList<QPointer<CoordinateSystem> > &stationSystems = job->getStationSystemsList();
    QList<QPointer<CoordinateSystem> > &bundleSystems = job->getBundleSystemList();
    QString largestSystemName = "";
    foreach(const QPointer<CoordinateSystem> &system, nominalSystems){
        if(!system.isNull() && system->getFeatureName().length() > largestSystemName.length()){
            largestSystemName = system->getFeatureName();
        }
    }
    foreach(const QPointer<CoordinateSystem> &system, stationSystems){
        if(!system.isNull() && system->getFeatureName().length() > largestSystemName.length()){
            largestSystemName = system->getFeatureName();
        }
    }
    foreach(const QPointer<CoordinateSystem> &system, bundleSystems){
        if(!system.isNull() && system->getFeatureName().length() > largestSystemName.length()){
            largestSystemName = system->getFeatureName();
        }
    }

    //calculate width of popup dependend on size of group name
    QFont font;
    QFontMetrics fm(font);
    int width = fm.width(largestSystemName);
    int boxWidth = this->ui->comboBox_activeCoordSystem->width();

    if((width + (0.1*width)) > boxWidth){ // if text is bigger than combobox
        this->ui->comboBox_activeCoordSystem->view()->setMinimumWidth(width + (0.1 * width));
    }else{ // if combobox is bigger than text
        this->ui->comboBox_activeCoordSystem->view()->setMinimumWidth(boxWidth);
    }*/

}

/*!
 * \brief MainWindow::updateActualNominalFilterSize
 */
void MainWindow::updateActualNominalFilterSize(){

    /*//set combobox size
    int sizeFilter = oi::getDropDownMenuSize(ModelManager::getActualNominalFilterModel().stringList(),this->ui->comboBox_actualNominal->width());
    this->ui->comboBox_actualNominal->view()->setMinimumWidth(sizeFilter);*/
}

/*!
 * \brief MainWindow::resetBundleView
 * Resets the bundle view
 */
void MainWindow::resetBundleView(){

    //reset scalar parameters
    this->bundleParameterWidget->blockSignals(true);
    this->bundleParameterWidget->clearAll();
    this->bundleParameterWidget->setEnabled(false);
    this->bundleParameterWidget->blockSignals(false);

    //reset input stations
    QJsonArray stations;
    QObject::disconnect(this->bundleStationsModel, &BundleStationsModel::stationsChanged,
                        this, &MainWindow::bundleSettingsChanged);
    this->bundleStationsModel->setStations(stations);
    this->ui->treeView_inputStations->setEnabled(false);
    QObject::connect(this->bundleStationsModel, &BundleStationsModel::stationsChanged,
                     this, &MainWindow::bundleSettingsChanged, Qt::AutoConnection);

    //reset input geometries
    this->bundleGeometriesModel->setStations(stations);
    this->ui->treeView_inputGeometries->setEnabled(false);

}

/*!
 * \brief MainWindow::saveAsProject
 * creates a file name for save path and emits the signal to save
 */
void MainWindow::saveProjectAs(bool asTemplate)
{
    QString caption;
    QString preferedName = ProjectConfig::getProjectPath();
    if(asTemplate) {
        caption = "Save template as";
        preferedName += QDir::separator();
        preferedName += "_template";

        if ((emit showMessageBox("save current project", MessageTypes::eQuestionMessage) == QMessageBox::Yes)) {
            emit this->saveProject();
        } else {
            this->log("save as template: aborted", eInformationMessage, eConsoleMessage);
            return;
        }
    } else {
        caption = "Save project as";
    }

    QString filename = QFileDialog::getSaveFileName(this, caption, preferedName, tr("xml (*.xml)"));

    if(!filename.isEmpty()){
        QFileInfo info(filename);
        ProjectConfig::setProjectPath(info.absolutePath());
        if(asTemplate) {
            emit this->createTemplateFromJob();
        }
        emit this->saveProject(filename);
    }
}

/*!
 * \brief MainWindow::loadDefaultBundlePlugIn
 */
void MainWindow::loadDefaultBundlePlugIn(int bundleID)
{
    //get selected bundle template
    int templateIndex = this->ui->comboBox_bundleTemplate->currentIndex();
    QJsonObject bundleTemplate = ModelManager::getBundleTemplatesModel().getBundleTemplate(templateIndex);
    if(bundleTemplate.isEmpty()){
        return;
    }

    //load template
    emit this->loadBundleTemplate(bundleID, bundleTemplate);
    this->bundleSelectionChanged();
}

/*!
 * \brief MainWindow::goToNextFeature
 * Jumping to the next feature in the feature list after an successful measurement
 */
void MainWindow::autoSwitchToNextFeature(bool sucessMeasure)
{
    if(sucessMeasure){
        if(!this->ui->actiongo_to_next_feature->isChecked()
                && !this->ui->actiongo_to_next_feature->property("PREVIOUS_ISCHECKED_VALUE").isValid()){
            this->ui->tableView_features->selectRow(this->ui->tableView_features->currentIndex().row() + 1);
        }
    }
    return;
}

/*!
 * \brief MainWindow::on_actionShortcut_import_triggered
 * shortcut for import nominal features
 */
void MainWindow::on_actionShortcut_import_triggered()
{
    on_actionimport_triggered();
}

void MainWindow::createMessageBoxTrafoParamWarning()
{
    QMessageBox msgBox;
    msgBox.setText("Transformation with these coordinate systems already exists and is used.");
    msgBox.setInformativeText("New transformation is created but not set to use.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
}

void MainWindow::on_tableView_bundleParameter_clicked(const QModelIndex &index)
{
    //get and check model
    BundleParameterTableProxyModel *model = static_cast<BundleParameterTableProxyModel *>(this->ui->tableView_bundleParameter->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //set active feature
    sourceModel->setActiveFeature(model->mapToSource(index));
}

/*!
 * \brief MainWindow::on_actiondifferences_triggered
 */
void MainWindow::on_actiondifferences_triggered()
{
    //show and hide differences of features with actual and nominals
    if(this->ui->dockWidget_differences->isVisible()){
        this->ui->dockWidget_differences->setVisible(false);
    }else{
        this->ui->dockWidget_differences->setVisible(true);
    }
}

/*!
 * \brief MainWindow::on_lineEdit_tolerance_returnPressed
 */
void MainWindow::on_lineEdit_tolerance_returnPressed()
{
    bool check = false;
    double value = this->ui->lineEdit_tolerance->text().toDouble(&check);
    if(check){
        ModelManager::getFeatureDifferenceTableModel().setTolerance(value);
    }
}

/*!
 * \brief MainWindow::on_tableView_FeatureDifferences_customContextMenuRequested
 * \param pos
 */
void MainWindow::on_tableView_FeatureDifferences_customContextMenuRequested(const QPoint &pos)
{
    //create menu and add delete action
    QMenu *menu = new QMenu();

    //get feature table models
    FeatureDifferenceProxyModel *model = static_cast<FeatureDifferenceProxyModel*>(this->ui->tableView_FeatureDifferences->model());
    if(model == NULL){
        delete menu;
        return;
    }
    FeatureDifferenceTableModel *sourceModel = static_cast<FeatureDifferenceTableModel*>(model->sourceModel());
    if(sourceModel == NULL){
        delete menu;
        return;
    }

    //get the selected index (where the right click was done)
    QModelIndex selectedIndex = this->ui->tableView_FeatureDifferences->indexAt(pos);

    menu->addAction("copy to clipboard", this, SLOT(copyDifferencesToClipboard()));
    menu->exec(this->ui->tableView_FeatureDifferences->mapToGlobal(pos));
}

/*!
 * \brief MainWindow::showEvent
 * \param e
 */
void MainWindow::showEvent(QShowEvent *e)
{
    //load and restore project unit settings
    ProjectConfig::loadProjectPathConfigFile();
    //parse them to the model, to display all values in correct unit
    this->settingsDialog.updateDisplayConfigFromSelection();

    emit loadAndSaveConfigs();

    e->accept();
}

void MainWindow::startAutoSave() {
    int i = ProjectConfig::getAutoSaveInterval();
    qDebug() << "auto save interval: " << i << "minutes" << (i>0 ? "" : ": disabled");

    if(i>0) {
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(autoSaveProject()));
        timer->start(60000 * i);
    }
}

void MainWindow::autoSaveProject() {
    if(this->control.hasProjectDigestChanged()) {
        emit this->saveProject();
    }
}

void MainWindow::enableObservationsOfActiveFeature() {
    enableOrDisableObservationsOfActiveFeature(true);
}

void MainWindow::disableObservationsOfActiveFeature() {
    enableOrDisableObservationsOfActiveFeature(false);
}

void MainWindow::enableOrDisableObservationsOfActiveFeature(bool enable) {
    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get and check the active feature
    QPointer<FeatureWrapper> feature = sourceModel->getActiveFeature();
    if(!feature.isNull() && !feature->getFeature().isNull()){
        if(enable) {
            emit this->enableObservations(feature->getFeature()->getId());
        } else {
            emit this->disableObservations(feature->getFeature()->getId());
        }
    }
}

/**
 * show dialog centered on the current screen / monitor
 *
 * @brief MainWindow::showCentered
 * @param dialog
 */
void MainWindow::showCentered(QDialog &dialog) {
    dialog.show();
    dialog.activateWindow();
    QScreen *screen = this->windowHandle()->screen();
    dialog.move( screen->geometry().center() - dialog.rect().center() );
    dialog.windowHandle()->setScreen(screen);
}

void MainWindow::on_actionNew_watch_window_triggered()
{
    openWatchWindow(WatchWindowBehavior::eShowCurrentSelectedFeature);
}

void MainWindow::openWatchWindow(WatchWindowBehavior behavior) {
    QPointer<OiJob> job = ModelManager::getCurrentJob();
    if(!job.isNull()) {

        QList<QPointer<FeatureWrapper> > features;
        QVariant watchWindowKey;
        QString windowTitleSuffix;

        switch(behavior) {
        case eShowCurrentSelectedFeature: // open new watch window for current selected feature
        {
            QPointer<FeatureWrapper> feature = job->getActiveFeature();
            if(feature.isNull()) {
                return;
            }
            features.append(feature);

            watchWindowKey = QString("%1%2")
                                .arg(feature->getFeature()->getFeatureName())
                                .arg(feature->getGeometry().isNull() ? ""  : feature->getGeometry()->getIsNominal() ? "  nom" : "  act");
            windowTitleSuffix =  QString("[%1]").arg(watchWindowKey.toString());
            break;
        }
        case eShowNearestNominal:   // find nearest nominal feature
        {
            QSortFilterProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
            if(model == NULL){
                return;
            }
            FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
            if(sourceModel == NULL){
                return;
            }
            QModelIndexList selection = this->ui->tableView_features->selectionModel()->selectedIndexes();
            foreach(const QModelIndex &index, selection){
                int id = sourceModel->getFeatureIdAtIndex(model->mapToSource(index));
                QPointer<FeatureWrapper> feature = job->getFeatureById(id);
                if(!feature.isNull()) {
                    features.append(feature);
                }
            }

            watchWindowKey = QVariant(-2); // key for nearest nominal or actual feature
            windowTitleSuffix = " [nearest]";
            if(watchWindowDialogs.contains(watchWindowKey)){ // remove watchwindow because of new selection
                delete watchWindowDialogs[watchWindowKey].data();
                watchWindowDialogs.remove(watchWindowKey);
            }
            break;
        }
        case eShowAlwaysActiveFeature: // show always the active feature
        {
            watchWindowKey = QVariant(-1); // key for active feature
            windowTitleSuffix = " [active]";
            break;
        }
        } // switch

        if(!watchWindowDialogs.contains(watchWindowKey)) {
            QPointer<WatchWindowDialog> watchWindowDialog = new WatchWindowDialog(behavior, job, features);
            watchWindowDialog->setWindowTitle(watchWindowDialog->windowTitle() + windowTitleSuffix);
            watchWindowDialogs[watchWindowKey] = watchWindowDialog;

            //connect watch window dialog
            QObject::connect(watchWindowDialog, &WatchWindowDialog::startStreaming, &this->control, &Controller::startReadingStream, Qt::AutoConnection);
            QObject::connect(watchWindowDialog, &WatchWindowDialog::stopStreaming, &this->control, &Controller::stopReadingStream, Qt::AutoConnection);
        }

        qDebug() << "openWatchWindow"
                    << ", key=" << watchWindowKey
                    << ", behavior=" << behavior;

        watchWindowDialogs[watchWindowKey]->show();
        watchWindowDialogs[watchWindowKey]->activateWindow();
    }

}

void MainWindow::on_actionWatch_window_nearest_nominal_triggered()
{
    openWatchWindow(WatchWindowBehavior::eShowNearestNominal);
}

void MainWindow::updateCompleter() {
    QPointer<OiJob> job = ModelManager::getCurrentJob();
    if(!job.isNull()) {
        FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
        if(model == NULL){
            return;
        }

        int featureNameColumn = model->getFeatureTableColumnConfig().getColumnPosition(eFeatureDisplayName);
        int groupNameColumn = model->getFeatureTableColumnConfig().getColumnPosition(eFeatureDisplayGroup);
        QStringList featureNames;
        QStringList groupNames;
        for(int row=0; row < model->rowCount(); row++) {
            QString featureName = model->data(model->index(row,featureNameColumn), Qt::DisplayRole).toString();
            if(!featureNames.contains(featureName)) {
                featureNames.append(featureName);
            }
            QString groupName = model->data(model->index(row,groupNameColumn), Qt::DisplayRole).toString();
            if(!groupNames.contains(groupName) && !groupName.isNull() && !groupName.isEmpty()) {
                groupNames.append(groupName);
            }
        }
        QCompleter *completer = new QCompleter(featureNames, this);
        completer->setFilterMode(Qt::MatchContains);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        this->ui->lineEdit_searchFeatureName->setCompleter(completer);

    }
}

void MainWindow::on_lineEdit_searchFeatureName_returnPressed()
{
    QPointer<OiJob> job = ModelManager::getCurrentJob();
    if(!job.isNull()) {
        foundFeatures = job->getFeaturesByName(this->ui->lineEdit_searchFeatureName->text(), true);

        this->ui->pushButton_showNextFoundFeature->setEnabled(foundFeatures.size()>1);

        showFoundFeatureIndex = 0;
        if(!foundFeatures.isEmpty()) {
            showFoundFeature(showFoundFeatureIndex);
        }
    }
}

void MainWindow::showFoundFeature(int index) {
    if(!foundFeatures.isEmpty() && foundFeatures.size() > index) {
        QPointer<FeatureWrapper> feature = foundFeatures.at(index);

        if(!feature.isNull()) {

            FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
            if(model == NULL){
                return;
            }

            // get and check source model
            FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
            if(sourceModel == NULL){
                return;
            }

            int column = model->getFeatureTableColumnConfig().getColumnPosition(eFeatureDisplayName);
            for(int row=0; row < model->rowCount(); row++) {
                QModelIndex index = model->index(row,column);
                QString name = model->data(index, Qt::DisplayRole).toString();
                if(name == feature->getFeature()->getFeatureName()) {
                    feature->getFeature()->setActiveFeatureState(true);
                    this->ui->tableView_features->scrollTo(index);
                    this->ui->tableView_features->viewport()->update(); // redraw the viewport so that only the active feature is highlighted
                }
            }

        }
    }

}

void MainWindow::on_pushButton_showNextFoundFeature_clicked()
{
    showFoundFeatureIndex++;
    if(foundFeatures.isEmpty() || showFoundFeatureIndex > (foundFeatures.size() -1)) {
        showFoundFeatureIndex = 0; // wrap around
    }
    showFoundFeature(showFoundFeatureIndex);
}

void MainWindow::showStatusSensor(const SensorStatus &status, const QString &msg) {

    if(this->animation_label_statusSensor->state() == QVariantAnimation::Running) {
        this->animation_label_statusSensor->stop();
        this->label_statusSensor->setVisible(true);
    }

    switch(status){
    case SensorStatus::eReadyForMeasurement:
        this->label_statusSensor->setStyleSheet("QLabel { background-color : lightgreen;}");
        break;
     case SensorStatus::eNotReadyForMeasurement:
        this->label_statusSensor->setStyleSheet("QLabel { background-color : red;}");
        break;
     case SensorStatus::eSensorActionInProgress:
       this->label_statusSensor->setStyleSheet("QLabel { background-color : yellow;}");
       this->animation_label_statusSensor->start(); // start "blink" animation
       break;
     case SensorStatus::eClearStatus:
        this->label_statusSensor->setStyleSheet("");
        break;
     }

     this->label_statusSensor->setText(msg);
 }

void MainWindow::on_comboBox_sortBy_currentIndexChanged(int index)
{

    FeatureTableProxyModel *ftmodel = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(ftmodel != NULL){
        ftmodel->setSortingMode((FeatureSorter::SortingMode)index);
    }

    FeatureDifferenceProxyModel *fdmodel = static_cast<FeatureDifferenceProxyModel *>(this->ui->tableView_FeatureDifferences->model());
    if(fdmodel != NULL){
        fdmodel->setSortingMode((FeatureSorter::SortingMode)index);
    }

}

void MainWindow::measureBehaviorLogicStarted() {
    // disable "go to next feautre"
    this->ui->actiongo_to_next_feature->setProperty("PREVIOUS_ISCHECKED_VALUE", this->ui->actiongo_to_next_feature->isChecked());
    this->ui->actiongo_to_next_feature->setChecked(false);
}

void MainWindow::measureBehaviorLogicFinished() {
    QVariant value = this->ui->actiongo_to_next_feature->property("PREVIOUS_ISCHECKED_VALUE");
    this->ui->actiongo_to_next_feature->setProperty("PREVIOUS_ISCHECKED_VALUE", QVariant::Invalid);
    if(value.isValid()) {
        this->ui->actiongo_to_next_feature->setChecked(value.toBool());
    }
}
