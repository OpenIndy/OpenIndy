#include "trafocontroller.h"

TrafoController::TrafoController(QObject *parent) :
    QObject(parent)
{
}

/*!
 * \brief TrafoController::transformObservations
 * Transforms all observations of the given feature into the destination system
 * \param feature
 * \param destinationSystem
 */
void TrafoController::transformObservations(const QPointer<Feature> &feature, const QPointer<CoordinateSystem> &destinationSystem){

    //check feature and system
    if(feature.isNull() || feature->getFeatureWrapper().isNull() || feature->getFeatureWrapper()->getGeometry().isNull() || destinationSystem.isNull()){
        return;
    }

    //run through all observations of the feature
    foreach(const QPointer<Observation> &obs, feature->getFeatureWrapper()->getGeometry()->getObservations()){

        //check observation
        if(obs.isNull() || obs->getStation().isNull() || obs->getStation()->getCoordinateSystem().isNull()){
            continue;
        }

        //get the station system from which the observation was made
        QPointer<CoordinateSystem> startSystem = obs->getStation()->getCoordinateSystem();

        //transform all observations of the station system
        this->transformObservations(startSystem, destinationSystem);

    }

}

/*!
 * \brief TrafoController::transformObservations
 * Transforms all observations of the start
 * \param startSystem
 * \param destinationSystem
 */
void TrafoController::transformObservations(const QPointer<CoordinateSystem> &startSystem, const QPointer<CoordinateSystem> &destinationSystem){

    //check systems
    if(startSystem.isNull() || destinationSystem.isNull()){
        return;
    }

    //check if the systems are identical
    if(startSystem == destinationSystem){
        foreach(const QPointer<Observation> &observation, startSystem->getObservations()){
            if(!observation.isNull()){
                observation->setXYZ(observation->getOriginalXYZ());
                observation->setIsSolved(true);
            }
        }
        return;
    }

    //get homogeneous transformation matrix to transform all observations
    //the matrix transforms the obs to current coord system and also handles datum - transformations
    OiMat trafoMat;
    bool hasTransformation = this->getTransformationMatrix(trafoMat, startSystem, destinationSystem);

    //if trafo matrix is valid
    //check if matrix is 4x4 = homogeneous matrix
    if(hasTransformation && trafoMat.getRowCount() == 4 && trafoMat.getColCount() == 4){

        startSystem->setIsSolved(true);

        //transform coordinate system origin and axes
        startSystem->origin.setVector(trafoMat * startSystem->getOrigin().getVector());

        //transform observations
        foreach(const QPointer<Observation> &obs, startSystem->getObservations()) {
            if(obs->getOriginalStatistic().getQxx().getRowCount() == 4 && obs->getOriginalStatistic().getQxx().getColCount() == 4
                    && obs->getOriginalXYZ().getSize() == 4){
                obs->xyz = trafoMat * obs->originalXyz;
                obs->statistic.setQxx(trafoMat * obs->originalStatistic.getQxx());
                obs->setIsSolved(true);
            }
        }

        //then apply movements if active system is a part system
        //if active system is a station -> do nothing
        //this->CheckToApplyMovements(from);

    }else{

        //set the coord sys to not solved
        startSystem->setIsSolved(false);

        //set all observations to not solved
        foreach(const QPointer<Observation> &observation, startSystem->getObservations()){
            if(!observation.isNull()){
                observation->setIsSolved(false);
            }
        }

    }

}

/*!
 * \brief TrafoController::getTransformationMatrix
 * \param trafoMat
 * \param startSystem
 * \param destinationSystem
 * \return
 */
bool TrafoController::getTransformationMatrix(OiMat &trafoMat, const QPointer<CoordinateSystem> &startSystem, const QPointer<CoordinateSystem> &destinationSystem){

    //try to find a corresponding transformation to directly transform between systems
    QPointer<TrafoParam> trafoParam = this->findTransformation(startSystem, destinationSystem);
    if(!trafoParam.isNull()){
        if(trafoParam->getStartSystem() == startSystem){
            trafoMat = trafoParam->getHomogenMatrix();
        }else{
            trafoMat = trafoParam->getHomogenMatrix().inv();
        }
        return true;
    }

    //try to find a transformation chain
    foreach(const QPointer<TrafoParam> &tp, startSystem->getTransformationParameters()){

        //check tp
        if(tp.isNull() || !tp->getIsUsed()){
            continue;
        }

        //search trafo param in tp that are in relation to the target system
        foreach(const QPointer<TrafoParam> &t, tp->getStartSystem()->getTransformationParameters()){

            //check t
            if(t.isNull() || !t->getIsUsed() || !t->getIsDatumTrafo()){
                continue;
            }

            //check if the start system is the active system
            if(t->getStartSystem()->getIsActiveCoordinateSystem()){

                trafoMat = t->getHomogenMatrix().inv();
                if(tp->getStartSystem() == startSystem){
                    trafoMat = trafoMat * tp->getHomogenMatrix();
                }else{
                    trafoMat = trafoMat * tp->getHomogenMatrix().inv();
                }
                return true;

            }else if(t->getDestinationSystem()->getIsActiveCoordinateSystem()){

                trafoMat = t->getHomogenMatrix();
                if(tp->getStartSystem() == startSystem){
                    trafoMat = trafoMat * tp->getHomogenMatrix();
                }else{
                    trafoMat = trafoMat * tp->getHomogenMatrix().inv();
                }
                return true;

            }

        }

        //search trafo param in tp that are in relation to the target system
        foreach(const QPointer<TrafoParam> &t, tp->getDestinationSystem()->getTransformationParameters()) {

            //check tp
            if(tp.isNull() || !tp->getIsUsed()){
                continue;
            }

            //check if start system is the active system
            if(t->getStartSystem()->getIsActiveCoordinateSystem()){

                trafoMat = t->getHomogenMatrix().inv();
                if(tp->getStartSystem() == destinationSystem){
                    trafoMat = trafoMat * tp->getHomogenMatrix();
                }else{
                    trafoMat = trafoMat * tp->getHomogenMatrix().inv();
                }
                return true;

            }else if(t->getDestinationSystem()->getIsActiveCoordinateSystem()){

                trafoMat = t->getHomogenMatrix();
                if(tp->getStartSystem() == destinationSystem){
                    trafoMat = trafoMat * tp->getHomogenMatrix();
                }else{
                    trafoMat = trafoMat * tp->getHomogenMatrix().inv();
                }
                return true;

            }
        }
    }

}

/*!
 * \brief TrafoController::findTransformation
 * \param startSystem
 * \param destinationSystem
 * \return
 */
QPointer<TrafoParam> TrafoController::findTransformation(const QPointer<CoordinateSystem> &startSystem, const QPointer<CoordinateSystem> &destinationSystem){

    foreach(const QPointer<TrafoParam> tp, startSystem->getTransformationParameters()){

        //check trafo param
        if(tp.isNull() || tp->getStartSystem().isNull() || tp->getDestinationSystem().isNull()){
            continue;
        }

        //check if tp transforms from given start system to given destination system and is usable
        if((tp->getStartSystem() == destinationSystem || tp->getDestinationSystem() == destinationSystem) && tp->getIsUsed()){
            return tp;
        }

    }

    return QPointer<TrafoParam>(NULL);

}

/*
void TrafoController::addObservation(Observation *obs)
{
    //not used yet
}


void TrafoController::transformNewObservations(Observation *obs)
{

    //not used yet!!!
    if(obs->myStation->coordSys != OiJob::getActiveCoordinateSystem()){
        TrafoParam *tp = findTrafoParam(obs->myStation->coordSys,OiJob::getActiveCoordinateSystem());
        if(tp != NULL){
            OiMat t;
            if(tp->getDestinationSystem() == OiJob::getActiveCoordinateSystem()){
                t = tp->getHomogenMatrix();
            }else{
                t = tp->getHomogenMatrix().inv();
            }
            obs->myXyz = t * obs->myXyz;
            obs->myStatistic.qxx = t * obs->myStatistic.qxx;

            obs->setIsSolved(true);
            obs->setIsValid(true);
        }else{
            obs->setIsSolved(false);
            obs->setIsValid(true);
        }
        return;
    }
    obs->setIsSolved(true);
    obs->setIsValid(true);

}

bool TrafoController::transformObservations(CoordinateSystem *from)
{
    if(from != NULL){

        if(from == OiJob::getActiveCoordinateSystem()){
            //set observations valid, because start system is also destination system.
            //No transformation required
            setObservationState(from,true);
            return true;
        }

        //first set the coord sys not solved. Change after transformation
        from->setIsSolved(false);

        //get homogeneous transformation matrix to transform observations with
        //this matrix transforms the obs to current coord system, and also handles datum - transformations
        OiMat trafoMat = this->getTransformationMatrix(from);

        //if trafo matrix is valid
        //check if matrix is 4x4 = homogeneous matrix
        if(trafoMat.getRowCount() == 4 && trafoMat.getColCount() == 4){

            from->setIsSolved(true);

            //transform coordinate system origin
            from->origin = trafoMat * from->origin;

            //transform observations
            foreach (Observation *obs, from->getObservations()) {
                if(obs->myOriginalStatistic.qxx.getRowCount() == 4 && obs->myOriginalStatistic.qxx.getColCount() == 4
                        && obs->myOriginalXyz.getSize() == 4){
                    obs->myXyz = trafoMat * obs->myOriginalXyz;
                    obs->myStatistic.qxx = trafoMat * obs->myOriginalStatistic.qxx;
                    obs->setIsSolved(true);
                }
            }

            //then apply movements if active system is a part system
            //if active system is a station -> do nothing
            this->CheckToApplyMovements(from);

            return true;

        }else{
            //no trafo param available
            //set this observation to not valid for the current coord system
            setObservationState(from,false);
            return false;
        }
    }
    return false;
}

void TrafoController::setObservationState(CoordinateSystem *cs, bool solved)
{
    foreach(Observation *obs, cs->getObservations()){
        if(solved == true){
            //reset xyz to original values
            obs->myXyz = obs->myOriginalXyz;
        }
        obs->setIsSolved(solved);
    }
    //reset origin to 0/0/0
    cs->origin.setAt(0,0.0);
    cs->origin.setAt(1,0.0);
    cs->origin.setAt(2,0.0);
    cs->origin.setAt(3,1.0);
    cs->setIsSolved(solved);
}

TrafoParam *TrafoController::findTrafoParam(CoordinateSystem *from, CoordinateSystem *to)
{
    foreach(TrafoParam *tp, from->getTransformationParameters()){
        if(tp->getDestinationSystem() != NULL && tp->getStartSystem() != NULL){
            if(tp->getDestinationSystem() == to || tp->getStartSystem() == to){
                if(tp->getIsUsed()){    //check if trafo param should be used.
                    return tp;
                }
            }
        }
    }
    return NULL;
}

QList<TrafoParam *> TrafoController::sortMovements(QList<TrafoParam *> movements)
{
    QMap<QDateTime,TrafoParam*> map;
    //sort movements with a map. map key list is always sorted ascending.
    for(int k=0;k<movements.size();k++){
        map.insert(movements.at(k)->getValidTime(),movements.at(k));
    }
    movements.clear();
    movements = map.values();

    return movements;
}

void TrafoController::CheckToApplyMovements(CoordinateSystem *from)
{
    //check if active system is a PART
    if(OiJob::getCoordinateSystems().contains(OiJob::getActiveCoordinateSystem())){

        QList<Station*> stations = OiJob::getStations();

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

QList<TrafoParam *> TrafoController::findMovements(CoordinateSystem *from)
{
    QList<TrafoParam*> movements;

    for(int i=0; i<from->getTransformationParameters().size();i++){
        //check if trafo parameter is a movement and if it should be used.
        if(from->getTransformationParameters().at(i)->getIsMovement() &&
                from->getTransformationParameters().at(i)->getIsUsed()){

            movements.append(from->getTransformationParameters().at(i));
        }
    }
    //sort list on valid time attributes (ascending)
    movements = this->sortMovements(movements);

    return movements;
}

void TrafoController::applyMovements(QList<TrafoParam*> movements, CoordinateSystem *from)
{

     *The point from which the part expands. This must not be the origin of the part coordinate system
    If the real origin of expansion is different from the simulated one, the correction of temperature expansion
    will not be correct and cannot compensate the complete expansion.

    OiVec expansionOrigin = OiJob::getActiveCoordinateSystem()->getExpansionOrigin();
    expansionOrigin.setAt(3,1.0);

    for(int i=0; i< movements.size();i++){ //iterate through all movements for this station

        for(int k=0; k<from->getObservations().size();k++){ //iterate through all observations of this station

            Observation *obs = from->getObservations().at(k);

            //check if there is only one movement to apply
            if(movements.size() == 1){
                //check if you can apply the movement to the observation
                if(movements.at(0)->getValidTime() < obs->myReading->measuredAt){

                    OiMat t= movements.at(0)->getHomogenMatrix();

                    //reduce part transformed observation to expansion origin and apply movement.
                    OiVec tmp = obs->myXyz - expansionOrigin;
                    tmp.setAt(3,1.0);

                    tmp = t*tmp;
                    //move back to original position
                    obs->myXyz = tmp + expansionOrigin;
                    obs->myXyz.setAt(3,1.0);
                    obs->myStatistic.qxx = t* obs->myStatistic.qxx;
                }
            }else{
                //check if you can apply the movement to the observation
                if(movements.at(i)->getValidTime() < obs->myReading->measuredAt){
                    //check if next movement is not valid for this observation
                    if((i+1)<movements.size() && movements.at(i+1)->getValidTime() > obs->myReading->measuredAt){

                        OiMat t = movements.at(i)->getHomogenMatrix();

                        //recude part transformed observation to expansion origin and apply movement.
                        OiVec tmp = obs->myXyz - expansionOrigin;
                        tmp.setAt(3,1.0);

                        tmp = t*tmp;
                        //move back to original position
                        obs->myXyz = tmp + expansionOrigin;
                        obs->myXyz.setAt(3,1.0);
                        obs->myStatistic.qxx = t* obs->myStatistic.qxx;
                    }else{
                    //check if there is another valid movement for this observation
                        int count = 0;

                        for(int k =i+1;k<movements.size();k++){
                            //if the next movement is no more valid for this observation
                            if((k+1)<movements.size() && movements.at(k+1)->getValidTime() > obs->myReading->measuredAt){
                                count = k;
                            }else{
                                //if it is the last movement in the list
                                count = k;
                            }
                        }

                        if(count != 0){
                            OiMat t = movements.at(count)->getHomogenMatrix();

                            //recude part transformed observation to expansion origin and apply movement.
                            OiVec tmp = obs->myXyz - expansionOrigin;
                            tmp.setAt(3,1.0);

                            tmp = t*tmp;
                            //move back to original position
                            obs->myXyz = tmp + expansionOrigin;
                            obs->myXyz.setAt(3,1.0);
                            obs->myStatistic.qxx = t* obs->myStatistic.qxx;
                        }
                    }
                }
            }
        }
    }
}

OiMat TrafoController::getTransformationMatrix(CoordinateSystem *from)
{
    OiMat trafoMat;

   //get transformation parameters to transform
   TrafoParam *tp = findTrafoParam(from, OiJob::getActiveCoordinateSystem());

   if(tp != NULL && tp->getIsUsed()){
       //estimate start and destination system of trafo parameter
       if(tp->getStartSystem() == from){
           trafoMat = tp->getHomogenMatrix();
       }else{
           trafoMat = tp->getHomogenMatrix().inv();
       }
       return trafoMat;
   }else{
       // no trafo params available from this coordinate system to active coord system.
       //search for datumstransformation of other station


        *search a transformation "chain".
        *watch each trafo param of the from system and check if it has a connection to another trafo param
        *that has the active coord sys as start or destination system


       foreach (TrafoParam *tp, from->getTransformationParameters()) {

           if(tp->getIsUsed()){

               //search trafo param in tp that are in relation to the target system
               foreach (TrafoParam *t, tp->getStartSystem()->getTransformationParameters()) {
                   //watch if the trafo param is active and can  be used in the chain
                   if(t->getisDatumTrafo() && t->getIsUsed()){
                       //check if the start system is the active system
                       if(t->getStartSystem() == OiJob::getActiveCoordinateSystem()){

                           OiMat tt = t->getHomogenMatrix().inv();
                           OiMat ttp;

                           if(tp->getStartSystem() == from){
                               ttp = tp->getHomogenMatrix();
                           }else{
                               ttp = tp->getHomogenMatrix().inv();
                           }

                           trafoMat = tt*ttp;

                           return trafoMat;

                        //else if the destination system is the active system
                       }else if(t->getDestinationSystem() == OiJob::getActiveCoordinateSystem()){//check if the destination system is the active system

                           OiMat tt = t->getHomogenMatrix();
                           OiMat ttp;

                           if(tp->getStartSystem() == from){
                               ttp = tp->getHomogenMatrix();
                           }else{
                               ttp = tp->getHomogenMatrix().inv();
                           }

                           trafoMat = tt*ttp;

                           return trafoMat;
                       }
                   }
               }
               foreach (TrafoParam *t, tp->getDestinationSystem()->getTransformationParameters()) {
                   //watch if the trafo param is active and can  be used in the chain
                   if(t->getisDatumTrafo() && t->getIsUsed()){
                       //check if start system is the active system
                       if(t->getStartSystem() == OiJob::getActiveCoordinateSystem()){

                           OiMat tt = t->getHomogenMatrix().inv();
                           OiMat ttp;

                           if(tp->getStartSystem() == from){
                               ttp = tp->getHomogenMatrix();
                           }else{
                               ttp = tp->getHomogenMatrix().inv();
                           }

                           trafoMat = tt*ttp;

                           return trafoMat;

                        //else if the destination system is the active system
                       }else if(t->getDestinationSystem() == OiJob::getActiveCoordinateSystem()){//check if destination system is the active system

                           OiMat tt = t->getHomogenMatrix();
                           OiMat ttp;

                           if(tp->getStartSystem() == from){
                               ttp = tp->getHomogenMatrix();
                           }else{
                               ttp = tp->getHomogenMatrix().inv();
                           }

                           trafoMat = tt*ttp;

                           return trafoMat;

                       }
                   }
               }
           }
       }
   }
   return trafoMat;
}

void TrafoController::transformObsForMovementCalculation(CoordinateSystem *from, CoordinateSystem *to)
{
    OiMat m = this->getTransformationMatrix(from);

    if(m.getRowCount() == 4 && m.getColCount() == 4){

        foreach (Observation *obs, from->getObservations()) {
            obs->myXyz = m*obs->myOriginalXyz;
            obs->setIsSolved(true);
        }
    }else{
        this->setObservationState(from,false);
    }
}
*/
