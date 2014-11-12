#include "observation.h"

#include "geometry.h"
#include "station.h"
#include "reading.h"
#include "function.h"


Observation::Observation(Reading *r, Station *s, bool isActiveCoordSys) :
    myReading(r), myStation(s), myXyz(4), myOriginalXyz(4), sigmaXyz(4), isActiveCoordSys(isActiveCoordSys)
{
    this->id = Configuration::generateID();
    if(r != NULL){
        r->obs = this;
    }
    //use all observations on default
    this->isUsed = true;

    //calculate observation and set valid and solved bools
    this->calcFromReading();

    //initialize matrices
    myStatistic.qxx = OiMat(4,4);
    myOriginalStatistic.qxx = OiMat(4,4);
}

Observation::Observation() :
    myReading(NULL), myStation(NULL), myXyz(4), myOriginalXyz(4), sigmaXyz(4)
{
    this->id = Configuration::generateID();

    //initialize matrices
    myStatistic.qxx = OiMat(4,4);
    myOriginalStatistic.qxx = OiMat(4,4);
}

Observation::~Observation(){

    //delete corresponding reading
    if(this->myReading != NULL){
        delete this->myReading;
    }

    //delete this observation from target geometries
    foreach(Geometry *myGeom, this->myTargetGeometries){
        if(myGeom != NULL){
            myGeom->removeObservation(this);
        }
    }

}

/*!
 * \brief Observation::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Observation::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement observation = Element::toOpenIndyXML(xmlDoc);

    if(observation.isNull()){
        return observation;
    }

    observation.setTagName("observation");

    //add observation attributes
    if(this->myXyz.getSize() >= 3){
        observation.setAttribute("x", this->myXyz.getAt(0));
        observation.setAttribute("y", this->myXyz.getAt(1));
        observation.setAttribute("z", this->myXyz.getAt(2));
    }else{
        observation.setAttribute("x", this->myXyz.getAt(0));
        observation.setAttribute("y", this->myXyz.getAt(1));
        observation.setAttribute("z", this->myXyz.getAt(2));
    }
    if(this->sigmaXyz.getSize() >= 3){
        observation.setAttribute("sigmaX", this->sigmaXyz.getAt(0));
        observation.setAttribute("sigmaY", this->sigmaXyz.getAt(1));
        observation.setAttribute("sigmaZ", this->sigmaXyz.getAt(2));
    }else{
        observation.setAttribute("sigmaX", this->sigmaXyz.getAt(0));
        observation.setAttribute("sigmaY", this->sigmaXyz.getAt(1));
        observation.setAttribute("sigmaZ", this->sigmaXyz.getAt(2));
    }
    observation.setAttribute("isValid", this->isValid);

    //add station
    if(this->myStation != NULL){
        QDomElement station = xmlDoc.createElement("station");
        station.setAttribute("ref", this->myStation->getId());
        observation.appendChild(station);
    }

    //add reading
    if(this->myReading != NULL){
        QDomElement reading = this->myReading->toOpenIndyXML(xmlDoc);
        if(!reading.isNull()){
            observation.appendChild(reading);
        }
    }

    return observation;

}

/*!
 * \brief Observation::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Observation::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Element::fromOpenIndyXML(xmlElem);

    if(result){

        //set observation attributes
        if(!xmlElem.hasAttribute("isValid") || !xmlElem.hasAttribute("x") || !xmlElem.hasAttribute("y")
                || !xmlElem.hasAttribute("z") || !xmlElem.hasAttribute("sigmaX") || !xmlElem.hasAttribute("sigmaY")
                || !xmlElem.hasAttribute("sigmaZ")){
            return false;
        }
        this->isValid = xmlElem.attribute("isValid").toInt();
        this->myXyz.setAt(0, xmlElem.attribute("x").toDouble());
        this->myXyz.setAt(1, xmlElem.attribute("y").toDouble());
        this->myXyz.setAt(2, xmlElem.attribute("z").toDouble());
        this->myXyz.setAt(3, 1.0);
        this->sigmaXyz.setAt(0, xmlElem.attribute("sigmaX").toDouble());
        this->sigmaXyz.setAt(1, xmlElem.attribute("sigmaY").toDouble());
        this->sigmaXyz.setAt(2, xmlElem.attribute("sigmaZ").toDouble());

        //set reading
        QDomElement reading = xmlElem.firstChildElement("reading");
        if(reading.isNull()){
            return false;
        }
        Reading *myReading = new Reading();
        this->myReading = myReading;
        myReading->obs = this;
        result = myReading->fromOpenIndyXML(reading);

        //get original xyz from reading
        this->myReading->toCartesian();
        this->myOriginalXyz.setAt(0, this->myReading->rCartesian.xyz.getAt(0));
        this->myOriginalXyz.setAt(1, this->myReading->rCartesian.xyz.getAt(1));
        this->myOriginalXyz.setAt(2, this->myReading->rCartesian.xyz.getAt(2));
        this->myOriginalXyz.setAt(3, 1.0);

    }

    return result;

}

/*!
 * \brief calcFromReading calculates the observation depending on its reading
 */
void Observation::calcFromReading()
{
    Reading *r;
    r = this->myReading;

    switch (this->myReading->typeofReading) {
    case Configuration::ePolar:
        //calc obs xyz
        this->myOriginalXyz = Reading::toCartesian(r->rPolar.azimuth,r->rPolar.zenith,r->rPolar.distance);
        this->myXyz = Reading::toCartesian(r->rPolar.azimuth,r->rPolar.zenith,r->rPolar.distance);

        //calc sigma xyz
        this->sigmaXyz = r->errorPropagationPolarToCart();

        //created from reading polar
        this->setIsValid(true);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    case Configuration::eCartesian:
        //calc xyz
        this->myOriginalXyz.setAt(0,r->rCartesian.xyz.getAt(0));
        this->myOriginalXyz.setAt(1,r->rCartesian.xyz.getAt(1));
        this->myOriginalXyz.setAt(2,r->rCartesian.xyz.getAt(2));
        this->myOriginalXyz.setAt(3,1.0);
        this->myXyz.setAt(0,r->rCartesian.xyz.getAt(0));
        this->myXyz.setAt(1,r->rCartesian.xyz.getAt(1));
        this->myXyz.setAt(2,r->rCartesian.xyz.getAt(2));
        this->myXyz.setAt(3,1.0);

        //created from reading cartesian
        this->setIsValid(true);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    case Configuration::eDirection:
        //created from reading direction
        this->setIsValid(false);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    case Configuration::eDistance:
        //created from reading distance
        this->setIsValid(false);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    case Configuration::eTemperatur:
        //created from reading temperature
        this->setIsValid(false);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    case Configuration::eLevel:
        //created from reading level
        this->setIsValid(false);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    case Configuration::eUndefined:
        //created from reading undefined
        this->setIsValid(false);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    default:
        this->setIsValid(false);

        //measured in active coord system
        this->setIsSolved(this->isActiveCoordSys);

        break;
    }
}

/*!
 * \brief setIsValid defines if the observation has valid xyz coords created from its reading
 * \param isValid
 */
void Observation::setIsValid(bool isValid)
{
    this->isValid = isValid;
}

/*!
 * \brief getIsValid returns if the observation is valid
 * \return
 */
bool Observation::getIsValid()
{
    return this->isValid;
}

/*!
 * \brief setIsSolved defines if the observation is solved in the current coordinate system
 * \param isSolved
 */
void Observation::setIsSolved(bool isSolved)
{
    this->isSolved = isSolved;
}

/*!
 * \brief getIsSolved
 * \return
 */
bool Observation::getIsSolved()
{
    return this->isSolved;
}

/*!
 * \brief setIsUsed
 * \param use
 */
void Observation::setIsUsed(bool use)
{
    this->isUsed = use;
}

/*!
 * \brief getIsUsed
 * \return
 */
bool Observation::getIsUsed()
{
    return this->isUsed;
}

/*!
 * \brief getUseState if the observation can be used for calculation or other things.
 * For this the observation has to be solved and valid.
 * \return
 */
bool Observation::getUseState()
{
    if(this->isSolved && this->isValid && this->isUsed){
        return true;
    }
    return false;
}
