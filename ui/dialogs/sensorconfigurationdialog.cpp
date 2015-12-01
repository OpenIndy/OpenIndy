#include "sensorconfigurationdialog.h"
#include "ui_sensorconfigurationdialog.h"

/*!
 * \brief SensorConfigurationDialog::SensorConfigurationDialog
 * \param parent
 */
SensorConfigurationDialog::SensorConfigurationDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::SensorConfigurationDialog)
{
    ui->setupUi(this);

    //init models
    this->initModels();

    //init GUI elements
    this->initGUI();

}

/*!
 * \brief SensorConfigurationDialog::~SensorConfigurationDialog
 */
SensorConfigurationDialog::~SensorConfigurationDialog(){

}

/*!
 * \brief SensorConfigurationDialog::on_pushButton_save_clicked
 */
void SensorConfigurationDialog::on_pushButton_save_clicked(){

    //check sensor config model
    if(this->sensorConfigModel.isNull()){
        return;
    }

    emit this->sensorConfigurationsEdited(this->sensorConfigModel->getSensorConfigurationManager());

    this->setChangesMade(false);

}

/*!
 * \brief SensorConfigurationDialog::on_pushButton_setAndConnect_clicked
 */
void SensorConfigurationDialog::on_pushButton_setAndConnect_clicked(){

    //check sensor config model
    if(this->sensorConfigModel.isNull()){
        return;
    }

    //check active sensor config
    if(!this->sensorConfigModel->getActiveSensorConfig().getIsValid()){
        emit this->sendMessage("No sensor configuration selected", eErrorMessage, eMessageBoxMessage);
        return;
    }

    emit this->sensorConfigurationsEdited(this->sensorConfigModel->getSensorConfigurationManager());
    emit this->setSensorConfig(this->sensorConfigModel->getActiveSensorConfig());

    this->close();

}

/*!
 * \brief SensorConfigurationDialog::on_pushButton_cancel_clicked
 */
void SensorConfigurationDialog::on_pushButton_cancel_clicked(){
    this->close();
}

/*!
 * \brief SensorConfigurationDialog::on_pushButton_remove_clicked
 */
void SensorConfigurationDialog::on_pushButton_remove_clicked(){

    //check model
    if(this->sensorConfigModel.isNull()){
        return;
    }

    //get active sensor config and remove it
    QModelIndex index = this->sensorConfigModel->getActiveSensorConfigIndex();
    bool success = this->sensorConfigModel->removeSensorConfiguration(index);

    //update change status
    if(success){
        this->setChangesMade(true);
    }

}

/*!
 * \brief SensorConfigurationDialog::on_pushButton_add_clicked
 */
void SensorConfigurationDialog::on_pushButton_add_clicked(){

    //check model
    if(this->sensorConfigModel.isNull()){
        return;
    }

    SensorConfiguration sConfig = this->sensorConfigModel->addSensorConfiguration();

    //update change status
    if(sConfig.getIsValid()){
        this->setChangesMade(true);
    }

}

/*!
 * \brief SensorConfigurationDialog::activeSensorConfigChanged
 * Called whenever there is another sensor config selected in sensor config model
 */
void SensorConfigurationDialog::activeSensorConfigChanged(){

    //check models
    if(this->sensorConfigModel.isNull() || this->sensorAccuracyModel.isNull()
            || this->sensorParametersModel.isNull()){
        return;
    }

    //get sensor config
    const SensorConfiguration &sConfig = this->sensorConfigModel->getActiveSensorConfig();

    //update models and GUI based on the selected config
    this->sensorAccuracyModel->setSensorConfiguration(sConfig);
    this->sensorParametersModel->setSensorConfiguration(sConfig);
    this->updateGuiFromSensorConfig(sConfig);

    //update sensor table model
    ModelManager::getSensorTableModel().blockSignals(true);
    ModelManager::getSensorTableModel().selectSensorPlugin(sConfig);
    ModelManager::getSensorTableModel().blockSignals(false);
    ModelManager::getSensorTableModel().updateModel();

}

/*!
 * \brief SensorConfigurationDialog::sensorConfigRenamed
 */
void SensorConfigurationDialog::sensorConfigRenamed(){
    this->changesMade = true;
}

/*!
 * \brief SensorConfigurationDialog::sensorConfigsChanged
 */
void SensorConfigurationDialog::sensorConfigsChanged(){

    //check model
    if(this->sensorConfigModel.isNull()){
        return;
    }

    this->sensorConfigModel->updateModel();

}

/*!
 * \brief SensorConfigurationDialog::sensorAccuracyChanged
 * \param sConfig
 */
void SensorConfigurationDialog::sensorAccuracyChanged(const SensorConfiguration &sConfig){

    //check config
    if(!sConfig.getIsValid()){
        return;
    }

    //check model
    if(this->sensorConfigModel.isNull()){
        return;
    }

    //replace sensor config
    this->sensorConfigModel->updateSensorAccuracy(sConfig);

    //update change status
    this->setChangesMade(true);

    qDebug() << "sensor accuracy changed";

}

/*!
 * \brief SensorConfigurationDialog::sensorParametersChanged
 * \param sConfig
 */
void SensorConfigurationDialog::sensorParametersChanged(const SensorConfiguration &sConfig){

    //check config
    if(!sConfig.getIsValid()){
        return;
    }

    //check model
    if(this->sensorConfigModel.isNull()){
        return;
    }

    //replace sensor config
    this->sensorConfigModel->updateSensorParameters(sConfig);

    //update change status
    this->setChangesMade(true);

    qDebug() << "sensor parameters changed";

}

/*!
 * \brief SensorConfigurationDialog::sensorConnectionChanged
 */
void SensorConfigurationDialog::sensorConnectionChanged(){

    //check model
    if(this->sensorConfigModel.isNull()){
        return;
    }

    //get and check active sensor config
    SensorConfiguration sConfig = this->sensorConfigModel->getActiveSensorConfig();
    if(!sConfig.getIsValid()){
        return;
    }

    //set up connection config from GUI
    ConnectionConfig cConfig;
    cConfig.typeOfConnection = getConnectionTypeEnum(this->ui->comboBox_typeOfConnection->currentText());
    cConfig.ip = this->ui->comboBox_ip->currentText();
    cConfig.port = this->ui->lineEdit_port->text();
    cConfig.baudRate = static_cast<QSerialPort::BaudRate>(this->ui->comboBox_baudrate->itemData(ui->comboBox_baudrate->currentIndex()).toInt());
    cConfig.comPort = this->ui->comboBox_comPort->currentText();;
    cConfig.dataBits = static_cast<QSerialPort::DataBits>(this->ui->comboBox_baudrate->itemData(ui->comboBox_baudrate->currentIndex()).toInt());
    cConfig.flowControl = static_cast<QSerialPort::FlowControl>(this->ui->comboBox_baudrate->itemData(ui->comboBox_baudrate->currentIndex()).toInt());
    cConfig.parity = static_cast<QSerialPort::Parity>(this->ui->comboBox_baudrate->itemData(ui->comboBox_baudrate->currentIndex()).toInt());
    cConfig.stopBits = static_cast<QSerialPort::StopBits>(this->ui->comboBox_baudrate->itemData(ui->comboBox_baudrate->currentIndex()).toInt());
    sConfig.setConnectionConfig(cConfig);

    //toggle visibility depending on the type of connection
    this->toggleVisibility(sConfig);

    //replace sensor config
    this->sensorConfigModel->updateSensorConnection(sConfig);

    //update change status
    this->setChangesMade(true);

    qDebug() << "sensor connection changed";

}

/*!
 * \brief SensorConfigurationDialog::sensorPluginChanged
 * \param index
 */
void SensorConfigurationDialog::sensorPluginChanged(){

    //check model
    if(this->sensorConfigModel.isNull()){
        return;
    }

    //get and check active sensor config
    SensorConfiguration sConfig = this->sensorConfigModel->getActiveSensorConfig();
    if(!sConfig.getIsValid()){
        return;
    }

    //get and check default sensor config
    SensorConfiguration defaultConfig = ModelManager::getSensorTableModel().getDefaultSensorConfig();
    if(!defaultConfig.getIsValid()){
        return;
    }

    //compare active and default sensor config
    if(sConfig.getPluginName().compare(defaultConfig.getPluginName()) == 0
            && sConfig.getSensorName().compare(defaultConfig.getSensorName()) == 0){
        return;
    }

    //replace sensor config
    this->sensorConfigModel->updateSensorPlugin(defaultConfig);

    //update change status
    this->setChangesMade(true);

    qDebug() << "sensor plugin changed";

}

/*!
 * \brief SensorConfigurationDialog::on_comboBox_availableSensorTypes_currentIndexChanged
 * \param arg1
 */
void SensorConfigurationDialog::on_comboBox_availableSensorTypes_currentIndexChanged(const QString &arg1){
    ModelManager::getSensorTableProxyModel().setSensorType(getSensorTypeEnum(arg1));
}

/*!
 * \brief SensorConfigurationDialog::configSelectionChanged
 * \param selected
 * \param deselected
 */
void SensorConfigurationDialog::configSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected){

    //check sensor config model
    if(this->sensorConfigModel.isNull()){
        return;
    }

    //check selection
    if(selected.size() != 1){
        return;
    }

    //get and check index
    QModelIndex index = selected.indexes().first();
    if(!index.isValid()){
        return;
    }

    this->sensorConfigModel->setActiveSensorConfig(index);

}

/*!
 * \brief SensorConfigurationDialog::sensorPluginSelectionChanged
 * \param index
 */
void SensorConfigurationDialog::sensorPluginSelectionChanged(const QModelIndex &index){

    //check index
    if(!index.isValid()){
        return;
    }

    //convert index
    QModelIndex sourceIndex = ModelManager::getSensorTableProxyModel().mapToSource(index);

    //select sensor plugin
    ModelManager::getSensorTableModel().selectSensorPlugin(sourceIndex);

}

/*!
 * \brief SensorConfigurationDialog::updateGuiFromSensorConfig
 * \param sConfig
 */
void SensorConfigurationDialog::updateGuiFromSensorConfig(const SensorConfiguration &sConfig){

    //toggle visibility depending on connection type
    this->toggleVisibility(sConfig);

    //do not trigger edits while setting up connection config
    this->ui->comboBox_typeOfConnection->blockSignals(true);
    this->ui->comboBox_baudrate->blockSignals(true);
    this->ui->comboBox_databits->blockSignals(true);
    this->ui->comboBox_flowcontrol->blockSignals(true);
    this->ui->comboBox_parity->blockSignals(true);
    this->ui->comboBox_stopbits->blockSignals(true);
    this->ui->comboBox_comPort->blockSignals(true);
    this->ui->comboBox_ip->blockSignals(true);
    this->ui->lineEdit_port->blockSignals(true);

    //set up available connection types
    QList<ConnectionTypes> supportedConnections = ModelManager::getSensorTableModel().getSupportedConnectionTypes();
    this->ui->comboBox_typeOfConnection->clear();
    foreach(const ConnectionTypes &type, supportedConnections){
        this->ui->comboBox_typeOfConnection->addItem(getConnectionTypeName(type));
    }
    this->ui->comboBox_typeOfConnection->setCurrentText(getConnectionTypeName(sConfig.getConnectionConfig().typeOfConnection));

    //set up connection parameters
    const ConnectionConfig &cConfig = sConfig.getConnectionConfig();
    int baudRateIndex = this->ui->comboBox_baudrate->findData(cConfig.baudRate, Qt::UserRole);
    if(baudRateIndex >= 0){
        this->ui->comboBox_baudrate->setCurrentIndex(baudRateIndex);
    }
    int dataBitsIndex = this->ui->comboBox_databits->findData(cConfig.dataBits, Qt::UserRole);
    if(dataBitsIndex >= 0){
        this->ui->comboBox_databits->setCurrentIndex(dataBitsIndex);
    }
    int flowControlIndex = this->ui->comboBox_flowcontrol->findData(cConfig.flowControl, Qt::UserRole);
    if(flowControlIndex >= 0){
        this->ui->comboBox_flowcontrol->setCurrentIndex(flowControlIndex);
    }
    int parityIndex = this->ui->comboBox_parity->findData(cConfig.parity, Qt::UserRole);
    if(parityIndex >= 0){
        this->ui->comboBox_parity->setCurrentIndex(parityIndex);
    }
    int stopBitsIndex = this->ui->comboBox_stopbits->findData(cConfig.stopBits, Qt::UserRole);
    if(stopBitsIndex >= 0){
        this->ui->comboBox_stopbits->setCurrentIndex(stopBitsIndex);
    }
    int comPortIndex = this->ui->comboBox_comPort->findData(cConfig.comPort, Qt::UserRole);
    if(comPortIndex >= 0){
        this->ui->comboBox_comPort->setCurrentIndex(comPortIndex);
    }
    this->ui->comboBox_ip->setCurrentText(cConfig.ip);
    this->ui->lineEdit_port->setText(cConfig.port);

    //from now on trigger edits
    this->ui->comboBox_typeOfConnection->blockSignals(false);
    this->ui->comboBox_baudrate->blockSignals(false);
    this->ui->comboBox_databits->blockSignals(false);
    this->ui->comboBox_flowcontrol->blockSignals(false);
    this->ui->comboBox_parity->blockSignals(false);
    this->ui->comboBox_stopbits->blockSignals(false);
    this->ui->comboBox_comPort->blockSignals(false);
    this->ui->comboBox_ip->blockSignals(false);
    this->ui->lineEdit_port->blockSignals(false);

    //select the correct sensor type
    this->ui->comboBox_availableSensorTypes->blockSignals(true);
    this->ui->comboBox_availableSensorTypes->setCurrentText(getSensorTypeName(sConfig.getTypeOfSensor()));
    this->ui->comboBox_availableSensorTypes->blockSignals(false);
    ModelManager::getSensorTableProxyModel().setSensorType(sConfig.getTypeOfSensor());

    //update sensor description
    QString description = ModelManager::getSensorTableModel().getSensorDescription();
    this->ui->textBrowser_description->setText(description);

    //check models
    if(this->sensorAccuracyModel.isNull() || this->sensorParametersModel.isNull()){
        return;
    }

    //update readonly state of models
    if(sConfig.getIsSaved()){
        this->sensorAccuracyModel->setIsReadOnly(false);
        this->sensorParametersModel->setIsReadOnly(false);
    }else{
        this->sensorAccuracyModel->setIsReadOnly(true);
        this->sensorParametersModel->setIsReadOnly(true);
    }

}

/*!
 * \brief SensorConfigurationDialog::toggleVisibility
 * \param sConfig
 */
void SensorConfigurationDialog::toggleVisibility(const SensorConfiguration &sConfig){

    //sensor config
    if(!sConfig.getIsValid()){

        //set connection field invisible
        this->ui->comboBox_baudrate->setVisible(false);
        this->ui->label_baudRate->setVisible(false);
        this->ui->comboBox_databits->setVisible(false);
        this->ui->label_databits->setVisible(false);
        this->ui->comboBox_flowcontrol->setVisible(false);
        this->ui->label_flowControl->setVisible(false);
        this->ui->comboBox_parity->setVisible(false);
        this->ui->label_parity->setVisible(false);
        this->ui->comboBox_stopbits->setVisible(false);
        this->ui->label_stopBits->setVisible(false);
        this->ui->comboBox_comPort->setVisible(false);
        this->ui->label_comPort->setVisible(false);
        this->ui->comboBox_ip->setVisible(false);
        this->ui->label_ip->setVisible(false);
        this->ui->lineEdit_port->setVisible(false);
        this->ui->label_port->setVisible(false);

        //disable sensor config tab-widget
        this->ui->tabWidget_sensorConfig->setEnabled(false);

        return;

    }

    //set visibility depending on connection type
    if(sConfig.getConnectionConfig().typeOfConnection == eSerialConnection){
        this->ui->comboBox_baudrate->setVisible(true);
        this->ui->label_baudRate->setVisible(true);
        this->ui->comboBox_databits->setVisible(true);
        this->ui->label_databits->setVisible(true);
        this->ui->comboBox_flowcontrol->setVisible(true);
        this->ui->label_flowControl->setVisible(true);
        this->ui->comboBox_parity->setVisible(true);
        this->ui->label_parity->setVisible(true);
        this->ui->comboBox_stopbits->setVisible(true);
        this->ui->label_stopBits->setVisible(true);
        this->ui->comboBox_comPort->setVisible(true);
        this->ui->label_comPort->setVisible(true);
        this->ui->comboBox_ip->setVisible(false);
        this->ui->label_ip->setVisible(false);
        this->ui->lineEdit_port->setVisible(false);
        this->ui->label_port->setVisible(false);
    }else if(sConfig.getConnectionConfig().typeOfConnection == eNetworkConnection){
        this->ui->comboBox_baudrate->setVisible(false);
        this->ui->label_baudRate->setVisible(false);
        this->ui->comboBox_databits->setVisible(false);
        this->ui->label_databits->setVisible(false);
        this->ui->comboBox_flowcontrol->setVisible(false);
        this->ui->label_flowControl->setVisible(false);
        this->ui->comboBox_parity->setVisible(false);
        this->ui->label_parity->setVisible(false);
        this->ui->comboBox_stopbits->setVisible(false);
        this->ui->label_stopBits->setVisible(false);
        this->ui->comboBox_comPort->setVisible(false);
        this->ui->label_comPort->setVisible(false);
        this->ui->comboBox_ip->setVisible(true);
        this->ui->label_ip->setVisible(true);
        this->ui->lineEdit_port->setVisible(true);
        this->ui->label_port->setVisible(true);
    }

    //enable sensor config tab-widget
    this->ui->tabWidget_sensorConfig->setEnabled(true);

}

/*!
 * \brief SensorConfigurationDialog::toggleCommandVisibility
 */
void SensorConfigurationDialog::toggleCommandVisibility(){

    //disable or enable buttons depending on wether there are unsaved changes
    if(this->changesMade){
        this->ui->pushButton_setAndConnect->setEnabled(false);
        this->ui->pushButton_save->setEnabled(true);
    }else{
        this->ui->pushButton_setAndConnect->setEnabled(true);
        this->ui->pushButton_save->setEnabled(false);
    }

}

/*!
 * \brief SensorConfigurationDialog::setChangesMade
 * \param changesMade
 */
void SensorConfigurationDialog::setChangesMade(bool changesMade){
    this->changesMade = changesMade;
    this->toggleCommandVisibility();
}

/*!
 * \brief SensorConfigurationDialog::showEvent
 * \param event
 */
void SensorConfigurationDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    //update models and GUI based on the selected config
    if(!this->sensorConfigModel.isNull()){

        //assign current sensor config manager
        if(!ModelManager::getSensorConfigManager().isNull() && !this->sensorConfigModel.isNull()){
            SensorConfigurationManager manager(*ModelManager::getSensorConfigManager().data());
            this->sensorConfigModel->setSensorConfigurationManager(manager);
        }

        //get sensor config
        QModelIndex index = this->sensorConfigModel->getActiveSensorConfigIndex();
        const SensorConfiguration &sConfig = this->sensorConfigModel->getActiveSensorConfig();

        //select the active sensor config
        QItemSelectionModel *selectionModel = this->ui->listView_sensorConfigs->selectionModel();
        if(selectionModel != NULL && index.isValid()){

            selectionModel->select(index, QItemSelectionModel::SelectCurrent);

        }else{
            this->sensorAccuracyModel->setSensorConfiguration(sConfig);
            this->sensorParametersModel->setSensorConfiguration(sConfig);
            ModelManager::getSensorTableModel().selectSensorPlugin(sConfig);
            this->updateGuiFromSensorConfig(sConfig);
        }

    }

    //update change status
    this->setChangesMade(false);

    event->accept();

}

/*!
 * \brief SensorConfigurationDialog::closeEvent
 * \param event
 */
void SensorConfigurationDialog::closeEvent(QCloseEvent *event){

    //check if changes were made
    if(this->changesMade){

        //show message box and ask the user wether to close the dialog or not
        QMessageBox msgBox;
        msgBox.setText("There are unsaved changes available. When pressing OK this changes are rejected.");
        msgBox.setInformativeText("Do you want to reject all changes?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Warning);
        int ret = msgBox.exec();

        //check user input
        switch (ret) {
        case QMessageBox::Ok:
            break;
        case QMessageBox::Cancel:
            event->ignore();
            return;
        default:
            break;
        }

    }

    event->accept();

}

/*!
 * \brief SensorConfigurationDialog::initGUI
 */
void SensorConfigurationDialog::initGUI(){

    //init table views
    this->ui->tableView_accuracy->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tableView_accuracy->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->tableView_accuracy->setEditTriggers(QAbstractItemView::DoubleClicked);
    this->ui->tableView_sensorParams->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tableView_sensorParams->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->tableView_sensorParams->setEditTriggers(QAbstractItemView::DoubleClicked);
    this->ui->tableView_sensorPlugins->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->tableView_sensorPlugins->verticalHeader()->setDefaultSectionSize(22);

    //connect GUI interactions
    QObject::connect(this->ui->tableView_sensorPlugins, &QTableView::doubleClicked,
                     this, &SensorConfigurationDialog::sensorPluginSelectionChanged, Qt::AutoConnection);
    QObject::connect(this->ui->comboBox_typeOfConnection, &QComboBox::currentTextChanged,
                     this, &SensorConfigurationDialog::sensorConnectionChanged, Qt::AutoConnection);
    QObject::connect(this->ui->comboBox_ip, &QComboBox::currentTextChanged,
                     this, &SensorConfigurationDialog::sensorConnectionChanged, Qt::AutoConnection);
    QObject::connect(this->ui->lineEdit_port, &QLineEdit::textChanged,
                     this, &SensorConfigurationDialog::sensorConnectionChanged, Qt::AutoConnection);
    QObject::connect(this->ui->comboBox_baudrate, &QComboBox::currentTextChanged,
                     this, &SensorConfigurationDialog::sensorConnectionChanged, Qt::AutoConnection);
    QObject::connect(this->ui->comboBox_comPort, &QComboBox::currentTextChanged,
                     this, &SensorConfigurationDialog::sensorConnectionChanged, Qt::AutoConnection);
    QObject::connect(this->ui->comboBox_databits, &QComboBox::currentTextChanged,
                     this, &SensorConfigurationDialog::sensorConnectionChanged, Qt::AutoConnection);
    QObject::connect(this->ui->comboBox_flowcontrol, &QComboBox::currentTextChanged,
                     this, &SensorConfigurationDialog::sensorConnectionChanged, Qt::AutoConnection);
    QObject::connect(this->ui->comboBox_parity, &QComboBox::currentTextChanged,
                     this, &SensorConfigurationDialog::sensorConnectionChanged, Qt::AutoConnection);
    QObject::connect(this->ui->comboBox_stopbits, &QComboBox::currentTextChanged,
                     this, &SensorConfigurationDialog::sensorConnectionChanged, Qt::AutoConnection);

}

/*!
 * \brief SensorConfigurationDialog::initModels
 */
void SensorConfigurationDialog::initModels(){

    //init sensor models
    this->ui->tableView_sensorPlugins->setModel(&ModelManager::getSensorTableProxyModel());
    this->ui->comboBox_availableSensorTypes->setModel(&ModelManager::getSensorTypeNamesModel());

    //init sensor config model
    this->sensorConfigModel = ModelManager::getSensorConfigurationModel(this);
    this->sensorConfigProxyModel = ModelManager::getSensorConfigurationProxyModel(this->sensorConfigModel);
    this->ui->listView_sensorConfigs->setModel(this->sensorConfigProxyModel);
    SensorConfigurationListDelegate *sensorConfigDelegate = new SensorConfigurationListDelegate(this->sensorConfigModel);
    this->ui->listView_sensorConfigs->setItemDelegate(sensorConfigDelegate);

    //init sensor accuracy model
    this->sensorAccuracyModel = ModelManager::getSensorAccuracyModel(this);
    this->ui->tableView_accuracy->setModel(this->sensorAccuracyModel);
    SensorAccuracyDelegate *sensorAccuracyDelegate = new SensorAccuracyDelegate(this);
    this->ui->tableView_accuracy->setItemDelegate(sensorAccuracyDelegate);

    //init sensor parameters model
    this->sensorParametersModel = ModelManager::getSensorParametersModel(this);
    this->ui->tableView_sensorParams->setModel(this->sensorParametersModel);
    SensorParametersDelegate *sensorParametersDelegate = new SensorParametersDelegate(this);
    this->ui->tableView_sensorParams->setItemDelegate(sensorParametersDelegate);

    //init connection config models
    this->ui->comboBox_baudrate->setModel(&ModelManager::getBaudRateTypesModel());
    this->ui->comboBox_databits->setModel(&ModelManager::getDataBitTypesModel());
    this->ui->comboBox_flowcontrol->setModel(&ModelManager::getFlowControlTypesModel());
    this->ui->comboBox_parity->setModel(&ModelManager::getParityTypesModel());
    this->ui->comboBox_stopbits->setModel(&ModelManager::getStopBitTypesModel());
    this->ui->comboBox_comPort->setModel(&ModelManager::getAvailableSerialPortsModel());
    this->ui->comboBox_ip->setModel(&ModelManager::getAvailableIpAdressesModel());

    this->connectModels();

}

/*!
 * \brief SensorConfigurationDialog::connectModels
 */
void SensorConfigurationDialog::connectModels(){

    //connect configs list
    QItemSelectionModel *selectionModel = this->ui->listView_sensorConfigs->selectionModel();
    if(selectionModel != NULL){
        QObject::connect(selectionModel, &QItemSelectionModel::selectionChanged,
                         this, &SensorConfigurationDialog::configSelectionChanged, Qt::AutoConnection);
    }

    //connect sensor config model
    QObject::connect(this->sensorConfigModel, &SensorConfigurationModel::activeSensorConfigChanged,
                     this, &SensorConfigurationDialog::activeSensorConfigChanged, Qt::AutoConnection);
    QObject::connect(this->sensorConfigModel, &SensorConfigurationModel::sensorConfigsChanged,
                     this, &SensorConfigurationDialog::sensorConfigsChanged, Qt::AutoConnection);
    QObject::connect(this->sensorConfigModel, &SensorConfigurationModel::sensorConfigRenamed,
                     this, &SensorConfigurationDialog::sensorConfigRenamed, Qt::AutoConnection);

    //connect sensor table model
    QObject::connect(&ModelManager::getSensorTableModel(), &SensorTableModel::selectedSensorPluginChanged,
                     this, &SensorConfigurationDialog::sensorPluginChanged, Qt::AutoConnection);

    //connect sensor accuracy model
    QObject::connect(this->sensorAccuracyModel, &SensorAccuracyModel::sensorConfigurationChanged,
                     this, &SensorConfigurationDialog::sensorAccuracyChanged, Qt::AutoConnection);

    //connect sensor parameters model
    QObject::connect(this->sensorParametersModel, &SensorParametersModel::sensorConfigurationChanged,
                     this, &SensorConfigurationDialog::sensorParametersChanged, Qt::AutoConnection);

}
