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



SensorConfiguration* oiProjectExchanger::loadSensorConfig(QString name){

    return NULL;
}


MeasurementConfig* oiProjectExchanger::loadMeasurementConfig(QString name){

    return NULL;
}

bool oiProjectExchanger::saveSettings(QString name){


    return false;
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
