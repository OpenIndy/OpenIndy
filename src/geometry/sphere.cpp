#include "sphere.h"

#include "featurewrapper.h"
/*
#include "function.h"

MeasurementConfig Sphere::defaultMeasurementConfig;

Sphere::Sphere(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4), radius(0.0)
{
    //this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

Sphere::Sphere(const Sphere &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->xyz = copy.xyz;
    this->radius = copy.radius;
    this->isSolved = copy.isSolved;
}

Sphere::~Sphere(){

}

OiVec Sphere::getXYZ() const
{
    return this->xyz;
}

double Sphere::getRadius() const
{
    return this->radius;
}

void Sphere::recalc(){

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
        this->setIsSolved(false);

    }

}

QDomElement Sphere::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement sphere = Geometry::toOpenIndyXML(xmlDoc);

    if(sphere.isNull()){
        return sphere;
    }

    sphere.setAttribute("type", getGeometryTypeName(eSphereGeometry));

    QDomElement radius = xmlDoc.createElement("radius");
    if(this->getIsSolved() || this->getIsNominal()){
        radius.setAttribute("value", this->radius);
    }else{
        radius.setAttribute("value", 0.0);
    }
    sphere.appendChild(radius);

    return sphere;

}

bool Sphere::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set circle attributes
        QDomElement radius = xmlElem.firstChildElement("radius");
        QDomElement center = xmlElem.firstChildElement("coordinates");

        if(radius.isNull() || center.isNull() || !radius.hasAttribute("value")
                || !center.hasAttribute("x") || !center.hasAttribute("y") || !center.hasAttribute("z")){
            return false;
        }

        this->radius = radius.attribute("value").toDouble();
        this->xyz.setAt(0, center.attribute("x").toDouble());
        this->xyz.setAt(1, center.attribute("y").toDouble());
        this->xyz.setAt(2, center.attribute("z").toDouble());
        this->xyz.setAt(3, 1.0);

    }

    return result;

}

bool Sphere::saveSimulationData()
{


    this->mySimulationData.addXYZ(this->xyz.getAt(0),this->xyz.getAt(1),this->xyz.getAt(2));

    this->mySimulationData.addRadius(this->radius);

    return true;
}

QString Sphere::getDisplayX(bool showDiff) const{

    double displayValue = this->xyz.getAt(0);
    this->convertMetricValue(displayValue);
    QString value = QString::number(displayValue, 'f', this->getMetricDigits());

    //QString value = QString::number(this->xyz.getAt(0)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   displayValue = this->xyz.getAt(0)-g->getXYZ().getAt(0);
                   this->convertMetricValue(displayValue);
                   QString diff = QString::number(displayValue, 'f', this->getMetricDigits());

                   //QString diff = QString::number((this->xyz.getAt(0)-g->getXYZ().getAt(0))*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;

}

QString Sphere::getDisplayY(bool showDiff) const{

    double displayValue = this->xyz.getAt(1);
    this->convertMetricValue(displayValue);
    QString value = QString::number(displayValue, 'f', this->getMetricDigits());

    //QString value = QString::number(this->xyz.getAt(1)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   displayValue = this->xyz.getAt(1)-g->getXYZ().getAt(1);
                   this->convertMetricValue(displayValue);
                   QString diff = QString::number(displayValue, 'f', this->getMetricDigits());

                   //QString diff = QString::number((this->xyz.getAt(1)-g->getXYZ().getAt(1))*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;
}

QString Sphere::getDisplayZ(bool showDiff) const{

    double displayValue = this->xyz.getAt(2);
    this->convertMetricValue(displayValue);
    QString value = QString::number(displayValue, 'f', this->getMetricDigits());

    //QString value = QString::number(this->xyz.getAt(2)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   displayValue = this->xyz.getAt(2)-g->getXYZ().getAt(2);
                   this->convertMetricValue(displayValue);
                   QString diff = QString::number(displayValue, 'f', this->getMetricDigits());

                   //QString diff = QString::number((this->xyz.getAt(2)-g->getXYZ().getAt(2))*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;
}

QString Sphere::getDisplayRadius(bool showDiff) const{

    double displayValue = this->radius;
    this->convertMetricValue(displayValue);
    QString value = QString::number(displayValue, 'f', this->getMetricDigits());


    //QString value = QString::number(this->radius*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
            foreach (Geometry *g, this->nominals) {
                if(g != NULL && g->getIsSolved()){

                    displayValue = this->radius - g->getRadius();
                    this->convertMetricValue(displayValue);
                    QString diff = QString::number(displayValue, 'f', this->getMetricDigits());

                    //QString diff = QString::number((this->radius - g->getRadius())*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
                    return QString(value + " (" + diff + ")" );
                }
            }
        }
    }

    return value;
}

QString Sphere::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString Sphere::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}
QString Sphere::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}

QString Sphere::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString Sphere::getDisplayMConfig() const{
    return this->activeMeasurementConfig.getDisplayName();
}

QString Sphere::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        //return QString::number(this->myStatistic.stdev*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
    }
    return "-/-";
}
*/

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

    //reset sphere definition if not solved
    if(!this->isSolved){
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
/*
    if(sphere.isNull()){
        return sphere;
    }

    sphere.setAttribute("type", getGeometryTypeName(eSphereGeometry));

    QDomElement radius = xmlDoc.createElement("radius");
    if(this->getIsSolved() || this->getIsNominal()){
        radius.setAttribute("value", this->radius);
    }else{
        radius.setAttribute("value", 0.0);
    }
    sphere.appendChild(radius);
*/
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
/*
        //set sphere attributes
        QDomElement radius = xmlElem.firstChildElement("radius");
        QDomElement center = xmlElem.firstChildElement("coordinates");

        if(radius.isNull() || center.isNull() || !radius.hasAttribute("value")
                || !center.hasAttribute("x") || !center.hasAttribute("y") || !center.hasAttribute("z")){
            return false;
        }

        this->radius = radius.attribute("value").toDouble();
        this->xyz.setAt(0, center.attribute("x").toDouble());
        this->xyz.setAt(1, center.attribute("y").toDouble());
        this->xyz.setAt(2, center.attribute("z").toDouble());
        this->xyz.setAt(3, 1.0);
*/
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
        return QString::number(convertToDefault(this->center.getVector().getAt(0), type), 'f', digits);
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
        return QString::number(convertToDefault(this->center.getVector().getAt(1), type), 'f', digits);
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
        return QString::number(convertToDefault(this->center.getVector().getAt(2), type), 'f', digits);
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
