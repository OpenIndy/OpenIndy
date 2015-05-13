#include "point.h"

#include "featurewrapper.h"

/*!
 * \brief Point::Point
 * \param isNominal
 * \param parent
 */
Point::Point(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPoint(this);
    }

}

/*!
 * \brief Point::Point
 * \param isNominal
 * \param xyz
 * \param parent
 */
Point::Point(const bool &isNominal, const Position &xyz, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPoint(this);
    }

    this->setPoint(xyz);

}

/*!
 * \brief Point::Point
 * \param copy
 * \param parent
 */
Point::Point(const Point &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPoint(this);
    }

    this->xyz = copy.xyz;

}

/*!
 * \brief Point::operator =
 * \param copy
 * \return
 */
Point &Point::operator=(const Point &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPoint(this);
    }

    this->xyz = copy.xyz;

    return *this;

}

/*!
 * \brief Point::~Point
 */
Point::~Point(){

}

/*!
 * \brief Point::hasPosition
 * \return
 */
bool Point::hasPosition() const{
    return true;
}

/*!
 * \brief Point::getPosition
 * Returns the point's position
 * \return
 */
const Position &Point::getPosition() const{
    return this->xyz;
}

/*!
 * \brief Point::setPoint
 * \param xyz
 */
void Point::setPoint(const Position &xyz){

    //set the given parameters
    this->xyz = xyz;

}

/*!
 * \brief Point::recalc
 */
void Point::recalc(){

    Geometry::recalc();

    //reset point definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->xyz.setVector(0.0, 0.0, 0.0);
    }

}

/*!
 * \brief Point::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Point::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement point = Geometry::toOpenIndyXML(xmlDoc);

    if(point.isNull()){
        return point;
    }

    point.setAttribute("type", getGeometryTypeName(ePointGeometry));

    return point;

}

/*!
 * \brief Point::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Point::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set point attributes
        QDomElement coordinates = xmlElem.firstChildElement("coordinates");

        if(coordinates.isNull() || !coordinates.hasAttribute("x") || !coordinates.hasAttribute("y") || !coordinates.hasAttribute("z")){
            return false;
        }

        this->xyz.setVector(coordinates.attribute("x").toDouble(),
                            coordinates.attribute("y").toDouble(),
                            coordinates.attribute("z").toDouble());

    }

    return result;

}

/*!
 * \brief Point::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Point::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->xyz.getVector().getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Point::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Point::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->xyz.getVector().getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Point::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Point::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->xyz.getVector().getAt(2), type), 'f', digits);
    }
    return QString("");
}
