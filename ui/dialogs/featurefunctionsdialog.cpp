#include "featurefunctionsdialog.h"
#include "ui_featurefunctionsdialog.h"

/*!
 * \brief FeatureFunctionsDialog::FeatureFunctionsDialog
 * \param parent
 */
FeatureFunctionsDialog::FeatureFunctionsDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::FeatureFunctionsDialog)
{
    ui->setupUi(this);

    //init GUI elements
    this->initGUI();

    //init models
    this->initModels();

}

/*!
 * \brief FeatureFunctionsDialog::~FeatureFunctionsDialog
 */
FeatureFunctionsDialog::~FeatureFunctionsDialog()
{
    delete ui;
}

/*!
 * \brief FeatureFunctionsDialog::scalarParameterChanged
 * Is called whenever the user has edited the scalar parameters of a function
 */
void FeatureFunctionsDialog::scalarParameterChanged(){

    //get and check selected index
    QModelIndexList selection = this->ui->treeView_functions->selectionModel()->selectedIndexes();
    if(selection.size() != 1){
        return;
    }
    QModelIndex index = selection.at(0);
    if(!index.isValid() || index.parent().isValid()){
        return;
    }

    //get and check active feature function model
    ActiveFeatureFunctionsModel *model = static_cast<ActiveFeatureFunctionsModel *>(this->ui->treeView_functions->model());
    if(model == NULL){
        return;
    }

    //set scalar input parameters for the selected function
    model->setScalarInputParameters(index, this->scalarParameterWidget->getIntParameter(),
                                    this->scalarParameterWidget->getDoubleParameter(),
                                    this->scalarParameterWidget->getStringParameter());

}

/*!
 * \brief FeatureFunctionsDialog::on_tableView_functionPlugins_clicked
 * Shows the description of a function plugin whenever the user selectes a function by single click
 * \param index
 */
void FeatureFunctionsDialog::on_tableView_functionPlugins_clicked(const QModelIndex &index){

    //get and check model
    FunctionTableProxyModel *model = static_cast<FunctionTableProxyModel *>(this->ui->tableView_functionPlugins->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FunctionTableModel *sourceModel = static_cast<FunctionTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //set active feature
    this->ui->textBrowser_description_2->setText(sourceModel->getFunctionDescription(model->mapToSource(index)));

}

/*!
 * \brief FeatureFunctionsDialog::on_tableView_functionPlugins_doubleClicked
 * Adds the selected function to the active features's function list whenever the user double clicks a function
 * \param index
 */
void FeatureFunctionsDialog::on_tableView_functionPlugins_doubleClicked(const QModelIndex &index){

    //get and check model
    FunctionTableProxyModel *model = static_cast<FunctionTableProxyModel *>(this->ui->tableView_functionPlugins->model());
    if(model == NULL){
        return;
    }

    //add the selected function
    model->addFunction(index);

    //set focus to function configuration tab
    this->ui->tabWidget_functionParameter->setCurrentIndex(0);
    this->ui->tabWidget_functionParameter->setTabEnabled(1,false);
    this->ui->tabWidget_functionParameter->setTabEnabled(2,false);
    this->ui->tabWidget_functionSetter->setCurrentIndex(0);

}

/*!
 * \brief FeatureFunctionsDialog::on_treeView_functions_clicked
 * Triggered whenever the user selects a function or an input element in the function tree view
 * \param index
 */
void FeatureFunctionsDialog::on_treeView_functions_clicked(const QModelIndex &index){

    //check if a function item has been selected
    if(index.isValid() && !index.parent().isValid()){ //function item

        emit this->setFunctionPos(index.row());

        this->setFunctionParameters(index);
        this->ui->tabWidget_functionParameter->setTabEnabled(0,true);
        this->ui->tabWidget_functionParameter->setCurrentIndex(0);
        this->ui->tabWidget_functionParameter->setTabEnabled(1, false);
        this->ui->tabWidget_functionParameter->setTabEnabled(2, false);

    }else if(index.isValid()){ //input element item

        this->setFunctionElements(index);

        this->ui->tabWidget_functionParameter->setTabEnabled(1, true);
        this->ui->tabWidget_functionParameter->setCurrentIndex(1);
        //this->ui->tabWidget_functionParameter->setTabEnabled(2, true);
        this->ui->tabWidget_functionParameter->setTabEnabled(0,false);

        //if the function supports weighted inpul elements, enable the tabwidget to set the weights
        if(this->checkSupportsWeights()){
            this->ui->tabWidget_functionParameter->setTabEnabled(2,true);
        }else{
            this->ui->tabWidget_functionParameter->setTabEnabled(2,false);
        }
    }

}

/*!
 * \brief FeatureFunctionsDialog::on_treeView_availableElements_clicked
 * Triggered whenever the user has selected one or more available elements
 * \param index
 */
void FeatureFunctionsDialog::on_treeView_availableElements_clicked(const QModelIndex &index){

    //get and check available elements proxy model
    AvailableElementsTreeViewProxyModel *model = static_cast<AvailableElementsTreeViewProxyModel *>(this->ui->treeView_availableElements->model());
    if(model == NULL){
        return;
    }

    //get selected indexes
    QModelIndexList selection = this->ui->treeView_availableElements->selectionModel()->selectedIndexes();
    if(selection.size() == 0){
        this->ui->cmd_addElement->setEnabled(false);
        return;
    }

    //validate selection
    if(model->validateSelection(selection)){
        this->ui->cmd_addElement->setEnabled(true);
        return;
    }

    this->ui->cmd_addElement->setEnabled(false);

}

/*!
 * \brief FeatureFunctionsDialog::on_treeView_availableElements_entered
 * Triggered whenever the user has selected one or more available elements
 * \param index
 */
void FeatureFunctionsDialog::on_treeView_availableElements_entered(const QModelIndex &index){

    //get and check available elements proxy model
    AvailableElementsTreeViewProxyModel *model = static_cast<AvailableElementsTreeViewProxyModel *>(this->ui->treeView_availableElements->model());
    if(model == NULL){
        return;
    }

    //get selected indexes
    QModelIndexList selection = this->ui->treeView_availableElements->selectionModel()->selectedIndexes();
    if(selection.size() == 0){
        this->ui->cmd_addElement->setEnabled(false);
        return;
    }

    //validate selection
    if(model->validateSelection(selection)){
        this->ui->cmd_addElement->setEnabled(true);
        return;
    }

    this->ui->cmd_addElement->setEnabled(false);

}

void FeatureFunctionsDialog::on_treeView_availableElements_doubleClicked(const QModelIndex &index) {
    if(this->ui->cmd_addElement->isEnabled()) {
        this->on_cmd_addElement_clicked();
    }
}

/*!
 * \brief FeatureFunctionsDialog::on_treeView_usedElements_clicked
 * Triggered whenever the user has selected one or more used elements
 * \param index
 */
void FeatureFunctionsDialog::on_treeView_usedElements_clicked(const QModelIndex &index){

    //get and check used elements proxy model
    UsedElementsModel *model = static_cast<UsedElementsModel *>(this->ui->treeView_usedElements->model());
    if(model == NULL){
        return;
    }

    //get selected indexes
    QModelIndexList selection = this->ui->treeView_usedElements->selectionModel()->selectedIndexes();
    if(selection.size() == 0){
        this->ui->cmd_removeElement->setEnabled(false);
        return;
    }

    this->ui->cmd_removeElement->setEnabled(true);

}

/*!
 * \brief FeatureFunctionsDialog::on_treeView_usedElements_entered
 * Triggered whenever the user has selected one or more used elements
 * \param index
 */
void FeatureFunctionsDialog::on_treeView_usedElements_entered(const QModelIndex &index){

    //get and check used elements proxy model
    UsedElementsModel *model = static_cast<UsedElementsModel *>(this->ui->treeView_usedElements->model());
    if(model == NULL){
        return;
    }

    //get selected indexes
    QModelIndexList selection = this->ui->treeView_usedElements->selectionModel()->selectedIndexes();
    if(selection.size() == 0){
        this->ui->cmd_removeElement->setEnabled(false);
        return;
    }

    this->ui->cmd_removeElement->setEnabled(true);

}

void FeatureFunctionsDialog::on_treeView_usedElements_doubleClicked(const QModelIndex &index) {
    if(this->ui->cmd_removeElement->isEnabled()) {
        this->on_cmd_removeElement_clicked();
    }
}

/*!
 * \brief FeatureFunctionsDialog::on_cmd_addElement_clicked
 * Add selected elements
 */
void FeatureFunctionsDialog::on_cmd_addElement_clicked(){

    //get and check available elements proxy model
    AvailableElementsTreeViewProxyModel *model = static_cast<AvailableElementsTreeViewProxyModel *>(this->ui->treeView_availableElements->model());
    if(model == NULL){
        return;
    }

    //get selected indexes
    QModelIndexList selection = this->ui->treeView_availableElements->selectionModel()->selectedIndexes();
    if(selection.size() == 0){
        return;
    }

    //clear selection
    this->ui->treeView_availableElements->selectionModel()->clearSelection();
    this->ui->cmd_addElement->setEnabled(false);

    //add elements
    model->addInputElements(selection);

    ModelManager::getFunctionWeightTableModel().updateModel();

}

/*!
 * \brief FeatureFunctionsDialog::on_cmd_removeElement_clicked
 * Remove selected elements
 */
void FeatureFunctionsDialog::on_cmd_removeElement_clicked(){

    //get and check used elements model
    UsedElementsModel *model = static_cast<UsedElementsModel *>(this->ui->treeView_usedElements->model());
    if(model == NULL){
        return;
    }

    //get selected indexes
    QModelIndexList selection = this->ui->treeView_usedElements->selectionModel()->selectedIndexes();
    if(selection.size() == 0){
        return;
    }

    //clear selection
    this->ui->treeView_usedElements->selectionModel()->clearSelection();
    this->ui->cmd_removeElement->setEnabled(false);

    //add elements
    model->removeUsedElements(selection);

    ModelManager::getFunctionWeightTableModel().updateModel();

}

/*!
 * \brief FeatureFunctionsDialog::activeFeatureFunctionsContextMenuRequested
 * \param point
 */
void FeatureFunctionsDialog::activeFeatureFunctionsContextMenuRequested(const QPoint &point){

    //get and check index
    QModelIndex index = this->ui->treeView_functions->indexAt(point);
    if(!index.isValid() || index.parent().isValid()){
        return;
    }

    //get and check active feature function model
    ActiveFeatureFunctionsModel *model = static_cast<ActiveFeatureFunctionsModel *>(this->ui->treeView_functions->model());
    if(model == NULL){
        return;
    }

    //check wether the selected function could be removed
    if(!model->canRemoveFunction(index)){
        return;
    }

    //set selected index
    this->ui->treeView_functions->selectionModel()->select(index, QItemSelectionModel::Select);

    QMenu *menu = new QMenu();
    menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("delete function"), this, SLOT(removeSelectedFunction()));
    menu->exec(ui->treeView_functions->mapToGlobal(point));

}

/*!
 * \brief FeatureFunctionsDialog::removeSelectedFunction
 */
void FeatureFunctionsDialog::removeSelectedFunction(){

    //get and check index
    QModelIndexList selection = this->ui->treeView_functions->selectionModel()->selectedIndexes();
    if(selection.size() != 1){
        return;
    }
    QModelIndex index =selection.at(0);
    if(!index.isValid() || index.parent().isValid()){
        return;
    }

    //get and check active feature function model
    ActiveFeatureFunctionsModel *model = static_cast<ActiveFeatureFunctionsModel *>(this->ui->treeView_functions->model());
    if(model == NULL){
        return;
    }

    model->removeFunction(index);

}

/*!
 * \brief FeatureFunctionsDialog::on_cmd_ok_clicked
 */
void FeatureFunctionsDialog::on_cmd_ok_clicked(){
    emit this->recalculateActiveFeature();
    this->close();
}

/*!
 * \brief FeatureFunctionsDialog::showEvent
 * \param event
 */
void FeatureFunctionsDialog::showEvent(QShowEvent *event){

    //set initial visibility of widgets to false
    this->ui->widget_element->setVisible(false);
    this->ui->widget_customParameter->setVisible(false);
    this->ui->widget_filter->setVisible(false);

    this->ui->tabWidget_functionParameter->setCurrentIndex(0);
    this->ui->tabWidget_functionParameter->setTabEnabled(1,false);
    this->ui->tabWidget_functionParameter->setTabEnabled(2,false);

    //clear selection
    this->ui->tableView_functionPlugins->selectionModel()->clearSelection();
    this->ui->textBrowser_description_2->setText("");

    QObject::connect(this, &FeatureFunctionsDialog::setFunctionPos, &ModelManager::getFunctionWeightTableModel(), &FunctionWeightsTableModel::setFunctionPosition, Qt::AutoConnection);

    event->accept();

}

/*!
 * \brief FeatureFunctionsDialog::initGUI
 */
void FeatureFunctionsDialog::initGUI(){

    //stretch function table view
    this->ui->tableView_functionPlugins->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->tableView_functionPlugins->verticalHeader()->setDefaultSectionSize(22);

    //set up scalar parameter widget
    QGridLayout *extraParameterLayout = new QGridLayout();
    this->ui->widget_customParameter->setLayout(extraParameterLayout);
    this->scalarParameterWidget = new ScalarParameterWidget();
    QObject::connect(this->scalarParameterWidget, &ScalarParameterWidget::scalarParametersChanged, this, &FeatureFunctionsDialog::scalarParameterChanged, Qt::AutoConnection);
    extraParameterLayout->addWidget(this->scalarParameterWidget);

    //allow contect menu for active feature functions model
    this->ui->treeView_functions->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this->ui->treeView_functions, &QTreeView::customContextMenuRequested, this, &FeatureFunctionsDialog::activeFeatureFunctionsContextMenuRequested, Qt::AutoConnection);

    //connect weights tableview
    QObject::connect(this->ui->tableView_weights->horizontalHeader(), &QHeaderView::sectionDoubleClicked, this, &FeatureFunctionsDialog::resizeTableView, Qt::AutoConnection);

}

/*!
 * \brief FeatureFunctionsDialog::initModels
 */
void FeatureFunctionsDialog::initModels(){

    //init function model
    this->ui->tableView_functionPlugins->setModel(&ModelManager::getFunctionTableProxyModel());

    //init active feature function tree view model
    this->ui->treeView_functions->setModel(&ModelManager::getActiveFeatureFunctionsModel());

    //init used and available elements models
    this->ui->treeView_availableElements->setModel(&ModelManager::getAvailableElementsTreeViewProxyModel());
    this->ui->treeView_usedElements->setModel(&ModelManager::getUsedElementsModel());

    //assign multi selection model to available elements proxy model
    QItemSelectionModel *selectionModel = new MultiSelectionModel(&ModelManager::getAvailableElementsTreeViewProxyModel());
    this->ui->treeView_availableElements->setSelectionModel(selectionModel);

    //set weighted elements model
    this->ui->tableView_weights->setModel(&ModelManager::getFunctionWeightProxyModel());
    FunctionWeightDelegate *functionweightDelegate = new FunctionWeightDelegate();
    this->ui->tableView_weights->setItemDelegate(functionweightDelegate);

    this->ui->tableView_weights->setSortingEnabled(true);

}

/*!
 * \brief FeatureFunctionsDialog::setFunctionParameters
 */
void FeatureFunctionsDialog::setFunctionParameters(const QModelIndex &index)
{
    //get and check active feature function model
    ActiveFeatureFunctionsModel *actFeatModel = static_cast<ActiveFeatureFunctionsModel *>(this->ui->treeView_functions->model());
    if(actFeatModel == NULL){
        return;
    }

    //set function description
    this->ui->textBrowser_description->setText(actFeatModel->getFunctionDescription(index));

    //reset scalar parameter widget and add scalar function parameters
    this->scalarParameterWidget->clearAll();
    this->scalarParameterWidget->setIntParameter(actFeatModel->getIntegerParameter(index));
    this->scalarParameterWidget->setDoubleParameter(actFeatModel->getDoubleParameter(index));
    this->scalarParameterWidget->setStringParameter(actFeatModel->getStringParameter(index), actFeatModel->getStringParameterSelection(index));

    //set visibility of widgets
    this->ui->widget_element->setVisible(true);
    this->ui->widget_customParameter->setVisible(true);
    this->ui->widget_filter->setVisible(true);
}

/*!
 * \brief FeatureFunctionsDialog::setFunctionElements
 */
void FeatureFunctionsDialog::setFunctionElements(const QModelIndex &index)
{
    //get and check active feature function model
    ActiveFeatureFunctionsModel *model = static_cast<ActiveFeatureFunctionsModel *>(this->ui->treeView_functions->model());
    if(model == NULL){
        return;
    }

    //update function position and element index
    AvailableElementsTreeViewProxyModel *availableElementsModel = static_cast<AvailableElementsTreeViewProxyModel *>(this->ui->treeView_availableElements->model());
    UsedElementsModel *usedElementsModel = static_cast<UsedElementsModel *>(this->ui->treeView_usedElements->model());
    if(availableElementsModel != NULL || usedElementsModel != NULL){
        availableElementsModel->setSelectedFunctionPosition(index.parent().row(), index.row());
        usedElementsModel->setSelectedFunctionPosition(index.parent().row(), index.row());
    }

    //set input element dexcription
    this->ui->textBrowser_description->setText(model->getInputElementDescription(index));

    //set visibility of widgets
    this->ui->widget_element->setVisible(true);
    this->ui->widget_customParameter->setVisible(false);
    this->ui->widget_filter->setVisible(true);
}

/*!
 * \brief FeatureFunctionsDialog::resizeTableView
 */
void FeatureFunctionsDialog::resizeTableView()
{
    this->ui->tableView_weights->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

/*!
 * \brief FeatureFunctionsDialog::checkSupportsWeights
 * \return
 */
bool FeatureFunctionsDialog::checkSupportsWeights()
{
    //get and check index
    QModelIndexList selection = this->ui->treeView_functions->selectionModel()->selectedIndexes();
    if(selection.size() != 1){
        return false;
    }
    QModelIndex index =selection.at(0);
    if(!index.isValid() || !index.parent().isValid()){
        return false;
    }

    //get and check active feature function model
    ActiveFeatureFunctionsModel *model = static_cast<ActiveFeatureFunctionsModel *>(this->ui->treeView_functions->model());
    if(model == NULL){
        return false;
    }

    return model->getSupportsWeights(index);
}

void FeatureFunctionsDialog::setFeatureNameCompleter(QCompleter *completer) {
   this->ui->lineEdit_searchByFeatureName->setCompleter(completer);
}

void FeatureFunctionsDialog::setGroupNames(QStringList groupNames) {
    this->ui->comboBox_groups->clear();
    this->ui->comboBox_groups->addItem("All Groups");
    this->ui->comboBox_groups->addItems(groupNames);
}

void FeatureFunctionsDialog::on_comboBox_features_currentIndexChanged(int index)
{
    AvailableElementsTreeViewProxyModel *model = static_cast<AvailableElementsTreeViewProxyModel *>(this->ui->treeView_availableElements->model());
    if(model == NULL){
        return;
    }
    model->filterByActualNominal((ActualNominalFilter)index);
}

void FeatureFunctionsDialog::on_comboBox_groups_currentIndexChanged(int index)
{
    AvailableElementsTreeViewProxyModel *model = static_cast<AvailableElementsTreeViewProxyModel *>(this->ui->treeView_availableElements->model());
    if(model == NULL){
        return;
    }
    if(index == 0) {
        model->filterByGroup("");
    } else {
        model->filterByGroup(this->ui->comboBox_groups->itemText(index));
    }
}

void FeatureFunctionsDialog::on_lineEdit_searchByFeatureName_textChanged(const QString &text)
{
    AvailableElementsTreeViewProxyModel *model = static_cast<AvailableElementsTreeViewProxyModel *>(this->ui->treeView_availableElements->model());
    if(model == NULL){
        return;
    }

    model->filterByFeatureName(text);
}
