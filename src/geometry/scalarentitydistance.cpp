#include "scalarentitydistance.h"

#include "function.h"

ScalarEntityDistance::ScalarEntityDistance()
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = false;
}

/*!
 * \brief ScalarEntityDistance::ScalarEntityDistance
 * \param copy
 */
ScalarEntityDistance::ScalarEntityDistance(const ScalarEntityDistance &copy){
    this->id = copy.id;
    this->name = copy.name;
    this->distance = copy.getDistance();
    this->isSolved = copy.isSolved;
}

/*!
 * \brief ScalarEntityDistance::recalc
 * Execute alls functions in the specified order
 */
void ScalarEntityDistance::recalc(){
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

/*!
 * \brief ScalarEntityDistance::getDistance
 * \return
 */
double ScalarEntityDistance::getDistance() const{
    return this->distance;
}

/*!
 * \brief ScalarEntityDistance::setDistance
 * \param distance
 */
void ScalarEntityDistance::setDistance(double distance){
    this->distance = distance;
}

QString ScalarEntityDistance::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString ScalarEntityDistance::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}
QString ScalarEntityDistance::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}

QString ScalarEntityDistance::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString ScalarEntityDistance::getDisplayMConfig() const{
    return this->mConfig.name;
}

QString ScalarEntityDistance::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        return QString::number(this->myStatistic.stdev*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    }else{
        return "-/-";
    }
}

QString ScalarEntityDistance::getDisplayScalarDistanceValue() const{
    return QString::number(this->distance,'f',UnitConverter::distanceDigits);
}
