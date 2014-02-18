#include "oiprojectexchanger.h"

#include <QFile>

QList<Observation*> oiProjectExchanger::observations;
QList<FeatureWrapper*> oiProjectExchanger::geometries;
QList<FeatureWrapper*> oiProjectExchanger::stations;
QList<FeatureWrapper*> oiProjectExchanger::coordSystems;
QList<FeatureWrapper*> oiProjectExchanger::trafoParams;
QList<FeatureWrapper*> oiProjectExchanger::features;

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

      }else if(data.features.at(i)->getTrafoParam() != NULL){

         oiProjectExchanger::trafoParams.append(data.features.at(i));

       }
    }

    oiProjectExchanger::parseStationsToXML(stream);
    oiProjectExchanger::parseCoordSysToXML(stream);
    oiProjectExchanger::parseTrafoParamToXML(stream);
    oiProjectExchanger::parseGeometryToXML(stream);
    oiProjectExchanger::parseObservationToXML(stream);

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
                Observation *o = oiProjectExchanger::parseObservation(xml);
                if(o!=NULL){
                    oiProjectExchanger::observations.append(o);
                }
            }

            if(xml.name() == "oiProjectData"){

            }

            if(xml.name() == "station"){
                FeatureWrapper *fs = oiProjectExchanger::parseStation(xml);
                if(fs!= NULL){
                    oiProjectExchanger::stations.append(fs);
                    features.append(fs);
                }
            }

            if(xml.name() == "coordinatesystem"){
                FeatureWrapper *fc = oiProjectExchanger::parseCoordinatesystem(xml);
                if(fc!=NULL){
                    oiProjectExchanger::coordSystems.append(fc);
                    features.append(fc);
                }
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

bool oiProjectExchanger::saveSensorConfig(QString name){


    return false;
}

SensorConfiguration* oiProjectExchanger::loadSensorConfig(QString name){

    return NULL;
}

bool oiProjectExchanger::saveMeasurementConfig(QString name){


    return false;
}

MeasurementConfig* oiProjectExchanger::loadMeasurementConfig(QString name){

    return NULL;
}

bool oiProjectExchanger::saveSettings(QString name){


    return false;
}

bool oiProjectExchanger::parseCommonFeatureToXML(QXmlStreamWriter& stream, Feature *f){

    for(int k =0;k<f ->usedFor.size();k++){
        stream.writeStartElement("member");
        stream.writeAttribute("type", "usedForFeature");
        stream.writeAttribute("ref", QString::number(f->usedFor.at(k)->getFeature()->id));
        stream.writeEndElement();
    }

    for(int k =0;k<f->previouslyNeeded.size();k++){
        stream.writeStartElement("member");
        stream.writeAttribute("type", "previouslyNeeded");
        stream.writeAttribute("ref", QString::number(f->previouslyNeeded.at(k)->getFeature()->id));
        stream.writeEndElement();
    }

    for(int k =0;k<f->functionList.size();k++){
        stream.writeStartElement("function");
        stream.writeAttribute("name", f->functionList.at(k)->getMetaData()->name);
        stream.writeAttribute("plugin", f->functionList.at(k)->getMetaData()->pluginName);
        stream.writeAttribute("executionIndex", QString::number(k));

        QMapIterator<int, QList<InputFeature> > j(f->functionList.at(k)->getFeatureOrder());
        while (j.hasNext()) {
            j.next();

            for(int m = 0; m < j.value().size();m++){
                stream.writeStartElement("inputElement");
                stream.writeAttribute("index", QString::number(j.key()));
                int neededFeatureId = j.value().at(m).id;
                stream.writeAttribute("type", QString::number( j.value().at(m).typeOfElement));
                stream.writeAttribute("ref", QString::number(neededFeatureId));
                stream.writeEndElement();
            }


        }

        stream.writeEndElement();
    }

    return true;

}

bool oiProjectExchanger::saveProxyObservation(QXmlStreamWriter& stream,QList<Observation*> o){
    for(int k =0;k<o.size();k++){
        stream.writeStartElement("member");
        stream.writeAttribute("type", "observation");
        stream.writeAttribute("ref", QString::number(o.at(k)->id));
        stream.writeEndElement();
        if(!observations.contains(o.at(k))){
            observations.append(o.at(k));
        }
     }
    return true;
}

bool oiProjectExchanger::parseNominalGeometry(QXmlStreamWriter& stream,QList<Geometry*> g){

    for(int k =0;k<g.size();k++){
        stream.writeStartElement("member");
        stream.writeAttribute("type", "nominalGeometry");
        stream.writeAttribute("ref", QString::number(g.at(k)->id));
        stream.writeEndElement();
    }
    return true;
}

bool oiProjectExchanger::parseStationsToXML(QXmlStreamWriter& stream){

    for(int i = 0; i<oiProjectExchanger::stations.size();i++){

        Station *s = oiProjectExchanger::stations.at(i)->getStation();

        stream.writeStartElement("station");
        stream.writeAttribute("id", QString::number(s->id));
        stream.writeAttribute("name", s->name);

        if(s->instrument != NULL){
            stream.writeStartElement("sensor");
            stream.writeAttribute("name",s->instrument->getMetaData()->name);
            stream.writeAttribute("plugin", s->instrument->getMetaData()->pluginName);
            stream.writeEndElement();
        }

        if(s->position != NULL){

            stream.writeStartElement("member");
            stream.writeAttribute("type", "position");
            stream.writeAttribute("ref", QString::number(s->position->id));
            stream.writeEndElement();

            FeatureWrapper* tp = new FeatureWrapper();
            tp->setPoint(s->position);

            if(!geometries.contains(tp)){
                geometries.append(tp);
            }


        }

        if(s->coordSys != NULL){
            stream.writeStartElement("member");
            stream.writeAttribute("type", "coordinatesystem");
            stream.writeAttribute("ref", QString::number(s->coordSys->id));
            stream.writeEndElement();

            FeatureWrapper* tp = new FeatureWrapper();
            tp->setCoordinateSystem(s->coordSys);

            if(!coordSystems.contains(tp)){
                coordSystems.append(tp);
            }

        }

        oiProjectExchanger::parseCommonFeatureToXML(stream,oiProjectExchanger::stations.at(i)->getFeature());

        stream.writeEndElement();
    }

    return true;
}


bool oiProjectExchanger::parseCoordSysToXML(QXmlStreamWriter& stream){

    for(int i = 0; i<coordSystems.size();i++){

        CoordinateSystem *c = coordSystems.at(i)->getCoordinateSystem();

        stream.writeStartElement("coordinatesystem");
        stream.writeAttribute("id", QString::number(c->id));
        stream.writeAttribute("name", c->name);
        stream.writeAttribute("solved", QString::number(c->isSolved));

            oiProjectExchanger::saveProxyObservation(stream,c->observations);

            oiProjectExchanger::parseNominalGeometry(stream,c->nominals);

            for(int k =0;k<c->trafoParams.size();k++){
                stream.writeStartElement("member");
                stream.writeAttribute("type", "transformationParameter");
                stream.writeAttribute("ref", QString::number(c->trafoParams.at(k)->id));

                FeatureWrapper* tp = new FeatureWrapper();
                tp->setTrafoParam(c->trafoParams.at(k));

                if(!trafoParams.contains(tp)){
                    trafoParams.append(tp);
                }

                stream.writeEndElement();
            }

            oiProjectExchanger::parseCommonFeatureToXML(stream,oiProjectExchanger::coordSystems.at(i)->getFeature());

            stream.writeEndElement();


    }
    return true;
}

bool oiProjectExchanger::parseTrafoParamToXML(QXmlStreamWriter& stream){

    for(int i = 0; i<trafoParams.size();i++){

        TrafoParam *t = trafoParams.at(i)->getTrafoParam();

        stream.writeStartElement("transformationsparameter");
        stream.writeAttribute("id", QString::number(t->id));
        stream.writeAttribute("name", t->name);
        stream.writeAttribute("solved", QString::number(t->isSolved));
        stream.writeAttribute("tx", QString::number(t->translation.getAt(0)));
        stream.writeAttribute("ty", QString::number(t->translation.getAt(1)));
        stream.writeAttribute("tz", QString::number(t->translation.getAt(2)));
        stream.writeAttribute("rx", QString::number(t->rotation.getAt(0)));
        stream.writeAttribute("ry", QString::number(t->rotation.getAt(1)));
        stream.writeAttribute("rz", QString::number(t->rotation.getAt(2)));
        stream.writeAttribute("mx", QString::number(t->scale.getAt(0)));
        stream.writeAttribute("my", QString::number(t->scale.getAt(1)));
        stream.writeAttribute("mz", QString::number(t->scale.getAt(2)));


        stream.writeStartElement("from");
        stream.writeAttribute("type", "coordinatesystem");
        stream.writeAttribute("ref", QString::number(t->from->id));
        stream.writeEndElement();

        stream.writeStartElement("to");
        stream.writeAttribute("type", "coordinatesystem");
        stream.writeAttribute("ref", QString::number(t->to->id));
        stream.writeEndElement();


        oiProjectExchanger::parseCommonFeatureToXML(stream,trafoParams.at(i)->getFeature());


        stream.writeEndElement();


    }
    return true;
}

bool oiProjectExchanger::parseGeometryToXML(QXmlStreamWriter& stream){

    for(int i = 0; i<geometries.size();i++){

        Geometry *g = geometries.at(i)->getGeometry();

        stream.writeStartElement("geometry");
        stream.writeAttribute("id", QString::number(g->id));
        stream.writeAttribute("name", g->name);
        stream.writeAttribute("type", geometries.at(i)->returnFeatureType());
        stream.writeAttribute("nominal",QString::number(g->isNominal));
        stream.writeAttribute("common",QString::number(g->isCommon));
        stream.writeAttribute("solved", QString::number(g->isSolved));


        if(g->isSolved || g->isNominal){
            if(geometries.at(i)->getPoint() != NULL){
                stream.writeStartElement("coordinates");
                stream.writeAttribute("x", QString::number(geometries.at(i)->getPoint()->xyz.getAt(0)));
                stream.writeAttribute("y", QString::number(geometries.at(i)->getPoint()->xyz.getAt(1)));
                stream.writeAttribute("z", QString::number(geometries.at(i)->getPoint()->xyz.getAt(2)));
                stream.writeEndElement();

                stream.writeStartElement("standardDeviation");
                stream.writeAttribute("value", QString::number(geometries.at(i)->getPoint()->myStatistic.stdev));
                stream.writeEndElement();
            }else if(geometries.at(i)->getLine() != NULL){
                stream.writeStartElement("coordinates");
                stream.writeAttribute("x", QString::number(geometries.at(i)->getLine()->xyz.getAt(0)));
                stream.writeAttribute("y", QString::number(geometries.at(i)->getLine()->xyz.getAt(1)));
                stream.writeAttribute("z", QString::number(geometries.at(i)->getLine()->xyz.getAt(2)));
                stream.writeEndElement();

                stream.writeStartElement("spatialDirection");
                stream.writeAttribute("i", QString::number(geometries.at(i)->getLine()->ijk.getAt(0)));
                stream.writeAttribute("j", QString::number(geometries.at(i)->getLine()->ijk.getAt(1)));
                stream.writeAttribute("k", QString::number(geometries.at(i)->getLine()->ijk.getAt(2)));
                stream.writeEndElement();

                stream.writeStartElement("standardDeviation");
                stream.writeAttribute("value", QString::number(geometries.at(i)->getLine()->myStatistic.stdev));
                stream.writeEndElement();
            }else if(geometries.at(i)->getPlane() != NULL){
                stream.writeStartElement("coordinates");
                stream.writeAttribute("x", QString::number(geometries.at(i)->getPlane()->xyz.getAt(0)));
                stream.writeAttribute("y", QString::number(geometries.at(i)->getPlane()->xyz.getAt(1)));
                stream.writeAttribute("z", QString::number(geometries.at(i)->getPlane()->xyz.getAt(2)));
                stream.writeEndElement();

                stream.writeStartElement("spatialDirection");
                stream.writeAttribute("i", QString::number(geometries.at(i)->getPlane()->ijk.getAt(0)));
                stream.writeAttribute("j", QString::number(geometries.at(i)->getPlane()->ijk.getAt(1)));
                stream.writeAttribute("k", QString::number(geometries.at(i)->getPlane()->ijk.getAt(2)));
                stream.writeEndElement();

                stream.writeStartElement("standardDeviation");
                stream.writeAttribute("value", QString::number(geometries.at(i)->getPlane()->myStatistic.stdev));
                stream.writeEndElement();

            }else if(geometries.at(i)->getSphere() != NULL){

                stream.writeStartElement("coordinates");
                stream.writeAttribute("x", QString::number(geometries.at(i)->getSphere()->xyz.getAt(0)));
                stream.writeAttribute("y", QString::number(geometries.at(i)->getSphere()->xyz.getAt(1)));
                stream.writeAttribute("z", QString::number(geometries.at(i)->getSphere()->xyz.getAt(2)));
                stream.writeEndElement();

                stream.writeStartElement("radius");
                stream.writeAttribute("value", QString::number(geometries.at(i)->getSphere()->radius));
                stream.writeEndElement();

                stream.writeStartElement("standardDeviation");
                stream.writeAttribute("value", QString::number(geometries.at(i)->getSphere()->myStatistic.stdev));
                stream.writeEndElement();

            }else if (geometries.at(i)->getScalarEntityAngle() != NULL){

                stream.writeStartElement("angle");
                stream.writeAttribute("value", QString::number(geometries.at(i)->getScalarEntityAngle()->getAngle()));
                stream.writeEndElement();

                stream.writeStartElement("standardDeviation");
                stream.writeAttribute("value", QString::number(geometries.at(i)->getScalarEntityAngle()->myStatistic.stdev));
                stream.writeEndElement();

            }else if (geometries.at(i)->getScalarEntityDistance() != NULL){

                stream.writeStartElement("distance");
                stream.writeAttribute("value", QString::number(geometries.at(i)->getScalarEntityDistance()->getDistance()));
                stream.writeEndElement();

                stream.writeStartElement("standardDeviation");
                stream.writeAttribute("value", QString::number(geometries.at(i)->getScalarEntityDistance()->myStatistic.stdev));
                stream.writeEndElement();

            }
        }


        oiProjectExchanger::saveProxyObservation(stream,g->myObservations);

        oiProjectExchanger::parseNominalGeometry(stream,g->nominals);

        if(g->myNominalCoordSys != NULL){
            stream.writeStartElement("member");
            stream.writeAttribute("type", "coordinatesystem");
            stream.writeAttribute("ref", QString::number(g->myNominalCoordSys->id));
            stream.writeEndElement();
        }

        if(!g->isNominal){
            stream.writeStartElement("measurementconfig");
            stream.writeAttribute("name", g->mConfig.name);
            stream.writeEndElement();
        }

       oiProjectExchanger::parseCommonFeatureToXML(stream,geometries.at(i)->getFeature());

        stream.writeEndElement();
    }
    return true;
}

bool oiProjectExchanger::parseObservationToXML(QXmlStreamWriter& stream){

    for (int i = 0; i < observations.size();i++){

        stream.writeStartElement("observation");
        stream.writeAttribute("id", QString::number(observations.at(i)->id));
        stream.writeAttribute("x", QString::number(observations.at(i)->myXyz.getAt(0)));
        stream.writeAttribute("y", QString::number(observations.at(i)->myXyz.getAt(1)));
        stream.writeAttribute("z", QString::number(observations.at(i)->myXyz.getAt(2)));
        stream.writeAttribute("isValid", QString::number(observations.at(i)->isValid));
        stream.writeAttribute("sigmaX", QString::number(observations.at(i)->sigmaXyz.getAt(0)));
        stream.writeAttribute("sigmaY", QString::number(observations.at(i)->sigmaXyz.getAt(1)));
        stream.writeAttribute("sigmaZ", QString::number(observations.at(i)->sigmaXyz.getAt(2)));


        if(observations.at(i)->myStation != NULL){
            stream.writeStartElement("member");
            stream.writeAttribute("type", "station");
            stream.writeAttribute("ref", QString::number(observations.at(i)->myStation->id));
            stream.writeEndElement();
        }

        if(observations.at(i)->myReading != NULL){
            stream.writeStartElement("reading");
            stream.writeAttribute("id",QString::number(observations.at(i)->myReading->id));
            stream.writeAttribute("type",QString::number(observations.at(i)->myReading->typeofReading));

            QString measuredAtTime = observations.at(i)->myReading->measuredAt.toString(Qt::ISODate);

            switch(observations.at(i)->myReading->typeofReading){
                case(Configuration::ePolar) :{

                //get current date and time


                    stream.writeStartElement("measurement");
                    stream.writeAttribute("time",measuredAtTime);
                    stream.writeAttribute("type","azimuth");
                    stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rPolar.azimuth));
                    stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rPolar.sigmaAzimuth));
                    stream.writeEndElement();

                    stream.writeStartElement("measurement");
                    stream.writeAttribute("time",measuredAtTime);
                    stream.writeAttribute("type","zenith");
                    stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rPolar.zenith));
                    stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rPolar.sigmaZenith));
                    stream.writeEndElement();

                    stream.writeStartElement("measurement");
                    stream.writeAttribute("time",measuredAtTime);
                    stream.writeAttribute("type","distance");
                    stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rPolar.distance));
                    stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rPolar.sigmaDistance));
                    stream.writeEndElement();


                    break;
                }
                case(Configuration::eDistance) :{

                    stream.writeStartElement("measurement");
                    stream.writeAttribute("time",measuredAtTime);
                    stream.writeAttribute("type","distance");
                    stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rDistance.distance));
                    stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rDistance.sigmaDistance));
                    stream.writeEndElement();

                    break;
                }
                case(Configuration::eDirection) :{

                    stream.writeStartElement("measurement");
                    stream.writeAttribute("time",measuredAtTime);
                    stream.writeAttribute("type","azimuth");
                    stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rDirection.azimuth));
                    stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rDirection.sigmaAzimuth));
                    stream.writeEndElement();

                    stream.writeStartElement("measurement");
                    stream.writeAttribute("time",measuredAtTime);
                    stream.writeAttribute("type","zenith");
                    stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rDirection.zenith));
                    stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rDirection.sigmaZenith));
                    stream.writeEndElement();

                    break;
                }
                case(Configuration::eCartesian) :{

                    stream.writeStartElement("measurement");
                    stream.writeAttribute("time",measuredAtTime);
                    stream.writeAttribute("type","x");
                    stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rCartesian.xyz.getAt(0)));
                    stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rCartesian.sigmaXyz.getAt(0)));
                    stream.writeEndElement();

                    stream.writeStartElement("measurement");
                    stream.writeAttribute("time",measuredAtTime);
                    stream.writeAttribute("type","y");
                    stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rCartesian.xyz.getAt(1)));
                    stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rCartesian.sigmaXyz.getAt(1)));
                    stream.writeEndElement();

                    stream.writeStartElement("measurement");
                    stream.writeAttribute("time",measuredAtTime);
                    stream.writeAttribute("type","z");
                    stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rCartesian.xyz.getAt(2)));
                    stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rCartesian.sigmaXyz.getAt(2)));
                    stream.writeEndElement();


                    break;
                }
                case(Configuration::eLevel) :{

                    stream.writeStartElement("measurement");
                    stream.writeAttribute("time",measuredAtTime);
                    stream.writeAttribute("type","angleXZ");
                    stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rLevel.angleXZ));
                    stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rLevel.sigmaAngleXZ));
                    stream.writeEndElement();

                    stream.writeStartElement("measurement");
                    stream.writeAttribute("time",measuredAtTime);
                    stream.writeAttribute("type","angleYZ");
                    stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rLevel.angleYZ));
                    stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rLevel.sigmaAngleYZ));
                    stream.writeEndElement();

                    stream.writeStartElement("measurement");
                    stream.writeAttribute("time",measuredAtTime);
                    stream.writeAttribute("type","diffXY");
                    stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rLevel.diffXY));
                    stream.writeAttribute("sigma","-");
                    stream.writeEndElement();

                    stream.writeStartElement("measurement");
                    stream.writeAttribute("time",measuredAtTime);
                    stream.writeAttribute("type","diffXZ");
                    stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rLevel.diffXZ));
                    stream.writeAttribute("sigma","-");
                    stream.writeEndElement();


                    break;
            }
            case(Configuration::eUndefined) :{

                QMapIterator<QString, double > j(observations.at(i)->myReading->rUndefined.values);
                while (j.hasNext()) {
                    j.next();

                    stream.writeStartElement("measurement");
                    stream.writeAttribute("time",measuredAtTime);
                    stream.writeAttribute("type",j.key());
                    stream.writeAttribute("value",QString::number(j.value()));
                    stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rUndefined.sigmaValues.value(j.key())));
                    stream.writeEndElement();

                }

                    break;
            }
            }

            stream.writeEndElement();
        }


        stream.writeEndElement();

    }
    return true;
}

Observation* oiProjectExchanger::parseObservation(QXmlStreamReader& xml) {

    Observation* obs = new Observation(NULL,NULL);


    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("id")) {
        obs->id = attributes.value("id").toInt();
    }
    if(attributes.hasAttribute("x")){
        obs->myOriginalXyz.setAt(0,attributes.value("x").toDouble());
    }
    if(attributes.hasAttribute("y")){
        obs->myOriginalXyz.setAt(1,attributes.value("y").toDouble());
    }
    if(attributes.hasAttribute("z")){
        obs->myOriginalXyz.setAt(2,attributes.value("z").toDouble());
    }
    if(attributes.hasAttribute("isValid")){
        obs->isValid = attributes.value("isVaild").toInt();
    }
    if(attributes.hasAttribute("sigmaX")){
        obs->sigmaXyz.setAt(0,attributes.value("sigmaX").toDouble());
    }
    if(attributes.hasAttribute("sigmaY")){
        obs->sigmaXyz.setAt(1,attributes.value("sigmaY").toDouble());
    }
    if(attributes.hasAttribute("sigmaZ")){
        obs->sigmaXyz.setAt(2,attributes.value("sigmaZ").toDouble());
    }
    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "observation")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */
            if(xml.name() == "reading") {
                Reading *r = oiProjectExchanger::parseReading(xml);
                r->obs = obs;
                obs->myReading = r;
            }

            if(xml.name() == "member"){

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "member")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes memberAttributes = xml.attributes();

                        if(memberAttributes.hasAttribute("type")){

                            if (memberAttributes.value("type") == "station"){

                                if(memberAttributes.hasAttribute("ref")){
                                    obs->myStation = new Station("proxy");
                                    obs->myStation->id= memberAttributes.value("ref").toInt();
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
    return obs;
}

Reading* oiProjectExchanger::parseReading(QXmlStreamReader& xml){

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
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "reading")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */

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

        Console::addLine(QString("xml not valid: " + xml.errorString()));
        return r;
    }

    return r;

}

FeatureWrapper* oiProjectExchanger::parseStation(QXmlStreamReader& xml){

    FeatureWrapper* fw = new FeatureWrapper();
    Station *s = NULL;
    SensorConfiguration *sc = new SensorConfiguration();

    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("name")){
        s = new Station(attributes.value("name").toString());
    }
    if(attributes.hasAttribute("id")) {
        s->id = attributes.value("id").toInt();
    }

    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "station")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */
            if(xml.name() == "sensor") {

                QString sensorName;
                QString sensorPlugin;

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "sensor")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {


                        QXmlStreamAttributes sensorAttributes = xml.attributes();

                        if(sensorAttributes.hasAttribute("name")){
                            sensorName = sensorAttributes.value("name").toString();
                        }
                        if(sensorAttributes.hasAttribute("plugin")) {
                            sensorPlugin = sensorAttributes.value("plugin").toString();
                        }

                    }

                    QString sensorPath = SystemDbManager::getPluginFilePath(sensorName,sensorPlugin);
                    s->instrument = PluginLoader::loadSensorPlugin(sensorPath,sensorName);
                    s->setInstrumentConfig(sc);
                    /* ...and next... */
                    xml.readNext();
                }


            }

            if(xml.name() == "member"){

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "member")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes memberAttributes = xml.attributes();

                        if(memberAttributes.hasAttribute("type")){

                            if (memberAttributes.value("type") == "position"){

                                if(memberAttributes.hasAttribute("ref")){
                                    s->position->id = memberAttributes.value("ref").toInt();
                                    oiProjectExchanger::stationElements.append(s->position->id );
                                }

                            }

                            if (memberAttributes.value("type") == "coordinatesystem"){

                                if(memberAttributes.hasAttribute("ref")){
                                    s->coordSys->id = memberAttributes.value("ref").toInt();
                                    oiProjectExchanger::stationElements.append(s->coordSys->id);
                                }
                            }

                            if (memberAttributes.value("type") == "usedForFeature"){

                                if(memberAttributes.hasAttribute("ref")){
                                    FeatureWrapper *tmpFeature = new FeatureWrapper();
                                    Point *p = new Point();
                                    p->id = memberAttributes.value("ref").toInt();
                                    tmpFeature->setPoint(p);
                                    s->usedFor.append(tmpFeature);
                                }
                            }

                            if (memberAttributes.value("type") == "previouslyNeeded"){

                                if(memberAttributes.hasAttribute("ref")){
                                    FeatureWrapper *tmpFeature = new FeatureWrapper();
                                    Point *p = new Point();
                                    p->id = memberAttributes.value("ref").toInt();
                                    tmpFeature->setPoint(p);
                                    s->previouslyNeeded.append(tmpFeature);
                                }
                            }
                        }

                    }
                    /* ...and next... */
                    xml.readNext();
                }

            }


            fw->setStation(s);

            if(xml.name() == "function"){

                oiProjectExchanger::parseFunction(xml,fw);

            }


        }
        /* ...and next... */
        xml.readNext();
    }
    fw->setStation(s);
    return fw;
}


FeatureWrapper* oiProjectExchanger::parseGeometry(QXmlStreamReader& xml){

    FeatureWrapper* fw = new FeatureWrapper();

    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("type")){
        if(attributes.value("type").toString() == Configuration::sPlane){
            Plane *pl = new Plane();

            if(attributes.hasAttribute("name")){
                pl->name = attributes.value("name").toString();
            }
            if(attributes.hasAttribute("id")) {
                pl->id = attributes.value("id").toInt();
            }
            if(attributes.hasAttribute("nominal")) {
                pl->isNominal = attributes.value("nominal").toInt();
            }
            if(attributes.hasAttribute("common")) {
                pl->isCommon = attributes.value("common").toInt();
            }
            if(attributes.hasAttribute("solved")) {
                pl->isSolved= attributes.value("solved").toInt();
            }

            fw->setPlane(pl);
        }else if(attributes.value("type").toString() == Configuration::sPoint){
            Point *p = new Point();

            if(attributes.hasAttribute("name")){
                p->name = attributes.value("name").toString();
            }
            if(attributes.hasAttribute("id")) {
                p->id = attributes.value("id").toInt();
            }
            if(attributes.hasAttribute("nominal")) {
                p->isNominal = attributes.value("nominal").toInt();
            }
            if(attributes.hasAttribute("common")) {
                p->isCommon = attributes.value("common").toInt();
            }
            if(attributes.hasAttribute("solved")) {
                p->isSolved= attributes.value("solved").toInt();
            }

            fw->setPoint(p);

        }else if(attributes.value("type").toString() == Configuration::sLine){

            Line *l = new Line();

            if(attributes.hasAttribute("name")){
                l->name = attributes.value("name").toString();
            }
            if(attributes.hasAttribute("id")) {
                l->id = attributes.value("id").toInt();
            }
            if(attributes.hasAttribute("nominal")) {
                l->isNominal = attributes.value("nominal").toInt();
            }
            if(attributes.hasAttribute("common")) {
                l->isCommon = attributes.value("common").toInt();
            }
            if(attributes.hasAttribute("solved")) {
                l->isSolved= attributes.value("solved").toInt();
            }

            fw->setLine(l);

        }else if(attributes.value("type").toString() == Configuration::sSphere){

            Sphere *s = new Sphere();

            if(attributes.hasAttribute("name")){
                s->name = attributes.value("name").toString();
            }
            if(attributes.hasAttribute("id")) {
                s->id = attributes.value("id").toInt();
            }
            if(attributes.hasAttribute("nominal")) {
                s->isNominal = attributes.value("nominal").toInt();
            }
            if(attributes.hasAttribute("common")) {
                s->isCommon = attributes.value("common").toInt();
            }
            if(attributes.hasAttribute("solved")) {
                s->isSolved= attributes.value("solved").toInt();
            }

            fw->setSphere(s);

        }else if(attributes.value("type").toString() == Configuration::sEntitiyAngle){

            ScalarEntityAngle *sa = new ScalarEntityAngle();

            if(attributes.hasAttribute("name")){
                sa->name = attributes.value("name").toString();
            }
            if(attributes.hasAttribute("id")) {
                sa->id = attributes.value("id").toInt();
            }
            if(attributes.hasAttribute("nominal")) {
                sa->isNominal = attributes.value("nominal").toInt();
            }
            if(attributes.hasAttribute("common")) {
                sa->isCommon = attributes.value("common").toInt();
            }
            if(attributes.hasAttribute("solved")) {
                sa->isSolved= attributes.value("solved").toInt();
            }

            fw->setScalarEntityAngle(sa);

        }else if(attributes.value("type").toString() == Configuration::sEntityDistance){

            ScalarEntityDistance *sd = new ScalarEntityDistance();

            if(attributes.hasAttribute("name")){
                sd->name = attributes.value("name").toString();
            }
            if(attributes.hasAttribute("id")) {
                sd->id = attributes.value("id").toInt();
            }
            if(attributes.hasAttribute("nominal")) {
                sd->isNominal = attributes.value("nominal").toInt();
            }
            if(attributes.hasAttribute("common")) {
                sd->isCommon = attributes.value("common").toInt();
            }
            if(attributes.hasAttribute("solved")) {
                sd->isSolved= attributes.value("solved").toInt();
            }

            fw->setScalarEntityDistance(sd);

        }

    }


    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "geometry")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */
            if(xml.name() == "coordinates") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "coordinates")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes coordinatesAttributes = xml.attributes();

                        if(fw->getPoint() != NULL){

                            if(coordinatesAttributes.hasAttribute("x")){
                               fw->getPoint()->xyz.setAt(0,coordinatesAttributes.value("x").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("y")){
                               fw->getPoint()->xyz.setAt(1,coordinatesAttributes.value("y").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("z")){
                               fw->getPoint()->xyz.setAt(2,coordinatesAttributes.value("z").toDouble());
                            }

                        }else if(fw->getPlane() != NULL){

                            if(coordinatesAttributes.hasAttribute("x")){
                               fw->getPlane()->xyz.setAt(0,coordinatesAttributes.value("x").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("y")){
                               fw->getPlane()->xyz.setAt(1,coordinatesAttributes.value("y").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("z")){
                               fw->getPlane()->xyz.setAt(2,coordinatesAttributes.value("z").toDouble());
                            }

                        }else if(fw->getLine() != NULL){

                            if(coordinatesAttributes.hasAttribute("x")){
                               fw->getLine()->xyz.setAt(0,coordinatesAttributes.value("x").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("y")){
                               fw->getLine()->xyz.setAt(1,coordinatesAttributes.value("y").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("z")){
                               fw->getLine()->xyz.setAt(2,coordinatesAttributes.value("z").toDouble());
                            }

                        }else if(fw->getSphere() != NULL){

                            if(coordinatesAttributes.hasAttribute("x")){
                               fw->getSphere()->xyz.setAt(0,coordinatesAttributes.value("x").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("y")){
                               fw->getSphere()->xyz.setAt(1,coordinatesAttributes.value("y").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("z")){
                               fw->getSphere()->xyz.setAt(2,coordinatesAttributes.value("z").toDouble());
                            }

                        }


                    }
                    /* ...and next... */
                    xml.readNext();
                }
            }

            if(xml.name() == "spatialDirection") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "spatialDirection")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes spatialDirectionAttributes = xml.attributes();

                       if(fw->getPlane() != NULL){

                            if(spatialDirectionAttributes.hasAttribute("i")){
                               fw->getPlane()->ijk.setAt(0,spatialDirectionAttributes.value("i").toDouble());
                            }

                            if(spatialDirectionAttributes.hasAttribute("j")){
                               fw->getPlane()->ijk.setAt(1,spatialDirectionAttributes.value("j").toDouble());
                            }

                            if(spatialDirectionAttributes.hasAttribute("k")){
                               fw->getPlane()->ijk.setAt(2,spatialDirectionAttributes.value("k").toDouble());
                            }

                        }else if(fw->getLine() != NULL){

                            if(spatialDirectionAttributes.hasAttribute("i")){
                               fw->getLine()->ijk.setAt(0,spatialDirectionAttributes.value("i").toDouble());
                            }

                            if(spatialDirectionAttributes.hasAttribute("j")){
                               fw->getLine()->ijk.setAt(1,spatialDirectionAttributes.value("j").toDouble());
                            }

                            if(spatialDirectionAttributes.hasAttribute("k")){
                               fw->getLine()->ijk.setAt(2,spatialDirectionAttributes.value("k").toDouble());
                            }

                         }
                    }
                    xml.readNext();
                }
            }

            if(xml.name() == "radius") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "radius")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes radiusAttributes = xml.attributes();

                       if(fw->getSphere() != NULL){

                            if(radiusAttributes.hasAttribute("value")){
                               fw->getSphere()->radius = radiusAttributes.value("value").toDouble();
                            }

                         }
                     }
                 xml.readNext();
                }
            }

            if(xml.name() == "standardDeviation") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "standardDeviation")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes sigmaAttributes = xml.attributes();

                       if(fw->getGeometry() != NULL){

                            if(sigmaAttributes.hasAttribute("value")){
                               fw->getGeometry()->myStatistic.stdev = sigmaAttributes.value("value").toDouble();
                            }

                         }
                     }
                xml.readNext();
                }
            }

            if(xml.name() == "angle") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "angle")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes angleAttributes = xml.attributes();

                       if(fw->getScalarEntityAngle() != NULL){

                            if(angleAttributes.hasAttribute("value")){
                               fw->getScalarEntityAngle()->setAngle(angleAttributes.value("value").toDouble());
                            }

                         }
                     }
                xml.readNext();
                }
            }

            if(xml.name() == "distance") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "distance")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes distanceAttributes = xml.attributes();

                       if(fw->getScalarEntityAngle() != NULL){

                            if(distanceAttributes.hasAttribute("value")){
                               fw->getScalarEntityDistance()->setDistance(distanceAttributes.value("value").toDouble());
                            }

                         }
                     }
                xml.readNext();
                }
            }

            if(xml.name() == "measurementconfig") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "measurementconfig")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes measurementconfigAttributes = xml.attributes();

                       if(!fw->getGeometry()->isNominal){

                            if(measurementconfigAttributes.hasAttribute("name")){
                              //TODO MeasurementConfig setzten - datenbank abfrage
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
                                    Observation *tmpObs = new Observation(NULL,NULL);
                                    tmpObs->id = memberAttributes.value("ref").toInt();
                                    fw->getGeometry()->myObservations.append(tmpObs);
                                }

                            }

                            if (memberAttributes.value("type") == "coordinatesystem"){

                                if(memberAttributes.hasAttribute("ref")){
                                    CoordinateSystem* tmpCoordSys = new CoordinateSystem();
                                    tmpCoordSys->id = memberAttributes.value("ref").toInt();
                                    fw->getGeometry()->myNominalCoordSys = tmpCoordSys;
                                }
                            }

                            if (memberAttributes.value("type") == "usedForFeature"){

                                if(memberAttributes.hasAttribute("ref")){
                                    FeatureWrapper *tmpFeature = new FeatureWrapper();
                                    Point *p = new Point();
                                    p->id = memberAttributes.value("ref").toInt();
                                    tmpFeature->setPoint(p);
                                    fw->getGeometry()->usedFor.append(tmpFeature);
                                }
                            }

                            if (memberAttributes.value("type") == "previouslyNeeded"){

                                if(memberAttributes.hasAttribute("ref")){
                                    FeatureWrapper *tmpFeature = new FeatureWrapper();
                                    Point *p = new Point();
                                    p->id = memberAttributes.value("ref").toInt();
                                    tmpFeature->setPoint(p);
                                    fw->getGeometry()->previouslyNeeded.append(tmpFeature);
                                }
                            }

                            if (memberAttributes.value("type") == "nominalGeometry"){

                                if(memberAttributes.hasAttribute("ref")){
                                    Geometry *tmpGeom= new Point();
                                    tmpGeom->id = memberAttributes.value("ref").toInt();
                                    fw->getGeometry()->nominals.append(tmpGeom);
                                }
                            }

                        }

                    }
                    /* ...and next... */
                    xml.readNext();
                }
            }

            if(xml.name() == "function"){

                oiProjectExchanger::parseFunction(xml,fw);

            }


        }
        /* ...and next... */
        xml.readNext();
    }

    return fw;

}

FeatureWrapper* oiProjectExchanger::parseCoordinatesystem(QXmlStreamReader& xml){


    FeatureWrapper* fw = new FeatureWrapper();
    CoordinateSystem *c = NULL;


    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("name")){
        c = new CoordinateSystem();
        c->name = attributes.value("name").toString();
    }
    if(attributes.hasAttribute("id")) {
        c->id = attributes.value("id").toInt();
    }

    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "coordinatesystem")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */

            if(xml.name() == "member"){

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "member")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes memberAttributes = xml.attributes();

                        if(memberAttributes.hasAttribute("type")){

                            if (memberAttributes.value("type") == "observation"){

                                if(memberAttributes.hasAttribute("ref")){
                                    Observation *tmpObs = new Observation(NULL,NULL);
                                    tmpObs->id = memberAttributes.value("ref").toInt();
                                    c->observations.append(tmpObs);
                                }

                            }

                            if (memberAttributes.value("type") == "nominalGeometry"){

                                if(memberAttributes.hasAttribute("ref")){
                                    Geometry *tmpGeom= new Point();
                                    tmpGeom->id = memberAttributes.value("ref").toInt();
                                    c->nominals.append(tmpGeom);
                                }
                            }

                            if (memberAttributes.value("type") == "usedForFeature"){

                                if(memberAttributes.hasAttribute("ref")){
                                    FeatureWrapper *tmpFeature = new FeatureWrapper();
                                    Point *p = new Point();
                                    p->id = memberAttributes.value("ref").toInt();
                                    tmpFeature->setPoint(p);
                                    c->usedFor.append(tmpFeature);
                                }
                            }

                            if (memberAttributes.value("type") == "previouslyNeeded"){

                                if(memberAttributes.hasAttribute("ref")){
                                    FeatureWrapper *tmpFeature = new FeatureWrapper();
                                    Point *p = new Point();
                                    p->id = memberAttributes.value("ref").toInt();
                                    tmpFeature->setPoint(p);
                                    c->previouslyNeeded.append(tmpFeature);
                                }
                            }
                        }



                    }
                    /* ...and next... */
                    xml.readNext();
                }

            }


            fw->setCoordinateSystem(c);

            if(xml.name() == "function"){

                oiProjectExchanger::parseFunction(xml,fw);

            }


        }
        /* ...and next... */
        xml.readNext();
    }

    fw->setCoordinateSystem(c);
    return fw;

}

FeatureWrapper* oiProjectExchanger::parseTrafoPara(QXmlStreamReader& xml){

    FeatureWrapper* fw = new FeatureWrapper();
    TrafoParam *tp = NULL;


    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("name")){
        tp = new TrafoParam();
        tp->name = attributes.value("name").toString();
    }
    if(attributes.hasAttribute("id")) {
        tp->id = attributes.value("id").toInt();
    }
    if(attributes.hasAttribute("tx")) {
        tp->translation.setAt(0,attributes.value("tx").toDouble());
    }
    if(attributes.hasAttribute("ty")) {
        tp->translation.setAt(1,attributes.value("ty").toDouble());
    }
    if(attributes.hasAttribute("tz")) {
        tp->translation.setAt(2,attributes.value("tz").toDouble());
    }
    if(attributes.hasAttribute("rx")) {
        tp->rotation.setAt(0,attributes.value("rx").toDouble());
    }
    if(attributes.hasAttribute("ry")) {
        tp->rotation.setAt(1,attributes.value("ry").toDouble());
    }
    if(attributes.hasAttribute("rz")) {
        tp->rotation.setAt(2,attributes.value("rz").toDouble());
    }
    if(attributes.hasAttribute("mx")) {
        tp->scale.setAt(0,attributes.value("mx").toDouble());
    }
    if(attributes.hasAttribute("my")) {
        tp->scale.setAt(1,attributes.value("my").toDouble());
    }
    if(attributes.hasAttribute("mz")) {
        tp->scale.setAt(2,attributes.value("mz").toDouble());
    }

    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "transformationsparameter")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */

            if(xml.name() == "from") {


                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "from")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes fromAttributes = xml.attributes();

                        if(fromAttributes.hasAttribute("ref")){
                            CoordinateSystem *tmpCoord = new CoordinateSystem();
                            tmpCoord->id = fromAttributes.value("ref").toInt();
                            tp->from = tmpCoord;
                        }

                    }

                    xml.readNext();
                }


            }

            if(xml.name() == "to") {


                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "to")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes toAttributes = xml.attributes();

                        if(toAttributes.hasAttribute("ref")){
                            CoordinateSystem *tmpCoord = new CoordinateSystem();
                            tmpCoord->id = toAttributes.value("ref").toInt();
                            tp->to = tmpCoord;
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


                            if (memberAttributes.value("type") == "usedForFeature"){

                                if(memberAttributes.hasAttribute("ref")){
                                    FeatureWrapper *tmpFeature = new FeatureWrapper();
                                    Point *p = new Point();
                                    p->id = memberAttributes.value("ref").toInt();
                                    tmpFeature->setPoint(p);
                                    tp->usedFor.append(tmpFeature);
                                }
                            }

                            if (memberAttributes.value("type") == "previouslyNeeded"){

                                if(memberAttributes.hasAttribute("ref")){
                                    FeatureWrapper *tmpFeature = new FeatureWrapper();
                                    Point *p = new Point();
                                    p->id = memberAttributes.value("ref").toInt();
                                    tmpFeature->setPoint(p);
                                    tp->previouslyNeeded.append(tmpFeature);
                                }
                            }
                        }



                    }
                    /* ...and next... */
                    xml.readNext();
                }

            }


            if(xml.name() == "function"){

                oiProjectExchanger::parseFunction(xml,fw);

            }


        }
        /* ...and next... */
        xml.readNext();
    }

    fw->setTrafoParam(tp);
    return fw;

}

void oiProjectExchanger::parseFunction(QXmlStreamReader& xml, FeatureWrapper *feature){



}

/*!
 * \brief sortID
 * \param f1
 * \param f2
 * \return
 * comperator function for sorting FeatureWrapper* by id
 */
bool sortID(FeatureWrapper *f1, FeatureWrapper *f2){
    return f1->getFeature()->id < f2->getFeature()->id;
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
