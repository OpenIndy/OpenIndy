#include "featuredata.h"
#include "ui_featuredata.h"

/*!
 * \brief FeatureData constructor
 * \param parent
 */
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

/*!
 * \brief FeatureData destructor
 */
FeatureData::~FeatureData()
{
    delete ui;
}

/*!
 * \brief cancels the dialog without saving anything.
 */
void FeatureData::on_pushButton_cancel_clicked()
{
    this->close();
}

/*!
 * \brief closes the dialog without saving anything
 */
void FeatureData::on_pushButton_ok_clicked()
{
    this->close();
}

/*!
 * \brief showEvent
 * \param event
 */
void FeatureData::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    //this->updateModels();

	this->initGUI();
    event->accept();
}

/*!
 * \brief resizeView to data size
 */
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

/*!
 * \brief initiates the gui and fills the combobox with the functions of the given feature.
 */
void FeatureData::initGUI(){

    this->displayUsedReadings();

    this->setWindowTitle(QString("information abaout " + OiFeatureState::getActiveFeature()->getFeature()->getFeatureName()));

    ui->tableView_observation->setModel(this->observationOverviewModel);
    ui->tableView_readings->setModel(this->readingOverviewModel);

    ui->tableView_displayedfunctionStatistic->setModel(NULL);
    ui->tableView_qxxAposteriori->setModel(NULL);
    ui->tableView_sxxApriori->setModel(NULL);
    ui->label_s0aposterioriValue->setText("");
    ui->label_s0aprioriValue->setText("");

    ui->comboBox_displayedFunction->clear();

    for(int i=0; i<OiFeatureState::getActiveFeature()->getFeature()->getFunctions().size();i++){
        ui->comboBox_displayedFunction->addItem(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(i)->getMetaData()->name);
    }

    this->updateModels();
}

/*!
 * \brief handles the initialisation and actualisation of the models, if the selected function is changed.
 * \param QString arg1
 */
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

        if(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().size()>0){
            ui->textBrowser_protocol->clear();

            for(int i=0; i<OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getResultProtocol().size();i++){
                ui->textBrowser_protocol->append(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getResultProtocol().at(i));
            }

            if(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getStatistic().isValid){
                OiMat tmpQxxOriginal = OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getStatistic().qxx;
                double tmps0aposteriori = OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getStatistic().s0_aposteriori;
                double tmps0apriori = OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getStatistic().s0_apriori;


                OiMat tmpQxx = (tmps0aposteriori*tmps0aposteriori)*tmpQxxOriginal;

                OiMat tmpSxx = (tmps0apriori*tmps0apriori)*tmpQxxOriginal;

                qxxModel->updateModel(tmpQxx);
                sxxModel->updateModel(tmpSxx);
                ui->label_s0aposterioriValue->setText(QString::number(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getStatistic().s0_aposteriori,'f',6));
                ui->label_s0aprioriValue->setText(QString::number(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getStatistic().s0_apriori,'f',6));
            }
        }

        if(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getFeatureOrder().size() > 0){

            fModel->setFunction(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(currentFunction));
            ui->tableView_displayedfunctionStatistic->setModel(fModel);
            fModel->updateModel();
        }else{
            ui->tableView_displayedfunctionStatistic->setModel(NULL);
        }
    }
}

/*!
 * \brief overwritten closeevent that destructs the dynamic gui.
 * if the selected feature is a station some parts of the gui are created dynamically.
 * \param event
 */
void FeatureData::closeEvent(QCloseEvent *event)
{
    event->accept();
}

/*!
 * \brief displayUsedReadings checks the used reading types of this geometry and displays them in the combobox.
 */
void FeatureData::displayUsedReadings()
{
    this->ui->comboBox_readings->clear();

    GUIConfiguration::readingType = "";

    Geometry *geom = NULL;

    if(OiFeatureState::getActiveFeature()->getGeometry() != NULL){
        geom = OiFeatureState::getActiveFeature()->getGeometry();
    }else if(OiFeatureState::getActiveFeature()->getStation() != NULL){
        geom = OiFeatureState::getActiveFeature()->getStation()->position;
    }

    if(geom != NULL){

        QMapIterator<Configuration::ReadingTypes, QString> i(geom->getUsedReadingTypes());
        while(i.hasNext()){
            i.next();
            this->ui->comboBox_readings->addItem(i.value());
        }
    }
}

/*!
 * \brief sets current selected reading type name in GUIConfiguration class.
 * After this the reading model can display the right columns depending on the selected reading type.
 * \param arg1
 */
void FeatureData::on_comboBox_readings_currentTextChanged(const QString &arg1)
{
    GUIConfiguration::readingType = arg1;
    if(GUIConfiguration::readingType.compare("") != 0){
        this->updateModels();
    }
}

/*!
 * \brief updateModels updates the reading and observation model
 */
void FeatureData::updateModels()
{
    this->rModel->updateModel();
    this->oModel->updateModel();
}
