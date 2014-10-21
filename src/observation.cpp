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
 * \param stream
 * \return
 */
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
        stream.writeStartElement("station");
        stream.writeAttribute("ref", QString::number(this->myStation->getId()));
        stream.writeEndElement();
    }

    if(this->myReading != NULL){

        QString measuredAtTime = this->myReading->measuredAt.toString(Qt::ISODate);

        stream.writeStartElement("reading");
        stream.writeAttribute("id",QString::number(this->myReading->id));
        stream.writeAttribute("type",QString::number(this->myReading->typeofReading));
        stream.writeAttribute("time",measuredAtTime);

        stream.writeStartElement("measurements");

        switch(this->myReading->typeofReading){
            case(Configuration::ePolar) :{

            //get current date and time


                stream.writeStartElement("measurement");
                stream.writeAttribute("type","azimuth");
                stream.writeAttribute("value",QString::number(this->myReading->rPolar.azimuth));
                stream.writeAttribute("sigma",QString::number(this->myReading->rPolar.sigmaAzimuth));
                stream.writeEndElement();

                stream.writeStartElement("measurement");
                stream.writeAttribute("type","zenith");
                stream.writeAttribute("value",QString::number(this->myReading->rPolar.zenith));
                stream.writeAttribute("sigma",QString::number(this->myReading->rPolar.sigmaZenith));
                stream.writeEndElement();

                stream.writeStartElement("measurement");
                stream.writeAttribute("type","distance");
                stream.writeAttribute("value",QString::number(this->myReading->rPolar.distance));
                stream.writeAttribute("sigma",QString::number(this->myReading->rPolar.sigmaDistance));
                stream.writeEndElement();


                break;
            }
            case(Configuration::eDistance) :{

                stream.writeStartElement("measurement");
                stream.writeAttribute("type","distance");
                stream.writeAttribute("value",QString::number(this->myReading->rDistance.distance));
                stream.writeAttribute("sigma",QString::number(this->myReading->rDistance.sigmaDistance));
                stream.writeEndElement();

                break;
            }
            case(Configuration::eDirection) :{

                stream.writeStartElement("measurement");
                stream.writeAttribute("type","azimuth");
                stream.writeAttribute("value",QString::number(this->myReading->rDirection.azimuth));
                stream.writeAttribute("sigma",QString::number(this->myReading->rDirection.sigmaAzimuth));
                stream.writeEndElement();

                stream.writeStartElement("measurement");
                stream.writeAttribute("type","zenith");
                stream.writeAttribute("value",QString::number(this->myReading->rDirection.zenith));
                stream.writeAttribute("sigma",QString::number(this->myReading->rDirection.sigmaZenith));
                stream.writeEndElement();

                break;
            }
            case(Configuration::eCartesian) :{

                stream.writeStartElement("measurement");
                stream.writeAttribute("type","x");
                stream.writeAttribute("value",QString::number(this->myReading->rCartesian.xyz.getAt(0)));
                stream.writeAttribute("sigma",QString::number(this->myReading->rCartesian.sigmaXyz.getAt(0)));
                stream.writeEndElement();

                stream.writeStartElement("measurement");
                stream.writeAttribute("type","y");
                stream.writeAttribute("value",QString::number(this->myReading->rCartesian.xyz.getAt(1)));
                stream.writeAttribute("sigma",QString::number(this->myReading->rCartesian.sigmaXyz.getAt(1)));
                stream.writeEndElement();

                stream.writeStartElement("measurement");
                stream.writeAttribute("type","z");
                stream.writeAttribute("value",QString::number(this->myReading->rCartesian.xyz.getAt(2)));
                stream.writeAttribute("sigma",QString::number(this->myReading->rCartesian.sigmaXyz.getAt(2)));
                stream.writeEndElement();


                break;
            }
            case(Configuration::eLevel) :{

                stream.writeStartElement("measurement");
                stream.writeAttribute("type","RX");
                stream.writeAttribute("value",QString::number(this->myReading->rLevel.RX));
                stream.writeAttribute("sigma",QString::number(this->myReading->rLevel.sigmaRX));
                stream.writeEndElement();

                stream.writeStartElement("measurement");
                stream.writeAttribute("type","RY");
                stream.writeAttribute("value",QString::number(this->myReading->rLevel.RY));
                stream.writeAttribute("sigma",QString::number(this->myReading->rLevel.sigmaRY));
                stream.writeEndElement();

                stream.writeStartElement("measurement");
                stream.writeAttribute("type","RZ");
                stream.writeAttribute("value",QString::number(this->myReading->rLevel.RZ));
                stream.writeAttribute("sigma",QString::number(this->myReading->rLevel.sigmaRZ));
                stream.writeEndElement();


                break;
        }
        case(Configuration::eUndefined) :{

            QMapIterator<QString, double > j(this->myReading->rUndefined.values);
            while (j.hasNext()) {
                j.next();

                stream.writeStartElement("measurement");
                stream.writeAttribute("type",j.key());
                stream.writeAttribute("value",QString::number(j.value()));
                stream.writeAttribute("sigma",QString::number(this->myReading->rUndefined.sigmaValues.value(j.key())));
                stream.writeEndElement();

            }

                break;
        }
        }

        stream.writeEndElement();

        stream.writeEndElement();
    }


    stream.writeEndElement();
    return true;
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
