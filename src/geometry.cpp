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







