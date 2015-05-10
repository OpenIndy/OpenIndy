#include "function.h"
/*
Function::Function(QObject *parent) : QObject(parent){

}

bool Function::exec(Circle &c){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type circle");
    return false;
}

bool Function::exec(Cone &c){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type cone");
    return false;
}

bool Function::exec(Cylinder &c){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type cylinder");
    return false;
}

bool Function::exec(CoordinateSystem &c){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type point");
    return false;
}

bool Function::exec(Ellipsoid &e){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type ellipsoid");
    return false;
}

bool Function::exec(Hyperboloid &h){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type hyperboloid");
    return false;
}

bool Function::exec(Line &l){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type line");
    return false;
}

bool Function::exec(Nurbs &n){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type nurbs");
    return false;
}

bool Function::exec(Paraboloid &p){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type paraboloid");
    return false;
}

bool Function::exec(Plane &p){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type plane");
    return false;
}

bool Function::exec(Point &p){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type point");
    return false;
}

bool Function::exec(PointCloud &p){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type pointcloud");
    return false;
}

bool Function::exec(ScalarEntityAngle &s){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type angle");
    return false;
}

bool Function::exec(ScalarEntityDistance &s){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type distance");
    return false;
}

bool Function::exec(ScalarEntityTemperature &s){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type temperature");
    return false;
}

bool Function::exec(ScalarEntityMeasurementSeries &s){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type temperature");
    return false;
}

bool Function::exec(Sphere &s){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type sphere");
    return false;
}

bool Function::exec(Station &s){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type point");
    return false;
}

bool Function::exec(TrafoParam &t){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type point");
    return false;
}

QMap<QString, int> Function::getIntegerParameter() const{
    QMap<QString, int> result;
    return result;
}

QMap<QString,double> Function::getDoubleParameter() const{
    QMap<QString, double> result;
    return result;
}

QMultiMap<QString, QString> Function::getStringParameter() const{
    QMultiMap<QString, QString> result;
    return result;
}

QStringList Function::getResultProtocol() const{
    return QStringList();
}

QDomElement Function::toOpenIndyXML(QDomDocument &xmlDoc) const{

    if(xmlDoc.isNull()){
        return QDomElement();
    }

    //add function attributes
    QDomElement function = xmlDoc.createElement("function");
    function.setAttribute("name", this->getMetaData().name);
    function.setAttribute("type", this->getMetaData().iid);
    function.setAttribute("plugin", this->getMetaData().pluginName);

    //add used elements
    QDomElement inputElements = xmlDoc.createElement("inputElements");
    QMapIterator<int, QList<InputElement> > inputIterator(this->getFeatureOrder());
    while (inputIterator.hasNext()) {
        inputIterator.next();
        foreach(InputElement input, inputIterator.value()){
            QDomElement inputElement = xmlDoc.createElement("element");
            inputElement.setAttribute("index", inputIterator.key());
            inputElement.setAttribute("type", input.typeOfElement);
            inputElement.setAttribute("ref", input.id);
            inputElements.appendChild(inputElement);
        }
    }
    function.appendChild(inputElements);

    //add integer parameters
    QDomElement integerParams = xmlDoc.createElement("integerParameters");
    QMapIterator<QString, int> intIterator(this->myConfiguration.intParameter);
    while (intIterator.hasNext()) {
        intIterator.next();
        QDomElement intParam = xmlDoc.createElement("parameter");
        intParam.setAttribute("value", intIterator.value());
        integerParams.appendChild(intParam);
    }
    function.appendChild(integerParams);

    //add double parameters
    QDomElement doubleParams = xmlDoc.createElement("doubleParameters");
    QMapIterator<QString, double> doubleIterator(this->myConfiguration.doubleParameter);
    while (doubleIterator.hasNext()) {
        doubleIterator.next();
        QDomElement doubleParam = xmlDoc.createElement("parameter");
        doubleParam.setAttribute("value", doubleIterator.value());
        doubleParams.appendChild(doubleParam);
    }
    function.appendChild(doubleParams);

    //add string parameters
    QDomElement stringParams = xmlDoc.createElement("stringParameters");
    QMapIterator<QString, QString> stringIterator(this->myConfiguration.stringParameter);
    while (stringIterator.hasNext()) {
        stringIterator.next();
        QDomElement stringParam = xmlDoc.createElement("parameter");
        stringParam.setAttribute("value", stringIterator.value());
        stringParams.appendChild(stringParam);
    }
    function.appendChild(stringParams);

    return function;
}

bool Function::fromOpenIndyXML(QDomElement &xmlElem){

    if(xmlElem.isNull()){
        return false;
    }

    if(!xmlElem.hasAttribute("name") || !xmlElem.hasAttribute("plugin") || !xmlElem.hasAttribute("executionIndex")
            || !xmlElem.hasAttribute("type")){
        return false;
    }

    //set integer parameter
    QDomElement integerParameters = xmlElem.firstChildElement("integerParameters");
    if(!integerParameters.isNull()){
        QDomNodeList paramList = integerParameters.childNodes();
        for(int i = 0; i < paramList.size(); i++){
            QDomElement integerParameter = paramList.at(i).toElement();
            if(integerParameter.tagName().compare("parameter") && integerParameter.hasAttribute("value")
                    && integerParameter.hasAttribute("name")){
                this->myConfiguration.intParameter.insert(integerParameter.attribute("name"), integerParameter.attribute("value").toInt());
            }
        }
    }

    //set double parameter
    QDomElement doubleParameters = xmlElem.firstChildElement("doubleParameters");
    if(!doubleParameters.isNull()){
        QDomNodeList paramList = doubleParameters.childNodes();
        for(int i = 0; i < paramList.size(); i++){
            QDomElement doubleParameter = paramList.at(i).toElement();
            if(doubleParameter.tagName().compare("parameter") && doubleParameter.hasAttribute("value")
                    && doubleParameter.hasAttribute("name")){
                this->myConfiguration.doubleParameter.insert(doubleParameter.attribute("name"), doubleParameter.attribute("value").toDouble());
            }
        }
    }

    //set string parameter
    QDomElement stringParameters = xmlElem.firstChildElement("stringParameters");
    if(!stringParameters.isNull()){
        QDomNodeList paramList = stringParameters.childNodes();
        for(int i = 0; i < paramList.size(); i++){
            QDomElement stringParameter = paramList.at(i).toElement();
            if(stringParameter.tagName().compare("parameter") && stringParameter.hasAttribute("value")
                    && stringParameter.hasAttribute("name")){
                this->myConfiguration.stringParameter.insert(stringParameter.attribute("name"), stringParameter.attribute("value"));
            }
        }
    }

    return true;
}

void Function::setScalarInputParams(const ScalarInputParams &params){
    this->scalarInputParams = params;
    this->scalarInputParams.isValid = true;
}

const ScalarInputParams &Function::getScalarInputParams(){

    //if scalar input params are queried the first time it is filled with default values
    if(!this->scalarInputParams.isValid){

        this->scalarInputParams.intParameter = this->getIntegerParameter();
        this->scalarInputParams.doubleParameter = this->getDoubleParameter();
        QMultiMap<QString, QString> stringParameter = this->getStringParameter();
        QStringList stringKeys = stringParameter.keys();
        foreach(const QString &key, stringKeys){
            this->scalarInputParams.stringParameter.insert(key, stringParameter.values().last());
        }

    }

    return this->scalarInputParams;

}

bool Function::isValid(){
    return true;



    QList<NeededElement> params = this->getNeededElements();
    for(int i = 0; i < params.size(); i++){
        if(this->featureOrder.contains(i)){ //if featureOrder contains needed element i
            QMap<int, QList<InputElement> >::iterator idx = this->featureOrder.find(i);
            if(idx != this->featureOrder.end() && idx.value().size() > 0){ //if the list with elements is not empty
                QList<InputElement> featurePosition = idx.value();
                for(int j = 0; j < featurePosition.size(); j++){
                    if(featurePosition.at(j).typeOfElement != params.at(i).typeOfElement){ //if the element type is correct
                        return false;
                    }
                }
            }else{
                return false;
            }
        }else{
            return false;
        }
    }
    return true;
}

const QMultiMap<int, InputElement> &Function::getFeatureOrder() const{
    return this->featureOrder;
}

Statistic& Function::getStatistic(){
    return this->myStatistic;
}

const int &Function::getId() const{
    return this->id;
}

void Function::addStation(const QPointer<Station> &station, const int &position){

    //check the pointer
    if(station.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->stations.insert(station->getId(), station);

    //create an input element
    InputElement element;
    element.id = station->getId();
    element.typeOfElement = eStationElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeStation(const int &id){

    //remove element from corresponding map
    this->stations.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Station> Function::getStation(const int &id) const{
    return stations.value(id, QPointer<Station>());
}

const QMap<int, QPointer<Station> > &Function::getStations() const{
    return this->stations;
}

void Function::addCoordSystem(const QPointer<CoordinateSystem> &coordSys, const int &position){

    //check the pointer
    if(coordSys.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->coordSystems.insert(coordSys->getId(), coordSys);

    //create an input element
    InputElement element;
    element.id = coordSys->getId();
    element.typeOfElement = eCoordinateSystemElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeCoordSystem(const int &id){

    //remove element from corresponding map
    this->coordSystems.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<CoordinateSystem> Function::getCoordinateSystem(const int &id) const{
    return coordSystems.value(id, QPointer<CoordinateSystem>());
}

const QMap<int, QPointer<CoordinateSystem> > &Function::getCoordinateSystems() const{
    return this->coordSystems;
}

void Function::addTrafoParam(const QPointer<TrafoParam> &trafoParam, const int &position){

    //check the pointer
    if(trafoParam.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->trafoParams.insert(trafoParam->getId(), trafoParam);

    //create an input element
    InputElement element;
    element.id = trafoParam->getId();
    element.typeOfElement = eTrafoParamElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeTrafoParam(const int &id){

    //remove element from corresponding map
    this->trafoParams.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<TrafoParam> Function::getTrafoParam(const int &id) const{
    return trafoParams.value(id, QPointer<TrafoParam>());
}

const QMap<int, QPointer<TrafoParam> > &Function::getTrafoParams() const{
    return this->trafoParams;
}

void Function::addPoint(const QPointer<Point> &point, const int &position){

    //check the pointer
    if(point.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->points.insert(point->getId(), point);

    //create an input element
    InputElement element;
    element.id = point->getId();
    element.typeOfElement = ePointElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removePoint(const int &id){

    //remove element from corresponding map
    this->points.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Point> Function::getPoint(const int &id) const{
    return points.value(id, QPointer<Point>());
}

const QMap<int, QPointer<Point> > &Function::getPoints() const{
    return this->points;
}

void Function::addLine(const QPointer<Line> &line, const int &position){

    //check the pointer
    if(line.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->lines.insert(line->getId(), line);

    //create an input element
    InputElement element;
    element.id = line->getId();
    element.typeOfElement = eLineElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeLine(const int &id){

    //remove element from corresponding map
    this->lines.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Line> Function::getLine(const int &id) const{
    return lines.value(id, QPointer<Line>());
}

const QMap<int, QPointer<Line> > &Function::getLines() const{
    return this->lines;
}

void Function::addPlane(const QPointer<Plane> &plane, const int &position){

    //check the pointer
    if(plane.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->planes.insert(plane->getId(), plane);

    //create an input element
    InputElement element;
    element.id = plane->getId();
    element.typeOfElement = ePlaneElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removePlane(const int &id){

    //remove element from corresponding map
    this->planes.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Plane> Function::getPlane(const int &id) const{
    return planes.value(id, QPointer<Plane>());
}

const QMap<int, QPointer<Plane> > &Function::getPlanes() const{
    return this->planes;
}

void Function::addSphere(const QPointer<Sphere> &sphere, const int &position){

    //check the pointer
    if(sphere.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->spheres.insert(sphere->getId(), sphere);

    //create an input element
    InputElement element;
    element.id = sphere->getId();
    element.typeOfElement = eSphereElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeSphere(const int &id){

    //remove element from corresponding map
    this->spheres.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Sphere> Function::getSphere(const int &id) const{
    return spheres.value(id, QPointer<Sphere>());
}

const QMap<int, QPointer<Sphere> > &Function::getSpheres() const{
    return this->spheres;
}

void Function::addCircle(const QPointer<Circle> &circle, const int &position){

    //check the pointer
    if(circle.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->circles.insert(circle->getId(), circle);

    //create an input element
    InputElement element;
    element.id = circle->getId();
    element.typeOfElement = eCircleElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeCircle(const int &id){

    //remove element from corresponding map
    this->circles.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Circle> Function::getCircle(const int &id) const{
    return circles.value(id, QPointer<Circle>());
}

const QMap<int, QPointer<Circle> > &Function::getCircles() const{
    return this->circles;
}

void Function::addCone(const QPointer<Cone> &cone, const int &position){

    //check the pointer
    if(cone.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->cones.insert(cone->getId(), cone);

    //create an input element
    InputElement element;
    element.id = cone->getId();
    element.typeOfElement = eConeElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeCone(const int &id){

    //remove element from corresponding map
    this->cones.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Cone> Function::getCone(const int &id) const{
    return cones.value(id, QPointer<Cone>());
}

const QMap<int, QPointer<Cone> > &Function::getCones() const{
    return this->cones;
}

void Function::addCylinder(const QPointer<Cylinder> &cylinder, const int &position){

    //check the pointer
    if(cylinder.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->cylinders.insert(cylinder->getId(), cylinder);

    //create an input element
    InputElement element;
    element.id = cylinder->getId();
    element.typeOfElement = eCylinderElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeCylinder(const int &id){

    //remove element from corresponding map
    this->cylinders.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Cylinder> Function::getCylinder(const int &id) const{
    return cylinders.value(id, QPointer<Cylinder>());
}

const QMap<int, QPointer<Cylinder> > &Function::getCylinders() const{
    return this->cylinders;
}

void Function::addEllipsoid(const QPointer<Ellipsoid> &ellipsoid, const int &position){

    //check the pointer
    if(ellipsoid.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->ellipsoids.insert(ellipsoid->getId(), ellipsoid);

    //create an input element
    InputElement element;
    element.id = ellipsoid->getId();
    element.typeOfElement = eEllipsoidElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeEllipsoid(const int &id){

    //remove element from corresponding map
    this->ellipsoids.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Ellipsoid> Function::getEllipsoid(const int &id) const{
    return ellipsoids.value(id, QPointer<Ellipsoid>());
}

const QMap<int, QPointer<Ellipsoid> > &Function::getEllipsoids() const{
    return this->ellipsoids;
}

void Function::addParaboloid(const QPointer<Paraboloid> &paraboloid, const int &position){

    //check the pointer
    if(paraboloid.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->paraboloids.insert(paraboloid->getId(), paraboloid);

    //create an input element
    InputElement element;
    element.id = paraboloid->getId();
    element.typeOfElement = eParaboloidElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeParaboloid(const int &id){

    //remove element from corresponding map
    this->paraboloids.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Paraboloid> Function::getParaboloid(const int &id) const{
    return paraboloids.value(id, QPointer<Paraboloid>());
}

const QMap<int, QPointer<Paraboloid> > &Function::getParaboloids() const{
    return this->paraboloids;
}

void Function::addHyperboloid(const QPointer<Hyperboloid> &hyperboloid, const int &position){

    //check the pointer
    if(hyperboloid.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->hyperboloids.insert(hyperboloid->getId(), hyperboloid);

    //create an input element
    InputElement element;
    element.id = hyperboloid->getId();
    element.typeOfElement = eHyperboloidElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeHyperboloid(const int &id){

    //remove element from corresponding map
    this->hyperboloids.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Hyperboloid> Function::getHyperboloid(const int &id) const{
    return hyperboloids.value(id, QPointer<Hyperboloid>());
}

const QMap<int, QPointer<Hyperboloid> > &Function::getHyperboloids() const{
    return this->hyperboloids;
}

void Function::addNurb(const QPointer<Nurbs> &nurbs, const int &position){

    //check the pointer
    if(nurbs.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->nurbs.insert(nurbs->getId(), nurbs);

    //create an input element
    InputElement element;
    element.id = nurbs->getId();
    element.typeOfElement = eNurbsElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeNurb(const int &id){

    //remove element from corresponding map
    this->nurbs.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Nurbs> Function::getNurb(const int &id) const{
    return nurbs.value(id, QPointer<Nurbs>());
}

const QMap<int, QPointer<Nurbs> > &Function::getNurbs() const{
    return this->nurbs;
}

void Function::addPointCloud(const QPointer<PointCloud> &pointCloud, const int &position){

    //check the pointer
    if(pointCloud.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->pointClouds.insert(pointCloud->getId(), pointCloud);

    //create an input element
    InputElement element;
    element.id = pointCloud->getId();
    element.typeOfElement = ePointCloudElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removePointCloud(const int &id){

    //remove element from corresponding map
    this->pointClouds.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<PointCloud> Function::getPointCloud(const int &id) const{
    return pointClouds.value(id, QPointer<PointCloud>());
}

const QMap<int, QPointer<PointCloud> > &Function::getPointClouds() const{
    return this->pointClouds;
}

void Function::addScalarEntityDistance(const QPointer<ScalarEntityDistance> &distance, const int &position){

    //check the pointer
    if(distance.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->scalarEntityDistances.insert(distance->getId(), distance);

    //create an input element
    InputElement element;
    element.id = distance->getId();
    element.typeOfElement = eScalarEntityDistanceElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeScalarEntityDistance(const int &id){

    //remove element from corresponding map
    this->scalarEntityDistances.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<ScalarEntityDistance> Function::getScalarEntityDistance(const int &id) const{
    return scalarEntityDistances.value(id, QPointer<ScalarEntityDistance>());
}

const QMap<int, QPointer<ScalarEntityDistance> > &Function::getScalarEntityDistances() const{
    return this->scalarEntityDistances;
}

void Function::addScalarEntityAngle(const QPointer<ScalarEntityAngle> &angle, const int &position){

    //check the pointer
    if(angle.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->scalarEntityAngles.insert(angle->getId(), angle);

    //create an input element
    InputElement element;
    element.id = angle->getId();
    element.typeOfElement = eScalarEntityAngleElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeScalarEntityAngle(const int &id){

    //remove element from corresponding map
    this->scalarEntityAngles.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<ScalarEntityAngle> Function::getScalarEntityAngle(const int &id) const{
    return scalarEntityAngles.value(id, QPointer<ScalarEntityAngle>());
}

const QMap<int, QPointer<ScalarEntityAngle> > &Function::getScalarEntityAngles() const{
    return this->scalarEntityAngles;
}

void Function::addObservation(const QPointer<Observation> &observation, const int &position){

    //check the pointer
    if(observation.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->observations.insert(observation->getId(), observation);

    //create an input element
    InputElement element;
    element.id = observation->getId();
    element.typeOfElement = eObservationElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeObservation(const int &id){

    //remove element from corresponding map
    this->observations.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Observation> Function::getObservation(const int &id) const{
    return observations.value(id, QPointer<Observation>());
}

const QMap<int, QPointer<Observation> > &Function::getObservations() const{
    return this->observations;
}

void Function::addReadingPolar(const QPointer<Reading> &reading, const int &position){

    //check the pointer
    if(reading.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->polarReadings.insert(reading->getId(), reading);

    //create an input element
    InputElement element;
    element.id = reading->getId();
    element.typeOfElement = eReadingPolarElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeReadingPolar(const int &id){

    //remove element from corresponding map
    this->polarReadings.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Reading> Function::getReadingPolar(const int &id) const{
    return polarReadings.value(id, QPointer<Reading>());
}

const QMap<int, QPointer<Reading> > &Function::getPolarReadings() const{
    return this->polarReadings;
}

void Function::addReadingDistance(const QPointer<Reading> &reading, const int &position){

    //check the pointer
    if(reading.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->distanceReadings.insert(reading->getId(), reading);

    //create an input element
    InputElement element;
    element.id = reading->getId();
    element.typeOfElement = eReadingDistanceElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeReadingDistance(const int &id){

    //remove element from corresponding map
    this->distanceReadings.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Reading> Function::getReadingDistance(const int &id) const{
    return distanceReadings.value(id, QPointer<Reading>());
}

const QMap<int, QPointer<Reading> > &Function::getDistanceReadings() const{
    return this->distanceReadings;
}

void Function::addReadingDirection(const QPointer<Reading> &reading, const int &position){

    //check the pointer
    if(reading.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->directionReadings.insert(reading->getId(), reading);

    //create an input element
    InputElement element;
    element.id = reading->getId();
    element.typeOfElement = eReadingDirectionElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeReadingDirection(const int &id){

    //remove element from corresponding map
    this->directionReadings.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Reading> Function::getReadingDirection(const int &id) const{
    return directionReadings.value(id, QPointer<Reading>());
}

const QMap<int, QPointer<Reading> > &Function::getDirectionReadings() const{
    return this->directionReadings;
}

void Function::addReadingCartesian(const QPointer<Reading> &reading, const int &position){

    //check the pointer
    if(reading.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->cartesianReadings.insert(reading->getId(), reading);

    //create an input element
    InputElement element;
    element.id = reading->getId();
    element.typeOfElement = eReadingCartesianElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeReadingCartesian(const int &id){

    //remove element from corresponding map
    this->cartesianReadings.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<Reading> Function::getReadingCartesian(const int &id) const{
    return cartesianReadings.value(id, QPointer<Reading>());
}

const QMap<int, QPointer<Reading> > &Function::getCartesianReadings() const{
    return this->cartesianReadings;
}

void Function::addScalarEntityTemperature(const QPointer<ScalarEntityTemperature> &temperature, const int &position){

    //check the pointer
    if(temperature.isNull()){
        return;
    }

    //add the element to the corresponding map
    this->scalarEntityTemperatures.insert(temperature->getId(), temperature);

    //create an input element
    InputElement element;
    element.id = temperature->getId();
    element.typeOfElement = eScalarEntityTemperatureElement;
    element.isUsed = true;

    //add the input element to the feature order map so it is used for calculation
    this->featureOrder.insert(position, element);

}

void Function::removeScalarEntityTemperature(const int &id){

    //remove element from corresponding map
    this->scalarEntityTemperatures.remove(id);

    //remove element from feature order map
    InputElement element;
    element.id = id;
    QList<int> keys = this->featureOrder.keys();
    foreach(const int &key, keys){
        this->featureOrder.remove(key, element);
    }

}

QPointer<ScalarEntityTemperature> Function::getScalarEntityTemperature(const int &id) const{
    return scalarEntityTemperatures.value(id, QPointer<ScalarEntityTemperature>());
}

const QMap<int, QPointer<ScalarEntityTemperature> > &Function::getScalarEntityTemperatures() const{
    return this->scalarEntityTemperatures;
}

void Function::addReading(const QPointer<Reading> &reading, const int &position){
    switch(reading->getTypeOfReading()){
    case eCartesianReading:
        this->addReadingCartesian(reading, position);
        break;
    case ePolarReading:
        this->addReadingPolar(reading, position);
        break;
    case eDirectionReading:
        this->addReadingDirection(reading, position);
        break;
    case eDistanceReading:
        this->addReadingDistance(reading, position);
        break;
    }
}

void Function::addFeature(const QPointer<FeatureWrapper> &feature, const int &position){
    switch(feature->getFeatureTypeEnum()){
    case ePointFeature:
        this->addPoint(feature->getPoint(), position);
        break;
    case eLineFeature:
        this->addLine(feature->getLine(), position);
        break;
    case ePlaneFeature:
        this->addPlane(feature->getPlane(), position);
        break;
    case eCircleFeature:
        this->addCircle(feature->getCircle(), position);
        break;
    case eSphereFeature:
        this->addSphere(feature->getSphere(), position);
        break;
    case eCylinderFeature:
        this->addCylinder(feature->getCylinder(), position);
        break;
    case eConeFeature:
        this->addCone(feature->getCone(), position);
        break;
    case eEllipsoidFeature:
        this->addEllipsoid(feature->getEllipsoid(), position);
        break;
    case eHyperboloidFeature:
        this->addHyperboloid(feature->getHyperboloid(), position);
        break;
    case eParaboloidFeature:
        this->addParaboloid(feature->getParaboloid(), position);
        break;
    case ePointCloudFeature:
        this->addPointCloud(feature->getPointCloud(), position);
        break;
    case eNurbsFeature:
        this->addNurb(feature->getNurbs(), position);
        break;
    case eScalarEntityAngleFeature:
        this->addScalarEntityAngle(feature->getScalarEntityAngle(), position);
        break;
    case eScalarEntityDistanceFeature:
        this->addScalarEntityDistance(feature->getScalarEntityDistance(), position);
        break;
    case eScalarEntityMeasurementSeriesFeature:
        break;
    case eScalarEntityTemperatureFeature:
        this->addScalarEntityTemperature(feature->getScalarEntityTemperature(), position);
        break;
    case eStationFeature:
        this->addStation(feature->getStation(), position);
        break;
    case eCoordinateSystemFeature:
        this->addCoordSystem(feature->getCoordinateSystem(), position);
        break;
    case eTrafoParamFeature:
        this->addTrafoParam(feature->getTrafoParam(), position);
        break;
    }
}

void Function::clear(){

    //clear lists with used elements
    this->stations.clear();
    this->trafoParams.clear();
    this->coordSystems.clear();
    this->points.clear();
    this->lines.clear();
    this->planes.clear();
    this->spheres.clear();
    this->observations.clear();
    this->scalarEntityAngles.clear();
    this->scalarEntityDistances.clear();
    this->polarReadings.clear();
    this->cartesianReadings.clear();
    this->directionReadings.clear();
    this->distanceReadings.clear();

    //clear map with feature order
    this->featureOrder.clear();

    //delete statistic
    this->myStatistic.setIsValid(false);

}

void Function::clearResults(){

    //clear statistic
    Statistic emptyStatistic;
    this->myStatistic = emptyStatistic;

    //set all feature to not be used
    QMap<int, QList<InputElement> > newFeatureOrder;
    QMapIterator<int, QList<InputElement> > iterate(this->featureOrder);
    int k = 0;
    while(iterate.hasNext()){
        iterate.next();
        QList<InputElement> newValue;
        QList<InputElement> value = iterate.value();
        for(int i = 0; i < value.size(); i++){
            InputElement f = value.at(i);
            f.isUsed = false;
            newValue.append(f);
        }
        newFeatureOrder.insert(k, newValue);
        k++;
    }
    this->featureOrder = newFeatureOrder;

}

void Function::setUseState(const int &id, bool state){
    for(int i = 0; i < this->getNeededElements().size(); i++){
        if(this->featureOrder.contains(i)){
            QMap<int, QList<InputElement> >::iterator idx = this->featureOrder.find(i);
            if(idx != this->featureOrder.end()){
                QList<InputElement> featurePosition = idx.value();
                for(int j = 0; j < featurePosition.size(); j++){
                    if(featurePosition.at(j).id == id){
                        InputElement feature;
                        feature.id = id;
                        feature.isUsed = state;
                        feature.typeOfElement = featurePosition.at(j).typeOfElement;
                        featurePosition.removeAt(j);
                        featurePosition.insert(j, feature);
                        break;
                    }
                }
                this->featureOrder.insert(i, featurePosition);
                break;
            }
        }
    }
}

OiFunctionEmitter& Function::getOiEmitter(){
    return this->myEmitter;
}

void Function::writeToConsole(QString message){
    myEmitter.sendString(message);
}

void Function::removeFeature(const int &id){
    this->removePoint(id);
    this->removeLine(id);
    this->removePlane(id);
    this->removeSphere(id);
    this->removeCylinder(id);
    this->removeCone(id);
    this->removeCircle(id);
    this->removeStation(id);
    this->removeCoordSystem(id);
    this->removeTrafoParam(id);
    this->removeEllipsoid(id);
    this->removeParaboloid(id);
    this->removeHyperboloid(id);
    this->removeNurb(id);
    this->removePointCloud(id);
    this->removeScalarEntityAngle(id);
    this->removeScalarEntityDistance(id);
}
*/

/*!
 * \brief Function::Function
 * \param parent
 */
Function::Function(QObject *parent) : QObject(parent){

}

/*!
 * \brief Function::~Function
 */
Function::~Function(){

}

/*!
 * \brief Function::exec
 * \param feature
 * \return
 */
bool Function::exec(const QPointer<FeatureWrapper> &feature){

    if(!feature.isNull()){

        switch(feature->getFeatureTypeEnum()){
        case eStationFeature:
            if(!feature->getStation().isNull()){ return this->exec(*feature->getStation()); }
        case eTrafoParamFeature:
            if(!feature->getTrafoParam().isNull()){ return this->exec(*feature->getTrafoParam()); }
        case eCoordinateSystemFeature:
            if(!feature->getCoordinateSystem().isNull()){ return this->exec(*feature->getCoordinateSystem()); }
        case eCircleFeature:
            if(!feature->getCircle().isNull()){ return this->exec(*feature->getCircle()); }
        case eConeFeature:
            if(!feature->getCone().isNull()){ return this->exec(*feature->getCone()); }
        case eCylinderFeature:
            if(!feature->getCylinder().isNull()){ return this->exec(*feature->getCylinder()); }
        case eEllipseFeature:
            if(!feature->getEllipse().isNull()){ return this->exec(*feature->getEllipse()); }
        case eEllipsoidFeature:
            if(!feature->getEllipsoid().isNull()){ return this->exec(*feature->getEllipsoid()); }
        case eHyperboloidFeature:
            if(!feature->getHyperboloid().isNull()){ return this->exec(*feature->getHyperboloid()); }
        case eLineFeature:
            if(!feature->getLine().isNull()){ return this->exec(*feature->getLine()); }
        case eNurbsFeature:
            if(!feature->getNurbs().isNull()){ return this->exec(*feature->getNurbs()); }
        case eParaboloidFeature:
            if(!feature->getParaboloid().isNull()){ return this->exec(*feature->getParaboloid()); }
        case ePlaneFeature:
            if(!feature->getPlane().isNull()){ return this->exec(*feature->getPlane()); }
        case ePointFeature:
            if(!feature->getPoint().isNull()){ return this->exec(*feature->getPoint()); }
        case ePointCloudFeature:
            if(!feature->getPointCloud().isNull()){ return this->exec(*feature->getPointCloud()); }
        case eScalarEntityAngleFeature:
            if(!feature->getScalarEntityAngle().isNull()){ return this->exec(*feature->getScalarEntityAngle()); }
        case eScalarEntityDistanceFeature:
            if(!feature->getScalarEntityDistance().isNull()){ return this->exec(*feature->getScalarEntityDistance()); }
        case eScalarEntityMeasurementSeriesFeature:
            if(!feature->getScalarEntityMeasurementSeries().isNull()){ return this->exec(*feature->getScalarEntityMeasurementSeries()); }
        case eScalarEntityTemperatureFeature:
            if(!feature->getScalarEntityTemperature().isNull()){ return this->exec(*feature->getScalarEntityTemperature()); }
        case eSlottedHoleFeature:
            if(!feature->getSlottedHole().isNull()){ return this->exec(*feature->getSlottedHole()); }
        case eSphereFeature:
            if(!feature->getSphere().isNull()){ return this->exec(*feature->getSphere()); }
        case eTorusFeature:
            if(!feature->getTorus().isNull()){ return this->exec(*feature->getTorus()); }
        }

    }

}

/*!
 * \brief Function::getNeededElements
 * \return
 */
const QList<NeededElement> &Function::getNeededElements() const{
    return this->neededElements;
}

/*!
 * \brief Function::getApplicableFor
 * \return
 */
const QList<FeatureTypes> &Function::getApplicableFor() const{
    return this->applicableFor;
}

/*!
 * \brief Function::getMetaData
 * \return
 */
const PluginMetaData &Function::getMetaData() const{
    return this->metaData;
}

/*!
 * \brief Function::getIntegerParameter
 * \return
 */
const QMap<QString, int> &Function::getIntegerParameter() const{
    return this->integerParameters;
}

/*!
 * \brief Function::getDoubleParameter
 * \return
 */
const QMap<QString, double> &Function::getDoubleParameter() const{
    return this->doubleParameters;
}

/*!
 * \brief Function::getStringParameter
 * \return
 */
const QMultiMap<QString, QString> &Function::getStringParameter() const{
    return this->stringParameters;
}

/*!
 * \brief Function::setScalarInputParams
 * \param params
 */
void Function::setScalarInputParams(const ScalarInputParams &params){
    this->scalarInputParams = params;
    this->scalarInputParams.isValid = true;
    emit this->scalarInputParametersChanged();
}

/*!
 * \brief Function::getScalarInputParams
 * \return
 */
const ScalarInputParams &Function::getScalarInputParams(){

    //use default values if no scalar input params have been specified
    if(!this->scalarInputParams.isValid){
        this->scalarInputParams.doubleParameter = this->doubleParameters;
        this->scalarInputParams.intParameter = this->integerParameters;
        QStringList keys = this->stringParameters.keys();
        foreach(const QString &key, keys){
            this->scalarInputParams.stringParameter.insert(key, this->stringParameters.value(key));
        }
    }

    return this->scalarInputParams;

}

/*!
 * \brief Function::getFixedParameters
 * \return
 */
const QList<FixedParameter> &Function::getFixedParameters() const{
    return this->fixedParameters;
}

/*!
 * \brief Function::fixParameter
 * \param parameter
 */
void Function::fixParameter(const FixedParameter &parameter){
    if(!this->fixedParameters.contains(parameter)){
        this->fixedParameters.append(parameter);
    }
}

/*!
 * \brief Function::unfixParameter
 * \param parameter
 */
void Function::unfixParameter(const UnknownParameters &parameter){
    this->fixedParameters.removeOne(FixedParameter(parameter));
}

/*!
 * \brief Function::unfixAllParameters
 */
void Function::unfixAllParameters(){
    this->fixedParameters.clear();
}

/*!
 * \brief Function::getResultProtocol
 * \return
 */
const QStringList &Function::getResultProtocol() const{
    return this->resultProtocol;
}

/*!
 * \brief Function::getStatistic
 * \return
 */
const Statistic &Function::getStatistic() const{
    return this->statistic;
}

/*!
 * \brief Function::getId
 * \return
 */
const int &Function::getId() const{
    return this->id;
}

/*!
 * \brief Function::getInputElements
 * \return
 */
const QMap<int, QList<InputElement> > &Function::getInputElements() const{
    return this->inputElements;
}

/*!
 * \brief Function::addInputElement
 * \param element
 * \param position
 */
void Function::addInputElement(const InputElement &element, const int &position){

    //add the input element
    if(this->inputElements.contains(position)){
        this->inputElements[position].append(element);
    }else{
        QList<InputElement> elements;
        elements.append(element);
        this->inputElements.insert(position, elements);
    }

    emit this->inputElementsChanged();

}

/*!
 * \brief Function::removeInputElement
 * \param id
 * \param position
 */
void Function::removeInputElement(const int &id, const int &position){
    if(this->inputElements.contains(position)){
        this->inputElements[position].removeOne(InputElement(id));
        emit this->inputElementsChanged();
    }
}

/*!
 * \brief Function::removeInputElement
 * \param id
 */
void Function::removeInputElement(const int &id){
    for(int i = 0; i < this->inputElements.size(); i++){
        this->inputElements[i].removeOne(InputElement(id));
        emit this->inputElementsChanged();
    }
}

/*!
 * \brief Function::replaceInputElement
 * \param element
 * \param position
 */
void Function::replaceInputElement(const InputElement &element, const int &position){
    if(this->inputElements.contains(position)){
        int index = this->inputElements[position].indexOf(element);
        if(index > -1){
            this->inputElements[position].replace(index, element);
            emit this->inputElementsChanged();
        }
    }
}

/*!
 * \brief Function::clear
 */
void Function::clear(){
    this->inputElements.clear();
    this->fixedParameters.clear();
    this->scalarInputParams.isValid = false;
    this->resultProtocol.clear();
    this->statistic.setIsValid(false);

    emit this->inputElementsChanged();
}

/*!
 * \brief Function::clearResults
 */
void Function::clearResults(){

    //clear statistic and protocol
    this->resultProtocol.clear();
    this->statistic.setIsValid(false);

    //set all elements to not have been used
    QList<int> keys = this->inputElements.keys();
    foreach(const int &key, keys){
        for(int i = 0; i < this->inputElements[key].size(); ++i){
            this->inputElements[key][i].isUsed = false;
        }
    }

}

/*!
 * \brief Function::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Function::toOpenIndyXML(QDomDocument &xmlDoc) const{

    if(xmlDoc.isNull()){
        return QDomElement();
    }

    //add function attributes
    QDomElement function = xmlDoc.createElement("function");
    function.setAttribute("name", this->getMetaData().name);
    function.setAttribute("type", this->getMetaData().iid);
    function.setAttribute("plugin", this->getMetaData().pluginName);

    //add used elements
    QDomElement inputElements = xmlDoc.createElement("inputElements");
    QMapIterator<int, QList<InputElement> > inputIterator(this->getInputElements());
    while (inputIterator.hasNext()) {
        inputIterator.next();
        foreach(const InputElement &input, inputIterator.value()){
            QDomElement inputElement = xmlDoc.createElement("element");
            inputElement.setAttribute("index", inputIterator.key());
            inputElement.setAttribute("type", input.typeOfElement);
            inputElement.setAttribute("ref", input.id);
            inputElements.appendChild(inputElement);
        }
    }
    function.appendChild(inputElements);

    //add integer parameters
    QDomElement integerParams = xmlDoc.createElement("integerParameters");
    QMapIterator<QString, int> intIterator(this->scalarInputParams.intParameter);
    while (intIterator.hasNext()) {
        intIterator.next();
        QDomElement intParam = xmlDoc.createElement("parameter");
        intParam.setAttribute("name", intIterator.key());
        intParam.setAttribute("value", intIterator.value());
        integerParams.appendChild(intParam);
    }
    function.appendChild(integerParams);

    //add double parameters
    QDomElement doubleParams = xmlDoc.createElement("doubleParameters");
    QMapIterator<QString, double> doubleIterator(this->scalarInputParams.doubleParameter);
    while (doubleIterator.hasNext()) {
        doubleIterator.next();
        QDomElement doubleParam = xmlDoc.createElement("parameter");
        doubleParam.setAttribute("name", doubleIterator.key());
        doubleParam.setAttribute("value", doubleIterator.value());
        doubleParams.appendChild(doubleParam);
    }
    function.appendChild(doubleParams);

    //add string parameters
    QDomElement stringParams = xmlDoc.createElement("stringParameters");
    QMapIterator<QString, QString> stringIterator(this->scalarInputParams.stringParameter);
    while (stringIterator.hasNext()) {
        stringIterator.next();
        QDomElement stringParam = xmlDoc.createElement("parameter");
        stringParam.setAttribute("name", stringIterator.key());
        stringParam.setAttribute("value", stringIterator.value());
        stringParams.appendChild(stringParam);
    }
    function.appendChild(stringParams);

    return function;

}

/*!
 * \brief Function::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Function::fromOpenIndyXML(QDomElement &xmlElem){

    if(xmlElem.isNull()){
        return false;
    }

    if(!xmlElem.hasAttribute("name") || !xmlElem.hasAttribute("plugin") || !xmlElem.hasAttribute("executionIndex")
            || !xmlElem.hasAttribute("type")){
        return false;
    }

    //set integer parameter
    QDomElement integerParameters = xmlElem.firstChildElement("integerParameters");
    if(!integerParameters.isNull()){
        QDomNodeList paramList = integerParameters.childNodes();
        for(int i = 0; i < paramList.size(); i++){
            QDomElement integerParameter = paramList.at(i).toElement();
            if(integerParameter.tagName().compare("parameter") == 0 && integerParameter.hasAttribute("value")
                    && integerParameter.hasAttribute("name")){
                this->scalarInputParams.intParameter.insert(integerParameter.attribute("name"), integerParameter.attribute("value").toInt());
            }
        }
    }

    //set double parameter
    QDomElement doubleParameters = xmlElem.firstChildElement("doubleParameters");
    if(!doubleParameters.isNull()){
        QDomNodeList paramList = doubleParameters.childNodes();
        for(int i = 0; i < paramList.size(); i++){
            QDomElement doubleParameter = paramList.at(i).toElement();
            if(doubleParameter.tagName().compare("parameter") == 0 && doubleParameter.hasAttribute("value")
                    && doubleParameter.hasAttribute("name")){
                this->scalarInputParams.doubleParameter.insert(doubleParameter.attribute("name"), doubleParameter.attribute("value").toDouble());
            }
        }
    }

    //set string parameter
    QDomElement stringParameters = xmlElem.firstChildElement("stringParameters");
    if(!stringParameters.isNull()){
        QDomNodeList paramList = stringParameters.childNodes();
        for(int i = 0; i < paramList.size(); i++){
            QDomElement stringParameter = paramList.at(i).toElement();
            if(stringParameter.tagName().compare("parameter") == 0 && stringParameter.hasAttribute("value")
                    && stringParameter.hasAttribute("name")){
                this->scalarInputParams.stringParameter.insert(stringParameter.attribute("name"), stringParameter.attribute("value"));
            }
        }
    }

    this->scalarInputParams.isValid = true;

    return true;

}

/*!
 * \brief Function::init
 */
void Function::init(){

}

/*!
 * \brief Function::exec
 * \param station
 * \return
 */
bool Function::exec(Station &station){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type station");
    return false;
}

/*!
 * \brief Function::exec
 * \param coordinateSystem
 * \return
 */
bool Function::exec(CoordinateSystem &coordinateSystem){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type coordinate system");
    return false;
}

/*!
 * \brief Function::exec
 * \param trafoParam
 * \return
 */
bool Function::exec(TrafoParam &trafoParam){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type transformation parameter");
    return false;
}

/*!
 * \brief Function::exec
 * \param circle
 * \return
 */
bool Function::exec(Circle &circle){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type circle");
    return false;
}

/*!
 * \brief Function::exec
 * \param cone
 * \return
 */
bool Function::exec(Cone &cone){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type cone");
    return false;
}

/*!
 * \brief Function::exec
 * \param cylinder
 * \return
 */
bool Function::exec(Cylinder &cylinder){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type cylinder");
    return false;
}

/*!
 * \brief Function::exec
 * \param ellipse
 * \return
 */
bool Function::exec(Ellipse &ellipse){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type ellipse");
    return false;
}

/*!
 * \brief Function::exec
 * \param ellipsoid
 * \return
 */
bool Function::exec(Ellipsoid &ellipsoid){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type ellipsoid");
    return false;
}

/*!
 * \brief Function::exec
 * \param hyperboloid
 * \return
 */
bool Function::exec(Hyperboloid &hyperboloid){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type hyperboloid");
    return false;
}

/*!
 * \brief Function::exec
 * \param line
 * \return
 */
bool Function::exec(Line &line){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type line");
    return false;
}

/*!
 * \brief Function::exec
 * \param nurbs
 * \return
 */
bool Function::exec(Nurbs &nurbs){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type nurbs");
    return false;
}

/*!
 * \brief Function::exec
 * \param paraboloid
 * \return
 */
bool Function::exec(Paraboloid &paraboloid){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type paraboloid");
    return false;
}

/*!
 * \brief Function::exec
 * \param plane
 * \return
 */
bool Function::exec(Plane &plane){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type plane");
    return false;
}

/*!
 * \brief Function::exec
 * \param point
 * \return
 */
bool Function::exec(Point &point){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type point");
    return false;
}

/*!
 * \brief Function::exec
 * \param pointCloud
 * \return
 */
bool Function::exec(PointCloud &pointCloud){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type point cloud");
    return false;
}

/*!
 * \brief Function::exec
 * \param angle
 * \return
 */
bool Function::exec(ScalarEntityAngle &angle){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type angle");
    return false;
}

/*!
 * \brief Function::exec
 * \param distance
 * \return
 */
bool Function::exec(ScalarEntityDistance &distance){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type distance");
    return false;
}

/*!
 * \brief Function::exec
 * \param measurementSeries
 * \return
 */
bool Function::exec(ScalarEntityMeasurementSeries &measurementSeries){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type measurement series");
    return false;
}

/*!
 * \brief Function::exec
 * \param temperature
 * \return
 */
bool Function::exec(ScalarEntityTemperature &temperature){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type temperature");
    return false;
}

/*!
 * \brief Function::exec
 * \param slottedHole
 * \return
 */
bool Function::exec(SlottedHole &slottedHole){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type slotted hole");
    return false;
}

/*!
 * \brief Function::exec
 * \param sphere
 * \return
 */
bool Function::exec(Sphere &sphere){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type sphere");
    return false;
}

/*!
 * \brief Function::exec
 * \param torus
 * \return
 */
bool Function::exec(Torus &torus){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type torus");
    return false;
}

/*!
 * \brief Function::setUseState
 * \param position
 * \param id
 * \param state
 */
void Function::setUseState(const int &position, const int &id, const bool &state){

    //get the element at position with id and set isUsed to state
    if(this->inputElements.contains(position)){
        int index = this->inputElements[position].indexOf(id);
        if(index > -1){
            this->inputElements[position][index].isUsed = state;
        }
    }

}
