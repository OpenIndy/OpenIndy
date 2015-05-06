#include "geometry.h"

#include "oijob.h"
/*

#include "coordinatesystem.h"
#include "observation.h"
#include "station.h"
#include "function.h"
#include "oimetadata.h"

Geometry::Geometry(bool isNominal, QObject *parent) : Feature(parent),
    myActual(NULL), myNominalCoordSys(NULL), isNominal(isNominal)
{
}

Geometry::~Geometry(){

    if(this->isNominal){
        //delete this geometry from the nominal list of its actual
        if(this->myActual != NULL){
            int index = -1;
            int k = 0;
            foreach(Geometry *checkNominal, this->myActual->nominals){
                if(checkNominal != NULL && checkNominal->id == this->id){
                    index = k;
                    break;
                }
                k++;
            }
            if(index >= 0){
                this->myActual->nominals.removeAt(index);
            }
        }

        //delete this geometry from list of nominals in myNominalCoordSys
        if(this->myNominalCoordSys != NULL){
            this->myNominalCoordSys->removeNominal(this->id);
        }
    }else{
        //delete observations that only belong to this geometry
        for(int i = 0; i < this->myObservations.size(); i++){
            Observation *myObs = this->myObservations.at(i);
            if(myObs->myTargetGeometries.size() == 1){
                Station *myStation = myObs->myStation;
                if(myStation != NULL && myStation->coordSys != NULL){
                    //myStation->coordSys->getObservations().removeOne(myObs);
                }
            }
            delete myObs;
        }

        //delete property myActual from all nominal geometries which belong to this actual geometry
        foreach(Geometry *checkNominal, this->nominals){
            if(checkNominal != NULL){
                checkNominal->myActual = NULL;
            }
        }
    }

}

bool Geometry::getIsCommon() const{
    return this->isCommon;
}

void Geometry::setCommonState(bool isCommon){
    if(this->isCommon != isCommon){
        this->isCommon = isCommon;
        emit this->geomIsCommonChanged(this->id);
    }
}

bool Geometry::getIsNominal() const{
    return this->isNominal;
}

QString Geometry::getDisplayIsNominal() const
{
    return QString(this->isNominal?"nominal":"actual");
}

const QList<QPointer<Geometry> > &Geometry::getMyNominals() const{
    return this->nominals;
}

bool Geometry::addNominal(const QPointer<Geometry> &myNominal){
    if(!this->isNominal && !myNominal.isNull() && myNominal->getIsNominal()){
        this->nominals.append(myNominal);
        emit this->geomMyNominalsChanged(this->id);
        return true;
    }
    return false;
}

bool Geometry::removeNominal(const QPointer<Geometry> &myNominal){
    if(!this->isNominal && !myNominal.isNull() && myNominal->getIsNominal()){
        for(unsigned int i = 0; i < this->nominals.size(); i++){
            if(!this->nominals.at(i).isNull() && this->nominals.at(i)->getId() == myNominal->getId()){
                this->nominals.removeAt(i);
                emit this->geomMyNominalsChanged(this->id);
                return true;
            }
        }
    }
    return false;
}

const QPointer<Geometry> &Geometry::getMyActual() const{
    return this->myActual;
}

bool Geometry::setMyActual(const QPointer<Geometry> &myActual){
    if(this->isNominal && !myActual.isNull() && !myActual->getIsNominal()){
        if(!this->myActual.isNull() && this->myActual->getId() == myActual->getId()){
            return false;
        }else{
            this->myActual = myActual;
            emit this->geomMyActualChanged(this->id);
            return true;
        }
    }else if(!myActual.isNull() && !this->myActual.isNull()){
        this->myActual = QPointer<Geometry>();
        emit this->geomMyActualChanged(this->id);
        return true;
    }
    return false;
}

QList<Observation *> Geometry::getObservations() const{
    return this->myObservations;
}

bool Geometry::addObservation(Observation *obs){

    if(!this->isNominal && obs != NULL){

        //check if the observations has already been added
        foreach(Observation *o, this->myObservations){
            if(o->getId() == obs->getId()){
                return false;
            }
        }

        this->myObservations.append(obs);

        //add reading to geom
        //this->insertReadingType(obs->myReading->typeOfReading, getReadingTypeName(obs->myReading->typeOfReading));

        emit this->geomMyObservationsChanged(this->id);
        return true;
    }
    return false;

}

bool Geometry::removeObservation(Observation *obs){
    if(!this->isNominal && obs != NULL){
        for(int i = 0; i < this->myObservations.size(); i++){
            if(this->myObservations.at(i)->getId() == obs->getId()){

                this->myObservations.removeAt(i);
                this->removeReadingType(obs->getReading()->getTypeOfReading());

                foreach(Function *myFunc, this->getFunctions()){
                    if(myFunc != NULL){
                        myFunc->removeObservation(this->id);
                    }
                }

                emit this->geomMyObservationsChanged(this->id);
                return true;

            }
        }
        return true;
    }
    return false;
}

const QPointer<CoordinateSystem> &Geometry::getNominalSystem() const{
    return this->myNominalCoordSys;
}

bool Geometry::setNominalSystem(const QPointer<CoordinateSystem> &nomSys){
    if(this->isNominal && nomSys != NULL){
        this->myNominalCoordSys = nomSys;
        emit this->geomMyNominalSystemChanged(this->id);
        return true;
    }
    return false;
}

QString Geometry::getDisplayObs() const
{
    int validObs = 0;
    int totalObs = this->myObservations.size();

    Function *fitFunc = NULL;

    if(this->functionList.size() > 0 && this->functionList.at(0)->getMetaData()->iid.compare(OiMetaData::iid_FitFunction) == 0){

        fitFunc = this->functionList.at(0);

        if(fitFunc == NULL){
            return QString("-/"+QString::number(totalObs));
        }
        for(int i=0; i<fitFunc->getObservations().size();i++){
            if(fitFunc->getObservations().at(i)->getUseState()){
                validObs += 1;
            }
        }
        return QString(QString::number(validObs)+"/"+QString::number(totalObs));
    }
    return QString("-/"+QString::number(totalObs));

    return "";
}

void Geometry::insertReadingType(ReadingTypes readingType, QString displayName){

    //check if enum value is valid. if not return function, else go on with assignment
    switch (readingType) {
    case ePolarReading:
        break;
    case eCartesianReading:
        break;
    case eDistanceReading:
        break;
    case eDirectionReading:
        break;
    case eTemperatureReading:
        break;
    case eLevelReading:
        break;
    case eUndefinedReading:
        break;
    default:
        //return function if enum value is not valid
        return;
        break;
    }

    QMap<ReadingTypes,QString>::const_iterator i = usedReadingTypes.find(readingType);
    //add reading type to list if it is not in there yet
    if (i.key() != readingType) {
        usedReadingTypes.insert(readingType,displayName);
    }

    emit this->geomUsedReadingTypesChanged(this->id);
}

MeasurementConfig Geometry::getMeasurementConfig() const{
    return this->activeMeasurementConfig;
}

void Geometry::setMeasurementConfig(MeasurementConfig myConfig){
    this->activeMeasurementConfig = myConfig;
    emit this->geomMyMeasurementConfigChanged(this->id);
}

OiVec Geometry::getXYZ() const{
    return OiVec();
}

OiVec Geometry::getIJK() const{
    return OiVec();
}

double Geometry::getRadius() const
{
    return 0.0;
}

double Geometry::getScalar() const
{
    return 0.0;
}

QMap<ReadingTypes, QString> Geometry::getUsedReadingTypes() const{
    return this->usedReadingTypes;
}

void Geometry::removeReadingType(ReadingTypes rType)
{
    bool rTypeExists = false;

    //check if the geometry still has an observation with that reading type. else delete from map.
    for(int i=0; i<this->getObservations().size();i++){
        if(this->getObservations().at(i)->getReading()->getTypeOfReading() == rType){
            rTypeExists = true;
        }
    }

    if(!rTypeExists){
        this->usedReadingTypes.remove(rType);
    }
}

Statistic Geometry::getStatistic() const{
    return this->myStatistic;
}

void Geometry::setStatistic(Statistic myStatistic){
    this->myStatistic = myStatistic;
    emit this->geomMyStatisticChanged(this->id);
}

SimulationData& Geometry::getSimulationData()
{
    return this->mySimulationData;
}

void Geometry::setSimulationData(SimulationData s)
{
    this->mySimulationData = s;
}

QDomElement Geometry::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement geometry = Feature::toOpenIndyXML(xmlDoc);

    if(geometry.isNull()){
        return geometry;
    }

    geometry.setTagName("geometry");

    //set geometry attributes
    geometry.setAttribute("nominal", this->getIsNominal());
    geometry.setAttribute("common", this->getIsCommon());

    //add coordinates
    QDomElement coordinates = xmlDoc.createElement("coordinates");
    OiVec xyz = this->getXYZ();
    if(xyz.getSize() >= 3 && (this->getIsSolved() || this->getIsNominal())){
        coordinates.setAttribute("x", xyz.getAt(0));
        coordinates.setAttribute("y", xyz.getAt(1));
        coordinates.setAttribute("z", xyz.getAt(2));
    }else{
        coordinates.setAttribute("x", 0.0);
        coordinates.setAttribute("y", 0.0);
        coordinates.setAttribute("z", 0.0);
    }
    geometry.appendChild(coordinates);

    //add standard deviation
    QDomElement stdv = xmlDoc.createElement("standardDeviation");
    if(this->getIsSolved()){
        stdv.setAttribute("value", this->myStatistic.stdev);
    }else{
        stdv.setAttribute("value", 0.0);
    }
    geometry.appendChild(stdv);

    //add observations
    if(!this->getIsNominal() && this->myObservations.size() > 0){
        QDomElement observations = xmlDoc.createElement("observations");
        foreach(Observation *obs, this->myObservations){
            QDomElement observation = xmlDoc.createElement("observation");
            observation.setAttribute("ref", obs->getId());
            observations.appendChild(observation);
        }
        geometry.appendChild(observations);
    }

    //add nominal system
    if(this->getIsNominal() && this->myNominalCoordSys != NULL){
        QDomElement nominalSystem = xmlDoc.createElement("nominalCoordinateSystem");
        nominalSystem.setAttribute("ref", this->myNominalCoordSys->getId());
        geometry.appendChild(nominalSystem);
    }

    //add corresponding actual geometry
    if(this->getIsNominal() && this->myActual != NULL){
        QDomElement actual = xmlDoc.createElement("actual");
        actual.setAttribute("ref", this->myActual->getId());
        geometry.appendChild(actual);
    }

    //add nominals
    if(!this->getIsNominal() && this->nominals.size() >= 0){
        QDomElement nominals = xmlDoc.createElement("nominalGeometries");
        foreach(Geometry *geom, this->nominals){
            if(geom != NULL){
                QDomElement nominal = xmlDoc.createElement("geometry");
                nominal.setAttribute("ref", geom->getId());
                nominals.appendChild(nominal);
            }
        }
        geometry.appendChild(nominals);
    }

    //add measurement config
    if(!this->getIsNominal()){
        QDomElement mConfig = xmlDoc.createElement("measurementConfig");
        mConfig.setAttribute("name", this->activeMeasurementConfig.getName());
        geometry.appendChild(mConfig);
    }

    return geometry;

}

bool Geometry::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Feature::fromOpenIndyXML(xmlElem);

    if(result){

        //set geometry attributes
        if(!xmlElem.hasAttribute("nominal") || !xmlElem.hasAttribute("common")){
            return false;
        }
        this->isNominal = xmlElem.attribute("nominal").toInt();
        this->isCommon = xmlElem.attribute("common").toInt();

        //set standard deviation
        QDomElement stdv = xmlElem.firstChildElement("standardDeviation");
        if(stdv.isNull() || !stdv.hasAttribute("value")){
            return false;
        }
        this->myStatistic.stdev = stdv.attribute("value").toDouble();

    }

    return result;

}

void Geometry::resetSimulationData()
{
    SimulationData d;
    this->mySimulationData = d;

}
*/

#include "coordinatesystem.h"
#include "observation.h"
#include "station.h"
#include "function.h"

/*!
 * \brief Geometry::Geometry
 * \param isNominal
 * \param parent
 */
Geometry::Geometry(const bool &isNominal, QObject *parent) : Feature(parent), isNominal(isNominal), isCommon(false){

}

/*!
 * \brief Geometry::Geometry
 * \param copy
 * \param parent
 */
Geometry::Geometry(const Geometry &copy, QObject *parent) : Feature(copy, parent){

    //copy attributes
    this->isNominal = copy.isNominal;
    this->isCommon = copy.isCommon;
    this->statistic = copy.statistic;
    this->simulationData = copy.simulationData;
    this->activeMeasurementConfig = copy.activeMeasurementConfig;

    //copy nominals, actual, observations
    this->nominals = copy.nominals;
    this->actual = copy.actual;
    this->observations = copy.observations;
    this->nominalSystem = copy.nominalSystem;

}

/*!
 * \brief Geometry::operator =
 * \param copy
 * \return
 */
Geometry &Geometry::operator=(const Geometry &copy){

    //copy attributes
    this->isNominal = copy.isNominal;
    this->isCommon = copy.isCommon;
    this->statistic = copy.statistic;
    this->simulationData = copy.simulationData;
    this->activeMeasurementConfig = copy.activeMeasurementConfig;

    //copy nominals, actual, observations
    this->nominals = copy.nominals;
    this->actual = copy.actual;
    this->observations = copy.observations;
    this->nominalSystem = copy.nominalSystem;

    return *this;

}

/*!
 * \brief Geometry::~Geometry
 */
Geometry::~Geometry(){

    if(this->isNominal){

        //delete this geometry from the nominal list of its actual
        if(!this->actual.isNull()){
            this->actual->removeNominal(this);
        }

        //delete this geometry from list of nominals in nominal system
        if(!this->nominalSystem.isNull()){
            this->nominalSystem->removeNominal(this->selfFeature);
        }

    }else{

        //delete this geometry from target geometries list of the observations
        foreach(const QPointer<Observation> &observation, this->observations){
            if(!observation.isNull()){
                observation->removeTargetGeometry(this->id);
            }
        }

    }

}

/*!
 * \brief Geometry::getIsCommon
 * \return
 */
const bool &Geometry::getIsCommon() const{
    return this->isCommon;
}

/*!
 * \brief Geometry::setCommonState
 * \param isCommon
 */
void Geometry::setCommonState(const bool &isCommon){
    if(isCommon != this->isCommon){
        this->isCommon = isCommon;
        emit this->geomIsCommonChanged(this->id);
    }
}

/*!
 * \brief Geometry::getIsNominal
 * \return
 */
const bool &Geometry::getIsNominal() const{
    return this->isNominal;
}

/*!
 * \brief Geometry::getNominals
 * \return
 */
const QList<QPointer<Geometry> > &Geometry::getNominals() const{
    return this->nominals;
}

/*!
 * \brief Geometry::addNominal
 * \param nominal
 * \return
 */
bool Geometry::addNominal(const QPointer<Geometry> &nominal){

    if(!nominal.isNull() && nominal->getIsNominal() && !this->getIsNominal()){

        //compare feature types
        if(this->selfFeature.isNull() || nominal->selfFeature.isNull() || this->selfFeature->getFeatureTypeEnum() != nominal->selfFeature->getFeatureTypeEnum()){
            return false;
        }

        //check if the nominal is in the same job
        if(!this->job.isNull()){
            QPointer<FeatureWrapper> jobNominal = this->job->getFeatureById(nominal->getId());
            if(jobNominal.isNull() || jobNominal->getGeometry().isNull() || jobNominal->getGeometry() != nominal){
                return false;
            }
        }

        //check if this geometry already contains the nominal
        if(!this->nominals.contains(nominal)){
            this->nominals.append(nominal);
            nominal->setActual(this);
            emit this->geomNominalsChanged(this->id);
            return true;
        }

    }

    return false;

}

/*!
 * \brief Geometry::removeNominal
 * \param nominal
 * \return
 */
bool Geometry::removeNominal(const QPointer<Geometry> &nominal){

    //check if the nominal is valid
    if(nominal.isNull()){
        return false;
    }

    //remove nominal
    return this->nominals.removeOne(nominal);

}

/*!
 * \brief Geometry::getActual
 * \return
 */
const QPointer<Geometry> &Geometry::getActual() const{
    return this->actual;
}

/*!
 * \brief Geometry::setActual
 * \param actual
 * \return
 */
bool Geometry::setActual(const QPointer<Geometry> &actual){

    //check if this is a nominal
    if(this->isNominal){
        return false;
    }

    //check actual
    if(actual.isNull() || (!this->actual.isNull() && this->actual->getId() == actual->getId())){
        return false;
    }

    //compare feature types
    if(this->selfFeature.isNull() || actual->selfFeature.isNull() || this->selfFeature->getFeatureTypeEnum() != actual->selfFeature->getFeatureTypeEnum()){
        return false;
    }

    //check if actual is in the same job
    if(!this->job.isNull()){
        QPointer<FeatureWrapper> jobActual = this->job->getFeatureById(actual->getId());
        if(jobActual.isNull() || jobActual->getGeometry().isNull() || jobActual->getGeometry() != actual){
            return false;
        }
    }

    this->actual = actual;
    actual->addNominal(this);
    emit this->geomActualChanged(this->id);

    return true;

}

/*!
 * \brief Geometry::getObservations
 * \return
 */
const QList<QPointer<Observation> > &Geometry::getObservations() const{
    return this->observations;
}

/*!
 * \brief Geometry::addObservation
 * \param obs
 */
void Geometry::addObservation(const QPointer<Observation> &obs){

    if(!obs.isNull() && !this->isNominal){

        //check if obs is already included
        if(this->observations.contains(obs)){
            return;
        }

        //update target geometries
        obs->addTargetGeometry(this);

        this->observations.append(obs);

        this->isUpdated = false;

        emit this->geomObservationsChanged(this->id);

    }

}

/*!
 * \brief Geometry::removeObservation
 * \param obs
 */
void Geometry::removeObservation(const QPointer<Observation> &obs){

    //check if obs is valid
    if(obs.isNull()){
        return;
    }

    //remove observation
    if(this->observations.removeOne(obs)){

        //update target geometries
        obs->removeTargetGeometry(this);

        this->isUpdated = false;

        emit this->geomObservationsChanged(this->id);

    }

}

/*!
 * \brief Geometry::getNominalSystem
 * \return
 */
const QPointer<CoordinateSystem> &Geometry::getNominalSystem() const{
    return this->nominalSystem;
}

/*!
 * \brief Geometry::setNominalSystem
 * \param nomSys
 * \return
 */
bool Geometry::setNominalSystem(const QPointer<CoordinateSystem> &nomSys){

    if(!nomSys.isNull() && this->isNominal && this->nominalSystem != nomSys){

        //check if nomSys is in the same job
        if(!this->job.isNull()){
            QPointer<FeatureWrapper> jobSystem = this->job->getFeatureById(nomSys->getId());
            if(jobSystem.isNull() || jobSystem->getCoordinateSystem().isNull() || jobSystem->getCoordinateSystem() != nomSys){
                return false;
            }
        }

        this->nominalSystem = nomSys;

        nomSys->addNominal(this->selfFeature);

        emit this->geomNominalSystemChanged(this->id);

        return true;

    }

    return false;

}

/*!
 * \brief Geometry::getMeasurementConfig
 * \return
 */
const MeasurementConfig &Geometry::getMeasurementConfig() const{
    return this->activeMeasurementConfig;
}

/*!
 * \brief Geometry::setMeasurementConfig
 * \param myConfig
 */
void Geometry::setMeasurementConfig(const MeasurementConfig &myConfig){
    this->activeMeasurementConfig = myConfig;
    emit this->geomMeasurementConfigChanged(this->id);
}

/*!
 * \brief Geometry::getStatistic
 * \return
 */
const Statistic &Geometry::getStatistic() const{
    return this->statistic;
}

/*!
 * \brief Geometry::setStatistic
 * \param myStatistic
 */
void Geometry::setStatistic(const Statistic &myStatistic){
    this->statistic = myStatistic;
}

/*!
 * \brief Geometry::getSimulationData
 * \return
 */
const SimulationData &Geometry::getSimulationData(){
    return this->simulationData;
}

/*!
 * \brief Geometry::setSimulationData
 * \param s
 */
void Geometry::setSimulationData(const SimulationData &s){
    this->simulationData = s;
}

/*!
 * \brief Geometry::hasDirection
 * \return
 */
bool Geometry::hasDirection() const{
    return false;
}

/*!
 * \brief Geometry::hasPosition
 * \return
 */
bool Geometry::hasPosition() const{
    return false;
}

/*!
 * \brief Geometry::hasRadius
 * \return
 */
bool Geometry::hasRadius() const{
    return false;
}

/*!
 * \brief Geometry::getRadius
 * \return
 */
const Radius &Geometry::getRadius() const{
    return this->dummyRadius;
}

/*!
 * \brief Geometry::getDirection
 * \return
 */
const Direction &Geometry::getDirection() const{
    return this->dummyDirection;
}

/*!
 * \brief Geometry::getPosition
 * \return
 */
const Position &Geometry::getPosition() const{
    return this->dummyPosition;
}

/*!
 * \brief Geometry::recalc
 */
void Geometry::recalc(){

    Feature::recalc();

    //if the geometry is a nominal without functions set isSolved to true
    if(this->functionList.size() == 0 && this->isNominal){
        this->isSolved = true;
    }

    //reset statistic if not solved
    if(!this->isSolved){
        this->statistic.reset();
    }

}

/*!
 * \brief Geometry::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Geometry::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement geometry = Feature::toOpenIndyXML(xmlDoc);
/*
    if(geometry.isNull()){
        return geometry;
    }

    geometry.setTagName("geometry");

    //set geometry attributes
    geometry.setAttribute("nominal", this->getIsNominal());
    geometry.setAttribute("common", this->getIsCommon());

    //add coordinates
    QDomElement coordinates = xmlDoc.createElement("coordinates");
    OiVec xyz = this->getXYZ();
    if(xyz.getSize() >= 3 && (this->getIsSolved() || this->getIsNominal())){
        coordinates.setAttribute("x", xyz.getAt(0));
        coordinates.setAttribute("y", xyz.getAt(1));
        coordinates.setAttribute("z", xyz.getAt(2));
    }else{
        coordinates.setAttribute("x", 0.0);
        coordinates.setAttribute("y", 0.0);
        coordinates.setAttribute("z", 0.0);
    }
    geometry.appendChild(coordinates);

    //add standard deviation
    QDomElement stdv = xmlDoc.createElement("standardDeviation");
    if(this->getIsSolved()){
        stdv.setAttribute("value", this->myStatistic.stdev);
    }else{
        stdv.setAttribute("value", 0.0);
    }
    geometry.appendChild(stdv);

    //add observations
    if(!this->getIsNominal() && this->myObservations.size() > 0){
        QDomElement observations = xmlDoc.createElement("observations");
        foreach(Observation *obs, this->myObservations){
            QDomElement observation = xmlDoc.createElement("observation");
            observation.setAttribute("ref", obs->getId());
            observations.appendChild(observation);
        }
        geometry.appendChild(observations);
    }

    //add nominal system
    if(this->getIsNominal() && this->myNominalCoordSys != NULL){
        QDomElement nominalSystem = xmlDoc.createElement("nominalCoordinateSystem");
        nominalSystem.setAttribute("ref", this->myNominalCoordSys->getId());
        geometry.appendChild(nominalSystem);
    }

    //add corresponding actual geometry
    if(this->getIsNominal() && this->myActual != NULL){
        QDomElement actual = xmlDoc.createElement("actual");
        actual.setAttribute("ref", this->myActual->getId());
        geometry.appendChild(actual);
    }

    //add nominals
    if(!this->getIsNominal() && this->nominals.size() >= 0){
        QDomElement nominals = xmlDoc.createElement("nominalGeometries");
        foreach(Geometry *geom, this->nominals){
            if(geom != NULL){
                QDomElement nominal = xmlDoc.createElement("geometry");
                nominal.setAttribute("ref", geom->getId());
                nominals.appendChild(nominal);
            }
        }
        geometry.appendChild(nominals);
    }

    //add measurement config
    if(!this->getIsNominal()){
        QDomElement mConfig = xmlDoc.createElement("measurementConfig");
        mConfig.setAttribute("name", this->activeMeasurementConfig.getName());
        geometry.appendChild(mConfig);
    }
*/
    return geometry;

}

/*!
 * \brief Geometry::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Geometry::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Feature::fromOpenIndyXML(xmlElem);

    if(result){
/*
        //set geometry attributes
        if(!xmlElem.hasAttribute("nominal") || !xmlElem.hasAttribute("common")){
            return false;
        }
        this->isNominal = xmlElem.attribute("nominal").toInt();
        this->isCommon = xmlElem.attribute("common").toInt();

        //set standard deviation
        QDomElement stdv = xmlElem.firstChildElement("standardDeviation");
        if(stdv.isNull() || !stdv.hasAttribute("value")){
            return false;
        }
        this->myStatistic.stdev = stdv.attribute("value").toDouble();
*/
    }

    return result;

}

/*!
 * \brief Geometry::getDisplayStDev
 * \param type
 * \param digits
 * \return
 */
QString Geometry::getDisplayStDev(const UnitType &type, const int &digits) const{
    if(this->statistic.getIsValid()){
        return QString::number(convertToDefault(this->statistic.getStdev(), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Geometry::getDisplayMeasurementConfig
 * \return
 */
QString Geometry::getDisplayMeasurementConfig() const{
    return this->activeMeasurementConfig.getName();
}

/*!
 * \brief Geometry::getDisplayObservations
 * \return
 */
QString Geometry::getDisplayObservations() const{

    int validObs = 0;
    int totalObs = this->observations.size();

    //get the number of used observations in the first function of this geometry
    if(totalObs > 0 && this->functionList.size() > 0){

        //get and check function pointer
        QPointer<Function> firstFunction = this->functionList.at(0);
        if(firstFunction.isNull()){
            return QString("-/%1").arg(totalObs);
        }

        //get the number of used observations
        QMap<int, QList<InputElement> > inputElements = firstFunction->getInputElements();
        QList<int> keys = inputElements.keys();
        foreach(const int &key, keys){

            //check if input alements at key are observations
            if(inputElements[key].size() > 0 && inputElements[key][0].typeOfElement == eObservationElement){

                //run through all observations and check their used state
                foreach(const InputElement &element, inputElements[key]){
                    if(element.isUsed){
                        validObs++;
                    }
                }

            }

        }

        return QString("%1/%2").arg(validObs).arg(totalObs);

    }

    return QString("-/%1").arg(totalObs);

}
