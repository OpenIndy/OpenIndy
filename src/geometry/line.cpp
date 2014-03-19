#include "line.h"

#include "function.h"

Line::Line() : xyz(4), ijk(4)
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief Line::Line
 * \param copy
 */
Line::Line(const Line &copy){
    this->id = copy.id;
    this->name = copy.name;
    this->xyz = copy.xyz;
    this->ijk = copy.ijk;
    this->isSolved = copy.isSolved;
}

Line::~Line(){

}

/*!
 * \brief Line::recalc
 * Execute alls functions in the specified order
 */
void Line::recalc(){
    /*
     * isDefined -> becomes true as soon as the first function of a feature has been executed, which defines the feature
     * isSolved -> is true as long as there isn't any function which cannot be successfully executed
     */
    bool isDefined = false;
    foreach(Function *f, this->functionList){
        if(!isDefined){
            this->isSolved = f->exec(*this);
            isDefined = true;
        }else if(this->isSolved){
            this->isSolved = f->exec(*this);
        }
    }
    //if no function is set this feature cannot be solved and its coordinates are reset
    if(this->functionList.size() == 0 && this->isNominal == false){
        this->isSolved = false;
        this->xyz = OiVec(4);
        this->ijk = OiVec(4);
    }
}

bool Line::toOpenIndyXML(QXmlStreamWriter &stream){


    //---------------common geometry attributes--------------
    stream.writeStartElement("geometry");
    stream.writeAttribute("id", QString::number(this->id));
    stream.writeAttribute("name", this->name);
    stream.writeAttribute("type", Configuration::sPoint);
    stream.writeAttribute("nominal",QString::number(this->isNominal));
    stream.writeAttribute("common",QString::number(this->isCommon));
    stream.writeAttribute("solved", QString::number(this->isSolved));


    //---------------specific geometry attributes--------------
    if(this->isSolved || this->isNominal){
            stream.writeStartElement("coordinates");
            stream.writeAttribute("x", QString::number(this->xyz.getAt(0)));
            stream.writeAttribute("y", QString::number(this->xyz.getAt(1)));
            stream.writeAttribute("z", QString::number(this->xyz.getAt(2)));
            stream.writeEndElement();

            stream.writeStartElement("spatialDirection");
            stream.writeAttribute("i", QString::number(this->ijk.getAt(0)));
            stream.writeAttribute("j", QString::number(this->ijk.getAt(1)));
            stream.writeAttribute("k", QString::number(this->ijk.getAt(2)));
            stream.writeEndElement();

            stream.writeStartElement("standardDeviation");
            stream.writeAttribute("value", QString::number(this->myStatistic.stdev));
            stream.writeEndElement();
    }

   this->writeGeometryAttributes(stream);

   stream.writeEndElement();

    return true;

}

ElementDependencies Line::fromOpenIndyXML(QXmlStreamReader &xml){

    ElementDependencies dependencies;

    return dependencies;
}

QString Line::getDisplayX() const{
    return QString::number(this->xyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f', UnitConverter::distanceDigits);
}

QString Line::getDisplayY() const{
    return QString::number(this->xyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Line::getDisplayZ() const{
    return QString::number(this->xyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Line::getDisplayI() const{
    return QString::number(this->ijk.getAt(0),'f',UnitConverter::distanceDigits);
}

QString Line::getDisplayJ() const{
    return QString::number(this->ijk.getAt(1),'f',UnitConverter::distanceDigits);
}

QString Line::getDisplayK() const{
    return QString::number(this->ijk.getAt(2),'f',UnitConverter::distanceDigits);
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
    return this->mConfig.name;
}

QString Line::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        return QString::number(this->myStatistic.stdev*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    }else{
        return "-/-";
    }
}
