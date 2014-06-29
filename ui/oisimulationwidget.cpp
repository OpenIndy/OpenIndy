#include "oisimulationwidget.h"
#include "ui_oisimulationwidget.h"

OiSimulationWidget::OiSimulationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OiSimulationWidget)
{
    ui->setupUi(this);


    this->ui->listView_simulations->setModel(this->control.availableSimulations);

    this->ui->tableView_sensor->setModel(control.errorTableModel);

    SimulationDelegate *errorDelegate = new SimulationDelegate();
    this->ui->tableView_sensor->setItemDelegate(errorDelegate);

    this->ui->progressBar->setMaximum(100);
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

    emit startSimulation();
    //control.recalcAll();
}

void OiSimulationWidget::showEvent(QShowEvent *event)
{
    control.setUpSimulations();

    event->accept();
}



void OiSimulationWidget::on_listView_simulations_clicked(const QModelIndex &index)
{
    if(index.isValid()){
        control.setSimulationAt(index.row());

        control.errorTableModel->setErrors(control.actualSimulation->getSensorUncertainties());
        Console::addLine(control.actualSimulation->getMetaData()->name);
    }
}
