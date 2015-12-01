#include "stationpropertiesdialog.h"
#include "ui_stationpropertiesdialog.h"

/*!
 * \brief StationPropertiesDialog::StationPropertiesDialog
 * \param parent
 */
StationPropertiesDialog::StationPropertiesDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::StationPropertiesDialog)
{
    ui->setupUi(this);

    //init GUI elements
    this->initGUI();

    //init models
    this->initModels();

}

/*!
 * \brief StationPropertiesDialog::~StationPropertiesDialog
 */
StationPropertiesDialog::~StationPropertiesDialog(){

}

/*!
 * \brief StationPropertiesDialog::getSensorConfiguration
 * \return
 */
const SensorConfiguration &StationPropertiesDialog::getSensorConfiguration() const{
    return this->sConfig;
}

/*!
 * \brief StationPropertiesDialog::setSensorConfiguration
 * \param sConfig
 */
void StationPropertiesDialog::setSensorConfiguration(const SensorConfiguration &sConfig){

    //set sensor config
    this->sConfig = sConfig;

    //pass config to models
    if(!this->accuracyModel.isNull()){
        this->accuracyModel->setSensorConfiguration(sConfig);
    }
    if(!this->sensorParametersModel.isNull()){
        this->sensorParametersModel->setSensorConfiguration(sConfig);
    }

}

/*!
 * \brief StationPropertiesDialog::getIsActiveStation
 * \return
 */
bool StationPropertiesDialog::getIsActiveStation() const{
    return this->isActiveStation;
}

/*!
 * \brief StationPropertiesDialog::setIsActiveStation
 * \param isActiveStation
 */
void StationPropertiesDialog::setIsActiveStation(bool isActiveStation){

    //set active station state
    this->isActiveStation = isActiveStation;

    //set models read only state
    if(!this->accuracyModel.isNull()){
        this->accuracyModel->setIsReadOnly(!this->isActiveStation);
    }
    if(!this->sensorParametersModel.isNull()){
        this->sensorParametersModel->setIsReadOnly(!this->isActiveStation);
    }

}

/*!
 * \brief StationPropertiesDialog::on_buttonBox_clicked
 * \param button
 */
void StationPropertiesDialog::on_buttonBox_clicked(QAbstractButton *button){

    QDialogButtonBox::StandardButton stdBtn = this->ui->buttonBox->standardButton(button);
    switch(stdBtn){
    case QDialogButtonBox::Ok:
        emit this->sensorConfigurationChanged(this->sConfig);
        this->close();
        break;
    case QDialogButtonBox::Cancel:
        this->close();
        break;
    }

}

/*!
 * \brief StationPropertiesDialog::on_pushButton_sensorConfig1_clicked
 */
void StationPropertiesDialog::on_pushButton_sensorConfig1_clicked(){
    emit this->openSensorConfigurationDialog();
    this->close();
}

/*!
 * \brief StationPropertiesDialog::keyPressEvent
 * \param e
 */
void StationPropertiesDialog::keyPressEvent(QKeyEvent *e){

    //check triggered keys
    switch(e->key()){
    case Qt::Key_C: //copy to clipboard

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
 * \brief StationPropertiesDialog::updateSensorAccuracy
 * \param sConfig
 */
void StationPropertiesDialog::updateSensorAccuracy(const SensorConfiguration &sConfig){

    //check sensor config
    if(!sConfig.getIsValid()){
        return;
    }

    //update accuracy
    this->sConfig.setAccuracy(sConfig.getAccuracy());

}

/*!
 * \brief StationPropertiesDialog::updateSensorParameters
 * \param sConfig
 */
void StationPropertiesDialog::updateSensorParameters(const SensorConfiguration &sConfig){

    //check sensor config
    if(!sConfig.getIsValid()){
        return;
    }

    //update sensor parameters
    this->sConfig.setStringParameter(sConfig.getStringParameter());
    this->sConfig.setDoubleParameter(sConfig.getDoubleParameter());
    this->sConfig.setIntegerParameter(sConfig.getIntegerParameter());

}

/*!
 * \brief StationPropertiesDialog::showEvent
 * \param event
 */
void StationPropertiesDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    //update sensor config information
    this->initGUI();

    event->accept();

}

/*!
 * \brief StationPropertiesDialog::initGUI
 */
void StationPropertiesDialog::initGUI(){

    //set general sensor config information
    this->ui->lineEdit_sensorConfig1->setText(this->sConfig.getName());

    //set connection information
    this->ui->lineEdit_connectionType->setText(getConnectionTypeName(this->sConfig.getConnectionConfig().typeOfConnection));
    if(!this->sConfig.getIsValid()){

        //set visible states
        this->ui->lineEdit_baudrate->setVisible(false);
        this->ui->label_baudrate->setVisible(false);
        this->ui->lineEdit_comPort->setVisible(false);
        this->ui->label_comPort->setVisible(false);
        this->ui->lineEdit_databits->setVisible(false);
        this->ui->label_databits->setVisible(false);
        this->ui->lineEdit_flowcontrol->setVisible(false);
        this->ui->label_flowcontrol->setVisible(false);
        this->ui->lineEdit_parity->setVisible(false);
        this->ui->label_parity->setVisible(false);
        this->ui->lineEdit_stopbits->setVisible(false);
        this->ui->label_stopbits->setVisible(false);
        this->ui->lineEdit_ip->setVisible(false);
        this->ui->label_ip->setVisible(false);
        this->ui->lineEdit_port->setVisible(false);
        this->ui->label_port->setVisible(false);

    }else if(this->sConfig.getConnectionConfig().typeOfConnection == eNetworkConnection){

        //set visible states
        this->ui->lineEdit_baudrate->setVisible(false);
        this->ui->label_baudrate->setVisible(false);
        this->ui->lineEdit_comPort->setVisible(false);
        this->ui->label_comPort->setVisible(false);
        this->ui->lineEdit_databits->setVisible(false);
        this->ui->label_databits->setVisible(false);
        this->ui->lineEdit_flowcontrol->setVisible(false);
        this->ui->label_flowcontrol->setVisible(false);
        this->ui->lineEdit_parity->setVisible(false);
        this->ui->label_parity->setVisible(false);
        this->ui->lineEdit_stopbits->setVisible(false);
        this->ui->label_stopbits->setVisible(false);
        this->ui->lineEdit_ip->setVisible(true);
        this->ui->label_ip->setVisible(true);
        this->ui->lineEdit_port->setVisible(true);
        this->ui->label_port->setVisible(true);

        //set connection information
        this->ui->lineEdit_ip->setText(this->sConfig.getConnectionConfig().ip);
        this->ui->lineEdit_port->setText(this->sConfig.getConnectionConfig().port);

    }else if(this->sConfig.getConnectionConfig().typeOfConnection == eSerialConnection){

        //set visible states
        this->ui->lineEdit_baudrate->setVisible(true);
        this->ui->label_baudrate->setVisible(true);
        this->ui->lineEdit_comPort->setVisible(true);
        this->ui->label_comPort->setVisible(true);
        this->ui->lineEdit_databits->setVisible(true);
        this->ui->label_databits->setVisible(true);
        this->ui->lineEdit_flowcontrol->setVisible(true);
        this->ui->label_flowcontrol->setVisible(true);
        this->ui->lineEdit_parity->setVisible(true);
        this->ui->label_parity->setVisible(true);
        this->ui->lineEdit_stopbits->setVisible(true);
        this->ui->label_stopbits->setVisible(true);
        this->ui->lineEdit_ip->setVisible(false);
        this->ui->label_ip->setVisible(false);
        this->ui->lineEdit_port->setVisible(false);
        this->ui->label_port->setVisible(false);

        //set connection information
        //this->ui->lineEdit_baudrate->setText(this->sConfig.getConnectionConfig().baudRate);
        this->ui->lineEdit_comPort->setText(this->sConfig.getConnectionConfig().comPort);
        //this->ui->lineEdit_databits->setText(this->sConfig.getConnectionConfig().dataBits);
        //this->ui->lineEdit_flowcontrol->setText(this->sConfig.getConnectionConfig().flowControl);
        //this->ui->lineEdit_parity->setText(this->sConfig.getConnectionConfig().parity);
        //this->ui->lineEdit_stopbits->setText(this->sConfig.getConnectionConfig().stopBits);

    }

    //enable or disable the button to select another sensor config
    if(this->isActiveStation){
        this->ui->pushButton_sensorConfig1->setEnabled(true);
    }else{
        this->ui->pushButton_sensorConfig1->setEnabled(false);
    }

    //init table views
    this->ui->tableView_accuracy->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tableView_accuracy->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->tableView_accuracy->setEditTriggers(QAbstractItemView::DoubleClicked);
    this->ui->tableView_sensorParameters->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tableView_sensorParameters->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->tableView_sensorParameters->setEditTriggers(QAbstractItemView::DoubleClicked);

}

/*!
 * \brief StationPropertiesDialog::initModels
 */
void StationPropertiesDialog::initModels(){

    //get models from model manager
    this->accuracyModel = ModelManager::getSensorAccuracyModel(this);
    this->sensorParametersModel = ModelManager::getSensorParametersModel(this);

    //assign models and delegates
    if(!this->accuracyModel.isNull()){
        this->ui->tableView_accuracy->setModel(this->accuracyModel);
        SensorAccuracyDelegate *delegate = new SensorAccuracyDelegate(this);
        this->ui->tableView_accuracy->setItemDelegate(delegate);
    }
    if(!this->sensorParametersModel.isNull()){
        this->ui->tableView_sensorParameters->setModel(this->sensorParametersModel);
        SensorParametersDelegate *delegate = new SensorParametersDelegate(this);
        this->ui->tableView_sensorParameters->setItemDelegate(delegate);
    }

    //connect models
    if(!this->accuracyModel.isNull()){
        QObject::connect(this->accuracyModel, &SensorAccuracyModel::sensorConfigurationChanged, this, &StationPropertiesDialog::updateSensorAccuracy, Qt::AutoConnection);
    }
    if(!this->sensorParametersModel.isNull()){
        QObject::connect(this->sensorParametersModel, &SensorParametersModel::sensorConfigurationChanged, this, &StationPropertiesDialog::updateSensorParameters, Qt::AutoConnection);
    }

}

/*!
 * \brief StationPropertiesDialog::copyToClipboard
 */
void StationPropertiesDialog::copyToClipboard(){

    //init variables
    QAbstractItemModel *model = NULL;
    QItemSelectionModel *selectionModel = NULL;
    QModelIndexList selection;

    //get selection
    if(this->ui->tabWidget_stationProperties->currentWidget() == this->ui->tab_accuracy){ //accuracy table view
        model = this->ui->tableView_accuracy->model();
        selectionModel = this->ui->tableView_accuracy->selectionModel();
        selection = selectionModel->selectedIndexes();
    }else if(this->ui->tabWidget_stationProperties->currentWidget() == this->ui->tab_sensorParameters){ //sensor parameters table view
        model = this->ui->tableView_sensorParameters->model();
        selectionModel = this->ui->tableView_sensorParameters->selectionModel();
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

    //set values to clipboard, so you can paste them elsewhere
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->clear();
    clipboard->setText(copy_table);

}

/*!
 * \brief StationPropertiesDialog::pasteFromClipboard
 */
void StationPropertiesDialog::pasteFromClipboard(){

    //init variables
    QAbstractItemModel *model = NULL;
    QItemSelectionModel *selectionModel = NULL;
    QModelIndexList selection;

    //get selection
    if(this->ui->tabWidget_stationProperties->currentWidget() == this->ui->tab_accuracy){ //accuracy table view
        model = this->ui->tableView_accuracy->model();
        selectionModel = this->ui->tableView_accuracy->selectionModel();
        selection = selectionModel->selectedIndexes();
    }else if(this->ui->tabWidget_stationProperties->currentWidget() == this->ui->tab_sensorParameters){ //sensor parameters table view
        model = this->ui->tableView_sensorParameters->model();
        selectionModel = this->ui->tableView_sensorParameters->selectionModel();
        selection = selectionModel->selectedIndexes();
    }

    //check and sort selection
    if(selection.size() <= 0){
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
            if(model->flags(index) & Qt::ItemIsEditable){
                model->setData(index, rows.at(0));
            }
        }
    }else{
        int i = 0;
        foreach(const QModelIndex &index, selection){
            if(model->flags(index) & Qt::ItemIsEditable){
                model->setData(index, rows.at(i));
            }
            i++;
        }
    }

}
