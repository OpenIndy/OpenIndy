#include "simulationcontroller.h"

SimulationController::SimulationController(QObject *parent) :
    QObject(parent)
{

    this->availableSimulations = new QStringListModel();

    errorTableModel = new SimulationErrorTableModel();


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

SimulationModel *SimulationController::getSimulationAt(int i)
{
    SimulationPlugin p = this->simulations.at(i);

    QString filePath = SystemDbManager::getPluginFilePath(p.name,p.pluginName);

    return PluginLoader::loadSimulationPlugin(filePath,p.name);
}
