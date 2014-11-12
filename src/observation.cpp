#include "observation.h"

#include "geometry.h"
#include "station.h"
#include "reading.h"
#include "function.h"


Observation::Observation(Reading *r, Station *s) :
    myReading(r), myStation(s), myXyz(4), myOriginalXyz(4), sigmaXyz(4)
{
    this->id = Configuration::generateID();
    if(r != NULL){
        r->obs = this;
    }

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
