#include "plane.h"

#include "function.h"

Plane::Plane() : xyz(4), ijk(4)
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief Plane::Plane
 * \param copy
 */
Plane::Plane(const Plane &copy){
    this->id = copy.id;
    this->name = copy.name;
    this->xyz = copy.xyz;
    this->ijk = copy.ijk;
    this->isSolved = copy.isSolved;
}

Plane::~Plane(){

}

/*!
 * \brief Plane::recalc
 * Execute alls functions in the specified order
 */
void Plane::recalc(){
    /*
     * isDefined -> becomes true as soon as the first function of a feature has been executed, which defines the feature
     * isSolved -> is true as long as there isn't any function which cannot be successfully executed
     */
    bool isDefined = false;
    foreach(Function *f, this->functionList){
        if(!isDefined){
            this->isSolved = f->exec(*this);
            isDefined = true;
        }else if(this->isSolved){
            this->isSolved = f->exec(*this);
        }
    }
    //if no function is set this feature cannot be solved and its coordinates are reset
    if(this->functionList.size() == 0 && this->isNominal == false){
        this->isSolved = false;
        this->xyz = OiVec(4);
        this->ijk = OiVec(4);
    }
}

QString Plane::getDisplayX() const{
    return QString::number(this->xyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Plane::getDisplayY() const{
    return QString::number(this->xyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Plane::getDisplayZ() const{
    return QString::number(this->xyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Plane::getDisplayI() const{
    return QString::number(this->ijk.getAt(0),'f',UnitConverter::distanceDigits);
}

QString Plane::getDisplayJ() const{
    return QString::number(this->ijk.getAt(1),'f',UnitConverter::distanceDigits);
}

QString Plane::getDisplayK() const{
    return QString::number(this->ijk.getAt(2),'f',UnitConverter::distanceDigits);
}

QString Plane::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString Plane::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}

QString Plane::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}

QString Plane::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString Plane::getDisplayMConfig() const{
    return this->mConfig.name;
}

QString Plane::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        return QString::number(this->myStatistic.stdev*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    }else{
        return "-/-";
    }
}
