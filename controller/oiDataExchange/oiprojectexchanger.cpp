#include "oiprojectexchanger.h"

#include <QFile>

QList<Observation*> oiProjectExchanger::observations;
QList<FeatureWrapper*> oiProjectExchanger::geometries;
QList<FeatureWrapper*> oiProjectExchanger::stations;
QList<FeatureWrapper*> oiProjectExchanger::coordSystems;
QList<FeatureWrapper*> oiProjectExchanger::trafoParams;
QList<FeatureWrapper*> oiProjectExchanger::features;
QList<ElementDependencies> oiProjectExchanger::dependencies;

QList<int> oiProjectExchanger::stationElements;

oiProjectExchanger::oiProjectExchanger()
{
}


bool oiProjectExchanger::saveProject(oiProjectData &data){

    if (!data.device->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    oiProjectExchanger::observations.clear();
    oiProjectExchanger::geometries.clear();
    oiProjectExchanger::stations.clear();
    oiProjectExchanger::coordSystems.clear();
    oiProjectExchanger::trafoParams.clear();
    oiProjectExchanger::features.clear();

    //get current date and time
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString(Qt::ISODate);


    QXmlStreamWriter stream(data.device);

    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("oiProjectData");
    stream.writeAttribute("name", data.projectName);
    stream.writeAttribute("date", dateTimeString);
    stream.writeAttribute("idcount", QString::number(Configuration::idCount));

    stream.writeStartElement("member");
    stream.writeAttribute("type", "activeCoordinatesystem");
    stream.writeAttribute("ref", QString::number(data.activeCoordSystem->id));
    stream.writeEndElement();

    for(int i = 0; i<data.features.size();i++){

     if(data.features.at(i)->getGeometry() != NULL ){

        oiProjectExchanger::geometries.append(data.features.at(i));

     }else if(data.features.at(i)->getCoordinateSystem() != NULL){

        oiProjectExchanger::coordSystems.append(data.features.at(i));

      }else if (data.features.at(i)->getStation() != NULL){

         oiProjectExchanger::stations.append(data.features.at(i));

         FeatureWrapper* stationCoord = new FeatureWrapper;
         FeatureWrapper* stationPosition = new FeatureWrapper;

         stationCoord->setCoordinateSystem(data.features.at(i)->getStation()->coordSys);
         stationPosition->setPoint(data.features.at(i)->getStation()->position);

         oiProjectExchanger::coordSystems.append(stationCoord);
         oiProjectExchanger::geometries.append(stationPosition);

      }else if(data.features.at(i)->getTrafoParam() != NULL){

         oiProjectExchanger::trafoParams.append(data.features.at(i));

       }
    }


    //write stations to xml
    foreach(FeatureWrapper* s, oiProjectExchanger::stations){
        s->getStation()->toOpenIndyXML(stream);
    }

    //write coordnatesystem to xml
    foreach(FeatureWrapper* c, oiProjectExchanger::coordSystems){
        c->getCoordinateSystem()->toOpenIndyXML(stream);

        //harvest all observations
        foreach(Observation* obs, c->getCoordinateSystem()->observations){
            oiProjectExchanger::observations.append(obs);
        }
    }

    //write all trafoParam to xml
    foreach(FeatureWrapper* t, oiProjectExchanger::trafoParams){
        t->getTrafoParam()->toOpenIndyXML(stream);
    }

    //write all geometries to xml
    foreach(FeatureWrapper* g, oiProjectExchanger::geometries){
        g->getGeometry()->toOpenIndyXML(stream);
    }

    //write all observations to xml
    foreach(Observation* o, oiProjectExchanger::observations){
        o->toOpenIndyXML(stream);
    }

    stream.writeEndElement();
    stream.writeEndDocument();

    data.device->close();

    return true;
}

bool oiProjectExchanger::loadProject(oiProjectData &data){

    if (!data.device->open(QIODevice::ReadOnly | QIODevice::Text)) {

        return false;
    }

    oiProjectExchanger::observations.clear();
    oiProjectExchanger::geometries.clear();
    oiProjectExchanger::stations.clear();
    oiProjectExchanger::coordSystems.clear();
    oiProjectExchanger::trafoParams.clear();
    oiProjectExchanger::features.clear();

    QXmlStreamReader xml(data.device);

    while(!xml.atEnd() &&
            !xml.hasError()) {

        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if(token == QXmlStreamReader::StartElement) {

            qDebug() << xml.name();

            if(xml.name() == "observation") {
                Observation *o = new Observation(NULL,NULL);
                ElementDependencies d =  o->fromOpenIndyXML(xml);

                oiProjectExchanger::dependencies.append(d);
                oiProjectExchanger::observations.append(o);
            }

            if(xml.name() == "oiProjectData"){

            }

            if(xml.name() == "station"){

                Station *s = new Station("");
                ElementDependencies d = s->fromOpenIndyXML(xml);

                FeatureWrapper *fs = new FeatureWrapper();
                fs->setStation(s);

                oiProjectExchanger::stations.append(fs);
                oiProjectExchanger::dependencies.append(d);

            }

            if(xml.name() == "coordinatesystem"){

                CoordinateSystem *cs = new CoordinateSystem();
                ElementDependencies d = cs->fromOpenIndyXML(xml);

                FeatureWrapper *fs = new FeatureWrapper();
                fs->setCoordinateSystem(cs);

                oiProjectExchanger::coordSystems.append(fs);
                oiProjectExchanger::dependencies.append(d);

            }

            if(xml.name() == "geometry"){
                FeatureWrapper *fg = oiProjectExchanger::parseGeometry(xml);
                if(fg!=NULL){
                    oiProjectExchanger::geometries.append(fg);
                    features.append(fg);
                }
            }

            if(xml.name() == "transformationparameter"){
                FeatureWrapper *ft = oiProjectExchanger::parseTrafoPara(xml);
                if(ft!=NULL){
                    oiProjectExchanger::trafoParams.append(ft);
                    features.append(ft);
                }
            }
        }
    }

    if(xml.hasError()) {

        Console::addLine(QString("xml not valid: " + xml.errorString()));
        data.device->close();
        return false;
    }

     data.device->close();
     oiProjectExchanger::regenerateRelations(data);
     return true;


    return false;
}


bool oiProjectExchanger::saveSettings(QString name){


    return false;
}


bool oiProjectExchanger::regenerateRelations(oiProjectData &data){

    foreach (FeatureWrapper *fg, oiProjectExchanger::geometries){


        if(oiProjectExchanger::regenerateFeature(fg->getFeature())){
            //TODO error handling
        }

        for(int i = 0; i< fg->getGeometry()->myObservations.size();i++){
            Observation* proxyObs = oiProjectExchanger::findObservation(fg->getGeometry()->myObservations.at(i)->id);
            if(proxyObs != NULL){
               fg->getGeometry()->myObservations.replace(i,proxyObs);
            }
        }

        for(int i = 0; i< fg->getGeometry()->nominals.size();i++){
            Geometry* proxyGeom = oiProjectExchanger::findGeometrie(fg->getGeometry()->nominals.at(i)->id);
            if(proxyGeom != NULL){
               fg->getGeometry()->nominals.replace(i,proxyGeom);
            }
        }

        if(fg->getGeometry()->myNominalCoordSys != NULL){
            CoordinateSystem* proxyCoord= oiProjectExchanger::findCoordSys(fg->getGeometry()->myNominalCoordSys->id);
            if(proxyCoord != NULL){
               fg->getGeometry()->myNominalCoordSys = proxyCoord;
            }
        }

        if(!oiProjectExchanger::stationElements.contains(fg->getGeometry()->id)){
            data.features.append(fg);
        }


    }

    foreach (FeatureWrapper *fs, oiProjectExchanger::stations){



        if(oiProjectExchanger::regenerateFeature(fs->getFeature())){
            //TODO error handling
        }

        FeatureWrapper* proxyPoint= oiProjectExchanger::findFeature(fs->getStation()->position->id);
        if(proxyPoint != NULL){
           fs->getStation()->position = proxyPoint->getPoint();
        }

        if(fs->getStation()->coordSys!=NULL){
            CoordinateSystem* proxyCoord= oiProjectExchanger::findCoordSys(fs->getStation()->coordSys->id);
            if(proxyCoord != NULL){
               fs->getStation()->coordSys = proxyCoord;
            }
        }


        data.features.append(fs);

        data.stations.append(fs->getStation());
    }

    foreach (Observation *obs, oiProjectExchanger::observations){

        Station* proxyStation= oiProjectExchanger::findStation(obs->myStation->id);
        if(proxyStation != NULL){
           obs->myStation = proxyStation;
           obs->myReading->instrument = proxyStation->instrument;
        }

    }

    foreach (FeatureWrapper *fc, oiProjectExchanger::coordSystems){


        if(oiProjectExchanger::regenerateFeature(fc->getFeature())){
            //TODO error handling
        }

        for(int i = 0; i< fc->getCoordinateSystem()->observations.size();i++){
            Observation* proxyObs = oiProjectExchanger::findObservation(fc->getCoordinateSystem()->observations.at(i)->id);
            if(proxyObs != NULL){
               fc->getCoordinateSystem()->observations.replace(i,proxyObs);
            }
        }

        for(int i = 0; i< fc->getCoordinateSystem()->nominals.size();i++){
            Geometry* proxyGeom = oiProjectExchanger::findGeometrie(fc->getCoordinateSystem()->nominals.at(i)->id);
            if(proxyGeom != NULL){
               fc->getCoordinateSystem()->nominals.replace(i,proxyGeom);
            }
        }

        for(int i = 0; i< fc->getCoordinateSystem()->trafoParams.size();i++){
            TrafoParam* proxyTrafoParam = oiProjectExchanger::findTrafoParam(fc->getCoordinateSystem()->trafoParams.at(i)->id);
            if(proxyTrafoParam != NULL){
               fc->getCoordinateSystem()->trafoParams.replace(i,proxyTrafoParam);
            }
        }


        if(!oiProjectExchanger::stationElements.contains(fc->getCoordinateSystem()->id)){
            data.features.append(fc);
            data.coordSystems.append(fc->getCoordinateSystem());
        }


    }

    foreach (FeatureWrapper *ft, oiProjectExchanger::trafoParams){



        if(oiProjectExchanger::regenerateFeature(ft->getFeature())){
            //TODO error handling
        }


        if(ft->getTrafoParam()->from !=NULL){
            CoordinateSystem* proxyCoord= oiProjectExchanger::findCoordSys(ft->getTrafoParam()->from->id);
            if(proxyCoord != NULL){
               ft->getTrafoParam()->from = proxyCoord;
            }
        }

        if(ft->getTrafoParam()->to !=NULL){
            CoordinateSystem* proxyCoord= oiProjectExchanger::findCoordSys(ft->getTrafoParam()->to->id);
            if(proxyCoord != NULL){
               ft->getTrafoParam()->to = proxyCoord;
            }
        }


        data.features.append(ft);

    }

    //sort
    qSort(data.features.begin(), data.features.end(), sortID);

    return true;
}

Observation* oiProjectExchanger::findObservation(int id){

    foreach(Observation *obs, oiProjectExchanger::observations){
        if (obs->id == id){
            return obs;
        }
    }
    return NULL;
}

Geometry* oiProjectExchanger::findGeometrie(int id){

    foreach(FeatureWrapper *g, oiProjectExchanger::geometries){
        if (g->getGeometry()->id == id){
            return g->getGeometry();
        }
    }
    return NULL;
}

CoordinateSystem* oiProjectExchanger::findCoordSys(int id){
    foreach(FeatureWrapper *g, oiProjectExchanger::coordSystems){
        if (g->getCoordinateSystem()->id == id){
            return g->getCoordinateSystem();
        }
    }
    return NULL;
}

TrafoParam* oiProjectExchanger::findTrafoParam(int id){
    foreach(FeatureWrapper *g, oiProjectExchanger::trafoParams){
        if (g->getTrafoParam()->id == id){
            return g->getTrafoParam();
        }
    }
    return NULL;
}

FeatureWrapper* oiProjectExchanger::findFeature(int id){

    foreach(FeatureWrapper *f, oiProjectExchanger::features){
        if (f->getFeature()->id == id){
            return f;
        }
    }
    return NULL;
}

Station* oiProjectExchanger::findStation(int id){

    foreach(FeatureWrapper *f, oiProjectExchanger::stations){
        if (f->getStation()->id == id){
            return f->getStation();
        }
    }
    return NULL;

}

bool oiProjectExchanger::regenerateFeature(Feature *f){

    for(int i = 0;i<f->usedFor.size();i++){
        FeatureWrapper *proxyFw = oiProjectExchanger::findFeature(f->usedFor.at(i)->getFeature()->id);
        if(proxyFw != NULL){
            f->usedFor.replace(i, proxyFw);
        }
    }

    for(int i = 0;i<f->previouslyNeeded.size();i++){
        FeatureWrapper *proxyFw = oiProjectExchanger::findFeature(f->previouslyNeeded.at(i)->getFeature()->id);
        if(proxyFw != NULL){
            f->previouslyNeeded.replace(i,proxyFw);
        }
    }

    return true;
}
