#include "observation.h"

#include "geometry.h"
#include "station.h"
#include "reading.h"
#include "function.h"


Observation::Observation(Reading *r, Station *s) : myReading(r), myStation(s), myXyz(4), myOriginalXyz(4), sigmaXyz(4)
{
    this->id = Configuration::generateID();
}

Observation::~Observation(){

    //delete corresponding reading
    if(this->myReading != NULL){
        delete this->myReading;
    }

    //delete this observation from target geometries
    foreach(Geometry *myGeom, this->myTargetGeometries){
        if(myGeom != NULL){
            myGeom->myObservations.removeOne(this);
            foreach(Function *myFunc, myGeom->functionList){
                if(myFunc != NULL){
                    myFunc->removeObservation(this->id);
                }
            }
        }
    }

}

bool Observation::toOpenIndyXML(QXmlStreamWriter &stream){


    stream.writeStartElement("observation");
    stream.writeAttribute("id", QString::number(this->id));
    stream.writeAttribute("x", QString::number(this->myXyz.getAt(0)));
    stream.writeAttribute("y", QString::number(this->myXyz.getAt(1)));
    stream.writeAttribute("z", QString::number(this->myXyz.getAt(2)));
    stream.writeAttribute("isValid", QString::number(this->isValid));
    stream.writeAttribute("sigmaX", QString::number(this->sigmaXyz.getAt(0)));
    stream.writeAttribute("sigmaY", QString::number(this->sigmaXyz.getAt(1)));
    stream.writeAttribute("sigmaZ", QString::number(this->sigmaXyz.getAt(2)));


    if(this->myStation != NULL){
        stream.writeStartElement("member");
        stream.writeAttribute("type", "station");
        stream.writeAttribute("ref", QString::number(this->myStation->id));
        stream.writeEndElement();
    }

    if(this->myReading != NULL){
        stream.writeStartElement("reading");
        stream.writeAttribute("id",QString::number(this->myReading->id));
        stream.writeAttribute("type",QString::number(this->myReading->typeofReading));

        QString measuredAtTime = this->myReading->measuredAt.toString(Qt::ISODate);

        switch(this->myReading->typeofReading){
            case(Configuration::ePolar) :{

            //get current date and time


                stream.writeStartElement("measurement");
                stream.writeAttribute("time",measuredAtTime);
                stream.writeAttribute("type","azimuth");
                stream.writeAttribute("value",QString::number(this->myReading->rPolar.azimuth));
                stream.writeAttribute("sigma",QString::number(this->myReading->rPolar.sigmaAzimuth));
                stream.writeEndElement();

                stream.writeStartElement("measurement");
                stream.writeAttribute("time",measuredAtTime);
                stream.writeAttribute("type","zenith");
                stream.writeAttribute("value",QString::number(this->myReading->rPolar.zenith));
                stream.writeAttribute("sigma",QString::number(this->myReading->rPolar.sigmaZenith));
                stream.writeEndElement();

                stream.writeStartElement("measurement");
                stream.writeAttribute("time",measuredAtTime);
                stream.writeAttribute("type","distance");
                stream.writeAttribute("value",QString::number(this->myReading->rPolar.distance));
                stream.writeAttribute("sigma",QString::number(this->myReading->rPolar.sigmaDistance));
                stream.writeEndElement();


                break;
            }
            case(Configuration::eDistance) :{

                stream.writeStartElement("measurement");
                stream.writeAttribute("time",measuredAtTime);
                stream.writeAttribute("type","distance");
                stream.writeAttribute("value",QString::number(this->myReading->rDistance.distance));
                stream.writeAttribute("sigma",QString::number(this->myReading->rDistance.sigmaDistance));
                stream.writeEndElement();

                break;
            }
            case(Configuration::eDirection) :{

                stream.writeStartElement("measurement");
                stream.writeAttribute("time",measuredAtTime);
                stream.writeAttribute("type","azimuth");
                stream.writeAttribute("value",QString::number(this->myReading->rDirection.azimuth));
                stream.writeAttribute("sigma",QString::number(this->myReading->rDirection.sigmaAzimuth));
                stream.writeEndElement();

                stream.writeStartElement("measurement");
                stream.writeAttribute("time",measuredAtTime);
                stream.writeAttribute("type","zenith");
                stream.writeAttribute("value",QString::number(this->myReading->rDirection.zenith));
                stream.writeAttribute("sigma",QString::number(this->myReading->rDirection.sigmaZenith));
                stream.writeEndElement();

                break;
            }
            case(Configuration::eCartesian) :{

                stream.writeStartElement("measurement");
                stream.writeAttribute("time",measuredAtTime);
                stream.writeAttribute("type","x");
                stream.writeAttribute("value",QString::number(this->myReading->rCartesian.xyz.getAt(0)));
                stream.writeAttribute("sigma",QString::number(this->myReading->rCartesian.sigmaXyz.getAt(0)));
                stream.writeEndElement();

                stream.writeStartElement("measurement");
                stream.writeAttribute("time",measuredAtTime);
                stream.writeAttribute("type","y");
                stream.writeAttribute("value",QString::number(this->myReading->rCartesian.xyz.getAt(1)));
                stream.writeAttribute("sigma",QString::number(this->myReading->rCartesian.sigmaXyz.getAt(1)));
                stream.writeEndElement();

                stream.writeStartElement("measurement");
                stream.writeAttribute("time",measuredAtTime);
                stream.writeAttribute("type","z");
                stream.writeAttribute("value",QString::number(this->myReading->rCartesian.xyz.getAt(2)));
                stream.writeAttribute("sigma",QString::number(this->myReading->rCartesian.sigmaXyz.getAt(2)));
                stream.writeEndElement();


                break;
            }
            case(Configuration::eLevel) :{

                stream.writeStartElement("measurement");
                stream.writeAttribute("time",measuredAtTime);
                stream.writeAttribute("type","angleXZ");
                stream.writeAttribute("value",QString::number(this->myReading->rLevel.angleXZ));
                stream.writeAttribute("sigma",QString::number(this->myReading->rLevel.sigmaAngleXZ));
                stream.writeEndElement();

                stream.writeStartElement("measurement");
                stream.writeAttribute("time",measuredAtTime);
                stream.writeAttribute("type","angleYZ");
                stream.writeAttribute("value",QString::number(this->myReading->rLevel.angleYZ));
                stream.writeAttribute("sigma",QString::number(this->myReading->rLevel.sigmaAngleYZ));
                stream.writeEndElement();

                stream.writeStartElement("measurement");
                stream.writeAttribute("time",measuredAtTime);
                stream.writeAttribute("type","diffXY");
                stream.writeAttribute("value",QString::number(this->myReading->rLevel.diffXY));
                stream.writeAttribute("sigma","-");
                stream.writeEndElement();

                stream.writeStartElement("measurement");
                stream.writeAttribute("time",measuredAtTime);
                stream.writeAttribute("type","diffXZ");
                stream.writeAttribute("value",QString::number(this->myReading->rLevel.diffXZ));
                stream.writeAttribute("sigma","-");
                stream.writeEndElement();


                break;
        }
        case(Configuration::eUndefined) :{

            QMapIterator<QString, double > j(this->myReading->rUndefined.values);
            while (j.hasNext()) {
                j.next();

                stream.writeStartElement("measurement");
                stream.writeAttribute("time",measuredAtTime);
                stream.writeAttribute("type",j.key());
                stream.writeAttribute("value",QString::number(j.value()));
                stream.writeAttribute("sigma",QString::number(this->myReading->rUndefined.sigmaValues.value(j.key())));
                stream.writeEndElement();

            }

                break;
        }
        }

        stream.writeEndElement();
    }


    stream.writeEndElement();
    return true;
}

ElementDependencies Observation::fromOpenIndyXML(QXmlStreamReader &xml){

    ElementDependencies dependencies;

    dependencies.typeOfElement = Configuration::eObservationElement;

    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("id")) {
        this->id = attributes.value("id").toInt();
        dependencies.elementID = this->id;
    }
    if(attributes.hasAttribute("x")){
        this->myOriginalXyz.setAt(0,attributes.value("x").toDouble());
        this->myXyz.setAt(0,attributes.value("x").toDouble());
    }
    if(attributes.hasAttribute("y")){
        this->myOriginalXyz.setAt(1,attributes.value("y").toDouble());
        this->myXyz.setAt(1,attributes.value("y").toDouble());
    }
    if(attributes.hasAttribute("z")){
        this->myOriginalXyz.setAt(2,attributes.value("z").toDouble());
        this->myXyz.setAt(2,attributes.value("z").toDouble());
    }
    if(attributes.hasAttribute("isValid")){
        this->isValid = attributes.value("isVaild").toInt();
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
    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named observation.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "observation")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {

            if(xml.name() == "reading") {
                Reading *r = this->readReading(xml);
                r->obs = this;
                this->myReading = r;
            }

            if(xml.name() == "member"){

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "member")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes memberAttributes = xml.attributes();

                        if(memberAttributes.hasAttribute("type")){

                            if (memberAttributes.value("type") == "station"){

                                if(memberAttributes.hasAttribute("ref")){

                                    dependencies.addFeatureID(memberAttributes.value("ref").toInt(),"station");

                                }

                            }

                        }

                    }
                    /* ...and next... */
                    xml.readNext();
                }

            }

        }
        /* ...and next... */
        xml.readNext();
    }

    return dependencies;
}

bool Observation::writeProxyObservations(QXmlStreamWriter &stream){

        stream.writeStartElement("member");
        stream.writeAttribute("type", "observation");
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

                                if(attributes.value("type").toString() == "angleXZ"){

                                    if(attributes.hasAttribute("value")){
                                        r->rLevel.angleXZ = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rLevel.sigmaAngleXZ = attributes.value("sigma").toDouble();
                                    }

                                }
                                if(attributes.value("type").toString() == "angleYZ"){

                                    if(attributes.hasAttribute("value")){
                                        r->rLevel.angleYZ = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rLevel.sigmaAngleYZ = attributes.value("sigma").toDouble();
                                    }

                                }
                                if(attributes.value("type").toString() == "diffXY"){

                                    if(attributes.hasAttribute("value")){
                                        r->rLevel.diffXY = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){

                                    }

                                }
                                if(attributes.value("type").toString() == "diffXZ"){

                                    if(attributes.hasAttribute("value")){
                                        r->rLevel.diffXZ = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){

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
