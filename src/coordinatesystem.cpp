#include "coordinatesystem.h"

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

/*!
 * \brief CoordinateSystem::getObservations
 * \return
 */
const QList<QPointer<Observation> > &CoordinateSystem::getObservations() const{
    return this->observations;
}

/*!
 * \brief CoordinateSystem::getObservation
 * \param observationId
 * \return
 */
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

/*!
 * \brief CoordinateSystem::addObservation
 * \param observation
 * \return
 */
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

/*!
 * \brief CoordinateSystem::getTransformationParameter
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

/*!
 * \brief CoordinateSystem::addTransformationParameter
 * \param trafoParam
 * \return
 */
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

/*!
 * \brief CoordinateSystem::removeTransformationParameter
 * \param trafoParam
 * \return
 */
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

/*!
 * \brief CoordinateSystem::getNominals
 * \return
 */
const QList<QPointer<FeatureWrapper> > &CoordinateSystem::getNominals() const{
    return this->nominals;
}

/*bool CoordinateSystem::addNominal(Geometry * const nominal)
{
    this->nominals.append(nominal);
    return true;
}*/

/*!
 * \brief CoordinateSystem::addNominal
 * \param nominal
 * \return
 */
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

/*!
 * \brief CoordinateSystem::addNominals
 * \param nominals
 * \return
 */
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

/*!
 * \brief CoordinateSystem::removeNominal
 * \param nominal
 * \return
 */
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

/*!
 * \brief CoordinateSystem::removeNominal
 * \param featureId
 * \return
 */
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
void CoordinateSystem::setActiveCoordinateSystemState(const bool &isActiveCoordinateSystem){
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
