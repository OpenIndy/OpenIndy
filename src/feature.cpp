#include "feature.h"

#include "function.h"
#include "featurewrapper.h"

Feature::Feature(QObject *parent) : Element(parent), isActiveFeature(false), isSolved(false), isUpdated(false), isDrawn(false){

}

Feature::~Feature(){

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

/*!
 * \brief Feature::getFeatureName
 * \return
 */
QString Feature::getFeatureName() const{
    return this->name;
}

/*!
 * \brief Feature::setFeatureName
 * \param name
 * \return
 */
bool Feature::setFeatureName(QString name){
    if(this->name.compare(name) != 0){
        QString oldName = this->name;
        this->name = name;
        emit this->featureNameChanged(this->id, oldName);
        return true;
    }
    return false;
}

/*!
 * \brief Feature::getGroupName
 * \return
 */
QString Feature::getGroupName() const{
    return this->group;
}

/*!
 * \brief Feature::setGroupName
 * \param group
 */
void Feature::setGroupName(QString group){
    if(this->group.compare(group) != 0){
        this->group = group;
        emit this->featureGroupChanged(this->id);
    }
}

/*!
 * \brief Feature::getComment
 * \return
 */
QString Feature::getComment() const{
    return this->comment;
}

/*!
 * \brief Feature::setComment
 * \param comment
 */
void Feature::setComment(QString comment){
    if(this->comment.compare(comment) != 0){
        this->comment = comment;
        emit this->featureCommentChanged(this->id);
    }
}

/*!
 * \brief Feature::getIsUpdated
 * \return
 */
bool Feature::getIsUpdated() const{
    return this->isUpdated;
}

/*!
 * \brief Feature::setIsUpdated
 * \param isUpdated
 */
void Feature::setIsUpdated(bool isUpdated){
    if(this->isUpdated != isUpdated){
        this->isUpdated = isUpdated;
        emit this->featureIsUpdatedChanged(this->id);
    }
}

/*!
 * \brief Feature::getIsSolved
 * \return
 */
bool Feature::getIsSolved() const{
    return this->isSolved;
}

/*!
 * \brief Feature::setIsSolved
 * \param isSolved
 */
void Feature::setIsSolved(bool isSolved){
    if(this->isSolved != isSolved){
        this->isSolved = isSolved;
        emit this->featureIsSolvedChanged(this->id);
    }
}

/*!
 * \brief Feature::getDisplayColor
 * \return
 */
Configuration::eColor Feature::getDisplayColor() const{
    return displayColor;
}

/*!
 * \brief Feature::setDisplayColor
 * \param displayColor
 */
void Feature::setDisplayColor(Configuration::eColor displayColor){
    if(this->displayColor != displayColor){
        this->displayColor = displayColor;
        emit this->featureDisplayColorChanged(this->id);
    }
}

/*!
 * \brief Feature::getIsDrawn
 * \return
 */
bool Feature::getIsDrawn() const{
    return this->isDrawn;
}

/*!
 * \brief Feature::setIsDrawn
 * \param isDrawn
 */
void Feature::setIsDrawn(bool isDrawn){
    if(this->isDrawn != isDrawn){
        this->isDrawn = isDrawn;
        emit this->featureIsDrawnChanged(this->id);
    }
}

/*!
 * \brief Feature::getFunctions
 * \return
 */
const QList<Function *> Feature::getFunctions() const{
    return this->functionList;
}

/*!
 * \brief Feature::addFunction
 * \param f
 */
void Feature::addFunction(Function *f){
    if(f != NULL){
        this->functionList.append(f);
        emit this->featureFunctionListChanged(this->id);
    }
}

/*!
 * \brief Feature::removeFunction
 * \param index
 * \return
 */
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

/*!
 * \brief Feature::getIsActive
 * \return
 */
bool Feature::getIsActiveFeature() const{
    return this->isActiveFeature;
}

/*!
 * \brief Feature::setActiveState
 * \param isActive
 */
void Feature::setActiveFeatureState(bool isActive){
    if(this->isActiveFeature != isActive){
        this->isActiveFeature = isActive;
        emit this->featureIsActiveChanged(this->id);
    }
}

/*!
 * \brief Feature::getDisplayX
 * \return
 */
QString Feature::getDisplayX() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayY
 * \return
 */
QString Feature::getDisplayY() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayZ
 * \return
 */
QString Feature::getDisplayZ() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayI
 * \return
 */
QString Feature::getDisplayI() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayJ
 * \return
 */
QString Feature::getDisplayJ() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayK
 * \return
 */
QString Feature::getDisplayK() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayRadius
 * \return
 */
QString Feature::getDisplayRadius() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayIsCommon
 * \return
 */
QString Feature::getDisplayIsCommon() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayIsNominal
 * \return
 */
QString Feature::getDisplayIsNominal() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayObs
 * \return
 */
QString Feature::getDisplayObs() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplaySolved
 * \return
 */
QString Feature::getDisplaySolved() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayMConfig
 * \return
 */
QString Feature::getDisplayMConfig() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayStdDev
 * \return
 */
QString Feature::getDisplayStdDev() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayScalarDistanceValue
 * \return
 */
QString Feature::getDisplayScalarDistanceValue() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayScalarAngleValue
 * \return
 */
QString Feature::getDisplayScalarAngleValue() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayStartSystem
 * \return
 */
QString Feature::getDisplayStartSystem() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayDestinationSystem
 * \return
 */
QString Feature::getDisplayDestinationSystem() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayTranslationX
 * \return
 */
QString Feature::getDisplayTranslationX() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayTranslationY
 * \return
 */
QString Feature::getDisplayTranslationY() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayTranslationZ
 * \return
 */
QString Feature::getDisplayTranslationZ() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayScaleX
 * \return
 */
QString Feature::getDisplayScaleX() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayScaleY
 * \return
 */
QString Feature::getDisplayScaleY() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayScaleZ
 * \return
 */
QString Feature::getDisplayScaleZ() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayRotationX
 * \return
 */
QString Feature::getDisplayRotationX() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayRotationY
 * \return
 */
QString Feature::getDisplayRotationY() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayRotationZ
 * \return
 */
QString Feature::getDisplayRotationZ() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayScalarTemperatureValue
 * \return
 */
QString Feature::getDisplayScalarTemperatureValue() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayScalarMeasurementSeriesValue
 * \return
 */
QString Feature::getDisplayScalarMeasurementSeriesValue() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayUse
 * \return
 */
QString Feature::getDisplayUse() const{
    return "-/-";
}

/*!
 * \brief Feature::getDisplayTime
 * \return
 */
QString Feature::getDisplayTime() const{
    return "-/-";
}

/*!
 * \brief Feature::writeFeatureAttributes
 * \param stream
 * \return
 */
bool Feature::writeFeatureAttributes(QXmlStreamWriter &stream){

    //references to all features which need this feature to be recalculated
    for(int k = 0; k < this->usedFor.size(); k++){
        stream.writeStartElement("member");
        stream.writeAttribute("type", "usedForFeature");
        stream.writeAttribute("ref", QString::number(this->usedFor.at(k)->getFeature()->id));
        stream.writeEndElement();
    }

    //references to all features which this feature needs to be recalculated
    for(int k = 0; k < this->previouslyNeeded.size(); k++){
        stream.writeStartElement("member");
        stream.writeAttribute("type", "previouslyNeeded");
        stream.writeAttribute("ref", QString::number(this->previouslyNeeded.at(k)->getFeature()->id));
        stream.writeEndElement();
    }

    //all functions that are assigned to this feature
    for(int k =0;k<this->functionList.size();k++){
        stream.writeStartElement("function");
        stream.writeAttribute("name", this->functionList.at(k)->getMetaData()->name);
        stream.writeAttribute("plugin", this->functionList.at(k)->getMetaData()->pluginName);
        stream.writeAttribute("executionIndex", QString::number(k));

        //all input elements which were assigned to a function
        QMapIterator<int, QList<InputFeature> > j(this->functionList.at(k)->getFeatureOrder());
        while (j.hasNext()) {
            j.next();

            for(int m = 0; m < j.value().size();m++){
                stream.writeStartElement("inputElement");
                stream.writeAttribute("index", QString::number(j.key()));
                int neededFeatureId = j.value().at(m).id;
                stream.writeAttribute("type", QString::number( j.value().at(m).typeOfElement));
                stream.writeAttribute("ref", QString::number(neededFeatureId));
                stream.writeEndElement();
            }
        }

        stream.writeEndElement();
    }

    return true;

}

bool Feature::readFeatureAttributes(QXmlStreamReader &xml, ElementDependencies &dependencies){

    if(xml.name() == "member"){

            if(xml.tokenType() == QXmlStreamReader::StartElement) {

                QXmlStreamAttributes memberAttributes = xml.attributes();

                if(memberAttributes.hasAttribute("type")){

                    if (memberAttributes.value("type") == "usedForFeature"){

                        if(memberAttributes.hasAttribute("ref")){
                            dependencies.addFeatureID(memberAttributes.value("ref").toInt(),"usedForFeature");
                        }
                    }

                    if (memberAttributes.value("type") == "previouslyNeeded"){

                        if(memberAttributes.hasAttribute("ref")){
                            dependencies.addFeatureID(memberAttributes.value("ref").toInt(),"previouslyNeeded");
                        }
                    }
                }


                if(memberAttributes.hasAttribute("id")){
                    this->id = memberAttributes.value("ref").toInt();
                }

            }

    return true;
    }
}

bool Feature::readFunction(QXmlStreamReader &xml, ElementDependencies &d){

    functionInfo f;

    if(xml.name() == "function"){

        QXmlStreamAttributes attributes = xml.attributes();

        if(attributes.hasAttribute("name")){
            f.name= attributes.value("name").toString();
        }
        if(attributes.hasAttribute("plugin")) {
            f.plugin = attributes.value("plugin").toString();
        }
        if(attributes.hasAttribute("executionIndex")) {
            f.executionIndex = attributes.value("executionIndex").toInt();
        }


        while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                xml.name() == "function")) {
            if(xml.tokenType() == QXmlStreamReader::StartElement) {

                if(xml.name() == "inputElement"){

                    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                            xml.name() == "inputElement")) {
                        if(xml.tokenType() == QXmlStreamReader::StartElement) {

                            QXmlStreamAttributes memberAttributes = xml.attributes();

                                    featureIndex idx;

                                    if(memberAttributes.hasAttribute("index")){
                                       idx.idx = memberAttributes.value("index").toInt();
                                    }

                                    if(memberAttributes.hasAttribute("type") && memberAttributes.hasAttribute("ref")){

                                        int elementId = memberAttributes.value("ref").toInt();
                                        idx.typeOfElement =  memberAttributes.value("type").toInt();

                                        f.neededElements.insert(elementId,idx);
                                    }

                        }
                        /* ...and next... */
                        xml.readNext();
                    }
                }
            }
            /* ...and next... */
            xml.readNext();
        }

    d.addFunctionInfo(f);
    return true;
    }

}
