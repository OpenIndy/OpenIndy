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

    //assign models of ModelManager to GUI-elements
    this->assignModels();

    //init GUI elements
    this->initFeatureTableViews();
    this->initSensorPad();
    this->initToolMenus();
    this->initFilterComboBoxes();
    this->initStatusBar();

    //initially resize table view to fit the default job
    this->resizeTableView();

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
    this->loadingDialog.show();
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

    this->loadingDialog.close();

}

/*!
 * \brief MainWindow::importObservationsStarted
 */
void MainWindow::importObservationsStarted(){
    this->loadingDialog.show();
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
    this->activeSensorTypeChanged(activeStation->getActiveSensorType(), activeStation->getSupportedSensorActions(),
                                  activeStation->getSelfDefinedActions());

}

/*!
 * \brief MainWindow::currentJobChanged
 */
void MainWindow::currentJobChanged(){

    //get current job and pass it to watch window
    QPointer<OiJob> job = ModelManager::getCurrentJob();
    this->watchWindowDialog.setCurrentJob(job);

    //set window title
    if(!job.isNull()){
        this->setWindowTitle(job->getJobName());
    }else{
        this->setWindowTitle("");
    }

}

/*!
 * \brief MainWindow::sensorActionStarted
 * \param name
 */
void MainWindow::sensorActionStarted(const QString &name){
    this->sensorTaskInfoDialog.setDisplayMessage(name);
    this->sensorTaskInfoDialog.show();
}

/*!
 * \brief MainWindow::sensorActionFinished
 * \param success
 * \param msg
 */
void MainWindow::sensorActionFinished(const bool &success, const QString &msg){
    this->sensorTaskInfoDialog.close();
    emit this->log(msg, eInformationMessage, eConsoleMessage);
}

/*!
 * \brief MainWindow::measurementCompleted
 * Called whenever a measurement task was successfully completed
 */
void MainWindow::measurementCompleted(){

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

    //check wether there are more features left, that shall be aimed and measured
    if(this->measureFeatures.size() > 0){
        sourceModel->setActiveFeature(this->measureFeatures[0]);
        this->measureFeatures.removeAt(0);
        this->control.startAimAndMeasure();
    }

}

/*!
 * \brief MainWindow::showMessageBox
 * \param msg
 * \param msgType
 */
void MainWindow::showMessageBox(const QString &msg, const MessageTypes &msgType){

    QMessageBox msgBox;

    switch(msgType){
    case eInformationMessage:
        msgBox.setIcon(QMessageBox::Information);
        break;
    case eWarningMessage:
        msgBox.setIcon(QMessageBox::Warning);
        break;
    case eErrorMessage:
        msgBox.setIcon(QMessageBox::Critical);
        break;
    case eCriticalMessage:
        msgBox.setIcon(QMessageBox::Critical);
        break;
    }

    msgBox.setText(msg);
    msgBox.setStandardButtons(QMessageBox::Ok);

    msgBox.exec();

}

/*!
 * \brief MainWindow::showStatusMessage
 * \param msg
 * \param msgType
 */
void MainWindow::showStatusMessage(const QString &msg, const MessageTypes &msgType){

    QString status;

    switch(msgType){
    case eInformationMessage:
        break;
    case eWarningMessage:
        status.append("WARNING: ");
        break;
    case eErrorMessage:
        status.append("ERROR: ");
        break;
    case eCriticalMessage:
        status.append("CRITICAL: ");
        break;
    }

    status.append(msg);

    this->ui->statusBar->showMessage(status);

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
        }else{ //normal measurement
            this->control.startMeasurement();
        }
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

        QMessageBox msgBox;
        msgBox.setText(QString("Delete observations of feature %1?").arg(feature->getFeature()->getFeatureName()));
        msgBox.setInformativeText("");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Yes:
            this->removeObservationOfActiveFeature();
            break;
        }
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

    }

}

/*!
 * \brief MainWindow::on_actionCreate_point_triggered
 */
void MainWindow::on_actionCreate_point_triggered(){
    this->createFeatureDialog.setFeatureType(ePointFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_line_triggered
 */
void MainWindow::on_actionCreate_line_triggered(){
    this->createFeatureDialog.setFeatureType(eLineFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_plane_triggered
 */
void MainWindow::on_actionCreate_plane_triggered(){
    this->createFeatureDialog.setFeatureType(ePlaneFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_sphere_triggered
 */
void MainWindow::on_actionCreate_sphere_triggered(){
    this->createFeatureDialog.setFeatureType(eSphereFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_station_triggered
 */
void MainWindow::on_actionCreate_station_triggered(){
    this->createFeatureDialog.setFeatureType(eStationFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_coordinatesystem_triggered
 */
void MainWindow::on_actionCreate_coordinatesystem_triggered(){
    this->createFeatureDialog.setFeatureType(eCoordinateSystemFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_scalar_entity_triggered
 */
void MainWindow::on_actionCreate_scalar_entity_triggered(){
    this->createFeatureDialog.setFeatureType(eScalarEntityAngleFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_trafoParam_triggered
 */
void MainWindow::on_actionCreate_trafoParam_triggered(){
    this->createFeatureDialog.setFeatureType(eTrafoParamFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_cone_triggered
 */
void MainWindow::on_actionCreate_cone_triggered(){
    this->createFeatureDialog.setFeatureType(eConeFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_cylinder_triggered
 */
void MainWindow::on_actionCreate_cylinder_triggered(){
    this->createFeatureDialog.setFeatureType(eCylinderFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_ellipsoid_triggered
 */
void MainWindow::on_actionCreate_ellipsoid_triggered(){
    this->createFeatureDialog.setFeatureType(eEllipsoidFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_hyperboloid_triggered
 */
void MainWindow::on_actionCreate_hyperboloid_triggered(){
    this->createFeatureDialog.setFeatureType(eHyperboloidFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_paraboloid_triggered
 */
void MainWindow::on_actionCreate_paraboloid_triggered(){
    this->createFeatureDialog.setFeatureType(eParaboloidFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_nurbs_triggered
 */
void MainWindow::on_actionCreate_nurbs_triggered(){
    this->createFeatureDialog.setFeatureType(eNurbsFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_pointcloud_triggered
 */
void MainWindow::on_actionCreate_pointcloud_triggered(){
    this->createFeatureDialog.setFeatureType(ePointCloudFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_circle_triggered
 */
void MainWindow::on_actionCreate_circle_triggered(){
    this->createFeatureDialog.setFeatureType(eCircleFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_torus_triggered
 */
void MainWindow::on_actionCreate_torus_triggered(){
    this->createFeatureDialog.setFeatureType(eTorusFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_slotted_hole_triggered
 */
void MainWindow::on_actionCreate_slotted_hole_triggered(){
    this->createFeatureDialog.setFeatureType(eSlottedHoleFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_ellipse_triggered
 */
void MainWindow::on_actionCreate_ellipse_triggered(){
    this->createFeatureDialog.setFeatureType(eEllipseFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionLoad_plugins_triggered
 */
void MainWindow::on_actionLoad_plugins_triggered(){
    this->pluginLoaderDialog.show();
}

/*!
 * \brief MainWindow::on_actionPlugin_manager_triggered
 */
void MainWindow::on_actionPlugin_manager_triggered(){
    this->pluginManagerDialog.show();
}

/*!
 * \brief MainWindow::on_action_importNominals_triggered
 */
void MainWindow::on_action_importNominals_triggered(){
    this->importNominalDialog.show();
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
    menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("delete selected feature(s)"), this, SLOT(deleteFeatures(bool)));

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

    //if the selected feature is the active feature
    if(selectedFeature->getFeature()->getIsActiveFeature()){

        menu->addAction(QIcon(":/Images/icons/info.png"), QString("show properties of feature %1").arg(selectedFeature->getFeature()->getFeatureName()),
                        this, SLOT(showFeatureProperties(bool)));
        menu->addAction(QIcon(":/Images/icons/button_ok.png"), QString("recalc %1").arg(selectedFeature->getFeature()->getFeatureName()),
                        &this->control, SLOT(recalcActiveFeature()));

        //if the active feature is a geometry
        if(!selectedFeature->getGeometry().isNull()){

            menu->addAction(QIcon(""), QString("remove observations of feature %1").arg(selectedFeature->getFeature()->getFeatureName()),
                                 this, SLOT(removeObservationOfActiveFeature()));

        }

    }

    menu->exec(this->ui->tableView_features->mapToGlobal(pos));

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

        menu->addAction(QIcon(":/Images/icons/info.png"), QString("show properties of feature %1").arg(selectedFeature->getFeature()->getFeatureName()),
                        this, SLOT(showFeatureProperties(bool)));
        menu->addAction(QIcon(":/Images/icons/button_ok.png"), QString("recalc %1").arg(selectedFeature->getFeature()->getFeatureName()),
                        &this->control, SLOT(recalcActiveFeature()));

    }

    menu->exec(this->ui->tableView_features->mapToGlobal(pos));

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
        this->featureFunctionsDialog.show();
    }

}

/*!
 * \brief MainWindow::on_actionSet_sensor_triggered
 */
void MainWindow::on_actionSet_sensor_triggered(){
    this->sensorConfigurationDialog.show();
}

/*!
 * \brief MainWindow::setSensorConfiguration
 * \param name
 */
void MainWindow::setSensorConfiguration(const QString &name){

    //check name
    if(name.compare("") == 0){
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
        emit this->sensorConfigurationChanged(name, true);
        break;
    case QMessageBox::No:
        emit this->sensorConfigurationChanged(name, false);
        break;
    }

}

/*!
 * \brief MainWindow::showMoveSensorDialog
 */
void MainWindow::showMoveSensorDialog(){
    this->moveSensorDialog.show();
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

}

/*!
 * \brief MainWindow::on_actionWatch_window_triggered
 */
void MainWindow::on_actionWatch_window_triggered(){
    this->watchWindowDialog.show();
}

/*!
 * \brief MainWindow::on_actionOpen_triggered
 */
void MainWindow::on_actionOpen_triggered(){

    QFileDialog dlg;
    dlg.exec();

    QString filename = QFileDialog::getOpenFileName(this, "Choose a file", "", "oi.xml (*.oi.xml)");
    if(filename.compare("") == 0){
        return;
    }

    QPointer<QIODevice> device = new QFile(filename);
    QFileInfo info(filename);
    QString projectName = info.fileName();

    //clear current selection in table view
    this->ui->tableView_features->clearSelection();
    this->ui->tableView_trafoParams->clearSelection();

    emit this->loadProject(projectName, device);

}

/*!
 * \brief MainWindow::on_actionSave_triggered
 */
void MainWindow::on_actionSave_triggered(){

}

/*!
 * \brief MainWindow::on_actionSave_as_triggered
 */
void MainWindow::on_actionSave_as_triggered(){
    QString filename = QFileDialog::getSaveFileName(this, "Choose a filename", "oiProject", "oi.xml (*.oi.xml)");
    if(filename.compare("") != 0){
        emit this->saveProject(filename);
    }
}

/*!
 * \brief MainWindow::on_actionClose_triggered
 */
void MainWindow::on_actionClose_triggered(){
    this->close();
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

    this->measurementConfigDialog.show();

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

    //set selected station
    sourceModel->setActiveStation(model->mapToSource(index));

}

/*!
 * \brief MainWindow::on_actionView_settings_triggered
 */
void MainWindow::on_actionView_settings_triggered(){
    this->settingsDialog.show();
}

/*!
 * \brief MainWindow::showToolWidget
 * \param pluginName
 * \param toolName
 */
void MainWindow::showToolWidget(const QString &pluginName, const QString &toolName){

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

    //get a list of available tool plugins
    QList<QPointer<Tool> > tools = this->control.getAvailableTools();

    //search the list for the specified tool
    foreach(const QPointer<Tool> &tool, tools){

        if(tool.isNull()){
            continue;
        }

        if(tool->getMetaData().pluginName.compare(pluginName) == 0
                && tool->getMetaData().name.compare(toolName) == 0){
            tool->setJob(sourceModel->getCurrentJob());
            tool->show();
            continue;
        }else{
            delete tool;
        }

    }

}

/*!
 * \brief MainWindow::resizeTableView
 * Triggered whenever the user double clicks a header item of a table view
 */
void MainWindow::resizeTableView(){
    this->ui->tableView_features->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_trafoParams->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

/*!
 * \brief MainWindow::on_actionRemoveObservations_triggered
 */
void MainWindow::on_actionRemoveObservations_triggered(){
    emit this->removeAllObservations();
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
        emit this->removeObservations(feature->getFeature()->getId());
    }

}

/*!
 * \brief MainWindow::on_actionAbout_OpenIndy_triggered
 */
void MainWindow::on_actionAbout_OpenIndy_triggered(){
    this->aboutDialog.show();
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
        this->actualPropertiesDialog.show();
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

        this->nominalPropertiesDialog.show();

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

        this->trafoParamPropertiesDialog.show();

        return;

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

    //get selected features
    this->measureFeatures.clear();
    QModelIndexList selection = this->ui->tableView_features->selectionModel()->selectedIndexes();
    foreach(const QModelIndex &index, selection){
        int id = sourceModel->getFeatureIdAtIndex(model->mapToSource(index));
        if(id >= 0 && !this->measureFeatures.contains(id)){
            this->measureFeatures.append(id);
        }
    }

    //aim and measure the first feature in the list of selected features
    if(this->measureFeatures.size() > 0){
        sourceModel->setActiveFeature(this->measureFeatures[0]);
        this->measureFeatures.removeAt(0);
        this->control.startAimAndMeasure();
    }

}

/*!
 * \brief MainWindow::deleteFeatures
 * \param checked
 */
void MainWindow::deleteFeatures(bool checked){

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
    QAbstractItemModel *model = NULL;
    QItemSelectionModel *selectionModel = NULL;
    QModelIndexList selection;

    //get selection of the active table view
    if(this->ui->tabWidget_views->currentWidget() == this->ui->tab_features){ //feature table view
        model = this->ui->tableView_features->model();
        selectionModel = this->ui->tableView_features->selectionModel();
        selection = selectionModel->selectedIndexes();
    }else if(this->ui->tabWidget_views->currentWidget() == this->ui->tab_trafoParam){ //trafo param table view
        model = this->ui->tableView_trafoParams->model();
        selectionModel = this->ui->tableView_trafoParams->selectionModel();
        selection = selectionModel->selectedIndexes();
    }

    //check and sort selection
    if(selection.size() <= 0){
        return;
    }
    qSort(selection);

    //###############################
    //copy the selection to clipboard
    //###############################

    QString copy_table;
    QModelIndex last = selection.last();
    QModelIndex previous = selection.first();
    selection.removeFirst();

    //loop over all selected rows and columns
    for(int i = 0; i < selection.size(); i++){

        QVariant data = model->data(previous);
        QString text = data.toString();

        QModelIndex index = selection.at(i);
        copy_table.append(text);

        //if new line
        if(index.row() != previous.row()){
            copy_table.append("\n");
        }else{ //if same line, but new column
            copy_table.append("\t");
        }
        previous = index;

    }

    //get last selected cell
    copy_table.append(model->data(last).toString());
    copy_table.append("\n");

    //set values to clipboard, so you can copy them
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->clear();
    clipboard->setText(copy_table);

}

/*!
 * \brief MainWindow::pasteFromClipboard
 */
void MainWindow::pasteFromClipboard(){

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

    //get values from clipboard, so you can copy them
    QClipboard *clipboard = QApplication::clipboard();
    QString copy_table = clipboard->text();

    //seperate copy table into columns: only one column is allowed
    QStringList columns = copy_table.split('\t');
    if(columns.size() != 1){
        return;
    }

    //seperate copy table into rows: either one or selection.size rows are allowed
    QStringList rows = copy_table.split('\n');
    if(rows.size() != 2 && rows.size() != selection.size()+1){
        return;
    }
    rows.removeLast();

    //edit entries at selected indexes
    if(rows.size() == 1){
        foreach(const QModelIndex &index, selection){
            QModelIndex currentIndex = model->index(index.row(), index.column());
            sourceModel->setData(model->mapToSource(currentIndex), rows.at(0));
        }
    }else{
        int i = 0;
        foreach(const QModelIndex &index, selection){
            QModelIndex currentIndex = model->index(index.row(), index.column());
            sourceModel->setData(model->mapToSource(currentIndex), rows.at(i));
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
 * \brief MainWindow::connectController
 */
void MainWindow::connectController(){

    //connect actions triggered by user to slots in controller
    QObject::connect(this, &MainWindow::log, &this->control, &Controller::log, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::addFeatures, &this->control, &Controller::addFeatures, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::importNominals, &this->control, &Controller::importNominals, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::sensorConfigurationChanged, &this->control, &Controller::sensorConfigurationChanged, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::measurementConfigurationChanged, &this->control, &Controller::measurementConfigurationChanged, Qt::AutoConnection);
    QObject::connect(this, static_cast<void (MainWindow::*)()>(&MainWindow::saveProject),
                     &this->control, static_cast<void (Controller::*)()>(&Controller::saveProject), Qt::AutoConnection);
    QObject::connect(this, static_cast<void (MainWindow::*)(const QString&)>(&MainWindow::saveProject),
                     &this->control, static_cast<void (Controller::*)(const QString&)>(&Controller::saveProject), Qt::AutoConnection);
    QObject::connect(this, &MainWindow::loadProject, &this->control, &Controller::loadProject, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::removeObservations, &this->control, &Controller::removeObservations, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::removeAllObservations, &this->control, &Controller::removeAllObservations, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::removeFeatures, &this->control, &Controller::removeFeatures, Qt::AutoConnection);

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
    QObject::connect(&this->control, &Controller::showMessageBox, this, &MainWindow::showMessageBox, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::showStatusMessage, this, &MainWindow::showStatusMessage, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::availableGroupsChanged, this, &MainWindow::availableGroupsChanged, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::coordSystemSetChanged, this, &MainWindow::coordSystemSetChanged, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::featureNameChanged, this, &MainWindow::featureNameChanged, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::currentJobChanged, this, &MainWindow::currentJobChanged, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::trafoParamSetChanged, this, &MainWindow::trafoParamSetChanged, Qt::AutoConnection);

}

/*!
 * \brief MainWindow::connectDialogs
 */
void MainWindow::connectDialogs(){

    //connect create feature dialog
    QObject::connect(&this->createFeatureDialog, &CreateFeatureDialog::addFeatures, this, &MainWindow::addFeatures, Qt::AutoConnection);

    //connect console
    QObject::connect(Console::getInstance().data(), &Console::lineAdded, this->ui->listView_console, &QListView::scrollToBottom, Qt::AutoConnection);

    //connect import dialogs
    QObject::connect(&this->importNominalDialog, &ImportNominalDialog::startImport, this, &MainWindow::importNominals, Qt::AutoConnection);

    //connect loading dialog
    QObject::connect(&this->control, &Controller::nominalImportProgressUpdated, &this->loadingDialog, &LoadingDialog::updateProgress, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::observationImportProgressUpdated, &this->loadingDialog, &LoadingDialog::updateProgress, Qt::AutoConnection);

    //connect sensor config dialog
    QObject::connect(&this->sensorConfigurationDialog, &SensorConfigurationDialog::setSensorConfiguration, this, &MainWindow::setSensorConfiguration, Qt::AutoConnection);

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

    //connect nominal properties dialog
    QObject::connect(&this->nominalPropertiesDialog, &NominalPropertiesDialog::nominalParametersChanged, &this->control, &Controller::setNominalParameters, Qt::AutoConnection);

    //connect trafo param properties dialog
    QObject::connect(&this->trafoParamPropertiesDialog, &TrafoParamPropertiesDialog::trafoParamParametersChanged, &this->control, &Controller::setTrafoParamParameters, Qt::AutoConnection);

}

/*!
 * \brief MainWindow::connectStatusBar
 */
void MainWindow::connectStatusBar(){

    //connect unit updates
    QObject::connect(&this->control, &Controller::updateStatusBar, this, &MainWindow::updateStatusBar, Qt::AutoConnection);

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

    //assign console model
    this->ui->listView_console->setModel(&Console::getInstance()->getConsoleModel());

    //assign groups model
    this->ui->comboBox_groups->setModel(&ModelManager::getGroupNamesModel());

    //assign coordinate system model
    this->ui->comboBox_activeCoordSystem->setModel(&ModelManager::getCoordinateSystemsModel());

    //assign actual nominal filter model
    this->ui->comboBox_actualNominal->setModel(&ModelManager::getActualNominalFilterModel());

}

/*!
 * \brief MainWindow::initFeatureTableViews
 */
void MainWindow::initFeatureTableViews(){

    //resize rows and columns to table view contents on double click
    QObject::connect(this->ui->tableView_features->horizontalHeader(), &QHeaderView::sectionDoubleClicked, this, &MainWindow::resizeTableView, Qt::AutoConnection);
    this->ui->tableView_features->verticalHeader()->setDefaultSectionSize(22);
    QObject::connect(this->ui->tableView_trafoParams->horizontalHeader(), &QHeaderView::sectionDoubleClicked, this, &MainWindow::resizeTableView, Qt::AutoConnection);
    this->ui->tableView_trafoParams->verticalHeader()->setDefaultSectionSize(22);

    //enable context menu
    this->ui->tableView_features->setContextMenuPolicy(Qt::CustomContextMenu);
    this->ui->tableView_trafoParams->setContextMenuPolicy(Qt::CustomContextMenu);

    //change active feature by using up and down keys
    QObject::connect(this->ui->tableView_features->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::tableViewFeaturesSelectionChangedByKeyboard, Qt::AutoConnection);
    QObject::connect(this->ui->tableView_trafoParams->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::tableViewTrafoParamsSelectionChangedByKeyboard, Qt::AutoConnection);

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
    this->actionMeasure->setText("measure");
    this->actionAim = new QAction(0);
    this->actionAim->setShortcut(QKeySequence(Qt::ALT + Qt::Key_A));
    this->actionAim->setText("aim");
    this->actionMove = new QAction(0);
    this->actionMove->setText("move");
    this->actionHome = new QAction(0);
    this->actionHome->setShortcut(Qt::Key_F9);
    this->actionHome->setText("home");
    this->actionChangeMotorState = new QAction(0);
    this->actionChangeMotorState->setText("change motor state");
    this->actionToggleSightOrientation = new QAction(0);
    this->actionToggleSightOrientation->setText("toggle sight orientation");
    this->actionCompensation = new QAction(0);
    this->actionCompensation->setText("compensation");

    //add the actions to the sensor pad
    this->ui->toolBar_controlPad->addAction(this->actionConnect);
    this->ui->toolBar_controlPad->addAction(this->actionDisconnect);
    this->ui->toolBar_controlPad->addAction(this->actionMeasure);
    this->ui->toolBar_controlPad->addAction(this->actionAim);
    this->ui->toolBar_controlPad->addAction(this->actionMove);
    this->ui->toolBar_controlPad->addAction(this->actionHome);
    this->ui->toolBar_controlPad->addAction(this->actionChangeMotorState);
    this->ui->toolBar_controlPad->addAction(this->actionToggleSightOrientation);
    this->ui->toolBar_controlPad->addAction(this->actionCompensation);

    //disable and hide actions as default
    this->actionConnect->setVisible(false);
    this->actionConnect->setEnabled(false);
    this->actionDisconnect->setVisible(false);
    this->actionDisconnect->setEnabled(false);
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

    //connect actions
    QObject::connect(this->actionConnect, &QAction::triggered, &this->control, &Controller::startConnect, Qt::AutoConnection);
    QObject::connect(this->actionDisconnect, &QAction::triggered, &this->control, &Controller::startDisconnect, Qt::AutoConnection);
    QObject::connect(this->actionMeasure, &QAction::triggered, &this->control, &Controller::startMeasurement, Qt::AutoConnection);
    QObject::connect(this->actionAim, &QAction::triggered, &this->control, &Controller::startAim, Qt::AutoConnection);
    QObject::connect(this->actionHome, &QAction::triggered, &this->control, &Controller::startHome, Qt::AutoConnection);
    QObject::connect(this->actionChangeMotorState, &QAction::triggered, &this->control, &Controller::startChangeMotorState, Qt::AutoConnection);
    QObject::connect(this->actionToggleSightOrientation, &QAction::triggered, &this->control, &Controller::startToggleSight, Qt::AutoConnection);
    QObject::connect(this->actionCompensation, &QAction::triggered, &this->control, &Controller::startCompensation, Qt::AutoConnection);
    QObject::connect(this->actionMove, &QAction::triggered, this, &MainWindow::showMoveSensorDialog, Qt::AutoConnection);

}

/*!
 * \brief MainWindow::initToolMenus
 */
void MainWindow::initToolMenus(){

    //get all available tool plugins
    QList<QPointer<Tool> > tools = this->control.getAvailableTools();

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
        QObject::connect(action, &ToolAction::openToolWidget, this, &MainWindow::showToolWidget);

        //delete the tool
        delete tool;

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

/*!
 * \brief MainWindow::initStatusBar
 */
void MainWindow::initStatusBar(){

    //create GUI elements
    this->label_statusSensor = new QLabel();
    this->label_statusUnitMetric = new QLabel();
    this->label_statusUnitAngular = new QLabel();
    this->label_statusUnitTemperature = new QLabel();

    //format GUI elements
    this->label_statusUnitMetric->setMinimumWidth(50);
    this->label_statusUnitMetric->setAlignment(Qt::AlignHCenter);
    this->label_statusUnitAngular->setMinimumWidth(50);
    this->label_statusUnitAngular->setAlignment(Qt::AlignHCenter);
    this->label_statusUnitTemperature->setMinimumWidth(50);
    this->label_statusUnitTemperature->setAlignment(Qt::AlignHCenter);

    //add GUI elements to status bar
    this->ui->statusBar->addPermanentWidget(this->label_statusSensor);
    this->ui->statusBar->addPermanentWidget(this->label_statusUnitMetric);
    this->ui->statusBar->addPermanentWidget(this->label_statusUnitAngular);
    this->ui->statusBar->addPermanentWidget(this->label_statusUnitTemperature);

    //show initial status
    this->updateStatusBar();

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

        break;

    case eTotalStation:

        this->actionConnect->setVisible(true);
        this->actionConnect->setEnabled(true);
        this->actionDisconnect->setVisible(true);
        this->actionDisconnect->setEnabled(true);
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
        QPointer<QAction> customAction = new QAction(0);
        customAction->setText(action);
        this->selfDefinedActions.append(customAction);
        this->ui->toolBar_controlPad->addAction(customAction.data());
        QObject::connect(customAction.data(), SIGNAL(triggered()), this->customActionMapper.data(), SLOT(map()), Qt::AutoConnection);
        this->customActionMapper->setMapping(customAction.data(), action);
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
        this->ui->label_magnifyActualNominal->setText(feature->getGeometry()->getIsNominal()?"nominal":"actual");
    }
    this->ui->label_magnifyActualNominal->setText("actual");

    //resize labels to maximum
    QFont fontActualNominal = this->ui->label_magnifyActualNominal->font();
    double h = this->ui->label_magnifyActualNominal->height();
    double w = this->ui->label_magnifyActualNominal->width();
    QFontMetrics fmActualNominal(fontActualNominal);
    double scaleW = w/fmActualNominal.width(this->ui->label_magnifyActualNominal->text());
    double scaleH = h/fmActualNominal.height();
    if(scaleH > scaleW){
        fontActualNominal.setPointSizeF(fontActualNominal.pointSizeF()*scaleW);
    }else{
        fontActualNominal.setPointSizeF(fontActualNominal.pointSizeF()*scaleH);
    }
    this->ui->label_magnifyActualNominal->setFont(fontActualNominal);
    QFont fontName = this->ui->label_magnifyName->font();
    h = this->ui->label_magnifyName->height();
    w = this->ui->label_magnifyName->width();
    QFontMetrics fmName(fontName);
    scaleW = w/fmName.width(this->ui->label_magnifyName->text());
    scaleH = h/fmName.height();
    if(scaleH > scaleW){
        fontName.setPointSizeF(fontName.pointSizeF()*scaleW);
    }else{
        fontName.setPointSizeF(fontName.pointSizeF()*scaleH);
    }
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

    //get the largest group name
    const QStringList &groupNames = job->getFeatureGroupList();
    QString largestGroup = "All Groups";
    foreach(const QString &group, groupNames){
        if(group.length() > largestGroup.length()){
            largestGroup = group;
        }
    }

    //update combobox width with respect to the largest item
    this->ui->comboBox_groups->setMinimumContentsLength(largestGroup.length());

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

    //get the largest system name
    const QList<QPointer<CoordinateSystem> > &nominalSystems = job->getCoordinateSystemsList();
    QList<QPointer<CoordinateSystem> > &stationSystems = job->getStationSystemsList();
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

    //update combobox width with respect to the largest item
    this->ui->comboBox_activeCoordSystem->setMinimumContentsLength(largestSystemName.length());

}

/*!
 * \brief MainWindow::updateActualNominalFilterSize
 */
void MainWindow::updateActualNominalFilterSize(){

    //get the largest group name
    QStringList actualNominalfilters = ModelManager::getActualNominalFilterModel().stringList();
    QString largestFilter = "";
    foreach(const QString &filter, actualNominalfilters){
        if(filter.length() > largestFilter.length()){
            largestFilter = filter;
        }
    }

    //update combobox width with respect to the largest item
    this->ui->comboBox_actualNominal->setMinimumContentsLength(largestFilter.length());

}
