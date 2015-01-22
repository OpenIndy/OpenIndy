#include "featureupdater.h"

TrafoController FeatureUpdater::trafoControl;

FeatureUpdater::FeatureUpdater(QObject *parent) :
    QObject(parent)
{
}

/*!
 * \brief validateFeatureName checks if feature already exists. returns true if feature does not exist. Otherwise false.
 * \param features
 * \param featureName
 * \param fae
 * \return
 */
bool FeatureUpdater::validateFeatureName(QString featureName, FeatureAttributesExchange fae)
{
    if(featureName.compare("") == 0){
        Console::addLine("Feature name is empty");
        return false;
    }
    switch (fae.featureType) {
    case Configuration::eStationFeature:
        for(int k=0; k<OiFeatureState::getFeatures().size();k++){

            int res = QString::compare(OiFeatureState::getFeatures().at(k)->getFeature()->getFeatureName(),featureName, Qt::CaseSensitive);
            if(res == 0){

                if(OiFeatureState::getFeatures().at(k)->getTypeOfFeature() != fae.featureType){
                    if(OiFeatureState::getFeatures().at(k)->getTypeOfFeature() != Configuration::ePointFeature){
                        Console::addLine(QString("Feature name already exists " + featureName));
                        return false;
                    }
                }
                int r = QString::compare(OiFeatureState::getFeatures().at(k)->getFeature()->getDisplayIsNominal(),"true",Qt::CaseSensitive);
                if(fae.nominal == true && r == 0){
                    if(OiFeatureState::getFeatures().at(k)->getGeometry() != NULL && OiFeatureState::getFeatures().at(k)->getGeometry()->getNominalSystem() == fae.nominalSystem){
                        Console::addLine(QString("Feature name already exists " + featureName));
                        return false;
                    }
                }
                r = QString::compare(OiFeatureState::getFeatures().at(k)->getFeature()->getDisplayIsNominal(),"false",Qt::CaseSensitive);
                if(fae.actual == true && r == 0){
                    Console::addLine(QString("Feature name already exists " + featureName));
                    return false;
                }
            }
        }
        break;
    case Configuration::eCoordinateSystemFeature:
        for(int k = 0; k < OiFeatureState::getFeatures().size(); k++){
            if(OiFeatureState::getFeatures().at(k)->getFeature()->getFeatureName().compare(featureName) == 0){
                Console::addLine(QString("Feature name already exists " + featureName));
                return false;
            }
        }
        break;
    case Configuration::ePointFeature:
        for(int k=0; k<OiFeatureState::getFeatures().size();k++){
            int res = QString::compare(OiFeatureState::getFeatures().at(k)->getFeature()->getFeatureName(),featureName,Qt::CaseSensitive);
            if(res == 0){

                if(OiFeatureState::getFeatures().at(k)->getTypeOfFeature() != fae.featureType){
                    if(OiFeatureState::getFeatures().at(k)->getTypeOfFeature() != Configuration::eStationFeature){
                        Console::addLine(QString("Feature name already exists " + featureName));
                        return false;
                    }
                }
                int r = QString::compare(OiFeatureState::getFeatures().at(k)->getFeature()->getDisplayIsNominal(),"true",Qt::CaseSensitive);
                if(fae.nominal == true && r == 0){
                    if(OiFeatureState::getFeatures().at(k)->getGeometry() != NULL && OiFeatureState::getFeatures().at(k)->getGeometry()->getNominalSystem() == fae.nominalSystem){
                        Console::addLine(QString("Feature name already exists " + featureName));
                        return false;
                    }
                }
                r = QString::compare(OiFeatureState::getFeatures().at(k)->getFeature()->getDisplayIsNominal(),"false",Qt::CaseSensitive);
                if(fae.actual == true && r == 0){
                    Console::addLine(QString("Feature name already exists " + featureName));
                    return false;
                }
            }
        }
        break;
    /*case Configuration::eTrafoParamFeature: //allow TrafoParameters to exist multiple times for same transformation direction
        for(int k=0; k<features.size();k++){
            int res = QString::compare(features.at(k)->getFeature()->name, featureName, Qt::CaseSensitive);
            if( res == 0){
                Console::addLine(QString("Feature name already exists " + featureName));
                return false;
            }
        }

        for(int l=0; l<features.size();l++){
            if(features.at(l)->getTrafoParam() != NULL && features.at(l)->getFeature()->name != fae.name){
                if(features.at(l)->getTrafoParam()->from == fae.startSystem && features.at(l)->getTrafoParam()->to == fae.destSystem){
                    Console::addLine("Transformation parameter for this configuration already exist.");
                    return false;
                }
                if(features.at(l)->getTrafoParam()->from == fae.destSystem && features.at(l)->getTrafoParam()->to == fae.startSystem){
                    Console::addLine("Transformation parameter for this configuration already exist.");
                    return false;
                }
            }
        }
        break;*/
    default:
        for(int k=0; k<OiFeatureState::getFeatures().size();k++){
            int res = QString::compare(OiFeatureState::getFeatures().at(k)->getFeature()->getFeatureName(), featureName, Qt::CaseSensitive);
            if(res == 0){
                if(OiFeatureState::getFeatures().at(k)->getTypeOfFeature() != fae.featureType){
                    Console::addLine(QString("Feature name already exists " + featureName));
                    return false;
                }
                int r = QString::compare(OiFeatureState::getFeatures().at(k)->getFeature()->getDisplayIsNominal(),"true",Qt::CaseSensitive);
                if(fae.nominal == true && r == 0){
                    if(OiFeatureState::getFeatures().at(k)->getGeometry() != NULL && OiFeatureState::getFeatures().at(k)->getGeometry()->getNominalSystem() == fae.nominalSystem){
                        Console::addLine(QString("Feature name already exists " + featureName));
                        return false;
                    }
                }
                r = QString::compare(OiFeatureState::getFeatures().at(k)->getFeature()->getDisplayIsNominal(),"false",Qt::CaseSensitive);
                if(fae.actual == true && r == 0){
                    Console::addLine(QString("Feature name already exists " + featureName));
                    return false;
                }
            }
        }
        break;
    }
    return true;
}

/*!
 * \brief addFeature container function that calls all neccessary functions to create a feature.
 * \param features
 * \param fae
 */
int FeatureUpdater::addFeature(FeatureAttributesExchange fae, MeasurementConfig mConfig)
{
    QStringList featureNames = ConstructFeatureName(fae.name, fae.count);

    for(int i=0; i<featureNames.size();i++){
        /*if(!FeatureUpdater::validateFeatureName(featureNames.at(i),fae)){
            continue;
        }*/

        if(fae.actual){
            FeatureUpdater::createFeature(mConfig,featureNames.at(i),false,fae);
        }
        if(fae.nominal){
            FeatureUpdater::createFeature(MeasurementConfig(),featureNames.at(i),true,fae);
        }

    }
    OiFeatureState::sortFeatures();

    //if a function was selected save it as new default function for the specified feature type
    if(fae.function.compare("") != 0){
        QString functionName, pluginName;
        FeatureUpdater::getFunctionFromComboValue(fae.function, functionName, pluginName);
        SystemDbManager::saveDefaultFunction(fae.featureType, functionName, pluginName);
    }

    return fae.featureType;
}

/*!
 * \brief ConstructFeatureName creates the name of the feature with its name part and the count part.
 * \param name
 * \param count
 * \return
 */
QStringList FeatureUpdater::ConstructFeatureName(QString name, int count)
{
    QStringList featureNames;

    for(int k=0; k<count;k++){
        QString featureName;
        QString letterName;
        int numberName;

        if(k == 0){
            featureName = name;
        }else{
            featureName = name + QString::number(k);
        }

        for (int i=0; i<name.length();i++){
            if(name.at(i).isDigit()){
                letterName = name.left(i);
                numberName = name.mid(i).toInt();
                numberName += k;
                if(numberName == 0){
                    featureName = name;
                }else {
                    featureName = letterName + QString::number(numberName);
                }
                break;
            }
        }
        featureNames.append(featureName);
    }
    return featureNames;
}

/*!
 * \brief sortFeatures sorts nominal features next to their actual feature.
 * \param features
 */
/*void FeatureUpdater::sortFeatures()
{
    for(int i=0; i<OiFeatureState::getFeatureCount();i++){
        if(OiFeatureState::getFeatures().at(i)->getGeometry() != NULL && OiFeatureState::getFeatures().at(i)->getGeometry()->getIsNominal() == false){
            for(int k=0;k<OiFeatureState::getFeatures().size();k++){
                if(OiFeatureState::getFeatures().at(k)->getGeometry() != NULL && OiFeatureState::getFeatures().at(k)->getGeometry()->getIsNominal() == true){
                    if(OiFeatureState::getFeatures().at(i)->getFeature()->getFeatureName().compare(OiFeatureState::getFeatures().at(k)->getFeature()->getFeatureName(),Qt::CaseSensitive)==0){
                        if(!(i ==k-1)){
                            features->insert(i+1,features->at(k));
                            if(i<k){
                                features->removeAt(k+1);
                            }else{
                                i -= 1;
                                features->removeAt(k);
                            }
                            k -= 1;
                        }
                    }
                }
            }
        }else if(features->at(i)->getStation() != NULL){
            for(int j=0;j<features->size();j++){
                if(features->at(j)->getPoint() != NULL && features->at(j)->getPoint()->isNominal == true){
                    if(features->at(i)->getStation()->name.compare(features->at(j)->getPoint()->name,Qt::CaseSensitive)==0){
                        if(!(i ==j-1)){
                            features->insert(i+1,features->at(j));
                            if(i<j){
                                features->removeAt(j+1);
                            }else{
                                i -= 1;
                                features->removeAt(j);
                            }
                            j -= 1;
                        }
                    }
                }
            }
        }
    }
}*/

/*!
 * \brief checkForNominals searches for existing nominals that have been created before the actual feature was created
 */
void FeatureUpdater::checkForNominals(FeatureWrapper *fw)
{
    if(fw->getTypeOfFeature() == Configuration::eStationFeature){
        for(int i=0; i<OiFeatureState::getFeatureCount();i++){
            int res = QString::compare(OiFeatureState::getFeatures().at(i)->getFeature()->getFeatureName(), fw->getFeature()->getFeatureName(), Qt::CaseSensitive);
            if(res == 0 && OiFeatureState::getFeatures().at(i)->getGeometry() != NULL && OiFeatureState::getFeatures().at(i)->getGeometry()->getIsNominal()){
                fw->getStation()->position->addNominal(OiFeatureState::getFeatures().at(i)->getGeometry());
                OiFeatureState::getFeatures().at(i)->getGeometry()->setMyActual(fw->getStation()->position);
            }
        }
    }else{
        for(int i=0; i<OiFeatureState::getFeatures().size();i++){
            int res = QString::compare(OiFeatureState::getFeatures().at(i)->getFeature()->getFeatureName(), fw->getFeature()->getFeatureName(), Qt::CaseSensitive);
            if(res == 0 && OiFeatureState::getFeatures().at(i)->getGeometry() != NULL && OiFeatureState::getFeatures().at(i)->getGeometry()->getIsNominal()){
                fw->getGeometry()->addNominal(OiFeatureState::getFeatures().at(i)->getGeometry());
                OiFeatureState::getFeatures().at(i)->getGeometry()->setMyActual(fw->getGeometry());
            }
        }
    }
}

/*!
 * \brief addNominalToActual adds new created nominal features in the list of its parent actual feature
 */
void FeatureUpdater::addNominalToActual(FeatureWrapper *fw)
{
    for(int i=0; i< OiFeatureState::getFeatures().size();i++){
        int res = QString::compare(OiFeatureState::getFeatures().at(i)->getFeature()->getFeatureName(), fw->getFeature()->getFeatureName(), Qt::CaseSensitive);
        if(res == 0){
            if(fw->getStation() != NULL){
                if(OiFeatureState::getFeatures().at(i)->getGeometry() != NULL && OiFeatureState::getFeatures().at(i)->getGeometry()->getIsNominal() == false){
                    OiFeatureState::getFeatures().at(i)->getGeometry()->addNominal(fw->getStation()->position);
                    fw->getStation()->position->setMyActual(OiFeatureState::getFeatures().at(i)->getGeometry());
                    break;
                }else{
                    if(OiFeatureState::getFeatures().at(i)->getStation() != NULL){
                        OiFeatureState::getFeatures().at(i)->getStation()->position->addNominal(fw->getStation()->position);
                        fw->getStation()->position->setMyActual(OiFeatureState::getFeatures().at(i)->getStation()->position);
                        break;
                    }
                }
            }
            if(fw->getGeometry() != NULL){
                if(OiFeatureState::getFeatures().at(i)->getGeometry() != NULL && OiFeatureState::getFeatures().at(i)->getGeometry()->getIsNominal() == false){
                    OiFeatureState::getFeatures().at(i)->getGeometry()->addNominal(fw->getGeometry());
                    fw->getGeometry()->setMyActual(OiFeatureState::getFeatures().at(i)->getGeometry());
                    break;
                }else{
                    if(OiFeatureState::getFeatures().at(i)->getStation() != NULL){
                        OiFeatureState::getFeatures().at(i)->getStation()->position->addNominal(fw->getGeometry());
                        fw->getGeometry()->setMyActual(OiFeatureState::getFeatures().at(i)->getStation()->position);
                        break;
                    }
                }
            }
        }
    }
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
 * \brief createFeature creates the feature with its attributes and adds it to the referenced lists of the controller class.
 * \param stations
 * \param coordSys
 * \param features
 * \param mConfig
 * \param name
 * \param nominal
 * \param fae
 */
void FeatureUpdater::createFeature(MeasurementConfig mConfig, QString name, bool nominal, FeatureAttributesExchange fae)
{
    Function *defaultFunction = NULL;
    bool functionValid = false;
    QString functionName, pluginName;
    if(fae.function.compare("") != 0){
        defaultFunction = FeatureUpdater::getFunctionFromComboValue(fae.function, functionName, pluginName);
        if(defaultFunction != NULL){
            functionValid = true;
        }
    }

    switch(fae.featureType){
    case Configuration::ePointFeature: {
        Point *tmpPoint = new Point(nominal);

        tmpPoint->setMeasurementConfig(mConfig);
        tmpPoint->setFeatureName(name);
        tmpPoint->setGroupName(fae.group);
        tmpPoint->setCommonState(fae.common);

        if(nominal){
            tmpPoint->setNominalSystem(fae.nominalSystem);
            tmpPoint->setIsSolved(true);
        }else{
            if(functionValid){
                tmpPoint->addFunction(defaultFunction);
            }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setPoint(tmpPoint);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->getNominals().append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }

        break;
    }
    case Configuration::eLineFeature: {
        Line *tmp = new Line(nominal);

        tmp->setMeasurementConfig(mConfig);
        tmp->setFeatureName(name);
        tmp->setGroupName(fae.group);
        tmp->setCommonState(fae.common);

        if(nominal){
            tmp->setNominalSystem(fae.nominalSystem);
            tmp->setIsSolved(true);
        }else{
            if(functionValid){ tmp->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setLine(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }

        break;
    }
    case Configuration::ePlaneFeature:{
        Plane *tmp = new Plane(nominal);

        tmp->setMeasurementConfig(mConfig);
        tmp->setFeatureName(name);
        tmp->setGroupName(fae.group);
        tmp->setCommonState(fae.common);

        if(nominal){
            tmp->setNominalSystem(fae.nominalSystem);
            tmp->setIsSolved(true);
        }else{
            if(functionValid){ tmp->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setPlane(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }

        break;
    }
    case Configuration::eSphereFeature:{
        Sphere *tmp = new Sphere(nominal);

        tmp->setMeasurementConfig(mConfig);
        tmp->setFeatureName(name);
        tmp->setGroupName(fae.group);
        tmp->setCommonState(fae.common);

        if(nominal){
            tmp->setNominalSystem(fae.nominalSystem);
            tmp->setIsSolved(true);
        }else{
            if(functionValid){ tmp->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setSphere(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }

        break;
    }
    case Configuration::eCircleFeature:{
        Circle *tmp = new Circle(nominal);

        tmp->setMeasurementConfig(mConfig);
        tmp->setFeatureName(name);
        tmp->setGroupName(fae.group);
        tmp->setCommonState(fae.common);

        if(nominal){
            tmp->setNominalSystem(fae.nominalSystem);
            tmp->setIsSolved(true);
        }else{
            if(functionValid){ tmp->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setCircle(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }

        break;
    }
    case Configuration::eConeFeature:{
        Cone *tmp = new Cone(nominal);

        tmp->setMeasurementConfig(mConfig);
        tmp->setFeatureName(name);
        tmp->setGroupName(fae.group);
        tmp->setCommonState(fae.common);

        if(nominal){
            tmp->setNominalSystem(fae.nominalSystem);
            tmp->setIsSolved(true);
        }else{
            if(functionValid){ tmp->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setCone(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }

        break;
    }
    case Configuration::eCylinderFeature:{
        Cylinder *tmp = new Cylinder(nominal);

        tmp->setMeasurementConfig(mConfig);
        tmp->setFeatureName(name);
        tmp->setGroupName(fae.group);
        tmp->setCommonState(fae.common);

        if(nominal){
            tmp->setNominalSystem(fae.nominalSystem);
            tmp->setIsSolved(true);
        }else{
            if(functionValid){ tmp->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setCylinder(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }

        break;
    }
    case Configuration::eEllipsoidFeature:{
        Ellipsoid *tmp = new Ellipsoid(nominal);

        tmp->setMeasurementConfig(mConfig);
        tmp->setFeatureName(name);
        tmp->setGroupName(fae.group);
        tmp->setCommonState(fae.common);

        if(nominal){
            tmp->setNominalSystem(fae.nominalSystem);
            tmp->setIsSolved(true);
        }else{
            if(functionValid){ tmp->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setEllipsoid(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }

        break;
    }
    case Configuration::eHyperboloidFeature:{
        Hyperboloid *tmp = new Hyperboloid(nominal);

        tmp->setMeasurementConfig(mConfig);
        tmp->setFeatureName(name);
        tmp->setGroupName(fae.group);
        tmp->setCommonState(fae.common);

        if(nominal){
            tmp->setNominalSystem(fae.nominalSystem);
            tmp->setIsSolved(true);
        }else{
            if(functionValid){ tmp->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setHyperboloid(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }

        break;
    }
    case Configuration::eParaboloidFeature:{
        Paraboloid *tmp = new Paraboloid(nominal);

        tmp->setMeasurementConfig(mConfig);
        tmp->setFeatureName(name);
        tmp->setGroupName(fae.group);
        tmp->setCommonState(fae.common);

        if(nominal){
            tmp->setNominalSystem(fae.nominalSystem);
            tmp->setIsSolved(true);
        }else{
            if(functionValid){ tmp->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setParaboloid(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }

        break;
    }
    case Configuration::ePointCloudFeature:{
        PointCloud *tmp = new PointCloud(nominal);

        tmp->setMeasurementConfig(mConfig);
        tmp->setFeatureName(name);
        tmp->setGroupName(fae.group);
        tmp->setCommonState(fae.common);

        if(nominal){
            tmp->setNominalSystem(fae.nominalSystem);
            tmp->setIsSolved(true);
        }else{
            if(functionValid){ tmp->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setPointCloud(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }

        break;
    }
    case Configuration::eNurbsFeature:{
        Nurbs *tmp = new Nurbs(nominal);

        tmp->setMeasurementConfig(mConfig);
        tmp->setFeatureName(name);
        tmp->setGroupName(fae.group);
        tmp->setCommonState(fae.common);

        if(nominal){
            tmp->setNominalSystem(fae.nominalSystem);
            tmp->setIsSolved(true);
        }else{
            if(functionValid){ tmp->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setNurbs(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }

        break;
    }
    case Configuration::eStationFeature:{
        Station *tmp = new Station(name);

        tmp->setGroupName(fae.group);

        tmp->position->setMeasurementConfig(mConfig);

        tmp->setActiveStationState(false);

        if(nominal){
            tmp->setIsSolved(true);
        }else{
            if(functionValid){ tmp->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setStation(tmp);

        if(nominal){
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    //stations->append(fw->getStation());
                    break;
                }
            }
            addNominalToActual(fw);
        }else{
            OiFeatureState::addFeature(fw);
            //stations->append(fw->getStation());
            checkForNominals(fw);
        }

        break;
    }
    case Configuration::eCoordinateSystemFeature:{
        CoordinateSystem *tmp = new CoordinateSystem();

        tmp->setFeatureName(name);
        tmp->setGroupName(fae.group);

        if(functionValid){ tmp->addFunction(defaultFunction); }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setCoordinateSystem(tmp);

        OiFeatureState::addFeature(fw);
        //coordSys->append(fw->getCoordinateSystem());

        break;
    }
    case Configuration::eTrafoParamFeature:{
        TrafoParam *trafopara = new TrafoParam();

        trafopara->setFeatureName(name);
        trafopara->setIsSolved(false);
        trafopara->setCoordinateSystems(fae.startSystem, fae.destSystem);
        trafopara->setIsMovement(fae.isMovement);

        if(functionValid){ trafopara->addFunction(defaultFunction); }

        for(int i=0; i<OiFeatureState::getCoordinateSystems().size();i++){
            if(OiFeatureState::getCoordinateSystems().at(i) == fae.startSystem){
                OiFeatureState::getCoordinateSystems().at(i)->addTransformationParameter(trafopara);
            }
            if(OiFeatureState::getCoordinateSystems().at(i) == fae.destSystem){
                OiFeatureState::getCoordinateSystems().at(i)->addTransformationParameter(trafopara);
            }
        }
        for(int i=0; i<OiFeatureState::getStations().size();i++){
            if(OiFeatureState::getStations().at(i)->coordSys == fae.startSystem){
                OiFeatureState::getStations().at(i)->coordSys->addTransformationParameter(trafopara);
            }
            if(OiFeatureState::getStations().at(i)->coordSys == fae.destSystem){
                OiFeatureState::getStations().at(i)->coordSys->addTransformationParameter(trafopara);
            }
        }


        FeatureWrapper *fw = new FeatureWrapper();
        fw->setTrafoParam(trafopara);

        OiFeatureState::addFeature(fw);
        break;
    }
    case Configuration::eScalarEntityAngleFeature:{
        ScalarEntityAngle *tmpSEAngle = new ScalarEntityAngle(nominal);

        tmpSEAngle->setMeasurementConfig(mConfig);
        tmpSEAngle->setFeatureName(name);
        tmpSEAngle->setGroupName(fae.group);
        tmpSEAngle->setCommonState(fae.common);

        if(nominal){
            tmpSEAngle->setNominalSystem(fae.nominalSystem);
            tmpSEAngle->setIsSolved(true);
        }else{
            if(functionValid){ tmpSEAngle->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setScalarEntityAngle(tmpSEAngle);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }

        break;
    }
    case Configuration::eScalarEntityDistanceFeature:{
        ScalarEntityDistance *tmpSEDistance = new ScalarEntityDistance(nominal);

        tmpSEDistance->setMeasurementConfig(mConfig);
        tmpSEDistance->setFeatureName(name);
        tmpSEDistance->setGroupName(fae.group);
        tmpSEDistance->setCommonState(fae.common);

        if(nominal){
            tmpSEDistance->setNominalSystem(fae.nominalSystem);
            tmpSEDistance->setIsSolved(true);
        }else{
            if(functionValid){ tmpSEDistance->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setScalarEntityDistance(tmpSEDistance);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }
        break;
    }
    case Configuration::eScalarEntityTemperatureFeature:{
        ScalarEntityTemperature *tmpSETemperature = new ScalarEntityTemperature(nominal);

        tmpSETemperature->setMeasurementConfig(mConfig);
        tmpSETemperature->setFeatureName(name);
        tmpSETemperature->setGroupName(fae.group);
        tmpSETemperature->setCommonState(fae.common);

        if(nominal){
            tmpSETemperature->setNominalSystem(fae.nominalSystem);
            tmpSETemperature->setIsSolved(true);
        }else{
            if(functionValid){ tmpSETemperature->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setScalarEntityTemperature(tmpSETemperature);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }

        break;
    }
    case Configuration::eScalarEntityMeasurementSeriesFeature:{
        ScalarEntityMeasurementSeries *tmpSEMSeries = new ScalarEntityMeasurementSeries(nominal);

        tmpSEMSeries->setMeasurementConfig(mConfig);
        tmpSEMSeries->setFeatureName(name);
        tmpSEMSeries->setGroupName(fae.group);
        tmpSEMSeries->setCommonState(fae.common);

        if(nominal){
            tmpSEMSeries->setNominalSystem(fae.nominalSystem);
            tmpSEMSeries->setIsSolved(true);
        }else{
            if(functionValid){ tmpSEMSeries->addFunction(defaultFunction); }
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setScalarEntityMeasurementSeries(tmpSEMSeries);

        if(nominal){ //if nominal feature
            for(int i=0; i< OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == fae.nominalSystem){
                    //OiFeatureState::getCoordinateSystems().at(i)->nominals.append(fw->getGeometry());
                    OiFeatureState::addFeature(fw);
                    break;
                }
            }
            addNominalToActual(fw);
        }else{ //if no nominal feature
            OiFeatureState::addFeature(fw);
            checkForNominals(fw);
        }

        break;
    }
    default:
        break;
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

/*!
 * \brief FeatureUpdater::deleteFeature
 * Delete the specified feature with all dependencies
 * \param myFeature
 * \return
 */
/*bool FeatureUpdater::deleteFeature(FeatureWrapper *myFeatureWrapper){
    if(myFeatureWrapper != NULL && myFeatureWrapper->getFeature() != NULL){
        Feature *myFeature = myFeatureWrapper->getFeature();
        delete myFeature;
        return true;
    }
    return false;
}*/

/*!
 * \brief FeatureUpdater::getFunctionFromComboValue
 * Helper function that parses a special string into function and plugin name and queries the corresponding function plugin.
 * String format: <FunctionName> [<PluginName>]
 * \param functionValue
 * \param functionName
 * \param pluginName
 * \return
 */
Function* FeatureUpdater::getFunctionFromComboValue(QString functionValue, QString &functionName, QString &pluginName){
    QStringList split1 = functionValue.split('[');
    if(split1.size() >= 2){
        QStringList split2 = split1.at(split1.size()-1).split(']');
        if(split2.size() == 2){

            //get function name and plugin name
            functionName = "";
            for(int i = 0; i < split1.size()-1; i++){
                functionName.append(split1.at(i));
            }
            functionName.truncate(functionName.size()-1);
            pluginName = split2.at(0);

            //get instance of the function defined by its name and the plugin name
            QString filePath = SystemDbManager::getPluginFilePath(functionName, pluginName);
            Function *checkFunction = PluginLoader::loadFunctionPlugin(filePath, functionName);

            return checkFunction;

        }
    }
    return NULL;
}
