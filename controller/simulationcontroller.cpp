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
    /*this->simulations = SystemDbManager::getAvailableSimulationPlugins();

    QStringList simulationNames;

    foreach(SimulationPlugin p, simulations){
        simulationNames.append(p.name);
    }

    this->availableSimulations->setStringList(simulationNames);*/
}

void SimulationController::recalcAll()
{

    Uncertainties u;

    u.sensorUncertainties = sensorErrorModel->getErrors();
    u.enviromentUncertainties =environmentErrorModel->getErrors();
    u.objectUncertainties = objectErrorModel->getErrors();
    u.humanUncertainties = humanErrorModel->getErrors();
    actualSimulation->setGivenUncertainties(u);

    foreach(FeatureWrapper *f, OiJob::getFeatures()){
        if(f->getGeometry() != NULL){
            f->getGeometry()->resetSimulationData();
        }
    }

    QMap<Station*,OiMat> matrices;
    foreach(Station *s, OiJob::getStations()){
       OiMat A = myUpdater->trafoControl.getTransformationMatrix(s->coordSys);
       if(A.getRowCount()==4 && A.getColCount() == 4){
           matrices.insert(s,A);
       }
    }

    bool newIteration = true;

    for(int i = 0; i <(this->iterations+1); i++){

        newIteration = true;

        foreach(Station *s, OiJob::getStations()){
            foreach(Observation *o, s->coordSys->getObservations()){

                o->myReading->restoreBackup();

                if(i < this->iterations){
                    actualSimulation->distort(o->myReading,matrices.value(s), newIteration);
                    newIteration = false;
                }

                o->myOriginalXyz = Reading::toCartesian(o->myReading->rPolar.azimuth,o->myReading->rPolar.zenith,o->myReading->rPolar.distance);
                o->myXyz = Reading::toCartesian(o->myReading->rPolar.azimuth,o->myReading->rPolar.zenith,o->myReading->rPolar.distance);
            }
        }

        myUpdater->recalcAll();

        if(i < this->iterations){
            foreach(FeatureWrapper *f, OiJob::getFeatures()){
                if(f->getGeometry() != NULL){
                    f->getGeometry()->saveSimulationData();
                }
            }
        }

        emit counter(i);
    }

    foreach(FeatureWrapper *f, OiJob::getFeatures()){
        if(f->getPoint() != NULL){
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyX);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyY);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyZ);
        }else if(f->getPlane() != NULL){
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyX);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyY);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyZ);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyI);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyJ);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyK);
        }else if(f->getLine() != NULL){
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyX);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyY);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyZ);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyI);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyJ);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyK);
        }else if(f->getCircle() != NULL){
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyX);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyY);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyZ);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyI);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyJ);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyK);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyRadius);
        }else if(f->getSphere() != NULL){
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyX);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyY);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyZ);
            actualSimulation->analyseSimulationData(f->getGeometry()->getSimulationData().uncertaintyRadius);
        }

        this->setUpCorrelations(f);
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

void SimulationController::setUpCorrelations(FeatureWrapper *f)
{
    if(f->getGeometry() == NULL){
        return;
    }

    if(f->getGeometry()->getSimulationData().uncertaintyX.values.size() >0){
        double rxy = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyX.values,f->getGeometry()->getSimulationData().uncertaintyY.values);
        double rxz = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyX.values,f->getGeometry()->getSimulationData().uncertaintyZ.values);
        double ryz = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyY.values,f->getGeometry()->getSimulationData().uncertaintyZ.values);

        f->getGeometry()->getSimulationData().correlations.insert("rxy",rxy);
        f->getGeometry()->getSimulationData().correlations.insert("rxz",rxz);
        f->getGeometry()->getSimulationData().correlations.insert("ryz",ryz);
    }

    if(f->getGeometry()->getSimulationData().uncertaintyI.values.size() > 0){
        double rxi = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyX.values,f->getGeometry()->getSimulationData().uncertaintyI.values);
        double rxj = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyX.values,f->getGeometry()->getSimulationData().uncertaintyJ.values);
        double rxk = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyX.values,f->getGeometry()->getSimulationData().uncertaintyK.values);
        double ryi = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyY.values,f->getGeometry()->getSimulationData().uncertaintyI.values);
        double ryj = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyY.values,f->getGeometry()->getSimulationData().uncertaintyJ.values);
        double ryk = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyY.values,f->getGeometry()->getSimulationData().uncertaintyK.values);
        double rzi = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyZ.values,f->getGeometry()->getSimulationData().uncertaintyI.values);
        double rzj = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyZ.values,f->getGeometry()->getSimulationData().uncertaintyJ.values);
        double rzk = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyZ.values,f->getGeometry()->getSimulationData().uncertaintyK.values);
        double rij = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyI.values,f->getGeometry()->getSimulationData().uncertaintyJ.values);
        double rik = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyI.values,f->getGeometry()->getSimulationData().uncertaintyK.values);
        double rjk = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyJ.values,f->getGeometry()->getSimulationData().uncertaintyK.values);

        f->getGeometry()->getSimulationData().correlations.insert("rxi",rxi);
        f->getGeometry()->getSimulationData().correlations.insert("rxj",rxj);
        f->getGeometry()->getSimulationData().correlations.insert("rxk",rxk);
        f->getGeometry()->getSimulationData().correlations.insert("ryi",ryi);
        f->getGeometry()->getSimulationData().correlations.insert("ryj",ryj);
        f->getGeometry()->getSimulationData().correlations.insert("ryk",ryk);
        f->getGeometry()->getSimulationData().correlations.insert("rzi",rzi);
        f->getGeometry()->getSimulationData().correlations.insert("rzj",rzj);
        f->getGeometry()->getSimulationData().correlations.insert("rzk",rzk);
        f->getGeometry()->getSimulationData().correlations.insert("rij",rij);
        f->getGeometry()->getSimulationData().correlations.insert("rik",rik);
        f->getGeometry()->getSimulationData().correlations.insert("rjk",rjk);
    }

    if(f->getGeometry()->getSimulationData().uncertaintyRadius.values.size() > 0){
        double rxRadius = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyX.values,f->getGeometry()->getSimulationData().uncertaintyRadius.values);
        double ryRadius = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyY.values,f->getGeometry()->getSimulationData().uncertaintyRadius.values);
        double rzRadius = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyZ.values,f->getGeometry()->getSimulationData().uncertaintyRadius.values);

        f->getGeometry()->getSimulationData().correlations.insert("rxRadius",rxRadius);
        f->getGeometry()->getSimulationData().correlations.insert("ryRadius",ryRadius);
        f->getGeometry()->getSimulationData().correlations.insert("rzRadius",rzRadius);

        if(f->getGeometry()->getSimulationData().uncertaintyI.values.size() > 0){
                double riRadius = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyI.values,f->getGeometry()->getSimulationData().uncertaintyRadius.values);
                double rjRadius = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyJ.values,f->getGeometry()->getSimulationData().uncertaintyRadius.values);
                double rkRadius = actualSimulation->getCorrelationCoefficient(f->getGeometry()->getSimulationData().uncertaintyK.values,f->getGeometry()->getSimulationData().uncertaintyRadius.values);
                f->getGeometry()->getSimulationData().correlations.insert("riRadius",riRadius);
                f->getGeometry()->getSimulationData().correlations.insert("rjRadius",rjRadius);
                f->getGeometry()->getSimulationData().correlations.insert("rkRadius",rkRadius);
        }
    }


}

void SimulationController::setSimulationAt(int i)
{
    /*SimulationPlugin p = this->simulations.at(i);

    QString filePath = SystemDbManager::getPluginFilePath(p.name,p.pluginName);

    actualSimulation = PluginLoader::loadSimulationPlugin(filePath,p.name);*/
}
