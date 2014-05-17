#include "featureupdater.h"

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
bool FeatureUpdater::validateFeatureName(QList<FeatureWrapper *> features, QString featureName, FeatureAttributesExchange fae)
{
    if(featureName.compare("") == 0){
        Console::addLine("Feature name is empty");
        return false;
    }
    switch (fae.featureType) {
    case Configuration::eStationFeature:
        for(int k=0; k<features.size();k++){

            int res = QString::compare(features.at(k)->getFeature()->name,featureName, Qt::CaseSensitive);
            if(res == 0){

                if(features.at(k)->getTypeOfFeature() != fae.featureType){
                    if(features.at(k)->getTypeOfFeature() != Configuration::ePointFeature){
                        Console::addLine(QString("Feature name already exists " + featureName));
                        return false;
                    }
                }
                int r = QString::compare(features.at(k)->getFeature()->getDisplayIsNominal(),"true",Qt::CaseSensitive);
                if(fae.nominal == true && r == 0){
                    if(features.at(k)->getGeometry() != NULL && features.at(k)->getGeometry()->myNominalCoordSys == fae.nominalSystem){
                        Console::addLine(QString("Feature name already exists " + featureName));
                        return false;
                    }
                }
                r = QString::compare(features.at(k)->getFeature()->getDisplayIsNominal(),"false",Qt::CaseSensitive);
                if(fae.actual == true && r == 0){
                    Console::addLine(QString("Feature name already exists " + featureName));
                    return false;
                }
            }
        }
        break;
    case Configuration::eCoordinateSystemFeature:
        for(int k = 0; k < features.size(); k++){
            if(features.at(k)->getFeature()->name.compare(featureName) == 0){
                Console::addLine(QString("Feature name already exists " + featureName));
                return false;
            }
        }
        break;
    case Configuration::ePointFeature:
        for(int k=0; k<features.size();k++){
            int res = QString::compare(features.at(k)->getFeature()->name,featureName,Qt::CaseSensitive);
            if(res == 0){

                if(features.at(k)->getTypeOfFeature() != fae.featureType){
                    if(features.at(k)->getTypeOfFeature() != Configuration::eStationFeature){
                        Console::addLine(QString("Feature name already exists " + featureName));
                        return false;
                    }
                }
                int r = QString::compare(features.at(k)->getFeature()->getDisplayIsNominal(),"true",Qt::CaseSensitive);
                if(fae.nominal == true && r == 0){
                    if(features.at(k)->getGeometry() != NULL && features.at(k)->getGeometry()->myNominalCoordSys == fae.nominalSystem){
                        Console::addLine(QString("Feature name already exists " + featureName));
                        return false;
                    }
                }
                r = QString::compare(features.at(k)->getFeature()->getDisplayIsNominal(),"false",Qt::CaseSensitive);
                if(fae.actual == true && r == 0){
                    Console::addLine(QString("Feature name already exists " + featureName));
                    return false;
                }
            }
        }
        break;
    /*case Configuration::eTrafoParamFeature:
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
        for(int k=0; k<features.size();k++){
            int res = QString::compare(features.at(k)->getFeature()->name, featureName, Qt::CaseSensitive);
            if(res == 0){
                if(features.at(k)->getTypeOfFeature() != fae.featureType){
                    Console::addLine(QString("Feature name already exists " + featureName));
                    return false;
                }
                int r = QString::compare(features.at(k)->getFeature()->getDisplayIsNominal(),"true",Qt::CaseSensitive);
                if(fae.nominal == true && r == 0){
                    if(features.at(k)->getGeometry() != NULL && features.at(k)->getGeometry()->myNominalCoordSys == fae.nominalSystem){
                        Console::addLine(QString("Feature name already exists " + featureName));
                        return false;
                    }
                }
                r = QString::compare(features.at(k)->getFeature()->getDisplayIsNominal(),"false",Qt::CaseSensitive);
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
int FeatureUpdater::addFeature(QList<Station*>&stations,QList<CoordinateSystem*>&coordSys, QList<FeatureWrapper*> &features, FeatureAttributesExchange fae, MeasurementConfig mConfig)
{
    QStringList featureNames = ConstructFeatureName(fae.name, fae.count);

    for(int i=0; i<featureNames.size();i++){
        if(!FeatureUpdater::validateFeatureName(features,featureNames.at(i),fae)){
            continue;
        }

        if(fae.actual){
            FeatureUpdater::createFeature(stations,coordSys,features,mConfig,featureNames.at(i),false,fae);
        }
        if(fae.nominal){
            FeatureUpdater::createFeature(stations,coordSys,features,mConfig,featureNames.at(i),true,fae);
        }

    }
    sortFeatures(features);

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
void FeatureUpdater::sortFeatures(QList<FeatureWrapper *> &features)
{
    for(int i=0; i<features.size();i++){
        if(features.at(i)->getGeometry() != NULL && features.at(i)->getGeometry()->isNominal == false){
            for(int k=0;k<features.size();k++){
                if(features.at(k)->getGeometry() != NULL && features.at(k)->getGeometry()->isNominal == true){
                    if(features.at(i)->getFeature()->name.compare(features.at(k)->getFeature()->name,Qt::CaseSensitive)==0){
                        if(!(i ==k-1)){
                            features.insert(i+1,features.at(k));
                            if(i<k){
                                features.removeAt(k+1);
                            }else{
                                i -= 1;
                                features.removeAt(k);
                            }
                            k -= 1;
                        }
                    }
                }
            }
        }else if(features.at(i)->getStation() != NULL){
            for(int j=0;j<features.size();j++){
                if(features.at(j)->getPoint() != NULL && features.at(j)->getPoint()->isNominal == true){
                    if(features.at(i)->getStation()->name.compare(features.at(j)->getPoint()->name,Qt::CaseSensitive)==0){
                        if(!(i ==j-1)){
                            features.insert(i+1,features.at(j));
                            if(i<j){
                                features.removeAt(j+1);
                            }else{
                                i -= 1;
                                features.removeAt(j);
                            }
                            j -= 1;
                        }
                    }
                }
            }
        }
    }
}

/*!
 * \brief checkForNominals searches for existing nominals that have been created before the actual feature was created
 * \param features
 * \param fw
 */
void FeatureUpdater::checkForNominals(QList<FeatureWrapper *> &features, FeatureWrapper *fw)
{
    if(fw->getTypeOfFeature() == Configuration::eStationFeature){
        for(int i=0; i<features.size();i++){
            int res = QString::compare(features.at(i)->getFeature()->name, fw->getFeature()->name, Qt::CaseSensitive);
            if(res == 0 && features.at(i)->getGeometry() != NULL && features.at(i)->getGeometry()->isNominal){
                fw->getStation()->position->nominals.append(features.at(i)->getGeometry());
                features.at(i)->getGeometry()->myActual = fw->getStation()->position;
            }
        }
    }else{
        for(int i=0; i<features.size();i++){
            int res = QString::compare(features.at(i)->getFeature()->name, fw->getFeature()->name, Qt::CaseSensitive);
            if(res == 0 && features.at(i)->getGeometry() != NULL && features.at(i)->getGeometry()->isNominal){
                fw->getGeometry()->nominals.append(features.at(i)->getGeometry());
                features.at(i)->getGeometry()->myActual = fw->getGeometry();
            }
        }
    }
}

/*!
 * \brief addNominalToActual adds new created nominal features in the list of its parent actual feature
 * \param features
 * \param fw
 */
void FeatureUpdater::addNominalToActual(QList<FeatureWrapper *> &features, FeatureWrapper *fw)
{
    for(int i=0; i< features.size();i++){
        int res = QString::compare(features.at(i)->getFeature()->name, fw->getFeature()->name, Qt::CaseSensitive);
        if(res == 0){
            if(fw->getStation() != NULL){
                if(features.at(i)->getGeometry() != NULL && features.at(i)->getGeometry()->isNominal == false){
                    features.at(i)->getGeometry()->nominals.append(fw->getStation()->position);
                    fw->getStation()->position->myActual = features.at(i)->getGeometry();
                    break;
                }else{
                    if(features.at(i)->getStation() != NULL){
                        features.at(i)->getStation()->position->nominals.append(fw->getStation()->position);
                        fw->getStation()->position->myActual = features.at(i)->getStation()->position;
                        break;
                    }
                }
            }
            if(fw->getGeometry() != NULL){
                if(features.at(i)->getGeometry() != NULL && features.at(i)->getGeometry()->isNominal == false){
                    features.at(i)->getGeometry()->nominals.append(fw->getGeometry());
                    fw->getGeometry()->myActual = features.at(i)->getGeometry();
                    break;
                }else{
                    if(features.at(i)->getStation() != NULL){
                        features.at(i)->getStation()->position->nominals.append(fw->getGeometry());
                        fw->getGeometry()->myActual = features.at(i)->getStation()->position;
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
 * \brief createFeature creates the feature with its attributes and adds it to the referenced lists of the controller class.
 * \param stations
 * \param coordSys
 * \param features
 * \param mConfig
 * \param name
 * \param nominal
 * \param fae
 */
void FeatureUpdater::createFeature(QList<Station*>&stations,QList<CoordinateSystem *> &coordSys, QList<FeatureWrapper *> &features,
                                   MeasurementConfig mConfig, QString name, bool nominal, FeatureAttributesExchange fae)
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
        Point *tmpPoint = new Point();

        tmpPoint->mConfig = mConfig;
        tmpPoint->name = name;
        tmpPoint->group = fae.group;
        tmpPoint->isNominal = nominal;
        tmpPoint->isCommon = fae.common;
        tmpPoint->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmpPoint->addFunction(defaultFunction); }

        if(nominal){
            tmpPoint->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setPoint(tmpPoint);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
        }

        break;
    }
    case Configuration::eLineFeature: {
        Line *tmp = new Line();

        tmp->mConfig = mConfig;
        tmp->name = name;
        tmp->group = fae.group;
        tmp->isNominal = nominal;
        tmp->isCommon = fae.common;
        tmp->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmp->addFunction(defaultFunction); }

        if(nominal){
            tmp->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setLine(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
        }

        break;
    }
    case Configuration::ePlaneFeature:{
        Plane *tmp = new Plane();

        tmp->mConfig = mConfig;
        tmp->name = name;
        tmp->group = fae.group;
        tmp->isNominal = nominal;
        tmp->isCommon = fae.common;
        tmp->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmp->addFunction(defaultFunction); }

        if(nominal){
            tmp->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setPlane(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
        }

        break;
    }
    case Configuration::eSphereFeature:{
        Sphere *tmp = new Sphere();

        tmp->mConfig = mConfig;
        tmp->name = name;
        tmp->group = fae.group;
        tmp->isNominal = nominal;
        tmp->isCommon = fae.common;
        tmp->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmp->addFunction(defaultFunction); }

        if(nominal){
            tmp->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setSphere(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
        }

        break;
    }
    case Configuration::eCircleFeature:{
        Circle *tmp = new Circle();

        tmp->mConfig = mConfig;
        tmp->name = name;
        tmp->group = fae.group;
        tmp->isNominal = nominal;
        tmp->isCommon = fae.common;
        tmp->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmp->addFunction(defaultFunction); }

        if(nominal){
            tmp->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setCircle(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
        }

        break;
    }
    case Configuration::eConeFeature:{
        Cone *tmp = new Cone();

        tmp->mConfig = mConfig;
        tmp->name = name;
        tmp->group = fae.group;
        tmp->isNominal = nominal;
        tmp->isCommon = fae.common;
        tmp->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmp->addFunction(defaultFunction); }

        if(nominal){
            tmp->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setCone(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
        }

        break;
    }
    case Configuration::eCylinderFeature:{
        Cylinder *tmp = new Cylinder();

        tmp->mConfig = mConfig;
        tmp->name = name;
        tmp->group = fae.group;
        tmp->isNominal = nominal;
        tmp->isCommon = fae.common;
        tmp->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmp->addFunction(defaultFunction); }

        if(nominal){
            tmp->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setCylinder(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
        }

        break;
    }
    case Configuration::eEllipsoidFeature:{
        Ellipsoid *tmp = new Ellipsoid();

        tmp->mConfig = mConfig;
        tmp->name = name;
        tmp->group = fae.group;
        tmp->isNominal = nominal;
        tmp->isCommon = fae.common;
        tmp->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmp->addFunction(defaultFunction); }

        if(nominal){
            tmp->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setEllipsoid(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
        }

        break;
    }
    case Configuration::eHyperboloidFeature:{
        Hyperboloid *tmp = new Hyperboloid();

        tmp->mConfig = mConfig;
        tmp->name = name;
        tmp->group = fae.group;
        tmp->isNominal = nominal;
        tmp->isCommon = fae.common;
        tmp->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmp->addFunction(defaultFunction); }

        if(nominal){
            tmp->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setHyperboloid(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
        }

        break;
    }
    case Configuration::eParaboloidFeature:{
        Paraboloid *tmp = new Paraboloid();

        tmp->mConfig = mConfig;
        tmp->name = name;
        tmp->group = fae.group;
        tmp->isNominal = nominal;
        tmp->isCommon = fae.common;
        tmp->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmp->addFunction(defaultFunction); }

        if(nominal){
            tmp->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setParaboloid(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
        }

        break;
    }
    case Configuration::ePointCloudFeature:{
        PointCloud *tmp = new PointCloud();

        tmp->mConfig = mConfig;
        tmp->name = name;
        tmp->group = fae.group;
        tmp->isNominal = nominal;
        tmp->isCommon = fae.common;
        tmp->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmp->addFunction(defaultFunction); }

        if(nominal){
            tmp->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setPointCloud(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
        }

        break;
    }
    case Configuration::eNurbsFeature:{
        Nurbs *tmp = new Nurbs();

        tmp->mConfig = mConfig;
        tmp->name = name;
        tmp->group = fae.group;
        tmp->isNominal = nominal;
        tmp->isCommon = fae.common;
        tmp->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmp->addFunction(defaultFunction); }

        if(nominal){
            tmp->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setNurbs(tmp);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
        }

        break;
    }
    case Configuration::eStationFeature:{
        Station *tmp = new Station(name);

        tmp->group = fae.group;

        tmp->position->mConfig = mConfig;
        tmp->position->isNominal = nominal;
        tmp->position->isCommon = fae.common;
        tmp->position->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmp->addFunction(defaultFunction); }

        if(nominal){
            tmp->position->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setStation(tmp);

        if(nominal){
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    stations.append(fw->getStation());
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{
            features.append(fw);
            stations.append(fw->getStation());
            checkForNominals(features,fw);
        }

        break;
    }
    case Configuration::eCoordinateSystemFeature:{
        CoordinateSystem *tmp = new CoordinateSystem();

        tmp->name = name;
        tmp->group = fae.group;
        if(functionValid){ tmp->addFunction(defaultFunction); }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setCoordinateSystem(tmp);

        features.append(fw);
        coordSys.append(fw->getCoordinateSystem());

        break;
    }
    case Configuration::eTrafoParamFeature:{
        TrafoParam *trafopara = new TrafoParam();

        trafopara->name = name;
        trafopara->isSolved = false;
        trafopara->from = fae.startSystem;
        trafopara->to = fae.destSystem;
        if(functionValid){ trafopara->addFunction(defaultFunction); }

        //fae.startSystem->trafoParams.append(trafopara);
        //fae.destSystem->trafoParams.append(trafopara);

        //TODO check with debug if it works !!!!!!!!
        for(int i=0; i<coordSys.size();i++){
            if(coordSys.at(i) == fae.startSystem){
                coordSys.at(i)->trafoParams.append(trafopara);
            }
            if(coordSys.at(i) == fae.destSystem){
                coordSys.at(i)->trafoParams.append(trafopara);
            }
        }


        FeatureWrapper *fw = new FeatureWrapper();
        fw->setTrafoParam(trafopara);

        features.append(fw);
        break;
    }
    case Configuration::eScalarentityAngleFeature:{
        ScalarEntityAngle *tmpSEAngle = new ScalarEntityAngle();

        tmpSEAngle->mConfig = mConfig;
        tmpSEAngle->name = name;
        tmpSEAngle->group = fae.group;
        tmpSEAngle->isNominal = nominal;
        tmpSEAngle->isCommon = fae.common;
        tmpSEAngle->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmpSEAngle->addFunction(defaultFunction); }

        if(nominal){
            tmpSEAngle->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setScalarEntityAngle(tmpSEAngle);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
        }

        break;
    }
    case Configuration::eScalarEntityDistanceFeature:{
        ScalarEntityDistance *tmpSEDistance = new ScalarEntityDistance();

        tmpSEDistance->mConfig = mConfig;
        tmpSEDistance->name = name;
        tmpSEDistance->group = fae.group;
        tmpSEDistance->isNominal = nominal;
        tmpSEDistance->isCommon = fae.common;
        tmpSEDistance->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmpSEDistance->addFunction(defaultFunction); }
        tmpSEDistance->isSolved = true;

        if(nominal){
            tmpSEDistance->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setScalarEntityDistance(tmpSEDistance);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
        }
        break;
    }
    case Configuration::eScalarEntityTemperatureFeature:{
        ScalarEntityTemperature *tmpSETemperature = new ScalarEntityTemperature();

        tmpSETemperature->mConfig = mConfig;
        tmpSETemperature->name = name;
        tmpSETemperature->group = fae.group;
        tmpSETemperature->isNominal = nominal;
        tmpSETemperature->isCommon = fae.common;
        tmpSETemperature->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmpSETemperature->addFunction(defaultFunction); }
        tmpSETemperature->isSolved = true;

        if(nominal){
            tmpSETemperature->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setScalarEntityTemperature(tmpSETemperature);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
        }

        break;
    }
    case Configuration::eScalarEntityMeasurementSeriesFeature:{
        ScalarEntityMeasurementSeries *tmpSEMSeries = new ScalarEntityMeasurementSeries();

        tmpSEMSeries->mConfig = mConfig;
        tmpSEMSeries->name = name;
        tmpSEMSeries->group = fae.group;
        tmpSEMSeries->isNominal = nominal;
        tmpSEMSeries->isCommon = fae.common;
        tmpSEMSeries->myNominalCoordSys = fae.nominalSystem;
        if(functionValid){ tmpSEMSeries->addFunction(defaultFunction); }
        tmpSEMSeries->isSolved = true;

        if(nominal){
            tmpSEMSeries->isSolved = true;
        }

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setScalarEntityMeasurementSeries(tmpSEMSeries);

        if(nominal){ //if nominal feature
            for(int i=0; i< coordSys.size();i++){
                if(coordSys.at(i) == fae.nominalSystem){
                    coordSys.at(i)->nominals.append(fw->getGeometry());
                    features.append(fw);
                    break;
                }
            }
            addNominalToActual(features,fw);
        }else{ //if no nominal feature
            features.append(fw);
            checkForNominals(features,fw);
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
bool FeatureUpdater::deleteFeature(FeatureWrapper *myFeatureWrapper, QList<FeatureWrapper *> featureSet){
    if(myFeatureWrapper != NULL && myFeatureWrapper->getFeature() != NULL){
        Feature *myFeature = myFeatureWrapper->getFeature();
        delete myFeature;
        return true;
    }
    return false;
}

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
