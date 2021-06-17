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

    QObject::connect(&this->clipBoardUtil, &ClipBoardUtil::sendMessage, this, &ActualPropertiesDialog::sendMessage, Qt::AutoConnection);
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
    menu->addAction(QIcon(":/Images/icons/edit_add.png"), QString("use selected observation(s)"), this, SLOT(useObservations()));
    menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("unuse selected observation(s)"), this, SLOT(unUseObservations()));
    menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("delete selected observation(s)"), this, SLOT(deleteObservations()));

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
    this->ui->tableView_observation->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_readings->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
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
    QPointer<QAbstractItemModel> model = NULL;
    QPointer<QItemSelectionModel>selectionModel = NULL;

    //get selection of the active table view
    if(this->ui->tabWidget_selectedFeature->currentWidget() == this->ui->tab_observations){ //observation table view
        model = this->ui->tableView_observation->model();
        selectionModel = this->ui->tableView_observation->selectionModel();
    }else if(this->ui->tabWidget_selectedFeature->currentWidget() == this->ui->tab_readings){ //reading table view
        model = this->ui->tableView_readings->model();
        selectionModel = this->ui->tableView_readings->selectionModel();
    }

    clipBoardUtil.copyToClipBoard(model, selectionModel);

}

/*!
 * \brief ActualPropertiesDialog::showEvent
 * \param event
 */
void ActualPropertiesDialog::showEvent(QShowEvent *event){

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

    this->ui->tableView_observation->setSortingEnabled(true);
    this->ui->tableView_readings->setSortingEnabled(true);

}

/*!
 * \brief ActualPropertiesDialog::getSelection
 * \return
 */
QModelIndexList ActualPropertiesDialog::getSelection()
{
    //init variables
    QPointer<QSortFilterProxyModel> model;
    QPointer<QItemSelectionModel> selectionModel;
    QModelIndexList selection;

    //get models of observation tabview
    if(this->ui->tabWidget_selectedFeature->currentWidget() != this->ui->tab_observations){
        return selection;
    }

    model = static_cast<ObservationProxyModel *>(this->ui->tableView_observation->model());
    if(model == NULL){
        return selection;
    }

    //get selection
    selectionModel = this->ui->tableView_observation->selectionModel();

    //get and check source model
    ObservationModel *sourceModel = static_cast<ObservationModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return selection;
    }

    //get selected indexes
    selection = selectionModel->selectedIndexes();
    if(selection.size() > 0){
        qSort(selection);
    }

    return selection;
}

/*!
 * \brief ActualPropertiesDialog::on_tabWidget_selectedFeature_customContextMenuRequested
 * \param pos
 */
void ActualPropertiesDialog::on_tabWidget_selectedFeature_customContextMenuRequested(const QPoint &pos)
{
    //create  menu and add delete action
    QMenu *menu = new QMenu();

    menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("use selected observation(s)"), this, SLOT(useObservations()));
    menu->addAction(QIcon(":/Images/icons/edit_add.png"), QString("unuse selected observation(s)"), this, SLOT(unUseObservations()));

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
 */
void ActualPropertiesDialog::unUseObservations()
{
    //init variables
    QModelIndexList selection;

    //get selected indexes
    selection = this->getSelection();
    if(selection.size() > 0){
        ObservationProxyModel *model = static_cast<ObservationProxyModel *>(this->ui->tableView_observation->model());
        if(model == NULL){
            return;
        }
        foreach (QModelIndex idx, selection) {
            emit this->useObservation(false, model->mapToSource(idx));
        }
    }
}

/*!
 * \brief ActualPropertiesDialog::useObservations
 */
void ActualPropertiesDialog::useObservations()
{
    //init variables
    QModelIndexList selection;

    //get selected indexes
    selection = this->getSelection();
    if(selection.size() > 0){
        ObservationProxyModel *model = static_cast<ObservationProxyModel *>(this->ui->tableView_observation->model());
        if(model == NULL){
            return;
        }
        foreach (QModelIndex idx, selection) {
            emit this->useObservation(true, model->mapToSource(idx));
        }
    }
}

/*!
 * \brief ActualPropertiesDialog::deleteObservations
 */
void ActualPropertiesDialog::deleteObservations()
{
    //init variables
    QModelIndexList selection;

    //get selected indexes
    selection = this->getSelection();
    if(selection.size() > 0){
        QList<int> selectedIds;
        ObservationProxyModel *model = static_cast<ObservationProxyModel *>(this->ui->tableView_observation->model());
        if(model == NULL){
            return;
        }
        foreach (QModelIndex idx, selection) {
            int i = ModelManager::getObservationModel().getObservationIdByIndex(model->mapToSource(idx));
            if (i >= 0) {
                selectedIds.append(i);
            }
        }   
        emit this->removeObservationsById(selectedIds);
    }
}
