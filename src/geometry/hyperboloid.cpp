#include "hyperboloid.h"

#include "featurewrapper.h"

/*!
 * \brief Hyperboloid::Hyperboloid
 * \param isNominal
 * \param parent
 */
Hyperboloid::Hyperboloid(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setHyperboloid(this);
    }

}

/*!
 * \brief Hyperboloid::Hyperboloid
 * \param isNominal
 * \param center
 * \param axis
 * \param a
 * \param c
 * \param parent
 */
Hyperboloid::Hyperboloid(const bool &isNominal, const Position &center, const Direction &axis, const double &a, const double &c, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setHyperboloid(this);
    }

    this->setHyperboloid(center, axis, a, c);

}

/*!
 * \brief Hyperboloid::Hyperboloid
 * \param copy
 * \param parent
 */
Hyperboloid::Hyperboloid(const Hyperboloid &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setHyperboloid(this);
    }

    this->center = copy.center;
    this->axis = copy.axis;
    this->a = copy.a;
    this->c = copy.c;

}

/*!
 * \brief Hyperboloid::operator =
 * \param copy
 * \return
 */
Hyperboloid &Hyperboloid::operator=(const Hyperboloid &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setHyperboloid(this);
    }

    this->center = copy.center;
    this->axis = copy.axis;
    this->a = copy.a;
    this->c = copy.c;

    return *this;

}

/*!
 * \brief Hyperboloid::~Hyperboloid
 */
Hyperboloid::~Hyperboloid(){

}

/*!
 * \brief Hyperboloid::hasDirection
 * \return
 */
bool Hyperboloid::hasDirection() const{
    return true;
}

/*!
 * \brief Hyperboloid::hasPosition
 * \return
 */
bool Hyperboloid::hasPosition() const{
    return true;
}

/*!
 * \brief Hyperboloid::getDirection
 * Returns the rotation axis of the hyperboloid
 * \return
 */
const Direction &Hyperboloid::getDirection() const{
    return this->axis;
}

/*!
 * \brief Hyperboloid::getPosition
 * Returns the center of the hyperboloid
 * \return
 */
const Position &Hyperboloid::getPosition() const{
    return this->center;
}

/*!
 * \brief Hyperboloid::getA
 * \return
 */
const double &Hyperboloid::getA() const{
    return this->a;
}

/*!
 * \brief Hyperboloid::getC
 * \return
 */
const double &Hyperboloid::getC() const{
    return this->c;
}

/*!
 * \brief Hyperboloid::setHyperboloid
 * \param center
 * \param axis
 * \param a
 * \param c
 */
void Hyperboloid::setHyperboloid(const Position &center, const Direction &axis, const double &a, const double &c){

    //set the given parameters
    this->center = center;
    this->axis = axis;
    this->a = a;
    this->c = c;

}

/*!
 * \brief Hyperboloid::recalc
 */
void Hyperboloid::recalc(){

    Geometry::recalc();

    //reset hyperboloid definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->center.setVector(0.0, 0.0, 0.0);
        this->axis.setVector(0.0, 0.0, 0.0);
        this->a = 0.0;
        this->c = 0.0;
    }

}

/*!
 * \brief Hyperboloid::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Hyperboloid::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement hyperboloid = Geometry::toOpenIndyXML(xmlDoc);

    if(hyperboloid.isNull()){
        return hyperboloid;
    }

    hyperboloid.setAttribute("type", getGeometryTypeName(eHyperboloidGeometry));

    //set a
    QDomElement a = xmlDoc.createElement("a");
    if(this->isSolved || this->isNominal){
        a.setAttribute("value", this->a);
    }else{
        a.setAttribute("value", 0.0);
    }
    hyperboloid.appendChild(a);

    //set c
    QDomElement c = xmlDoc.createElement("c");
    if(this->isSolved || this->isNominal){
        c.setAttribute("value", this->c);
    }else{
        c.setAttribute("value", 0.0);
    }
    hyperboloid.appendChild(c);

    return hyperboloid;

}

/*!
 * \brief Hyperboloid::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Hyperboloid::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set hyperboloid attributes
        QDomElement a = xmlElem.firstChildElement("a");
        QDomElement c = xmlElem.firstChildElement("c");
        QDomElement axis = xmlElem.firstChildElement("spatialDirection");
        QDomElement center = xmlElem.firstChildElement("coordinates");

        if(a.isNull() || c.isNull() || axis.isNull() || center.isNull()
                || !a.hasAttribute("value") || !c.hasAttribute("value") || axis.hasAttribute("i")
                || !axis.hasAttribute("j") || !axis.hasAttribute("k")
                || !center.hasAttribute("x") || !center.hasAttribute("y") || !center.hasAttribute("z")){
            return false;
        }

        this->a = a.attribute("value").toDouble();
        this->c = c.attribute("value").toDouble();
        this->axis.setVector(axis.attribute("i").toDouble(),
                             axis.attribute("j").toDouble(),
                             axis.attribute("k").toDouble());
        this->center.setVector(center.attribute("x").toDouble(),
                               center.attribute("y").toDouble(),
                               center.attribute("z").toDouble());

    }

    return result;

}

/*!
 * \brief Hyperboloid::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Hyperboloid::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Hyperboloid::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Hyperboloid::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Hyperboloid::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Hyperboloid::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(2), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Hyperboloid::getDisplayPrimaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString Hyperboloid::getDisplayPrimaryI(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->axis.getVector().getAt(0), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Hyperboloid::getDisplayPrimaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString Hyperboloid::getDisplayPrimaryJ(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->axis.getVector().getAt(1), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Hyperboloid::getDisplayPrimaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString Hyperboloid::getDisplayPrimaryK(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->axis.getVector().getAt(2), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Hyperboloid::getDisplayA
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Hyperboloid::getDisplayA(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->a, type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Hyperboloid::getDisplayC
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Hyperboloid::getDisplayC(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->c, type), 'f', digits);
    }
    return QString("");
}
