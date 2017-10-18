#include "actualpropertiesdialog.h"
#include "ui_actualpropertiesdialog.h"

/*!
 * \brief ActualPropertiesDialog::ActualPropertiesDialog
 * \param parent
 */
ActualPropertiesDialog::ActualPropertiesDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::ActualPropertiesDialog)
{
    this->ui->setupUi(this);

    //init GUI elements and assign models
    this->initGUI();
    this->initModels();

    QObject::connect(this,&ActualPropertiesDialog::useObservation, &ModelManager::getObservationModel(),
                     &ObservationModel::setObservationUseStateByContextmenu, Qt::AutoConnection);
    QObject::connect(this,&ActualPropertiesDialog::unUseObservation, &ModelManager::getObservationModel(),
                     &ObservationModel::setObservationUseStateByContextmenu, Qt::AutoConnection);
}

/*!
 * \brief ActualPropertiesDialog::~ActualPropertiesDialog
 */
ActualPropertiesDialog::~ActualPropertiesDialog(){
    QObject::disconnect(this,&ActualPropertiesDialog::useObservation, &ModelManager::getObservationModel(),
                     &ObservationModel::setObservationUseStateByContextmenu);
    QObject::disconnect(this,&ActualPropertiesDialog::unUseObservation, &ModelManager::getObservationModel(),
                     &ObservationModel::setObservationUseStateByContextmenu);
    delete this->ui;
}

/*!
 * \brief ActualPropertiesDialog::keyPressEvent
 * \param e
 */
void ActualPropertiesDialog::keyPressEvent(QKeyEvent *e){

    //check triggered keys
    switch(e->key()){
    case Qt::Key_C:

        if(e->modifiers() == Qt::CTRL){
            this->copyToClipboard();
        }

    }

}

/*!
 * \brief ActualPropertiesDialog::on_tableView_observation_customContextMenuRequested
 * \param pos
 */
void ActualPropertiesDialog::on_tableView_observation_customContextMenuRequested(const QPoint &pos){

    //create menu and add import action
    QMenu *menu = new QMenu();
    menu->addAction(QIcon(":/Images/icons/edit_add.png"), QString("import observations"), this, SLOT(importObservationsMenuClicked(bool)));

    //add use / unuse actions
    menu->addAction(QIcon(":/Images/icons/edit_add.png"), QString("use selected observation(s)"), this, SLOT(useObservations(bool)));
    menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("unuse selected observation(s)"), this, SLOT(unUseObservations(bool)));

    //get observation table models
    ObservationProxyModel *model = static_cast<ObservationProxyModel*>(this->ui->tableView_observation->model());
    if(model == NULL){
        delete menu;
        return;
    }

    ObservationModel *sourceModel =  static_cast<ObservationModel*>(model->sourceModel());
    if(sourceModel == NULL){
        delete menu;
        return;
    }
    menu->exec(this->ui->tableView_observation->mapToGlobal(pos));

}

/*!
 * \brief ActualPropertiesDialog::importObservationsMenuClicked
 * \param checked
 */
void ActualPropertiesDialog::importObservationsMenuClicked(bool checked){

    QString filename = QFileDialog::getOpenFileName(this, "Choose a file", "", "");
    if(filename.compare("") == 0){
        return;
    }

    emit this->importObservations(filename);

}

/*!
 * \brief ActualPropertiesDialog::resizeTableView
 */
void ActualPropertiesDialog::resizeTableView(){
    this->ui->tableView_observation->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_readings->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_displayedfunctionStatistic->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_observation->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_readings->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_displayedfunctionStatistic->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

/*!
 * \brief ActualPropertiesDialog::on_comboBox_displayedFunction_currentIndexChanged
 * \param index
 */
void ActualPropertiesDialog::on_comboBox_displayedFunction_currentIndexChanged(int index){

    //check function statistic model
    if(this->functionStatisticModel.isNull()){
        return;
    }

    this->functionStatisticModel->setFunctionIndex(index);

}

/*!
 * \brief ActualPropertiesDialog::copyToClipboard
 * Copies the current selection into the clipboard
 */
void ActualPropertiesDialog::copyToClipboard(){

    //init variables
    QAbstractItemModel *model = NULL;
    QItemSelectionModel *selectionModel = NULL;
    QModelIndexList selection;

    //get selection of the active table view
    if(this->ui->tabWidget_selectedFeature->currentWidget() == this->ui->tab_observations){ //observation table view
        model = this->ui->tableView_observation->model();
        selectionModel = this->ui->tableView_observation->selectionModel();
        selection = selectionModel->selectedIndexes();
    }else if(this->ui->tabWidget_selectedFeature->currentWidget() == this->ui->tab_readings){ //reading table view
        model = this->ui->tableView_readings->model();
        selectionModel = this->ui->tableView_readings->selectionModel();
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
            copy_table.append('\n');
        }else{ //if same line, but new column
            copy_table.append('\t');
        }
        previous = index;

    }

    //get last selected cell
    copy_table.append(model->data(last).toString());
    copy_table.append('\n');

    //set values to clipboard, so you can copy them
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(copy_table);

}

/*!
 * \brief ActualPropertiesDialog::showEvent
 * \param event
 */
void ActualPropertiesDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    //assign function statistic model
    if(!this->functionStatisticModel.isNull()){
        delete this->functionStatisticModel;
        this->ui->tableView_displayedfunctionStatistic->setModel(NULL);
    }
    this->functionStatisticModel = ModelManager::getFunctionStatisticModel();
    if(!this->functionStatisticModel.isNull()){
        int functionIndex = this->ui->comboBox_displayedFunction->currentIndex();
        if(functionIndex >= 0){
            this->functionStatisticModel->setFunctionIndex(functionIndex);
        }
        this->ui->tableView_displayedfunctionStatistic->setModel(this->functionStatisticModel);
    }

    this->resizeTableView();

    event->accept();

}

/*!
 * \brief ActualPropertiesDialog::initGUI
 */
void ActualPropertiesDialog::initGUI(){

    //resize rows and columns to table view contents on double click
    QObject::connect(this->ui->tableView_observation->horizontalHeader(), &QHeaderView::sectionDoubleClicked, this, &ActualPropertiesDialog::resizeTableView, Qt::AutoConnection);
    QObject::connect(this->ui->tableView_observation->verticalHeader(), &QHeaderView::sectionDoubleClicked, this, &ActualPropertiesDialog::resizeTableView, Qt::AutoConnection);
    QObject::connect(this->ui->tableView_readings->horizontalHeader(), &QHeaderView::sectionDoubleClicked, this, &ActualPropertiesDialog::resizeTableView, Qt::AutoConnection);
    QObject::connect(this->ui->tableView_readings->verticalHeader(), &QHeaderView::sectionDoubleClicked, this, &ActualPropertiesDialog::resizeTableView, Qt::AutoConnection);
    QObject::connect(this->ui->tableView_displayedfunctionStatistic->horizontalHeader(), &QHeaderView::sectionDoubleClicked, this, &ActualPropertiesDialog::resizeTableView, Qt::AutoConnection);
    QObject::connect(this->ui->tableView_displayedfunctionStatistic->verticalHeader(), &QHeaderView::sectionDoubleClicked, this, &ActualPropertiesDialog::resizeTableView, Qt::AutoConnection);

    //enable context menu in observations table view
    this->ui->tableView_observation->setContextMenuPolicy(Qt::CustomContextMenu);

    //set up delegates
    ObservationTableDelegate *observationTableDelegate = new ObservationTableDelegate();
    this->ui->tableView_observation->setItemDelegate(observationTableDelegate);
    ReadingTableDelegate *readingTableDelegate = new ReadingTableDelegate();
    this->ui->tableView_readings->setItemDelegate(readingTableDelegate);

}

/*!
 * \brief ActualPropertiesDialog::initModels
 */
void ActualPropertiesDialog::initModels(){

    //assign observation and reading models
    this->ui->tableView_observation->setModel(&ModelManager::getObservationProxyModel());
    this->ui->tableView_readings->setModel(&ModelManager::getReadingProxyModel());

    //assign active feature functions model
    this->ui->comboBox_displayedFunction->setModel(&ModelManager::getActiveFeatureFunctionsModel());

}

/*!
 * \brief ActualPropertiesDialog::on_tabWidget_selectedFeature_customContextMenuRequested
 * \param pos
 */
void ActualPropertiesDialog::on_tabWidget_selectedFeature_customContextMenuRequested(const QPoint &pos)
{
    //create  menu and add delete action
    QMenu *menu = new QMenu();

    menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("use selected observation(s)"), this, SLOT(useObservations(bool)));
    menu->addAction(QIcon(":/Images/icons/edit_add.png"), QString("unuse selected observation(s)"), this, SLOT(unUseObservations(bool)));

    //get observation table models
    ObservationProxyModel *model = static_cast<ObservationProxyModel*>(this->ui->tableView_observation->model());
    if(model == NULL){
        delete menu;
        return;
    }

    ObservationModel *sourceModel =  static_cast<ObservationModel*>(model->sourceModel());
    if(sourceModel == NULL){
        delete menu;
        return;
    }
}

/*!
 * \brief ActualPropertiesDialog::unUseObservations
 * \param use
 */
void ActualPropertiesDialog::unUseObservations(bool use)
{
    //init variables
    QPointer<QSortFilterProxyModel> model;
    QPointer<QItemSelectionModel> selectionModel;
    QModelIndexList selection;

    //get models of observation tabview
    if(this->ui->tabWidget_selectedFeature->currentWidget() != this->ui->tab_observations){
        return;
    }

    model = static_cast<ObservationProxyModel *>(this->ui->tableView_observation->model());
    if(model == NULL){
        return;
    }

    //get selection
    selectionModel = this->ui->tableView_observation->selectionModel();

    //get and check source model
    ObservationModel *sourceModel = static_cast<ObservationModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get selected indexes
    selection = selectionModel->selectedIndexes();
    if(selection.size() <= 0){
        //emit this->log("No observations selected", eErrorMessage, eMessageBoxMessage);
        return;
    }
    qSort(selection);

    foreach (QModelIndex idx, selection) {
        emit this->useObservation(false, idx);
    }
}

/*!
 * \brief ActualPropertiesDialog::useUnuseObservations
 * \param use
 */
void ActualPropertiesDialog::useObservations(bool use)
{
    //init variables
    QPointer<QSortFilterProxyModel> model;
    QPointer<QItemSelectionModel> selectionModel;
    QModelIndexList selection;

    //get models of observation tabview
    if(this->ui->tabWidget_selectedFeature->currentWidget() != this->ui->tab_observations){
        return;
    }

    model = static_cast<ObservationProxyModel *>(this->ui->tableView_observation->model());
    if(model == NULL){
        return;
    }

    //get selection
    selectionModel = this->ui->tableView_observation->selectionModel();

    //get and check source model
    ObservationModel *sourceModel = static_cast<ObservationModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get selected indexes
    selection = selectionModel->selectedIndexes();
    if(selection.size() <= 0){
        //emit this->log("No observations selected", eErrorMessage, eMessageBoxMessage);
        return;
    }
    qSort(selection);

    foreach (QModelIndex idx, selection) {
        emit this->useObservation(true, idx);
    }
}
