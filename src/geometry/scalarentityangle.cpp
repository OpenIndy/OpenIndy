#include "scalarentityangle.h"

#include "function.h"

ScalarEntityAngle::ScalarEntityAngle()
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = false;
}

/*!
 * \brief ScalarEntityAngle::ScalarEntityAngle
 * \param copy
 */
ScalarEntityAngle::ScalarEntityAngle(const ScalarEntityAngle &copy){
    this->id = copy.id;
    this->name = copy.name;
    this->angle = copy.getAngle();
    this->isSolved = copy.isSolved;
}

/*!
 * \brief ScalarEntityAngle::recalc
 * Execute alls functions in the specified order
 */
void ScalarEntityAngle::recalc(){
    /*
     * isDefined -> becomes true as soon as the first function of a feature has been executed, which defines the feature
     * isSolved -> is true as long as there isn't any function which cannot be successfully executed
     */
    bool isDefined = false;
    foreach(Function *f, this->functionList){
        if(!isDefined){
            //this->isSolved = f->exec(*this);
            isDefined = true;
        }else if(this->isSolved){
            //this->isSolved = f->exec(*this);
        }
    }
}

bool ScalarEntityAngle::toOpenIndyXML(QXmlStreamWriter &stream){

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
            stream.writeStartElement("angle");
            stream.writeAttribute("value", QString::number(this->getAngle()));
            stream.writeEndElement();


            stream.writeStartElement("standardDeviation");
            stream.writeAttribute("value", QString::number(this->myStatistic.stdev));
            stream.writeEndElement();
    }

   this->writeGeometryAttributes(stream);

   stream.writeEndElement();

    return true;

}

ElementDependencies ScalarEntityAngle::fromOpenIndyXML(QXmlStreamReader &xml){
    ElementDependencies dependencies;

    return dependencies;
}

/*!
 * \brief ScalarEntityAngle::getAngle
 */
double ScalarEntityAngle::getAngle() const{
    return this->angle;
}

/*!
 * \brief ScalarEntityAngle::setAngle
 * \param angle
 */
void ScalarEntityAngle::setAngle(double angle){
    this->angle = angle;
}

/*!
 * \brief ScalarEntityAngle::setAngle
 * \param direction1
 * \param direction2
 */
void ScalarEntityAngle::setAngle(OiVec direction1, OiVec direction2){
    if(direction1.getSize() >= 3 && direction2.getSize() >= 3){
        //copy OiVec's to new OiVec's of size 3
        OiVec a(3);
        OiVec b(3);
        for(int i = 0; i < 3; i++){
            a.setAt(i, direction1.getAt(i));
            b.setAt(i, direction2.getAt(i));
        }
        //normalize vectors
        a = a.normalize();
        b = b.normalize();
        //calculate angle between directions
        this->angle = qAcos(OiVec::dot(a, b));
    }
}

QString ScalarEntityAngle::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString ScalarEntityAngle::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}
QString ScalarEntityAngle::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}

QString ScalarEntityAngle::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString ScalarEntityAngle::getDisplayMConfig() const{
    return this->mConfig.name;
}

QString ScalarEntityAngle::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        return QString::number(this->myStatistic.stdev*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    }else{
        return "-/-";
    }
}

QString ScalarEntityAngle::getDisplayScalarAngleValue() const{
    return QString::number(this->angle*180.0/3.141592653589793,'f',UnitConverter::angleDigits);
}
