#include "projectrestorer.h"

ProjectRestorer::ProjectRestorer(QObject *parent) :
    QObject(parent)
{
}

/*!
 * \brief ProjectRestorer::saveProject
 * \param data
 * \return
 */
bool ProjectRestorer::saveProject(OiProjectData &data){

    if (!data.getDevice()->open(QIODevice::WriteOnly | QIODevice::Text)){
        Console::addLine("Cannot open the device");
        return false;
    }

    Console::addLine("saving project...");

    this->clearAllLists();

    //get current date and time
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString(Qt::ISODate);

    QXmlStreamWriter stream(data.getDevice());

    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    //write general project information
    stream.writeStartElement("oiProjectData");
    stream.writeAttribute("name", data.getProjectName());
    stream.writeAttribute("date", dateTimeString);
    stream.writeAttribute("idcount", QString::number(Configuration::idCount));

    //write active coordinate system
    stream.writeStartElement("activeCoordinatesystem");
    int activeSystem = -1;
    if(OiFeatureState::getActiveCoordinateSystem() != NULL){
        activeSystem = OiFeatureState::getActiveCoordinateSystem()->getId();
    }
    stream.writeAttribute("ref", QString::number(activeSystem));
    stream.writeEndElement();

    //write active coordinate system
    stream.writeStartElement("activeStation");
    int activeStation = -1;
    if(OiFeatureState::getActiveStation() != NULL){
        activeStation = OiFeatureState::getActiveStation()->getId();
    }
    stream.writeAttribute("ref", QString::number(activeStation));
    stream.writeEndElement();

    Console::addLine("sort and seperate features");

    //Helper to save the stations's positions as a point feature (delete later)
    QList<FeatureWrapper *> stationPositions;

    //iterate through all features of the current project and save them into lists
    //so that the features in the XML are ordered by feature type
    const QList<FeatureWrapper *> myFeatures = OiFeatureState::getFeatures();
    for(int i = 0; i < myFeatures.size();i++){

        if(myFeatures.at(i)->getGeometry() != NULL ){ //geometries

            this->geometries.append(myFeatures.at(i));

        }else if(myFeatures.at(i)->getCoordinateSystem() != NULL){ //coordinate systems

            this->coordSystems.append(myFeatures.at(i)->getCoordinateSystem());

        }else if (myFeatures.at(i)->getStation() != NULL){ //stations

            this->stations.append(myFeatures.at(i)->getStation());
            this->coordSystems.append(myFeatures.at(i)->getStation()->coordSys);

            FeatureWrapper *fwStationPosition = new FeatureWrapper();
            fwStationPosition->setPoint(myFeatures.at(i)->getStation()->position);
            stationPositions.append(fwStationPosition);
            this->geometries.append(fwStationPosition);

        }else if(myFeatures.at(i)->getTrafoParam() != NULL){ //trafo params

            this->trafoParams.append(myFeatures.at(i)->getTrafoParam());

        }

    }

    //write stations to xml
    Console::addLine("write stations to xml...");
    stream.writeStartElement("stations");
    foreach(Station* s, this->stations){
        s->toOpenIndyXML(stream);
    }
    stream.writeEndElement();

    //write coordinatesystems to xml
    Console::addLine("write coordinatesystems to xml...");
    stream.writeStartElement("coordinatesystems");
    foreach(CoordinateSystem* c, this->coordSystems){
        c->toOpenIndyXML(stream);

        //harvest all observations
        foreach(Observation* obs, c->getObservations()){
            this->observations.append(obs);
        }
    }
    stream.writeEndElement();

    //write all trafoParam to xml
    Console::addLine("write transformationparamters to xml...");
    stream.writeStartElement("transformationparameters");
    foreach(TrafoParam* t, this->trafoParams){
        t->toOpenIndyXML(stream);
    }
    stream.writeEndElement();

    //write all geometries to xml
    Console::addLine("write geometries to xml...");
    stream.writeStartElement("geometries");
    foreach(FeatureWrapper* fw, this->geometries){
        fw->getGeometry()->toOpenIndyXML(stream);
    }
    stream.writeEndElement();

    //write all observations to xml
    Console::addLine("write observations to xml...");
    stream.writeStartElement("observations");
    foreach(Observation* o, this->observations){
        o->toOpenIndyXML(stream);
    }
    stream.writeEndElement();

    stream.writeEndElement();
    stream.writeEndDocument();

    data.getDevice()->close();

    //delete station position helpers
    foreach(FeatureWrapper *stationPosition, stationPositions){
        delete stationPosition;
    }

    Console::addLine("saving completed");

    return true;

}

/*!
 * \brief ProjectRestorer::loadProject
 * \param data
 * \return
 */
bool ProjectRestorer::loadProject(OiProjectData &data){

    if (!data.getDevice()->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    this->clearAllLists(); //reset lists from a previous Save or Load

    QXmlStreamReader xml(data.getDevice());

    Console::addLine("load project from xml");
    while(!xml.atEnd() && !xml.hasError()) {

        qDebug() << QString("%1").arg(xml.lineNumber());

        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if(token == QXmlStreamReader::StartElement) {

            if(xml.name().compare("observation") == 0) {

                Observation *o = new Observation(NULL,NULL);
                ElementDependencies d =  o->fromOpenIndyXML(xml);
                this->dependencies.append(d);
                this->observations.append(o);

            }else if(xml.name().compare("activeCoordinatesystem") == 0){

                if(xml.attributes().hasAttribute("ref")){
                    this->activeCoordSystemId = xml.attributes().value("ref").toInt();
                }

            }else if(xml.name().compare("activeStation") == 0){

                if(xml.attributes().hasAttribute("ref")){
                    this->activeStationId = xml.attributes().value("ref").toInt();
                }

            }else if(xml.name().compare("station") == 0){

                Station *s = new Station("");
                ElementDependencies d = s->fromOpenIndyXML(xml);
                this->stationElements.append(d.getStationCoordSystem());
                this->stationElements.append(d.getStationPosition());
                this->stations.append(s);
                this->dependencies.append(d);

            }else if(xml.name().compare("coordinatesystem") == 0){

                CoordinateSystem *cs = new CoordinateSystem();
                ElementDependencies d = cs->fromOpenIndyXML(xml);
                this->coordSystems.append(cs);
                this->dependencies.append(d);

            }else if(xml.name().compare("geometry") == 0){

                QXmlStreamAttributes attributes = xml.attributes();
                QString geometryType;
                if(attributes.hasAttribute("type")){
                   geometryType = attributes.value("type").toString();
                }
                this->addGeometryToList(Configuration::getElementTypeEnum(geometryType), xml);

            }else if(xml.name().compare("transformationparameter") == 0){

                TrafoParam* t = new TrafoParam();
                ElementDependencies d = t->fromOpenIndyXML(xml);
                this->trafoParams.append(t);
                this->dependencies.append(d);
            }
        }
    }

    if(xml.hasError()) {
        Console::addLine(QString("xml not valid: " + xml.errorString()));
        data.getDevice()->close();
        return false;
    }

     data.getDevice()->close();

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

/*!
 * \brief ProjectRestorer::addGeometryToList
 * \param typeOfElement
 * \param xml
 */
void ProjectRestorer::addGeometryToList(Configuration::ElementTypes typeOfElement, QXmlStreamReader &xml){

    switch (typeOfElement) {
    case (Configuration::ePointElement):{

        Point *p = new Point(false);
        ElementDependencies dp = p->fromOpenIndyXML(xml);

        FeatureWrapper *fwp = new FeatureWrapper();
        fwp->setPoint(p);

        this->geometries.append(fwp);
        this->features.append(fwp);
        this->dependencies.append(dp);
        break;

    }case (Configuration::ePlaneElement):{

        Plane *pl = new Plane(false);
        ElementDependencies dpl = pl->fromOpenIndyXML(xml);

        FeatureWrapper *fwpl = new FeatureWrapper();
        fwpl->setPlane(pl);

        this->geometries.append(fwpl);
        this->features.append(fwpl);
        this->dependencies.append(dpl);
        break;

    }case (Configuration::eSphereElement):{

        Sphere *sp = new Sphere(false);
        ElementDependencies dsp = sp->fromOpenIndyXML(xml);

        FeatureWrapper *fwsp = new FeatureWrapper();
        fwsp->setSphere(sp);

        this->geometries.append(fwsp);
        this->features.append(fwsp);
        this->dependencies.append(dsp);
        break;

    }case (Configuration::eLineElement):{

        Line *l = new Line(false);
        ElementDependencies dl = l->fromOpenIndyXML(xml);

        FeatureWrapper *fwl = new FeatureWrapper();
        fwl->setLine(l);

        this->geometries.append(fwl);
        this->features.append(fwl);
        this->dependencies.append(dl);
        break;

    }case (Configuration::eScalarEntityAngleElement):{

        ScalarEntityAngle *sAngle = new ScalarEntityAngle(false);
        ElementDependencies dsAngle = sAngle->fromOpenIndyXML(xml);

        FeatureWrapper *fwsAngle = new FeatureWrapper();
        fwsAngle->setScalarEntityAngle(sAngle);

        this->geometries.append(fwsAngle);
        this->features.append(fwsAngle);
        this->dependencies.append(dsAngle);
        break;

    }case (Configuration::eScalarEntityDistanceElement):{

        ScalarEntityDistance *sDistance = new ScalarEntityDistance(false);
        ElementDependencies dsDistance = sDistance->fromOpenIndyXML(xml);

        FeatureWrapper *fwsDistance = new FeatureWrapper();
        fwsDistance->setScalarEntityDistance(sDistance);

        this->geometries.append(fwsDistance);
        this->features.append(fwsDistance);
        this->dependencies.append(dsDistance);
        break;

    }default:{
        break;
    }
    }

}

Observation* ProjectRestorer::findObservation(int id){

    foreach(Observation *obs, this->observations){
        if (obs->getId() == id){
            return obs;
        }
    }
    return NULL;
}

FeatureWrapper* ProjectRestorer::findGeometry(int id){

    foreach(FeatureWrapper *g, this->geometries){
        if (g->getGeometry()->getId() == id){
            return g;
        }
    }
    return NULL;
}

CoordinateSystem* ProjectRestorer::findCoordSys(int id){

    foreach(CoordinateSystem *cs, this->coordSystems){
        if (cs->getId() == id){
            return cs;
        }
    }
    return NULL;
}

TrafoParam* ProjectRestorer::findTrafoParam(int id){

    foreach(TrafoParam *t, this->trafoParams){
        if (t->getId() == id){
            return t;
        }
    }
    return NULL;
}

FeatureWrapper* ProjectRestorer::findFeature(int id){

    foreach(FeatureWrapper *f, this->features){
        if (f->getFeature()->getId() == id){
            return f;
        }
    }
    return NULL;
}

Station* ProjectRestorer::findStation(int id){

    foreach(Station *s, this->stations){
        if (s->getId() == id){
            return s;
        }
    }
    return NULL;

}

/*!
 * \brief ProjectRestorer::resolveDependencies
 * \param data
 */
void ProjectRestorer::resolveDependencies(OiProjectData &data){

    foreach(ElementDependencies d, this->dependencies){

        FeatureWrapper *resolvedFeature ;

        if(d.typeOfElement != Configuration::eObservationElement){
            resolvedFeature = new FeatureWrapper();
        }

        switch (d.typeOfElement) {
        case (Configuration::eStationElement):{
            this->resolveStation(resolvedFeature,d);
            this->features.append(resolvedFeature);
            break;
        }case (Configuration::eCoordinateSystemElement):{
            this->resolveCoordinateSystem(resolvedFeature,d);
            this->features.append(resolvedFeature);
            break;
        }case (Configuration::eTrafoParamElement):{
            this->resolveTrafoParam(resolvedFeature,d);
            this->features.append(resolvedFeature);
            break;
        }case (Configuration::eObservationElement):{
            this->resolveObservation(d);
            break;
        }default:
            this->resolveGeometry(resolvedFeature,d);
            break;
        }

        if(d.typeOfElement != Configuration::eObservationElement && !this->stationElements.contains(d.elementID)){
            OiFeatureState::addFeature(resolvedFeature);

            if(resolvedFeature->getStation()!=NULL && resolvedFeature->getFeature()->getId() == activeStationId){
                resolvedFeature->getStation()->setActiveStationState(true);

                if(resolvedFeature->getStation()->coordSys !=NULL && resolvedFeature->getStation()->coordSys->getId() == activeCoordSystemId){
                    resolvedFeature->getStation()->coordSys->setActiveCoordinateSystemState(true);
                }
            }

            if(resolvedFeature->getCoordinateSystem()!=NULL && resolvedFeature->getFeature()->getId() == activeCoordSystemId){
                resolvedFeature->getCoordinateSystem()->setActiveCoordinateSystemState(true);
            }

        }
    }

    /*
     * add references to usedFor and previouslyNeeded features here
     * because now all features are in the features list
     */
    foreach(ElementDependencies d, this->dependencies){

        FeatureWrapper *myFeature = this->findFeature(d.elementID);
        if(myFeature != NULL && myFeature->getFeature() != NULL){
            this->addUsedFor(myFeature, d);
            this->addPreviouslyNeeded(myFeature, d);
        }

    }

    OiFeatureState::sortFeaturesById();

}

void ProjectRestorer::resolveFeature(FeatureWrapper *fw, ElementDependencies &d)
{

    /*QMap<QString,QList<int>* > featureDependencies = d.getfeatureDependencies();

    QList<int>* usedForFeature = featureDependencies.value("usedForFeature");
    QList<int>* previouslyNeeded = featureDependencies.value("previouslyNeeded");

    if(usedForFeature != NULL){
        for(int i = 0;i<usedForFeature->size(); i++){
            FeatureWrapper* uff = this->findFeature(usedForFeature->at(i));
            if(uff != NULL){
                fw->getFeature()->usedFor.append(uff);
            }
        }
    }

    if(previouslyNeeded != NULL){
        for(int i = 0;i<previouslyNeeded->size(); i++){
            FeatureWrapper* pnf = this->findFeature(previouslyNeeded->at(i));
            if(pnf != NULL){
                fw->getFeature()->previouslyNeeded.append(pnf);
            }
        }
    }*/

    foreach(Function* f, this->resolveFunctions(d)){
        fw->getFeature()->addFunction(f);
    }




}

/*!
 * \brief ProjectRestorer::resolveGeometry
 * \param fw
 * \param d
 */
void ProjectRestorer::resolveGeometry(FeatureWrapper *fw, ElementDependencies &d){

    for(int i = 0; i< this->geometries.size();i++){
        if(geometries.at(i)->getFeature()->getId() == d.elementID){
            *fw = *geometries.at(i);
            break;
        }
    }

    this->resolveFeature(fw,d);

    QList<int> neededObs = d.getObservationDependencies();
    QList<int>* nominalCoordSys = d.getfeatureDependencies().value("coordinatesystem");
    QList<int>* nominalGeom= d.getfeatureDependencies().value("nominalGeometry");

    for(int i = 0;i<neededObs.size();i++){
        Observation* obs = this->findObservation(neededObs.at(i));
        if(obs != NULL){
            fw->getGeometry()->addObservation(obs);
            obs->myTargetGeometries.append(fw->getGeometry());
        }
    }

    if(nominalCoordSys != NULL){
        for(int i = 0;i<nominalCoordSys->size();i++){
            CoordinateSystem* c = this->findCoordSys(nominalCoordSys->at(i));
            if(c != NULL){
                fw->getGeometry()->setNominalSystem(c);
            }
        }
    }

    if(nominalGeom != NULL){
        for(int i = 0;i<nominalGeom->size();i++){
            FeatureWrapper* nominalGeometry = this->findGeometry(nominalGeom->at(i));
            if(nominalGeometry != NULL){
                fw->getGeometry()->addNominal(nominalGeometry->getGeometry());
            }
        }
    }

}

void ProjectRestorer::resolveStation(FeatureWrapper *fw, ElementDependencies &d)
{
    //find Station
    foreach(Station* s, this->stations){
        if(s->getId() == d.elementID){
            fw->setStation(s);
            break;
        }
    }

    //resolve common feature attributes
    this->resolveFeature(fw,d);

    //find Station coordsystem and postion
    CoordinateSystem* stationCoordSys = this->findCoordSys(d.getStationCoordSystem());
    FeatureWrapper* stationPosition = this->findGeometry(d.getStationPosition());

    if(stationCoordSys != NULL){
        fw->getStation()->coordSys = stationCoordSys;
    }
    if(stationPosition != NULL){
        fw->getStation()->position = stationPosition->getPoint();
    }


    sensorInfo activeSensor =  d.getActiveSensor();
    QList<sensorInfo> usedSensors = d.getNeededSensors();

    QString sensorPluginPath = SystemDbManager::getPluginFilePath(activeSensor.name,activeSensor.plugin);
    fw->getStation()->sensorPad->instrument = PluginLoader::loadSensorPlugin(sensorPluginPath,activeSensor.name);

    for(int i = 0; i<usedSensors.size();i++){
        QString sensorName = usedSensors.at(i).name;
        QString sensorPlugin = usedSensors.at(i).plugin;

        QString sensorPluginPath = SystemDbManager::getPluginFilePath(sensorName,sensorPlugin);
        fw->getStation()->sensorPad->usedSensors.append(PluginLoader::loadSensorPlugin(sensorPluginPath,sensorName));
    }

}

/*!
 * \brief ProjectRestorer::resolveTrafoParam
 * \param fw
 * \param d
 */
void ProjectRestorer::resolveTrafoParam(FeatureWrapper *fw, ElementDependencies &d){

    foreach(TrafoParam* t, this->trafoParams){
        if(t->getId() == d.elementID){
            fw->setTrafoParam(t);
            break;
        }
    }

    if(fw->getTrafoParam() != NULL){
        CoordinateSystem *fromSystem = this->findCoordSys(d.getFromSystem());
        CoordinateSystem *toSystem = this->findCoordSys(d.getToSystem());
        if(fromSystem != NULL && toSystem != NULL){
            fw->getTrafoParam()->setCoordinateSystems(fromSystem, toSystem);
            fromSystem->addTransformationParameter(fw->getTrafoParam());
            toSystem->addTransformationParameter(fw->getTrafoParam());
        }

        this->resolveFeature(fw,d);
    }

}

void ProjectRestorer::resolveCoordinateSystem(FeatureWrapper *fw, ElementDependencies &d)
{
    foreach(CoordinateSystem* c, this->coordSystems){
        if(c->getId() == d.elementID){
            fw->setCoordinateSystem(c);
            break;
        }
    }

    this->resolveFeature(fw,d);

    QList<int> neededObs = d.getObservationDependencies();
    QList<int>* nominalGeom= d.getfeatureDependencies().value("nominalGeometry");

    for(int i = 0;i<neededObs.size();i++){
        Observation* obs = this->findObservation(neededObs.at(i));
        if(obs != NULL){
            fw->getCoordinateSystem()->addObservation(obs);
        }
    }

    if(nominalGeom != NULL){
        for(int i = 0;i<nominalGeom->size();i++){
            FeatureWrapper* nominalGeometry = this->findGeometry(nominalGeom->at(i));
            if(nominalGeometry != NULL){
                fw->getCoordinateSystem()->addNominal(nominalGeometry->getGeometry());
            }
        }
    }

}

/*!
 * \brief ProjectRestorer::resolveObservation
 * \param d
 */
void ProjectRestorer::resolveObservation(ElementDependencies &d){

    Observation* obs = this->findObservation(d.elementID);

    QList<int>* obsStations = d.getfeatureDependencies().value("station");

    if(obsStations != NULL){
        for(int i = 0;i<obsStations->size();i++){
            obs->myStation = this->findStation(obsStations->at(i));
            obs->myStation->coordSys->addObservation(obs);
            break;
        }
    }
}

QList<Function *> ProjectRestorer::resolveFunctions(ElementDependencies &d)
{
    QList<Function*> featureFunctions;

    QList<functionInfo>neededFunctions =  d.getNeededFunctions();

    for(int i = 0;i<neededFunctions.size();i++){

        QString name = neededFunctions.at(i).name;
        QString plugin = neededFunctions.at(i).plugin;
        int exIndex = neededFunctions.at(i).executionIndex;

        QString functionPluginPath = SystemDbManager::getPluginFilePath(name,plugin);
        Function* tmpFunction = PluginLoader::loadFunctionPlugin(functionPluginPath,name);

        QMap<int,featureIndex> neededElements = neededFunctions.at(i).neededElements;

        QMapIterator<int, featureIndex> j(neededElements);
        while (j.hasNext()) {
            j.next();


            switch (j.value().typeOfElement) {
            case (Configuration::eObservationElement):{
               Observation *obs = this->findObservation(j.key());
               tmpFunction->addObservation(obs,j.value().idx);
                break;}
            case (Configuration::eStationElement):{
                Station *s = this->findStation(j.key());
                tmpFunction->addStation(s,j.value().idx);
                break;}
            case (Configuration::eTrafoParamElement):{
                TrafoParam *t = this->findTrafoParam(j.key());
                tmpFunction->addTrafoParam(t,j.value().idx);
                break;}
            case (Configuration::eCoordinateSystemElement):{
                CoordinateSystem *c = this->findCoordSys(j.key());
                tmpFunction->addCoordSystem(c,j.value().idx);
                break;}
            case (Configuration::ePointElement):{
                FeatureWrapper* point = this->findGeometry(j.key());
                tmpFunction->addPoint(point->getPoint(),j.value().idx);
                break;}
            case (Configuration::ePlaneElement):{
                FeatureWrapper* plane = this->findGeometry(j.key());
                tmpFunction->addPlane(plane->getPlane(),j.value().idx);
                break;}
            case (Configuration::eSphereElement):{
                FeatureWrapper* sphere = this->findGeometry(j.key());
                tmpFunction->addSphere(sphere->getSphere(),j.value().idx);
                break;}
            case (Configuration::eLineElement):{
                FeatureWrapper* line = this->findGeometry(j.key());
                tmpFunction->addLine(line->getLine(),j.value().idx);
                 break;}
            case (Configuration::eScalarEntityAngleElement):{
                FeatureWrapper* angle = this->findGeometry(j.key());
                tmpFunction->addScalarEntityAngle(angle->getScalarEntityAngle(),j.value().idx);
                 break;}
            case (Configuration::eScalarEntityDistanceElement):{
                FeatureWrapper* distance = this->findGeometry(j.key());
                tmpFunction->addScalarEntityDistance(distance->getScalarEntityDistance(),j.value().idx);
                 break;}
            default:{
                break;}
            }

    }

    featureFunctions.insert(exIndex,tmpFunction);

  }

  return featureFunctions;

}

/*!
 * \brief ProjectRestorer::addUsedFor
 * \param f
 * \param d
 */
void ProjectRestorer::addUsedFor(FeatureWrapper *f, ElementDependencies &d){

    QList<int> usedFor = d.getUsedFor();
    if(f != NULL && f->getFeature() != NULL && usedFor.size() > 0){

        //get all features and add them as usedFor features
        for(int i = 0; i < usedFor.size(); i++){

            FeatureWrapper *myFeature = this->findFeature(usedFor.at(i));
            if(myFeature != NULL && myFeature->getFeature() != NULL){
                f->getFeature()->usedFor.append(myFeature);
            }

        }

    }

}

/*!
 * \brief ProjectRestorer::addPreviouslyNeeded
 * \param f
 * \param d
 */
void ProjectRestorer::addPreviouslyNeeded(FeatureWrapper *f, ElementDependencies &d){

    QList<int> previouslyNeeded = d.getPreviouslyNeeded();
    if(f != NULL && f->getFeature() != NULL && previouslyNeeded.size() > 0){

        //get all features and add them as previouslyNeeded features
        for(int i = 0; i < previouslyNeeded.size(); i++){

            FeatureWrapper *myFeature = this->findFeature(previouslyNeeded.at(i));
            if(myFeature != NULL && myFeature->getFeature() != NULL){
                f->getFeature()->previouslyNeeded.append(myFeature);
            }

        }

    }

}

/*void ProjectRestorer::readOiProjectData(QXmlStreamReader &xml)
{

    if(xml.name() == "member"){

            if(xml.tokenType() == QXmlStreamReader::StartElement) {

                QXmlStreamAttributes memberAttributes = xml.attributes();

                if(memberAttributes.hasAttribute("type")){

                    if (memberAttributes.value("type") == "activeCoordinatesystem"){

                        if(memberAttributes.hasAttribute("ref")){
                            activeCoordSystemId = memberAttributes.value("ref").toInt();
                        }
                    }

                    if (memberAttributes.value("type") == "activeStation"){

                        if(memberAttributes.hasAttribute("ref")){
                            activeStationId = memberAttributes.value("ref").toInt();
                        }
                    }
                }

            }

    }


}*/


