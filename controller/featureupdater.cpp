#include "featureupdater.h"

FeatureUpdater::FeatureUpdater(QObject *parent) :
    QObject(parent)
{
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
                if(!neededFeature->getFeature()->isUpdated){
                    this->recalcFeature(neededFeature->getFeature());
                }
            }
        }
        f->recalc();
        f->isUpdated = true;
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
void FeatureUpdater::recalcTrafoParam(TrafoParam *tp, QList<FeatureWrapper *> featureSet,
                                      QList<CoordinateSystem *> coordinateSystems, QList<Station *> stations,
                                      CoordinateSystem *activeCoordSystem){
    //if trafo param has "from", "to" and a function
    if(tp != NULL && tp->from != NULL && tp->to != NULL && tp->functionList.size() > 0
            && tp->functionList.at(0) != NULL){

        SystemTransformation *tpFunction = dynamic_cast<SystemTransformation*>(tp->functionList.at(0));
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

            if(tp->from->nominals.size() > 0 && tp->to->nominals.size() > 0){ //if both nominals
                this->fillTrafoParamFunctionNN(tpFunction, tp);
            }else if(tp->from->nominals.size() == 0 && tp->to->nominals.size() == 0){ //if both actual
                this->fillTrafoParamFunctionAA(tpFunction, tp, featureSet, coordinateSystems, stations, activeCoordSystem);
            }else if( (tp->from->nominals.size() == 0 && tp->to->nominals.size() > 0)
                      || (tp->from->nominals.size() > 0 && tp->to->nominals.size() == 0) ){ //if one actual one nominal
                this->fillTrafoParamFunctionAN(tpFunction, tp, featureSet, coordinateSystems, stations, activeCoordSystem);
            }

        }

        tp->recalc();
        tp->isUpdated = true;

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
 * \param featureSet
 */
void FeatureUpdater::recalcFeatureSet(QList<FeatureWrapper *> featureSet){
    //set all features to not have been updated
    foreach(FeatureWrapper *feature, featureSet){
        if(feature != NULL && feature->getFeature() != NULL){
            feature->getFeature()->isUpdated = false;
        }
    }
    //recalc recursively
    foreach(FeatureWrapper *feature, featureSet){
        if(feature != NULL && feature->getFeature() != NULL){
            if(!feature->getFeature()->isUpdated && feature->getTrafoParam() == NULL){
                this->recalcFeature(feature->getFeature());
            }
        }
    }
}

/*!
 * \brief FeatureUpdater::switchCoordinateSystem
 * \param coordinateSystems
 * \param stations
 * \param featureSet
 * \param to
 */
void FeatureUpdater::switchCoordinateSystem(QList<CoordinateSystem*> coordinateSystems,
                                            QList<Station*> stations,
                                            QList<FeatureWrapper *> featureSet, CoordinateSystem *to){
    if(to != NULL){
        //transform all observations to current coordinate system
        foreach(CoordinateSystem* cs, coordinateSystems){
            if(cs != NULL){
                cs->transformObservations(to);
            }
        }
        foreach(Station* s, stations){
            if(s != NULL && s->coordSys != NULL){
                s->coordSys->transformObservations(to);
            }
        }
        //set isSolved of all nominals whose coordinate system is not "to" to false, otherwise to true
        foreach(FeatureWrapper *feature, featureSet){
            if(feature->getGeometry() != NULL && feature->getGeometry()->isNominal
                    && feature->getGeometry()->myNominalCoordSys != to){
                feature->getGeometry()->isSolved = false;
            }else if(feature->getGeometry() != NULL && feature->getGeometry()->isNominal){
                feature->getGeometry()->isSolved = true;
            }
        }
        //recalc all features
        this->recalcFeatureSet(featureSet);
    }
}

/*!
 * \brief FeatureUpdater::switchCoordinateSystemWithoutTransformation
 * \param coordinateSystems
 * \param stations
 * \param featureSet
 * \param to
 */
void FeatureUpdater::switchCoordinateSystemWithoutTransformation(QList<CoordinateSystem *> coordinateSystems,
                                                                 QList<Station *> stations,
                                                                 QList<FeatureWrapper *> featureSet, CoordinateSystem *to){
    if(to != NULL){
        //set all observation to target coordinate system
        foreach(CoordinateSystem* cs, coordinateSystems){
            if(cs != NULL){
                if(cs == to){ //if cs is the target coordinate system
                    cs->setObservationState(true);
                }else{
                    cs->setObservationState(false);
                }
            }
        }
        foreach(Station* s, stations){
            if(s != NULL && s->coordSys != NULL){
                if(s->coordSys == to){ //if cs is the target coordinate system
                    s->coordSys->setObservationState(true);
                }else{
                    s->coordSys->setObservationState(false);
                }
            }
        }
        //set isSolved of all nominals whose coordinate system is not "to" to false, otherwise to true
        foreach(FeatureWrapper *feature, featureSet){
            if(feature->getGeometry() != NULL && feature->getGeometry()->isNominal
                    && feature->getGeometry()->myNominalCoordSys != to){
                feature->getGeometry()->isSolved = false;
            }else if(feature->getGeometry() != NULL && feature->getGeometry()->isNominal){
                feature->getGeometry()->isSolved = true;
            }
        }
        //recalc all features
        this->recalcFeatureSet(featureSet);
    }
}

/*!
 * \brief FeatureUpdater::findTrafoParam
 * \param searchTP
 * \param trafoParams
 * \return
 */
TrafoParam* FeatureUpdater::findTrafoParam(CoordinateSystem *searchSystem, QList<TrafoParam *> trafoParams){
    foreach(TrafoParam *tp, trafoParams){
        if(tp->to != NULL && tp->to == searchSystem){
            return tp;
        }
    }
    return NULL;
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
        cpyP.isSolved = true;
        if(p->isNominal && p->myNominalCoordSys == tp->from){
            mySorter.addLocPoint(cpyP);
        }else if(p->isNominal && p->myNominalCoordSys == tp->to){
            mySorter.addRefPoint(cpyP);
        }
    }
    //add all lines
    foreach(Line *l, function->getLines()){
        Line cpyL(*l);
        cpyL.isSolved = true;
        if(l->isNominal && l->myNominalCoordSys == tp->from){
            mySorter.addLocLine(cpyL);
        }else if(l->isNominal && l->myNominalCoordSys == tp->to){
            mySorter.addRefLine(cpyL);
        }
    }
    //add all planes
    foreach(Plane *p, function->getPlanes()){
        Plane cpyP(*p);
        cpyP.isSolved = true;
        if(p->isNominal && p->myNominalCoordSys == tp->from){
            mySorter.addLocPlane(cpyP);
        }else if(p->isNominal && p->myNominalCoordSys == tp->to){
            mySorter.addRefPlane(cpyP);
        }
    }
    //add all spheres
    foreach(Sphere *s, function->getSpheres()){
        Sphere cpyS(*s);
        cpyS.isSolved = true;
        if(s->isNominal && s->myNominalCoordSys == tp->from){
            mySorter.addLocSphere(cpyS);
        }else if(s->isNominal && s->myNominalCoordSys == tp->to){
            mySorter.addRefSphere(cpyS);
        }
    }
    //add all scalar entity distances
    foreach(ScalarEntityDistance *s, function->getScalarEntityDistances()){
        ScalarEntityDistance cpyS(*s);
        cpyS.isSolved = true;
        if(s->isNominal && s->myNominalCoordSys == tp->from){
            mySorter.addLocScalarEntityDistance(cpyS);
        }else if(s->isNominal && s->myNominalCoordSys == tp->to){
            mySorter.addRefScalarEntityDistance(cpyS);
        }
    }
    //add all scalar entity angles
    foreach(ScalarEntityAngle *s, function->getScalarEntityAngles()){
        ScalarEntityAngle cpyS(*s);
        cpyS.isSolved = true;
        if(s->isNominal && s->myNominalCoordSys == tp->from){
            mySorter.addLocScalarEntityAngle(cpyS);
        }else if(s->isNominal && s->myNominalCoordSys == tp->to){
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
void FeatureUpdater::fillTrafoParamFunctionAN(SystemTransformation *function, TrafoParam *tp, QList<FeatureWrapper *> featureSet,
                                              QList<CoordinateSystem *> coordinateSystems, QList<Station *> stations, CoordinateSystem *activeCoordSystem){
    //sort helper class which compares and sorts the list of start and target points
    SortListByName mySorter;

    CoordinateSystem *actualSystem = NULL;
    CoordinateSystem *nominalSystem = NULL;
    if(tp->from->nominals.size() == 0){ //if "from" system is actual
        actualSystem = tp->from;
        nominalSystem = tp->to;
    }else if(tp->to->nominals.size() == 0){ //if "to" system is actual
        actualSystem = tp->to;
        nominalSystem = tp->from;
    }

    if(actualSystem != NULL && nominalSystem != NULL){

        //if coord sys needs to be switched
        if(actualSystem != activeCoordSystem){
            this->switchCoordinateSystemWithoutTransformation(coordinateSystems, stations, featureSet, actualSystem);
        }

        //add all points
        foreach(Point *p, function->getPoints()){
            if(p->isNominal && p->myNominalCoordSys == nominalSystem){
                Point cpyP(*p);
                cpyP.isSolved = true;
                mySorter.addLocPoint(cpyP);
            }else if(!p->isNominal && p->isSolved){
                mySorter.addRefPoint(Point(*p));
            }
        }
        //add all lines
        foreach(Line *l, function->getLines()){
            if(l->isNominal && l->myNominalCoordSys == nominalSystem){
                Line cpyL(*l);
                cpyL.isSolved = true;
                mySorter.addLocLine(cpyL);
            }else if(!l->isNominal && l->isSolved){
                mySorter.addRefLine(Line(*l));
            }
        }
        //add all planes
        foreach(Plane *p, function->getPlanes()){
            if(p->isNominal && p->myNominalCoordSys == nominalSystem){
                Plane cpyP(*p);
                cpyP.isSolved = true;
                mySorter.addLocPlane(cpyP);
            }else if(!p->isNominal && p->isSolved){
                mySorter.addRefPlane(Plane(*p));
            }
        }
        //add all spheres
        foreach(Sphere *s, function->getSpheres()){
            if(s->isNominal && s->myNominalCoordSys == nominalSystem){
                Sphere cpyS(*s);
                cpyS.isSolved = true;
                mySorter.addLocSphere(cpyS);
            }else if(!s->isNominal && s->isSolved){
                mySorter.addRefSphere(Sphere(*s));
            }
        }
        //add all scalar entity distances
        foreach(ScalarEntityDistance *s, function->getScalarEntityDistances()){
            if(s->isNominal && s->myNominalCoordSys == nominalSystem){
                ScalarEntityDistance cpyS(*s);
                cpyS.isSolved = true;
                mySorter.addLocScalarEntityDistance(cpyS);
            }else if(!s->isNominal && s->isSolved){
                mySorter.addRefScalarEntityDistance(ScalarEntityDistance(*s));
            }
        }
        //add all scalar entity angles
        foreach(ScalarEntityAngle *s, function->getScalarEntityAngles()){
            if(s->isNominal && s->myNominalCoordSys == nominalSystem){
                ScalarEntityAngle cpyS(*s);
                cpyS.isSolved = true;
                mySorter.addLocScalarEntityAngle(cpyS);
            }else if(!s->isNominal && s->isSolved){
                mySorter.addRefScalarEntityAngle(ScalarEntityAngle(*s));
            }
        }

        //add sorted lists to the function
        if(tp->from->nominals.size() > 0){ //if "from" system is nominal
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
        }else if(tp->to->nominals.size() > 0){ //if "to" system is nominal
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
        if(actualSystem != activeCoordSystem){
            this->switchCoordinateSystem(coordinateSystems, stations, featureSet, activeCoordSystem);
        }
    }
}

/*!
 * \brief FeatureUpdater::fillTrafoParamFunctionAA
 * Fill Trafo Param function with start and target features where start and target system are actual systems
 * \param function
 * \param tp
 */
void FeatureUpdater::fillTrafoParamFunctionAA(SystemTransformation *function, TrafoParam *tp, QList<FeatureWrapper *> featureSet,
                                              QList<CoordinateSystem *> coordinateSystems, QList<Station *> stations, CoordinateSystem *activeCoordSystem){
    //sort helper class which compares and sorts the list of start and target points
    SortListByName mySorter;

    //if coord sys needs to be switched to "from" system
    if(tp->from != activeCoordSystem){
        this->switchCoordinateSystemWithoutTransformation(coordinateSystems, stations, featureSet, tp->from);
    }

    //add all points
    foreach(Point *p, function->getPoints()){
        if(!p->isNominal && p->isSolved){
            mySorter.addLocPoint(Point(*p));
        }
    }
    //add all lines
    foreach(Line *l, function->getLines()){
        if(!l->isNominal && l->isSolved){
            mySorter.addLocLine(Line(*l));
        }
    }
    //add all planes
    foreach(Plane *p, function->getPlanes()){
        if(!p->isNominal && p->isSolved){
            mySorter.addLocPlane(Plane(*p));
        }
    }
    //add all spheres
    foreach(Sphere *s, function->getSpheres()){
        if(!s->isNominal && s->isSolved){
            mySorter.addLocSphere(Sphere(*s));
        }
    }
    //add all scalar entity distances
    foreach(ScalarEntityDistance *s, function->getScalarEntityDistances()){
        if(!s->isNominal && s->isSolved){
            mySorter.addLocScalarEntityDistance(ScalarEntityDistance(*s));
        }
    }
    //add all scalar entity angles
    foreach(ScalarEntityAngle *s, function->getScalarEntityAngles()){
        if(!s->isNominal && s->isSolved){
            mySorter.addLocScalarEntityAngle(ScalarEntityAngle(*s));
        }
    }

    //switch to "to" system
    this->switchCoordinateSystemWithoutTransformation(coordinateSystems, stations, featureSet, tp->to);

    //add all points
    foreach(Point *p, function->getPoints()){
        if(!p->isNominal && p->isSolved){
            mySorter.addRefPoint(Point(*p));
        }
    }
    //add all lines
    foreach(Line *l, function->getLines()){
        if(!l->isNominal && l->isSolved){
            mySorter.addRefLine(Line(*l));
        }
    }
    //add all planes
    foreach(Plane *p, function->getPlanes()){
        if(!p->isNominal && p->isSolved){
            mySorter.addRefPlane(Plane(*p));
        }
    }
    //add all spheres
    foreach(Sphere *s, function->getSpheres()){
        if(!s->isNominal && s->isSolved){
            mySorter.addRefSphere(Sphere(*s));
        }
    }
    //add all scalar entity distances
    foreach(ScalarEntityDistance *s, function->getScalarEntityDistances()){
        if(!s->isNominal && s->isSolved){
            mySorter.addRefScalarEntityDistance(ScalarEntityDistance(*s));
        }
    }
    //add all scalar entity angles
    foreach(ScalarEntityAngle *s, function->getScalarEntityAngles()){
        if(!s->isNominal && s->isSolved){
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
    if(tp->to != activeCoordSystem){
        this->switchCoordinateSystem(coordinateSystems, stations, featureSet, activeCoordSystem);
    }
}

/*!
 * \brief FeatureUpdater::deleteFeature
 * Delete the specified feature with all dependencies
 * \param myFeature
 * \return
 */
bool FeatureUpdater::deleteFeature(FeatureWrapper *myFeature, QList<FeatureWrapper *> featureSet){
    if(myFeature != NULL && myFeature->getFeature() != NULL){

        if(myFeature->getGeometry() != NULL){

        }

    }
    return false;
}
