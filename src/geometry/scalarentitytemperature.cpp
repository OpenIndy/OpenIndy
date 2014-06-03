#include "scalarentitytemperature.h"

#include "function.h"

ScalarEntityTemperature::ScalarEntityTemperature()
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = false;
    this->setTemperature(20.0);
}

ScalarEntityTemperature::ScalarEntityTemperature(const ScalarEntityTemperature &copy){
    this->id = copy.id;
    this->name = copy.name;
    this->temperature = copy.getTemperature();
    this->isSolved = copy.isSolved;
}

void ScalarEntityTemperature::recalc(){

    bool isDefined = false;
    foreach(Function *f, this->functionList){
        if(!isDefined){
            //this->isSolved = f->exec(*this);
            isDefined = true;
        }else if(this->isSolved){
            //this->isSolved = f->exec(*this);
        }
    }
}

bool ScalarEntityTemperature::toOpenIndyXML(QXmlStreamWriter &stream){

    return false;
}

ElementDependencies ScalarEntityTemperature::fromOpenIndyXML(QXmlStreamReader &xml){
    ElementDependencies dependencies;

    return dependencies;
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
