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






    /*this->pluginsModel = new QSqlQueryModel();
    //neededElementsModel = new QSqlQueryModel();

    this->ui->tableView_functionPlugins->setModel(pluginsModel);

    this->ui->treeView_functions->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this->ui->treeView_functions, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(deleteFunctionContextMenu(QPoint)));

    QGridLayout *extraParameterLayout = new QGridLayout();
    this->ui->widget_customParameter->setLayout(extraParameterLayout);
    this->extraParameterWidget = new CustomParameterWidget();
    connect(this->extraParameterWidget, SIGNAL(functionConfigurationChanged()), this, SLOT(functionConfigurationChanged()));

    extraParameterLayout->addWidget(this->extraParameterWidget);*/

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

        //check if weights are supported //TODO check if weights are suppported

        this->setFunctionParameters(index);
        this->ui->tabWidget_functionParameter->setTabEnabled(0,true);
        this->ui->tabWidget_functionParameter->setCurrentIndex(0);
        this->ui->tabWidget_functionParameter->setTabEnabled(1, false);
        this->ui->tabWidget_functionParameter->setTabEnabled(2, false);

    }else if(index.isValid()){ //input element item

        this->setFunctionElements(index);

        this->ui->tabWidget_functionParameter->setTabEnabled(1, true);
        this->ui->tabWidget_functionParameter->setCurrentIndex(1);
        this->ui->tabWidget_functionParameter->setTabEnabled(2, true);
        this->ui->tabWidget_functionParameter->setTabEnabled(0,false);
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

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

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
    //this->ui->tableView_weights->setModel(&ModelManager::getFunctionWeightTableModel());
    this->ui->tableView_weights->setModel(&ModelManager::getFunctionWeightProxyModel());

    this->ui->tableView_weights->setItemDelegate(&ModelManager::getFunctionWeightDelegate());

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

/*
void FunctionPluginLoader::receivePluginsModel(QSqlQueryModel *sqlModel){
    this->ui->tableView_functionPlugins->setModel(sqlModel);
}

void FunctionPluginLoader::receiveFunctionsModel(QStandardItemModel *model){
    ui->treeView_functions->setModel(model);
    //set visibility of parameter selection menus
    this->ui->widget_element->setVisible(false);
    this->ui->widget_customParameter->setVisible(false);
}

void FunctionPluginLoader::receiveFunctionDescription(QString description){
    this->ui->textBrowser_description->setText(description);
    this->ui->textBrowser_description_2->setText("");
}

void FunctionPluginLoader::receiveAvailableElementsModel(AvailableElementsTreeViewProxyModel *model){
    ui->treeView_availableElements->setModel(model);

    //TODO delete selection model here ?
    QItemSelectionModel *mySelectionModel = new OiMultiSelectionModel(model);
    this->ui->treeView_availableElements->setSelectionModel(mySelectionModel);

    ui->treeView_availableElements->collapseAll();
}

void FunctionPluginLoader::receiveUsedElementsModel(UsedElementsModel *model){
    ui->treeView_usedElements->setModel(model);
    connect(this->ui->treeView_usedElements->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), model, SLOT(selectionChanged(QModelIndex)));
}

void FunctionPluginLoader::receiveExtraParameterForFunction(QMap<QString, int> intParameter, QMap<QString, double> doubleParameter,
                                                            QMap<QString, QStringList> stringParameter, FunctionConfiguration config){
    this->extraParameterWidget->setIntParameter(intParameter, config);
    this->extraParameterWidget->setDoubleParameter(doubleParameter, config);
    this->extraParameterWidget->setStringParameter(stringParameter, config);
}

void FunctionPluginLoader::disableFunctionInteractions(){
    this->ui->treeView_usedElements->setEnabled(false);
    this->ui->treeView_availableElements->setEnabled(false);
}

void FunctionPluginLoader::on_tableView_functionPlugins_doubleClicked(const QModelIndex &index)
{
    if(index.row() > -1){
        emit this->createFunction(index.row());
        //activate function overview tab
        this->ui->tabWidget_functionSetter->setCurrentIndex(0);
        //set description to empty text
        this->ui->textBrowser_description->setText("");
        this->ui->textBrowser_description->update();
        //reset selected function
        emit this->setSelectedFunction(-1, -1);
        //disable treeviews for needed and used elements
        this->ui->treeView_availableElements->setEnabled(false);
        this->ui->treeView_usedElements->setEnabled(false);
        //hide add and remove buttons
        this->ui->cmd_addElement->setEnabled(false);
        this->ui->cmd_removeElement->setEnabled(false);
        //reset description for selected plugin
        this->ui->textBrowser_description_2->setText("");
        //set visibility of parameter selection menus
        this->ui->widget_element->setVisible(false);
        this->ui->widget_customParameter->setVisible(false);
    }
}

void FunctionPluginLoader::on_treeView_functions_clicked(const QModelIndex &index)
{
    //disable buttons for switching used elements if a new function / used element type was selected
    if(this->openCloseHelper == false || index.row() != this->selectedFunctionIndex.row()
            || index.parent().row() != this->selectedFunctionIndex.parent().row()){
        this->openCloseHelper = true;
        this->ui->cmd_addElement->setEnabled(false);
        this->ui->cmd_removeElement->setEnabled(false);
    }
    if(index.parent().row() != -1){ //if needed element of a function was selected
        emit this->setSelectedFunction(index.parent().row(), index.row());
        this->ui->treeView_availableElements->setEnabled(true);
        this->ui->treeView_usedElements->setEnabled(true);
        this->ui->widget_element->setVisible(true);
        this->ui->widget_customParameter->setVisible(false);
    }else{ //if function itself was selected
        emit this->setSelectedFunction(index.row(), -1);
        this->ui->treeView_availableElements->setEnabled(false);
        this->ui->treeView_usedElements->setEnabled(false);
        this->ui->widget_element->setVisible(false);
        this->ui->widget_customParameter->setVisible(true);
    }
    this->selectedFunctionIndex = index;
    this->ui->textBrowser_description->update();
}

void FunctionPluginLoader::deleteFunctionContextMenu(QPoint point)
{
    this->deleteSelectedFunctionIndex = this->ui->treeView_functions->indexAt(point);
    if(this->deleteSelectedFunctionIndex.row() >= 0 && this->deleteSelectedFunctionIndex.parent().row() == -1){ //if function was clicked
        QMenu *menu = new QMenu();
        menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("delete function"), this, SLOT(deleteFunctionFromFeature(bool)));
        menu->exec(ui->treeView_functions->mapToGlobal(point));
    }
}

void FunctionPluginLoader::deleteFunctionFromFeature(bool checked){
    emit this->deleteFunction(this->deleteSelectedFunctionIndex.row());
    //set description to empty text
    this->ui->textBrowser_description->setText("");
    this->ui->textBrowser_description->update();
    //reset available elements model
    emit this->setSelectedFunction(-1, -1);
    this->ui->treeView_availableElements->setEnabled(false);
    //reset used elements model
    this->ui->treeView_usedElements->setEnabled(false);
    this->selectedFunctionIndex = QModelIndex();
    //set visibility of parameter selection menus
    this->ui->widget_element->setVisible(false);
    this->ui->widget_customParameter->setVisible(false);
}

void FunctionPluginLoader::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    //if there is an active feature
    if(OiJob::getActiveFeature() != NULL){
        this->setWindowTitle(QString("function plugin loader - active feature: %1")
                             .arg(OiJob::getActiveFeature()->getFeature()->getFeatureName()));

        event->accept();
    }

    event->ignore();
}

bool FunctionPluginLoader::validateAvailableElements(QList<FeatureTreeItem*> selectedItems, AvailableElementsTreeViewProxyModel *model){

    foreach(FeatureTreeItem *item, selectedItems){

        //if the selected item matches the needed element-type
        if(model->getNeededElement() != item->getElementType()){

            //if the selected item is a parent of the needed element-type
            if(!this->checkChildren(item, model->getNeededElement())){
                return false;
            }

        }

    }

    return true;

}

bool FunctionPluginLoader::checkChildren(FeatureTreeItem *item, Configuration::ElementTypes neededType){

    if(item->getElementType() == neededType){
        return true;
    }else{
        for(int i = 0; i < item->getChildCount(); i++){
            if(item->getChild(i)->getElementType() == neededType || checkChildren(item->getChild(i), neededType)){
                return true;
            }
        }
    }

    return false;

}

void FunctionPluginLoader::setUpSelectedElements(QList<FeatureTreeItem *> &result, FeatureTreeItem *item, Configuration::ElementTypes neededType){
    FeatureTreeItem *copy = new FeatureTreeItem(item->getDisplayValue());
    *copy = *item;
    if(item->getElementType() == neededType){
        result.append(copy);
    }else{
        for(int i = 0; i < item->getChildCount(); i++){
            setUpSelectedElements(result, item->getChild(i), neededType);
        }
    }

}

void FunctionPluginLoader::on_treeView_availableElements_clicked(const QModelIndex &index){

    AvailableElementsTreeViewProxyModel *model = dynamic_cast<AvailableElementsTreeViewProxyModel*>(this->ui->treeView_availableElements->model());
    if(model != NULL){

        //get the selected items
        QList<FeatureTreeItem*> selectedItems;
        QModelIndexList selectedIndexes = this->ui->treeView_availableElements->selectionModel()->selectedIndexes();
        foreach(QModelIndex index, selectedIndexes){
            selectedItems.append(model->getSelectedItem(index));
        }

        //validate the items if they match the needed element
        if(this->validateAvailableElements(selectedItems, model)){
            this->selectedAvailableElements = selectedItems;
            this->ui->cmd_addElement->setEnabled(true);
        }else{
            this->selectedAvailableElements.clear();
            this->ui->cmd_addElement->setEnabled(false);
        }

    }

}

void FunctionPluginLoader::on_treeView_availableElements_entered(const QModelIndex &index){

    AvailableElementsTreeViewProxyModel *model = dynamic_cast<AvailableElementsTreeViewProxyModel*>(this->ui->treeView_availableElements->model());
    if(model != NULL){

        //get the selected items
        QList<FeatureTreeItem*> selectedItems;
        QModelIndexList selectedIndexes = this->ui->treeView_availableElements->selectionModel()->selectedIndexes();
        foreach(QModelIndex index, selectedIndexes){
            selectedItems.append(model->getSelectedItem(index));
        }

        //validate the items if they match the needed element
        if(this->validateAvailableElements(selectedItems, model)){
            this->selectedAvailableElements = selectedItems;
            this->ui->cmd_addElement->setEnabled(true);
        }else{
            this->selectedAvailableElements.clear();
            this->ui->cmd_addElement->setEnabled(false);
        }

    }

}

void FunctionPluginLoader::on_cmd_addElement_clicked(){

    //get the model to be able to check against the needed element
    AvailableElementsTreeViewProxyModel *model = dynamic_cast<AvailableElementsTreeViewProxyModel*>(this->ui->treeView_availableElements->model());
    if(model != NULL){

        if(this->selectedFunctionIndex.parent().isValid()){

            QList<FeatureTreeItem *> selectedElements; //list with selected elements to use

            //fill the list of selected elements
            foreach(FeatureTreeItem *item, this->selectedAvailableElements){
                this->setUpSelectedElements(selectedElements, item, model->getNeededElement());
            }

            //add each of the selected elements to the function
            foreach(FeatureTreeItem *item, selectedElements){
                emit this->addElement(item, this->selectedFunctionIndex.parent().row(), this->selectedFunctionIndex.row());
            }

            //enable used elements treeview
            this->ui->treeView_usedElements->setEnabled(true);

        }

    }

}

void FunctionPluginLoader::closeEvent(QCloseEvent *event){
    //recalculate the active feature so the changes become effective
    emit this->recalcActiveFeature();
    //set openCloseHelper to false to know when dialog is opened again
    this->openCloseHelper = false;
    //disable buttons for adding and removing used elements
    this->ui->cmd_addElement->setEnabled(false);
    this->ui->cmd_removeElement->setEnabled(false);
    event->accept();
}

void FunctionPluginLoader::on_tableView_functionPlugins_clicked(const QModelIndex &index){
    QModelIndex modelIdx = this->ui->tableView_functionPlugins->model()->index(index.row(), 1);
    QString description = this->ui->tableView_functionPlugins->model()->data(modelIdx).toString();
    this->ui->textBrowser_description_2->setText(description);
}

void FunctionPluginLoader::on_cmd_ok_clicked(){
    this->close();
}

void FunctionPluginLoader::on_cmd_removeElement_clicked(){

    UsedElementsModel *model = static_cast<UsedElementsModel*>(this->ui->treeView_usedElements->model());

    if(model != NULL && this->selectedFunctionIndex.parent().isValid()){

        foreach(QModelIndex index, this->ui->treeView_usedElements->selectionModel()->selectedIndexes()){
            emit this->removeElement(model->getSelectedItem(index), this->selectedFunctionIndex.parent().row(),
                                     this->selectedFunctionIndex.row());
        }
        this->ui->cmd_removeElement->setEnabled(false);

    }

}

void FunctionPluginLoader::on_treeView_usedElements_clicked(const QModelIndex &index){
    this->ui->cmd_removeElement->setEnabled(true);
}

void FunctionPluginLoader::on_treeView_usedElements_activated(const QModelIndex &index){
    this->ui->cmd_removeElement->setEnabled(true);
}

void FunctionPluginLoader::functionConfigurationChanged(){
    QModelIndex selectedIndex = this->ui->treeView_functions->currentIndex();
    if(selectedIndex.parent().row() == -1){ //if a function is selected
        emit this->sendFunctionConfiguration(selectedIndex.row(), this->extraParameterWidget->getFunctionConfiguration());
    }
}
*/
