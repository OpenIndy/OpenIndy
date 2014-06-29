#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QStringListModel>
#include "simulationmodel.h"
#include "systemdbmanager.h"
#include "pluginloader.h"
#include "simulationerrortablemodel.h"
#include "oifeaturestate.h"
#include "featureupdater.h"


class SimulationController : public QObject
{
    Q_OBJECT
public:
    explicit SimulationController(QObject *parent = 0);

    SimulationModel* actualSimulation;
    
    QList<SimulationPlugin> simulations;
    QStringListModel* availableSimulations;
    SimulationErrorTableModel *errorTableModel;




signals:
    void counter(int i);

public slots:
    void setFeatureUpdater(FeatureUpdater* f);
    void setUpSimulations();
    void setSimulationAt(int i);
    void recalcAll();
    
private:
    FeatureUpdater* myUpdater;



};

#endif // SIMULATIONCONTROLLER_H
