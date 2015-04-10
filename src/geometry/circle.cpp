#include "circle.h"



/*
#include "function.h"

MeasurementConfig Circle::defaultMeasurementConfig;

Circle::Circle(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4), ijk(4), radius(0.0)
{
    //this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

Circle::Circle(const Circle &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

OiVec Circle::getXYZ() const{
    return this->xyz;
}

OiVec Circle::getIJK() const
{
    return this->ijk;
}

double Circle::getRadius() const
{
    return this->radius;
}


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

QDomElement Circle::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement circle = Geometry::toOpenIndyXML(xmlDoc);

    if(circle.isNull()){
        return circle;
    }

    circle.setAttribute("type", getGeometryTypeName(eCircleGeometry));

    //add radius
    QDomElement radius = xmlDoc.createElement("radius");
    if(this->getIsSolved() || this->getIsNominal()){
        radius.setAttribute("value", this->radius);
    }else{
        radius.setAttribute("value", 0.0);
    }
    circle.appendChild(radius);

    //add normal vector of the circle
    QDomElement ijk = xmlDoc.createElement("spatialDirection");
    if(this->ijk.getSize() >= 3 && (this->getIsSolved() || this->getIsNominal())){
        ijk.setAttribute("i", this->ijk.getAt(0));
        ijk.setAttribute("j", this->ijk.getAt(1));
        ijk.setAttribute("k", this->ijk.getAt(2));
    }else{
        ijk.setAttribute("i", 0.0);
        ijk.setAttribute("j", 0.0);
        ijk.setAttribute("k", 0.0);
    }
    circle.appendChild(ijk);

    return circle;

}

bool Circle::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set circle attributes
        QDomElement radius = xmlElem.firstChildElement("radius");
        QDomElement normalVector = xmlElem.firstChildElement("spatialDirection");
        QDomElement center = xmlElem.firstChildElement("coordinates");

        if(radius.isNull() || normalVector.isNull() || center.isNull() || !radius.hasAttribute("value")
                || !normalVector.hasAttribute("i") || !normalVector.hasAttribute("j") || !normalVector.hasAttribute("k")
                || !center.hasAttribute("x") || !center.hasAttribute("y") || !center.hasAttribute("z")){
            return false;
        }

        this->radius = radius.attribute("value").toDouble();
        this->ijk.setAt(0, normalVector.attribute("i").toDouble());
        this->ijk.setAt(1, normalVector.attribute("j").toDouble());
        this->ijk.setAt(2, normalVector.attribute("k").toDouble());
        this->ijk.setAt(3, 1.0);
        this->xyz.setAt(0, center.attribute("x").toDouble());
        this->xyz.setAt(1, center.attribute("y").toDouble());
        this->xyz.setAt(2, center.attribute("z").toDouble());
        this->xyz.setAt(3, 1.0);

    }

    return result;

}*/

//QString Circle::getDisplayX(bool showDiff) const{

    /*QString value = QString::number(this->xyz.getAt(0)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   QString diff = QString::number((this->xyz.getAt(0)-g->getXYZ().getAt(0))*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;*/
    //return "";

//}

//QString Circle::getDisplayY(bool showDiff) const{

    /*QString value = QString::number(this->xyz.getAt(1)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   QString diff = QString::number((this->xyz.getAt(1)-g->getXYZ().getAt(1))*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;*/
    //return "";
//}

//QString Circle::getDisplayZ(bool showDiff) const{
    /*QString value = QString::number(this->xyz.getAt(2)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   QString diff = QString::number((this->xyz.getAt(2)-g->getXYZ().getAt(2))*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;*/
    //return "";
//}

//QString Circle::getDisplayRadius(bool showDiff) const{
    //return QString::number(this->radius*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);

    /*QString value = QString::number(this->radius*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
            foreach (Geometry *g, this->nominals) {
                if(g != NULL && g->getIsSolved()){

                    QString diff = QString::number((this->radius - g->getRadius())*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
                    return QString(value + " (" + diff + ")" );
                }
            }
        }
    }

    return value;*/
    //return "";
//}
/*
QString Circle::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}*/
/*
QString Circle::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}*/
/*QString Circle::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}*/
/*
QString Circle::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString Circle::getDisplayMConfig() const{
    return this->activeMeasurementConfig.getDisplayName();
}

QString Circle::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        //return QString::number(this->myStatistic.stdev*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
    }
    return "-/-";
}

QString Circle::getDisplayI(bool showDiff) const{
    QString value = QString::number(this->ijk.getAt(0),'f',6);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
            foreach (Geometry *g, this->nominals) {
                if(g != NULL && g->getIsSolved()){

                    QString diff = QString::number((this->ijk.getAt(0)-g->getIJK().getAt(0)),'f',6);
                    return QString(value + " (" + diff + ")");
                }
            }
        }
    }

    return value;
}

QString Circle::getDisplayJ(bool showDiff) const{
    QString value = QString::number(this->ijk.getAt(1),'f',6);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
            foreach (Geometry *g, this->nominals) {
                if(g != NULL && g->getIsSolved()){

                    QString diff = QString::number((this->ijk.getAt(1)-g->getIJK().getAt(1)),'f',6);
                    return QString(value + " (" + diff + ")");
                }
            }
        }
    }

    return value;
}

QString Circle::getDisplayK(bool showDiff) const{
    QString value = QString::number(this->ijk.getAt(2),'f',6);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
            foreach (Geometry *g, this->nominals) {
                if(g != NULL && g->getIsSolved()){

                    QString diff = QString::number((this->ijk.getAt(2)-g->getIJK().getAt(2)),'f',6);
                    return QString(value + " (" + diff + ")");
                }
            }
        }
    }

    return value;
}

bool Circle::saveSimulationData()
{
    this->mySimulationData.addXYZ(this->xyz.getAt(0),this->xyz.getAt(1),this->xyz.getAt(2));

    this->mySimulationData.addIJK(this->ijk.getAt(0),this->ijk.getAt(1),this->ijk.getAt(2));

    this->mySimulationData.addRadius(this->radius);

    return true;
}
*/

/*!
 * \brief Circle::Circle
 * \param isNominal
 * \param parent
 */
Circle::Circle(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

}

/*!
 * \brief Circle::Circle
 * \param isNominal
 * \param center
 * \param normal
 * \param radius
 * \param parent
 */
Circle::Circle(const bool &isNominal, const Position &center, const Direction &normal, const Radius &radius, QObject *parent) : Geometry(isNominal, parent){
    this->setCircle(center, normal, radius);
}

/*!
 * \brief Circle::Circle
 * \param copy
 * \param parent
 */
Circle::Circle(const Circle &copy, QObject *parent) : Geometry(copy, parent){

    this->center = copy.center;
    this->radius = copy.radius;
    this->normal = copy.normal;

}

/*!
 * \brief Circle::operator =
 * \param copy
 * \return
 */
Circle &Circle::operator=(const Circle &copy){

    this->center = copy.center;
    this->radius = copy.radius;
    this->normal = copy.normal;

    return *this;

}

Circle::~Circle()
{

}

/*!
 * \brief Circle::hasDirection
 * \return
 */
bool Circle::hasDirection() const{
    return true;
}

/*!
 * \brief Circle::hasPosition
 * \return
 */
bool Circle::hasPosition() const{
    return true;
}

/*!
 * \brief Circle::hasRadius
 * \return
 */
bool Circle::hasRadius() const{
    return true;
}

/*!
 * \brief Circle::getRadius
 * Returns the radius of the circle
 * \return
 */
const Radius &Circle::getRadius() const{
    return this->radius;
}

/*!
 * \brief Circle::getDirection
 * Returns the normal vector of the circle
 * \return
 */
const Direction &Circle::getDirection() const{
    return this->normal;
}

/*!
 * \brief Circle::getPosition
 * Returns the center of the circle
 * \return
 */
const Position &Circle::getPosition() const{
    return this->center;
}

/*!
 * \brief Circle::setCircle
 * \param center
 * \param normal
 * \param radius
 */
void Circle::setCircle(const Position &center, const Direction &normal, const Radius &radius){

    //set the given parameters
    this->center = center;
    this->normal = normal;
    this->radius = radius;

}

/*!
 * \brief Circle::recalc
 */
void Circle::recalc(){

    Geometry::recalc();

    //reset circle definition if not solved
    if(!this->isSolved){
        this->center.setVector(0.0, 0.0, 0.0);
        this->normal.setVector(0.0, 0.0, 0.0);
        this->radius.setRadius(0.0);
    }

}

/*!
 * \brief Circle::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Circle::toOpenIndyXML(QDomDocument &xmlDoc) const{

    /*QDomElement circle = Geometry::toOpenIndyXML(xmlDoc);

    if(circle.isNull()){
        return circle;
    }

    circle.setAttribute("type", getGeometryTypeName(eCircleGeometry));

    //add radius
    QDomElement radius = xmlDoc.createElement("radius");
    if(this->getIsSolved() || this->getIsNominal()){
        radius.setAttribute("value", this->radius);
    }else{
        radius.setAttribute("value", 0.0);
    }
    circle.appendChild(radius);

    //add normal vector of the circle
    QDomElement ijk = xmlDoc.createElement("spatialDirection");
    if(this->ijk.getSize() >= 3 && (this->getIsSolved() || this->getIsNominal())){
        ijk.setAttribute("i", this->ijk.getAt(0));
        ijk.setAttribute("j", this->ijk.getAt(1));
        ijk.setAttribute("k", this->ijk.getAt(2));
    }else{
        ijk.setAttribute("i", 0.0);
        ijk.setAttribute("j", 0.0);
        ijk.setAttribute("k", 0.0);
    }
    circle.appendChild(ijk);

    return circle;*/

}

/*!
 * \brief Circle::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Circle::fromOpenIndyXML(QDomElement &xmlElem){

    /*bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set circle attributes
        QDomElement radius = xmlElem.firstChildElement("radius");
        QDomElement normalVector = xmlElem.firstChildElement("spatialDirection");
        QDomElement center = xmlElem.firstChildElement("coordinates");

        if(radius.isNull() || normalVector.isNull() || center.isNull() || !radius.hasAttribute("value")
                || !normalVector.hasAttribute("i") || !normalVector.hasAttribute("j") || !normalVector.hasAttribute("k")
                || !center.hasAttribute("x") || !center.hasAttribute("y") || !center.hasAttribute("z")){
            return false;
        }

        this->radius = radius.attribute("value").toDouble();
        this->ijk.setAt(0, normalVector.attribute("i").toDouble());
        this->ijk.setAt(1, normalVector.attribute("j").toDouble());
        this->ijk.setAt(2, normalVector.attribute("k").toDouble());
        this->ijk.setAt(3, 1.0);
        this->center.setAt(0, center.attribute("x").toDouble());
        this->center.setAt(1, center.attribute("y").toDouble());
        this->center.setAt(2, center.attribute("z").toDouble());
        this->center.setAt(3, 1.0);

    }

    return result;*/

    return true;

}
