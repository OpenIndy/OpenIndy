#include "simulationcontroller.h"

SimulationController::SimulationController(QObject *parent) :
    QObject(parent)
{
    this->actualSimulation = NULL;

    this->availableSimulations = new QStringListModel();

    sensorErrorModel = new SimulationErrorTableModel();
    objectErrorModel = new SimulationErrorTableModel();
    humanErrorModel = new SimulationErrorTableModel();
    environmentErrorModel = new SimulationErrorTableModel();

    resultModel = new SimulationTreeViewModel();


    myUpdater = NULL;
}

void SimulationController::setFeatureUpdater(FeatureUpdater *f)
{
    this->myUpdater = f;
}

void SimulationController::setUpSimulations()
{
    this->simulations = SystemDbManager::getAvailableSimulationPlugins();

    QStringList simulationNames;

    foreach(SimulationPlugin p, simulations){
        simulationNames.append(p.name);
    }

    this->availableSimulations->setStringList(simulationNames);
}

void SimulationController::recalcAll()
{

    Uncertainties u;

    u.sensorUncertainties = sensorErrorModel->getErrors();
    actualSimulation->setGivenUncertainties(u);



    for(int i = 0; i <(this->iterations+1); i++){

        foreach(Station *s, OiFeatureState::getStations()){
            foreach(Observation *o, s->coordSys->getObservations()){
                OiMat A;
                o->myReading->restoreBackup();

                if(i < this->iterations){
                    actualSimulation->distort(o->myReading,A);
                }

                o->myOriginalXyz = Reading::toCartesian(o->myReading->rPolar.azimuth,o->myReading->rPolar.zenith,o->myReading->rPolar.distance);
                o->myXyz = Reading::toCartesian(o->myReading->rPolar.azimuth,o->myReading->rPolar.zenith,o->myReading->rPolar.distance);
            }
        }

        myUpdater->recalcAll();

        if(i < this->iterations){
            foreach(FeatureWrapper *f, OiFeatureState::getFeatures()){
                if(f->getGeometry() != NULL){
                    f->getGeometry()->saveSimulationData();
                }
            }
        }

        emit counter(i);
    }


}

void SimulationController::setIterations(int i)
{
    this->iterations = i;
}

int SimulationController::getIterations()
{
    return this->iterations;
}

void SimulationController::getDefaultSettings()
{
    this->simulationSettings.defaultIntegerParameter = this->actualSimulation->getIntegerParameter();
    this->simulationSettings.defaultDoubleParameter = this->actualSimulation->getDoubleParameter();
    this->simulationSettings.defaultStringParameter = this->actualSimulation->getStringParameter();
}

void SimulationController::setSettings()
{
    this->actualSimulation->setIntegerParameter(&this->simulationSettings.integerParameter);
    this->actualSimulation->setDoubleParameter(&this->simulationSettings.doubleParameter);
    this->actualSimulation->setStringParameter(&this->simulationSettings.stringParameter);
}

void SimulationController::setSettingItem(QString key, double value)
{
    this->simulationSettings.doubleParameter.insert(key,value);
}

void SimulationController::setSettingItem(QString key, int value)
{
    this->simulationSettings.integerParameter.insert(key,value);
}

void SimulationController::setSettingItem(QString key, QString value)
{
    this->simulationSettings.stringParameter.insert(key,value);
}

QMap<QString, int>* SimulationController::getIntegerParamter()
{
    return this->simulationSettings.defaultIntegerParameter;
}

QMap<QString, double>* SimulationController::getDoubleParamter()
{
    return this->simulationSettings.defaultDoubleParameter;
}

QMap<QString, QStringList>* SimulationController::getStringParamter()
{
    return this->simulationSettings.defaultStringParameter;
}

void SimulationController::setSimulationAt(int i)
{
    SimulationPlugin p = this->simulations.at(i);

    QString filePath = SystemDbManager::getPluginFilePath(p.name,p.pluginName);

    actualSimulation = PluginLoader::loadSimulationPlugin(filePath,p.name);
}
