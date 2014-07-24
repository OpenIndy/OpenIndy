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
    //not used yet
}

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
            //set observations valid, because start system is also destination system.
            //No transformation required
            setObservationState(from,true);
            return true;
        }

        //get homogeneous transformation matrix to transform observations with
        //this matrix transforms the obs to current coord system, and also handles datum - transformations
        OiMat trafoMat = this->getTransformationMatrix(from);

        //if trafo matrix is valid
        //check if matrix is 4x4 = homogeneous matrix
        if(trafoMat.getRowCount() == 4 && trafoMat.getColCount() == 4){
            //transform observations
            foreach (Observation *obs, from->getObservations()) {
                obs->myXyz = trafoMat * obs->myOriginalXyz;
                obs->myStatistic.qxx = trafoMat * obs->myOriginalStatistic.qxx;
                obs->isValid = true;
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

/*!
 * \brief setObservationState sets the observation valid or not valid, depending on current coord system
 * \param cs
 * \param valid
 */
void TrafoController::setObservationState(CoordinateSystem *cs, bool valid)
{
    foreach(Observation *obs, cs->getObservations()){
        if(valid == true){
            //reset xyz to original values
            obs->myXyz = obs->myOriginalXyz;
        }
        obs->isValid = valid;
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

/*!
 * \brief sortMovements ascending on their valid time attributes using a QMap
 * \param movements
 * \return
 */
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

/*!
 * \brief CheckToApplyMovements checks if a movement has to be applied or not.
 * If you need to apply a movement, this function will apply it, else it won´t do anything.
 * If active system is a PART system, movements can be applied.
 * If active system is a station coord system, don´t apply movements!!!
 * \param from
 */
void TrafoController::CheckToApplyMovements(CoordinateSystem *from)
{
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

/*!
 * \brief applyMovements checks each observation of the station and applys a movement transformation on it, if necessary
 * \param movements
 * \param from
 */
void TrafoController::applyMovements(QList<TrafoParam*> movements, CoordinateSystem *from)
{
    /*
     *The point from which the part expands. This must not be the origin of the part coordinate system
    If the real origin of expansion is different from the simulated one, the correction of temperature expansion
    will not be correct and cannot compensate the complete expansion.
    */
    OiVec expansionOrigin = OiFeatureState::getActiveCoordinateSystem()->getExpansionOrigin();
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

                        OiMat t = movements.at(i)->getHomogenMatrix();

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

/*!
 * \brief getTransformationMatrix searches and combines a homogeneous transformation matrix for the given coord sys to the active coord sys.
 * This homogeneous transformation matrix also includes datums transformations.
 * \param from
 * \return
 */
OiMat TrafoController::getTransformationMatrix(CoordinateSystem *from)
{
    OiMat trafoMat;

   //get transformation parameters to transform
   TrafoParam *tp = findTrafoParam(from, OiFeatureState::getActiveCoordinateSystem());

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

       /*
        *search a transformation "chain".
        *watch each trafo param of the from system and check if it has a connection to another trafo param
        *that has the active coord sys as start or destination system
       */

       foreach (TrafoParam *tp, from->getTransformationParameters()) {

           if(tp->getIsUsed()){

               //search trafo param in tp that are in relation to the target system
               foreach (TrafoParam *t, tp->getStartSystem()->getTransformationParameters()) {
                   //watch if the trafo param is active and can  be used in the chain
                   if(t->getisDatumTrafo() && t->getIsUsed()){
                       //check if the start system is the active system
                       if(t->getStartSystem() == OiFeatureState::getActiveCoordinateSystem()){

                           OiMat tt = t->getHomogenMatrix().inv();
                           OiMat ttp;

                           if(tp->getStartSystem() == from){
                               ttp = tp->getHomogenMatrix();
                           }else{
                               ttp = tp->getHomogenMatrix().inv();
                           }

                           trafoMat = ttp*tt;

                           return trafoMat;

                        //else if the destination system is the active system
                       }else if(t->getDestinationSystem() == OiFeatureState::getActiveCoordinateSystem()){//check if the destination system is the active system

                           OiMat tt = t->getHomogenMatrix();
                           OiMat ttp;

                           if(tp->getStartSystem() == from){
                               ttp = tp->getHomogenMatrix();
                           }else{
                               ttp = tp->getHomogenMatrix().inv();
                           }

                           trafoMat = ttp*tt;

                           return trafoMat;
                       }
                   }
               }
               foreach (TrafoParam *t, tp->getDestinationSystem()->getTransformationParameters()) {
                   //watch if the trafo param is active and can  be used in the chain
                   if(t->getisDatumTrafo() && t->getIsUsed()){
                       //check if start system is the active system
                       if(t->getStartSystem() == OiFeatureState::getActiveCoordinateSystem()){

                           OiMat tt = t->getHomogenMatrix().inv();
                           OiMat ttp;

                           if(tp->getStartSystem() == from){
                               ttp = tp->getHomogenMatrix();
                           }else{
                               ttp = tp->getHomogenMatrix().inv();
                           }

                           trafoMat = ttp*tt;

                           return trafoMat;

                        //else if the destination system is the active system
                       }else if(t->getDestinationSystem() == OiFeatureState::getActiveCoordinateSystem()){//check if destination system is the active system

                           OiMat tt = t->getHomogenMatrix();
                           OiMat ttp;

                           if(tp->getStartSystem() == from){
                               ttp = tp->getHomogenMatrix();
                           }else{
                               ttp = tp->getHomogenMatrix().inv();
                           }

                           trafoMat = ttp*tt;

                           return trafoMat;

                       }
                   }
               }
           }
       }
   }
   return trafoMat;
}

/*!
 * \brief transformObsForMovementCalculation
 * \param from
 * \param to
 */
void TrafoController::transformObsForMovementCalculation(CoordinateSystem *from, CoordinateSystem *to)
{
    TrafoParam *tp = NULL;

    tp = this->findTrafoParam(from,to);

    if(tp != NULL){

        if(tp->getStartSystem() == from){

            OiMat t = tp->getHomogenMatrix();

            foreach (Observation *obs, from->getObservations()) {

                obs->myXyz = t*obs->myOriginalXyz;
                obs->isValid = true;
            }

        }else{

            OiMat t = tp->getHomogenMatrix().inv();

            foreach (Observation *obs, from->getObservations()) {

                obs->myXyz = t*obs->myOriginalXyz;
                obs->isValid = true;
            }
        }
    }else{
        this->setObservationState(from,false);
    }
}
