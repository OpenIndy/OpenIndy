#include "plane.h"

#include "featurewrapper.h"
/*
#include "function.h"

MeasurementConfig Plane::defaultMeasurementConfig;

Plane::Plane(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4), ijk(4)
{
    //this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

Plane::Plane(const Plane &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->xyz = copy.xyz;
    this->ijk = copy.ijk;
    this->isSolved = copy.isSolved;
}

Plane::~Plane(){

}

OiVec Plane::getXYZ() const
{
    return this->xyz;
}

OiVec Plane::getIJK() const
{
    return this->ijk;
}

void Plane::recalc(){

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
        this->setIsSolved(false);

    }

}

QDomElement Plane::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement plane = Geometry::toOpenIndyXML(xmlDoc);

    if(plane.isNull()){
        return plane;
    }

    plane.setAttribute("type", getGeometryTypeName(ePlaneGeometry));

    //add normal vector
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
    plane.appendChild(ijk);

    return plane;

}

bool Plane::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set plane attributes
        QDomElement normalVector = xmlElem.firstChildElement("spatialDirection");
        QDomElement pointOnPlane = xmlElem.firstChildElement("coordinates");

        if(normalVector.isNull() || pointOnPlane.isNull()
                || !normalVector.hasAttribute("i") || !normalVector.hasAttribute("j") || !normalVector.hasAttribute("k")
                || !pointOnPlane.hasAttribute("x") || !pointOnPlane.hasAttribute("y") || !pointOnPlane.hasAttribute("z")){
            return false;
        }

        this->ijk.setAt(0, normalVector.attribute("i").toDouble());
        this->ijk.setAt(1, normalVector.attribute("j").toDouble());
        this->ijk.setAt(2, normalVector.attribute("k").toDouble());
        this->ijk.setAt(3, 1.0);
        this->xyz.setAt(0, pointOnPlane.attribute("x").toDouble());
        this->xyz.setAt(1, pointOnPlane.attribute("y").toDouble());
        this->xyz.setAt(2, pointOnPlane.attribute("z").toDouble());
        this->xyz.setAt(3, 1.0);

    }

    return result;

}

bool Plane::saveSimulationData()
{
    this->mySimulationData.addXYZ(this->xyz.getAt(0),this->xyz.getAt(1),this->xyz.getAt(2));

    this->mySimulationData.addIJK(this->ijk.getAt(0),this->ijk.getAt(1),this->ijk.getAt(2));
    return true;
}

QString Plane::getDisplayX(bool showDiff) const{

    QString value = QString::number(this->xyz.getAt(0)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);

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

    return value;
    return "";

}

QString Plane::getDisplayY(bool showDiff) const{

    QString value = QString::number(this->xyz.getAt(1)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);

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

    return value;
    return "";
}

QString Plane::getDisplayZ(bool showDiff) const{
    QString value = QString::number(this->xyz.getAt(2)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);

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

    return value;
    return "";
}

QString Plane::getDisplayI(bool showDiff) const{
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
    return "";
}

QString Plane::getDisplayJ(bool showDiff) const{
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
    return "";
}

QString Plane::getDisplayK(bool showDiff) const{
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

QString Plane::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString Plane::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}

QString Plane::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}

QString Plane::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString Plane::getDisplayMConfig() const{
    return this->activeMeasurementConfig.getDisplayName();
}

QString Plane::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        //return QString::number(this->myStatistic.stdev*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
    }
    return "-/-";

}
*/


/*!
 * \brief Plane::Plane
 * \param isNominal
 * \param parent
 */
Plane::Plane(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPlane(this);
    }

}

/*!
 * \brief Plane::Plane
 * \param isNominal
 * \param xyz
 * \param normal
 * \param parent
 */
Plane::Plane(const bool &isNominal, const Position &xyz, const Direction &normal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPlane(this);
    }

    this->setPlane(xyz, normal);

}

/*!
 * \brief Plane::Plane
 * \param isNominal
 * \param normal
 * \param d
 * \param parent
 */
Plane::Plane(const bool &isNominal, const Direction &normal, const double &d, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPlane(this);
    }

    this->setPlane(normal, d);

}

/*!
 * \brief Plane::Plane
 * \param copy
 * \param parent
 */
Plane::Plane(const Plane &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPlane(this);
    }

    this->xyz = copy.xyz;
    this->normal = copy.normal;
    this->d = copy.d;

}

/*!
 * \brief Plane::operator =
 * \param copy
 * \return
 */
Plane &Plane::operator=(const Plane &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPlane(this);
    }

    this->xyz = copy.xyz;
    this->normal = copy.normal;
    this->d = copy.d;

    return *this;

}

/*!
 * \brief Plane::~Plane
 */
Plane::~Plane(){

}

/*!
 * \brief Plane::hasDirection
 * \return
 */
bool Plane::hasDirection() const{
    return true;
}

/*!
 * \brief Plane::hasPosition
 * \return
 */
bool Plane::hasPosition() const{
    return true;
}

/*!
 * \brief Plane::getDirection
 * Returns the normal vector of the plane
 * \return
 */
const Direction &Plane::getDirection() const{
    return this->normal;
}

/*!
 * \brief Plane::getPosition
 * Returns a point on the plane
 * \return
 */
const Position &Plane::getPosition() const{
    return this->xyz;
}

/*!
 * \brief Plane::getDistanceFromOrigin
 * Returns the smallest vertical distance of the plane from the origin
 * \return
 */
const double &Plane::getDistanceFromOrigin() const{
    return this->d;
}

/*!
 * \brief Plane::setPlane
 * \param xyz
 * \param normal
 */
void Plane::setPlane(const Position &xyz, const Direction &normal){

    //set the given parameters
    this->xyz;
    this->normal = normal;

    //calculate the other parameters
    OiVec n0 = normal.getVector();
    n0.normalize();
    OiVec::dot(this->d, this->xyz.getVector(), n0);

}

/*!
 * \brief Plane::setPlane
 * \param normal
 * \param d
 */
void Plane::setPlane(const Direction &normal, const double &d){

    //set the given parameters
    this->normal = normal;
    this->d = d;

    //calculate the other parameters
    this->xyz.setVector(this->d * this->normal.getVector());

}

/*!
 * \brief Plane::recalc
 */
void Plane::recalc(){

    Geometry::recalc();

    //reset plane definition if not solved
    if(!this->isSolved){
        this->xyz.setVector(0.0, 0.0, 0.0);
        this->normal.setVector(0.0, 0.0, 0.0);
        this->d = 0.0;
    }

}

/*!
 * \brief Plane::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Plane::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement plane = Geometry::toOpenIndyXML(xmlDoc);
/*
    if(plane.isNull()){
        return plane;
    }

    plane.setAttribute("type", getGeometryTypeName(ePlaneGeometry));

    //add normal vector
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
    plane.appendChild(ijk);
*/
    return plane;

}

/*!
 * \brief Plane::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Plane::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){
/*
        //set plane attributes
        QDomElement normalVector = xmlElem.firstChildElement("spatialDirection");
        QDomElement pointOnPlane = xmlElem.firstChildElement("coordinates");

        if(normalVector.isNull() || pointOnPlane.isNull()
                || !normalVector.hasAttribute("i") || !normalVector.hasAttribute("j") || !normalVector.hasAttribute("k")
                || !pointOnPlane.hasAttribute("x") || !pointOnPlane.hasAttribute("y") || !pointOnPlane.hasAttribute("z")){
            return false;
        }

        this->ijk.setAt(0, normalVector.attribute("i").toDouble());
        this->ijk.setAt(1, normalVector.attribute("j").toDouble());
        this->ijk.setAt(2, normalVector.attribute("k").toDouble());
        this->ijk.setAt(3, 1.0);
        this->xyz.setAt(0, pointOnPlane.attribute("x").toDouble());
        this->xyz.setAt(1, pointOnPlane.attribute("y").toDouble());
        this->xyz.setAt(2, pointOnPlane.attribute("z").toDouble());
        this->xyz.setAt(3, 1.0);
*/
    }

    return result;

}
