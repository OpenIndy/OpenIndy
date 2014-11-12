#include "geometry.h"

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
            this->myNominalCoordSys->removeNominal(this);
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

/*!
 * \brief Geometry::getIsCommon
 * \return
 */
bool Geometry::getIsCommon() const{
    return this->isCommon;
}

/*!
 * \brief Geometry::setCommonState
 * \param isCommon
 */
void Geometry::setCommonState(bool isCommon){
    if(this->isCommon != isCommon){
        this->isCommon = isCommon;
        emit this->geomIsCommonChanged(this->id);
    }
}

/*!
 * \brief Geometry::getIsNominal
 * \return
 */
bool Geometry::getIsNominal() const{
    return this->isNominal;
}

/*!
 * \brief Geometry::getMyNominals
 * \return
 */
QList<Geometry *> Geometry::getMyNominals() const{
    return this->nominals;
}

/*!
 * \brief Geometry::addNominal
 * \param myNominal
 * \return
 */
bool Geometry::addNominal(Geometry *myNominal){
    if(!this->isNominal && myNominal != NULL && myNominal->getIsNominal()){
        this->nominals.append(myNominal);
        emit this->geomMyNominalsChanged(this->id);
        return true;
    }
    return false;
}

/*!
 * \brief Geometry::removeNominal
 * \param myNominal
 * \return
 */
bool Geometry::removeNominal(Geometry *myNominal){
    if(!this->isNominal && myNominal != NULL && myNominal->getIsNominal()){
        for(unsigned int i = 0; i < this->nominals.size(); i++){
            if(this->nominals.at(i) != NULL && this->nominals.at(i)->getId() == myNominal->getId()){
                this->nominals.removeAt(i);
                emit this->geomMyNominalsChanged(this->id);
                return true;
            }
        }
    }
    return false;
}

/*!
 * \brief Geometry::getMyActual
 * \return
 */
Geometry *Geometry::getMyActual() const{
    return this->myActual;
}

/*!
 * \brief Geometry::setMyActual
 * \param myActual
 * \return
 */
bool Geometry::setMyActual(Geometry *myActual){
    if(this->isNominal && myActual != NULL && !myActual->getIsNominal()){
        if(this->myActual != NULL && this->myActual->getId() == myActual->getId()){
            return false;
        }else{
            this->myActual = myActual;
            emit this->geomMyActualChanged(this->id);
            return true;
        }
    }else if(myActual == NULL && this->myActual != NULL){
        this->myActual = NULL;
        emit this->geomMyActualChanged(this->id);
        return true;
    }
    return false;
}

/*!
 * \brief Geometry::getObservations
 * \return
 */
QList<Observation *> Geometry::getObservations() const{
    return this->myObservations;
}

/*!
 * \brief Geometry::addObservation
 * \param obs
 * \return
 */
bool Geometry::addObservation(Observation *obs){
    if(!this->isNominal && obs != NULL){
        this->myObservations.append(obs);

        //add reading to geom
        switch (obs->myReading->typeofReading) {
        case Configuration::ePolar:
            this->insertReadingType(obs->myReading->typeofReading,Configuration::sPolar);
            break;
        case Configuration::eCartesian:
            this->insertReadingType(obs->myReading->typeofReading,Configuration::sCartesian);
            break;
        case Configuration::eDirection:
            this->insertReadingType(obs->myReading->typeofReading,Configuration::sDirection);
            break;
        case Configuration::eDistance:
            this->insertReadingType(obs->myReading->typeofReading,Configuration::sDistance);
            break;
        case Configuration::eLevel:
            this->insertReadingType(obs->myReading->typeofReading,Configuration::sLevel);
            break;
        case Configuration::eTemperatur:
            this->insertReadingType(obs->myReading->typeofReading,Configuration::sTemperatur);
            break;
        case Configuration::eUndefined:
            this->insertReadingType(obs->myReading->typeofReading,"undefined");
            break;
        default:
            break;
        }

        emit this->geomMyObservationsChanged(this->id);
        return true;
    }
    return false;
}

/*!
 * \brief Geometry::removeObservation
 * \param obs
 * \return
 */
bool Geometry::removeObservation(Observation *obs){
    if(!this->isNominal && obs != NULL){
        for(int i = 0; i < this->myObservations.size(); i++){
            if(this->myObservations.at(i)->getId() == obs->getId()){

                this->myObservations.removeAt(i);
                this->removeReadingType(obs->myReading->typeofReading);

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

/*!
 * \brief Geometry::getNominalSystem
 * \return
 */
CoordinateSystem *Geometry::getNominalSystem() const{
    return this->myNominalCoordSys;
}

/*!
 * \brief Geometry::setNominalSystem
 * \param nomSys
 * \return
 */
bool Geometry::setNominalSystem(CoordinateSystem *nomSys){
    if(this->isNominal && nomSys != NULL){
        this->myNominalCoordSys = nomSys;
        emit this->geomMyNominalSystemChanged(this->id);
        return true;
    }
    return false;
}

/*!
 * \brief getDisplayObs displays the number of current valid observations and total observations in the gui.
 * Display style x/y where x is current valid and y is total observation count.
 * \return
 */
QString Geometry::getDisplayObs() const
{
    int validObs = 0;
    int totalObs = this->myObservations.size();

    Function *fitFunc = NULL;

    if(this->functionList.at(0)->getMetaData()->iid.compare(OiMetaData::iid_FitFunction) == 0){

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
}

/*!
 * \brief insertReadingType
 * \param readingType
 * \param displayName
 */
void Geometry::insertReadingType(Configuration::ReadingTypes readingType, QString displayName){

    //check if enum value is valid. if not return function, else go on with assignment
    switch (readingType) {
    case Configuration::ePolar:
        break;
    case Configuration::eCartesian:
        break;
    case Configuration::eDistance:
        break;
    case Configuration::eDirection:
        break;
    case Configuration::eTemperatur:
        break;
    case Configuration::eLevel:
        break;
    case Configuration::eUndefined:
        break;
    default:
        //return function if enum value is not valid
        return;
        break;
    }

    QMap<Configuration::ReadingTypes,QString>::const_iterator i = usedReadingTypes.find(readingType);
    //add reading type to list if it is not in there yet
    if (i.key() != readingType) {
        usedReadingTypes.insert(readingType,displayName);
    }

    emit this->geomUsedReadingTypesChanged(this->id);
}

/*!
 * \brief Geometry::getMeasurementConfig
 * \return
 */
MeasurementConfig Geometry::getMeasurementConfig() const{
    return this->mConfig;
}

/*!
 * \brief Geometry::setMeasurementConfig
 * \param myConfig
 */
void Geometry::setMeasurementConfig(MeasurementConfig myConfig){
    this->mConfig = myConfig;
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

/*!
 * \brief Geometry::getUsedReadingTypes
 * \return
 */
QMap<Configuration::ReadingTypes, QString> Geometry::getUsedReadingTypes() const{
    return this->usedReadingTypes;
}

/*!
 * \brief removeReadingType removes the reading type if there is no more observation with this reading type in the geometry list.
 */
void Geometry::removeReadingType(Configuration::ReadingTypes rType)
{
    bool rTypeExists = false;

    //check if the geometry still has an observation with that reading type. else delete from map.
    for(int i=0; i<this->getObservations().size();i++){
        if(this->getObservations().at(i)->myReading->typeofReading == rType){
            rTypeExists = true;
        }
    }

    if(!rTypeExists){
        this->usedReadingTypes.remove(rType);
    }
}

/*!
 * \brief Geometry::getStatistic
 * \return
 */
Statistic Geometry::getStatistic() const{
    return this->myStatistic;
}

/*!
 * \brief Geometry::setStatistic
 * \param myStatistic
 */
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

/*!
 * \brief Geometry::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
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
        mConfig.setAttribute("name", this->mConfig.name);
        geometry.appendChild(mConfig);
    }

    return geometry;

}

/*!
 * \brief Geometry::fromOpenIndyXML
 * \param xmlDoc
 * \return
 */
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
