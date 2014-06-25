#include "oisimulationwidget.h"
#include "ui_oisimulationwidget.h"

OiSimulationWidget::OiSimulationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OiSimulationWidget)
{
    ui->setupUi(this);

    actualSimulation = NULL;

    this->ui->listView_simulations->setModel(this->control.availableSimulations);

    this->ui->tableView_sensor->setModel(control.errorTableModel);

}

OiSimulationWidget::~OiSimulationWidget()
{
    delete ui;
}

void OiSimulationWidget::recalcAll()
{
    /*// safe readings/obs in a QMap
    //logic for shuffling the observations
    //overwrite observations/readings

    for(int i = 0; i <1000; i++){



    //TODO  put this function to the OiFeatureState class
    foreach(CoordinateSystem *c, OiFeatureState::getCoordinateSystems()){
        foreach(TrafoParam *t, c->getTransformationParameters()){
            //check if t is already solved
            FeatureUpdater::recalcTrafoParam(t);
        }
    }

    FeatureUpdater::recalcFeatureSet();

    }

    //reset obs/readings to orignal
    //recalc all*/

}

void OiSimulationWidget::on_pushButton_startSimulation_clicked()
{
    recalcAll();
}

void OiSimulationWidget::showEvent(QShowEvent *event)
{
    control.setUpSimulations();

    event->accept();
}



void OiSimulationWidget::on_listView_simulations_clicked(const QModelIndex &index)
{
    if(index.isValid()){
        actualSimulation = control.getSimulationAt(index.row());
        Console::addLine(actualSimulation->getMetaData()->name);
    }
}
