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
            //if active system is a station => do nothing
            this->CheckToApplyMovements(from);

            return true;

        }else{
            //no trafo param available
            /*foreach (Observation *obs, from->getObservations()) {
                obs->isValid = false;
            }*/
            setObservationState(from,false);
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

        OiVec nominalCentroid = this->getNominalCentroid();
        OiVec ScaledNominalCentroid = this->getScaledNominalCentroid(movements.at(i)->getHomogenMatrix());

        OiVec translation = this->getTranslation(nominalCentroid, ScaledNominalCentroid, movements.at(i)->getHomogenMatrix());

        for(int k=0; k<from->getObservations().size();k++){ //iterate through all observations of this station

            Observation *obs = from->getObservations().at(k);

            if(movements.size() == 1){
                if(movements.at(0)->getValidTime() < obs->myReading->measuredAt){
                    OiMat t= movements.at(0)->getHomogenMatrix();

                    obs->myXyz = t*obs->myXyz;
                    obs->myXyz = obs->myXyz + translation;
                    obs->myStatistic.qxx = t* obs->myStatistic.qxx;
                }
            }else{
                if(movements.at(i)->getValidTime() < obs->myReading->measuredAt){
                    if((i+1)<movements.size() && movements.at(i+1)->getValidTime() > obs->myReading->measuredAt){
                        OiMat t = movements.at(i)->getHomogenMatrix();

                        obs->myXyz = t* obs->myXyz;
                        obs->myXyz = obs->myXyz + translation;
                        obs->myStatistic.qxx = t* obs->myStatistic.qxx;
                    }else{
                        OiMat t = movements.at(i)->getHomogenMatrix();

                        obs->myXyz = t* obs->myXyz;
                        obs->myXyz = obs->myXyz + translation;
                        obs->myStatistic.qxx = obs->myStatistic.qxx;
                    }
                }
            }
        }
    }
}

OiVec TrafoController::getNominalCentroid()
{
    OiVec centroid(4);
    int count = 0;

    for(int i=0; i<OiFeatureState::getFeatureCount();i++){

        if(OiFeatureState::getFeatures().at(i)->getPoint() != NULL){

            if(OiFeatureState::getFeatures().at(i)->getPoint()->getIsNominal() &&
                    OiFeatureState::getFeatures().at(i)->getPoint()->getNominalSystem() ==
                    OiFeatureState::getActiveCoordinateSystem()){

                centroid = centroid + OiFeatureState::getFeatures().at(i)->getPoint()->getXYZ();
                count++;
            }
        }
    }

    centroid = centroid /(double)count;

    return centroid;
}

OiVec TrafoController::getScaledNominalCentroid(OiMat homogenMat)
{
    OiVec centroid(4);
    int count = 0;

    for(int i=0; i<OiFeatureState::getFeatureCount();i++){

        if(OiFeatureState::getFeatures().at(i)->getPoint() != NULL){

            if(OiFeatureState::getFeatures().at(i)->getPoint()->getIsNominal() &&
                    OiFeatureState::getFeatures().at(i)->getPoint()->getNominalSystem() ==
                    OiFeatureState::getActiveCoordinateSystem()){

                centroid = centroid + homogenMat*OiFeatureState::getFeatures().at(i)->getPoint()->getXYZ();
                count++;
            }
        }
    }

    centroid = centroid /(double)count;

    return centroid;
}

OiVec TrafoController::getTranslation(OiVec nomCen, OiVec ScaledNomCen, OiMat homogenMat)
{
    OiVec translation = nomCen - ScaledNomCen;

    //TODO
    //cause not working good and as i want it. but it helps

    translation.setAt(2,0.0);

    return translation;
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

   if(tp != NULL){
       if(tp->getStartSystem() == from){
           trafoMat = tp->getHomogenMatrix();
       }else{
           trafoMat = tp->getHomogenMatrix().inv();
       }
       return trafoMat;
   }else{
       // no trafo params available from this coordinate system to active coord system.
       //search for datumstransformation of other station

       //search a transformation "chain".
       //watch each trafo param of the from system and check if it has a connection to another trafo param
       //that has the active coord sys as start or destination system

       foreach (TrafoParam *tp, from->getTransformationParameters()) {

           foreach (TrafoParam *t, tp->getStartSystem()->getTransformationParameters()) {
               if(t->getisDatumTrafo() && t->getIsUsed()){ //watch if the trafo param is active and can  be used in the chain
                   if(t->getStartSystem() == OiFeatureState::getActiveCoordinateSystem()){//check if the start system is the active system
                       OiMat tt = t->getHomogenMatrix().inv();
                       OiMat ttp;
                       if(tp->getStartSystem() == from){
                           ttp = tp->getHomogenMatrix();
                       }else{
                           ttp = tp->getHomogenMatrix().inv();
                       }

                       trafoMat = ttp*tt;
                       return trafoMat;

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
               if(t->getisDatumTrafo() && t->getIsUsed()){ // watch if trafo param is active and can be used in the chain
                   if(t->getStartSystem() == OiFeatureState::getActiveCoordinateSystem()){//check if start system is the active system
                       OiMat tt = t->getHomogenMatrix().inv();
                       OiMat ttp;
                       if(tp->getStartSystem() == from){
                           ttp = tp->getHomogenMatrix();
                       }else{
                           ttp = tp->getHomogenMatrix().inv();
                       }

                       trafoMat = ttp*tt;
                       return trafoMat;

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
   return trafoMat;
}
