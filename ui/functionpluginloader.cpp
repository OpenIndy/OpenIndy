#include "functionpluginloader.h"
#include "ui_functionpluginloader.h"

/*!
 * \brief FunctionPluginLoader::FunctionPluginLoader
 * \param parent
 */
FunctionPluginLoader::FunctionPluginLoader(QWidget *parent) :
    QDialog(parent), ui(new Ui::FunctionPluginLoader), selectedUsedElement(NULL), openCloseHelper(false)
{
    ui->setupUi(this);

    this->pluginsModel = new QSqlQueryModel();
    //neededElementsModel = new QSqlQueryModel();

    this->ui->tableView_functionPlugins->setModel(pluginsModel);

    this->ui->treeView_functions->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this->ui->treeView_functions, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(deleteFunctionContextMenu(QPoint)));

    QGridLayout *extraParameterLayout = new QGridLayout();
    this->ui->widget_customParameter->setLayout(extraParameterLayout);
    this->extraParameterWidget = new CustomParameterWidget();
    connect(this->extraParameterWidget, SIGNAL(functionConfigurationChanged()), this, SLOT(functionConfigurationChanged()));

    extraParameterLayout->addWidget(this->extraParameterWidget);

}

/*!
 * \brief FunctionPluginLoader::~FunctionPluginLoader
 */
FunctionPluginLoader::~FunctionPluginLoader()
{
    delete ui;
}

/*!
 * \brief FunctionPluginLoader::receivePluginsModel
 * Sets the model of the tableview for function plugins with the received available functions from the sql model.
 * \param sqlModel
 */
void FunctionPluginLoader::receivePluginsModel(QSqlQueryModel *sqlModel){
    this->ui->tableView_functionPlugins->setModel(sqlModel);
}

/*!
 * \brief FunctionPluginLoader::receiveFunctionsModel
 * \param model
 */
void FunctionPluginLoader::receiveFunctionsModel(QStandardItemModel *model){
    ui->treeView_functions->setModel(model);
    //set visibility of parameter selection menus
    this->ui->widget_element->setVisible(false);
    this->ui->widget_customParameter->setVisible(false);
}

/*!
 * \brief FunctionPluginLoader::receiveFunctionDescriptionModel
 * \param model
 */
void FunctionPluginLoader::receiveFunctionDescription(QString description){
    this->ui->textBrowser_description->setText(description);
    this->ui->textBrowser_description_2->setText("");
}

/*!
 * \brief FunctionPluginLoader::receiveAvailableElementsModel
 * \param model
 */
void FunctionPluginLoader::receiveAvailableElementsModel(AvailableElementsTreeViewProxyModel *model){
    ui->treeView_availableElements->setModel(model);

    //TODO delete selection model here ?
    QItemSelectionModel *mySelectionModel = new OiMultiSelectionModel(model);
    this->ui->treeView_availableElements->setSelectionModel(mySelectionModel);

    ui->treeView_availableElements->collapseAll();
}

/*!
 * \brief FunctionPluginLoader::receiveUsedElementsModel
 * \param model
 */
void FunctionPluginLoader::receiveUsedElementsModel(UsedElementsModel *model){
    ui->treeView_usedElements->setModel(model);
    connect(this->ui->treeView_usedElements->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), model, SLOT(selectionChanged(QModelIndex)));
}

/*!
 * \brief FunctionPluginLoader::receiveExtraParameterForFunction
 * \param intParameter
 * \param doubleParameter
 * \param stringParameter
 */
void FunctionPluginLoader::receiveExtraParameterForFunction(QMap<QString, int> intParameter, QMap<QString, double> doubleParameter,
                                                            QMap<QString, QStringList> stringParameter, FunctionConfiguration config){
    this->extraParameterWidget->setIntParameter(intParameter, config);
    this->extraParameterWidget->setDoubleParameter(doubleParameter, config);
    this->extraParameterWidget->setStringParameter(stringParameter, config);
}

/*!
 * \brief FunctionPluginLoader::disableFunctionInteractions
 */
void FunctionPluginLoader::disableFunctionInteractions(){
    this->ui->treeView_usedElements->setEnabled(false);
    this->ui->treeView_availableElements->setEnabled(false);
}

/*!
 * \brief FunctionPluginLoader::on_tableView_functionPlugins_doubleClicked
 * Add selected function to the active feature
 * \param index
 */
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

/*!
 * \brief FunctionPluginLoader::on_treeView_functions_clicked
 * Activate one of the active feature's functions
 * \param index
 */
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

/*!
 * \brief FunctionPluginLoader::deleteFunctionContextMenu
 * Show context menu to delete a function
 * \param point
 */
void FunctionPluginLoader::deleteFunctionContextMenu(QPoint point)
{
    this->deleteSelectedFunctionIndex = this->ui->treeView_functions->indexAt(point);
    if(this->deleteSelectedFunctionIndex.row() >= 0 && this->deleteSelectedFunctionIndex.parent().row() == -1){ //if function was clicked
        QMenu *menu = new QMenu();
        menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("delete function"), this, SLOT(deleteFunctionFromFeature(bool)));
        menu->exec(ui->treeView_functions->mapToGlobal(point));
    }
}

/*!
 * \brief FunctionPluginLoader::deleteFunctionFromFeature
 * Delete the function at index from the current feature
 * \param index
 */
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

/*!
 * \brief FunctionPluginLoader::setWindowTitle
 * Adds the name of the active feature to the title of the function plugin loader dialog
 * \param activeFeature
 */
/*void FunctionPluginLoader::setActiveFeatureTitle(QString activeFeature){
    //this->setWindowTitle(QString("function plugin loader - active feature: %1").arg(activeFeature));
}*/

void FunctionPluginLoader::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    //if there is an active feature
    if(OiFeatureState::getActiveFeature() != NULL){
        this->setWindowTitle(QString("function plugin loader - active feature: %1")
                             .arg(OiFeatureState::getActiveFeature()->getFeature()->getFeatureName()));

        event->accept();
    }

    event->ignore();
}

/*!
 * \brief FunctionPluginLoader::validateAvailableElements
 * Checks wether the selected items match the needed elements of the current function
 * \param selectedItems
 * \param model
 * \return
 */
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

/*!
 * \brief FunctionPluginLoader::checkChildren
 * Returns false if no child of item matches the needed type, otherwise true
 * \param item
 * \param neededType
 * \return
 */
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

/*!
 * \brief FunctionPluginLoader::setUpSelectedElements
 * Sets up the list of selected elements by checking the element types of the selected items and their children
 * \param result
 * \param item
 * \param neededType
 */
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

/*!
 * \brief FunctionPluginLoader::on_treeView_availableElements_clicked
 * Is called when available elements were selected
 * \param index
 */
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

    /*
    FeatureTreeItem *item = model->getSelectedItem(index);
    if(item != NULL && index.isValid()){
        if(model != NULL){
            bool enable = false;
            switch (model->getNeededElement()){
                case Configuration::ePointElement:
                    enable = (item->getFeature() != NULL && item->getFeature()->getPoint() != NULL);
                    break;
                case Configuration::eLineElement:
                    enable = (item->getFeature() != NULL && item->getFeature()->getLine() != NULL);
                    break;
                case Configuration::ePlaneElement:
                    enable = (item->getFeature() != NULL && item->getFeature()->getPlane() != NULL);
                    break;
                case Configuration::eSphereElement:
                    enable = (item->getFeature() != NULL && item->getFeature()->getSphere() != NULL);
                    break;
                case Configuration::eScalarEntityAngleElement:
                    enable = (item->getFeature() != NULL && item->getFeature()->getScalarEntityAngle() != NULL);
                    break;
                case Configuration::eScalarEntityDistanceElement:
                    enable = (item->getFeature() != NULL && item->getFeature()->getScalarEntityDistance() != NULL);
                    break;
                case Configuration::eCoordinateSystemElement:
                    enable = (item->getFeature() != NULL && item->getFeature()->getCoordinateSystem() != NULL);
                    break;
                case Configuration::eStationElement:
                    enable = (item->getFeature() != NULL && item->getFeature()->getStation() != NULL);
                    break;
                case Configuration::eTrafoParamElement:
                    enable = (item->getFeature() != NULL && item->getFeature()->getTrafoParam() != NULL);
                    break;
                case Configuration::eObservationElement:
                    enable = item->getIsObservation();
                    break;
                case Configuration::eReadingCartesianElement:
                    enable = (item->getReading() != NULL && item->getReading()->typeofReading == Configuration::eCartesian);
                    break;
                case Configuration::eReadingDirectionElement:
                    enable = (item->getReading() != NULL && item->getReading()->typeofReading == Configuration::eDirection);
                    break;
                case Configuration::eReadingDistanceElement:
                    enable = (item->getReading() != NULL && item->getReading()->typeofReading == Configuration::eDistance);
                    break;
                case Configuration::eReadingPolarElement:
                    enable = (item->getReading() != NULL && item->getReading()->typeofReading == Configuration::ePolar);
                    break;
                case Configuration::eScalarEntityTemperatureElement:
                    enable = (item->getFeature() != NULL && item->getFeature()->getScalarEntityTemperature() != NULL);
                    break;
            }
            this->selectedAvailableElement = item;
            this->ui->cmd_addElement->setEnabled(enable);
        }
    }*/
}

/*!
 * \brief FunctionPluginLoader::on_treeView_availableElements_entered
 * Is called when available elements were selected
 * \param index
 */
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

/*!
 * \brief FunctionPluginLoader::on_cmd_addElement_clicked
 * Add an element to the active function of the active feature
 */
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

/*!
 * \brief FunctionPluginLoader::closeEvent
 * \param event
 */
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

/*!
 * \brief FunctionPluginLoader::on_tableView_functionPlugins_clicked
 * If a plugin was selected its description is shown
 * \param index
 */
void FunctionPluginLoader::on_tableView_functionPlugins_clicked(const QModelIndex &index){
    QModelIndex modelIdx = this->ui->tableView_functionPlugins->model()->index(index.row(), 1);
    QString description = this->ui->tableView_functionPlugins->model()->data(modelIdx).toString();
    this->ui->textBrowser_description_2->setText(description);
}

/*!
 * \brief FunctionPluginLoader::on_cmd_ok_clicked
 * Close the dialog
 */
void FunctionPluginLoader::on_cmd_ok_clicked(){
    this->close();
}

/*!
 * \brief FunctionPluginLoader::on_cmd_removeElement_clicked
 * Remove an element from the active function of the active feature
 */
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

/*!
 * \brief FunctionPluginLoader::on_treeView_usedElements_clicked
 * Is called when used elements were selected
 * \param index
 */
void FunctionPluginLoader::on_treeView_usedElements_clicked(const QModelIndex &index){
    this->ui->cmd_removeElement->setEnabled(true);
}

/*!
 * \brief FunctionPluginLoader::on_treeView_usedElements_activated
 * Is called when used elements were selected
 * \param index
 */
void FunctionPluginLoader::on_treeView_usedElements_activated(const QModelIndex &index){
    this->ui->cmd_removeElement->setEnabled(true);
}

/*!
 * \brief FunctionPluginLoader::functionConfigurationChanged
 */
void FunctionPluginLoader::functionConfigurationChanged(){
    QModelIndex selectedIndex = this->ui->treeView_functions->currentIndex();
    if(selectedIndex.parent().row() == -1){ //if a function is selected
        emit this->sendFunctionConfiguration(selectedIndex.row(), this->extraParameterWidget->getFunctionConfiguration());
    }
}
