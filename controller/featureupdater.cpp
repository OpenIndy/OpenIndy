#include "featureupdater.h"

/*!
 * \brief FeatureUpdater::FeatureUpdater
 * \param parent
 */
FeatureUpdater::FeatureUpdater(QObject *parent) : QObject(parent)
{
}

/*!
 * \brief FeatureUpdater::getCurrentJob
 * \return
 */
const QPointer<OiJob> &FeatureUpdater::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief FeatureUpdater::setCurrentJob
 * \param job
 */
void FeatureUpdater::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){

        //disconnect current job
        if(!this->currentJob.isNull()){
            this->disconnectJob();
        }

        this->currentJob = job;
        this->connectJob();

    }
}

/*!
 * \brief FeatureUpdater::recalcAll
 * Recalculates the hole set of features and trafo params of the current OpenIndy job
 */
/*void FeatureUpdater::recalcAll(){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    //check active coordinate system
    if(this->currentJob->getActiveCoordinateSystem().isNull()){
        return;
    }

    //save a list with ids of the solved trafo params
    QList<int> solvedTrafoParams;

    //run through all nominal systems
    foreach(const QPointer<CoordinateSystem> &system, this->currentJob->getCoordinateSystemsList()){

        //check system
        if(system.isNull()){
            continue;
        }

        //run through all trafo params of the system
        foreach(const QPointer<TrafoParam> &tp, system->getTransformationParameters()){

            //check trafo param
            if(tp.isNull()){
                continue;
            }

            //recalculate the trafo param if not yet done
            if(!solvedTrafoParams.contains(tp->getId())){
               this->recalcTrafoParam(tp);
               solvedTrafoParams.append(tp->getId());
            }

        }

    }

    //run through all station systems
    foreach(const QPointer<Station> &station, this->currentJob->getStationsList()){

        //check station system
        if(station.isNull() || station->getCoordinateSystem().isNull()){
            continue;
        }

        //run through all trafo params of the station system
        foreach(const QPointer<TrafoParam> &tp, station->getCoordinateSystem()->getTransformationParameters()){

            //check trafo param
            if(tp.isNull()){
                continue;
            }

            //recalculate the trafo param if not yet done
            if(!solvedTrafoParams.contains(tp->getId())){
                this->recalcTrafoParam(tp);
                solvedTrafoParams.append(tp->getId());
            }

        }
    }

    //recalculate all features
    this->recalcFeatureSet();

}*/

/*!
 * \brief FeatureUpdater::recalcFeatureSet
 * Recalculate the hole set of features of the current OpenIndy job
 */
void FeatureUpdater::recalcFeatureSet(){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    //check active coordinate system
    if(this->currentJob->getActiveCoordinateSystem().isNull()){
        return;
    }

    //set all features to not have been updated
    foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesList()){
        if(!feature.isNull() && !feature->getFeature().isNull()){
            feature->getFeature()->blockSignals(true);
            feature->getFeature()->setIsUpdated(false);
            feature->getFeature()->blockSignals(false);
        }
    }

    //recalc recursively (block signals during recalculation, one signal for all features is emitted at the end)
    this->blockSignals(true);
    foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesList()){

        //check feature
        if(feature.isNull() || feature->getFeature().isNull()){
            continue;
        }

        //recalc the feature if it was not recalced yet and is no trafo param
        if(!feature->getFeature()->getIsUpdated() && feature->getFeatureTypeEnum() != eTrafoParamFeature){
            //this->recalcFeature(feature->getFeature());
            this->recursiveFeatureRecalculation(feature->getFeature());
        }

    }
    this->blockSignals(false);

    emit this->featuresRecalculated();

}

/*!
 * \brief FeatureUpdater::recalcFeature
 * Recalculates a single feature of an OpenIndy job and all its dependencies
 * \param feature
 */
void FeatureUpdater::recalcFeature(const QPointer<Feature> &feature){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    //check active coordinate system
    if(this->currentJob->getActiveCoordinateSystem().isNull()){
        return;
    }

    //check the feature
    if(feature.isNull() || feature->getFeatureWrapper().isNull() || !feature->getFeatureWrapper()->getTrafoParam().isNull()){
        return;
    }

    //transform all observations of the feature to the active coordinate system
    this->trafoController.transformObservations(feature, this->currentJob->getActiveCoordinateSystem());

    //set all features to not have been updated
    foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesList()){
        if(!feature.isNull() && !feature->getFeature().isNull()){
            feature->getFeature()->blockSignals(true);
            feature->getFeature()->setIsUpdated(false);
            feature->getFeature()->blockSignals(false);
        }
    }

    //recalculate feature
    this->recursiveFeatureRecalculation(feature);

    emit this->featureRecalculated(feature->getId());

}

/*!
 * \brief FeatureUpdater::recalcTrafoParam
 * Recalculates a transformation parameter feature
 * \param trafoParam
 */
void FeatureUpdater::recalcTrafoParam(const QPointer<TrafoParam> &trafoParam){

    //check trafo param
    if(trafoParam.isNull() || trafoParam->getStartSystem().isNull() || trafoParam->getDestinationSystem().isNull()){
        return;
    }

    //get and check system transformation
    if(trafoParam->getFunctions().size() == 0 || trafoParam->getFunctions().at(0).isNull()){
        return;
    }
    QPointer<SystemTransformation> systemTransformation = dynamic_cast<SystemTransformation *>(trafoParam->getFunctions().at(0).data());
    if(systemTransformation.isNull()){
        return;
    }

    //set up input feature
    if(trafoParam->getStartSystem()->getIsStationSystem() && trafoParam->getDestinationSystem()->getIsStationSystem()){ //actual - actual
        this->setUpTrafoParamActualActual(trafoParam, systemTransformation);
    }else if((trafoParam->getStartSystem()->getIsStationSystem() && !trafoParam->getDestinationSystem()->getIsStationSystem())
             || (!trafoParam->getStartSystem()->getIsStationSystem() && trafoParam->getDestinationSystem()->getIsStationSystem())){ //actual - nominal
        this->setUpTrafoParamActualNominal(trafoParam, systemTransformation);
    }else{ //nominal - nominal
        this->setUpTrafoParamNominalNominal(trafoParam, systemTransformation);
    }

    //recalculate trafo param
    trafoParam->recalc();
    trafoParam->setIsUpdated(true);

    //recalc dependent features
    foreach(const QPointer<FeatureWrapper> &dependentFeature, trafoParam->getUsedFor()){

        //check feature
        if(dependentFeature.isNull() || dependentFeature->getFeature().isNull() || !dependentFeature->getTrafoParam().isNull()){
            continue;
        }

        this->recalcFeature(dependentFeature->getFeature());

    }

}

/*!
 * \brief FeatureUpdater::switchCoordinateSystem
 * Does all recalculation steps necessary when the active coordinate system has changed
 */
void FeatureUpdater::switchCoordinateSystem(){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //#######################################################
    //transform all observations to current coordinate system
    //#######################################################

    //run through all station systems
    foreach(const QPointer<Station> &station, this->currentJob->getStationsList()){

        //check station system
        if(station.isNull() || station->getCoordinateSystem().isNull()){
            continue;
        }

        //transform all observations of the station system to the active coordinate system
        this->trafoController.transformObservations(station->getCoordinateSystem(), this->currentJob->getActiveCoordinateSystem());

    }

    //###################################################################################
    //set nominals to solved only if their nominal system is the active coordinate system
    //###################################################################################

    //run through all nominal systems
    foreach(const QPointer<CoordinateSystem> &system, this->currentJob->getCoordinateSystemsList()){

        //check system
        if(system.isNull()){
            continue;
        }

        //run through all nominals of the system
        foreach(const QPointer<FeatureWrapper> &feature, system->getNominals()){

            bool isSolved = (system == this->currentJob->getActiveCoordinateSystem());

            //check feature
            if(!feature.isNull() && !feature->getGeometry().isNull() && feature->getGeometry()->getIsNominal()){
                feature->getFeature()->blockSignals(true);
                feature->getGeometry()->setIsSolved(isSolved);
                feature->getFeature()->blockSignals(false);
            }

        }

    }

    //###################
    //recalc all features
    //###################

    this->recalcFeatureSet();

}

/*!
 * \brief FeatureUpdater::connectJob
 */
void FeatureUpdater::connectJob(){

    QObject::connect(this, &FeatureUpdater::featureRecalculated, this->currentJob.data(), &OiJob::featureRecalculated, Qt::AutoConnection);
    QObject::connect(this, &FeatureUpdater::featuresRecalculated, this->currentJob.data(), &OiJob::featuresRecalculated, Qt::AutoConnection);
    QObject::connect(this, &FeatureUpdater::trafoParamRecalculated, this->currentJob.data(), &OiJob::trafoParamRecalculated, Qt::AutoConnection);

    QObject::connect(this->currentJob.data(), &OiJob::recalcFeature, this, &FeatureUpdater::recalcFeature, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::activeCoordinateSystemChanged, this, &FeatureUpdater::switchCoordinateSystem, Qt::AutoConnection);

}

/*!
 * \brief FeatureUpdater::disconnectJob
 */
void FeatureUpdater::disconnectJob(){

    QObject::disconnect(this, &FeatureUpdater::featureRecalculated, this->currentJob.data(), &OiJob::featureRecalculated);
    QObject::disconnect(this, &FeatureUpdater::featuresRecalculated, this->currentJob.data(), &OiJob::featuresRecalculated);
    QObject::disconnect(this, &FeatureUpdater::trafoParamRecalculated, this->currentJob.data(), &OiJob::trafoParamRecalculated);

    QObject::disconnect(this->currentJob.data(), &OiJob::recalcFeature, this, &FeatureUpdater::recalcFeature);
    QObject::disconnect(this->currentJob.data(), &OiJob::activeCoordinateSystemChanged, this, &FeatureUpdater::switchCoordinateSystem);

}

/*!
 * \brief FeatureUpdater::setFeatureIsUpdated
 * Sets isUpdated for the given feature and all dependent features
 * \param feature
 * \param isUpdated
 */
/*void FeatureUpdater::setFeatureIsUpdated(const QPointer<Feature> &feature, bool isUpdated){

    //check if isUpdated is already set correctly
    if(feature->getIsUpdated() == isUpdated){
        return;
    }

    //set isUpdated for feature
    feature->setIsUpdated(isUpdated);

    //set isUpdated for previously needed features
    foreach(const QPointer<FeatureWrapper> &neededFeature, feature->getPreviouslyNeeded()){
        if(!neededFeature.isNull() || !neededFeature->getFeature().isNull()){
            this->setFeatureIsUpdated(neededFeature->getFeature(), isUpdated);
        }
    }

    //set isUpdated for used for features
    foreach(const QPointer<FeatureWrapper> &usedFor, feature->getUsedFor()){
        if(!usedFor.isNull() || !usedFor->getFeature().isNull()){
            this->setFeatureIsUpdated(usedFor->getFeature(), isUpdated);
        }
    }

}*/

/*!
 * \brief FeatureUpdater::recursiveFeatureRecalculation
 * Is called on a feature to recursively recalculate the feature and all its dependencies
 * \param feature
 */
void FeatureUpdater::recursiveFeatureRecalculation(const QPointer<Feature> &feature){

    //check wether the feature has already been updated
    if(feature->getIsUpdated()){
        return;
    }

    //recalculate all needed features
    foreach(QPointer<FeatureWrapper> neededFeature, feature->getPreviouslyNeeded()){

        //check needed feature
        if(neededFeature.isNull() || neededFeature->getFeature().isNull()
                || !neededFeature->getTrafoParam().isNull()){
            continue;
        }

        //recalculate needed feature
        this->recursiveFeatureRecalculation(neededFeature->getFeature());

    }

    //check again wether the feature has already been updated
    if(feature->getIsUpdated()){
        return;
    }

    //recalculate feature and mark it as updated
    feature->recalc();
    feature->setIsUpdated(true);

    //recalculate dependent features
    foreach(QPointer<FeatureWrapper> dependentFeature, feature->getUsedFor()){

        //check needed feature
        if(dependentFeature.isNull() || dependentFeature->getFeature().isNull()
                || !dependentFeature->getTrafoParam().isNull()){
            continue;
        }

        //recalculate dependent feature
        this->recursiveFeatureRecalculation(dependentFeature->getFeature());

    }

}

/*!
 * \brief FeatureUpdater::setUpTrafoParamActualActual
 * \param trafoParam
 * \param systemTransformation
 */
void FeatureUpdater::setUpTrafoParamActualActual(const QPointer<TrafoParam> &trafoParam, const QPointer<SystemTransformation> &systemTransformation){

    //delete old copy elements
    systemTransformation->inputPointsStartSystem.clear();
    systemTransformation->inputPointsDestinationSystem.clear();
    QList<int> startKeys = systemTransformation->inputElementsStartSystem.keys();
    foreach(const int &key, startKeys){
        QList<InputElement> startElements = systemTransformation->inputElementsStartSystem.value(key);
        foreach(const InputElement &element, startElements){
            if(!element.geometry.isNull()){
                delete element.geometry;
            }
        }
    }
    systemTransformation->inputElementsStartSystem.clear();
    QList<int> destKeys = systemTransformation->inputElementsDestinationSystem.keys();
    foreach(const int &key, destKeys){
        QList<InputElement> destElements = systemTransformation->inputElementsDestinationSystem.value(key);
        foreach(const InputElement &element, destElements){
            if(!element.geometry.isNull()){
                delete element.geometry;
            }
        }
    }
    systemTransformation->inputElementsDestinationSystem.clear();

    //get and check keys
    bool isAlignment;
    QList<int> keys = systemTransformation->getInputElements().keys();
    if(keys.size() <= 0){ //no input elements
        return;
    }else if(keys.size() == 1){ //normal transformation
        isAlignment = false;
    }else{ //alignment
        isAlignment = true;
    }

    //sort helper object which compares and sorts the list of start and target points
    SortListByName sorter;

    //###################
    //set up start system
    //###################

    //switch coordinate system to "from" system
    this->switchCoordinateSystemWithoutTransformation(trafoParam->getStartSystem());

    //set up input elements for alignment transformations
    if(isAlignment){

        //run through all keys and add start system elements
        QList<int> keys = systemTransformation->getInputElements().keys();
        foreach(const int &key, keys){

            QList<InputElement> newElements;

            //get input elements at the position key
            QList<InputElement> inputElements = systemTransformation->getInputElements().value(key);
            foreach(const InputElement &element, inputElements){

                //check geometry
                if(element.geometry.isNull() || element.geometry->getFeatureWrapper().isNull()){
                    continue;
                }

                //check if the geometry is an actual and is solved
                if(element.geometry->getIsNominal() || !element.geometry->getIsSolved()){
                    continue;
                }

                InputElement copyElement(element.id);
                this->copyGeometry(copyElement, element.geometry->getFeatureWrapper(), element.typeOfElement);
                newElements.append(copyElement);

            }

            if(newElements.size() > 0){
                systemTransformation->inputElementsStartSystem.insert(key, newElements);
            }

        }

    }

    //set up input elements for normal transformations
    if(!isAlignment){

        //get input elements at the position 0
        QList<InputElement> inputElements = systemTransformation->getInputElements().value(0);
        foreach(const InputElement &element, inputElements){

            //add all points
            if(element.typeOfElement == ePointElement && !element.point.isNull()){
                if(!element.point->getIsNominal() && element.point->getIsSolved()){
                    Point p(false);
                    p.setFeatureName(element.point->getFeatureName());
                    p.setPoint(element.point->getPosition());
                    sorter.addLocPoint(p);
                }
            }

        }

    }

    //#########################
    //set up destination system
    //#########################

    //switch system to "to" system
    this->switchCoordinateSystemWithoutTransformation(trafoParam->getDestinationSystem());

    //set up input elements for alignment transformations
    if(isAlignment){

        //run through all keys and add destination system elements
        QList<int> keys = systemTransformation->getInputElements().keys();
        foreach(const int &key, keys){

            QList<InputElement> newElements;

            //get input elements at the position key
            QList<InputElement> inputElements = systemTransformation->getInputElements().value(key);
            foreach(const InputElement &element, inputElements){

                //check geometry
                if(element.geometry.isNull() || element.geometry->getFeatureWrapper().isNull()){
                    continue;
                }

                //check if the geometry is an actual and is solved
                if(element.geometry->getIsNominal() || !element.geometry->getIsSolved()){
                    continue;
                }

                InputElement copyElement(element.id);
                this->copyGeometry(copyElement, element.geometry->getFeatureWrapper(), element.typeOfElement);
                newElements.append(copyElement);

            }

            if(newElements.size() > 0){
                systemTransformation->inputElementsDestinationSystem.insert(key, newElements);
            }

        }

    }

    //set up input elements for normal transformations
    if(!isAlignment){

        //get input elements at the position 0
        QList<InputElement> inputElements = systemTransformation->getInputElements().value(0);
        foreach(const InputElement &element, inputElements){

            //add all points
            if(element.typeOfElement == ePointElement && !element.point.isNull()){
                if(!element.point->getIsNominal() && element.point->getIsSolved()){
                    Point p(false);
                    p.setFeatureName(element.point->getFeatureName());
                    p.setPoint(element.point->getPosition());
                    sorter.addRefPoint(p);
                    //sorter.addLocPoint(Point(*element.point.data()));
                }
            }

        }

    }

    //#########################
    //reswitch to active system
    //#########################

    //add sorted lists to the function
    systemTransformation->inputPointsStartSystem = sorter.getLocPoints();
    systemTransformation->inputPointsDestinationSystem = sorter.getRefPoints();

    //switch back to the active system
    this->switchCoordinateSystem();

}

/*!
 * \brief FeatureUpdater::setUpTrafoParamActualNominal
 * \param trafoParam
 * \param systemTransformation
 */
void FeatureUpdater::setUpTrafoParamActualNominal(const QPointer<TrafoParam> &trafoParam, const QPointer<SystemTransformation> &systemTransformation){

    //delete old copy elements
    systemTransformation->inputPointsStartSystem.clear();
    systemTransformation->inputPointsDestinationSystem.clear();
    QList<int> startKeys = systemTransformation->inputElementsStartSystem.keys();
    foreach(const int &key, startKeys){
        QList<InputElement> startElements = systemTransformation->inputElementsStartSystem.value(key);
        foreach(const InputElement &element, startElements){
            if(!element.geometry.isNull()){
                delete element.geometry;
            }
        }
    }
    systemTransformation->inputElementsStartSystem.clear();
    QList<int> destKeys = systemTransformation->inputElementsDestinationSystem.keys();
    foreach(const int &key, destKeys){
        QList<InputElement> destElements = systemTransformation->inputElementsDestinationSystem.value(key);
        foreach(const InputElement &element, destElements){
            if(!element.geometry.isNull()){
                delete element.geometry;
            }
        }
    }
    systemTransformation->inputElementsDestinationSystem.clear();

    //get and check keys
    bool isAlignment;
    QList<int> keys = systemTransformation->getInputElements().keys();
    if(keys.size() <= 0){ //no input elements
        return;
    }else if(keys.size() == 1){ //normal transformation
        isAlignment = false;
    }else{ //alignment
        isAlignment = true;
    }

    //sort helper object which compares and sorts the list of start and target points
    SortListByName sorter;

    //###################
    //set up start system
    //###################

    //switch to "from" system
    this->switchCoordinateSystemWithoutTransformation(trafoParam->getStartSystem());

    //set up input elements for alignment transformations
    if(isAlignment){

        //run through all keys and add start system elements
        QList<int> keys = systemTransformation->getInputElements().keys();
        foreach(const int &key, keys){

            QList<InputElement> newElements;

            //get input elements at the position key
            QList<InputElement> inputElements = systemTransformation->getInputElements().value(key);
            foreach(const InputElement &element, inputElements){

                //check geometry
                if(element.geometry.isNull() || element.geometry->getFeatureWrapper().isNull()){
                    continue;
                }

                //check if the geometry is an actual and is solved
                if(element.geometry->getIsNominal() || !element.geometry->getIsSolved()){
                    continue;
                }

                InputElement copyElement(element.id);
                this->copyGeometry(copyElement, element.geometry->getFeatureWrapper(), element.typeOfElement);
                newElements.append(copyElement);

            }

            if(newElements.size() > 0){
                systemTransformation->inputElementsStartSystem.insert(key, newElements);
            }

        }

    }

    //set up input elements for normal transformations
    if(!isAlignment){

        //get input elements at the position 0
        QList<InputElement> inputElements = systemTransformation->getInputElements().value(0);
        foreach(const InputElement &element, inputElements){

            //add all points
            if(element.typeOfElement == ePointElement && !element.point.isNull()){
                if((!trafoParam->getStartSystem()->getIsStationSystem() && element.point->getIsNominal())
                        || element.point->getIsSolved()){
                    Point p(element.point->getIsNominal());
                    p.setFeatureName(element.point->getFeatureName());
                    p.setPoint(element.point->getPosition());
                    sorter.addLocPoint(p);
                }
            }

        }

    }

    //#########################
    //set up destination system
    //#########################

    //switch to "to" system
    this->switchCoordinateSystemWithoutTransformation(trafoParam->getDestinationSystem());

    //set up input elements for alignment transformations
    if(isAlignment){

        //run through all keys and add destination system elements
        QList<int> keys = systemTransformation->getInputElements().keys();
        foreach(const int &key, keys){

            QList<InputElement> newElements;

            //get input elements at the position key
            QList<InputElement> inputElements = systemTransformation->getInputElements().value(key);
            foreach(const InputElement &element, inputElements){

                //check geometry
                if(element.geometry.isNull() || element.geometry->getFeatureWrapper().isNull()){
                    continue;
                }

                //check if the geometry is an actual and is solved
                if(element.geometry->getIsNominal() || !element.geometry->getIsSolved()){
                    continue;
                }

                InputElement copyElement(element.id);
                this->copyGeometry(copyElement, element.geometry->getFeatureWrapper(), element.typeOfElement);
                newElements.append(copyElement);

            }

            if(newElements.size() > 0){
                systemTransformation->inputElementsDestinationSystem.insert(key, newElements);
            }

        }

    }

    //set up input elements for normal transformations
    if(!isAlignment){

        //get input elements at the position 0
        QList<InputElement> inputElements = systemTransformation->getInputElements().value(0);
        foreach(const InputElement &element, inputElements){

            //add all points
            if(element.typeOfElement == ePointElement && !element.point.isNull()){
                if((!trafoParam->getDestinationSystem()->getIsStationSystem() && element.point->getIsNominal())
                        || element.point->getIsSolved()){
                    Point p(element.point->getIsNominal());
                    p.setFeatureName(element.point->getFeatureName());
                    p.setPoint(element.point->getPosition());
                    sorter.addRefPoint(p);
                }
            }

        }

    }

    //#########################
    //reswitch to active system
    //#########################

    //add sorted lists to the function
    systemTransformation->inputPointsStartSystem = sorter.getLocPoints();
    systemTransformation->inputPointsDestinationSystem = sorter.getRefPoints();

    //switch back to active system
    this->switchCoordinateSystem();

}

/*!
 * \brief FeatureUpdater::setUpTrafoParamNominalNominal
 * \param trafoParam
 * \param systemTransformation
 */
void FeatureUpdater::setUpTrafoParamNominalNominal(const QPointer<TrafoParam> &trafoParam, const QPointer<SystemTransformation> &systemTransformation){

    //delete old copy elements
    systemTransformation->inputPointsStartSystem.clear();
    systemTransformation->inputPointsDestinationSystem.clear();
    QList<int> startKeys = systemTransformation->inputElementsStartSystem.keys();
    foreach(const int &key, startKeys){
        QList<InputElement> startElements = systemTransformation->inputElementsStartSystem.value(key);
        foreach(const InputElement &element, startElements){
            if(!element.geometry.isNull()){
                delete element.geometry;
            }
        }
    }
    systemTransformation->inputElementsStartSystem.clear();
    QList<int> destKeys = systemTransformation->inputElementsDestinationSystem.keys();
    foreach(const int &key, destKeys){
        QList<InputElement> destElements = systemTransformation->inputElementsDestinationSystem.value(key);
        foreach(const InputElement &element, destElements){
            if(!element.geometry.isNull()){
                delete element.geometry;
            }
        }
    }
    systemTransformation->inputElementsDestinationSystem.clear();

    //get and check keys
    bool isAlignment;
    QList<int> keys = systemTransformation->getInputElements().keys();
    if(keys.size() <= 0){ //no input elements
        return;
    }else if(keys.size() == 1){ //normal transformation
        isAlignment = false;
    }else{ //alignment
        isAlignment = true;
    }

    //sort helper object which compares and sorts the list of start and target points
    SortListByName sorter;

    //###################
    //set up start system
    //###################

    //set up input elements for normal transformations
    if(!isAlignment){

        //get input elements at the position 0
        QList<InputElement> inputElements = systemTransformation->getInputElements().value(0);
        foreach(const InputElement &element, inputElements){

            //add all points
            if(element.typeOfElement == ePointElement && !element.point.isNull()){
                if(element.point->getIsNominal() && element.point->getNominalSystem() == trafoParam->getStartSystem()){
                    sorter.addLocPoint(Point(*element.point.data()));
                }
            }

        }

    }

    //#########################
    //set up destination system
    //#########################

    //if coord sys needs to be switched to "to" system
    this->switchCoordinateSystemWithoutTransformation(trafoParam->getDestinationSystem());

    //set up input elements for normal transformations
    if(!isAlignment){

        //get input elements at the position 0
        QList<InputElement> inputElements = systemTransformation->getInputElements().value(0);
        foreach(const InputElement &element, inputElements){

            //add all points
            if(element.typeOfElement == ePointElement && !element.point.isNull()){
                if(element.point->getIsNominal() && element.point->getNominalSystem() == trafoParam->getDestinationSystem()){
                    sorter.addLocPoint(Point(*element.point.data()));
                }
            }

        }

    }

    //add sorted lists to the function
    systemTransformation->inputPointsStartSystem = sorter.getLocPoints();
    systemTransformation->inputPointsDestinationSystem = sorter.getRefPoints();

}

/*!
 * \brief FeatureUpdater::switchCoordinateSystemWithoutTransformation
 * \param destinationSystem
 */
void FeatureUpdater::switchCoordinateSystemWithoutTransformation(const QPointer<CoordinateSystem> &destinationSystem){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //check destination system
    if(destinationSystem.isNull()){
        return;
    }

    //################################################
    //transform all observations to destination system
    //################################################

    //run through all station systems
    foreach(const QPointer<Station> &station, this->currentJob->getStationsList()){

        //check station system
        if(station.isNull() || station->getCoordinateSystem().isNull()){
            continue;
        }

        //run through all observations of the station system
        foreach(const QPointer<Observation> &obs, station->getCoordinateSystem()->getObservations()){

            bool isSolved = (station->getCoordinateSystem() == destinationSystem);

            //set observation to solved only if it has been measured in the active coordinate system
            if(!obs.isNull()){
                obs->setXYZ(obs->getOriginalXYZ());
                obs->setIsSolved(isSolved);
            }

        }

    }

    //###################
    //recalc all features
    //###################

    this->recalcFeatureSetWithoutTransformation();

    //#############################################################################
    //set nominals to solved only if their nominal system is the destination system
    //#############################################################################

    //run through all nominal systems
    foreach(const QPointer<CoordinateSystem> &system, this->currentJob->getCoordinateSystemsList()){

        //check system
        if(system.isNull()){
            continue;
        }

        //run through all nominals of the system
        foreach(const QPointer<FeatureWrapper> &feature, system->getNominals()){

            bool isSolved = (system == destinationSystem);

            //check feature
            if(!feature.isNull() && !feature->getGeometry().isNull() && feature->getGeometry()->getIsNominal()){
                feature->getGeometry()->setIsSolved(isSolved);
            }

        }

    }

}

/*!
 * \brief FeatureUpdater::recalcFeatureSetWithoutTransformation
 */
void FeatureUpdater::recalcFeatureSetWithoutTransformation(){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    //set all features to not have been updated
    foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesList()){
        if(!feature.isNull() && !feature->getFeature().isNull()){
            feature->getFeature()->setIsUpdated(false);
        }
    }

    //recalc recursively
    foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesList()){

        //check feature
        if(feature.isNull() || feature->getFeature().isNull()){
            continue;
        }

        //recalc the feature if it was not recalced yet and is no trafo param
        if(!feature->getFeature()->getIsUpdated() && feature->getFeatureTypeEnum() != eTrafoParamFeature){
            this->recalcFeatureWithoutTransformation(feature->getFeature());
        }

    }

}

/*!
 * \brief FeatureUpdater::recalcFeatureWithoutTransformation
 * \param feature
 */
void FeatureUpdater::recalcFeatureWithoutTransformation(const QPointer<Feature> &feature){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    //check the feature
    if(feature.isNull() || feature->getFeatureWrapper().isNull() || !feature->getFeatureWrapper()->getTrafoParam().isNull()){
        return;
    }

    //set feature to not updated
    feature->setIsUpdated(false);

    //recalculate feature
    this->recursiveFeatureRecalculation(feature);

}

/*!
 * \brief FeatureUpdater::copyGeometry
 * \param newElement
 * \param oldElement
 * \param type
 */
void FeatureUpdater::copyGeometry(InputElement &newElement, const QPointer<FeatureWrapper> &oldElement, const ElementTypes &type){

    //add all elements
    switch(type){
    case eCircleElement:{
        if(!oldElement.isNull() && !oldElement->getCircle().isNull() && !oldElement->getGeometry().isNull()){
            newElement.circle = new Circle(*oldElement->getCircle().data());
            newElement.geometry = newElement.circle.data();
            newElement.typeOfElement = eCircleElement;
        }
        return;
    }case eConeElement:{
        if(!oldElement.isNull() && !oldElement->getCone().isNull() && !oldElement->getGeometry().isNull()){
            newElement.cone = new Cone(*oldElement->getCone().data());
            newElement.geometry = newElement.cone.data();
            newElement.typeOfElement = eConeElement;
        }
        return;
    }
    case eCylinderElement:{
        if(!oldElement.isNull() && !oldElement->getCylinder().isNull() && !oldElement->getGeometry().isNull()){
            newElement.cylinder = new Cylinder(*oldElement->getCylinder().data());
            newElement.geometry = newElement.cylinder.data();
            newElement.typeOfElement = eCylinderElement;
        }
        return;
    }case eEllipseElement:{
        if(!oldElement.isNull() && !oldElement->getEllipse().isNull() && !oldElement->getGeometry().isNull()){
            newElement.ellipse = new Ellipse(*oldElement->getEllipse().data());
            newElement.geometry = newElement.ellipse.data();
            newElement.typeOfElement = eEllipseElement;
        }
        return;
    }case eEllipsoidElement:{
        if(!oldElement.isNull() && !oldElement->getEllipsoid().isNull() && !oldElement->getGeometry().isNull()){
            newElement.ellipsoid = new Ellipsoid(*oldElement->getEllipsoid().data());
            newElement.geometry = newElement.ellipsoid.data();
            newElement.typeOfElement = eEllipsoidElement;
        }
        return;
    }case eHyperboloidElement:{
        if(!oldElement.isNull() && !oldElement->getHyperboloid().isNull() && !oldElement->getGeometry().isNull()){
            newElement.hyperboloid = new Hyperboloid(*oldElement->getHyperboloid().data());
            newElement.geometry = newElement.hyperboloid.data();
            newElement.typeOfElement = eHyperboloidElement;
        }
        return;
    }case eLineElement:{
        if(!oldElement.isNull() && !oldElement->getLine().isNull() && !oldElement->getGeometry().isNull()){
            newElement.line = new Line(*oldElement->getLine().data());
            newElement.geometry = newElement.line.data();
            newElement.typeOfElement = eLineElement;
        }
        return;
    }case eNurbsElement:{
        if(!oldElement.isNull() && !oldElement->getNurbs().isNull() && !oldElement->getGeometry().isNull()){
            newElement.nurbs = new Nurbs(*oldElement->getNurbs().data());
            newElement.geometry = newElement.nurbs.data();
            newElement.typeOfElement = eNurbsElement;
        }
        return;
    }case eParaboloidElement:{
        if(!oldElement.isNull() && !oldElement->getParaboloid().isNull() && !oldElement->getGeometry().isNull()){
            newElement.paraboloid = new Paraboloid(*oldElement->getParaboloid().data());
            newElement.geometry = newElement.paraboloid.data();
            newElement.typeOfElement = eParaboloidElement;
        }
        return;
    }case ePlaneElement:{
        if(!oldElement.isNull() && !oldElement->getPlane().isNull() && !oldElement->getGeometry().isNull()){
            newElement.plane = new Plane(*oldElement->getPlane().data());
            newElement.geometry = newElement.plane.data();
            newElement.typeOfElement = ePlaneElement;
        }
        return;
    }case ePointElement:{
        if(!oldElement.isNull() && !oldElement->getPoint().isNull() && !oldElement->getGeometry().isNull()){
            newElement.point = new Point(*oldElement->getPoint().data());
            newElement.geometry = newElement.point.data();
            newElement.typeOfElement = ePointElement;
        }
        return;
    }case ePointCloudElement:{
        if(!oldElement.isNull() && !oldElement->getPointCloud().isNull() && !oldElement->getGeometry().isNull()){
            newElement.pointCloud = new PointCloud(*oldElement->getPointCloud().data());
            newElement.geometry = newElement.pointCloud.data();
            newElement.typeOfElement = ePointCloudElement;
        }
        return;
    }case eScalarEntityAngleElement:{
        if(!oldElement.isNull() && !oldElement->getScalarEntityAngle().isNull() && !oldElement->getGeometry().isNull()){
            newElement.scalarEntityAngle = new ScalarEntityAngle(*oldElement->getScalarEntityAngle().data());
            newElement.geometry = newElement.scalarEntityAngle.data();
            newElement.typeOfElement = eScalarEntityAngleElement;
        }
        return;
    }case eScalarEntityDistanceElement:{
        if(!oldElement.isNull() && !oldElement->getScalarEntityDistance().isNull() && !oldElement->getGeometry().isNull()){
            newElement.scalarEntityDistance = new ScalarEntityDistance(*oldElement->getScalarEntityDistance().data());
            newElement.geometry = newElement.scalarEntityDistance.data();
            newElement.typeOfElement = eScalarEntityDistanceElement;
        }
        return;
    }case eScalarEntityMeasurementSeriesElement:{
        if(!oldElement.isNull() && !oldElement->getScalarEntityMeasurementSeries().isNull() && !oldElement->getGeometry().isNull()){
            newElement.scalarEntityMeasurementSeries = new ScalarEntityMeasurementSeries(*oldElement->getScalarEntityMeasurementSeries().data());
            newElement.geometry = newElement.scalarEntityMeasurementSeries.data();
            newElement.typeOfElement = eScalarEntityMeasurementSeriesElement;
        }
        return;
    }case eScalarEntityTemperatureElement:{
        if(!oldElement.isNull() && !oldElement->getScalarEntityTemperature().isNull() && !oldElement->getGeometry().isNull()){
            newElement.scalarEntityTemperature = new ScalarEntityTemperature(*oldElement->getScalarEntityTemperature().data());
            newElement.geometry = newElement.scalarEntityTemperature.data();
            newElement.typeOfElement = eScalarEntityTemperatureElement;
        }
        return;
    }case eSlottedHoleElement:{
        if(!oldElement.isNull() && !oldElement->getSlottedHole().isNull() && !oldElement->getGeometry().isNull()){
            newElement.slottedHole = new SlottedHole(*oldElement->getSlottedHole().data());
            newElement.geometry = newElement.slottedHole.data();
            newElement.typeOfElement = eSlottedHoleElement;
        }
        return;
    }case eSphereElement:{
        if(!oldElement.isNull() && !oldElement->getSphere().isNull() && !oldElement->getGeometry().isNull()){
            newElement.sphere = new Sphere(*oldElement->getSphere().data());
            newElement.geometry = newElement.sphere.data();
            newElement.typeOfElement = eSphereElement;
        }
        return;
    }case eTorusElement:{
        if(!oldElement.isNull() && !oldElement->getTorus().isNull() && !oldElement->getGeometry().isNull()){
            newElement.torus = new Torus(*oldElement->getTorus().data());
            newElement.geometry = newElement.torus.data();
            newElement.typeOfElement = eTorusElement;
        }
        return;
    }case eDirectionElement:{
        if(!oldElement.isNull() && !oldElement->getGeometry().isNull()){
            this->copyGeometry(newElement, oldElement, getElementTypeEnum(oldElement->getFeatureTypeEnum()));
        }
        return;
    }case ePositionElement:{
        if(!oldElement.isNull() && !oldElement->getGeometry().isNull()){
            this->copyGeometry(newElement, oldElement, getElementTypeEnum(oldElement->getFeatureTypeEnum()));
        }
        return;
    }case eRadiusElement:{
        if(!oldElement.isNull() && !oldElement->getGeometry().isNull()){
            this->copyGeometry(newElement, oldElement, getElementTypeEnum(oldElement->getFeatureTypeEnum()));
        }
        return;
    }
    }

}
