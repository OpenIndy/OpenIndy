#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QStringListModel>
#include <simulationmodel.h>
#include <systemdbmanager.h>
#include <pluginloader.h>
#include <simulationerrortablemodel.h>

class SimulationController : public QObject
{
    Q_OBJECT
public:
    explicit SimulationController(QObject *parent = 0);

    QList<SimulationPlugin> simulations;
    QStringListModel* availableSimulations;
    SimulationErrorTableModel *errorTableModel;


signals:

public slots:
    void setUpSimulations();
    SimulationModel* getSimulationAt(int i);



};

#endif // SIMULATIONCONTROLLER_H
