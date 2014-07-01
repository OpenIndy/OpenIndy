#include "oisimulationwidget.h"
#include "ui_oisimulationwidget.h"

OiSimulationWidget::OiSimulationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OiSimulationWidget)
{
    ui->setupUi(this);


    this->ui->listView_simulations->setModel(this->control.availableSimulations);

    this->ui->tableView_sensor->setModel(control.sensorErrorModel);
    this->ui->treeView_feature->setModel(control.resultModel);

    SimulationDelegate *errorDelegate = new SimulationDelegate();
    this->ui->tableView_sensor->setItemDelegate(errorDelegate);

    this->ui->progressBar->setMaximum(1000);
    this->ui->progressBar->setMinimum(0);

    connect(this,SIGNAL(startSimulation()),&this->control,SLOT(recalcAll()));
    connect(&this->control,SIGNAL(counter(int)),this->ui->progressBar,SLOT(setValue(int)));

    control.moveToThread(&workerThread);

    workerThread.start();

}

OiSimulationWidget::~OiSimulationWidget()
{
    delete ui;
}

void OiSimulationWidget::setFeatureUpdater(FeatureUpdater *f)
{
    control.setFeatureUpdater(f);
}


void OiSimulationWidget::on_pushButton_startSimulation_clicked()
{
    ui->progressBar->setValue(0);

    if(control.actualSimulation == NULL){

        QMessageBox::information(NULL, "no simulation set", "please choose your simulation first");

    }else{

    emit startSimulation();

    }

}

void OiSimulationWidget::showEvent(QShowEvent *event)
{
    control.setUpSimulations();
    control.resultModel->refreshModel();

    event->accept();
}



void OiSimulationWidget::on_listView_simulations_clicked(const QModelIndex &index)
{
    if(index.isValid()){
        control.setSimulationAt(index.row());

        control.sensorErrorModel->setErrors(control.actualSimulation->getSensorUncertainties());
        Console::addLine(control.actualSimulation->getMetaData()->name);
    }
}

void OiSimulationWidget::on_treeView_feature_clicked(const QModelIndex &index)
{

    SimulationTreeViewModel *model = dynamic_cast<SimulationTreeViewModel*>(this->ui->treeView_feature->model());
    FeatureTreeItem *item = model->getSelectedItem(index);
    if(item != NULL && index.isValid()){
        if(model != NULL){

            if(item->getParent() != NULL && item->getParent()->getIsFeature()){ //if an attribute of a feature was clicked

                ui->widgetHistogram->paintData(item->getParent()->getFeature(),"all");

            }else if(item->getIsFeature()){

                ui->widgetHistogram->paintData(item->getFeature(),"all");
            }

        }
    }

}
