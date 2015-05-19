#include "actualpropertiesdialog.h"
#include "ui_actualpropertiesdialog.h"

/*
#include "featuredata.h"
#include "ui_featuredata.h"


FeatureData::FeatureData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FeatureData)
{
    ui->setupUi(this);

    qxxModel = NULL;
    sxxModel = NULL;

    this->observationOverviewModel = NULL;
    this->readingOverviewModel = NULL;

    rModel = new ReadingModel(0);
    oModel = new ObservationModel(0);

    this->observationOverviewModel = new ObservationProxyModel();
    this->observationOverviewModel->setSourceModel(this->oModel);

    this->readingOverviewModel = new ReadingProxyModel();
    this->readingOverviewModel->setSourceModel(this->rModel);


    this->oModel->updateModel();
    this->rModel->updateModel();

    //connect reading and observation view
    connect(this->rModel,SIGNAL(resizeView()),this,SLOT(resizeView()));
    connect(this->oModel,SIGNAL(resizeView()),this,SLOT(resizeView()));
}

FeatureData::~FeatureData()
{
    delete ui;
}

void FeatureData::on_pushButton_cancel_clicked()
{
    this->close();
}

void FeatureData::on_pushButton_ok_clicked()
{
    this->close();
}

void FeatureData::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    //this->updateModels();

	this->initGUI();
    event->accept();
}

void FeatureData::resizeView()
{
    ui->tableView_observation->resizeColumnsToContents();
    ui->tableView_observation->resizeRowsToContents();

    ui->tableView_readings->resizeColumnsToContents();
    ui->tableView_readings->resizeRowsToContents();

    ui->tableView_displayedfunctionStatistic->resizeColumnsToContents();
    ui->tableView_displayedfunctionStatistic->resizeRowsToContents();

    ui->tableView_qxxAposteriori->resizeColumnsToContents();
    ui->tableView_qxxAposteriori->resizeRowsToContents();

    ui->tableView_sxxApriori->resizeColumnsToContents();
    ui->tableView_sxxApriori->resizeRowsToContents();
}

void FeatureData::initGUI(){

    this->displayUsedReadings();

    this->setWindowTitle(QString("information abaout " + OiJob::getActiveFeature()->getFeature()->getFeatureName()));

    ui->tableView_observation->setModel(this->observationOverviewModel);
    ui->tableView_readings->setModel(this->readingOverviewModel);

    ObservationDelegate *myObsDelegate = new ObservationDelegate();
    ui->tableView_observation->setItemDelegate(myObsDelegate);

    ui->tableView_displayedfunctionStatistic->setModel(NULL);
    ui->tableView_qxxAposteriori->setModel(NULL);
    ui->tableView_sxxApriori->setModel(NULL);
    ui->label_s0aposterioriValue->setText("");
    ui->label_s0aprioriValue->setText("");

    ui->comboBox_displayedFunction->clear();

    for(int i=0; i<OiJob::getActiveFeature()->getFeature()->getFunctions().size();i++){
        ui->comboBox_displayedFunction->addItem(OiJob::getActiveFeature()->getFeature()->getFunctions().at(i)->getMetaData()->name);
    }

    this->updateModels();
}

void FeatureData::on_comboBox_displayedFunction_currentIndexChanged(const QString &arg1)
{
    int currentFunction = ui->comboBox_displayedFunction->currentIndex();

    if(currentFunction != -1){

        fModel = new FunctionStatistic();
        //fModel->setFunction(NULL);
        //ui->tableView_displayedfunctionStatistic->setModel(fModel);
        //fModel->updateModel();
        ui->tableView_displayedfunctionStatistic->setModel(NULL);

        qxxModel = new MatrixModel();
        sxxModel = new MatrixModel();

        ui->tableView_qxxAposteriori->setModel(qxxModel);
        ui->tableView_sxxApriori->setModel(sxxModel);

        if(OiJob::getActiveFeature()->getFeature()->getFunctions().size()>0){
            ui->textBrowser_protocol->clear();

            for(int i=0; i<OiJob::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getResultProtocol().size();i++){
                ui->textBrowser_protocol->append(OiJob::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getResultProtocol().at(i));
            }

            if(OiJob::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getStatistic().isValid){
                OiMat tmpQxxOriginal = OiJob::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getStatistic().qxx;
                double tmps0aposteriori = OiJob::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getStatistic().s0_aposteriori;
                double tmps0apriori = OiJob::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getStatistic().s0_apriori;


                OiMat tmpQxx = (tmps0aposteriori*tmps0aposteriori)*tmpQxxOriginal;

                OiMat tmpSxx = (tmps0apriori*tmps0apriori)*tmpQxxOriginal;

                qxxModel->updateModel(tmpQxx);
                sxxModel->updateModel(tmpSxx);
                ui->label_s0aposterioriValue->setText(QString::number(OiJob::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getStatistic().s0_aposteriori,'f',6));
                ui->label_s0aprioriValue->setText(QString::number(OiJob::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getStatistic().s0_apriori,'f',6));
            }
        }

        if(OiJob::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getFeatureOrder().size() > 0){

            fModel->setFunction(OiJob::getActiveFeature()->getFeature()->getFunctions().at(currentFunction));
            ui->tableView_displayedfunctionStatistic->setModel(fModel);
            fModel->updateModel();
        }else{
            fModel->setFunction(NULL);
            ui->tableView_displayedfunctionStatistic->setModel(NULL);
        }
    }
}

void FeatureData::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void FeatureData::displayUsedReadings()
{
    this->ui->comboBox_readings->clear();

    GUIConfiguration::readingType = "";

    Geometry *geom = NULL;

    if(OiJob::getActiveFeature()->getGeometry() != NULL){
        geom = OiJob::getActiveFeature()->getGeometry();
    }else if(OiJob::getActiveFeature()->getStation() != NULL){
        geom = OiJob::getActiveFeature()->getStation()->position;
    }

    if(geom != NULL){

        QMapIterator<Configuration::ReadingTypes, QString> i(geom->getUsedReadingTypes());
        while(i.hasNext()){
            i.next();
            this->ui->comboBox_readings->addItem(i.value());
        }
    }
}

void FeatureData::on_comboBox_readings_currentTextChanged(const QString &arg1)
{
    GUIConfiguration::readingType = arg1;
    if(GUIConfiguration::readingType.compare("") != 0){
        this->updateModels();
    }
}

void FeatureData::updateModels()
{
    this->rModel->updateModel();
    this->oModel->updateModel();
}

void FeatureData::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    int modifiers = event->modifiers();

    if(key == Qt::Key_C && modifiers == Qt::CTRL){
        this->copyValuesFromView();
    }
}

void FeatureData::copyValuesFromView()
{
    //get model and selected rows and columns
    QAbstractItemModel *model = ui->tableView_observation->model();
    QItemSelectionModel *selection = ui->tableView_observation->selectionModel();
    QModelIndexList indexes = selection->selectedIndexes();

    //sort indexes
    qSort(indexes);

    //check if something in observation view is selected
    if(indexes.size()<1){

        //if not, check if something in reading view is selected
        model = ui->tableView_readings->model();
        selection = ui->tableView_readings->selectionModel();
        indexes = selection->selectedIndexes();

        qSort(indexes);

        //if also not, copy nothing
        if(indexes.size()<1){
            return;
        }
        //return;
    }

    QString copy_table;
    QModelIndex last = indexes.last();
    QModelIndex previous = indexes.first();

    indexes.removeFirst();

    //loop over all selected rows and columns
    for(int i=0;i<indexes.size();i++){
        QVariant data = model->data(previous);
        QString text = data.toString();

        QModelIndex index = indexes.at(i);
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
*/


ActualPropertiesDialog::ActualPropertiesDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::ActualPropertiesDialog)
{
    this->ui->setupUi(this);

    //init GUI elements and assign models
    this->initGUI();
    this->initModels();
}

/*!
 * \brief ActualPropertiesDialog::~ActualPropertiesDialog
 */
ActualPropertiesDialog::~ActualPropertiesDialog(){
    delete this->ui;
}

/*!
 * \brief ActualPropertiesDialog::on_tableView_observation_customContextMenuRequested
 * \param pos
 */
void ActualPropertiesDialog::on_tableView_observation_customContextMenuRequested(const QPoint &pos){

    //create menu and add import action
    QMenu *menu = new QMenu();
    menu->addAction(QIcon(":/Images/icons/edit_add.png"), QString("import observations"), this, SLOT(importObservationsMenuClicked(bool)));
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
 * \brief ActualPropertiesDialog::showEvent
 * \param event
 */
void ActualPropertiesDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    event->accept();

}

/*!
 * \brief ActualPropertiesDialog::initGUI
 */
void ActualPropertiesDialog::initGUI(){

    //resize rows and columns to table view contents
    this->ui->tableView_observation->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->tableView_observation->verticalHeader()->setDefaultSectionSize(22);
    this->ui->tableView_readings->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->tableView_readings->verticalHeader()->setDefaultSectionSize(22);

    //enable context menu in observations table view
    this->ui->tableView_observation->setContextMenuPolicy(Qt::CustomContextMenu);

}

/*!
 * \brief ActualPropertiesDialog::initModels
 */
void ActualPropertiesDialog::initModels(){

    //assign observation and reading models
    this->ui->tableView_observation->setModel(&ModelManager::getObservationProxyModel());
    this->ui->tableView_readings->setModel(&ModelManager::getReadingProxyModel());

}
