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



}

OiSimulationWidget::~OiSimulationWidget()
{
    delete ui;
}


void OiSimulationWidget::on_pushButton_startSimulation_clicked()
{

    control.recalcAll();
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
