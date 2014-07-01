#ifndef OISIMULATIONWIDGET_H
#define OISIMULATIONWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QThread>
#include <QMessageBox>
#include "featureupdater.h"
#include "oifeaturestate.h"
#include "simulationcontroller.h"
#include "simulationdelegate.h"


namespace Ui {
class OiSimulationWidget;
}

class OiSimulationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OiSimulationWidget(QWidget *parent = 0);
    ~OiSimulationWidget();

    SimulationController control;
    QThread workerThread;

    void setFeatureUpdater(FeatureUpdater *f);

private slots:
    void on_pushButton_startSimulation_clicked();
    void showEvent(QShowEvent * event);

    void on_listView_simulations_clicked(const QModelIndex &index);

    void on_treeView_feature_clicked(const QModelIndex &index);

signals:
    void startSimulation();

private:
    Ui::OiSimulationWidget *ui;


};

#endif // OISIMULATIONWIDGET_H
