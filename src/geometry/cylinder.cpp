#include "cylinder.h"

/*

#include "function.h"

MeasurementConfig Cylinder::defaultMeasurementConfig;

Cylinder::Cylinder(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4),ijk(4),radius(0.0)
{
    //this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

Cylinder::Cylinder(const Cylinder &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

OiVec Cylinder::getXYZ() const
{
    return this->xyz;
}

OiVec Cylinder::getIJK() const
{
    return this->ijk;
}

void Cylinder::recalc(){

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
        this->ijk = OiVec(4);
        this->radius = 0.0;
        this->setIsSolved(false);

    }

}

QDomElement Cylinder::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement cylinder = Geometry::toOpenIndyXML(xmlDoc);

    if(cylinder.isNull()){
        return cylinder;
    }

    cylinder.setAttribute("type", getGeometryTypeName(eCylinderGeometry));

    return cylinder;

}

bool Cylinder::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set cylinder attributes
        QDomElement radius = xmlElem.firstChildElement("radius");
        QDomElement directionVector = xmlElem.firstChildElement("spatialDirection");
        QDomElement axisPoint = xmlElem.firstChildElement("coordinates");

        if(radius.isNull() || directionVector.isNull() || axisPoint.isNull() || !radius.hasAttribute("value")
                || !directionVector.hasAttribute("i") || !directionVector.hasAttribute("j") || !directionVector.hasAttribute("k")
                || !axisPoint.hasAttribute("x") || !axisPoint.hasAttribute("y") || !axisPoint.hasAttribute("z")){
            return false;
        }

        this->radius = radius.attribute("value").toDouble();
        this->ijk.setAt(0, directionVector.attribute("i").toDouble());
        this->ijk.setAt(1, directionVector.attribute("j").toDouble());
        this->ijk.setAt(2, directionVector.attribute("k").toDouble());
        this->ijk.setAt(3, 1.0);
        this->xyz.setAt(0, axisPoint.attribute("x").toDouble());
        this->xyz.setAt(1, axisPoint.attribute("y").toDouble());
        this->xyz.setAt(2, axisPoint.attribute("z").toDouble());
        this->xyz.setAt(3, 1.0);

    }

    return result;

}

bool Cylinder::saveSimulationData()
{
    return false;
}
*/

/*!
 * \brief Cylinder::Cylinder
 * \param isNominal
 * \param parent
 */
Cylinder::Cylinder(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

}

/*!
 * \brief Cylinder::Cylinder
 * \param isNominal
 * \param axisPoint
 * \param axis
 * \param radius
 * \param parent
 */
Cylinder::Cylinder(const bool &isNominal, const Position &axisPoint, const Direction &axis, const Radius &radius, QObject *parent) : Geometry(isNominal, parent){
    this->setCylinder(axisPoint, axis, radius);
}

/*!
 * \brief Cylinder::Cylinder
 * \param copy
 * \param parent
 */
Cylinder::Cylinder(const Cylinder &copy, QObject *parent) : Geometry(copy, parent){

    this->axisPoint = copy.axisPoint;
    this->axis = copy.axis;
    this->radius = copy.radius;

}

/*!
 * \brief Cylinder::operator =
 * \param copy
 * \return
 */
Cylinder &Cylinder::operator=(const Cylinder &copy){

    this->axisPoint = copy.axisPoint;
    this->axis = copy.axis;
    this->radius = copy.radius;

    return *this;

}

/*!
 * \brief Cylinder::~Cylinder
 */
Cylinder::~Cylinder(){

}

/*!
 * \brief Cylinder::hasDirection
 * \return
 */
bool Cylinder::hasDirection() const{
    return true;
}

/*!
 * \brief Cylinder::hasPosition
 * \return
 */
bool Cylinder::hasPosition() const{
    return true;
}

/*!
 * \brief Cylinder::hasRadius
 * \return
 */
bool Cylinder::hasRadius() const{
    return true;
}

/*!
 * \brief Cylinder::getRadius
 * Returns the radius of the cylinder
 * \return
 */
const Radius &Cylinder::getRadius() const{
    return this->radius;
}

/*!
 * \brief Cylinder::getDirection
 * Returns the axis of the cylinder
 * \return
 */
const Direction &Cylinder::getDirection() const{
    return this->axis;
}

/*!
 * \brief Cylinder::getPosition
 * Returns an arbitrary point on the cylinder axis
 * \return
 */
const Position &Cylinder::getPosition() const{
    return this->axisPoint;
}

/*!
 * \brief Cylinder::setCylinder
 * \param axisPoint
 * \param axis
 * \param radius
 */
void Cylinder::setCylinder(const Position &axisPoint, const Direction &axis, const Radius &radius){

    //set the given parameters
    this->axisPoint = axisPoint;
    this->axis = axis;
    this->radius = radius;

}

/*!
 * \brief Cylinder::recalc
 */
void Cylinder::recalc(){

    Geometry::recalc();

    //reset cylinder definition if not solved
    if(!this->isSolved){
        this->axisPoint.setVector(0.0, 0.0, 0.0);
        this->axis.setVector(0.0, 0.0, 0.0);
        this->radius.setRadius(0.0);
    }

}

/*!
 * \brief Cylinder::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Cylinder::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement cylinder = Geometry::toOpenIndyXML(xmlDoc);

    if(cylinder.isNull()){
        return cylinder;
    }

    cylinder.setAttribute("type", getGeometryTypeName(eCylinderGeometry));

    return cylinder;

}

/*!
 * \brief Cylinder::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Cylinder::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set cylinder attributes
        QDomElement radius = xmlElem.firstChildElement("radius");
        QDomElement directionVector = xmlElem.firstChildElement("spatialDirection");
        QDomElement axisPoint = xmlElem.firstChildElement("coordinates");

        if(radius.isNull() || directionVector.isNull() || axisPoint.isNull() || !radius.hasAttribute("value")
                || !directionVector.hasAttribute("i") || !directionVector.hasAttribute("j") || !directionVector.hasAttribute("k")
                || !axisPoint.hasAttribute("x") || !axisPoint.hasAttribute("y") || !axisPoint.hasAttribute("z")){
            return false;
        }

        this->radius = radius.attribute("value").toDouble();
        this->ijk.setAt(0, directionVector.attribute("i").toDouble());
        this->ijk.setAt(1, directionVector.attribute("j").toDouble());
        this->ijk.setAt(2, directionVector.attribute("k").toDouble());
        this->ijk.setAt(3, 1.0);
        this->xyz.setAt(0, axisPoint.attribute("x").toDouble());
        this->xyz.setAt(1, axisPoint.attribute("y").toDouble());
        this->xyz.setAt(2, axisPoint.attribute("z").toDouble());
        this->xyz.setAt(3, 1.0);

    }

    return result;

}
