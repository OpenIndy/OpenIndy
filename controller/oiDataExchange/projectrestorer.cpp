#include "projectrestorer.h"

ProjectRestorer::ProjectRestorer(QObject *parent) :
    QObject(parent)
{
}

bool ProjectRestorer::saveProject(oiProjectData &data){

    if (!data.device->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        Console::addLine("can't open  device");
        return false;
    }

    Console::addLine("saving project");
    this->clearAllLists();

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

    //sort and seperate features
    Console::addLine("sort and seperate features");

    for(int i = 0; i<data.features.size();i++){

     if(data.features.at(i)->getGeometry() != NULL ){

        this->geometries.append(data.features.at(i));

     }else if(data.features.at(i)->getCoordinateSystem() != NULL){

        this->coordSystems.append(data.features.at(i)->getCoordinateSystem());

      }else if (data.features.at(i)->getStation() != NULL){

         this->stations.append(data.features.at(i)->getStation());

         this->coordSystems.append(data.features.at(i)->getStation()->coordSys);

         FeatureWrapper *fwStationPosition = new FeatureWrapper();
         fwStationPosition->setPoint(data.features.at(i)->getStation()->position);

         this->geometries.append(fwStationPosition);

      }else if(data.features.at(i)->getTrafoParam() != NULL){

         this->trafoParams.append(data.features.at(i)->getTrafoParam());

       }
    }

    //write stations to xml
    Console::addLine("write stations to xml");
    foreach(Station* s, this->stations){
        s->toOpenIndyXML(stream);
    }

    //write coordnatesystem to xml
     Console::addLine("write coordinatesystems to xml");
    foreach(CoordinateSystem* c, this->coordSystems){

        c->toOpenIndyXML(stream);

        //harvest all observations
        foreach(Observation* obs, c->observations){
            this->observations.append(obs);
        }
    }

    //write all trafoParam to xml
     Console::addLine("write transformationparamters to xml");
    foreach(TrafoParam* t, this->trafoParams){
        t->toOpenIndyXML(stream);
    }

    //write all geometries to xml
     Console::addLine("write geometries to xml");
    foreach(FeatureWrapper* fw, this->geometries){
        fw->getGeometry()->toOpenIndyXML(stream);
    }

    //write all observations to xml
     Console::addLine("write observations to xml");
    foreach(Observation* o, this->observations){
        o->toOpenIndyXML(stream);
    }

    stream.writeEndElement();
    stream.writeEndDocument();

    data.device->close();

    Console::addLine("saving completed");

    return true;

}

bool ProjectRestorer::loadProject(oiProjectData &data){

    if (!data.device->open(QIODevice::ReadOnly | QIODevice::Text)) {

      return false;
    }

        this->clearAllLists();

        QXmlStreamReader xml(data.device);

        Console::addLine("load project from xml");
        while(!xml.atEnd() &&
                !xml.hasError()) {

            QXmlStreamReader::TokenType token = xml.readNext();

            if(token == QXmlStreamReader::StartDocument) {
                continue;
            }

            if(token == QXmlStreamReader::StartElement) {

                if(xml.name() == "observation") {
                    Observation *o = new Observation(NULL,NULL);
                    ElementDependencies d =  o->fromOpenIndyXML(xml);

                    this->dependencies.append(d);
                    this->observations.append(o);
                }

                if(xml.name() == "oiProjectData"){

                }

                if(xml.name() == "station"){

                    Station *s = new Station("");
                    ElementDependencies d = s->fromOpenIndyXML(xml);

                    stationElements.append(s->coordSys->id);
                    stationElements.append(s->position->id);

                    this->stations.append(s);
                    this->dependencies.append(d);

                }

                if(xml.name() == "coordinatesystem"){

                    CoordinateSystem *cs = new CoordinateSystem();
                    ElementDependencies d = cs->fromOpenIndyXML(xml);

                    this->coordSystems.append(cs);
                    this->dependencies.append(d);

                }

                if(xml.name() == "geometry"){

                    QXmlStreamAttributes attributes = xml.attributes();

                     QString geometryType;

                    if(attributes.hasAttribute("type")){
                       geometryType = attributes.value("type").toString();
                    }

                    this->addGeometryToList(Configuration::getElementTypeEnum(geometryType),xml);

                }

                if(xml.name() == "transformationparameter"){

                    TrafoParam* t = new TrafoParam();
                    ElementDependencies d = t->fromOpenIndyXML(xml);

                    this->trafoParams.append(t);
                    this->dependencies.append(d);
                }
            }
        }

        if(xml.hasError()) {

            Console::addLine(QString("xml not valid: " + xml.errorString()));
            data.device->close();
            return false;
        }

         data.device->close();
         Console::addLine("resolve dependencies");
         this->resolveDependencies(data);
         return true;


}

void ProjectRestorer::clearAllLists(){

    this->observations.clear();
    this->geometries.clear();
    this->stations.clear();
    this->coordSystems.clear();
    this->trafoParams.clear();
    this->features.clear();
    this->dependencies.clear();

}

void ProjectRestorer::addGeometryToList(Configuration::ElementTypes typeOfElement, QXmlStreamReader &xml){

    switch (typeOfElement) {
    case (Configuration::ePointElement):{
            Point *p = new Point();
            ElementDependencies dp = p->fromOpenIndyXML(xml);

            FeatureWrapper *fwp = new FeatureWrapper();
            fwp->setPoint(p);

            this->geometries.append(fwp);
            this->dependencies.append(dp);
        break;}
    case (Configuration::ePlaneElement):{
            Plane *pl = new Plane();
            ElementDependencies dpl = pl->fromOpenIndyXML(xml);

            FeatureWrapper *fwpl = new FeatureWrapper();
            fwpl->setPlane(pl);

            this->geometries.append(fwpl);
            this->dependencies.append(dpl);
        break;}
    case (Configuration::eSphereElement):{
            Sphere *sp = new Sphere();
            ElementDependencies dsp = sp->fromOpenIndyXML(xml);

            FeatureWrapper *fwsp = new FeatureWrapper();
            fwsp->setSphere(sp);

            this->geometries.append(fwsp);
            this->dependencies.append(dsp);
        break;}
    case (Configuration::eLineElement):{
            Line *l = new Line();
            ElementDependencies dl = l->fromOpenIndyXML(xml);

            FeatureWrapper *fwl = new FeatureWrapper();
            fwl->setLine(l);

            this->geometries.append(fwl);
            this->dependencies.append(dl);
         break;}
    case (Configuration::eScalarEntityAngleElement):{
            ScalarEntityAngle *sAngle = new ScalarEntityAngle();
            ElementDependencies dsAngle = sAngle->fromOpenIndyXML(xml);

            FeatureWrapper *fwsAngle = new FeatureWrapper();
            fwsAngle->setScalarEntityAngle(sAngle);

            this->geometries.append(fwsAngle);
            this->dependencies.append(dsAngle);
         break;}
    case (Configuration::eScalarEntityDistanceElement):{
            ScalarEntityDistance *sDistance = new ScalarEntityDistance();
            ElementDependencies dsDistance = sDistance->fromOpenIndyXML(xml);

            FeatureWrapper *fwsDistance = new FeatureWrapper();
            fwsDistance->setScalarEntityDistance(sDistance);

            this->geometries.append(fwsDistance);
            this->dependencies.append(dsDistance);
         break;}
    default:{
        break;}
    }

}

Observation* ProjectRestorer::findObservation(int id){

    foreach(Observation *obs, this->observations){
        if (obs->id == id){
            return obs;
        }
    }
    return NULL;
}

Geometry* ProjectRestorer::findGeometry(int id){

    foreach(FeatureWrapper *g, this->geometries){
        if (g->getGeometry()->id == id){
            return g->getGeometry();
        }
    }
    return NULL;
}

CoordinateSystem* ProjectRestorer::findCoordSys(int id){

    foreach(CoordinateSystem *cs, this->coordSystems){
        if (cs->id == id){
            return cs;
        }
    }
    return NULL;
}

TrafoParam* ProjectRestorer::findTrafoParam(int id){

    foreach(TrafoParam *t, this->trafoParams){
        if (t->id == id){
            return t;
        }
    }
    return NULL;
}

FeatureWrapper* ProjectRestorer::findFeature(int id){

    foreach(FeatureWrapper *f, this->features){
        if (f->getFeature()->id == id){
            return f;
        }
    }
    return NULL;
}

Station* ProjectRestorer::findStation(int id){

    foreach(Station *s, this->stations){
        if (s->id == id){
            return s;
        }
    }
    return NULL;

}

void ProjectRestorer::resolveDependencies(oiProjectData &data){

    foreach(ElementDependencies d, this->dependencies){

        FeatureWrapper *resolvedFeature = new FeatureWrapper();

        switch (d.typeOfElement) {
        case (Configuration::eStationElement):{

            this->resolveStation(resolvedFeature,d);

            break;}
        default:
            break;
        }



        data.features.append(resolvedFeature);
    }

}

void ProjectRestorer::resolveFeature(FeatureWrapper *fw, ElementDependencies &d)
{

}

void ProjectRestorer::geometryGeometry(FeatureWrapper *fw, ElementDependencies &d)
{

}

void ProjectRestorer::resolveStation(FeatureWrapper *fw, ElementDependencies &d)
{

}

void ProjectRestorer::resolveTrafoParam(FeatureWrapper *fw, ElementDependencies &d)
{

}

void ProjectRestorer::resolveCoordinateSystem(FeatureWrapper *fw, ElementDependencies &d)
{

}


