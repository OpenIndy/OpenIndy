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
            feature->getFeature()->setProperty("OI_EXEC_CONTEXT", 1);
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

    trafoParam->setProperty("OI_EXEC_CONTEXT", 2);

    //set up input feature
    if(trafoParam->getStartSystem()->getIsStationSystem() && trafoParam->getDestinationSystem()->getIsStationSystem()){ //actual - actual
        this->setUpTrafoParamActualActual(trafoParam, systemTransformation);
    }else if((trafoParam->getStartSystem()->getIsStationSystem() && !trafoParam->getDestinationSystem()->getIsStationSystem())
             || (!trafoParam->getStartSystem()->getIsStationSystem() && trafoParam->getDestinationSystem()->getIsStationSystem())){ //actual - nominal
        this->setUpTrafoParamActualNominal(trafoParam, systemTransformation);
    }else if((trafoParam->getStartSystem()->getIsBundleSystem() && (!trafoParam->getDestinationSystem()->getIsStationSystem()
                                                                   && !trafoParam->getDestinationSystem()->getIsBundleSystem()))
             || (trafoParam->getDestinationSystem()->getIsBundleSystem() && (!trafoParam->getStartSystem()->getIsStationSystem()
                                                                             && !trafoParam->getStartSystem()->getIsBundleSystem()))){ //bundle - nominal
        this->setUpTrafoParamBundleNominal(trafoParam, systemTransformation);
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
        dependentFeature->getFeature()->setProperty("OI_EXEC_CONTEXT", trafoParam->property("OI_EXEC_CONTEXT"));
        this->recalcFeature(dependentFeature->getFeature());

    }

    //recalculate the whole feature set (because transformation parameters have changed)
    this->switchCoordinateSystem();

}

/*!
 * \brief FeatureUpdater::recalcBundle
 * Recalculates a bundle adjustment
 * \param bundleSystem
 * \return
 */
bool FeatureUpdater::recalcBundle(const QPointer<CoordinateSystem> &bundleSystem){

    //check job
    if(this->currentJob.isNull()){
        return false;
    }

    //check bundle system
    if(bundleSystem.isNull() || bundleSystem->getBundlePlugin().isNull()){
        return false;
    }
    QPointer<BundleAdjustment> bundlePlugin = bundleSystem->getBundlePlugin();

    //clear old bundle results
    this->currentJob->blockSignals(true);
    this->clearBundleResults(bundleSystem);
    this->currentJob->blockSignals(false);

    //check bundle stations
    QList<BundleStation> stations = bundlePlugin->getInputStations();
    if(stations.size() < 2){
        return false;
    }

    //set up input geometries
    QList<BundleStation> inputStations;
    foreach(BundleStation inputStation, stations){

        //get corresponding station feature
        QPointer<FeatureWrapper> station = this->currentJob->getFeatureById(inputStation.id);
        if(station.isNull() || station->getStation().isNull() || station->getStation()->getCoordinateSystem().isNull()){
            continue;
        }
        qDebug() << "FeatureUpdater::recalcBundle station:" <<  station->getStation()->getFeatureName();
        //switch to station system
        this->transformAllObsToDestSystem(station->getStation()->getCoordinateSystem());
        this->recalcFeatureSetWithoutTransformation();

        //add station geometries
        inputStation.geometries.clear();
        QList<QPointer<Geometry> > geometries = station->getStation()->getTargetGeometries();
        foreach(const QPointer<Geometry> &geom, geometries){
            if(!geom.isNull() && geom->getIsCommon() && geom->getIsSolved()){
                BundleGeometry inputGeometry;
                inputGeometry.id = geom->getId();
                inputGeometry.parameters[eUnknownX] = geom->getPosition().getVector().getAt(0);
                inputGeometry.parameters[eUnknownY] = geom->getPosition().getVector().getAt(1);
                inputGeometry.parameters[eUnknownZ] = geom->getPosition().getVector().getAt(2);
                inputStation.geometries.append(inputGeometry);
                qDebug() << "FeatureUpdater::recalcBundle inputGeometry:" <<  geom->getId();
            }
        }
        inputStations.append(inputStation);

    }

    //check input stations
    if(inputStations.size() < 2){
        return false;
    }
    bool hasCommonPoints = false;
    foreach (BundleStation bStation, inputStations) {
        if(bStation.geometries.size() > 2){
            hasCommonPoints = true;
            break;
        }
    }

    if(!hasCommonPoints){
        return false;
    }

    if(QLoggingCategory::defaultCategory()->isDebugEnabled()) {
        foreach(BundleStation bs,  inputStations) {
             qDebug() << "BundleStation.id=" << bs.id;
             foreach(BundleGeometry bg, bs.geometries) {
                 qDebug() << "\tgeometries.id=" << bg.id;
             }
        }
    }

    //set up base station and job
    bundlePlugin->setInputStations(inputStations);
    bundlePlugin->setBaseStation(inputStations.at(0));
    bundlePlugin->setCurrentJob(this->currentJob);

    //calculate bundle
    if(!bundlePlugin->runBundle()){
        this->switchCoordinateSystem();
        return false;
    }

    //save bundle results
    this->currentJob->blockSignals(true);
    this->saveBundleResults(bundleSystem);
    this->currentJob->blockSignals(false);

    //recalculate all features
    this->switchCoordinateSystem();

    //recalc all transformations that include the bundle system
    //except own station-> bundle transformations
    this->recalcAllBundleTrafoDependencies(bundleSystem);
    this->recalcFeatureSet();

    return true;
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

    this->transformObsAndNominals(this->currentJob->getActiveCoordinateSystem());

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
    QObject::connect(this->currentJob.data(), &OiJob::trafoParamParametersChanged, this, &FeatureUpdater::switchCoordinateSystem, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::trafoParamIsUsedChanged, this, &FeatureUpdater::switchCoordinateSystem, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::recalcFeatureSet, this, &FeatureUpdater::recalcFeatureSet, Qt::AutoConnection);

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

        neededFeature->getFeature()->setProperty("OI_EXEC_CONTEXT", feature->property("OI_EXEC_CONTEXT"));
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

        dependentFeature->getFeature()->setProperty("OI_EXEC_CONTEXT", feature->property("OI_EXEC_CONTEXT"));
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
                delete element.geometry.data();
            }
        }
    }
    systemTransformation->inputElementsStartSystem.clear();
    QList<int> destKeys = systemTransformation->inputElementsDestinationSystem.keys();
    foreach(const int &key, destKeys){
        QList<InputElement> destElements = systemTransformation->inputElementsDestinationSystem.value(key);
        foreach(const InputElement &element, destElements){
            if(!element.geometry.isNull()){
                delete element.geometry.data();
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
                delete element.geometry.data();
            }
        }
    }
    systemTransformation->inputElementsStartSystem.clear();
    QList<int> destKeys = systemTransformation->inputElementsDestinationSystem.keys();
    foreach(const int &key, destKeys){
        QList<InputElement> destElements = systemTransformation->inputElementsDestinationSystem.value(key);
        foreach(const InputElement &element, destElements){
            if(!element.geometry.isNull()){
                delete element.geometry.data();
            }
        }
    }
    systemTransformation->inputElementsDestinationSystem.clear();

    //get and check keys
    bool isAlignment;
    QList<int> keys = systemTransformation->getInputElements().keys();
    if(keys.size() <= 0){ //no input elements
        return;
    }else if((keys.size() == 1) || (keys.size() == 2)){ //normal transformation
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
        //bestFit with level
        if(keys.size() == 2){
            //get input elements at the position key
            QList<InputElement> newElements;

            //get input elements at the position key
            QList<InputElement> inputElements = systemTransformation->getInputElements().value(1);
            foreach(const InputElement &element, inputElements){

                //check geometry
                if(element.geometry.isNull() || element.geometry->getFeatureWrapper().isNull()){
                    continue;
                }

                //check if the geometry is an actual and is solved
                if(element.geometry->getIsNominal() || !element.geometry->getIsSolved()){
                    continue;
                }
                Plane plane = *element.geometry->getFeatureWrapper()->getPlane();
                InputElement copyElement(element.id);
                this->copyGeometry(copyElement, element.geometry->getFeatureWrapper(), element.typeOfElement);
                newElements.append(copyElement);
            }
            if(newElements.size() > 0){
                systemTransformation->inputElementsStartSystem.insert(1, newElements);
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
        //bestFit with level
        if(keys.size() == 2){
            //get input elements at the position key
            QList<InputElement> newElements;

            //get input elements at the position key
            QList<InputElement> inputElements = systemTransformation->getInputElements().value(1);
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
                systemTransformation->inputElementsDestinationSystem.insert(1, newElements);
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
                delete element.geometry.data();
            }
        }
    }
    systemTransformation->inputElementsStartSystem.clear();
    QList<int> destKeys = systemTransformation->inputElementsDestinationSystem.keys();
    foreach(const int &key, destKeys){
        QList<InputElement> destElements = systemTransformation->inputElementsDestinationSystem.value(key);
        foreach(const InputElement &element, destElements){
            if(!element.geometry.isNull()){
                delete element.geometry.data();
            }
        }
    }
    systemTransformation->inputElementsDestinationSystem.clear();

    //TODO bool oder aehnliches verwenden und nicht keys

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
                    Point p(true);
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
                    Point p(true);
                    p.setFeatureName(element.point->getFeatureName());
                    p.setPoint(element.point->getPosition());
                    sorter.addRefPoint(p);
                }
            }

        }

    }

    //add sorted lists to the function
    systemTransformation->inputPointsStartSystem = sorter.getLocPoints();
    systemTransformation->inputPointsDestinationSystem = sorter.getRefPoints();

}

/*!
 * \brief FeatureUpdater::setUpTrafoParamBundleNominal
 * \param trafoParam
 * \param systemTransformation
 */
void FeatureUpdater::setUpTrafoParamBundleNominal(const QPointer<TrafoParam> &trafoParam, const QPointer<SystemTransformation> &systemTransformation)
{
    trafoParam->setProperty("OI_EXEC_CONTEXT", 3);
    //delete old copy elements
    systemTransformation->inputPointsStartSystem.clear();
    systemTransformation->inputPointsDestinationSystem.clear();

    QList<int> startKeys = systemTransformation->inputElementsStartSystem.keys();
    foreach (const int &key, startKeys) {
        QList<InputElement> startElements = systemTransformation->inputElementsStartSystem.value(key);
        foreach (const InputElement &element, startElements) {
            if(!element.geometry.isNull()){
                delete element.geometry.data();
            }
        }
    }
    systemTransformation->inputElementsStartSystem.clear();

    QList<int> destKeys = systemTransformation->inputElementsDestinationSystem.keys();
    foreach (const int &key, destKeys) {
        QList<InputElement> destElements = systemTransformation->inputElementsDestinationSystem.value(key);
        foreach (const InputElement &element, destElements) {
            if(!element.geometry.isNull()){
                delete element.geometry.data();
            }
        }
    }
    systemTransformation->inputElementsDestinationSystem.clear();

    //check if this trafo already exists and temporary de activate the transformation
    QList<QPointer<TrafoParam> > existingTrafoParams = this->currentJob->getTransformationParametersList();

    QList<QPointer<TrafoParam> > tempUnsolvedTRafoParam;

    //check if trafo param has the same configuration
    foreach (QPointer<TrafoParam> existingTP, existingTrafoParams) {
        if((existingTP->getStartSystem() == trafoParam->getStartSystem() && existingTP->getDestinationSystem() == trafoParam->getDestinationSystem())
                || (existingTP->getStartSystem() == trafoParam->getDestinationSystem() && existingTP->getDestinationSystem() == trafoParam->getStartSystem())){

            //if it is currently used, set it temporarly to unused
            if(existingTP->getIsUsed()){
                existingTP->setIsUsed(false);
                tempUnsolvedTRafoParam.append(existingTP);
            }
        }
    }

    //get and check keys
    bool isAlignment;
    QList<int> keys = systemTransformation->getInputElements().keys();
    if(keys.size() <= 0){ //no input elements
        return;
    }else if(keys.size() == 1 || keys.size() == 2){ //normal transformation
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
    this->switchCoordinateSystemWithTransformation(trafoParam->getStartSystem());

    //set up input elements for alignment transformations
    if(isAlignment){

        //run through all keys and add start system elements
        QList<int> keys = systemTransformation->getInputElements().keys();
        foreach (const int &key, keys) {

            QList<InputElement> newElements;

            //get input elements at the position key
            QList<InputElement> inputElements = systemTransformation->getInputElements().value(key);
            foreach (const InputElement &element, inputElements) {

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

    //set up input elements for normal transformation
    if(!isAlignment){

        //get input elements at the position 0
        QList<InputElement> inputElements = systemTransformation->getInputElements().value(0);
        foreach (const InputElement &element, inputElements) {

            //add all points
            if(element.typeOfElement == ePointElement && !element.point.isNull()){

                if(!trafoParam->getStartSystem()->getIsStationSystem() && element.point->getIsSolved()){
                    Point p(element.point->getIsNominal());
                    p.setFeatureName(element.point->getFeatureName());
                    p.setPoint(element.point->getPosition());
                    sorter.addLocPoint(p);
                }
            }
        }
        //bestFit with level
        if(keys.size() == 2){
            //get input elements at the position key
            QList<InputElement> newElements;

            //get input elements at the position key
            QList<InputElement> inputElements = systemTransformation->getInputElements().value(1);
            foreach(const InputElement &element, inputElements){

                //check geometry
                if(element.geometry.isNull() || element.geometry->getFeatureWrapper().isNull()){
                    continue;
                }

                //check if the geometry is an actual and is solved
                if(element.geometry->getIsNominal() || !element.geometry->getIsSolved()){
                    continue;
                }
                Plane plane = *element.geometry->getFeatureWrapper()->getPlane();
                InputElement copyElement(element.id);
                this->copyGeometry(copyElement, element.geometry->getFeatureWrapper(), element.typeOfElement);
                newElements.append(copyElement);
            }
            if(newElements.size() > 0){
                systemTransformation->inputElementsStartSystem.insert(1, newElements);
            }
        }
    }

    //#########################
    //set up destination system
    //#########################

    this->switchCoordinateSystemWithTransformation(trafoParam->getDestinationSystem());

    //set up input elements for alignment transformation
    if(isAlignment){

        //run through all keys and add destination system elements
        QList<int> keys = systemTransformation->getInputElements().keys();
        foreach (const int &key, keys) {

            QList<InputElement> newElements;

            //get input elements at the position key
            QList<InputElement> inputElements = systemTransformation->getInputElements().value(key);
            foreach (const InputElement &element, inputElements) {

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

    //set up input elements for normal transformation

    if(!isAlignment){

        //get input elements at the position 0
        QList<InputElement> inputElements = systemTransformation->getInputElements().value(0);
        foreach (const InputElement &element, inputElements) {

            //add all points
            if(element.typeOfElement == ePointElement && !element.point.isNull()){

                if(!trafoParam->getDestinationSystem()->getIsStationSystem() && element.point->getIsSolved()){
                    Point p(element.point->getIsNominal());
                    p.setFeatureName(element.point->getFeatureName());
                    p.setPoint(element.point->getPosition());
                    sorter.addRefPoint(p);
                }
            }
        }
        if(keys.size() == 2){
            //get input elements at the position key
            QList<InputElement> newElements;

            //get input elements at the position key
            QList<InputElement> inputElements = systemTransformation->getInputElements().value(1);
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
                systemTransformation->inputElementsDestinationSystem.insert(1, newElements);
            }
        }
    }

    //#########################
    //reswitch to active system
    //#########################

    //add sorted lists to the function
    systemTransformation->inputPointsStartSystem = sorter.getLocPoints();
    systemTransformation->inputPointsDestinationSystem = sorter.getRefPoints();

    //re activate all existing transformations
    foreach (QPointer<TrafoParam> tp, tempUnsolvedTRafoParam) {
        tp->setIsUsed(true);
    }

    //switch back to active system
    this->switchCoordinateSystem();
}

void FeatureUpdater::transformAllObsToDestSystem(const QPointer<CoordinateSystem> &destinationSystem) {

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //check destination system
    if(destinationSystem.isNull()){
        return;
    }

    //run through all station systems
    foreach(const QPointer<Station> &station, this->currentJob->getStationsList()){

        //check station system
        if(station.isNull() || station->getCoordinateSystem().isNull()){
            continue;
        }


        const bool isSolved = (station->getCoordinateSystem() == destinationSystem);
        //run through all observations of the station system
        foreach(const QPointer<Observation> &obs, station->getCoordinateSystem()->getObservations()){

            //set observation to solved only if it has been measured in the active coordinate system
            if(!obs.isNull()){
                obs->setXYZ(obs->getOriginalXYZ());
                obs->setIJK(obs->getOriginalIJK());
                obs->setIsSolved(isSolved);
            }

        }

    }
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

    this->transformAllObsToDestSystem(destinationSystem);


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

    //set all features to not have been updated before recalcFeatureWithoutTransformation()
    foreach(const QPointer<FeatureWrapper> &feature, this->currentJob->getFeaturesList()){
        if(!feature.isNull() && !feature->getFeature().isNull()){
            feature->getFeature()->blockSignals(true);
            feature->getFeature()->setIsUpdated(false);
            feature->getFeature()->blockSignals(false);
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
 * \brief FeatureUpdater::switchCoordinateSystemWithTransformation
 * \param destinationSystem
 */
void FeatureUpdater::switchCoordinateSystemWithTransformation(const QPointer<CoordinateSystem> &destinationSystem)
{
    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //check destination system
    if(destinationSystem.isNull()){
        return;
    }

    this->transformObsAndNominals(destinationSystem);

    //this->recalcFeatureSet();
}

/*!
 * \brief FeatureUpdater::transformObsAndNominals
 * \param destinationSystem
 */
void FeatureUpdater::transformObsAndNominals(const QPointer<CoordinateSystem> &destinationSystem)
{
    if(destinationSystem.isNull()) {
        return; // nothing to do
    }
    //################################################
    //transform all observations to destination system
    //################################################
    qDebug() << "transformObsAndNominals: destinationSystem=" << destinationSystem->getFeatureName();
    //run through all station systems
    foreach(const QPointer<Station> &station, this->currentJob->getStationsList()){

        //check station system
        if(station.isNull() || station->getCoordinateSystem().isNull()){
            continue;
        }

        //transform all observations of the station system to the Destination system
        this->trafoController.transformObservations(station->getCoordinateSystem(), destinationSystem);
        this->trafoController.transformCoordSystems(station->getCoordinateSystem(), destinationSystem,true);

    }

    this->recalcFeatureSet();

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
                feature->getFeature()->blockSignals(true);
                feature->getGeometry()->setIsSolved(isSolved);
                feature->getFeature()->blockSignals(false);
            }

        }

        //set origin value in current coordinate system
        this->trafoController.transformCoordSystems(system, destinationSystem,false);
    }
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

/*!
 * \brief FeatureUpdater::clearBundleResults
 * \param bundleSystem
 */
void FeatureUpdater::clearBundleResults(const QPointer<CoordinateSystem> &bundleSystem){

    //get and delete nominals in bundle system
    foreach(const QPointer<FeatureWrapper> nominal, bundleSystem->getNominals()){
        if(!nominal.isNull() && !nominal->getGeometry().isNull()){
            delete nominal->getGeometry().data();
            delete nominal.data();
        }
    }

    //get and delete bundle transformations
    QList< QPointer<TrafoParam> > transformations = bundleSystem->getTransformationParameters();
    foreach(const QPointer<TrafoParam> &trafo, transformations){
        if(!trafo.isNull() && trafo->getIsBundle()){
            qDebug() << "FeatureUpdater::clearBundleResults transformation " << trafo->getId();
            //check if trafo is between a "bundle-station" and "bundle" or "bundle" and "PART" system
            if(trafo->getStartSystem() == bundleSystem){

                //if second coordinate system is a bundlestation => delete trafoParam
                if(!trafo->getDestinationSystem()->getStation().isNull()){
                    foreach (BundleStation bStation, bundleSystem->getBundlePlugin()->getInputStations()) {
                        if(bStation.id == trafo->getDestinationSystem()->getStation()->getId()){
                            delete trafo.data();
                            break;
                        }
                    }
                }
            }else if(trafo->getDestinationSystem() == bundleSystem){

                if(!trafo->getStartSystem()->getStation().isNull()){
                    //if second coordinate system is a bundlestation => delete trafoParam
                    foreach (BundleStation bStation, bundleSystem->getBundlePlugin()->getInputStations()) {
                        if(bStation.id == trafo->getStartSystem()->getStation()->getId()){
                            delete trafo.data();
                            break;
                        }
                    }
                }
            }
        }
    }
}

/*!
 * \brief FeatureUpdater::saveBundleResults
 * \param bundleSystem
 */
void FeatureUpdater::saveBundleResults(const QPointer<CoordinateSystem> &bundleSystem){

    //get bundle plugin
    QPointer<BundleAdjustment> plugin = bundleSystem->getBundlePlugin();

    //get bundle results
    QList<BundleGeometry> geometries = plugin->getOutputGeometries();
    QList<BundleTransformation> transformations = plugin->getOutputTransformations();

    //create nominal geometries in bundle system
    //this->createBundleGeometries(geometries, bundleSystem);

    //create bundle transformations
    this->createBundleTransformations(transformations, bundleSystem);

}

/*!
 * \brief FeatureUpdater::createBundleGeometries
 * \param geometries
 * \param bundleSystem
 */
/*void FeatureUpdater::createBundleGeometries(const QList<BundleGeometry> &geometries, const QPointer<CoordinateSystem> &bundleSystem){

    //create common points as nominals in bundle system
    QList<QPointer<FeatureWrapper> > features;
    foreach(const BundleGeometry &geom, geometries){

        //get corresponding actual
        QPointer<FeatureWrapper> actual = this->currentJob->getFeatureById(geom.id);
        if(actual.isNull() || actual->getPoint().isNull()){
            continue;
        }
        QString name = actual->getPoint()->getFeatureName();
        QString group = actual->getPoint()->getGroupName();

        //create point
        QPointer<FeatureWrapper> feature = new FeatureWrapper();
        QPointer<Point> point = new Point(true);
        feature->setPoint(point);

        //set up point attributes
        point->setNominalSystem(bundleSystem);
        point->setFeatureName(name);
        point->setGroupName(group);

        //set up point coordinates
        Position pos;
        double x, y, z;
        x = geom.parameters[eUnknownX];
        y = geom.parameters[eUnknownY];
        z = geom.parameters[eUnknownZ];
        pos.setVector(x, y, z);
        point->setPoint(pos);

        //add feature
        features.append(feature);

    }
    this->currentJob->addFeatures(features);

}*/

/*!
 * \brief FeatureUpdater::createBundleTransformations
 * \param transformations
 * \param bundleSystem
 */
void FeatureUpdater::createBundleTransformations(QList<BundleTransformation> &transformations, const QPointer<CoordinateSystem> &bundleSystem){

    //create transformations
    FeatureAttributes fAttr;
    fAttr.typeOfFeature = eTrafoParamFeature;
    fAttr.group = bundleSystem->getGroupName();
    fAttr.count = 1;
    fAttr.startSystem = bundleSystem->getFeatureName();
    QList<QPointer<FeatureWrapper> > features;
    foreach(const BundleTransformation &transformation, transformations){
        qDebug() << "transformation.id=" << transformation.id;

        //get station
        QPointer<FeatureWrapper> feature = this->currentJob->getFeatureById(transformation.id);
        if(feature.isNull() || feature->getStation().isNull()){
            continue;
        }
        QString stationName = feature->getStation()->getFeatureName();

        qDebug() << "feature->getStation()->getFeatureName() " << stationName;

        //set up name
        fAttr.name = bundleSystem->getFeatureName();
        int index = 1;
        while(!this->currentJob->validateFeatureName(fAttr.name, fAttr.typeOfFeature)){
            fAttr.name = QString("%1_%2").arg(bundleSystem->getFeatureName()).arg(index);
            index++;
        }

        //create trafo param feature
        fAttr.destinationSystem = stationName;
        features.append(this->currentJob->addFeatures(fAttr));

    }

    //check number of created transformations
    if(transformations.size() != features.size()){
        this->currentJob->removeFeatures(features);
        return;
    }

    //set up transformation parameters
    int index = 0;
    foreach(const QPointer<FeatureWrapper> &feature, features){

        //check feature
        if(feature.isNull() || feature->getTrafoParam().isNull()){
            index++;
            continue;
        }
        QPointer<TrafoParam> trafo = feature->getTrafoParam();

        //set up parameters
        QMap<TrafoParamParameters, double> parameters = transformations.at(index).parameters;
        trafo->setUnknownParameters(parameters);
        trafo->setIsUsed(true);
        trafo->setIsDatumTrafo(true);

        index++;

    }

}

/*!
 * \brief FeatureUpdater::recalcAllBundleTrafoDependencies
 * \param bundleSystem
 */
void FeatureUpdater::recalcAllBundleTrafoDependencies(const QPointer<CoordinateSystem> &bundleSystem)
{
    //if bundle system
    if(bundleSystem->getIsBundleSystem()){
        //get all trafo params from job
        QList<QPointer<TrafoParam> > globalTps = this->currentJob->getTransformationParametersList();

        foreach (QPointer<TrafoParam> gTP, globalTps) {
            bool recalc = true;

            if(gTP->getStartSystem() == bundleSystem || gTP->getDestinationSystem() == bundleSystem){
                //and check if they are included in bundle trafo
                foreach (QPointer<TrafoParam> bundleTP, bundleSystem->getTransformationParameters()) {
                    if(gTP->getId() == bundleTP->getId()){
                        recalc = false;
                    }
                }
            }else{
                recalc = false;
            }

            //if not, but they include the bundle system => recalc
            if(recalc){
                gTP->recalc();
            }
        }
    }
}
