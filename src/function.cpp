#include "function.h"

/*!
 * \brief Function::exec
 * \param c
 * \return
 */
bool Function::exec(Circle &c){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type circle");
    return false;
}

/*!
 * \brief Function::exec
 * \param c
 * \return
 */
bool Function::exec(Cone &c){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type cone");
    return false;
}

/*!
 * \brief Function::exec
 * \param c
 * \return
 */
bool Function::exec(Cylinder &c){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type cylinder");
    return false;
}

/*!
 * \brief Function::exec
 * \param c
 * \return
 */
bool Function::exec(CoordinateSystem &c){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type point");
    return false;
}

/*!
 * \brief Function::exec
 * \param e
 * \return
 */
bool Function::exec(Ellipsoid &e){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type ellipsoid");
    return false;
}

/*!
 * \brief Function::exec
 * \param h
 * \return
 */
bool Function::exec(Hyperboloid &h){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type hyperboloid");
    return false;
}

/*!
 * \brief Function::exec
 * \param l
 * \return
 */
bool Function::exec(Line &l){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type line");
    return false;
}

/*!
 * \brief Function::exec
 * \param n
 * \return
 */
bool Function::exec(Nurbs &n){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type nurbs");
    return false;
}

/*!
 * \brief Function::exec
 * \param p
 * \return
 */
bool Function::exec(Paraboloid &p){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type paraboloid");
    return false;
}

/*!
 * \brief Function::exec
 * \param p
 * \return
 */
bool Function::exec(Plane &p){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type plane");
    return false;
}

/*!
 * \brief Function::exec
 * \param p
 * \return
 */
bool Function::exec(Point &p){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type point");
    return false;
}

/*!
 * \brief Function::exec
 * \param p
 * \return
 */
bool Function::exec(PointCloud &p){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type pointcloud");
    return false;
}

/*!
 * \brief Function::exec
 * \param s
 * \return
 */
bool Function::exec(ScalarEntityAngle &s){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type angle");
    return false;
}

/*!
 * \brief Function::exec
 * \param s
 * \return
 */
bool Function::exec(ScalarEntityDistance &s){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type distance");
    return false;
}

/*!
 * \brief Function::exec
 * \return
 */
bool Function::exec(ScalarEntityTemperature &s){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type temperature");
    return false;
}

/*!
 * \brief Function::exec
 * \return
 */
bool Function::exec(ScalarEntityMeasurementSeries &s){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type temperature");
    return false;
}

/*!
 * \brief Function::exec
 * \param s
 * \return
 */
bool Function::exec(Sphere &s){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type sphere");
    return false;
}

/*!
 * \brief Function::exec
 * \param s
 * \return
 */
bool Function::exec(Station &s){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type point");
    return false;
}

/*!
 * \brief Function::exec
 * \param t
 * \return
 */
bool Function::exec(TrafoParam &t){
    this->writeToConsole("Function \"" + this->getMetaData().name + "\" not implemented for type point");
    return false;
}

/*!
 * \brief getIntegerParameter
 * Can be used for custom parameter definition of type int
 * \return
 */
QMap<QString, int> Function::getIntegerParameter() const{
    QMap<QString, int> result;
    return result;
}

/*!
 * \brief getDoubleParameter
 * Can be used for custom parameter definition of type double
 * \return
 */
QMap<QString,double> Function::getDoubleParameter() const{
    QMap<QString, double> result;
    return result;
}

/*!
 * \brief getStringParameter
 * Can be used for custom parameter definition string
 * \return
 */
QMultiMap<QString, QString> Function::getStringParameter() const{
    QMultiMap<QString, QString> result;
    return result;
}

/*!
 * \brief Function::getResultProtocol
 * The complete result protocol is shown for every function after execution
 * \return
 */
QStringList Function::getResultProtocol() const{
    return QStringList();
}

/*!
 * \brief Function::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Function::toOpenIndyXML(QDomDocument &xmlDoc) const{
/*
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
*/
    return QDomElement();
}

/*!
 * \brief Function::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Function::fromOpenIndyXML(QDomElement &xmlElem){
/*
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
*/
    return false;
}

/*!
 * \brief Function::setScalarInputParams
 * \param params
 */
void Function::setScalarInputParams(const ScalarInputParams &params){
    this->scalarInputParams = params;
    this->scalarInputParams.isValid = true;
}

/*!
 * \brief Function::getScalarInputParams
 * \return
 */
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

/*!
 * \brief Function::isValid
 * Checks wether all elements needed for calculation are present
 * \return
 */
bool Function::isValid(){
    return true;
    /*


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
    return true;*/
}

/*!
 * \brief Function::getFeatureOrder
 * \return
 */
const QMultiMap<int, InputElement> &Function::getFeatureOrder() const{
    return this->featureOrder;
}

/*!
 * \brief Function::getStatistic
 * \return
 */
Statistic& Function::getStatistic(){
    return this->myStatistic;
}

/*!
 * \brief Function::getId
 * \return
 */
const int &Function::getId() const{
    return this->id;
}

/*!
 * \brief Function::addStation
 * \param station
 * \param position
 */
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

/*!
 * \brief Function::removeStation
 * Remove the station with the specified id from the list of stations (if present)
 * \param id
 */
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

/*!
 * \brief Function::getStation
 * Returns the station with the specified id (if present) or an empty pointer
 * \param id
 * \return
 */
QPointer<Station> Function::getStation(const int &id) const{
    return stations.value(id, QPointer<Station>());
}

/*!
 * \brief Function::getStations
 * Returns the complete list of stations
 * \return
 */
const QMap<int, QPointer<Station> > &Function::getStations() const{
    return this->stations;
}

/*!
 * \brief Function::addCoordSystem
 * \param coordSys
 * \param position
 */
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

/*!
 * \brief Function::removeCoordSystem
 * Remove the coordinate system with the specified id from the list of coordinate systems (if present)
 * \param id
 */
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

/*!
 * \brief Function::getCoordinateSystem
 * Returns the coordinate system with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<CoordinateSystem> Function::getCoordinateSystem(const int &id) const{
    return coordSystems.value(id, QPointer<CoordinateSystem>());
}

/*!
 * \brief Function::getCoordinateSystems
 * Returns the complete list of coordinate systems
 * \return
 */
const QMap<int, QPointer<CoordinateSystem> > &Function::getCoordinateSystems() const{
    return this->coordSystems;
}

/*!
 * \brief Function::addTrafoParam
 * \param trafoParam
 * \param position
 */
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

/*!
 * \brief Function::removeTrafoParam
 * Remove the transformations parameter with the specified id from the list of transformation parameters (if present)
 * \param id
 */
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

/*!
 * \brief Function::getTrafoParam
 * Returns the transformation parameter with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<TrafoParam> Function::getTrafoParam(const int &id) const{
    return trafoParams.value(id, QPointer<TrafoParam>());
}

/*!
 * \brief Function::getTrafoParams
 * Returns the complete list of transformation parameters
 * \return
 */
const QMap<int, QPointer<TrafoParam> > &Function::getTrafoParams() const{
    return this->trafoParams;
}

/*!
 * \brief Function::addPoint
 * \param point
 * \param position
 */
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

/*!
 * \brief Function::removePoint
 * Remove the point with the specified id from the list of points (if present)
 * \param id
 */
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

/*!
 * \brief Function::getPoint
 * Returns the point with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Point> Function::getPoint(const int &id) const{
    return points.value(id, QPointer<Point>());
}

/*!
 * \brief Function::getPoints
 * Returns the complete list of points
 * \return
 */
const QMap<int, QPointer<Point> > &Function::getPoints() const{
    return this->points;
}

/*!
 * \brief Function::addLine
 * \param l
 * \param position
 */
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

/*!
 * \brief Function::removeLine
 * Remove the line with the specified id from the list of lines (if present)
 * \param id
 */
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

/*!
 * \brief Function::getLine
 * Returns the line with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Line> Function::getLine(const int &id) const{
    return lines.value(id, QPointer<Line>());
}

/*!
 * \brief Function::getLines
 * Returns the complete list of lines
 * \return
 */
const QMap<int, QPointer<Line> > &Function::getLines() const{
    return this->lines;
}

/*!
 * \brief Function::addPlane
 * \param p
 * \param position
 */
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

/*!
 * \brief removePlane
 * Remove the plane with the specified id from the list of planes (if present)
 * \param id
 */
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

/*!
 * \brief getPlane
 * Returns the plane with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Plane> Function::getPlane(const int &id) const{
    return planes.value(id, QPointer<Plane>());
}

/*!
 * \brief getPlanes
 * Returns the complete list of planes
 * \return
 */
const QMap<int, QPointer<Plane> > &Function::getPlanes() const{
    return this->planes;
}

/*!
 * \brief Function::addSphere
 * \param s
 * \param position
 */
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

/*!
 * \brief removeSphere
 * Remove the sphere with the specified id from the list of spheres (if present)
 * \param id
 */
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

/*!
 * \brief getSphere
 * Returns the sphere with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Sphere> Function::getSphere(const int &id) const{
    return spheres.value(id, QPointer<Sphere>());
}

/*!
 * \brief getSpheres
 * Returns the complete list of spheres
 * \return
 */
const QMap<int, QPointer<Sphere> > &Function::getSpheres() const{
    return this->spheres;
}

/*!
 * \brief Function::addCircle
 * \param c
 * \param position
 */
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

/*!
 * \brief Function::removeCircle
 * Remove the circle with the specified id from the list of circles (if present)
 * \param id
 */
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

/*!
 * \brief Function::getCircle
 * Returns the circle with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Circle> Function::getCircle(const int &id) const{
    return circles.value(id, QPointer<Circle>());
}

/*!
 * \brief Function::getCircles
 * Returns the complete list of circles
 * \return
 */
const QMap<int, QPointer<Circle> > &Function::getCircles() const{
    return this->circles;
}

/*!
 * \brief Function::addCone
 * \param c
 * \param position
 */
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

/*!
 * \brief removeCone
 * Remove the cone with the specified id from the list of cones (if present)
 * \param id
 */
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

/*!
 * \brief getCone
 * Returns the cone with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Cone> Function::getCone(const int &id) const{
    return cones.value(id, QPointer<Cone>());
}

/*!
 * \brief getCones
 * Returns the complete list of cones
 * \return
 */
const QMap<int, QPointer<Cone> > &Function::getCones() const{
    return this->cones;
}

/*!
 * \brief Function::addCylinder
 * \param c
 * \param position
 */
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

/*!
 * \brief removeCylinder
 * Remove the cylinder with the specified id from the list of cylinders (if present)
 * \param id
 */
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

/*!
 * \brief getCylinder
 * Returns the cylinder with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Cylinder> Function::getCylinder(const int &id) const{
    return cylinders.value(id, QPointer<Cylinder>());
}

/*!
 * \brief getCylinders
 * Returns the complete list of cylinders
 * \return
 */
const QMap<int, QPointer<Cylinder> > &Function::getCylinders() const{
    return this->cylinders;
}

/*!
 * \brief Function::addEllipsoid
 * \param e
 * \param position
 */
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

/*!
 * \brief removeEllipsoid
 * Remove the ellipsoid with the specified id from the list of ellipsoids (if present)
 * \param id
 */
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

/*!
 * \brief getEllipsoid
 * Returns the ellipsoid with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Ellipsoid> Function::getEllipsoid(const int &id) const{
    return ellipsoids.value(id, QPointer<Ellipsoid>());
}

/*!
 * \brief getEllipsoids
 * Returns the complete list of ellipsoids
 * \return
 */
const QMap<int, QPointer<Ellipsoid> > &Function::getEllipsoids() const{
    return this->ellipsoids;
}

/*!
 * \brief Function::addParaboloid
 * \param p
 * \param position
 */
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

/*!
 * \brief removeParaboloid
 * Remove the paraboloid with the specified id from the list of paraboloids (if present)
 * \param id
 */
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

/*!
 * \brief getParaboloid
 * Returns the paraboloid with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Paraboloid> Function::getParaboloid(const int &id) const{
    return paraboloids.value(id, QPointer<Paraboloid>());
}

/*!
 * \brief getParaboloids
 * Returns the complete list of paraboloids
 * \return
 */
const QMap<int, QPointer<Paraboloid> > &Function::getParaboloids() const{
    return this->paraboloids;
}

/*!
 * \brief Function::addHyperboloid
 * \param h
 * \param position
 */
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

/*!
 * \brief removeHyperboloid
 * Remove the hyperboloid with the specified id from the list of hyperboloids (if present)
 * \param id
 */
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

/*!
 * \brief getHyperboloid
 * Returns the hyperboloid with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Hyperboloid> Function::getHyperboloid(const int &id) const{
    return hyperboloids.value(id, QPointer<Hyperboloid>());
}

/*!
 * \brief getHyperboloids
 * Returns the complete list of hyperboloids
 * \return
 */
const QMap<int, QPointer<Hyperboloid> > &Function::getHyperboloids() const{
    return this->hyperboloids;
}

/*!
 * \brief Function::addNurb
 * \param n
 * \param position
 */
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

/*!
 * \brief removeNurb
 * Remove the nurb with the specified id from the list of nurbs (if present)
 * \param id
 */
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

/*!
 * \brief getNurb
 * Returns the nurb with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Nurbs> Function::getNurb(const int &id) const{
    return nurbs.value(id, QPointer<Nurbs>());
}

/*!
 * \brief getNurbs
 * Returns the complete list of nurbs
 * \return
 */
const QMap<int, QPointer<Nurbs> > &Function::getNurbs() const{
    return this->nurbs;
}

/*!
 * \brief Function::addPointCloud
 * \param p
 * \param position
 */
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

/*!
 * \brief removePointCloud
 * Remove the point cloud with the specified id from the list of point clouds (if present)
 * \param id
 */
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

/*!
 * \brief getPointCloud
 * Returns the point cloud with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<PointCloud> Function::getPointCloud(const int &id) const{
    return pointClouds.value(id, QPointer<PointCloud>());
}

/*!
 * \brief getPointClouds
 * Returns the complete list of point clouds
 * \return
 */
const QMap<int, QPointer<PointCloud> > &Function::getPointClouds() const{
    return this->pointClouds;
}

/*!
 * \brief addScalarEntityDistance
 * \param sed
 * \param position
 */
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

/*!
 * \brief removeScalarEntityDistance
 * Remove the distance with the specified id from the list of distances (if present)
 * \param id
 */
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

/*!
 * \brief getScalarEntityDistance
 * Returns the distance with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<ScalarEntityDistance> Function::getScalarEntityDistance(const int &id) const{
    return scalarEntityDistances.value(id, QPointer<ScalarEntityDistance>());
}

/*!
 * \brief getScalarEntityDistances
 * Returns the complete list of distances
 * \return
 */
const QMap<int, QPointer<ScalarEntityDistance> > &Function::getScalarEntityDistances() const{
    return this->scalarEntityDistances;
}

/*!
 * \brief Function::addScalarEntityAngle
 * \param sea
 * \param position
 */
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

/*!
 * \brief removeScalarEntityAngle
 * Remove the angle with the specified id from the list of angles (if present)
 * \param id
 */
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

/*!
 * \brief getScalarEntityAngle
 * Returns the angle with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<ScalarEntityAngle> Function::getScalarEntityAngle(const int &id) const{
    return scalarEntityAngles.value(id, QPointer<ScalarEntityAngle>());
}

/*!
 * \brief getScalarEntityAngles
 * Returns the complete list of angles
 * \return
 */
const QMap<int, QPointer<ScalarEntityAngle> > &Function::getScalarEntityAngles() const{
    return this->scalarEntityAngles;
}

/*!
 * \brief addObservation
 * \param o
 * \param position
 */
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

/*!
 * \brief removeObservation
 * Remove the observation with the specified id from the list of observations (if present)
 * \param id
 */
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

/*!
 * \brief getObservation
 * Returns the observation with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Observation> Function::getObservation(const int &id) const{
    return observations.value(id, QPointer<Observation>());
}

/*!
 * \brief getObservations
 * Returns the complete list of observations
 * \return
 */
const QMap<int, QPointer<Observation> > &Function::getObservations() const{
    return this->observations;
}

/*!
 * \brief addReadingPolar
 * \param r
 * \param position
 */
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

/*!
 * \brief removeReadingPolar
 * Remove the polar reading with the specified id from the list of polar readings (if present)
 * \param id
 */
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

/*!
 * \brief getReadingPolar
 * Returns the polar reading with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Reading> Function::getReadingPolar(const int &id) const{
    return polarReadings.value(id, QPointer<Reading>());
}

/*!
 * \brief getPolarReadings
 * Returns the complete list of polar readings
 * \return
 */
const QMap<int, QPointer<Reading> > &Function::getPolarReadings() const{
    return this->polarReadings;
}

/*!
 * \brief addReadingDistance
 * \param r
 * \param position
 */
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

/*!
 * \brief removeReadingDistance
 * Remove the distance reading with the specified id from the list of distance readings (if present)
 * \param id
 */
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

/*!
 * \brief getReadingDistance
 * Returns the distance reading with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Reading> Function::getReadingDistance(const int &id) const{
    return distanceReadings.value(id, QPointer<Reading>());
}

/*!
 * \brief getDistanceReadings
 * Returns the complete list of distances readings
 * \return
 */
const QMap<int, QPointer<Reading> > &Function::getDistanceReadings() const{
    return this->distanceReadings;
}

/*!
 * \brief addReadingDirection
 * \param r
 * \param position
 */
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

/*!
 * \brief removeReadingDirection
 * Remove the direction reading with the specified id from the list of direction readings (if present)
 * \param id
 */
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

/*!
 * \brief getReadingDirection
 * Returns the direction reading with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Reading> Function::getReadingDirection(const int &id) const{
    return directionReadings.value(id, QPointer<Reading>());
}

/*!
 * \brief getDirectionReadings
 * Returns the complete list of direction readings
 * \return
 */
const QMap<int, QPointer<Reading> > &Function::getDirectionReadings() const{
    return this->directionReadings;
}

/*!
 * \brief addReadingCartesian
 * \param r
 * \param position
 */
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

/*!
 * \brief removeReadingCartesian
 * Remove the cartesian reading with the specified id from the list of cartesian readings (if present)
 * \param id
 */
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

/*!
 * \brief getReadingCartesian
 * Returns the cartesian reading with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<Reading> Function::getReadingCartesian(const int &id) const{
    return cartesianReadings.value(id, QPointer<Reading>());
}

/*!
 * \brief getCartesianReadings
 * Returns the complete list of cartesian readings
 * \return
 */
const QMap<int, QPointer<Reading> > &Function::getCartesianReadings() const{
    return this->cartesianReadings;
}

/*!
 * \brief addScalarEntityTemperature
 * \param SET
 * \param position
 */
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

/*!
 * \brief removeScalarEntityTemperature removes the scalar entity temperature with the specified id from the list of scalar entity temperatures (if present)
 * \param id
 */
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

/*!
 * \brief getScalaeEntityTemperature returns the scalar entity temperature with the specified id (if present) or NULL
 * \param id
 * \return
 */
QPointer<ScalarEntityTemperature> Function::getScalarEntityTemperature(const int &id) const{
    return scalarEntityTemperatures.value(id, QPointer<ScalarEntityTemperature>());
}

/*!
 * \brief getScalarEntityTemperatures returns the complete list of scalar entity temperatures
 * \return
 */
const QMap<int, QPointer<ScalarEntityTemperature> > &Function::getScalarEntityTemperatures() const{
    return this->scalarEntityTemperatures;
}

/*!
 * \brief Function::addReading
 * \param r
 * \param position
 */
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

/*!
 * \brief Function::addFeature
 * \param feature
 * \param position
 */
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

/*!
 * \brief clear
 * Reset function
 */
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
    this->myStatistic.isValid = false;

}

/*!
 * \brief Function::clearResults
 * Clear statistic of this function
 */
void Function::clearResults(){
/*
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
    this->featureOrder = newFeatureOrder;*/

}

/*!
 * \brief Function::setUseState
 * Set state of feature
 * \param id
 * \param state
 */
void Function::setUseState(const int &id, bool state){
    /*for(int i = 0; i < this->getNeededElements().size(); i++){
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
    }*/
}

/*!
 * \brief Function::getOiEmitter
 * \return
 */
OiFunctionEmitter& Function::getOiEmitter(){
    return this->myEmitter;
}

/*!
 * \brief Function::writeToConsole
 * Write a specified message to the console
 * \param message
 */
void Function::writeToConsole(QString message){
    myEmitter.sendString(message);
}

/*!
 * \brief Function::removeFeature
 * Remove the feature with the specified id and of given type
 * \param typeOfFeature
 * \param id
 */
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
