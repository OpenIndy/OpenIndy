#include "featureupdater.h"

TrafoController FeatureUpdater::trafoControl;

FeatureUpdater *FeatureUpdater::myFeatureUpdater = NULL;

FeatureUpdater::FeatureUpdater(QObject *parent) :
    QObject(parent)
{
}

/*!
 * \brief FeatureUpdater::getInstance
 * \return
 */
FeatureUpdater *FeatureUpdater::getInstance(){
    if(FeatureUpdater::myFeatureUpdater == NULL){
        FeatureUpdater::myFeatureUpdater = new FeatureUpdater();
    }
    return FeatureUpdater::myFeatureUpdater;
}

/*!
 * \brief FeatureUpdater::recalcFeature
 * \param f
 */
void FeatureUpdater::recalcFeature(Feature *f){
    if(f != NULL){
        //recalc needed features
        foreach(FeatureWrapper *neededFeature, f->previouslyNeeded){
            if(neededFeature->getFeature() != NULL && neededFeature->getTrafoParam() == NULL){ //if wrapper contains a feature but not a trafo param
                if(!neededFeature->getFeature()->getIsUpdated()){
                    this->recalcFeature(neededFeature->getFeature());
                }
            }
        }
        f->recalc();
        f->setIsUpdated(true);
        //recalc dependent features
        foreach(FeatureWrapper *dependentFeature, f->usedFor){
            if(dependentFeature->getFeature() != NULL && dependentFeature->getTrafoParam() == NULL){ //if wrapper contains a feature but not a trafo param
                this->recalcFeature(dependentFeature->getFeature());
            }
        }
    }
}

/*!
 * \brief FeatureUpdater::recalcTrafoParam
 * \param tp
 */
void FeatureUpdater::recalcTrafoParam(TrafoParam *tp){
    //if trafo param has "from", "to" and a function
    if(tp != NULL && tp->getStartSystem() != NULL && tp->getDestinationSystem() != NULL && tp->getFunctions().size() > 0
            && tp->getFunctions().at(0) != NULL){

        SystemTransformation *tpFunction = dynamic_cast<SystemTransformation*>(tp->getFunctions().at(0));
        //if function of tp is a system transformation
        if(tpFunction != NULL){

            //clear all copy lists of function
            tpFunction->points_startSystem.clear();
            tpFunction->points_targetSystem.clear();
            tpFunction->lines_startSystem.clear();
            tpFunction->lines_targetSystem.clear();
            tpFunction->planes_startSystem.clear();
            tpFunction->planes_targetSystem.clear();
            tpFunction->spheres_startSystem.clear();
            tpFunction->spheres_targetSystem.clear();
            tpFunction->scalarEntityAngles_startSystem.clear();
            tpFunction->scalarEntityAngles_targetSystem.clear();
            tpFunction->scalarEntityDistances_startSystem.clear();
            tpFunction->scalarEntityDistances_targetSystem.clear();

            if(tp->getStartSystem()->getNominals().size() > 0 && tp->getDestinationSystem()->getNominals().size() > 0){ //if both nominals
                this->fillTrafoParamFunctionNN(tpFunction, tp);
            }else if(tp->getStartSystem()->getNominals().size() == 0 && tp->getDestinationSystem()->getNominals().size() == 0){ //if both actual
                if(tp->getIsMovement()){
                    this->fillTrafoParamFunctionMovement(tpFunction,tp);
                }else{
                    this->fillTrafoParamFunctionAA(tpFunction, tp);
                }
            }else if( (tp->getStartSystem()->getNominals().size() == 0 && tp->getDestinationSystem()->getNominals().size() > 0)
                      || (tp->getStartSystem()->getNominals().size() > 0 && tp->getDestinationSystem()->getNominals().size() == 0) ){ //if one actual one nominal
                this->fillTrafoParamFunctionAN(tpFunction, tp);
            }
        }

        tp->recalc();
        tp->setIsUpdated(true);

        //recalc dependent features
        foreach(FeatureWrapper *dependentFeature, tp->usedFor){
            if(dependentFeature->getFeature() != NULL && dependentFeature->getTrafoParam() == NULL){ //if wrapper contains a feature but not a trafo param
                this->recalcFeature(dependentFeature->getFeature());
            }
        }
    }
}

/*!
 * \brief FeatureUpdater::recalcFeatureSet
 */
void FeatureUpdater::recalcFeatureSet(){
    //set all features to not have been updated
    foreach(FeatureWrapper *feature, OiFeatureState::getFeatures()){
        if(feature != NULL && feature->getFeature() != NULL){
            feature->getFeature()->setIsUpdated(false);
        }
    }
    //recalc recursively
    foreach(FeatureWrapper *feature, OiFeatureState::getFeatures()){
        if(feature != NULL && feature->getFeature() != NULL){
            if(!feature->getFeature()->getIsUpdated() && feature->getTrafoParam() == NULL){
                this->recalcFeature(feature->getFeature());
            }
        }
    }
    emit this->featuresRecalculated();
}

/*!
 * \brief FeatureUpdater::switchCoordinateSystem
 * \param coordinateSystems
 * \param stations
 * \param featureSet
 * \param to
 */
void FeatureUpdater::switchCoordinateSystem(CoordinateSystem *to){
    if(to != NULL){
        //transform all observations to current coordinate system
        foreach(CoordinateSystem* cs, OiFeatureState::getCoordinateSystems()){
            if(cs != NULL){
                //cs->transformObservations(to);
                trafoControl.transformObservations(cs);
            }
        }
        foreach(Station* s, OiFeatureState::getStations()){
            if(s != NULL && s->coordSys != NULL){
                //s->coordSys->transformObservations(to);
                trafoControl.transformObservations(s->coordSys);
            }
        }
        //set isSolved of all nominals whose coordinate system is not "to" to false, otherwise to true
        foreach(FeatureWrapper *feature, OiFeatureState::getFeatures()){
            if(feature->getGeometry() != NULL && feature->getGeometry()->getIsNominal()
                    && feature->getGeometry()->getNominalSystem() != to){
                feature->getGeometry()->setIsSolved(false);
            }else if(feature->getGeometry() != NULL && feature->getGeometry()->getIsNominal()){
                feature->getGeometry()->setIsSolved(true);
            }
        }
        //recalc all features
        this->recalcFeatureSet();
    }
}

/*!
 * \brief FeatureUpdater::switchCoordinateSystemWithoutTransformation
 * \param coordinateSystems
 * \param stations
 * \param featureSet
 * \param to
 */
void FeatureUpdater::switchCoordinateSystemWithoutTransformation(CoordinateSystem *to){
    if(to != NULL){
        //set all observation to target coordinate system
        foreach(CoordinateSystem* cs, OiFeatureState::getCoordinateSystems()){
            if(cs != NULL){
                if(cs == to){ //if cs is the target coordinate system
                    //cs->setObservationState(true);
                    trafoControl.setObservationState(cs,true);
                }else{
                    //cs->setObservationState(false);
                    trafoControl.setObservationState(cs,false);
                }
            }
        }
        foreach(Station* s, OiFeatureState::getStations()){
            if(s != NULL && s->coordSys != NULL){
                if(s->coordSys == to){ //if cs is the target coordinate system
                    //s->coordSys->setObservationState(true);
                    trafoControl.setObservationState(s->coordSys,true);
                }else{
                    //s->coordSys->setObservationState(false);
                    trafoControl.setObservationState(s->coordSys,false);
                }
            }
        }
        //set isSolved of all nominals whose coordinate system is not "to" to false, otherwise to true
        foreach(FeatureWrapper *feature, OiFeatureState::getFeatures()){
            if(feature->getGeometry() != NULL && feature->getGeometry()->getIsNominal()
                    && feature->getGeometry()->getNominalSystem() != to){
                feature->getGeometry()->setIsSolved(false);
            }else if(feature->getGeometry() != NULL && feature->getGeometry()->getIsNominal()){
                feature->getGeometry()->setIsSolved(true);
            }
        }
        //recalc all features
        this->recalcFeatureSet();
    }
}

/*!
 * \brief switchCoordinateSystemWithoutMovement
 * \param from
 * \param to
 */
void FeatureUpdater::switchCoordinateSystemWithoutMovement(CoordinateSystem *to)
{
    foreach (CoordinateSystem *cs, OiFeatureState::getCoordinateSystems()) {
        if(cs != NULL){
            if(cs == to){
                trafoControl.setObservationState(cs,true);
            }else{
                trafoControl.transformObsForMovementCalculation(cs,to);
            }
        }
    }

    foreach (Station *s, OiFeatureState::getStations()) {
        if(s != NULL && s->coordSys != NULL){
            if(s->coordSys == to){
                trafoControl.setObservationState(s->coordSys,true);
            }else{
                trafoControl.transformObsForMovementCalculation(s->coordSys,to);
            }
        }
    }

    //set isSolved of all nominals whose coordinate system is not "to" to false, otherwise to true
    foreach(FeatureWrapper *feature, OiFeatureState::getFeatures()){
        if(feature->getGeometry() != NULL && feature->getGeometry()->getIsNominal()
                && feature->getGeometry()->getNominalSystem() != to){
            feature->getGeometry()->setIsSolved(false);
        }else if(feature->getGeometry() != NULL && feature->getGeometry()->getIsNominal()){
            feature->getGeometry()->setIsSolved(true);
        }
    }
    //recalc all features
    this->recalcFeatureSet();
}

/*!
 * \brief FeatureUpdater::findTrafoParam
 * \param searchTP
 * \param trafoParams
 * \return
 */
TrafoParam* FeatureUpdater::findTrafoParam(CoordinateSystem *searchSystem, QList<TrafoParam *> trafoParams){
    foreach(TrafoParam *tp, trafoParams){
        if(tp->getDestinationSystem() != NULL && tp->getDestinationSystem() == searchSystem){
            return tp;
        }
    }
    return NULL;
}

void FeatureUpdater::recalcAll()
{
    QMap<int,bool> solvedSystems;

    foreach(CoordinateSystem *c, OiFeatureState::getCoordinateSystems()){
        foreach(TrafoParam *t, c->getTransformationParameters()){
            if(!solvedSystems.contains(t->getId())){
               FeatureUpdater::recalcTrafoParam(t);
               solvedSystems.insert(t->getId(),true);
            }
        }
    }

    foreach(Station *s, OiFeatureState::getStations()){
        foreach(TrafoParam *t, s->coordSys->getTransformationParameters()){
            if(!solvedSystems.contains(t->getId())){
               FeatureUpdater::recalcTrafoParam(t);
               solvedSystems.insert(t->getId(),true);
            }
        }
    }

    FeatureUpdater::recalcFeatureSet();

}

/*!
 * \brief FeatureUpdater::fillTrafoParamFunctionNN
 * Fill Trafo Param function with start and target features where start and target system are nominal systems
 * \param function
 * \param tp
 */
void FeatureUpdater::fillTrafoParamFunctionNN(SystemTransformation *function, TrafoParam *tp){
    //sort helper class which compares and sorts the list of start and target points
    SortListByName mySorter;

    //add all points
    foreach(Point *p, function->getPoints()){
        Point cpyP(*p);
        cpyP.setIsSolved(true);
        if(p->getIsNominal() && p->getNominalSystem() == tp->getStartSystem()){
            mySorter.addLocPoint(cpyP);
        }else if(p->getIsNominal() && p->getNominalSystem() == tp->getDestinationSystem()){
            mySorter.addRefPoint(cpyP);
        }
    }
    //add all lines
    foreach(Line *l, function->getLines()){
        Line cpyL(*l);
        cpyL.setIsSolved(true);
        if(l->getIsNominal() && l->getNominalSystem() == tp->getStartSystem()){
            mySorter.addLocLine(cpyL);
        }else if(l->getIsNominal() && l->getNominalSystem() == tp->getDestinationSystem()){
            mySorter.addRefLine(cpyL);
        }
    }
    //add all planes
    foreach(Plane *p, function->getPlanes()){
        Plane cpyP(*p);
        cpyP.setIsSolved(true);
        if(p->getIsNominal() && p->getNominalSystem() == tp->getStartSystem()){
            mySorter.addLocPlane(cpyP);
        }else if(p->getIsNominal() && p->getNominalSystem() == tp->getDestinationSystem()){
            mySorter.addRefPlane(cpyP);
        }
    }
    //add all spheres
    foreach(Sphere *s, function->getSpheres()){
        Sphere cpyS(*s);
        cpyS.setIsSolved(true);
        if(s->getIsNominal() && s->getNominalSystem() == tp->getStartSystem()){
            mySorter.addLocSphere(cpyS);
        }else if(s->getIsNominal() && s->getNominalSystem() == tp->getDestinationSystem()){
            mySorter.addRefSphere(cpyS);
        }
    }
    //add all scalar entity distances
    foreach(ScalarEntityDistance *s, function->getScalarEntityDistances()){
        ScalarEntityDistance cpyS(*s);
        cpyS.setIsSolved(true);
        if(s->getIsNominal() && s->getNominalSystem() == tp->getStartSystem()){
            mySorter.addLocScalarEntityDistance(cpyS);
        }else if(s->getIsNominal() && s->getNominalSystem() == tp->getDestinationSystem()){
            mySorter.addRefScalarEntityDistance(cpyS);
        }
    }
    //add all scalar entity angles
    foreach(ScalarEntityAngle *s, function->getScalarEntityAngles()){
        ScalarEntityAngle cpyS(*s);
        cpyS.setIsSolved(true);
        if(s->getIsNominal() && s->getNominalSystem() == tp->getStartSystem()){
            mySorter.addLocScalarEntityAngle(cpyS);
        }else if(s->getIsNominal() && s->getNominalSystem() == tp->getDestinationSystem()){
            mySorter.addRefScalarEntityAngle(cpyS);
        }
    }

    //add sorted lists to the function
    function->points_startSystem = mySorter.getLocPoints();
    function->points_targetSystem = mySorter.getRefPoints();
    function->lines_startSystem = mySorter.getLocLines();
    function->lines_targetSystem = mySorter.getRefLines();
    function->planes_startSystem = mySorter.getLocPlanes();
    function->planes_targetSystem = mySorter.getRefPlanes();
    function->spheres_startSystem = mySorter.getLocSpheres();
    function->spheres_targetSystem = mySorter.getRefSpheres();
    function->scalarEntityAngles_startSystem = mySorter.getLocScalarEntityAngles();
    function->scalarEntityAngles_targetSystem = mySorter.getRefScalarEntityAngles();
    function->scalarEntityDistances_startSystem = mySorter.getLocScalarEntityDistances();
    function->scalarEntityDistances_targetSystem = mySorter.getRefScalarEntityDistances();
}

/*!
 * \brief FeatureUpdater::fillTrafoParamFunctionAN
 * Fill Trafo Param function with start and target features where one system is actual and the other one is a nominal system
 * \param function
 * \param tp
 */
void FeatureUpdater::fillTrafoParamFunctionAN(SystemTransformation *function, TrafoParam *tp){
    //sort helper class which compares and sorts the list of start and target points
    SortListByName mySorter;

    CoordinateSystem *actualSystem = NULL;
    CoordinateSystem *nominalSystem = NULL;
    if(tp->getStartSystem()->getNominals().size() == 0){ //if "from" system is actual
        actualSystem = tp->getStartSystem();
        nominalSystem = tp->getDestinationSystem();
    }else if(tp->getDestinationSystem()->getNominals().size() == 0){ //if "to" system is actual
        actualSystem = tp->getDestinationSystem();
        nominalSystem = tp->getStartSystem();
    }

    if(actualSystem != NULL && nominalSystem != NULL){

        //TODO switch immer auslösen

        //if coord sys needs to be switched
        if(!actualSystem->getIsActiveCoordinateSystem()){
            this->switchCoordinateSystemWithoutTransformation(actualSystem);
        }

        //add all points
        foreach(Point *p, function->getPoints()){
            if(p->getIsNominal() && p->getNominalSystem() == nominalSystem){
                Point cpyP(*p);
                cpyP.setIsSolved(true);
                mySorter.addLocPoint(cpyP);
            }else if(!p->getIsNominal() && p->getIsSolved()){
                mySorter.addRefPoint(Point(*p));
            }
        }
        //add all lines
        foreach(Line *l, function->getLines()){
            if(l->getIsNominal() && l->getNominalSystem() == nominalSystem){
                Line cpyL(*l);
                cpyL.setIsSolved(true);
                mySorter.addLocLine(cpyL);
            }else if(!l->getIsNominal() && l->getIsSolved()){
                mySorter.addRefLine(Line(*l));
            }
        }
        //add all planes
        foreach(Plane *p, function->getPlanes()){
            if(p->getIsNominal() && p->getNominalSystem() == nominalSystem){
                Plane cpyP(*p);
                cpyP.setIsSolved(true);
                mySorter.addLocPlane(cpyP);
            }else if(!p->getIsNominal() && p->getIsSolved()){
                mySorter.addRefPlane(Plane(*p));
            }
        }
        //add all spheres
        foreach(Sphere *s, function->getSpheres()){
            if(s->getIsNominal() && s->getNominalSystem() == nominalSystem){
                Sphere cpyS(*s);
                cpyS.setIsSolved(true);
                mySorter.addLocSphere(cpyS);
            }else if(!s->getIsNominal() && s->getIsSolved()){
                mySorter.addRefSphere(Sphere(*s));
            }
        }
        //add all scalar entity distances
        foreach(ScalarEntityDistance *s, function->getScalarEntityDistances()){
            if(s->getIsNominal() && s->getNominalSystem() == nominalSystem){
                ScalarEntityDistance cpyS(*s);
                cpyS.setIsSolved(true);
                mySorter.addLocScalarEntityDistance(cpyS);
            }else if(!s->getIsNominal() && s->getIsSolved()){
                mySorter.addRefScalarEntityDistance(ScalarEntityDistance(*s));
            }
        }
        //add all scalar entity angles
        foreach(ScalarEntityAngle *s, function->getScalarEntityAngles()){
            if(s->getIsNominal() && s->getNominalSystem() == nominalSystem){
                ScalarEntityAngle cpyS(*s);
                cpyS.setIsSolved(true);
                mySorter.addLocScalarEntityAngle(cpyS);
            }else if(!s->getIsNominal() && s->getIsSolved()){
                mySorter.addRefScalarEntityAngle(ScalarEntityAngle(*s));
            }
        }

        //add sorted lists to the function
        if(tp->getStartSystem()->getNominals().size() > 0){ //if "from" system is nominal
            function->points_startSystem = mySorter.getLocPoints();
            function->points_targetSystem = mySorter.getRefPoints();
            function->lines_startSystem = mySorter.getLocLines();
            function->lines_targetSystem = mySorter.getRefLines();
            function->planes_startSystem = mySorter.getLocPlanes();
            function->planes_targetSystem = mySorter.getRefPlanes();
            function->spheres_startSystem = mySorter.getLocSpheres();
            function->spheres_targetSystem = mySorter.getRefSpheres();
            function->scalarEntityAngles_startSystem = mySorter.getLocScalarEntityAngles();
            function->scalarEntityAngles_targetSystem = mySorter.getRefScalarEntityAngles();
            function->scalarEntityDistances_startSystem = mySorter.getLocScalarEntityDistances();
            function->scalarEntityDistances_targetSystem = mySorter.getRefScalarEntityDistances();
        }else if(tp->getDestinationSystem()->getNominals().size() > 0){ //if "to" system is nominal
            function->points_startSystem = mySorter.getRefPoints();
            function->points_targetSystem = mySorter.getLocPoints();
            function->lines_startSystem = mySorter.getRefLines();
            function->lines_targetSystem = mySorter.getLocLines();
            function->planes_startSystem = mySorter.getRefPlanes();
            function->planes_targetSystem = mySorter.getLocPlanes();
            function->spheres_startSystem = mySorter.getRefSpheres();
            function->spheres_targetSystem = mySorter.getLocSpheres();
            function->scalarEntityAngles_startSystem = mySorter.getRefScalarEntityAngles();
            function->scalarEntityAngles_targetSystem = mySorter.getLocScalarEntityAngles();
            function->scalarEntityDistances_startSystem = mySorter.getRefScalarEntityDistances();
            function->scalarEntityDistances_targetSystem = mySorter.getLocScalarEntityDistances();
        }

        //if coord sys needs to be re-switched
        if(!actualSystem->getIsActiveCoordinateSystem()){
            this->switchCoordinateSystem(OiFeatureState::getActiveCoordinateSystem());
        }
    }
}

/*!
 * \brief FeatureUpdater::fillTrafoParamFunctionAA
 * Fill Trafo Param function with start and target features where start and target system are actual systems
 * \param function
 * \param tp
 */
void FeatureUpdater::fillTrafoParamFunctionAA(SystemTransformation *function, TrafoParam *tp){
    //sort helper class which compares and sorts the list of start and target points
    SortListByName mySorter;

    //if coord sys needs to be switched to "from" system
    if(!tp->getStartSystem()->getIsActiveCoordinateSystem()){
        this->switchCoordinateSystemWithoutTransformation(tp->getStartSystem());
    }

    //add all points
    foreach(Point *p, function->getPoints()){
        if(!p->getIsNominal() && p->getIsSolved()){
            mySorter.addLocPoint(Point(*p));
        }
    }
    //add all lines
    foreach(Line *l, function->getLines()){
        if(!l->getIsNominal() && l->getIsSolved()){
            mySorter.addLocLine(Line(*l));
        }
    }
    //add all planes
    foreach(Plane *p, function->getPlanes()){
        if(!p->getIsNominal() && p->getIsSolved()){
            mySorter.addLocPlane(Plane(*p));
        }
    }
    //add all spheres
    foreach(Sphere *s, function->getSpheres()){
        if(!s->getIsNominal() && s->getIsSolved()){
            mySorter.addLocSphere(Sphere(*s));
        }
    }
    //add all scalar entity distances
    foreach(ScalarEntityDistance *s, function->getScalarEntityDistances()){
        if(!s->getIsNominal() && s->getIsSolved()){
            mySorter.addLocScalarEntityDistance(ScalarEntityDistance(*s));
        }
    }
    //add all scalar entity angles
    foreach(ScalarEntityAngle *s, function->getScalarEntityAngles()){
        if(!s->getIsNominal() && s->getIsSolved()){
            mySorter.addLocScalarEntityAngle(ScalarEntityAngle(*s));
        }
    }

    //switch to "to" system
    this->switchCoordinateSystemWithoutTransformation(tp->getDestinationSystem());

    //add all points
    foreach(Point *p, function->getPoints()){
        if(!p->getIsNominal() && p->getIsSolved()){
            mySorter.addRefPoint(Point(*p));
        }
    }
    //add all lines
    foreach(Line *l, function->getLines()){
        if(!l->getIsNominal() && l->getIsSolved()){
            mySorter.addRefLine(Line(*l));
        }
    }
    //add all planes
    foreach(Plane *p, function->getPlanes()){
        if(!p->getIsNominal() && p->getIsSolved()){
            mySorter.addRefPlane(Plane(*p));
        }
    }
    //add all spheres
    foreach(Sphere *s, function->getSpheres()){
        if(!s->getIsNominal() && s->getIsSolved()){
            mySorter.addRefSphere(Sphere(*s));
        }
    }
    //add all scalar entity distances
    foreach(ScalarEntityDistance *s, function->getScalarEntityDistances()){
        if(!s->getIsNominal() && s->getIsSolved()){
            mySorter.addRefScalarEntityDistance(ScalarEntityDistance(*s));
        }
    }
    //add all scalar entity angles
    foreach(ScalarEntityAngle *s, function->getScalarEntityAngles()){
        if(!s->getIsNominal() && s->getIsSolved()){
            mySorter.addRefScalarEntityAngle(ScalarEntityAngle(*s));
        }
    }

    //add sorted lists to the function
    function->points_startSystem = mySorter.getLocPoints();
    function->points_targetSystem = mySorter.getRefPoints();
    function->lines_startSystem = mySorter.getLocLines();
    function->lines_targetSystem = mySorter.getRefLines();
    function->planes_startSystem = mySorter.getLocPlanes();
    function->planes_targetSystem = mySorter.getRefPlanes();
    function->spheres_startSystem = mySorter.getLocSpheres();
    function->spheres_targetSystem = mySorter.getRefSpheres();
    function->scalarEntityAngles_startSystem = mySorter.getLocScalarEntityAngles();
    function->scalarEntityAngles_targetSystem = mySorter.getRefScalarEntityAngles();
    function->scalarEntityDistances_startSystem = mySorter.getLocScalarEntityDistances();
    function->scalarEntityDistances_targetSystem = mySorter.getRefScalarEntityDistances();

    //if coord sys needs to be re-switched
    if(!tp->getDestinationSystem()->getIsActiveCoordinateSystem()){
        this->switchCoordinateSystem(OiFeatureState::getActiveCoordinateSystem());
    }
}

/*!
 * \brief fillTrafoParamFunctionMovement fill TrafoParam function start and destination lists.
 * The points have observations from different times. The first time is reference, and a point with obs of this time gets
 * in to the reference list. Another points created from observations with "valid time" of trafo param gets created for the
 * actual system. So the trafo param calculates the transformation from now "valid time" to the first situation.
 * \param function
 * \param tp
 */
void FeatureUpdater::fillTrafoParamFunctionMovement(SystemTransformation *function, TrafoParam *tp)
{
    //sort helper class which compares and sorts the list of start and target points
    SortListByName mySorter;

    //can only be calculated when active coord system is a PART
    this->switchCoordinateSystemWithoutMovement(OiFeatureState::getActiveCoordinateSystem());

    QDateTime startTime;

    //get smallest QDateTime from first point as reference time
    if(function->getPoints().size()>0){
        startTime = function->getPoints().at(0)->getObservations().at(0)->myReading->measuredAt;
    }else{
        return;
    }

    //edit the points and assign them to the right list.
    foreach(Point *p, function->getPoints()){

        foreach (Observation *obs, p->getObservations()) {
            //only obs that are valid in the coord system of the movement
            //and transformed obs to the coord system of the movement
            if(obs->getUseState()){

                if(obs->myReading->measuredAt.time() > startTime.time().addSecs(-180) && //is obs in the time span
                        obs->myReading->measuredAt.time() < startTime.time().addSecs(180)){ //for being a reference obs
                    obs->setIsSolved(true);
                }else{
                    obs->setIsSolved(false);
                }
            }else{
                obs->setIsSolved(false);
            }
        }
        p->recalc(); //recalc points only with obs that are in the reference time span
        if(p->getIsSolved()){ //if point can be recalced
            Point cpyPRef(*p); //create a copy and assign to the reference list
            cpyPRef.setIsSolved(true);
            mySorter.addRefPoint(cpyPRef);
        }

        foreach (Observation *obs, p->getObservations()) {
            if(obs->myStation->coordSys == tp->getStartSystem()){//as actual state use only obs from the current station
                if(obs->myReading->measuredAt.time() > tp->getValidTime().time().addSecs(-180) && //is obs in the time span
                        obs->myReading->measuredAt.time() < tp->getValidTime().time().addSecs(180)){ //for being a actual obs
                    obs->setIsSolved(true);
                }else{
                    obs->setIsSolved(false);
                }
            }else{
                obs->setIsSolved(false);
            }
        }
        p->recalc(); //recalc points only with obs that are in the actual time span
        if(p->getIsSolved()){ //if point can be recalced
            Point cpyPStart(*p); //create a copy and assign to the actual list
            cpyPStart.setIsSolved(true);
            mySorter.addLocPoint(cpyPStart);
        }
    }

    //add sorted lists to the function
    function->points_startSystem = mySorter.getLocPoints();
    function->points_targetSystem = mySorter.getRefPoints();
    function->lines_startSystem = mySorter.getLocLines();
    function->lines_targetSystem = mySorter.getRefLines();
    function->planes_startSystem = mySorter.getLocPlanes();
    function->planes_targetSystem = mySorter.getRefPlanes();
    function->spheres_startSystem = mySorter.getLocSpheres();
    function->spheres_targetSystem = mySorter.getRefSpheres();
    function->scalarEntityAngles_startSystem = mySorter.getLocScalarEntityAngles();
    function->scalarEntityAngles_targetSystem = mySorter.getRefScalarEntityAngles();
    function->scalarEntityDistances_startSystem = mySorter.getLocScalarEntityDistances();
    function->scalarEntityDistances_targetSystem = mySorter.getRefScalarEntityDistances();

    //if coord sys needs to be re-switched
    if(!tp->getStartSystem()->getIsActiveCoordinateSystem()){
        this->switchCoordinateSystem(OiFeatureState::getActiveCoordinateSystem());
    }

    //recalc featureSet, because some observations are disabled. So the feature uses all its observations again now
    this->recalcFeatureSet();
}
