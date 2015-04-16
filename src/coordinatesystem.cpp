#include "coordinatesystem.h"

#include "featurewrapper.h"
#include "observation.h"
#include "geometry.h"
#include "trafoparam.h"
/*
#include "observation.h"
#include "trafoparam.h"
#include "geometry.h"
#include "point.h"

CoordinateSystem::CoordinateSystem(QObject *parent) : Feature(parent), origin(4),expansionOrigin(4){
    //this->id = Configuration::generateID();
    this->isUpdated = false;
    this->isDrawn = true;
    this->expansionOrigin.setAt(3,1.0);
    this->origin.setAt(3,1.0);
    this->isStationSystem = false;
    this->isActiveCoordinateSystem = false;
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
    foreach(FeatureWrapper *myGeom, this->nominals){
        if(myGeom != NULL && myGeom->getGeometry() != NULL){
            delete myGeom->getGeometry();
            delete myGeom;
            myGeom = NULL;
        }
    }

}

const QList<QPointer<Observation> > &CoordinateSystem::getObservations() const{
    return this->observations;
}

QPointer<Observation> CoordinateSystem::getObservation(const int &observationId) const{
    try{

        foreach(QPointer<Observation> obs, this->observations){
            if(!obs.isNull() && obs->getId() == observationId){
                return obs;
            }
        }
        return QPointer<Observation>();

    }catch(exception &e){
        return QPointer<Observation>();
    }
}

bool CoordinateSystem::addObservation(const QPointer<Observation> &observation){
    try{

        if(!observation.isNull()){
            this->observations.append(observation);
            emit this->observationsChanged(this->id, observation->getId());
            return true;
        }
        return false;

    }catch(exception &e){

        return false;
    }
}

const QList<QPointer<TrafoParam> > &CoordinateSystem::getTransformationParameters() const{
    return this->trafoParams;
}

const QList< QPointer<TrafoParam> > CoordinateSystem::getTransformationParameters(QPointer<CoordinateSystem> const &to) const{
    try{

        QList< QPointer<TrafoParam> > result;

        foreach(QPointer<TrafoParam> trafo, this->trafoParams){
            if(!trafo.isNull() && !to.isNull()){
                if(trafo->getDestinationSystem()->getId() == to->getId() || trafo->getStartSystem()->getId() == to->getId()){
                   result.append(trafo);
                }
            }
        }

        return result;

    }catch(exception &e){
        QList< QPointer<TrafoParam> > result;
        return result;
    }
}

bool CoordinateSystem::addTransformationParameter(const QPointer<TrafoParam> &trafoParam){
    try{

        if(!trafoParam.isNull()){
            this->trafoParams.append(trafoParam);
            emit this->transformationParametersChanged(this->id);
            return true;
        }
        return false;

    }catch(exception &e){

        return false;
    }
}

bool CoordinateSystem::removeTransformationParameter(const QPointer<TrafoParam> &trafoParam){
    try{

        if(!trafoParam.isNull()){
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

const QList<QPointer<FeatureWrapper> > &CoordinateSystem::getNominals() const{
    return this->nominals;
}

bool CoordinateSystem::addNominal(Geometry * const nominal)
{
    this->nominals.append(nominal);
    return true;
}

bool CoordinateSystem::addNominal(const QPointer<FeatureWrapper> &nominal){
    try{

        if(!nominal.isNull() && !nominal->getGeometry().isNull()){

            if(nominal->getGeometry()->getFeatureName().compare("") == 0){
                //nominal->getGeometry()->setFeatureName(QString("%1").arg(Configuration::generateID()));
            }

            nominal->getGeometry()->setNominalSystem(this);
            nominal->getGeometry()->setIsSolved(true);

            this->nominals.append(nominal);
            emit this->nominalsChanged(this->id);
            return true;
        }
        return false;

    }catch(exception &e){

        return false;
    }
}

bool CoordinateSystem::addNominals(const QList< QPointer<FeatureWrapper> > &nominals){
    try{

        if(nominals.size() > 0){

            foreach(const QPointer<FeatureWrapper> &geom, nominals){
                geom->getGeometry()->setNominalSystem(this);
                geom->getGeometry()->setIsSolved(true);
                geom->setCoordinateSystem(this);
                this->nominals.append(geom);
            }

            emit this->nominalsChanged(this->id);
            return true;
        }
        return false;

    }catch(exception &e){

        return false;
    }
}

bool CoordinateSystem::removeNominal(const QPointer<FeatureWrapper> &nominal){
    try{

        if(!nominal.isNull() && !nominal->getGeometry().isNull()){
            int removeIndex = -1;
            for(int i = 0; i < this->nominals.size(); i++){
                if(this->nominals.at(i) != NULL && this->nominals.at(i)->getGeometry() != NULL
                        && this->nominals.at(i)->getGeometry()->getId() == nominal->getGeometry()->getId()){
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

bool CoordinateSystem::removeNominal(const int &featureId){
    try{

        int removeIndex = -1;
        for(int i = 0; i < this->nominals.size(); i++){
            if(this->nominals.at(i) != NULL && this->nominals.at(i)->getGeometry() != NULL
                    && this->nominals.at(i)->getGeometry()->getId() == featureId){
                removeIndex = i;
                break;
            }
        }
        if(removeIndex >= 0){
            this->nominals.removeAt(removeIndex);
            emit this->nominalsChanged(this->id);
            return true;
        }
        return false;

    }catch(exception &e){

        return false;
    }
}

bool CoordinateSystem::getIsActiveCoordinateSystem() const{
    return this->isActiveCoordinateSystem;
}

void CoordinateSystem::setActiveCoordinateSystemState(const bool &isActiveCoordinateSystem){
    if(this->isActiveCoordinateSystem != isActiveCoordinateSystem){
        this->isActiveCoordinateSystem = isActiveCoordinateSystem;
        emit this->activeCoordinateSystemChanged(this->id);
    }
}

OiVec CoordinateSystem::getExpansionOrigin()
{
    return this->expansionOrigin;
}

void CoordinateSystem::setExpansionOrigin(OiVec expOri)
{
    this->expansionOrigin = expOri;
}

void CoordinateSystem::setExpansionOriginX(double x)
{
    this->expansionOrigin.setAt(0,x);
}

void CoordinateSystem::setExpansionOriginY(double y)
{
    this->expansionOrigin.setAt(1,y);
}

void CoordinateSystem::setExpansionOriginZ(double z)
{
    this->expansionOrigin.setAt(2,z);
}

void CoordinateSystem::recalc(){

}

QString CoordinateSystem::getDisplayX() const{
    double displayValue = this->origin.getAt(0);
    this->convertMetricValue(displayValue);
    return QString::number(displayValue, 'f', 2);
    //return QString::number(this->origin.getAt(0)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
}

QString CoordinateSystem::getDisplayY() const{
    double displayValue = this->origin.getAt(1);
    this->convertMetricValue(displayValue);
    return QString::number(displayValue, 'f', 2);
    //return QString::number(this->origin.getAt(1)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
}

QString CoordinateSystem::getDisplayZ() const{
    double displayValue = this->origin.getAt(2);
    this->convertMetricValue(displayValue);
    return QString::number(displayValue, 'f', 2);
    //return QString::number(this->origin.getAt(2)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
}

QString CoordinateSystem::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString CoordinateSystem::getDisplayExpansionOriginX() const
{
    double displayValue = this->expansionOrigin.getAt(0);
    this->convertMetricValue(displayValue);
    return QString::number(displayValue, 'f', 2);
    //return QString::number(this->expansionOrigin.getAt(0)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
}

QString CoordinateSystem::getDisplayExpansionOriginY() const
{
    double displayValue = this->expansionOrigin.getAt(1);
    this->convertMetricValue(displayValue);
    return QString::number(displayValue, 'f', 2);
    //return QString::number(this->expansionOrigin.getAt(1)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
}

QString CoordinateSystem::getDisplayExpansionOriginZ() const
{
    double displayValue = this->expansionOrigin.getAt(2);
    this->convertMetricValue(displayValue);
    return QString::number(displayValue, 'f', 2);
    //return QString::number(this->expansionOrigin.getAt(2)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
}

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
        foreach(FeatureWrapper *geom, this->nominals){
            if(geom != NULL && geom->getGeometry() != NULL){
                QDomElement nominal = xmlDoc.createElement("geometry");
                nominal.setAttribute("ref", geom->getGeometry()->getId());
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
*/

/*!
 * \brief CoordinateSystem::CoordinateSystem
 * \param parent
 */
CoordinateSystem::CoordinateSystem(QObject *parent) : Feature(parent), isActiveCoordinateSystem(false), isStationSystem(false){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCoordinateSystem(this);
    }

}

/*!
 * \brief CoordinateSystem::CoordinateSystem
 * \param station
 * \param parent
 */
CoordinateSystem::CoordinateSystem(const QPointer<Station> &station, QObject *parent) : Feature(parent), isActiveCoordinateSystem(false){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCoordinateSystem(this);
    }

    //set up corresponding station
    if(!station.isNull()){
        this->isStationSystem = true;
        this->station = station;
    }

}

/*!
 * \brief CoordinateSystem::CoordinateSystem
 * \param copy
 * \param parent
 */
CoordinateSystem::CoordinateSystem(const CoordinateSystem &copy, QObject *parent) : Feature(parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCoordinateSystem(this);
    }

    //copy attributes
    this->origin = copy.origin;
    this->xAxis = copy.xAxis;
    this->yAxis = copy.yAxis;
    this->zAxis = copy.zAxis;
    this->isStationSystem = copy.isStationSystem;
    this->station = copy.station;
    this->isActiveCoordinateSystem = copy.isActiveCoordinateSystem;
    this->expansionOrigin = copy.expansionOrigin;

}

/*!
 * \brief CoordinateSystem::operator =
 * \param copy
 * \return
 */
CoordinateSystem &CoordinateSystem::operator=(const CoordinateSystem &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCoordinateSystem(this);
    }

    //copy attributes
    this->origin = copy.origin;
    this->xAxis = copy.xAxis;
    this->yAxis = copy.yAxis;
    this->zAxis = copy.zAxis;
    this->isStationSystem = copy.isStationSystem;
    this->station = copy.station;
    this->isActiveCoordinateSystem = copy.isActiveCoordinateSystem;
    this->expansionOrigin = copy.expansionOrigin;

    return *this;

}

/*!
 * \brief CoordinateSystem::~CoordinateSystem
 */
CoordinateSystem::~CoordinateSystem(){

}

/*!
 * \brief CoordinateSystem::getIsActiveCoordinateSystem
 * \return
 */
const bool &CoordinateSystem::getIsActiveCoordinateSystem() const{
    return this->isActiveCoordinateSystem;
}

/*!
 * \brief CoordinateSystem::setActiveCoordinateSystemState
 * \param isActiveCoordinateSystem
 */
void CoordinateSystem::setActiveCoordinateSystemState(const bool &isActiveCoordinateSystem){
    if(this->isActiveCoordinateSystem != isActiveCoordinateSystem){
        this->isActiveCoordinateSystem = isActiveCoordinateSystem;
        emit this->activeCoordinateSystemChanged(this->id);
    }
}

/*!
 * \brief CoordinateSystem::getIsStationSystem
 * \return
 */
const bool &CoordinateSystem::getIsStationSystem() const{
    return this->isStationSystem;
}

/*!
 * \brief CoordinateSystem::getStation
 * \return
 */
const QPointer<Station> &CoordinateSystem::getStation() const{
    return this->station;
}

/*!
 * \brief CoordinateSystem::getOrigin
 * \return
 */
const Position &CoordinateSystem::getOrigin() const{
    return this->origin;
}

/*!
 * \brief CoordinateSystem::getXAxis
 * \return
 */
const Direction &CoordinateSystem::getXAxis() const{
    return this->xAxis;
}

/*!
 * \brief CoordinateSystem::getYAxis
 * \return
 */
const Direction &CoordinateSystem::getYAxis() const{
    return this->yAxis;
}

/*!
 * \brief CoordinateSystem::getZAxis
 * \return
 */
const Direction &CoordinateSystem::getZAxis() const{
    return this->zAxis;
}

/*!
 * \brief CoordinateSystem::setCoordinateSystem
 * \param origin
 * \param xAxis
 * \param yAxis
 * \param zAxis
 */
void CoordinateSystem::setCoordinateSystem(const Position &origin, const Direction &xAxis, const Direction &yAxis, const Direction &zAxis){
    this->origin = origin;
    this->xAxis = xAxis;
    this->yAxis = yAxis;
    this->zAxis = zAxis;
}

/*!
 * \brief CoordinateSystem::getExpansionOrigin
 * \return
 */
const Position &CoordinateSystem::getExpansionOrigin() const{
    return this->expansionOrigin;
}

/*!
 * \brief CoordinateSystem::setExpansionOrigin
 * \param expansionOrigin
 */
void CoordinateSystem::setExpansionOrigin(const Position &expansionOrigin){
    this->expansionOrigin = expansionOrigin;
}

/*!
 * \brief CoordinateSystem::getObservations
 * \return
 */
const QList<QPointer<Observation> > &CoordinateSystem::getObservations() const{
    return this->observationsList;
}

/*!
 * \brief CoordinateSystem::getObservation
 * \param observationId
 * \return
 */
QPointer<Observation> CoordinateSystem::getObservation(const int &observationId) const{
    return this->observationsMap.value(observationId, QPointer<Observation>());
}

/*!
 * \brief CoordinateSystem::addObservation
 * \param observation
 * \return
 */
bool CoordinateSystem::addObservation(const QPointer<Observation> &observation){

    if(!observation.isNull()){

        this->observationsList.append(observation);
        this->observationsMap.insert(observation->getId(), observation);

        emit this->observationsChanged(this->id, observation->getId());

        return true;

    }

    return false;

}

/*!
 * \brief CoordinateSystem::getTransformationParameters
 * \return
 */
const QList<QPointer<TrafoParam> > &CoordinateSystem::getTransformationParameters() const{
    return this->trafoParams;
}

/*!
 * \brief CoordinateSystem::getTransformationParameters
 * \param to
 * \return
 */
const QList<QPointer<TrafoParam> > CoordinateSystem::getTransformationParameters(const QPointer<CoordinateSystem> &to) const{

    QList< QPointer<TrafoParam> > result;

    //check the destination system
    if(to.isNull()){
        return result;
    }

    //run through all trafo params and select those whose destination system quals "to"
    foreach(const QPointer<TrafoParam> &trafo, this->trafoParams){
        if(!trafo.isNull() && !trafo->getDestinationSystem().isNull() && !trafo->getStartSystem().isNull()){
            if(trafo->getDestinationSystem()->getId() == to->getId() || trafo->getStartSystem()->getId() == to->getId()){
               result.append(trafo);
            }
        }
    }

    return result;

}

/*!
 * \brief CoordinateSystem::addTransformationParameter
 * \param trafoParam
 */
void CoordinateSystem::addTransformationParameter(const QPointer<TrafoParam> &trafoParam){
    if(!trafoParam.isNull()){
        this->trafoParams.append(trafoParam);
        emit this->transformationParametersChanged(this->id);
    }
}

/*!
 * \brief CoordinateSystem::removeTransformationParameter
 * \param trafoParam
 */
void CoordinateSystem::removeTransformationParameter(const QPointer<TrafoParam> &trafoParam){
    if(!trafoParam.isNull()){
        this->trafoParams.removeOne(trafoParam);
        emit this->transformationParametersChanged(this->id);
    }
}

/*!
 * \brief CoordinateSystem::getNominals
 * \return
 */
const QList<QPointer<FeatureWrapper> > &CoordinateSystem::getNominals() const{
    return this->nominalsList;
}

/*!
 * \brief CoordinateSystem::addNominal
 * \param nominal
 */
void CoordinateSystem::addNominal(const QPointer<FeatureWrapper> &nominal){
    if(!nominal.isNull() && !nominal->getGeometry().isNull()){
        nominal->getGeometry()->setNominalSystem(this);
        this->nominalsList.append(nominal);
        this->nominalsMap.insert(nominal->getGeometry()->getId(), nominal);
        emit this->nominalsChanged(this->id);
    }
}

/*!
 * \brief CoordinateSystem::addNominals
 * \param nominals
 */
void CoordinateSystem::addNominals(const QList<QPointer<FeatureWrapper> > &nominals){

    if(nominals.size() > 0){

        foreach(const QPointer<FeatureWrapper> &geom, nominals){

            if(geom.isNull() || geom->getGeometry().isNull()){
                continue;
            }

            geom->getGeometry()->setNominalSystem(this);
            this->nominalsList.append(geom);
            this->nominalsMap.insert(geom->getGeometry()->getId(), geom);

        }

        emit this->nominalsChanged(this->id);

    }

}

/*!
 * \brief CoordinateSystem::removeNominal
 * \param nominal
 */
void CoordinateSystem::removeNominal(const QPointer<FeatureWrapper> &nominal){
    if(!nominal.isNull() && !nominal->getGeometry().isNull()){
        this->nominalsMap.remove(nominal->getGeometry()->getId());
        this->nominalsList.removeOne(nominal);
        emit this->nominalsChanged(this->id);
    }
}

/*!
 * \brief CoordinateSystem::removeNominal
 * \param featureId
 */
void CoordinateSystem::removeNominal(const int &featureId){
    QPointer<FeatureWrapper> nominal = this->nominalsMap.value(featureId, QPointer<FeatureWrapper>());
    this->removeNominal(nominal);
}

/*!
 * \brief CoordinateSystem::recalc
 */
void CoordinateSystem::recalc(){
    Feature::recalc();
}

/*!
 * \brief CoordinateSystem::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement CoordinateSystem::toOpenIndyXML(QDomDocument &xmlDoc){

    QDomElement coordinateSystem = Feature::toOpenIndyXML(xmlDoc);
/*
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
        foreach(FeatureWrapper *geom, this->nominals){
            if(geom != NULL && geom->getGeometry() != NULL){
                QDomElement nominal = xmlDoc.createElement("geometry");
                nominal.setAttribute("ref", geom->getGeometry()->getId());
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
*/
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
/*
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
*/
    }

    return result;

}
