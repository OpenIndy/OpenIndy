#include "trafoparam.h"

#include "coordinatesystem.h"
#include "statistic.h"
#include "function.h"

TrafoParam::TrafoParam() : homogenMatrix(4, 4), translation(3), rotation(3), scale(3), quaternion(4)
{
    this->id = Configuration::generateID();
    this->isUpdated = false;
}

TrafoParam::~TrafoParam(){

    //delete this trafo set in from system
    if(this->from != NULL){
        this->from->trafoParams.removeOne(this);
    }

    //delete this trafo set in to system
    if(this->to != NULL){
        this->to->trafoParams.removeOne(this);
    }

}

/*!
 * \brief TrafoParam::recalc
 */
void TrafoParam::recalc(){
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
}
/*!
 * \brief set up the homogenious matrix for transformation.
 * This function is called, if the user sets some values for transformation by hand via the transformation parameter
 * input dialog.
 */
void TrafoParam::generateHomogenMatrix()
{
    OiMat tmpTranslation(4,4);
    OiMat tmpRotation(4,4);
    OiMat tmpScale(4,4);

    tmpTranslation.setAt(0,0,1.0);
    tmpTranslation.setAt(0,1,0.0);
    tmpTranslation.setAt(0,2,0.0);
    tmpTranslation.setAt(0,3,this->translation.getAt(0));
    tmpTranslation.setAt(1,0,0.0);
    tmpTranslation.setAt(1,1,1.0);
    tmpTranslation.setAt(1,2,0.0);
    tmpTranslation.setAt(1,3,this->translation.getAt(1));
    tmpTranslation.setAt(2,0,0.0);
    tmpTranslation.setAt(2,1,0.0);
    tmpTranslation.setAt(2,2,1.0);
    tmpTranslation.setAt(2,3,this->translation.getAt(2));
    tmpTranslation.setAt(3,0,0.0);
    tmpTranslation.setAt(3,1,0.0);
    tmpTranslation.setAt(3,2,0.0);
    tmpTranslation.setAt(3,3,1.0);

    tmpScale.setAt(0,0,this->scale.getAt(0));
    tmpScale.setAt(0,1,0.0);
    tmpScale.setAt(0,2,0.0);
    tmpScale.setAt(0,3,0.0);
    tmpScale.setAt(1,0,0.0);
    tmpScale.setAt(1,1,this->scale.getAt(1));
    tmpScale.setAt(1,2,0.0);
    tmpScale.setAt(1,3,0.0);
    tmpScale.setAt(2,0,0.0);
    tmpScale.setAt(2,1,0.0);
    tmpScale.setAt(2,2,this->scale.getAt(2));
    tmpScale.setAt(2,3,0.0);
    tmpScale.setAt(3,0,0.0);
    tmpScale.setAt(3,1,0.0);
    tmpScale.setAt(3,2,0.0);
    tmpScale.setAt(3,3,1.0);

    tmpRotation.setAt(0,0,qCos(this->rotation.getAt(1))*qCos(this->rotation.getAt(2)));
    tmpRotation.setAt(0,1,qCos(this->rotation.getAt(0))*qSin(this->rotation.getAt(2))+qSin(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qCos(this->rotation.getAt(2)));
    tmpRotation.setAt(0,2,qSin(this->rotation.getAt(0))*qSin(this->rotation.getAt(2))-qCos(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qCos(this->rotation.getAt(2)));
    tmpRotation.setAt(0,3,0.0);
    tmpRotation.setAt(1,0,-qCos(this->rotation.getAt(1))*qSin(this->rotation.getAt(2)));
    tmpRotation.setAt(1,1,qCos(this->rotation.getAt(0))*qCos(this->rotation.getAt(2))-qSin(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qSin(this->rotation.getAt(2)));
    tmpRotation.setAt(1,2,qSin(this->rotation.getAt(0))*qCos(this->rotation.getAt(2))+qCos(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qSin(this->rotation.getAt(2)));
    tmpRotation.setAt(1,3,0.0);
    tmpRotation.setAt(2,0,qSin(this->rotation.getAt(1)));
    tmpRotation.setAt(2,1,-qSin(this->rotation.getAt(0))*qCos(this->rotation.getAt(2)));
    tmpRotation.setAt(2,2,qCos(this->rotation.getAt(0))*qCos(this->rotation.getAt(1)));
    tmpRotation.setAt(2,3,0.0);
    tmpRotation.setAt(3,0,0.0);
    tmpRotation.setAt(3,1,0.0);
    tmpRotation.setAt(3,2,0.0);
    tmpRotation.setAt(3,3,1.0);

    this->homogenMatrix = tmpTranslation*tmpScale*tmpRotation;
}

QString TrafoParam::getDisplayStartSystem() const{
    return this->from->name;
}

QString TrafoParam::getDisplayDestinationSystem() const{
    return this->to->name;
}

QString TrafoParam::getDisplayTranslationX() const
{
    return QString::number(this->translation.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString TrafoParam::getDisplayTranslationY() const
{
    return QString::number(this->translation.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString TrafoParam::getDisplayTranslationZ() const
{
    return QString::number(this->translation.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString TrafoParam::getDisplayRotationX() const
{
    return QString::number(this->rotation.getAt(0)*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits);
}

QString TrafoParam::getDisplayRotationY() const
{
    return QString::number(this->rotation.getAt(1)*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits);
}

QString TrafoParam::getDisplayRotationZ() const
{
    return QString::number(this->rotation.getAt(2)*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits);
}

QString TrafoParam::getDisplayScaleX() const
{
    return QString::number(this->scale.getAt(0),'f',6);
}

QString TrafoParam::getDisplayScaleY() const
{
    return QString::number(this->scale.getAt(1),'f',6);
}

QString TrafoParam::getDisplayScaleZ() const
{
    return QString::number(this->scale.getAt(2),'f',6);
}
