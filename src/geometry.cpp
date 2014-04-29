#include "geometry.h"

#include "coordinatesystem.h"
#include "observation.h"
#include "station.h"

Geometry::Geometry() : myActual(NULL){

}

Geometry::~Geometry(){

    if(this->isNominal){
        //delete this geometry from the nominal list of its actual
        if(this->myActual != NULL){
            int index = -1;
            int k = 0;
            foreach(Geometry *checkNominal, this->myActual->nominals){
                if(checkNominal != NULL && checkNominal->id == this->id){
                    index = k;
                    break;
                }
                k++;
            }
            if(index >= 0){
                this->myActual->nominals.removeAt(index);
            }
        }

        //delete this geometry from list of nominals in myNominalCoordSys
        if(this->myNominalCoordSys != NULL){
            this->myNominalCoordSys->nominals.removeOne(this);
        }
    }else{
        //delete observations that only belong to this geometry
        for(int i = 0; i < this->myObservations.size(); i++){
            Observation *myObs = this->myObservations.at(i);
            if(myObs->myTargetGeometries.size() == 1){
                Station *myStation = myObs->myStation;
                if(myStation != NULL && myStation->coordSys != NULL){
                    myStation->coordSys->observations.removeOne(myObs);
                }
            }
            delete myObs;
        }

        //delete property myActual from all nominal geometries which belong to this actual geometry
        foreach(Geometry *checkNominal, this->nominals){
            if(checkNominal != NULL){
                checkNominal->myActual = NULL;
            }
        }
    }

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
    return true;
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







