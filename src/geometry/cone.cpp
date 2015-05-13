#include "cone.h"

#include "featurewrapper.h"

/*!
 * \brief Cone::Cone
 * \param isNominal
 * \param parent
 */
Cone::Cone(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCone(this);
    }

}

/*!
 * \brief Cone::Cone
 * \param isNominal
 * \param apex
 * \param axis
 * \param aperture
 * \param parent
 */
Cone::Cone(const bool &isNominal, const Position &apex, const Direction &axis, const double &aperture, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCone(this);
    }

    this->setCone(apex, axis, aperture);

}

/*!
 * \brief Cone::Cone
 * \param copy
 * \param parent
 */
Cone::Cone(const Cone &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCone(this);
    }

    this->apex = copy.apex;
    this->axis = copy.axis;
    this->aperture = copy.aperture;

}

/*!
 * \brief Cone::operator =
 * \param copy
 * \return
 */
Cone &Cone::operator=(const Cone &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCone(this);
    }

    this->apex = copy.apex;
    this->axis = copy.axis;
    this->aperture = copy.aperture;

    return *this;

}

/*!
 * \brief Cone::~Cone
 */
Cone::~Cone(){

}

/*!
 * \brief Cone::hasDirection
 * \return
 */
bool Cone::hasDirection() const{
    return true;
}

/*!
 * \brief Cone::hasPosition
 * \return
 */
bool Cone::hasPosition() const{
    return true;
}

/*!
 * \brief Cone::getDirection
 * Returns the vector pointing from the apex inside the cone
 * \return
 */
const Direction &Cone::getDirection() const{
    return this->axis;
}

/*!
 * \brief Cone::getPosition
 * Returns the apex of the cone
 * \return
 */
const Position &Cone::getPosition() const{
    return this->apex;
}

/*!
 * \brief Cone::getAperture
 * Returns the aperture of the cone
 * \return
 */
const double &Cone::getAperture() const{
    return this->aperture;
}

/*!
 * \brief Cone::setCone
 * \param apex
 * \param axis
 * \param aperture
 */
void Cone::setCone(const Position &apex, const Direction &axis, const double &aperture){

    //set the given parameters
    this->apex = apex;
    this->axis = axis;
    this->aperture = aperture;

}

/*!
 * \brief Cone::recalc
 */
void Cone::recalc(){

    Geometry::recalc();

    //reset cone definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->apex.setVector(0.0, 0.0, 0.0);
        this->axis.setVector(0.0, 0.0, 0.0);
        this->aperture = 0.0;
    }

}

/*!
 * \brief Cone::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Cone::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement cone = Geometry::toOpenIndyXML(xmlDoc);

    if(cone.isNull()){
        return cone;
    }

    cone.setAttribute("type", getGeometryTypeName(eConeGeometry));

    //set aperture
    QDomElement aperture = xmlDoc.createElement("aperture");
    if(this->isSolved || this->isNominal){
        aperture.setAttribute("value", this->aperture);
    }else{
        aperture.setAttribute("value", 0.0);
    }
    cone.appendChild(aperture);

    return cone;

}

/*!
 * \brief Cone::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Cone::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set cone attributes
        QDomElement aperture = xmlElem.firstChildElement("aperture");
        QDomElement axis = xmlElem.firstChildElement("spatialDirection");
        QDomElement center = xmlElem.firstChildElement("coordinates");

        if(aperture.isNull() || axis.isNull() || center.isNull() || !aperture.hasAttribute("value")
                || !axis.hasAttribute("i") || !axis.hasAttribute("j") || !axis.hasAttribute("k")
                || !center.hasAttribute("x") || !center.hasAttribute("y") || !center.hasAttribute("z")){
            return false;
        }

        this->aperture = aperture.attribute("value").toDouble();
        this->axis.setVector(axis.attribute("i").toDouble(),
                             axis.attribute("j").toDouble(),
                             axis.attribute("k").toDouble());
        this->apex.setVector(center.attribute("x").toDouble(),
                             center.attribute("y").toDouble(),
                             center.attribute("z").toDouble());

    }

    return result;

}

/*!
 * \brief Cone::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Cone::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->apex.getVector().getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Cone::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Cone::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->apex.getVector().getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Cone::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Cone::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->apex.getVector().getAt(2), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Cone::getDisplayPrimaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString Cone::getDisplayPrimaryI(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->axis.getVector().getAt(0), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Cone::getDisplayPrimaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString Cone::getDisplayPrimaryJ(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->axis.getVector().getAt(1), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Cone::getDisplayPrimaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString Cone::getDisplayPrimaryK(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->axis.getVector().getAt(2), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Cone::getDisplayAperture
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Cone::getDisplayAperture(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->aperture, type), 'f', digits);
    }
    return QString("");
}
