#include "paraboloid.h"

#include "featurewrapper.h"

/*!
 * \brief Paraboloid::Paraboloid
 * \param isNominal
 * \param parent
 */
Paraboloid::Paraboloid(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setParaboloid(this);
    }

}

/*!
 * \brief Paraboloid::Paraboloid
 * \param isNominal
 * \param apex
 * \param axis
 * \param a
 * \param parent
 */
Paraboloid::Paraboloid(const bool &isNominal, const Position &apex, const Direction &axis, const double &a, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setParaboloid(this);
    }

    this->setParaboloid(apex, axis, a);

}

/*!
 * \brief Paraboloid::Paraboloid
 * \param copy
 * \param parent
 */
Paraboloid::Paraboloid(const Paraboloid &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setParaboloid(this);
    }

    this->apex = copy.apex;
    this->axis = copy.axis;

}

/*!
 * \brief Paraboloid::operator =
 * \param copy
 * \return
 */
Paraboloid &Paraboloid::operator=(const Paraboloid &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setParaboloid(this);
    }

    this->apex = copy.apex;
    this->axis = copy.axis;

    return *this;

}

/*!
 * \brief Paraboloid::~Paraboloid
 */
Paraboloid::~Paraboloid(){

}

/*!
 * \brief Paraboloid::hasDirection
 * \return
 */
bool Paraboloid::hasDirection() const{
    return true;
}

/*!
 * \brief Paraboloid::hasPosition
 * \return
 */
bool Paraboloid::hasPosition() const{
    return true;
}

/*!
 * \brief Paraboloid::getDirection
 * Returns the axis vector that points from the apex inside the paraboloid (rotation axis)
 * \return
 */
const Direction &Paraboloid::getDirection() const{
    return this->axis;
}

/*!
 * \brief Paraboloid::getPosition
 * Returns the peak of the paraboloid
 * \return
 */
const Position &Paraboloid::getPosition() const{
    return this->apex;
}

/*!
 * \brief Paraboloid::getA
 * Returns the compression or streatch parameter
 * \return
 */
const double &Paraboloid::getA() const{
    return this->a;
}

/*!
 * \brief Paraboloid::setParaboloid
 * \param apex
 * \param axis
 * \param a
 */
void Paraboloid::setParaboloid(const Position &apex, const Direction &axis, const double &a){

    //set the given parameters
    this->apex = apex;
    this->axis = axis;
    this->a = a;

}

/*!
 * \brief Paraboloid::recalc
 */
void Paraboloid::recalc(){

    Geometry::recalc();

    //reset paraboloid definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->apex.setVector(0.0, 0.0, 0.0);
        this->axis.setVector(0.0, 0.0, 0.0);
        this->a = 0.0;
    }

}

/*!
 * \brief Paraboloid::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Paraboloid::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement paraboloid = Geometry::toOpenIndyXML(xmlDoc);

    if(paraboloid.isNull()){
        return paraboloid;
    }

    paraboloid.setAttribute("type", getGeometryTypeName(eParaboloidGeometry));

    //set a
    QDomElement a = xmlDoc.createElement("a");
    if(this->isSolved || this->isNominal){
        a.setAttribute("value", this->a);
    }else{
        a.setAttribute("value", 0.0);
    }
    paraboloid.appendChild(a);

    return paraboloid;

}

/*!
 * \brief Paraboloid::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Paraboloid::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set paraboloid attributes
        QDomElement a = xmlElem.firstChildElement("a");
        QDomElement axis = xmlElem.firstChildElement("spatialDirection");
        QDomElement apex = xmlElem.firstChildElement("coordinates");

        if(a.isNull() || axis.isNull() || apex.isNull() || !a.hasAttribute("value")
                || !axis.hasAttribute("i") || axis.hasAttribute("j") || axis.hasAttribute("k")
                || !apex.hasAttribute("x") || !apex.hasAttribute("y") || !apex.hasAttribute("z")){
            return false;
        }

        this->a = a.attribute("value").toDouble();
        this->axis.setVector(axis.attribute("i").toDouble(),
                               axis.attribute("j").toDouble(),
                               axis.attribute("k").toDouble());
        this->apex.setVector(apex.attribute("x").toDouble(),
                             apex.attribute("y").toDouble(),
                             apex.attribute("z").toDouble());

    }

    return result;

}

/*!
 * \brief Paraboloid::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Paraboloid::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->apex.getVector().getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Paraboloid::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Paraboloid::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->apex.getVector().getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Paraboloid::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Paraboloid::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->apex.getVector().getAt(2), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Paraboloid::getDisplayPrimaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString Paraboloid::getDisplayPrimaryI(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->axis.getVector().getAt(0), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Paraboloid::getDisplayPrimaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString Paraboloid::getDisplayPrimaryJ(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->axis.getVector().getAt(1), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Paraboloid::getDisplayPrimaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString Paraboloid::getDisplayPrimaryK(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->axis.getVector().getAt(2), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Paraboloid::getDisplayA
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Paraboloid::getDisplayA(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->a, type), 'f', digits);
    }
    return QString("");
}
