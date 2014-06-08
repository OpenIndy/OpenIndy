#include "circle.h"

#include "function.h"

Circle::Circle(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4), ijk(4), radius(0.0)
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief Circle::Circle
 * \param copy
 */
Circle::Circle(const Circle &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

/*!
 * \brief Circle::getXYZ returns xyz vector
 * \return
 */
OiVec Circle::getXYZ() const{
    return this->xyz;
}

/*!
 * \brief Circle::getIJK returns ijk vector
 * \return
 */
OiVec Circle::getIJK() const
{
    return this->ijk;
}

/*!
 * \brief Circle::recalc
 */
void Circle::recalc(){

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

        this->xyz = OiVec(4);
        this->radius = 0.0;
        this->dist2origin = 0.0;
        this->setIsSolved(false);

    }

}


bool Circle::toOpenIndyXML(QXmlStreamWriter &stream){

    return false;
}

ElementDependencies Circle::fromOpenIndyXML(QXmlStreamReader &xml){

    ElementDependencies dependencies;

    return dependencies;
}

QString Circle::getDisplayX() const{
    return QString::number(this->xyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Circle::getDisplayY() const{
    return QString::number(this->xyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Circle::getDisplayZ() const{
    return QString::number(this->xyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Circle::getDisplayRadius() const{
    return QString::number(this->radius*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Circle::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString Circle::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}
/*QString Circle::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}*/

QString Circle::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString Circle::getDisplayMConfig() const{
    return this->mConfig.name;
}

QString Circle::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        return QString::number(this->myStatistic.stdev*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    }else{
        return "-/-";
    }
}

QString Circle::getDisplayI() const{
    return QString::number(this->ijk.getAt(0),'f',6);
}

QString Circle::getDisplayJ() const{
    return QString::number(this->ijk.getAt(1),'f',6);
}

QString Circle::getDisplayK() const{
    return QString::number(this->ijk.getAt(2),'f',6);
}
