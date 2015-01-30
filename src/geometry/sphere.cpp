#include "sphere.h"

#include "function.h"

MeasurementConfig Sphere::defaultMeasurementConfig;

Sphere::Sphere(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4), radius(0.0)
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
Sphere::Sphere(const Sphere &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->xyz = copy.xyz;
    this->radius = copy.radius;
    this->isSolved = copy.isSolved;
}

Sphere::~Sphere(){

}

/*!
 * \brief Sphere::getXYZ returns the xyz vector
 * \return
 */
OiVec Sphere::getXYZ() const
{
    return this->xyz;
}

double Sphere::getRadius() const
{
    return this->radius;
}

/*!
 * \brief Sphere::recalc
 * Execute alls functions in the specified order
 */
void Sphere::recalc(){

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
        this->setIsSolved(false);

    }

}

/*!
 * \brief Sphere::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Sphere::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement sphere = Geometry::toOpenIndyXML(xmlDoc);

    if(sphere.isNull()){
        return sphere;
    }

    sphere.setAttribute("type", Configuration::sSphere);

    QDomElement radius = xmlDoc.createElement("radius");
    if(this->getIsSolved() || this->getIsNominal()){
        radius.setAttribute("value", this->radius);
    }else{
        radius.setAttribute("value", 0.0);
    }
    sphere.appendChild(radius);

    return sphere;

}

/*!
 * \brief Sphere::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Sphere::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set circle attributes
        QDomElement radius = xmlElem.firstChildElement("radius");
        QDomElement center = xmlElem.firstChildElement("coordinates");

        if(radius.isNull() || center.isNull() || !radius.hasAttribute("value")
                || !center.hasAttribute("x") || !center.hasAttribute("y") || !center.hasAttribute("z")){
            return false;
        }

        this->radius = radius.attribute("value").toDouble();
        this->xyz.setAt(0, center.attribute("x").toDouble());
        this->xyz.setAt(1, center.attribute("y").toDouble());
        this->xyz.setAt(2, center.attribute("z").toDouble());
        this->xyz.setAt(3, 1.0);

    }

    return result;

}

bool Sphere::saveSimulationData()
{


    this->mySimulationData.addXYZ(this->xyz.getAt(0),this->xyz.getAt(1),this->xyz.getAt(2));

    this->mySimulationData.addRadius(this->radius);

    return true;
}

QString Sphere::getDisplayX(bool showDiff) const{

    QString value = QString::number(this->xyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   QString diff = QString::number((this->xyz.getAt(0)-g->getXYZ().getAt(0))*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;

}

QString Sphere::getDisplayY(bool showDiff) const{

    QString value = QString::number(this->xyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   QString diff = QString::number((this->xyz.getAt(1)-g->getXYZ().getAt(1))*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;
}

QString Sphere::getDisplayZ(bool showDiff) const{
    QString value = QString::number(this->xyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   QString diff = QString::number((this->xyz.getAt(2)-g->getXYZ().getAt(2))*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;
}

QString Sphere::getDisplayRadius(bool showDiff) const{

    QString value = QString::number(this->radius*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
            foreach (Geometry *g, this->nominals) {
                if(g != NULL && g->getIsSolved()){

                    QString diff = QString::number((this->radius - g->getRadius())*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                    return QString(value + " (" + diff + ")" );
                }
            }
        }
    }

    return value;
}

QString Sphere::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}
/*
QString Sphere::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}*/
/*QString Sphere::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}*/

QString Sphere::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString Sphere::getDisplayMConfig() const{
    return this->activeMeasurementConfig.getDisplayName();
}

QString Sphere::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        return QString::number(this->myStatistic.stdev*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    }else{
        return "-/-";
    }
}
