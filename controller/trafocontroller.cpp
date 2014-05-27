#include "trafocontroller.h"

QList<FeatureWrapper*>* TrafoController::features;
FeatureWrapper* TrafoController::activeFeature;
QList<CoordinateSystem*>* TrafoController::coordSys;
QList<Station*>* TrafoController::stations;
Station* TrafoController::activeStation;
CoordinateSystem* TrafoController::activeCoordinateSystem;
QMap<QString, int>* TrafoController::availableGroups;

TrafoController::TrafoController(QObject *parent) :
    QObject(parent)
{
}

/*!
 * \brief addObservation corrects and transforms new observations and adds them afterwards in the list of observations.
 * \param obs
 */
void TrafoController::addObservation(Observation *obs)
{
    if(obs->myReading->typeofReading == Configuration::ePolar || obs->myReading->typeofReading == Configuration::eCartesian){
        applyMovements(obs);
        transformNewObservations(obs);
        this->activeStation->coordSys->observations.append(obs);
    }
}

/*!
 * \brief applyMovements applys the valid movement transformation on the observation, if one exists.
 * \param obs
 */
void TrafoController::applyMovements(Observation *obs)
{
    //find movements for that observation
    QList<TrafoParam*> movements;
    for(int i=0;i<features->size();i++){
        if(features->at(i)->getTrafoParam() != NULL && features->at(i)->getTrafoParam()->isMovement){
            if(features->at(i)->getTrafoParam()->to == obs->myStation->coordSys){
                movements.append(features->at(i)->getTrafoParam());
            }
        }
    }
    if(movements.size()>0){
        //sort list on valid time attributes
        QMap<QDateTime,TrafoParam*> map;
        for(int k=0;k<movements.size();k++){
            map.insert(movements.at(k)->validTime,movements.at(k));
        }
        movements = map.values();

        //apply movement
        for(int i=0;i<movements.size();i++){
            if(obs->myReading->measuredAt<movements.at(i)->validTime && i != 0){
                OiMat t= movements.at(i)->homogenMatrix;
                obs->myXyz = t * obs->myOriginalXyz;
                obs->myStatistic.qxx = t * obs->myOriginalStatistic.qxx;
            }
        }
    }else{
        obs->myXyz = obs->myOriginalXyz;
        obs->myStatistic = obs->myOriginalStatistic;
    }
    obs->isValid = true;
}

/*!
 * \brief transformNewObservations transforms new observations directly into the active coordinate system if possible.
 * \param obs
 */
void TrafoController::transformNewObservations(Observation *obs)
{
    if(obs->myStation->coordSys != activeCoordinateSystem){
        TrafoParam *tp = findTrafoParam(obs->myStation->coordSys,activeCoordinateSystem);
        if(tp != NULL){
            OiMat t;
            if(tp->to == activeCoordinateSystem){
                t = tp->homogenMatrix;
            }else{
                t = tp->homogenMatrix.inv();
            }
            obs->myXyz = t * obs->myXyz;
            obs->myStatistic.qxx = t * obs->myStatistic.qxx;
        }
    }
}

/*!
 * \brief initPointers gets actual lists
 * \param dlh
 */
void TrafoController::initPointers(DataListHandler dlh)
{
    TrafoController::features = dlh.features;
    TrafoController::activeFeature = dlh.activeFeature;
    TrafoController::coordSys = dlh.coordSys;
    TrafoController::stations = dlh.stations;
    TrafoController::activeStation = dlh.activeStation;
    TrafoController::activeCoordinateSystem = dlh.activeCoordinateSystem;
    TrafoController::availableGroups = dlh.availableGroups;
}

/*!
 * \brief transformObservations transforms the observations of the specified coordinatesystem.
 * \param from
 * \return
 */
bool TrafoController::transformObservations(CoordinateSystem *from)
{
    if(from != NULL){
        //first apply movements
        foreach (Observation *obs, from->observations) {
            applyMovements(obs);
        }
        if(from == activeCoordinateSystem){
            return true;
        }
        //then transform
        TrafoParam *tp = findTrafoParam(from,activeCoordinateSystem);
        if(tp != NULL){
            OiMat t;
            if(tp->from == from){
                t = tp->homogenMatrix;
            }else{
                t = tp->homogenMatrix.inv();
            }
            foreach (Observation *obs, from->observations) {
                obs->myXyz = t*obs->myXyz;
                obs->myStatistic.qxx = t*obs->myStatistic.qxx;
                obs->isValid = true;
            }
            return true;
        }else{ //no trafo params available
            foreach(Observation *obs, from->observations){
                obs->isValid = false;
            }
            return false;
        }
    }
    return false;
}

/*!
 * \brief setObservationState
 * \param cs
 * \param valid
 */
void TrafoController::setObservationState(CoordinateSystem *cs, bool valid)
{
    foreach(Observation *obs, cs->observations){
        if(valid == true){
            applyMovements(obs);
            //obs->myXyz = obs->myOriginalXyz;
        }
        obs->isValid = valid;
    }
}

/*!
 * \brief MovementsChanged need to recalc movements and correct all observations after a time of a movement or the state changed, a movement was added or a movement was deleted.
 */
void TrafoController::MovementsChanged()
{
    recalcObservations();
    recalcAllMovements();
    recalcObservations();
}

/*!
 * \brief :recalcAllMovements recalculates all movement transformations.
 */
void TrafoController::recalcAllMovements()
{
    for(int i=0;i<features->size();i++){
        if(features->at(i)->getTrafoParam() != NULL){
            features->at(i)->getTrafoParam()->recalc();
        }
    }
}

/*!
 * \brief recalcObservations with the new movement transformations
 */
void TrafoController::recalcObservations()
{
    for(int i=0;i<stations->size();i++){
        foreach (Observation *obs, stations->at(i)->coordSys->observations) {
            applyMovements(obs);
        }
    }for(int i=0;i<coordSys->size();i++){
        foreach (Observation *obs, coordSys->at(i)->observations) {
            applyMovements(obs);
        }
    }
}

/*!
 * \brief findTrafoParam for the specified coordinate system.
 * \param from
 * \param to
 * \return
 */
TrafoParam *TrafoController::findTrafoParam(CoordinateSystem *from, CoordinateSystem *to)
{
    foreach(TrafoParam *tp, from->trafoParams){
        if(tp->to != NULL && tp->from != NULL){
            if(tp->to == to || tp->from == to){
                if(tp->use){
                    return tp;
                }
            }
        }
    }
    return NULL;
}
