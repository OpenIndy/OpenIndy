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
        if(xmlElem.hasAttribute("isValid") && xmlElem.hasAttribute("x") && xmlElem.hasAttribute("y")
                && xmlElem.hasAttribute("z") && xmlElem.hasAttribute("sigmaX") && xmlElem.hasAttribute("sigmaY")
                && xmlElem.hasAttribute("sigmaZ")){
            this->isValid = xmlElem.attribute("isValid").toInt();
            this->myXyz.setAt(0, xmlElem.attribute("x"));
            this->myXyz.setAt(1, xmlElem.attribute("y"));
            this->myXyz.setAt(2, xmlElem.attribute("z"));
            this->sigmaXyz.setAt(0, xmlElem.attribute("sigmaX"));
            this->sigmaXyz.setAt(1, xmlElem.attribute("sigmaY"));
            this->sigmaXyz.setAt(2, xmlElem.attribute("sigmaZ"));
        }

        //set reading


    }

    return result;

}

/*!
 * \brief Observation::fromOpenIndyXML
 * \param xml
 * \return
 */
ElementDependencies Observation::fromOpenIndyXML(QXmlStreamReader &xml){

    ElementDependencies dependencies;
    dependencies.typeOfElement = Configuration::eObservationElement;

    //fill observation attributes
    QXmlStreamAttributes attributes = xml.attributes();
    if(attributes.hasAttribute("id")) {
        this->id = attributes.value("id").toInt();
        dependencies.elementID = this->id;
    }
    if(attributes.hasAttribute("x")){
        this->myXyz.setAt(0,attributes.value("x").toDouble());
    }
    if(attributes.hasAttribute("y")){
        this->myXyz.setAt(1,attributes.value("y").toDouble());
    }
    if(attributes.hasAttribute("z")){
        this->myXyz.setAt(2,attributes.value("z").toDouble());
    }
    this->myXyz.setAt(3,1.0);
    if(attributes.hasAttribute("isValid")){
        if(attributes.value("isValid").toInt() == 1){
            this->isValid = true;
        }else{
            this->isValid = false;
        }
    }
    if(attributes.hasAttribute("sigmaX")){
        this->sigmaXyz.setAt(0,attributes.value("sigmaX").toDouble());
    }
    if(attributes.hasAttribute("sigmaY")){
        this->sigmaXyz.setAt(1,attributes.value("sigmaY").toDouble());
    }
    if(attributes.hasAttribute("sigmaZ")){
        this->sigmaXyz.setAt(2,attributes.value("sigmaZ").toDouble());
    }

    xml.readNext();

    //fill observation's values
    while( !xml.atEnd() && xml.name().compare("observation") != 0 ){

        if(xml.tokenType() == QXmlStreamReader::StartElement) {

            if(xml.name() == "reading"){

                //load reading and connect to observation
                Reading *r = this->readReading(xml);
                r->obs = this;
                this->myReading = r;

                //get time of measurement
                if(xml.attributes().hasAttribute("time")){
                    r->measuredAt = QDateTime(QDate::fromString(xml.attributes().value("time").toString(),Qt::ISODate));
                }

                //get original xyz to save at observation object
                r->toCartesian();
                this->myOriginalXyz = r->rCartesian.xyz;

                xml.readNext();

            }else if(xml.name() == "station"){

                if(xml.attributes().hasAttribute("ref")){
                    dependencies.addFeatureID(xml.attributes().value("ref").toInt(), "station");
                }
                xml.readNext();

            }else{
                xml.readNext();
            }

        }else{
            xml.readNext();
        }

    }

    return dependencies;

}

/*!
 * \brief Observation::writeProxyObservations
 * \param stream
 * \return
 */
bool Observation::writeProxyObservations(QXmlStreamWriter &stream){

    stream.writeStartElement("observation");
    stream.writeAttribute("ref", QString::number(this->id));
    stream.writeEndElement();

    return true;

}

Reading* Observation::readReading(QXmlStreamReader& xml){

    Reading* r = new Reading();


    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("id")) {
        r->id = attributes.value("id").toInt();
    }
    if(attributes.hasAttribute("type")){
       r->typeofReading = (Configuration::ReadingTypes) attributes.value("type").toInt();
    }
    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named reading.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "reading")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {

            switch(r->typeofReading){
                case(Configuration::ePolar) :{

                if(xml.name() == "measurement") {
                    QXmlStreamAttributes attributes = xml.attributes();

                            if(attributes.hasAttribute("time")) {
                                r->measuredAt = QDateTime::fromString(attributes.value("time").toString());
                            }
                            if(attributes.hasAttribute("type")){

                                if(attributes.value("type").toString() == "azimuth"){

                                    if(attributes.hasAttribute("value")){
                                        r->rPolar.azimuth = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rPolar.sigmaAzimuth = attributes.value("sigma").toDouble();
                                    }

                                }
                                if(attributes.value("type").toString() == "zenith"){

                                    if(attributes.hasAttribute("value")){
                                        r->rPolar.zenith = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rPolar.sigmaZenith = attributes.value("sigma").toDouble();
                                    }

                                }
                                if(attributes.value("type").toString() == "distance"){

                                    if(attributes.hasAttribute("value")){
                                        r->rPolar.distance = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rPolar.sigmaDistance = attributes.value("sigma").toDouble();
                                    }

                                }

                            }
                        }

                    break;
                }
                case(Configuration::eDistance) :{


                if(xml.name() == "measurement") {
                    QXmlStreamAttributes attributes = xml.attributes();

                            if(attributes.hasAttribute("time")) {
                                r->measuredAt = QDateTime::fromString(attributes.value("time").toString());
                            }
                            if(attributes.hasAttribute("type")){

                                if(attributes.value("type").toString() == "distance"){

                                    if(attributes.hasAttribute("value")){
                                        r->rDistance.distance = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rDistance.sigmaDistance = attributes.value("sigma").toDouble();
                                    }
                                }
                            }
                     }


                    break;
                }
                case(Configuration::eDirection) :{

                if(xml.name() == "measurement") {
                    QXmlStreamAttributes attributes = xml.attributes();

                            if(attributes.hasAttribute("time")) {
                                r->measuredAt = QDateTime::fromString(attributes.value("time").toString());
                            }
                            if(attributes.hasAttribute("type")){

                                if(attributes.value("type").toString() == "azimuth"){

                                    if(attributes.hasAttribute("value")){
                                        r->rDirection.azimuth = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rDirection.sigmaAzimuth = attributes.value("sigma").toDouble();
                                    }

                                }
                                if(attributes.value("type").toString() == "zenith"){

                                    if(attributes.hasAttribute("value")){
                                        r->rDirection.zenith = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rDirection.sigmaZenith = attributes.value("sigma").toDouble();
                                    }

                                }

                            }
                        }

                    break;
                }
                case(Configuration::eCartesian) :{

                if(xml.name() == "measurement") {
                    QXmlStreamAttributes attributes = xml.attributes();

                            if(attributes.hasAttribute("time")) {
                                r->measuredAt = QDateTime::fromString(attributes.value("time").toString());
                            }
                            if(attributes.hasAttribute("type")){

                                if(attributes.value("type").toString() == "x"){

                                    if(attributes.hasAttribute("value")){
                                        r->rCartesian.xyz.setAt(0,attributes.value("value").toDouble());
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                       r->rCartesian.sigmaXyz.setAt(0,attributes.value("sigma").toDouble());
                                    }

                                }
                                if(attributes.value("type").toString() == "y"){

                                    if(attributes.hasAttribute("value")){
                                        r->rCartesian.xyz.setAt(1,attributes.value("value").toDouble());
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                       r->rCartesian.sigmaXyz.setAt(1,attributes.value("sigma").toDouble());
                                    }

                                }
                                if(attributes.value("type").toString() == "z"){

                                    if(attributes.hasAttribute("value")){
                                        r->rCartesian.xyz.setAt(1,attributes.value("value").toDouble());
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                       r->rCartesian.sigmaXyz.setAt(1,attributes.value("sigma").toDouble());
                                    }

                                }

                            }
                        }

                    break;
                }
                case(Configuration::eLevel) :{

                if(xml.name() == "measurement") {
                    QXmlStreamAttributes attributes = xml.attributes();

                            if(attributes.hasAttribute("time")) {
                               r->measuredAt = QDateTime::fromString(attributes.value("time").toString());
                            }
                            if(attributes.hasAttribute("type")){

                                if(attributes.value("type").toString() == "RX"){

                                    if(attributes.hasAttribute("value")){
                                        r->rLevel.RX = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rLevel.sigmaRX = attributes.value("sigma").toDouble();
                                    }

                                }
                                if(attributes.value("type").toString() == "RY"){

                                    if(attributes.hasAttribute("value")){
                                        r->rLevel.RY = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rLevel.sigmaRY = attributes.value("sigma").toDouble();
                                    }

                                }
                                if(attributes.value("type").toString() == "RZ"){

                                    if(attributes.hasAttribute("value")){
                                        r->rLevel.RZ = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rLevel.sigmaRZ = attributes.value("sigma").toDouble();
                                    }

                                }


                            }
                        }


                    break;
            }
            case(Configuration::eUndefined) :{

                if(xml.name() == "measurement") {
                    QXmlStreamAttributes attributes = xml.attributes();

                            if(attributes.hasAttribute("time")) {

                                r->measuredAt = QDateTime::fromString(attributes.value("time").toString());
                            }
                            if(attributes.hasAttribute("type")){

                                    if(attributes.hasAttribute("value")){
                                        r->rUndefined.values.insert(attributes.value("type").toString(),attributes.value("value").toDouble());
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rUndefined.sigmaValues.insert(attributes.value("type").toString(),attributes.value("sigma").toDouble());
                                    }

                             }
                     }

                    break;
            }
            }


        }
        /* ...and next... */
        xml.readNext();
    }

    if(xml.hasError()) {

        return r;
    }

    return r;

}
