#include "trafocontroller.h"

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
}

/*!
 * \brief applyMovements applys the valid movement transformation on the observation, if one exists.
 * \param obs
 */
/*void TrafoController::applyMovements(Observation *obs)
{
    //find movements for that observation
    QList<TrafoParam*> movements;
    movements = this->findMovements(obs);

    if(movements.size()>0){

        //apply movement
        /*find the last movement that has a smaller timestamp than the observation and apply it.
         *if there is no movement with a a smaller timestamp, there is nothing to apply to the observation
        */
/*        if(movements.size() == 1){
            if(movements.at(0)->getValidTime() < obs->myReading->measuredAt){
                OiMat t= movements.at(0)->getHomogenMatrix();
                obs->myXyz = t * obs->myXyz;
                obs->myStatistic.qxx = t * obs->myStatistic.qxx;
            }
        }else{
            for(int i=0;i<movements.size();i++){
                if(movements.at(i)->getValidTime() < obs->myReading->measuredAt){
                    if((i+1)<movements.size() && movements.at(i+1)->getValidTime() > obs->myReading->measuredAt){
                        OiMat t= movements.at(i)->getHomogenMatrix();
                        obs->myXyz = t * obs->myXyz;
                        obs->myStatistic.qxx = t * obs->myStatistic.qxx;
                    }else if(i = movements.size()-1){
                        OiMat t= movements.at(i)->getHomogenMatrix();
                        obs->myXyz = t * obs->myXyz;
                        obs->myStatistic.qxx = t * obs->myStatistic.qxx;
                    }
                }
            }
        }
    }else{
        obs->myXyz = obs->myXyz;
        obs->myStatistic = obs->myOriginalStatistic;
    }
    obs->isValid = true;
}*/

/*!
 * \brief transformNewObservations transforms new observations directly into the active coordinate system if possible.
 * \param obs
 */
void TrafoController::transformNewObservations(Observation *obs)
{

    //not used yet!!!
    if(obs->myStation->coordSys != OiFeatureState::getActiveCoordinateSystem()){
        TrafoParam *tp = findTrafoParam(obs->myStation->coordSys,OiFeatureState::getActiveCoordinateSystem());
        if(tp != NULL){
            OiMat t;
            if(tp->getDestinationSystem() == OiFeatureState::getActiveCoordinateSystem()){
                t = tp->getHomogenMatrix();
            }else{
                t = tp->getHomogenMatrix().inv();
            }
            obs->myXyz = t * obs->myXyz;
            obs->myStatistic.qxx = t * obs->myStatistic.qxx;
        }
    }
}

/*!
 * \brief transformObservations transforms the observations of the specified coordinatesystem.
 * \param from
 * \return
 */
bool TrafoController::transformObservations(CoordinateSystem *from)
{
    if(from != NULL){

        if(from == OiFeatureState::getActiveCoordinateSystem()){
            //set observations valid, because start system is also target system. So you don´t need to transform
            setObservationState(from,true);
            return true;
        }

        //first transform
        TrafoParam *tp = findTrafoParam(from,OiFeatureState::getActiveCoordinateSystem());
        if(tp != NULL){
            OiMat t;
            if(tp->getStartSystem() == from){
                t = tp->getHomogenMatrix();
            }else{
                t = tp->getHomogenMatrix().inv();
            }
            foreach (Observation *obs, from->getObservations()) {
                obs->myXyz = t*obs->myXyz;
                obs->myStatistic.qxx = t*obs->myStatistic.qxx;
                obs->isValid = true;
            }

            //then apply movements if active system is a part system
            //if active system is a station => do nothing
            this->CheckToApplyMovements(from);

            return true;

        }else{ //no trafo params available from this coordsystem to active coord system.
               //search for datumstransformation of other station
            foreach(TrafoParam *tp, from->getTransformationParameters()){
                foreach (TrafoParam *t, tp->getStartSystem()->getTransformationParameters()) {
                    if(t->getisDatumTrafo() && t->getIsUsed() && t->getStartSystem() == OiFeatureState::getActiveCoordinateSystem()){
                        OiMat tt = t->getHomogenMatrix().inv();
                        OiMat ttp;
                        if(tp->getStartSystem() == from){
                            ttp = tp->getHomogenMatrix();
                        }else{
                            ttp = tp->getHomogenMatrix().inv();
                        }
                        foreach (Observation *obs, from->getObservations()) {
                            obs->myXyz = ttp * (tt * obs->myXyz);
                            obs->myStatistic.qxx = ttp * (tt * obs->myStatistic.qxx);
                            obs->isValid = true;
                        }
                        //then apply movements if active system is a part system
                        //if active system is a station => do nothing
                        this->CheckToApplyMovements(from);

                        return true;
                    }else if(t->getisDatumTrafo() && t->getIsUsed() && t->getDestinationSystem() == OiFeatureState::getActiveCoordinateSystem()){
                        OiMat tt = t->getHomogenMatrix();
                        OiMat ttp;
                        if(tp->getStartSystem() == from){
                            ttp = tp->getHomogenMatrix();
                        }else{
                            ttp = tp->getHomogenMatrix().inv();
                        }
                        foreach (Observation *obs, from->getObservations()) {
                            obs->myXyz = ttp * (tt * obs->myXyz);
                            obs->myStatistic.qxx = ttp * (tt * obs->myStatistic.qxx);
                            obs->isValid = true;
                        }
                        //then apply movements if active system is a part system
                        //if active system is a station => do nothing
                        this->CheckToApplyMovements(from);

                        return true;
                    }
                }
                foreach (TrafoParam *t, tp->getDestinationSystem()->getTransformationParameters()) {
                    if(t->getisDatumTrafo() && t->getIsUsed() && t->getStartSystem() == OiFeatureState::getActiveCoordinateSystem()){
                        OiMat tt = t->getHomogenMatrix().inv();
                        OiMat ttp;
                        if(tp->getStartSystem() == from){
                            ttp = tp->getHomogenMatrix();
                        }else{
                            ttp = tp->getHomogenMatrix().inv();
                        }
                        foreach (Observation *obs, from->getObservations()) {
                            obs->myXyz = ttp * (tt * obs->myXyz);
                            obs->myStatistic.qxx = ttp * (tt * obs->myStatistic.qxx);
                            obs->isValid = true;
                        }
                        //then apply movements if active system is a part system
                        //if active system is a station => do nothing
                        this->CheckToApplyMovements(from);

                        return true;
                    }else if(t->getisDatumTrafo() && t->getIsUsed() && t->getDestinationSystem() == OiFeatureState::getActiveCoordinateSystem()){
                        OiMat tt = t->getHomogenMatrix();
                        OiMat ttp;
                        if(tp->getStartSystem() == from){
                            ttp = tp->getHomogenMatrix();
                        }else{
                            ttp = tp->getHomogenMatrix().inv();
                        }
                        foreach (Observation *obs, from->getObservations()) {
                            obs->myXyz = ttp * (tt * obs->myXyz);
                            obs->myStatistic.qxx = ttp * (tt * obs->myStatistic.qxx);
                            obs->isValid = true;
                        }
                        //then apply movements if active system is a part system
                        //if active system is a station => do nothing
                        this->CheckToApplyMovements(from);

                        return true;
                    }
                }
            }
            //no trafo params available
            foreach(Observation *obs, from->getObservations()){
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
    foreach(Observation *obs, cs->getObservations()){
        if(valid == true){
            obs->myXyz = obs->myOriginalXyz;
        }
        obs->isValid = valid;
    }
}

/*!
 * \brief MovementsChanged need to recalc movements and correct all observations after a time of a movement or the state changed, a movement was added or a movement was deleted.
 */
void TrafoController::MovementsChanged()
{
}

/*!
 * \brief :recalcAllMovements recalculates all movement transformations.
 */
void TrafoController::recalcAllMovements()
{
}

/*!
 * \brief recalcObservations with the new movement transformations
 */
void TrafoController::recalcObservations()
{
}

/*!
 * \brief findTrafoParam for the specified coordinate system.
 * \param from
 * \param to
 * \return
 */
TrafoParam *TrafoController::findTrafoParam(CoordinateSystem *from, CoordinateSystem *to)
{
    foreach(TrafoParam *tp, from->getTransformationParameters()){
        if(tp->getDestinationSystem() != NULL && tp->getStartSystem() != NULL){
            if(tp->getDestinationSystem() == to || tp->getStartSystem() == to){
                if(tp->getIsUsed()){
                    return tp;
                }
            }
        }
    }
    return NULL;
}

/*!
 * \brief findMovements for the coordinate system of the observation and sort them by time
 * \param obs
 * \return
 */
/*QList<TrafoParam *> TrafoController::findMovements(Observation *obs)
{
    QList<TrafoParam*> movements;
    for(int i=0;i<OiFeatureState::getFeatureCount();i++){
        if(OiFeatureState::getFeatures().at(i)->getTrafoParam() != NULL && OiFeatureState::getFeatures().at(i)->getTrafoParam()->getIsMovement()){
            if(OiFeatureState::getFeatures().at(i)->getTrafoParam()->getDestinationSystem() == obs->myStation->coordSys){
                if(OiFeatureState::getFeatures().at(i)->getTrafoParam()->getIsUsed()){
                    movements.append(OiFeatureState::getFeatures().at(i)->getTrafoParam());
                }
            }
        }
    }

    //sort list on valid time attributes (ascending)
    movements = this->sortMovements(movements);

    return movements;
}*/

/*!
 * \brief sortMovements ascending on their valid time attributes using a QMap
 * \param movements
 * \return
 */
QList<TrafoParam *> TrafoController::sortMovements(QList<TrafoParam *> movements)
{
    QMap<QDateTime,TrafoParam*> map;
    for(int k=0;k<movements.size();k++){
        map.insert(movements.at(k)->getValidTime(),movements.at(k));
    }
    movements.clear();
    movements = map.values();

    return movements;
}

/*!
 * \brief CheckToApplyMovements checks if a movement has to be applied or not.
 * If you need to apply a movement, this function will apply it, else it won´t do anything.
 * If active system is a PART system, movements can be applied.
 * If active system is a station coord system, don´t apply movements !!!
 * \param from
 */
void TrafoController::CheckToApplyMovements(CoordinateSystem *from)
{/*
    //check if active system is a PART
    if(OiFeatureState::getCoordinateSystems().contains(OiFeatureState::getActiveCoordinateSystem())){
        QList<Station*> stations = OiFeatureState::getStations();
        for(int i=0;i<stations.size();i++){
            //check if from is a station with observations
            if(stations.at(i)->coordSys == from){
                //apply movements on the part-transformed obs of the station
                //get centroid point of old obs
                OiVec centroidRef(4);
                OiVec centroidLoc(4);
                foreach (Observation *obs, from->getObservations()) {
                    centroidRef = centroidRef + obs->myXyz; //get centroid of all obs from this station
                    applyMovements(obs);
                }
                centroidRef = centroidRef / from->getObservations().size();

                //now get centroid point of current obs state
                foreach (Observation *obs, from->getObservations()) {
                    centroidLoc = centroidLoc + obs->myXyz;
                }
                centroidLoc = centroidLoc / from->getObservations().size();

                //get translation between pre-movement state and movement state
                OiVec translation = centroidRef-centroidLoc;

                //now move the transformed observations
                foreach (Observation *obs, from->getObservations()) {
                    obs->myXyz = obs->myXyz + translation;
                }

                break; //exit, because from was found
            }
        }
    }*/
    //check if active system is a PART
    if(OiFeatureState::getCoordinateSystems().contains(OiFeatureState::getActiveCoordinateSystem())){
        QList<Station*> stations = OiFeatureState::getStations();
        for(int i=0;i<stations.size();i++){
            //check if from is a station with observations
            if(stations.at(i)->coordSys == from){
                //find movements for this station
                QList<TrafoParam*> movements;
                movements = this->findMovements(from);

                //apply movements on the part-transformed obs of the station
                this->applyMovements(movements,from);
            }
        }
    }
}

/*!
 * \brief findMovements find all movements for this coordinate system
 * \param from
 * \return
 */
QList<TrafoParam *> TrafoController::findMovements(CoordinateSystem *from)
{
    QList<TrafoParam*> movements;

    for(int i=0; i<from->getTransformationParameters().size();i++){
        if(from->getTransformationParameters().at(i)->getIsMovement() &&
                from->getTransformationParameters().at(i)->getIsUsed()){
            movements.append(from->getTransformationParameters().at(i));
        }
    }
    //sort list on valid time attributes (ascending)
    movements = this->sortMovements(movements);

    return movements;
}

/*!
 * \brief applyMovements checks each observation of the station and applys a movement transformation on it, if necessary
 * \param movements
 * \param from
 */
void TrafoController::applyMovements(QList<TrafoParam*> movements, CoordinateSystem *from)
{
    for(int i=0; i< movements.size();i++){ //iterate through all movements for this station

        QList<Observation*> movedObservations;
        OiVec centroidBefore(4);
        OiVec centroidAfter(4);

        for(int k=0; k<from->getObservations().size();k++){ // iterate through all observations of this station

            Observation *obs = from->getObservations().at(k);

            if(movements.size() == 1){
                if(movements.at(0)->getValidTime() < obs->myReading->measuredAt){
                    OiMat t= movements.at(0)->getHomogenMatrix();

                    centroidBefore = centroidBefore + obs->myXyz;
                    movedObservations.append(obs);

                    obs->myXyz = t*obs->myXyz;
                    obs->myStatistic.qxx = t* obs->myStatistic.qxx;

                    centroidAfter = centroidAfter + obs->myXyz;
                }
            }else{
                if(movements.at(i)->getValidTime() < obs->myReading->measuredAt){
                    if((i+1)<movements.size() && movements.at(i+1)->getValidTime() > obs->myReading->measuredAt){
                        OiMat t = movements.at(i)->getHomogenMatrix();

                        centroidBefore = centroidBefore + obs->myXyz;
                        movedObservations.append(obs);

                        obs->myXyz = t* obs->myXyz;
                        obs->myStatistic.qxx = t* obs->myStatistic.qxx;

                        centroidAfter = centroidAfter + obs->myXyz;
                    }else{
                        OiMat t = movements.at(i)->getHomogenMatrix();

                        centroidBefore = centroidBefore + obs->myXyz;
                        movedObservations.append(obs);

                        obs->myXyz = t* obs->myXyz;
                        obs->myStatistic.qxx = obs->myStatistic.qxx;

                        centroidAfter = centroidAfter + obs->myXyz;
                    }
                }
            }
        }
        //translate all obs to which this movement was applied.
        //must be done for every movement to its own
        centroidAfter = centroidAfter / movedObservations.size();
        centroidBefore = centroidBefore / movedObservations.size();
        this->applyTranslations(centroidBefore,centroidAfter,movedObservations);
    }
}

/*!
 * \brief applyTranslations
 * After observations were moved because of temperature change, they have to be translated, because applying the scale
 * calculated in the movement, translates all points.
 * \param centroidBefore
 * \param centroidAfter
 * \param observations
 */
void TrafoController::applyTranslations(OiVec centroidBefore, OiVec centroidAfter, QList<Observation *> observations)
{
    OiVec translation;
    translation = centroidBefore - centroidAfter;
    //apply the translation on each of the "moved" observations
    foreach (Observation *obs, observations) {
        obs->myXyz = obs->myXyz + translation;
    }
}
