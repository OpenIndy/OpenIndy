#include "feature.h"

/*

#include "function.h"
#include "featurewrapper.h"

Feature::Feature(QObject *parent) : Element(parent), isActiveFeature(false), isSolved(false), isUpdated(false), isDrawn(false){

}

Feature::~Feature(){

    emit this->featureAboutToBeDeleted(this->getId());

    //check and update features which need this feature to recalc
    if(this->usedFor.size() > 0){
        foreach(FeatureWrapper *myDependent, this->usedFor){
            Feature *dependentFeature = myDependent->getFeature();
            if(dependentFeature != NULL){

                //delete the feature from all functions
                for(int i = 0; i < dependentFeature->functionList.size(); i++){
                    Function *myFunc = dependentFeature->functionList.at(i);
                    if(myFunc != NULL){
                        myFunc->removeFeature(this->id);
                    }
                }
                int index = -1;
                int i = 0;
                foreach(FeatureWrapper *prev, dependentFeature->previouslyNeeded){
                    if(prev != NULL && prev->getFeature() != NULL && prev->getFeature()->id == this->id){
                        index = i;
                        break;
                    }
                    i++;
                }
                if(index >= 0){
                    dependentFeature->previouslyNeeded.removeAt(index);
                }

            }
        }
    }

    //check and update features which were needed to recalc this feature
    if(this->previouslyNeeded.size() > 0){
        foreach(FeatureWrapper *myDependent, this->previouslyNeeded){
            Feature *dependentFeature = myDependent->getFeature();
            if(dependentFeature != NULL){

                //delete the feature from all functions
                int index = -1;
                int i = 0;
                foreach(FeatureWrapper *prev, dependentFeature->usedFor){
                    if(prev != NULL && prev->getFeature() != NULL && prev->getFeature()->id == this->id){
                        index = i;
                        break;
                    }
                    i++;
                }
                if(index >= 0){
                    dependentFeature->usedFor.removeAt(index);
                }

            }
        }
    }

    //delete all functions when deleting the feature
    for(int i = 0; i < this->functionList.size(); i++){
        delete this->functionList.at(i);
    }

}

QString Feature::getFeatureName() const{
    return this->name;
}

bool Feature::setFeatureName(const QString &name){
    if(this->name.compare(name) != 0){
        QString oldName = this->name;
        this->name = name;
        emit this->featureNameChanged(this->id, oldName);
        return true;
    }
    return false;
}

QString Feature::getGroupName() const{
    return this->group;
}

void Feature::setGroupName(const QString &group){
    if(this->group.compare(group) != 0){
        QString oldGroup = this->group;
        this->group = group;
        emit this->featureGroupChanged(this->id, oldGroup);
    }
}

QString Feature::getComment() const{
    return this->comment;
}

void Feature::setComment(const QString &comment){
    if(this->comment.compare(comment) != 0){
        this->comment = comment;
        emit this->featureCommentChanged(this->id);
    }
}

bool Feature::getIsUpdated() const{
    return this->isUpdated;
}

void Feature::setIsUpdated(const bool &isUpdated){
    if(this->isUpdated != isUpdated){
        this->isUpdated = isUpdated;
        emit this->featureIsUpdatedChanged(this->id);
    }
}

bool Feature::getIsSolved() const{
    return this->isSolved;
}

void Feature::setIsSolved(const bool &isSolved){
    if(this->isSolved != isSolved){
        this->isSolved = isSolved;
        emit this->featureIsSolvedChanged(this->id);
    }
}*/

/*!
 * \brief Feature::getDisplayColor
 * \return
 */
/*Configuration::eColor Feature::getDisplayColor() const{
    return displayColor;
}*/

/*!
 * \brief Feature::setDisplayColor
 * \param displayColor
 */
/*void Feature::setDisplayColor(Configuration::eColor displayColor){
    if(this->displayColor != displayColor){
        this->displayColor = displayColor;
        emit this->featureDisplayColorChanged(this->id);
    }
}*/

/*
bool Feature::getIsDrawn() const{
    return this->isDrawn;
}

void Feature::setIsDrawn(const bool &isDrawn){
    if(this->isDrawn != isDrawn){
        this->isDrawn = isDrawn;
        emit this->featureIsDrawnChanged(this->id);
    }
}

const QList<Function *> Feature::getFunctions() const{
    return this->functionList;
}

void Feature::addFunction(Function *f){
    if(f != NULL){
        this->functionList.append(f);
        emit this->featureFunctionListChanged(this->id);
    }
}

bool Feature::removeFunction(int index){
    try{

        if(this->functionList.size() > index){
            delete this->functionList.at(index);
            this->functionList.removeAt(index);
            emit this->featureFunctionListChanged(this->id);
            return true;
        }
        return false;

    }catch(exception &e){

        return false;
    }
}

bool Feature::getIsActiveFeature() const{
    return this->isActiveFeature;
}

void Feature::setActiveFeatureState(const bool &isActive){
    if(this->isActiveFeature != isActive){
        this->isActiveFeature = isActive;
        emit this->featureIsActiveChanged(this->id);
    }
}

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
    feature.setAttribute("active", this->getIsActiveFeature());

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

    }

    return result;

}

QString Feature::getDisplayX(bool showDiff) const{
    return "-/-";
}

QString Feature::getDisplayY(bool showDiff) const{
    return "-/-";
}

QString Feature::getDisplayZ(bool showDiff) const{
    return "-/-";
}

QString Feature::getDisplayI(bool showDiff) const{
    return "-/-";
}

QString Feature::getDisplayJ(bool showDiff) const{
    return "-/-";
}

QString Feature::getDisplayK(bool showDiff) const{
    return "-/-";
}

QString Feature::getDisplayRadius(bool showDiff) const{
    return "-/-";
}

QString Feature::getDisplayIsCommon() const{
    return "-/-";
}

QString Feature::getDisplayIsNominal() const{
    return "-/-";
}

QString Feature::getDisplayObs() const{
    return "-/-";
}

QString Feature::getDisplaySolved() const{
    return "-/-";
}

QString Feature::getDisplayMConfig() const{
    return "-/-";
}

QString Feature::getDisplayStdDev() const{
    return "-/-";
}

QString Feature::getDisplayScalarDistanceValue() const{
    return "-/-";
}

QString Feature::getDisplayScalarAngleValue() const{
    return "-/-";
}

QString Feature::getDisplayStartSystem() const{
    return "-/-";
}

QString Feature::getDisplayDestinationSystem() const{
    return "-/-";
}

QString Feature::getDisplayTranslationX() const{
    return "-/-";
}

QString Feature::getDisplayTranslationY() const{
    return "-/-";
}

QString Feature::getDisplayTranslationZ() const{
    return "-/-";
}

QString Feature::getDisplayScaleX() const{
    return "-/-";
}

QString Feature::getDisplayScaleY() const{
    return "-/-";
}

QString Feature::getDisplayScaleZ() const{
    return "-/-";
}

QString Feature::getDisplayRotationX() const{
    return "-/-";
}

QString Feature::getDisplayRotationY() const{
    return "-/-";
}

QString Feature::getDisplayRotationZ() const{
    return "-/-";
}

QString Feature::getDisplayScalarTemperatureValue() const{
    return "-/-";
}

QString Feature::getDisplayScalarMeasurementSeriesValue() const{
    return "-/-";
}

QString Feature::getDisplayUse() const{
    return "-/-";
}

QString Feature::getDisplayTime() const{
    return "-/-";
}

QString Feature::getDisplayExpansionOriginX() const
{
    return "-/-";
}

QString Feature::getDisplayExpansionOriginY() const
{
    return "-/-";
}

QString Feature::getDisplayExpansionOriginZ() const
{
    return "-/-";
}
*/


#include "function.h"
#include "featurewrapper.h"

/*!
 * \brief Feature::Feature
 * \param parent
 */
Feature::Feature(QObject *parent) : Element(parent), isActiveFeature(false), isSolved(false), isUpdated(false), isDrawn(false){

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
    this->isDrawn = copy.isDrawn;
    this->isUpdated = copy.isUpdated;

    //copy functions (usedFor is not copied)
    this->functionList = copy.functionList;
    this->previouslyNeeded = copy.previouslyNeeded;

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
    this->isDrawn = copy.isDrawn;
    this->isUpdated = copy.isUpdated;

    //copy functions (usedFor is not copied)
    this->functionList = copy.functionList;
    this->previouslyNeeded = copy.previouslyNeeded;

    return *this;

}

/*!
 * \brief Feature::~Feature
 */
Feature::~Feature(){

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
    if(name.compare(this->name) != 0){
        QString oldName = this->name;
        this->name = name;
        emit this->featureNameChanged(this->id, oldName);
    }
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
        emit this->featureIsUpdatedChanged(this->id);
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
 * \brief Feature::getIsDrawn
 * \return
 */
const bool &Feature::getIsDrawn() const{
    return this->isDrawn;
}

/*!
 * \brief Feature::setIsDrawn
 * \param isDrawn
 */
void Feature::setIsDrawn(const bool &isDrawn){
    if(isDrawn != this->isDrawn){
        this->isDrawn = isDrawn;
        emit this->featureIsDrawnChanged(this->id);
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
        this->functionList.append(function);
        emit this->featureFunctionListChanged(this->id);
    }
}

/*!
 * \brief Feature::removeFunction
 * \param index
 * \return
 */
bool Feature::removeFunction(const int &index){
    if(this->functionList.size() > index && index >= 0){
        this->functionList.removeAt(index);
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
 * \brief Feature::recalc
 */
void Feature::recalc(){

    this->isSolved = false;

    //execute all functions in the specified order
    foreach(QPointer<Function> function, this->functionList){

        //break if the function pointer is not valid
        if(function.isNull()){
            this->isSolved = false;
            break;
        }

        //try to solve the current function
        this->isSolved = function->exec(*this);
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
    feature.setAttribute("active", this->getIsActiveFeature());

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

    }

    return result;

}
