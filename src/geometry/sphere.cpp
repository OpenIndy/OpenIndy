#include "sphere.h"

#include "featurewrapper.h"

/*!
 * \brief Sphere::Sphere
 * \param isNominal
 * \param parent
 */
Sphere::Sphere(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setSphere(this);
    }

}

/*!
 * \brief Sphere::Sphere
 * \param isNominal
 * \param center
 * \param radius
 * \param parent
 */
Sphere::Sphere(const bool &isNominal, const Position &center, const Radius &radius, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setSphere(this);
    }

    this->setSphere(center, radius);

}

/*!
 * \brief Sphere::Sphere
 * \param copy
 * \param parent
 */
Sphere::Sphere(const Sphere &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setSphere(this);
    }

    this->center = copy.center;
    this->radius = copy.radius;

}

/*!
 * \brief Sphere::operator =
 * \param copy
 * \return
 */
Sphere &Sphere::operator=(const Sphere &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setSphere(this);
    }

    this->center = copy.center;
    this->radius = copy.radius;

    return *this;

}

/*!
 * \brief Sphere::~Sphere
 */
Sphere::~Sphere(){

}

/*!
 * \brief Sphere::hasPosition
 * \return
 */
bool Sphere::hasPosition() const{
    return true;
}

/*!
 * \brief Sphere::hasRadius
 * \return
 */
bool Sphere::hasRadius() const{
    return true;
}

/*!
 * \brief Sphere::getRadius
 * Returns the radius of the sphere
 * \return
 */
const Radius &Sphere::getRadius() const{
    return this->radius;
}

/*!
 * \brief Sphere::getPosition
 * Returns the center of the sphere
 * \return
 */
const Position &Sphere::getPosition() const{
    return this->center;
}

/*!
 * \brief Sphere::setSphere
 * \param center
 * \param radius
 */
void Sphere::setSphere(const Position &center, const Radius &radius){

    //set the given parameters
    this->center = center;
    this->radius = radius;

}

/*!
 * \brief Sphere::recalc
 */
void Sphere::recalc(){

    Geometry::recalc();

    //reset sphere definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->center.setVector(0.0, 0.0, 0.0);
        this->radius.setRadius(0.0);
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

    sphere.setAttribute("type", getGeometryTypeName(eSphereGeometry));

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

        //set sphere attributes
        QDomElement radius = xmlElem.firstChildElement("radius");
        QDomElement center = xmlElem.firstChildElement("coordinates");

        if(radius.isNull() || center.isNull() || !radius.hasAttribute("value")
                || !center.hasAttribute("x") || !center.hasAttribute("y") || !center.hasAttribute("z")){
            return false;
        }

        this->radius.setRadius(radius.attribute("value").toDouble());
        this->center.setVector(center.attribute("x").toDouble(),
                               center.attribute("y").toDouble(),
                               center.attribute("z").toDouble());

    }

    return result;

}

/*!
 * \brief Sphere::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Sphere::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Sphere::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Sphere::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Sphere::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Sphere::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(2), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Sphere::getDisplayRadiusA
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Sphere::getDisplayRadiusA(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->radius.getRadius(), type), 'f', digits);
    }
    return QString("");
}
