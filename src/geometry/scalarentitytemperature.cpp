#include "scalarentitytemperature.h"

#include "function.h"

ScalarEntityTemperature::ScalarEntityTemperature(bool isNominal, QObject *parent) : Geometry(isNominal, parent)
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = false;
    this->setTemperature(20.0);
}

ScalarEntityTemperature::ScalarEntityTemperature(const ScalarEntityTemperature &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->temperature = copy.getTemperature();
    this->isSolved = copy.isSolved;
}

void ScalarEntityTemperature::recalc(){

    if(this->functionList.size() > 0){

        bool solved = true;
        foreach(Function *f, this->functionList){

            //execute the function if it exists and if the last function was executed successfully
            if(f != NULL && solved == true){
                solved = f->exec(*this);
            }

        }
        this->setIsSolved(solved);

    }else if(this->isNominal == false){

        this->setIsSolved(false);

    }

}

bool ScalarEntityTemperature::toOpenIndyXML(QXmlStreamWriter &stream){

    return false;
}

ElementDependencies ScalarEntityTemperature::fromOpenIndyXML(QXmlStreamReader &xml){
    ElementDependencies dependencies;

    return dependencies;
}

bool ScalarEntityTemperature::saveSimulationData()
{
    this->myStatistic.simulationData.scalar.append(this->temperature);
    return true;
}

void ScalarEntityTemperature::setTemperature(double temperature){
    this->temperature = temperature;
}

double ScalarEntityTemperature::getTemperature() const{
    return this->temperature;
}

QString ScalarEntityTemperature::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString ScalarEntityTemperature::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}
/*QString ScalarEntityTemperature::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}*/

QString ScalarEntityTemperature::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString ScalarEntityTemperature::getDisplayMConfig() const{
    return this->mConfig.name;
}

QString ScalarEntityTemperature::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        return QString::number(this->myStatistic.stdev*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    }else{
        return "-/-";
    }
}

QString ScalarEntityTemperature::getDisplayScalarTemperatureValue() const{
    return QString::number(UnitConverter::getTemperature(this->temperature),'f',UnitConverter::temperatureDigits);
}
