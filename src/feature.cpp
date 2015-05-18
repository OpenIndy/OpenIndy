#include "feature.h"

#include "oijob.h"
#include "function.h"
#include "featurewrapper.h"

/*!
 * \brief Feature::Feature
 * \param parent
 */
Feature::Feature(QObject *parent) : Element(parent), isActiveFeature(false), isSolved(false), isUpdated(false){
    this->selfFeature = new FeatureWrapper();
}

/*!
 * \brief Feature::Feature
 * \param copy
 * \param parent
 */
Feature::Feature(const Feature &copy, QObject *parent) : Element(copy, parent){

    //copy attributes
    this->name = copy.name;
    this->comment = copy.comment;
    this->group = copy.group;
    this->isSolved = copy.isSolved;
    this->isActiveFeature = copy.isActiveFeature;
    this->isUpdated = copy.isUpdated;

    //copy functions (usedFor is not copied)
    //this->functionList = copy.functionList;
    //this->previouslyNeeded = copy.previouslyNeeded;

    this->selfFeature = new FeatureWrapper();

}

/*!
 * \brief Feature::operator =
 * \param copy
 * \return
 */
Feature &Feature::operator=(const Feature &copy){

    //copy attributes
    this->name = copy.name;
    this->comment = copy.comment;
    this->group = copy.group;
    this->isSolved = copy.isSolved;
    this->isActiveFeature = copy.isActiveFeature;
    this->isUpdated = copy.isUpdated;

    //copy functions (usedFor is not copied)
    //this->functionList = copy.functionList;
    //this->previouslyNeeded = copy.previouslyNeeded;

    this->selfFeature = new FeatureWrapper();

    return *this;

}

/*!
 * \brief Feature::~Feature
 */
Feature::~Feature(){

    //check and update features which need this feature to recalc
    if(this->usedFor.size() > 0 && this->selfFeature.isNull()){
        foreach(const QPointer<FeatureWrapper> &feature, this->usedFor){

            //check feature
            if(feature.isNull() || feature->getFeature().isNull()){
                continue;
            }

            //delete the feature from all functions
            foreach(const QPointer<Function> function, feature->getFeature()->getFunctions()){
                if(!function.isNull()){
                    function->removeInputElement(this->id);
                }
            }

            //remove this feature from previously needed list
            feature->getFeature()->previouslyNeeded.removeOne(this->selfFeature);

        }
    }

    //check and update features which were needed to recalc this feature
    if(this->previouslyNeeded.size() > 0){
        foreach(const QPointer<FeatureWrapper> &feature, this->previouslyNeeded){

            //remove this feature from usedFor list
            if(this->selfFeature.isNull() && !feature.isNull() && !feature->getFeature().isNull()){
                feature->getFeature()->usedFor.removeOne(this->selfFeature);
            }

        }
    }

    //delete all functions when deleting the feature
    foreach(const QPointer<Function> &function, this->functionList){
        if(!function.isNull()){
            delete function;
        }
    }

    emit this->elementAboutToBeDeleted(this->id, this->name, this->group, this->selfFeature->getFeatureTypeEnum());

    //delete self feature wrapper
    if(!this->selfFeature.isNull()){
        delete this->selfFeature;
    }

}

/*!
 * \brief Feature::getFeatureWrapper
 * \return
 */
const QPointer<FeatureWrapper> &Feature::getFeatureWrapper() const{
    return this->selfFeature;
}

/*!
 * \brief Feature::getFeatureName
 * \return
 */
const QString &Feature::getFeatureName() const{
    return this->name;
}

/*!
 * \brief Feature::setFeatureName
 * \param name
 */
void Feature::setFeatureName(const QString &name){

    //check if old name = new name
    if(this->name.compare(name) == 0){
        return;
    }

    //validate the new name before setting it
    if(!this->job.isNull() && !this->selfFeature.isNull()){

        bool isNominal = false;
        QPointer<CoordinateSystem> nominalSystem;
        if(!this->selfFeature->getGeometry().isNull()){
            isNominal = this->selfFeature->getGeometry()->getIsNominal();
            nominalSystem = this->selfFeature->getGeometry()->getNominalSystem();
        }

        if(!this->job->validateFeatureName(name, this->selfFeature->getFeatureTypeEnum(), isNominal, nominalSystem)){
            return;
        }

    }

    //set the new name
    QString oldName = this->name;
    this->name = name;
    emit this->featureNameChanged(this->id, oldName);

}

/*!
 * \brief Feature::getGroupName
 * \return
 */
const QString &Feature::getGroupName() const{
    return this->group;
}

/*!
 * \brief Feature::setGroupName
 * \param group
 */
void Feature::setGroupName(const QString &group){
    if(group.compare(this->group) != 0){
        QString oldGroup = this->group;
        this->group = group;
        emit this->featureGroupChanged(this->id, oldGroup);
    }
}

/*!
 * \brief Feature::getComment
 * \return
 */
const QString &Feature::getComment() const{
    return this->comment;
}

/*!
 * \brief Feature::setComment
 * \param comment
 */
void Feature::setComment(const QString &comment){
    if(comment.compare(this->comment) != 0){
        this->comment = comment;
        emit this->featureCommentChanged(this->id);
    }
}

/*!
 * \brief Feature::getIsUpdated
 * \return
 */
const bool &Feature::getIsUpdated() const{
    return this->isUpdated;
}

/*!
 * \brief Feature::setIsUpdated
 * \param isUpdated
 */
void Feature::setIsUpdated(const bool &isUpdated){
    if(isUpdated != this->isUpdated){
        this->isUpdated = isUpdated;
        //emit this->featureIsUpdatedChanged(this->id);
    }
}

/*!
 * \brief Feature::getIsSolved
 * \return
 */
const bool &Feature::getIsSolved() const{
    return this->isSolved;
}

/*!
 * \brief Feature::setIsSolved
 * \param isSolved
 */
void Feature::setIsSolved(const bool &isSolved){
    if(isSolved != this->isSolved){
        this->isSolved = isSolved;
        emit this->featureIsSolvedChanged(this->id);
    }
}

/*!
 * \brief Feature::getFunctions
 * \return
 */
const QList<QPointer<Function> > &Feature::getFunctions() const{
    return this->functionList;
}

/*!
 * \brief Feature::addFunction
 * \param function
 */
void Feature::addFunction(const QPointer<Function> &function){
    if(!function.isNull()){

        //let the current job generate a unique id if it is valid
        if(!this->job.isNull()){
            function->id = this->job->generateUniqueId();
        }

        this->functionList.append(function);
        this->isUpdated = false;
        emit this->featureFunctionListChanged(this->id);

    }
}

/*!
 * \brief Feature::removeFunction
 * \param index
 */
void Feature::removeFunction(const int &index){
    if(this->functionList.size() > index && index >= 0){
        this->functionList.removeAt(index);
        this->isUpdated = false;
        emit this->featureFunctionListChanged(this->id);
    }
}

/*!
 * \brief Feature::getIsActiveFeature
 * \return
 */
const bool &Feature::getIsActiveFeature() const{
    return this->isActiveFeature;
}

/*!
 * \brief Feature::setActiveFeatureState
 * \param isActiveFeature
 */
void Feature::setActiveFeatureState(const bool &isActiveFeature){
    if(isActiveFeature != this->isActiveFeature){
        this->isActiveFeature = isActiveFeature;
        emit this->featureIsActiveChanged(this->id);
    }
}

/*!
 * \brief Feature::getUsedFor
 * \return
 */
const QList<QPointer<FeatureWrapper> > &Feature::getUsedFor() const{
    return this->usedFor;
}

/*!
 * \brief Feature::addUsedFor
 * \param feature
 * \return
 */
bool Feature::addUsedFor(const QPointer<FeatureWrapper> &feature){

    //check feature
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //check if the feature is already included
    if(this->usedFor.contains(feature)){
        return false;
    }

    //check if feature is in same job
    if(!this->job.isNull()){
        QPointer<FeatureWrapper> jobFeature = this->job->getFeatureById(feature->getFeature()->getId());
        if(jobFeature.isNull() || jobFeature->getFeature().isNull() || feature->getFeature() != jobFeature->getFeature()){
            return false;
        }
    }

    this->usedFor.append(feature);

    //add this feature to the previously needed features of feature
    if(!this->selfFeature.isNull()){
        feature->getFeature()->addPreviouslyNeeded(this->selfFeature);
    }

    return true;

}

/*!
 * \brief Feature::removeUsedFor
 * \param feature
 * \return
 */
bool Feature::removeUsedFor(const QPointer<FeatureWrapper> &feature){

    //check feature
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //check if the feature is included
    if(!this->usedFor.contains(feature)){
        return false;
    }

    this->usedFor.removeOne(feature);

    //remove this feature from the previously needed features of feature
    if(!this->selfFeature.isNull()){
        feature->getFeature()->removePreviouslyNeeded(this->selfFeature);
    }

    return true;

}

/*!
 * \brief Feature::getPreviouslyNeeded
 * \return
 */
const QList<QPointer<FeatureWrapper> > &Feature::getPreviouslyNeeded() const{
    return this->previouslyNeeded;
}

/*!
 * \brief Feature::addPreviouslyNeeded
 * \param feature
 * \return
 */
bool Feature::addPreviouslyNeeded(const QPointer<FeatureWrapper> &feature){

    //check feature
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //check if the feature is already included
    if(this->previouslyNeeded.contains(feature)){
        return false;
    }

    //check if feature is in same job
    if(!this->job.isNull()){
        QPointer<FeatureWrapper> jobFeature = this->job->getFeatureById(feature->getFeature()->getId());
        if(jobFeature.isNull() || jobFeature->getFeature().isNull() || feature->getFeature() != jobFeature->getFeature()){
            return false;
        }
    }

    this->previouslyNeeded.append(feature);

    this->isUpdated = false;

    //add this feature to the used for features of feature
    if(!this->selfFeature.isNull()){
        feature->getFeature()->addUsedFor(this->selfFeature);
    }

    return true;

}

/*!
 * \brief Feature::removePreviouslyNeeded
 * \param feature
 * \return
 */
bool Feature::removePreviouslyNeeded(const QPointer<FeatureWrapper> &feature){

    //check feature
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //check if the feature is included
    if(!this->previouslyNeeded.contains(feature)){
        return false;
    }

    this->previouslyNeeded.removeOne(feature);

    this->isUpdated = false;

    //remove this feature from the used for features of feature
    if(!this->selfFeature.isNull()){
        feature->getFeature()->removeUsedFor(this->selfFeature);
    }

    return true;

}

/*!
 * \brief Feature::recalc
 */
void Feature::recalc(){

    this->isSolved = false;

    //execute all functions in the specified order
    foreach(const QPointer<Function> &function, this->functionList){

        //break if the function pointer is not valid
        if(function.isNull()){
            this->isSolved = false;
            break;
        }

        //try to solve the current function
        this->isSolved = function->exec(this->selfFeature);
        if(!this->isSolved){
            break;
        }

    }

}

/*!
 * \brief Feature::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Feature::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement feature = Element::toOpenIndyXML(xmlDoc);

    if(feature.isNull()){
        return feature;
    }

    feature.setTagName("feature");

    //add feature attributes
    feature.setAttribute("name", this->getFeatureName());
    feature.setAttribute("group", this->getGroupName());
    feature.setAttribute("solved", this->getIsSolved());
    feature.setAttribute("comment", this->getComment());
    feature.setAttribute("activeFeature", this->getIsActiveFeature());

    //add functions
    if(this->functionList.size() > 0){
        QDomElement functions = xmlDoc.createElement("functions");
        int index = 0;
        foreach(Function *f, this->functionList){
            QDomElement function = f->toOpenIndyXML(xmlDoc);
            if(!function.isNull()){
                function.setAttribute("executionIndex", index);
                functions.appendChild(function);
            }
            index++;
        }
        feature.appendChild(functions);
    }

    //add usedFor features
    if(this->usedFor.size() > 0){
        QDomElement usedForFeatures = xmlDoc.createElement("usedFor");
        foreach(FeatureWrapper *f, this->usedFor){
            if(f != NULL && f->getFeature() != NULL){
                QDomElement usedFor = xmlDoc.createElement("feature");
                usedFor.setAttribute("ref", f->getFeature()->getId());
                usedForFeatures.appendChild(usedFor);
            }
        }
        feature.appendChild(usedForFeatures);
    }

    //add previouslyNeeded features
    if(this->previouslyNeeded.size() > 0){
        QDomElement previouslyNeededFeatures = xmlDoc.createElement("previouslyNeeded");
        foreach(FeatureWrapper *f, this->previouslyNeeded){
            if(f != NULL && f->getFeature() != NULL){
                QDomElement previouslyNeeded = xmlDoc.createElement("feature");
                previouslyNeeded.setAttribute("ref", f->getFeature()->getId());
                previouslyNeededFeatures.appendChild(previouslyNeeded);
            }
        }
        feature.appendChild(previouslyNeededFeatures);
    }

    return feature;

}

/*!
 * \brief Feature::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Feature::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Element::fromOpenIndyXML(xmlElem);

    if(result){

        //set feature attributes
        if(!xmlElem.hasAttribute("name") || !xmlElem.hasAttribute("group") || !xmlElem.hasAttribute("comment")
                || !xmlElem.hasAttribute("solved")){
            return false;
        }
        this->name = xmlElem.attribute("name");
        this->group = xmlElem.attribute("group");
        this->comment = xmlElem.attribute("comment");
        this->isSolved = xmlElem.attribute("solved").toInt();
        this->isActiveFeature = xmlElem.attribute("activeFeature").toInt();

    }

    return result;

}

/*!
 * \brief Feature::getDisplayType
 * \return
 */
QString Feature::getDisplayType() const{
    if(!this->selfFeature.isNull()){
        return this->selfFeature->getFeatureTypeString();
    }
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayIsSolved
 * \return
 */
QString Feature::getDisplayIsSolved() const{
    return this->isSolved?"true":"false";
}

/*!
 * \brief Feature::getDisplayIsUpdated
 * \return
 */
QString Feature::getDisplayIsUpdated() const{
    return this->isUpdated?"true":"false";
}

/*!
 * \brief Feature::getDisplayFunctions
 * \return
 */
QString Feature::getDisplayFunctions() const{
    QString display = "";
    for(int i = 0; i < this->functionList.size(); i++){
        QPointer<Function> function = this->functionList.at(i);
        if(!function.isNull()){
            if(i == this->functionList.size() - 1){ // last function
                display += function->getMetaData().name;
            }else{
                display += function->getMetaData().name + ", ";
            }
        }
    }
    return display;
}

/*!
 * \brief Feature::getDisplayUsedFor
 * \return
 */
QString Feature::getDisplayUsedFor() const{
    QString display = "";
    for(int i = 0; i < this->usedFor.size(); i++){
        QPointer<FeatureWrapper> feature = this->usedFor.at(i);
        if(!feature.isNull() && !feature->getFeature().isNull()){
            if(i == this->usedFor.size() - 1){ // last feature
                display += feature->getFeature()->getFeatureName();
            }else{
                display += feature->getFeature()->getFeatureName() + ", ";
            }
        }
    }
    return display;
}

/*!
 * \brief Feature::getDisplayPreviouslyNeeded
 * \return
 */
QString Feature::getDisplayPreviouslyNeeded() const{
    QString display = "";
    for(int i = 0; i < this->previouslyNeeded.size(); i++){
        QPointer<FeatureWrapper> feature = this->previouslyNeeded.at(i);
        if(!feature.isNull() && !feature->getFeature().isNull()){
            if(i == this->previouslyNeeded.size() - 1){ // last feature
                display += feature->getFeature()->getFeatureName();
            }else{
                display += feature->getFeature()->getFeatureName() + ", ";
            }
        }
    }
    return display;
}

/*!
 * \brief Feature::getDisplayStDev
 * \param type
 * \param digits
 * \return
 */
QString Feature::getDisplayStDev(const UnitType &type, const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayMeasurementConfig
 * \return
 */
QString Feature::getDisplayMeasurementConfig() const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayObservations
 * \return
 */
QString Feature::getDisplayObservations() const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayPrimaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayPrimaryI(const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayPrimaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayPrimaryJ(const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayPrimaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayPrimaryK(const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayRadiusA
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayRadiusA(const UnitType &type, const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayRadiusB
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayRadiusB(const UnitType &type, const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplaySecondaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplaySecondaryI(const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplaySecondaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplaySecondaryJ(const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplaySecondaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplaySecondaryK(const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayAperture
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayAperture(const UnitType &type, const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayA
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayA(const UnitType &type, const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayB
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayB(const UnitType &type, const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayC
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayC(const UnitType &type, const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayAngle
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayAngle(const UnitType &type, const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayDistance
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayDistance(const UnitType &type, const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayMeasurementSeries
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayMeasurementSeries(const UnitType &type, const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayTemperature
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayTemperature(const UnitType &type, const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayLength
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Feature::getDisplayLength(const UnitType &type, const int &digits, const bool &showDiff) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayExpansionOriginX
 * \param type
 * \param digits
 * \return
 */
QString Feature::getDisplayExpansionOriginX(const UnitType &type, const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayExpansionOriginY
 * \param type
 * \param digits
 * \return
 */
QString Feature::getDisplayExpansionOriginY(const UnitType &type, const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayExpansionOriginZ
 * \param type
 * \param digits
 * \return
 */
QString Feature::getDisplayExpansionOriginZ(const UnitType &type, const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayXAxisI
 * \param digits
 * \return
 */
QString Feature::getDisplayXAxisI(const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayXAxisJ
 * \param digits
 * \return
 */
QString Feature::getDisplayXAxisJ(const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayXAxisK
 * \param digits
 * \return
 */
QString Feature::getDisplayXAxisK(const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayYAxisI
 * \param digits
 * \return
 */
QString Feature::getDisplayYAxisI(const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayYAxisJ
 * \param digits
 * \return
 */
QString Feature::getDisplayYAxisJ(const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayYAxisK
 * \param digits
 * \return
 */
QString Feature::getDisplayYAxisK(const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayZAxisI
 * \param digits
 * \return
 */
QString Feature::getDisplayZAxisI(const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayZAxisJ
 * \param digits
 * \return
 */
QString Feature::getDisplayZAxisJ(const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayZAxisK
 * \param digits
 * \return
 */
QString Feature::getDisplayZAxisK(const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayStartSystem
 * \return
 */
QString Feature::getDisplayStartSystem() const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayDestinationSystem
 * \return
 */
QString Feature::getDisplayDestinationSystem() const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayTranslationX
 * \param type
 * \param digits
 * \return
 */
QString Feature::getDisplayTranslationX(const UnitType &type, const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayTranslationY
 * \param type
 * \param digits
 * \return
 */
QString Feature::getDisplayTranslationY(const UnitType &type, const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayTranslationZ
 * \param type
 * \param digits
 * \return
 */
QString Feature::getDisplayTranslationZ(const UnitType &type, const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayRotationX
 * \param type
 * \param digits
 * \return
 */
QString Feature::getDisplayRotationX(const UnitType &type, const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayRotationY
 * \param type
 * \param digits
 * \return
 */
QString Feature::getDisplayRotationY(const UnitType &type, const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayRotationZ
 * \param type
 * \param digits
 * \return
 */
QString Feature::getDisplayRotationZ(const UnitType &type, const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayScaleX
 * \param digits
 * \return
 */
QString Feature::getDisplayScaleX(const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayScaleY
 * \param digits
 * \return
 */
QString Feature::getDisplayScaleY(const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayScaleZ
 * \param digits
 * \return
 */
QString Feature::getDisplayScaleZ(const int &digits) const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayIsUsed
 * \return
 */
QString Feature::getDisplayIsUsed() const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayValidTime
 * \return
 */
QString Feature::getDisplayValidTime() const{
    return QString("-/-");
}

/*!
 * \brief Feature::getDisplayIsMovement
 * \return
 */
QString Feature::getDisplayIsMovement() const{
    return QString("-/-");
}

/*!
 * \brief Feature::setUpFeatureId
 * Generate a new unique id when the current job was set
 */
void Feature::setUpFeatureId(){

    //check job
    if(this->job.isNull()){
        return;
    }

    //let the job generate a new unique id for this feature
    this->id = job->generateUniqueId();

    //generate unique ids for the feature's functions
    foreach(const QPointer<Function> &function, this->functionList){
        if(!function.isNull()){
            function->id = this->job->generateUniqueId();
        }
    }

}

/*!
 * \brief Feature::setJob
 * Called by OiJob. Sets the current job this feature belongs to and checks its id
 * \param job
 */
void Feature::setJob(const QPointer<OiJob> &job){

    //check and set job
    if(job.isNull()){
        return;
    }
    this->job = job;

    //generate feature id
    this->setUpFeatureId();

}
