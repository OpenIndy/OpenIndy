#include "scalarentitymeasurementseries.h"

ScalarEntityMeasurementSeries::ScalarEntityMeasurementSeries()
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = false;
}

ScalarEntityMeasurementSeries::ScalarEntityMeasurementSeries(const ScalarEntityMeasurementSeries &copy){
    this->id = copy.id;
    this->name = copy.name;
    this->seriesValue = copy.getSeriesValue();
    this->isSolved = copy.isSolved;
}

void ScalarEntityMeasurementSeries::recalc(){
    /*
     * isDefined -> becomes true as soon as the first function of a feature has been executed, which defines the feature
     * isSolved -> is true as long as there isn't any function which cannot be successfully executed
     */
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

bool ScalarEntityMeasurementSeries::toOpenIndyXML(QXmlStreamWriter &stream){

    return false;
}

ElementDependencies ScalarEntityMeasurementSeries::fromOpenIndyXML(QXmlStreamReader &xml){
    ElementDependencies dependencies;

    return dependencies;
}

double ScalarEntityMeasurementSeries::getSeriesValue() const{
    return this->seriesValue;
}

void ScalarEntityMeasurementSeries::setSeriesValue(double value){
    this->seriesValue = value;
}

QString ScalarEntityMeasurementSeries::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString ScalarEntityMeasurementSeries::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}
QString ScalarEntityMeasurementSeries::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}

QString ScalarEntityMeasurementSeries::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString ScalarEntityMeasurementSeries::getDisplayMConfig() const{
    return this->mConfig.name;
}

QString ScalarEntityMeasurementSeries::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        return QString::number(this->myStatistic.stdev*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    }else{
        return "-/-";
    }
}

QString ScalarEntityMeasurementSeries::getDisplayScalarMeasurementSeriesValue() const{
    return QString::number(this->seriesValue,'f',6);
}
