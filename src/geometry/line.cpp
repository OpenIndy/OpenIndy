#include "line.h"

#include "featurewrapper.h"
/*
#include "function.h"

MeasurementConfig Line::defaultMeasurementConfig;

Line::Line(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4), ijk(4)
{
    //this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

Line::Line(const Line &copy) : Geometry(copy.isNominal) {
    this->id = copy.id;
    this->name = copy.name;
    this->xyz = copy.xyz;
    this->ijk = copy.ijk;
    this->isSolved = copy.isSolved;
}

Line::~Line(){

}

OiVec Line::getXYZ() const
{
    return this->xyz;
}

OiVec Line::getIJK() const
{
    return this->ijk;
}

void Line::recalc(){

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

QDomElement Line::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement line = Geometry::toOpenIndyXML(xmlDoc);

    if(line.isNull()){
        return line;
    }

    line.setAttribute("type", getGeometryTypeName(eLineGeometry));

    //add vector of direction
    QDomElement ijk = xmlDoc.createElement("spatialDirection");
    if(this->ijk.getSize() >= 3 && (this->getIsSolved() || this->getIsNominal())){
        ijk.setAttribute("i", this->ijk.getAt(0));
        ijk.setAttribute("j", this->ijk.getAt(0));
        ijk.setAttribute("k", this->ijk.getAt(0));
    }else{
        ijk.setAttribute("i", 0.0);
        ijk.setAttribute("j", 0.0);
        ijk.setAttribute("k", 0.0);
    }
    line.appendChild(ijk);

    return line;

}

bool Line::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set line attributes
        QDomElement directionVector = xmlElem.firstChildElement("spatialDirection");
        QDomElement axisPoint = xmlElem.firstChildElement("coordinates");

        if(directionVector.isNull() || axisPoint.isNull()
                || !directionVector.hasAttribute("i") || !directionVector.hasAttribute("j") || !directionVector.hasAttribute("k")
                || !axisPoint.hasAttribute("x") || !axisPoint.hasAttribute("y") || !axisPoint.hasAttribute("z")){
            return false;
        }

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

bool Line::saveSimulationData()
{
    this->mySimulationData.addXYZ(this->xyz.getAt(0),this->xyz.getAt(1),this->xyz.getAt(2));

    this->mySimulationData.addIJK(this->ijk.getAt(0),this->ijk.getAt(1),this->ijk.getAt(2));
    return true;
}

QString Line::getDisplayX(bool showDiff) const{
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

QString Line::getDisplayY(bool showDiff) const{
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

QString Line::getDisplayZ(bool showDiff) const{
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

QString Line::getDisplayI(bool showDiff) const{
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

QString Line::getDisplayJ(bool showDiff) const{
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
}

QString Line::getDisplayK(bool showDiff) const{
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

QString Line::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString Line::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}

QString Line::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}

QString Line::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString Line::getDisplayMConfig() const{
    return this->activeMeasurementConfig.getDisplayName();
}

QString Line::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        //return QString::number(this->myStatistic.stdev*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
    }
    return "-/-";

}
*/

/*!
 * \brief Line::Line
 * \param isNominal
 * \param parent
 */
Line::Line(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setLine(this);
    }

}

/*!
 * \brief Line::Line
 * \param isNominal
 * \param xyz
 * \param axis
 * \param parent
 */
Line::Line(const bool &isNominal, const Position &xyz, const Direction &axis, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setLine(this);
    }

    this->setLine(xyz, axis);

}

/*!
 * \brief Line::Line
 * \param copy
 * \param parent
 */
Line::Line(const Line &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setLine(this);
    }

    this->xyz = copy.xyz;
    this->axis = copy.axis;

}

/*!
 * \brief Line::operator =
 * \param copy
 * \return
 */
Line &Line::operator=(const Line &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setLine(this);
    }

    this->xyz = copy.xyz;
    this->axis = copy.axis;

    return *this;

}

/*!
 * \brief Line::~Line
 */
Line::~Line(){

}

/*!
 * \brief Line::hasDirection
 * \return
 */
bool Line::hasDirection() const{
    return true;
}

/*!
 * \brief Line::hasPosition
 * \return
 */
bool Line::hasPosition() const{
    return true;
}

/*!
 * \brief Line::getDirection
 * Returns the direction vector
 * \return
 */
const Direction &Line::getDirection() const{
    return this->axis;
}

/*!
 * \brief Line::getPosition
 * Returns an arbitrary point on the line
 * \return
 */
const Position &Line::getPosition() const{
    return this->xyz;
}

/*!
 * \brief Line::setLine
 * \param xyz
 * \param axis
 */
void Line::setLine(const Position &xyz, const Direction &axis){

    //set the given parameters
    this->xyz = xyz;
    this->axis = axis;

}

/*!
 * \brief Line::recalc
 */
void Line::recalc(){

    Geometry::recalc();

    //reset line definition if not solved
    if(!this->isSolved){
        this->xyz.setVector(0.0, 0.0, 0.0);
        this->axis.setVector(0.0, 0.0, 0.0);
    }

}

/*!
 * \brief Line::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Line::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement line = Geometry::toOpenIndyXML(xmlDoc);

    /*if(line.isNull()){
        return line;
    }

    line.setAttribute("type", getGeometryTypeName(eLineGeometry));

    //add vector of direction
    QDomElement ijk = xmlDoc.createElement("spatialDirection");
    if(this->ijk.getSize() >= 3 && (this->getIsSolved() || this->getIsNominal())){
        ijk.setAttribute("i", this->ijk.getAt(0));
        ijk.setAttribute("j", this->ijk.getAt(0));
        ijk.setAttribute("k", this->ijk.getAt(0));
    }else{
        ijk.setAttribute("i", 0.0);
        ijk.setAttribute("j", 0.0);
        ijk.setAttribute("k", 0.0);
    }
    line.appendChild(ijk);*/

    return line;

}

/*!
 * \brief Line::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Line::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){
/*
        //set line attributes
        QDomElement directionVector = xmlElem.firstChildElement("spatialDirection");
        QDomElement axisPoint = xmlElem.firstChildElement("coordinates");

        if(directionVector.isNull() || axisPoint.isNull()
                || !directionVector.hasAttribute("i") || !directionVector.hasAttribute("j") || !directionVector.hasAttribute("k")
                || !axisPoint.hasAttribute("x") || !axisPoint.hasAttribute("y") || !axisPoint.hasAttribute("z")){
            return false;
        }

        this->ijk.setAt(0, directionVector.attribute("i").toDouble());
        this->ijk.setAt(1, directionVector.attribute("j").toDouble());
        this->ijk.setAt(2, directionVector.attribute("k").toDouble());
        this->ijk.setAt(3, 1.0);
        this->xyz.setAt(0, axisPoint.attribute("x").toDouble());
        this->xyz.setAt(1, axisPoint.attribute("y").toDouble());
        this->xyz.setAt(2, axisPoint.attribute("z").toDouble());
        this->xyz.setAt(3, 1.0);
*/
    }

    return result;

}

/*!
 * \brief Line::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Line::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->xyz.getVector().getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Line::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Line::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->xyz.getVector().getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Line::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Line::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->xyz.getVector().getAt(2), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Line::getDisplayPrimaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString Line::getDisplayPrimaryI(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->axis.getVector().getAt(0), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Line::getDisplayPrimaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString Line::getDisplayPrimaryJ(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->axis.getVector().getAt(1), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Line::getDisplayPrimaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString Line::getDisplayPrimaryK(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->axis.getVector().getAt(2), 'f', digits);
    }
    return QString("");
}
