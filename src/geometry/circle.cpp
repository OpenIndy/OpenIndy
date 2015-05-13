#include "circle.h"

#include "featurewrapper.h"

/*!
 * \brief Circle::Circle
 * \param isNominal
 * \param parent
 */
Circle::Circle(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCircle(this);
    }

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

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCircle(this);
    }

    this->setCircle(center, normal, radius);

}

/*!
 * \brief Circle::Circle
 * \param copy
 * \param parent
 */
Circle::Circle(const Circle &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCircle(this);
    }

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

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCircle(this);
    }

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

    //reset circle definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
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

    QDomElement circle = Geometry::toOpenIndyXML(xmlDoc);

    if(circle.isNull()){
        return circle;
    }

    circle.setAttribute("type", getGeometryTypeName(eCircleGeometry));

    return circle;

}

/*!
 * \brief Circle::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
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

        this->radius.setRadius(radius.attribute("value").toDouble());
        this->normal.setVector(normalVector.attribute("i").toDouble(),
                               normalVector.attribute("j").toDouble(),
                               normalVector.attribute("k").toDouble());
        this->center.setVector(center.attribute("x").toDouble(),
                               center.attribute("y").toDouble(),
                               center.attribute("z").toDouble());

    }

    return result;

}

/*!
 * \brief Circle::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Circle::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Circle::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Circle::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Circle::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Circle::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(2), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Circle::getDisplayPrimaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString Circle::getDisplayPrimaryI(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(0), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Circle::getDisplayPrimaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString Circle::getDisplayPrimaryJ(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(1), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Circle::getDisplayPrimaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString Circle::getDisplayPrimaryK(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(2), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Circle::getDisplayRadiusA
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Circle::getDisplayRadiusA(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->radius.getRadius(), type), 'f', digits);
    }
    return QString("");
}
