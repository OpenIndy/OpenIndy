#include "scalarentitymeasurementseries.h"

#include "function.h"

ScalarEntityMeasurementSeries::ScalarEntityMeasurementSeries(bool isNominal, QObject *parent) : Geometry(isNominal, parent)
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = false;
}

ScalarEntityMeasurementSeries::ScalarEntityMeasurementSeries(const ScalarEntityMeasurementSeries &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->seriesValue = copy.getSeriesValue();
    this->isSolved = copy.isSolved;
}

void ScalarEntityMeasurementSeries::recalc(){


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

bool ScalarEntityMeasurementSeries::toOpenIndyXML(QXmlStreamWriter &stream){

    return false;
}

ElementDependencies ScalarEntityMeasurementSeries::fromOpenIndyXML(QXmlStreamReader &xml){
    ElementDependencies dependencies;

    return dependencies;
}

bool ScalarEntityMeasurementSeries::saveSimulationData()
{
    this->myStatistic.addSimulationScalar(this->seriesValue);
    return true;
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
/*QString ScalarEntityMeasurementSeries::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}*/

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
