#include "coordinatesystem.h"

#include "observation.h"
#include "trafoparam.h"
#include "geometry.h"
#include "point.h"

CoordinateSystem::CoordinateSystem(QObject *parent) : Feature(parent), origin(4),expansionOrigin(4){
    this->id = Configuration::generateID();
    this->isUpdated = false;
    this->isDrawn = true;
    this->expansionOrigin.setAt(3,1.0);
    this->isStationSystem = false;
}

CoordinateSystem::~CoordinateSystem(){

    //delete all observations made from this station coordinate system (only if this is a station system)
    foreach(Observation *myObs, this->observations){
        if(myObs != NULL){
            delete myObs;
            myObs = NULL;
        }
    }

    //delete transformation parameter sets from this coordinate system
    foreach(TrafoParam *myTrafo, this->trafoParams){
        if(myTrafo != NULL){
            delete myTrafo;
            myTrafo = NULL;
        }
    }

    //delete nominals of this coordinate system
    foreach(Geometry *myGeom, this->nominals){
        if(myGeom != NULL){
            delete myGeom;
            myGeom = NULL;
        }
    }

}

/*!
 * \brief CoordinateSystem::getObservations
 * \return
 */
const QList<Observation *> &CoordinateSystem::getObservations() const{
    return this->observations;
}

/*!
 * \brief CoordinateSystem::getObservation
 * \param observationId
 * \return
 */
Observation * const CoordinateSystem::getObservation(int observationId) const{
    try{

        foreach(Observation * obs, this->observations){
            if(obs != NULL && obs->getId() == observationId){
                return obs;
            }
        }
        return NULL;

    }catch(exception &e){

        return NULL;
    }
}

/*!
 * \brief CoordinateSystem::addObservation
 * \param observation
 * \return
 */
bool CoordinateSystem::addObservation(Observation * const observation){
    try{

        if(observation != NULL){
            this->observations.append(observation);
            emit this->observationsChanged(this->id);
            return true;
        }
        return false;

    }catch(exception &e){

        return false;
    }
}

/*!
 * \brief CoordinateSystem::getTransformationParameter
 * \return
 */
const QList<TrafoParam *> &CoordinateSystem::getTransformationParameters() const{
    return this->trafoParams;
}

/*!
 * \brief CoordinateSystem::getTransformationParameters
 * \param to
 * \return
 */
const QList<TrafoParam *> CoordinateSystem::getTransformationParameters(CoordinateSystem * const to) const{
    try{

        QList<TrafoParam *> result;

        foreach(TrafoParam *trafo, this->trafoParams){
            if(trafo != NULL && to != NULL){
                if(trafo->getDestinationSystem()->getId() == to->getId() || trafo->getStartSystem()->getId() == to->getId()){
                   result.append(trafo);
                }
            }
        }

        return result;

    }catch(exception &e){
        QList<TrafoParam *> result;
        return result;
    }
}

/*!
 * \brief CoordinateSystem::addTransformationParameter
 * \param trafoParam
 * \return
 */
bool CoordinateSystem::addTransformationParameter(TrafoParam * const trafoParam){
    try{

        if(trafoParam != NULL){
            this->trafoParams.append(trafoParam);
            emit this->transformationParametersChanged(this->id);
            return true;
        }
        return false;

    }catch(exception &e){

        return false;
    }
}

/*!
 * \brief CoordinateSystem::removeTransformationParameter
 * \param trafoParam
 * \return
 */
bool CoordinateSystem::removeTransformationParameter(TrafoParam * const trafoParam){
    try{

        if(trafoParam != NULL){
            int removeIndex = -1;
            for(int i = 0; i < this->trafoParams.size(); i++){
                if(this->trafoParams.at(i) != NULL && this->trafoParams.at(i)->getId() == trafoParam->getId()){
                    removeIndex = i;
                    break;
                }
            }
            if(removeIndex >= 0){
                this->trafoParams.removeAt(removeIndex);
                emit this->transformationParametersChanged(this->id);
                return true;
            }
        }
        return false;

    }catch(exception &e){

        return false;
    }
}

/*!
 * \brief CoordinateSystem::getNominals
 * \return
 */
const QList<Geometry *> &CoordinateSystem::getNominals() const{
    return this->nominals;
}

bool CoordinateSystem::addNominal(Geometry * const nominal)
{
    this->nominals.append(nominal);
    return true;
}

/*!
 * \brief CoordinateSystem::addNominal
 * \param nominal
 * \return
 */
/*bool CoordinateSystem::addNominal(Geometry * const nominal){
    try{

        if(nominal != NULL){
            this->nominals.append(nominal);
            emit this->nominalsChanged(this->id);
            return true;
        }
        return false;

    }catch(exception &e){

        return false;
    }
}*/

/*!
 * \brief CoordinateSystem::removeNominal
 * \param nominal
 * \return
 */
bool CoordinateSystem::removeNominal(Geometry * const nominal){
    try{

        if(nominal != NULL){
            int removeIndex = -1;
            for(int i = 0; i < this->nominals.size(); i++){
                if(this->nominals.at(i) != NULL && this->nominals.at(i)->getId() == nominal->getId()){
                    removeIndex = i;
                    break;
                }
            }
            if(removeIndex >= 0){
                this->nominals.removeAt(removeIndex);
                emit this->nominalsChanged(this->id);
                return true;
            }
        }
        return false;

    }catch(exception &e){

        return false;
    }
}

/*!
 * \brief CoordinateSystem::getIsActiveCoordinateSystem
 * \return
 */
bool CoordinateSystem::getIsActiveCoordinateSystem() const{
    return this->isActiveCoordinateSystem;
}

/*!
 * \brief CoordinateSystem::setActiveCoordinateSystemState
 */
void CoordinateSystem::setActiveCoordinateSystemState(bool isActiveCoordinateSystem){
    if(this->isActiveCoordinateSystem != isActiveCoordinateSystem){
        this->isActiveCoordinateSystem = isActiveCoordinateSystem;
        emit this->activeCoordinateSystemChanged(this->id);
    }
}

/*!
 * \brief getExpansionOrigin returns the vector of the origiin of expansion
 * \return
 */
OiVec CoordinateSystem::getExpansionOrigin()
{
    return this->expansionOrigin;
}

/*!
 * \brief setExpansionOrigin
 * \param expOri
 */
void CoordinateSystem::setExpansionOrigin(OiVec expOri)
{
    this->expansionOrigin = expOri;
}

/*!
 * \brief setExpansionOriginX
 * \param x
 */
void CoordinateSystem::setExpansionOriginX(double x)
{
    this->expansionOrigin.setAt(0,x);
}

/*!
 * \brief setExpansionOriginY
 * \param y
 */
void CoordinateSystem::setExpansionOriginY(double y)
{
    this->expansionOrigin.setAt(1,y);
}

/*!
 * \brief setExpansionOriginZ
 * \param z
 */
void CoordinateSystem::setExpansionOriginZ(double z)
{
    this->expansionOrigin.setAt(2,z);
}

void CoordinateSystem::recalc(){

}

QString CoordinateSystem::getDisplayX() const{
    return QString::number(this->origin.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString CoordinateSystem::getDisplayY() const{
    return QString::number(this->origin.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString CoordinateSystem::getDisplayZ() const{
    return QString::number(this->origin.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString CoordinateSystem::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString CoordinateSystem::getDisplayExpansionOriginX() const
{
    return QString::number(this->expansionOrigin.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString CoordinateSystem::getDisplayExpansionOriginY() const
{
    return QString::number(this->expansionOrigin.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString CoordinateSystem::getDisplayExpansionOriginZ() const
{
    return QString::number(this->expansionOrigin.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

/*!
 * \brief CoordinateSystem::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement CoordinateSystem::toOpenIndyXML(QDomDocument &xmlDoc){

    QDomElement coordinateSystem = Feature::toOpenIndyXML(xmlDoc);

    if(coordinateSystem.isNull()){
        return coordinateSystem;
    }

    coordinateSystem.setTagName("coordinateSystem");

    //add trafo params
    if(this->trafoParams.size() > 0){
        QDomElement trafoParams = xmlDoc.createElement("transformationParameters");
        foreach(TrafoParam *tp, this->trafoParams){
            if(tp != NULL){
                QDomElement trafoParam = xmlDoc.createElement("transformationParameter");
                trafoParam.setAttribute("ref", tp->getId());
                trafoParams.appendChild(trafoParam);
            }
        }
        coordinateSystem.appendChild(trafoParams);
    }

    //add observations
    if(this->observations.size() > 0){
        QDomElement observations = xmlDoc.createElement("observations");
        foreach(Observation *obs, this->observations){
            if(obs != NULL){
                QDomElement observation = xmlDoc.createElement("observation");
                observation.setAttribute("ref", obs->getId());
                observations.appendChild(observation);
            }
        }
        coordinateSystem.appendChild(observations);
    }

    //add nominals
    if(this->nominals.size() > 0){
        QDomElement nominals = xmlDoc.createElement("nominalGeometries");
        foreach(Geometry *geom, this->nominals){
            if(geom != NULL){
                QDomElement nominal = xmlDoc.createElement("geometry");
                nominal.setAttribute("ref", geom->getId());
                nominals.appendChild(nominal);
            }
        }
        coordinateSystem.appendChild(nominals);
    }

    //add expansion origin
    if(this->expansionOrigin.getSize() >= 3){
        QDomElement expansionOrigin = xmlDoc.createElement("expansionOrigin");
        expansionOrigin.setAttribute("x", this->expansionOrigin.getAt(0));
        expansionOrigin.setAttribute("y", this->expansionOrigin.getAt(1));
        expansionOrigin.setAttribute("z", this->expansionOrigin.getAt(2));
        coordinateSystem.appendChild(expansionOrigin);
    }

    return coordinateSystem;

}

/*!
 * \brief CoordinateSystem::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool CoordinateSystem::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Feature::fromOpenIndyXML(xmlElem);

    if(result){

        //set expansion origin
        QDomElement expansionOrigin = xmlElem.firstChildElement("expansionOrigin");
        if(!expansionOrigin.isNull()){
            if(!expansionOrigin.hasAttribute("x") || !expansionOrigin.hasAttribute("y") || !expansionOrigin.hasAttribute("z")){
                return false;
            }
            this->expansionOrigin.setAt(0, expansionOrigin.attribute("x").toDouble());
            this->expansionOrigin.setAt(1, expansionOrigin.attribute("y").toDouble());
            this->expansionOrigin.setAt(2, expansionOrigin.attribute("z").toDouble());
        }

    }

    return result;

}

/*!
 * \brief CoordinateSystem::fromOpenIndyXML
 * \param xml
 * \return
 */
ElementDependencies CoordinateSystem::fromOpenIndyXML(QXmlStreamReader &xml){

    ElementDependencies dependencies;

    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("name")){
        this->name = attributes.value("name").toString();
    }
    if(attributes.hasAttribute("id")) {
        this->id = attributes.value("id").toInt();
        dependencies.elementID = this->id;
        dependencies.typeOfElement = Configuration::eCoordinateSystemElement;
    }

    xml.readNext();

    while( !xml.atEnd() && xml.name().compare("coordinatesystem") != 0 ){

        if(xml.tokenType() == QXmlStreamReader::StartElement){

            if(xml.name().compare("observations") == 0){

                xml.readNext();

                while(xml.name().compare("observation") == 0){
                    if(xml.tokenType() == QXmlStreamReader::StartElement && xml.attributes().hasAttribute("ref")){
                        dependencies.addObservationID(xml.attributes().value("ref").toInt());
                    }
                    xml.readNext();
                }

            }else if(xml.name().compare("nominalGeometries") == 0){

                xml.readNext();

                while(xml.name().compare("geometry") == 0){
                    if(xml.tokenType() == QXmlStreamReader::StartElement && xml.attributes().hasAttribute("ref")){
                        dependencies.addFeatureID(xml.attributes().value("ref").toInt(),"nominalGeometry");
                    }
                    xml.readNext();
                }

            }else if(xml.name().compare("expansionOrigin") == 0){

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "expansionOrigin")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes memberAttributes = xml.attributes();
                        if(memberAttributes.hasAttribute("x")){
                            this->expansionOrigin.setAt(0,memberAttributes.value("x").toDouble());
                        }
                        if(memberAttributes.hasAttribute("y")){
                            this->expansionOrigin.setAt(1,memberAttributes.value("y").toDouble());
                        }
                        if(memberAttributes.hasAttribute("z")){
                            this->expansionOrigin.setAt(2,memberAttributes.value("z").toDouble());
                        }
                    }

                    xml.readNext();
                }

            }else if(xml.name().compare("functions") == 0){

                this->readFunction(xml, dependencies);
                xml.readNext();

            }else if(xml.name().compare("usedFor") == 0){

                this->readFeatureAttributes(xml, dependencies);
                xml.readNext();

            }else if(xml.name().compare("previouslyNeeded") == 0){

                this->readFeatureAttributes(xml, dependencies);
                xml.readNext();

            }else{
                xml.readNext();
            }

        }else{
            xml.readNext();
        }

    }

    return dependencies;








    /*while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "coordinatesystem")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {



            if(xml.name().compare("observation") == 0){
                if(attributes.hasAttribute("ref")){
                    dependencies.addObservationID(attributes.value("ref").toInt());
                }
            }else if(xml.name().compare("nominalGeometry") == 0){
                if(attributes.hasAttribute("ref")){
                    dependencies.addFeatureID(attributes.value("ref").toInt(),"nominalGeometry");
                }
            }else{
                this->readFeatureAttributes(xml, dependencies);
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

                            }else if (memberAttributes.value("type") == "nominalGeometry"){

                                if(memberAttributes.hasAttribute("ref")){
                                    dependencies.addFeatureID(memberAttributes.value("ref").toInt(),"nominalGeometry");
                                }
                            }else{
                                 this->readFeatureAttributes(xml, dependencies);
                            }


                        }
                    }

                    xml.readNext();
                }
            }

            if(xml.name() == "expansionOrigin"){

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "expansionOrigin")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes memberAttributes = xml.attributes();
                        if(memberAttributes.hasAttribute("x")){
                            this->expansionOrigin.setAt(0,memberAttributes.value("x").toDouble());
                        }
                        if(memberAttributes.hasAttribute("y")){
                            this->expansionOrigin.setAt(1,memberAttributes.value("y").toDouble());
                        }
                        if(memberAttributes.hasAttribute("z")){
                            this->expansionOrigin.setAt(2,memberAttributes.value("z").toDouble());
                        }
                    }

                    xml.readNext();
                }
            }

            if(xml.name() == "function"){

                this->readFunction(xml, dependencies);

            }

        }

        xml.readNext();
    }

    return dependencies;*/
}
