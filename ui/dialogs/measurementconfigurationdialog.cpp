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

    //init GUI elements
    this->initGUI();

    //init models
    this->initModels();

}

/*!
 * \brief SensorConfigurationDialog::~SensorConfigurationDialog
 */
SensorConfigurationDialog::~SensorConfigurationDialog(){

}

/*!
 * \brief SensorConfigurationDialog::scalarParameterChanged
 */
void SensorConfigurationDialog::scalarParameterChanged(){
    this->updateSensorConfigFromSelection();
}

/*!
 * \brief SensorConfigurationDialog::on_listView_sensorConfigs_clicked
 * Triggered whenever the user selects a sensor config
 * \param index
 */
void SensorConfigurationDialog::on_listView_sensorConfigs_clicked(const QModelIndex &index){

    //check index
    if(!index.isValid()){
        return;
    }

    //set widgets visibility
    this->ui->tabWidget_sensorConfig->setEnabled(true);

    //get and check model
    SensorConfigurationProxyModel *sensorConfigProxyModel = static_cast<SensorConfigurationProxyModel *>(this->ui->listView_sensorConfigs->model());
    if(sensorConfigProxyModel == NULL){
        return;
    }

    //get and check source model
    SensorConfigurationModel *sensorConfigModel = static_cast<SensorConfigurationModel *>(sensorConfigProxyModel->sourceModel());
    if(sensorConfigModel == NULL){
        return;
    }

    //get information about the sensor of the selected config
    SensorTypes sensorType = sensorConfigModel->getSensorType(sensorConfigProxyModel->mapToSource(index));
    QString sensorName = sensorConfigModel->getSensorName(sensorConfigProxyModel->mapToSource(index));
    QString pluginName = sensorConfigModel->getPluginName(sensorConfigProxyModel->mapToSource(index));

    //set selected sensor type
    this->ui->comboBox_availableSensorTypes->setCurrentText(getSensorTypeName(sensorType));

    //get and check sensor plugins proxy model
    SensorTableProxyModel *sensorTableProxyModel = static_cast<SensorTableProxyModel *>(this->ui->tableView_sensorPlugins->model());
    if(sensorTableProxyModel == NULL){
        return;
    }

    SensorTableModel *sensorTableModel = static_cast<SensorTableModel *>(sensorTableProxyModel->sourceModel());
    if(sensorTableProxyModel == NULL){
        return;
    }

    //select the corresponding sensor plugin
    sensorTableModel->selectSensorPlugin(sensorName, pluginName);

    //set up connection config
    ConnectionConfig cConfig = sensorConfigModel->getConnectionConfig(sensorConfigProxyModel->mapToSource(index));
    this->updateConnectionConfigFromSensorConfig(sensorTableModel->getSupportedConnectionTypes(sensorTableModel->getSelectedIndex()), cConfig);

    //Accuracy accuracy = sensorConfigModel->getAccuracy(sensorConfigProxyModel->mapToSource(index));
    //this->updateAccuracyFromSensorConfig(accuracy);

    //set up scalar parameters
    QMap<QString, int> intParams = sensorConfigModel->getIntegerParameter(sensorConfigProxyModel->mapToSource(index));
    QMap<QString, double> doubleParams = sensorConfigModel->getDoubleParameter(sensorConfigProxyModel->mapToSource(index));
    QMap<QString, QString> stringParams = sensorConfigModel->getStringParameter(sensorConfigProxyModel->mapToSource(index));
    QMultiMap<QString, QString> availableStringOptions = sensorTableModel->getStringParameter();
    this->updateScalarParametersFromSensorConfig(intParams, doubleParams, stringParams, availableStringOptions);

}

/*!
 * \brief SensorConfigurationDialog::on_comboBox_availableSensorTypes_currentIndexChanged
 * \param arg1
 */
void SensorConfigurationDialog::on_comboBox_availableSensorTypes_currentIndexChanged(const QString &arg1){

    //get and check model
    SensorTableProxyModel *sensorTableProxyModel = static_cast<SensorTableProxyModel *>(this->ui->tableView_sensorPlugins->model());
    if(sensorTableProxyModel == NULL){
        return;
    }

    //set filter for sensor table model
    sensorTableProxyModel->setSensorType(getSensorTypeEnum(arg1));

}

/*!
 * \brief SensorConfigurationDialog::on_pushButton_cancel_clicked
 */
void SensorConfigurationDialog::on_pushButton_cancel_clicked(){
    this->close();
}

/*!
 * \brief SensorConfigurationDialog::on_pushButton_set_clicked
 */
void SensorConfigurationDialog::on_pushButton_set_clicked(){

    //get selected sensor config name
    QModelIndexList selection = this->ui->listView_sensorConfigs->selectionModel()->selectedIndexes();
    if(selection.size() != 1){
        Console::getInstance()->addLine("No sensor configuration selected");
        return;
    }
    QString name = selection.at(0).data().toString();

    emit this->setSensorConfiguration(name);

    this->close();

}

/*!
 * \brief SensorConfigurationDialog::on_tableView_sensorPlugins_clicked
 * \param index
 */
void SensorConfigurationDialog::on_tableView_sensorPlugins_clicked(const QModelIndex &index){

    //check index
    if(!index.isValid()){
        return;
    }

    //get and check sensor plugins proxy model
    SensorTableProxyModel *sensorTableProxyModel = static_cast<SensorTableProxyModel *>(this->ui->tableView_sensorPlugins->model());
    if(sensorTableProxyModel == NULL){
        return;
    }

    //get and check sensor plugins model
    SensorTableModel *sensorTableModel = static_cast<SensorTableModel *>(sensorTableProxyModel->sourceModel());
    if(sensorTableProxyModel == NULL){
        return;
    }

    //get and check source index
    QModelIndex sourceIndex = sensorTableProxyModel->mapToSource(index);
    if(!sourceIndex.isValid()){
        return;
    }

    //select the sensor plugin that was selected (only if it is not selected yet)
    if(sourceIndex == sensorTableModel->getSelectedIndex()){
        qDebug() << "same sensor plugin";
        return;
    }

    sensorTableModel->selectSensorPlugin(sensorTableModel->getSensorName(sourceIndex),
                                         sensorTableModel->getPluginName(sourceIndex));
    sourceIndex = sensorTableModel->getSelectedIndex();

    //######################
    //set default parameters
    //######################

    //this->blockSignals(true);

    //set connection config
    ConnectionConfig cConfig;
    QList<ConnectionTypes> connectionTypes = sensorTableModel->getSupportedConnectionTypes(sourceIndex);
    this->updateConnectionConfigFromSensorConfig(connectionTypes, cConfig);

    //set accuracy

    //set sensor parameters
    QMap<QString, int> intParams = sensorTableModel->getIntegerParameter();
    QMap<QString, double> doubleParams = sensorTableModel->getDoubleParameter();
    QMultiMap<QString, QString> availableStringParams = sensorTableModel->getStringParameter();
    QMap<QString, QString> stringParams;
    foreach(const QString &key, availableStringParams.keys()){
        stringParams.insert(key, availableStringParams.value(key));
    }
    this->updateScalarParametersFromSensorConfig(intParams, doubleParams, stringParams, availableStringParams);

    //this->blockSignals(false);

    //#############################
    //update selected sensor config
    //#############################

    this->updateSensorConfigFromSelection();

}

/*!
 * \brief SensorConfigurationDialog::on_comboBox_typeOfConnection_currentIndexChanged
 * \param arg1
 */
void SensorConfigurationDialog::on_comboBox_typeOfConnection_currentIndexChanged(const QString &arg1){

    //set visibility
    if(arg1.compare(getConnectionTypeName(eSerialConnection)) == 0){
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
    }else if(arg1.compare(getConnectionTypeName(eNetworkConnection)) == 0){
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

    //update selected sensor config
    this->updateSensorConfigFromSelection();

}

/*!
 * \brief SensorConfigurationDialog::on_comboBox_ip_currentTextChanged
 * \param arg1
 */
void SensorConfigurationDialog::on_comboBox_ip_currentTextChanged(const QString &arg1){
    this->updateSensorConfigFromSelection();
}

/*!
 * \brief SensorConfigurationDialog::on_lineEdit_port_textChanged
 * \param arg1
 */
void SensorConfigurationDialog::on_lineEdit_port_textChanged(const QString &arg1){
    this->updateSensorConfigFromSelection();
}

/*!
 * \brief SensorConfigurationDialog::on_comboBox_comPort_currentIndexChanged
 * \param index
 */
void SensorConfigurationDialog::on_comboBox_comPort_currentIndexChanged(int index){
    this->updateSensorConfigFromSelection();
}

/*!
 * \brief SensorConfigurationDialog::on_comboBox_baudrate_currentIndexChanged
 * \param index
 */
void SensorConfigurationDialog::on_comboBox_baudrate_currentIndexChanged(int index){
    this->updateSensorConfigFromSelection();
}

/*!
 * \brief SensorConfigurationDialog::on_comboBox_databits_currentIndexChanged
 * \param index
 */
void SensorConfigurationDialog::on_comboBox_databits_currentIndexChanged(int index){
    this->updateSensorConfigFromSelection();
}

/*!
 * \brief SensorConfigurationDialog::on_comboBox_flowcontrol_currentIndexChanged
 * \param index
 */
void SensorConfigurationDialog::on_comboBox_flowcontrol_currentIndexChanged(int index){
    this->updateSensorConfigFromSelection();
}

/*!
 * \brief SensorConfigurationDialog::on_comboBox_parity_currentIndexChanged
 * \param index
 */
void SensorConfigurationDialog::on_comboBox_parity_currentIndexChanged(int index){
    this->updateSensorConfigFromSelection();
}

/*!
 * \brief SensorConfigurationDialog::on_comboBox_stopbits_currentIndexChanged
 * \param index
 */
void SensorConfigurationDialog::on_comboBox_stopbits_currentIndexChanged(int index){
    this->updateSensorConfigFromSelection();
}


/*!
 * \brief SensorConfigurationDialog::updateConnectionConfigFromSensorConfig
 * \param supportedConnections
 * \param cConfig
 */
void SensorConfigurationDialog::updateConnectionConfigFromSensorConfig(const QList<ConnectionTypes> supportedConnections, const ConnectionConfig &cConfig){

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
    this->ui->comboBox_typeOfConnection->clear();
    foreach(const ConnectionTypes &type, supportedConnections){
        this->ui->comboBox_typeOfConnection->addItem(getConnectionTypeName(type));
        this->ui->comboBox_typeOfConnection->setCurrentText(getConnectionTypeName(type));
    }

    //set visibility
    if(this->ui->comboBox_typeOfConnection->currentText().compare(getConnectionTypeName(eSerialConnection)) == 0){
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
    }else if(this->ui->comboBox_typeOfConnection->currentText().compare(getConnectionTypeName(eNetworkConnection)) == 0){
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

    //set up connection parameters
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

}

/*!
 * \brief SensorConfigurationDialog::updateAccuracyFromSensorConfig
 * \param accuracy
 */
void SensorConfigurationDialog::updateAccuracyFromSensorConfig(const Accuracy &accuracy){

}

/*!
 * \brief SensorConfigurationDialog::updateScalarParametersFromSensorConfig
 * \param intParams
 * \param doubleParams
 * \param stringParams
 * \param availableStringOptions
 */
void SensorConfigurationDialog::updateScalarParametersFromSensorConfig(const QMap<QString, int> &intParams, const QMap<QString, double> &doubleParams, const QMap<QString, QString> &stringParams, const QMultiMap<QString, QString> &availableStringOptions){

    this->scalarParameterWidget->setIntParameter(intParams);
    this->scalarParameterWidget->setDoubleParameter(doubleParams);
    this->scalarParameterWidget->setStringParameter(availableStringOptions, stringParams);

}

/*!
 * \brief SensorConfigurationDialog::updateSensorConfigFromSelection
 */
void SensorConfigurationDialog::updateSensorConfigFromSelection(){

    //####################
    //get and check models
    //####################

    SensorConfigurationProxyModel *sensorConfigProxyModel = static_cast<SensorConfigurationProxyModel *>(this->ui->listView_sensorConfigs->model());
    if(sensorConfigProxyModel == NULL){
        return;
    }

    SensorConfigurationModel *sensorConfigModel = static_cast<SensorConfigurationModel *>(sensorConfigProxyModel->sourceModel());
    if(sensorConfigModel == NULL){
        return;
    }

    SensorTableProxyModel *sensorTableProxyModel = static_cast<SensorTableProxyModel *>(this->ui->tableView_sensorPlugins->model());
    if(sensorTableProxyModel == NULL){
        return;
    }

    SensorTableModel *sensorTableModel = static_cast<SensorTableModel *>(sensorTableProxyModel->sourceModel());
    if(sensorTableProxyModel == NULL){
        return;
    }

    //###############################
    //get selected sensor config name
    //###############################

    QModelIndexList selection = this->ui->listView_sensorConfigs->selectionModel()->selectedIndexes();
    //qDebug() << "selection: " << selection.size();
    if(selection.size() != 1){
        return;
    }
    QString name = selection.at(0).data().toString();

    SensorConfiguration sConfig;
    sConfig.setName(name);

    //#######################################
    //set up sensor config from GUI selection
    //#######################################

    //set up sensor plugin
    QModelIndex pluginIndex = sensorTableModel->getSelectedIndex();
    if(!pluginIndex.isValid()){
        return;
    }
    sConfig.setSensorName(sensorTableModel->getSensorName(pluginIndex));
    sConfig.setPluginName(sensorTableModel->getPluginName(pluginIndex));
    sConfig.setTypeOfSensor(sensorTableModel->getSensorType(pluginIndex));

    //set up connection config
    ConnectionConfig cConfig;
    cConfig.baudRate = static_cast<QSerialPort::BaudRate>(this->ui->comboBox_baudrate->itemData(ui->comboBox_baudrate->currentIndex()).toInt());
    cConfig.comPort = this->ui->comboBox_comPort->currentText();
    cConfig.dataBits = static_cast<QSerialPort::DataBits>(this->ui->comboBox_databits->itemData(ui->comboBox_databits->currentIndex()).toInt());
    cConfig.flowControl = static_cast<QSerialPort::FlowControl>(this->ui->comboBox_flowcontrol->itemData(ui->comboBox_flowcontrol->currentIndex()).toInt());
    cConfig.ip = this->ui->comboBox_ip->currentText();
    cConfig.parity = static_cast<QSerialPort::Parity>(this->ui->comboBox_parity->itemData(ui->comboBox_parity->currentIndex()).toInt());
    cConfig.port = this->ui->lineEdit_port->text();
    cConfig.stopBits = static_cast<QSerialPort::StopBits>(this->ui->comboBox_stopbits->itemData(ui->comboBox_stopbits->currentIndex()).toInt());
    cConfig.typeOfConnection = static_cast<ConnectionTypes>(this->ui->comboBox_typeOfConnection->itemData(ui->comboBox_typeOfConnection->currentIndex()).toInt());
    sConfig.setConnectionConfig(cConfig);

    //set up accuracy

    //set up sensor parameters
    QMap<QString, int> intParams = this->scalarParameterWidget->getIntParameter();
    QMap<QString, double> doubleParams = this->scalarParameterWidget->getDoubleParameter();
    QMap<QString, QString> stringParams = this->scalarParameterWidget->getStringParameter();
    sConfig.setIntegerParameter(intParams);
    sConfig.setDoubleParameter(doubleParams);
    sConfig.setStringParameter(stringParams);

    //#############################
    //replace the old sensor config
    //#############################

    //replace config
    sensorConfigModel->replaceSensorConfig(name, sConfig);

    //select the new config in tree view
    /*if(this->ui->listView_sensorConfigs->model()->hasIndex(selection.at(0).row(), selection.at(0).column())){
        QModelIndex newSelection = sensorConfigModel->getIndex(sConfig.getName());
        if(newSelection.isValid()){
            qDebug() << "true";
            this->ui->listView_sensorConfigs->selectionModel()->select(newSelection, QItemSelectionModel::Select);
        }
    }*/

}

/*!
 * \brief SensorConfigurationDialog::sensorConfigContextMenuRequested
 * \param point
 */
void SensorConfigurationDialog::sensorConfigContextMenuRequested(const QPoint &point){

    //get and check index
    QModelIndex index = this->ui->listView_sensorConfigs->indexAt(point);
    if(!index.isValid() || index.parent().isValid()){
        return;
    }

    //get and check sensor config proxy model
    SensorConfigurationProxyModel *sensorConfigProxyModel = static_cast<SensorConfigurationProxyModel *>(this->ui->listView_sensorConfigs->model());
    if(sensorConfigProxyModel == NULL){
        return;
    }

    //get and check sensor config model
    SensorConfigurationModel *sensorConfigModel = static_cast<SensorConfigurationModel *>(sensorConfigProxyModel->sourceModel());
    if(sensorConfigModel == NULL){
        return;
    }

    //check if the selected config is a saved config
    if(!sensorConfigModel->getIsSaved(sensorConfigProxyModel->mapToSource(index))){
        return;
    }

    //set selected index
    this->ui->listView_sensorConfigs->selectionModel()->select(index, QItemSelectionModel::Select);

    QMenu *menu = new QMenu();
    menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("delete config"), this, SLOT(removeSelectedSensorConfig()));
    menu->exec(ui->listView_sensorConfigs->mapToGlobal(point));

}

/*!
 * \brief SensorConfigurationDialog::removeSelectedSensorConfig
 */
void SensorConfigurationDialog::removeSelectedSensorConfig(){

    //get and check index
    QModelIndexList selection = this->ui->listView_sensorConfigs->selectionModel()->selectedIndexes();
    if(selection.size() != 1){
        return;
    }
    QModelIndex index = selection.at(0);
    if(!index.isValid() || index.parent().isValid()){
        return;
    }

    //get and check sensor config proxy model
    SensorConfigurationProxyModel *sensorConfigProxyModel = static_cast<SensorConfigurationProxyModel *>(this->ui->listView_sensorConfigs->model());
    if(sensorConfigProxyModel == NULL){
        return;
    }

    //get and check sensor config model
    SensorConfigurationModel *sensorConfigModel = static_cast<SensorConfigurationModel *>(sensorConfigProxyModel->sourceModel());
    if(sensorConfigModel == NULL){
        return;
    }

    sensorConfigModel->removeSensorConfig(sensorConfigProxyModel->mapToSource(index));

}

/*!
 * \brief SensorConfigurationDialog::on_pushButton_add_clicked
 * Triggered whenever the user wants to add a new sensor config
 */
void SensorConfigurationDialog::on_pushButton_add_clicked(){

    SensorConfiguration sConfig;
    sConfig.setName("new config");

    //get and check sensor config proxy model
    SensorConfigurationProxyModel *sensorConfigProxyModel = static_cast<SensorConfigurationProxyModel *>(this->ui->listView_sensorConfigs->model());
    if(sensorConfigProxyModel == NULL){
        return;
    }

    //get and check sensor config model
    SensorConfigurationModel *sensorConfigModel = static_cast<SensorConfigurationModel *>(sensorConfigProxyModel->sourceModel());
    if(sensorConfigModel == NULL){
        return;
    }

    //add new sensor config
    QModelIndex index = sensorConfigModel->addSensorConfig(sConfig);

    //check index
    if(!index.isValid() || !sensorConfigModel->hasIndex(index.row(), index.column())){
        return;
    }

    //select the new item
    this->ui->listView_sensorConfigs->selectionModel()->clearSelection();
    this->ui->listView_sensorConfigs->selectionModel()->select(index, QItemSelectionModel::Select);

}

/*!
 * \brief SensorConfigurationDialog::showEvent
 * \param event
 */
void SensorConfigurationDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    event->accept();

}

/*!
 * \brief SensorConfigurationDialog::initGUI
 */
void SensorConfigurationDialog::initGUI(){

    //set initial sensor type
    this->ui->comboBox_availableSensorTypes->setCurrentText("");

    //stretch function table view
    this->ui->tableView_sensorPlugins->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->tableView_sensorPlugins->verticalHeader()->setDefaultSectionSize(22);

    //set up scalar parameter widget
    QGridLayout *extraParameterLayout = new QGridLayout();
    this->ui->widget_scalarParameter->setLayout(extraParameterLayout);
    this->scalarParameterWidget = new ScalarParameterWidget();
    QObject::connect(this->scalarParameterWidget, &ScalarParameterWidget::scalarParametersChanged, this, &SensorConfigurationDialog::scalarParameterChanged, Qt::AutoConnection);
    extraParameterLayout->addWidget(this->scalarParameterWidget);

    //set initial visibility of widgets
    this->ui->tabWidget_sensorConfig->setEnabled(false);

    //allow contect menu for sensor config model
    this->ui->listView_sensorConfigs->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this->ui->listView_sensorConfigs, &QListView::customContextMenuRequested, this, &SensorConfigurationDialog::sensorConfigContextMenuRequested, Qt::AutoConnection);

    //assign delegate to sensor configs list view
    SensorConfigurationListDelegate *delegate = new SensorConfigurationListDelegate();
    this->ui->listView_sensorConfigs->setItemDelegate(delegate);

}

/*!
 * \brief SensorConfigurationDialog::initModels
 */
void SensorConfigurationDialog::initModels(){

    //init sensor models
    this->ui->tableView_sensorPlugins->setModel(&ModelManager::getSensorTableProxyModel());
    this->ui->comboBox_availableSensorTypes->setModel(&ModelManager::getSensorTypeNamesModel());

    //init sensor config model
    this->ui->listView_sensorConfigs->setModel(&ModelManager::getSensorConfigurationProxyModel());

    //init connection config models
    this->ui->comboBox_baudrate->setModel(&ModelManager::getBaudRateTypesModel());
    this->ui->comboBox_databits->setModel(&ModelManager::getDataBitTypesModel());
    this->ui->comboBox_flowcontrol->setModel(&ModelManager::getFlowControlTypesModel());
    this->ui->comboBox_parity->setModel(&ModelManager::getParityTypesModel());
    this->ui->comboBox_stopbits->setModel(&ModelManager::getStopBitTypesModel());
    this->ui->comboBox_comPort->setModel(&ModelManager::getAvailableSerialPortsModel());
    this->ui->comboBox_ip->setModel(&ModelManager::getAvailableIpAdressesModel());

}
