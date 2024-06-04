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

    QList<QPointer<CoordinateSystem> > startSystems;

    //run through all observations of the feature
    foreach(const QPointer<Observation> &obs, feature->getFeatureWrapper()->getGeometry()->getObservations()){

        //check observation
        if(obs.isNull() || obs->getStation().isNull() || obs->getStation()->getCoordinateSystem().isNull()){
            continue;
        }

        //get the station system from which the observation was made
        QPointer<CoordinateSystem> startSystem = obs->getStation()->getCoordinateSystem();

        //add the start system to the list of start systems
        if(!startSystems.contains(startSystem)){
            startSystems.append(startSystem);
        }

    }

    //transform all observations of the start system
    foreach(const QPointer<CoordinateSystem> &system, startSystems){
        this->transformObservations(system, destinationSystem);
    }

}

/*!
 * \brief TrafoController::transformObservations
 * Transforms all observations of the start system to the destination system
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
                observation->xyz = observation->getOriginalXYZ();
                observation->ijk = observation->getOriginalIJK();
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

        //create homogeneous rotation matrix
        OiMat rotMat = trafoMat;
        rotMat.setAt(0, 3, 0.0);
        rotMat.setAt(1, 3, 0.0);
        rotMat.setAt(2, 3, 0.0);

        startSystem->setIsSolved(true);

        //transform coordinate system origin and axes
        startSystem->origin.setVector(trafoMat * startSystem->getOrigin().getVectorH());

        //transform observations
        foreach(const QPointer<Observation> &obs, startSystem->getObservations()) {
            if(obs->getOriginalXYZ().getSize() == 4){
                obs->xyz = trafoMat * obs->originalXyz;
                obs->setIsSolved(true);
            }
            if(obs->getOriginalIJK().getSize() == 4 && obs->hasDirection == true){
                obs->ijk = rotMat * obs->originalIjk;
                obs->ijk.removeLast();
                obs->ijk.normalize();
                obs->ijk.add(1.0);
                obs->setIsSolved(true);
            }
            if(obs->getOriginalSigmaXyz().getSize() == 4){
                obs->sigmaXyz = trafoMat * obs->originalSigmaXyz;
            }
            if(obs->originalSigmaIjk.getSize() == 4 && obs->hasDirection == true){
                obs->sigmaIjk = rotMat * obs->originalSigmaIjk;
                obs->sigmaIjk.removeLast();
                obs->sigmaIjk.normalize();
                obs->sigmaIjk.add(1.0);
            }
        }

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
 * \brief TrafoController::transformCoordSystems
 * \param startSystem
 * \param destinationSystem
 */
void TrafoController::transformCoordSystems(const QPointer<CoordinateSystem> &startSystem, const QPointer<CoordinateSystem> &destinationSystem, bool isStation)
{
    //check systems
    if(startSystem.isNull() || destinationSystem.isNull()){
        return;
    }

    if(startSystem == destinationSystem){

        setDefaultOriginAndNormal(startSystem, isStation);

        return;

    }else {

        //get homogeneous transformation matrix to transform
        //the matrix transforms the origin to current coord system and also handles datum - transformations
        OiMat trafoMat;
        bool hasTransformation = this->getTransformationMatrix(trafoMat, startSystem, destinationSystem);

        //if trafo matrix is valid
        //check if matrix is 4x4 = homogeneous matrix
        if(hasTransformation && trafoMat.getRowCount() == 4 && trafoMat.getColCount() == 4){
            startSystem->transformOriginAndAxis(trafoMat);

            if(isStation){
                startSystem->getStation()->getPosition()->setIsSolved(true);
            }

            return;
        }
    }

    setDefaultOriginAndNormal(startSystem, isStation);

}

/*!
 * \brief TrafoController::setDefaultOriginAndNormal set default origin / position (0,0,0) and default direction / normal (0,0,1)
 */
void TrafoController::setDefaultOriginAndNormal(const QPointer<CoordinateSystem> &startSystem,  bool isStation) {
    startSystem->resetOriginAndAxis();

    if(isStation){
        startSystem->getStation()->getPosition()->setIsSolved(true);
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

    //if start and destination system are identical there is no real transformation
    if(startSystem == destinationSystem){
        trafoMat = OiMat(4,4);
        trafoMat.setAt(0, 0, 1.0);
        trafoMat.setAt(1, 1, 1.0);
        trafoMat.setAt(2, 2, 1.0);
        trafoMat.setAt(3, 3, 1.0);
        return true;
    }

    //try to find a corresponding transformation to directly transform between systems
    QPointer<TrafoParam> trafoParam = this->findTransformation(startSystem, destinationSystem);
    if(!trafoParam.isNull() && trafoParam->getIsSolved()){
        if(trafoParam->getStartSystem() == startSystem){
            trafoMat = trafoParam->getHomogenMatrix();
        }else{
            trafoMat = trafoParam->getHomogenMatrix().inv();
        }
        return true;
    }

    //helper variable to ensure that the trafo chain contains a datum transformation
    bool datumTrafoInChain = false;

    //try to find a transformation chain
    foreach(const QPointer<TrafoParam> &tp, startSystem->getTransformationParameters()){

        //check tp
        if(tp.isNull() || !tp->getIsUsed() || !tp->getIsSolved()){
            continue;
        }

        //check if tp is a datum transformation
        if(tp->getIsDatumTrafo()){
            datumTrafoInChain = true;
        }else{
            datumTrafoInChain = false;
        }

        //search trafo param in tp that are in relation to the target system
        foreach(const QPointer<TrafoParam> &t, tp->getStartSystem()->getTransformationParameters()){

            //check t
            if(t.isNull() || !t->getIsUsed() || !t->getIsSolved() || (!t->getIsDatumTrafo() && !datumTrafoInChain)){
                continue;
            }

            //check if the start system of t is the destination system
            if(t->getStartSystem() == destinationSystem){

                trafoMat = t->getHomogenMatrix().inv();
                if(tp->getStartSystem() == startSystem){
                    trafoMat = trafoMat * tp->getHomogenMatrix();
                }else{
                    trafoMat = trafoMat * tp->getHomogenMatrix().inv();
                }
                return true;

            }else if(t->getDestinationSystem() == destinationSystem){

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

            //check t
            if(t.isNull() || !t->getIsUsed() || !t->getIsSolved() || (!t->getIsDatumTrafo() && !datumTrafoInChain)){
                continue;
            }

            //check if the start system of t is the destination system
            if(t->getStartSystem() == destinationSystem){

                trafoMat = t->getHomogenMatrix().inv();
                if(tp->getStartSystem() == startSystem){
                    trafoMat = trafoMat * tp->getHomogenMatrix();
                }else{
                    trafoMat = trafoMat * tp->getHomogenMatrix().inv();
                }
                return true;

            }else if(t->getDestinationSystem() == destinationSystem){

                trafoMat = t->getHomogenMatrix();
                if(tp->getStartSystem() == startSystem){
                    trafoMat = trafoMat * tp->getHomogenMatrix();
                }else{
                    trafoMat = trafoMat * tp->getHomogenMatrix().inv();
                }
                return true;

            }
        }
    }

    return false;

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
