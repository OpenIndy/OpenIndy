#include "geometry.h"

#include "observation.h"
#include "coordinatesystem.h"

Geometry::~Geometry(){


}

void Geometry::insertReadingType(Configuration::ReadingTypes readingType, QString displayName){

    QMap<Configuration::ReadingTypes,QString>::const_iterator i = usedReadingTypes.find(readingType);

    if (i != usedReadingTypes.end() && i.key() != readingType) {
        usedReadingTypes.insert(readingType,displayName);
    }
}

bool Geometry::writeGeometryAttributes(QXmlStreamWriter &stream){

    foreach (Observation *obs, myObservations) {
        obs->writeProxyObservations(stream);
    }

    foreach (Geometry *geom, this->nominals) {

            stream.writeStartElement("member");
            stream.writeAttribute("type", "nominalGeometry");
            stream.writeAttribute("ref", QString::number(geom->id));
            stream.writeEndElement();

    }

    if(this->myNominalCoordSys != NULL){
        stream.writeStartElement("member");
        stream.writeAttribute("type", "coordinatesystem");
        stream.writeAttribute("ref", QString::number(this->myNominalCoordSys->id));
        stream.writeEndElement();
    }

    if(!this->isNominal){
        stream.writeStartElement("measurementconfig");
        stream.writeAttribute("name", this->mConfig.name);
        //TODO this->mConfig.writeMeasurementConfig(stream);
        stream.writeEndElement();
    }

    this->writeFeatureAttributes(stream);

}

bool Geometry::readGeometryAttributes(QXmlStreamReader &xml, ElementDependencies &dependencies){


    if(xml.name() == "standardDeviation") {

            if(xml.tokenType() == QXmlStreamReader::StartElement) {

                QXmlStreamAttributes sigmaAttributes = xml.attributes();

                    if(sigmaAttributes.hasAttribute("value")){
                       this->myStatistic.stdev = sigmaAttributes.value("value").toDouble();
                    }

             }
    }

    if(xml.name() == "measurementconfig") {

        while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                xml.name() == "measurementconfig")) {
            if(xml.tokenType() == QXmlStreamReader::StartElement) {

                QXmlStreamAttributes measurementconfigAttributes = xml.attributes();

               if(!this->isNominal){

                    if(measurementconfigAttributes.hasAttribute("name")){
                      //TODO MeasurementConfig setzten
                    }

                 }
             }
         xml.readNext();
        }
    }



    if(xml.name() == "member"){

        while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                xml.name() == "member")) {
            if(xml.tokenType() == QXmlStreamReader::StartElement) {


                QXmlStreamAttributes memberAttributes = xml.attributes();

                if(memberAttributes.hasAttribute("type")){

                    if (memberAttributes.value("type") == "observation"){

                        if(memberAttributes.hasAttribute("ref")){
                            dependencies.addObservationID(memberAttributes.value("ref").toInt());
                        }

                    }else if (memberAttributes.value("type") == "coordinatesystem"){

                        if(memberAttributes.hasAttribute("ref")){
                            dependencies.addFeatureID(memberAttributes.value("ref").toInt(),"coordinatesystem");
                        }
                    }else if (memberAttributes.value("type") == "nominalGeometry"){

                        if(memberAttributes.hasAttribute("ref")){
                            dependencies.addFeatureID(memberAttributes.value("ref").toInt(),"nominalGeometry");
                        }
                    }else{
                        this->readFeatureAttributes(xml,dependencies);
                    }

                }

            }
            /* ...and next... */
            xml.readNext();
        }
    }

    if(xml.name() == "function"){

       this->readFunction(xml, dependencies);

    }

    return true;
}







