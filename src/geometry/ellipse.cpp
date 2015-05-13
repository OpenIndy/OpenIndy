#include "ellipse.h"

#include "featurewrapper.h"

/*!
 * \brief Ellipse::Ellipse
 * \param isNominal
 * \param parent
 */
Ellipse::Ellipse(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setEllipse(this);
    }

}

/*!
 * \brief Ellipse::Ellipse
 * \param isNominal
 * \param center
 * \param normal
 * \param a
 * \param b
 * \param semiMajorAxis
 * \param parent
 */
Ellipse::Ellipse(const bool &isNominal, const Position &center, const Direction &normal, const double &a, const double &b, const Direction &semiMajorAxis, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setEllipse(this);
    }

    this->setEllipse(center, normal, a, b, semiMajorAxis);

}

/*!
 * \brief Ellipse::Ellipse
 * \param copy
 * \param parent
 */
Ellipse::Ellipse(const Ellipse &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setEllipse(this);
    }

    this->center = copy.center;
    this->normal = copy.normal;
    this->a = copy.a;
    this->b = copy.b;
    this->semiMajorAxis = copy.semiMajorAxis;

}

/*!
 * \brief Ellipse::operator =
 * \param copy
 * \return
 */
Ellipse &Ellipse::operator=(const Ellipse &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setEllipse(this);
    }

    this->center = copy.center;
    this->normal = copy.normal;
    this->a = copy.a;
    this->b = copy.b;
    this->semiMajorAxis = copy.semiMajorAxis;

    return *this;

}

/*!
 * \brief Ellipse::~Ellipse
 */
Ellipse::~Ellipse(){

}

/*!
 * \brief Ellipse::hasDirection
 * \return
 */
bool Ellipse::hasDirection() const{
    return true;
}

/*!
 * \brief Ellipse::hasPosition
 * \return
 */
bool Ellipse::hasPosition() const{
    return true;
}

/*!
 * \brief Ellipse::getDirection
 * Returns the normal vector of the ellipse
 * \return
 */
const Direction &Ellipse::getDirection() const{
    return this->normal;
}

/*!
 * \brief Ellipse::getPosition
 * Returns the center of the ellipse
 * \return
 */
const Position &Ellipse::getPosition() const{
    return this->center;
}

/*!
 * \brief Ellipse::getA
 * Returns the semi-major axis of the ellipse
 * \return
 */
const double &Ellipse::getA() const{
    return this->a;
}

/*!
 * \brief Ellipse::getB
 * Returns the semi-minor axis of the ellipse
 * \return
 */
const double &Ellipse::getB() const{
    return this->b;
}

/*!
 * \brief Ellipse::getSemiMajorAxisDirection
 * Returns the direction of the semi-major axis
 * \return
 */
const Direction &Ellipse::getSemiMajorAxisDirection() const{
    return this->semiMajorAxis;
}

/*!
 * \brief Ellipse::setEllipse
 * \param center
 * \param normal
 * \param a
 * \param b
 * \param semiMajorAxis
 */
void Ellipse::setEllipse(const Position &center, const Direction &normal, const double &a, const double &b, const Direction &semiMajorAxis){

    //set the given parameters
    this->center = center;
    this->normal = normal;
    this->a = a;
    this->b = b;
    this->semiMajorAxis = semiMajorAxis;

}

/*!
 * \brief Ellipse::recalc
 */
void Ellipse::recalc(){

    Geometry::recalc();

    //reset ellipse definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->center.setVector(0.0, 0.0, 0.0);
        this->normal.setVector(0.0, 0.0, 0.0);
        this->a = 0.0;
        this->b = 0.0;
        this->semiMajorAxis.setVector(0.0, 0.0, 0.0);
    }

}

/*!
 * \brief Ellipse::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Ellipse::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement ellipse = Geometry::toOpenIndyXML(xmlDoc);

    if(ellipse.isNull()){
        return ellipse;
    }

    ellipse.setAttribute("type", getGeometryTypeName(eEllipseGeometry));

    //set semi-major axis length
    QDomElement a = xmlDoc.createElement("a");
    if(this->isSolved || this->isNominal){
        a.setAttribute("value", this->a);
    }else{
        a.setAttribute("value", 0.0);
    }
    ellipse.appendChild(a);

    //set semi-minor axis length
    QDomElement b = xmlDoc.createElement("b");
    if(this->isSolved || this->isNominal){
        b.setAttribute("value", this->b);
    }else{
        b.setAttribute("value", 0.0);
    }
    ellipse.appendChild(b);

    //set semi-major axis direction
    QDomElement semiMajorAxis = xmlDoc.createElement("semiMajorAxis");
    if(this->isSolved || this->isNominal){
        semiMajorAxis.setAttribute("i", this->semiMajorAxis.getVector().getAt(0));
        semiMajorAxis.setAttribute("j", this->semiMajorAxis.getVector().getAt(1));
        semiMajorAxis.setAttribute("k", this->semiMajorAxis.getVector().getAt(2));
    }else{
        semiMajorAxis.setAttribute("i", 0.0);
        semiMajorAxis.setAttribute("j", 0.0);
        semiMajorAxis.setAttribute("k", 0.0);
    }
    ellipse.appendChild(semiMajorAxis);

    return ellipse;

}

/*!
 * \brief Ellipse::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Ellipse::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set ellipse attributes
        QDomElement a = xmlElem.firstChildElement("a");
        QDomElement b = xmlElem.firstChildElement("b");
        QDomElement semiMajorAxis = xmlElem.firstChildElement("semiMajorAxis");
        QDomElement normal = xmlElem.firstChildElement("spatialDirection");
        QDomElement center = xmlElem.firstChildElement("coordinates");

        if(a.isNull() || b.isNull() || semiMajorAxis.isNull() || normal.isNull() || center.isNull()
                || !a.hasAttribute("value") || !b.hasAttribute("value") || semiMajorAxis.hasAttribute("i")
                || !semiMajorAxis.hasAttribute("j") || !semiMajorAxis.hasAttribute("k")
                || !normal.hasAttribute("i") || normal.hasAttribute("j") || normal.hasAttribute("k")
                || !center.hasAttribute("x") || !center.hasAttribute("y") || !center.hasAttribute("z")){
            return false;
        }

        this->a = a.attribute("value").toDouble();
        this->b = b.attribute("value").toDouble();
        this->semiMajorAxis.setVector(semiMajorAxis.attribute("i").toDouble(),
                                      semiMajorAxis.attribute("j").toDouble(),
                                      semiMajorAxis.attribute("k").toDouble());
        this->normal.setVector(normal.attribute("i").toDouble(),
                               normal.attribute("j").toDouble(),
                               normal.attribute("k").toDouble());
        this->center.setVector(center.attribute("x").toDouble(),
                               center.attribute("y").toDouble(),
                               center.attribute("z").toDouble());

    }

    return result;

}

/*!
 * \brief Ellipse::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Ellipse::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Ellipse::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Ellipse::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Ellipse::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Ellipse::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(2), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Ellipse::getDisplayPrimaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString Ellipse::getDisplayPrimaryI(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(0), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Ellipse::getDisplayPrimaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString Ellipse::getDisplayPrimaryJ(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(1), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Ellipse::getDisplayPrimaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString Ellipse::getDisplayPrimaryK(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(2), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Ellipse::getDisplaySecondaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString Ellipse::getDisplaySecondaryI(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->semiMajorAxis.getVector().getAt(0), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Ellipse::getDisplaySecondaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString Ellipse::getDisplaySecondaryJ(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->semiMajorAxis.getVector().getAt(1), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Ellipse::getDisplaySecondaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString Ellipse::getDisplaySecondaryK(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->semiMajorAxis.getVector().getAt(2), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Ellipse::getDisplayA
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Ellipse::getDisplayA(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->a, type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Ellipse::getDisplayB
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Ellipse::getDisplayB(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->b, type), 'f', digits);
    }
    return QString("");
}
