#include "sphere.h"

#include "function.h"

Sphere::Sphere() : xyz(4), radius(0.0)
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief Sphere::Sphere
 * \param copy
 */
Sphere::Sphere(const Sphere &copy){
    this->id = copy.id;
    this->name = copy.name;
    this->xyz = copy.xyz;
    this->radius = copy.radius;
    this->isSolved = copy.isSolved;
}

Sphere::~Sphere(){

}

/*!
 * \brief Sphere::recalc
 * Execute alls functions in the specified order
 */
void Sphere::recalc(){
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
        this->radius = 0.0;
    }
}

QString Sphere::getDisplayX() const{
    return QString::number(this->xyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Sphere::getDisplayY() const{
    return QString::number(this->xyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Sphere::getDisplayZ() const{
    return QString::number(this->xyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Sphere::getDisplayRadius() const{
    return QString::number(this->radius*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Sphere::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString Sphere::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}
QString Sphere::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}

QString Sphere::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString Sphere::getDisplayMConfig() const{
    return this->mConfig.name;
}

QString Sphere::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        return QString::number(this->myStatistic.stdev*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    }else{
        return "-/-";
    }
}
