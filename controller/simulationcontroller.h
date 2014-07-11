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
#include "simulationtreeviewmodel.h"


struct SimulationSetting{
    QMap<QString,int>* defaultIntegerParameter;
    QMap<QString,double>* defaultDoubleParameter;
    QMap <QString, QStringList>* defaultStringParameter;

    QMap<QString,int> integerParameter;
    QMap<QString,double> doubleParameter;
    QMap <QString, QString> stringParameter;
};

class SimulationController : public QObject
{
    Q_OBJECT
public:
    explicit SimulationController(QObject *parent = 0);

    SimulationModel* actualSimulation;
    
    QList<SimulationPlugin> simulations;
    QStringListModel* availableSimulations;


    SimulationErrorTableModel *sensorErrorModel;
    SimulationErrorTableModel *objectErrorModel;
    SimulationErrorTableModel *humanErrorModel;
    SimulationErrorTableModel *environmentErrorModel;

    SimulationTreeViewModel *resultModel;





signals:
    void counter(int i);

public slots:
    void setFeatureUpdater(FeatureUpdater* f);
    void setUpSimulations();
    void setSimulationAt(int i);
    void recalcAll();

    void setIterations(int i);
    int getIterations();

    void getDefaultSettings();
    void setSettings();
    void setSettingItem(QString key, double value);
    void setSettingItem(QString key, int value);
    void setSettingItem(QString key, QString value);
    QMap<QString,int>*  getIntegerParamter();
    QMap<QString,double>*  getDoubleParamter();
    QMap<QString,QStringList>*  getStringParamter();


    
private:
    FeatureUpdater* myUpdater;
    int iterations;

    SimulationSetting simulationSettings;




};

#endif // SIMULATIONCONTROLLER_H
