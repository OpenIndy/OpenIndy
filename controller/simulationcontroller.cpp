#include "simulationcontroller.h"

SimulationController::SimulationController(QObject *parent) :
    QObject(parent)
{

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



    for(int i = 0; i <1001; i++){

    foreach(Station *s, OiFeatureState::getStations()){
        foreach(Observation *o, s->coordSys->getObservations()){
            OiMat A;
            o->myReading->restoreBackup();

            if(i < 1000){
            actualSimulation->distort(o->myReading,A);
            }

            o->myOriginalXyz = Reading::toCartesian(o->myReading->rPolar.azimuth,o->myReading->rPolar.zenith,o->myReading->rPolar.distance);
            o->myXyz = Reading::toCartesian(o->myReading->rPolar.azimuth,o->myReading->rPolar.zenith,o->myReading->rPolar.distance);
        }
      }



        myUpdater->recalcAll();

        if(i < 1000){
            foreach(FeatureWrapper *f, OiFeatureState::getFeatures()){
                if(f->getGeometry() != NULL){
                    f->getGeometry()->saveSimulationData();
                }
            }
        }

        emit counter(i);
    }



    /*// safe readings/obs in a QMap
    //logic for shuffling the observations
    //overwrite observations/readings

    for(int i = 0; i <1000; i++){


    //FeatureUpdater::recalcAll();


    //reset obs/readings to orignal
    //recalc all*/
}

void SimulationController::setSimulationAt(int i)
{
    SimulationPlugin p = this->simulations.at(i);

    QString filePath = SystemDbManager::getPluginFilePath(p.name,p.pluginName);

    actualSimulation = PluginLoader::loadSimulationPlugin(filePath,p.name);
}
