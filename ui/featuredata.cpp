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
 * \brief receives the active feature and initiates the models and gui.
 * If the received feature is a station with an active instrument, you can change the instrument config with this dialog
 * and connect the instrument new, if the connection settings were changed.
 * \param FeatureWrapper activeFeature
 */
/*void FeatureData::getActiveFeature(FeatureWrapper *activeFeature){

    this->selectedFeature = activeFeature;
    this->setWindowTitle(QString("information abaout " + OiFeatureState::getActiveFeature()->getFeature()->name));
    rModel = new ReadingModel(*this->selectedFeature,0);
    oModel = new ObservationModel(*OiFeatureState::getActiveFeature(),0);

    ui->tableView_observation->setModel(oModel);
    ui->tableView_readings->setModel(rModel);

    ui->tableView_displayedfunctionStatistic->setModel(NULL);
    ui->tableView_qxxAposteriori->setModel(NULL);
    ui->tableView_sxxApriori->setModel(NULL);
    ui->label_s0aposterioriValue->setText("");
    ui->label_s0aprioriValue->setText("");

    initGUI();
}*/

void FeatureData::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
	this->initGUI();
    event->accept();
}

/*!
 * \brief initiates the gui and fills the combobox with the functions of the given feature.
 */
void FeatureData::initGUI(){

    this->setWindowTitle(QString("information abaout " + OiFeatureState::getActiveFeature()->getFeature()->getFeatureName()));
    rModel = new ReadingModel(0);
    oModel = new ObservationModel(0);

    ui->tableView_observation->setModel(oModel);
    ui->tableView_readings->setModel(rModel);

    ui->tableView_displayedfunctionStatistic->setModel(NULL);
    ui->tableView_qxxAposteriori->setModel(NULL);
    ui->tableView_sxxApriori->setModel(NULL);
    ui->label_s0aposterioriValue->setText("");
    ui->label_s0aprioriValue->setText("");

    ui->comboBox_displayedFunction->clear();

    for(int i=0; i<OiFeatureState::getActiveFeature()->getFeature()->getFunctions().size();i++){
        ui->comboBox_displayedFunction->addItem(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(i)->getMetaData()->name);
    }
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
