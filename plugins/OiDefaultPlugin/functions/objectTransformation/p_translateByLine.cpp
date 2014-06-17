#include "p_translateByLine.h"

/*!
 * \brief TranslateByLine::getMetaData
 * \return
 */
PluginMetaData* TranslateByLine::getMetaData(){
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "TranslateByLine";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1 %2 %3")
            .arg("This function shifts the target geometry by the specified offset along the direction vector of the specified line.")
            .arg("<br><br> <u>parameter description:</u> <br>")
            .arg("<b>invert:</b> Defines wether the direction vector shall be inverted before the target geometry is moved");
    metaData->iid = "de.openIndy.Plugin.Function.ObjectTransformation.v001";
    return metaData;
}

/*!
 * \brief TranslateByLine::getNeededElements
 * \return
 */
QList<InputParams> TranslateByLine::getNeededElements(){
    QList<InputParams> result;
    InputParams param1;
    param1.index = 0;
    param1.description = "Select a line whose direction vector will be used to shift the target geometry.";
    param1.infinite = false;
    param1.typeOfElement = Configuration::eLineElement;
    result.append(param1);
    InputParams param2;
    param2.index = 0;
    param2.description = "Select a distance as the shift amount.";
    param2.infinite = false;
    param2.typeOfElement = Configuration::eScalarEntityDistanceElement;
    result.append(param2);
    return result;
}

/*!
 * \brief TranslateByLine::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> TranslateByLine::applicableFor(){
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::ePointFeature);
    result.append(Configuration::eLineFeature);
    result.append(Configuration::ePlaneFeature);
    result.append(Configuration::eSphereFeature);
    return result;
}

/*!
 * \brief TranslateByLine::getStringParameter
 * \return
 */
QMap<QString, QStringList> TranslateByLine::getStringParameter(){
    QMap<QString, QStringList> result;
    QString key = "invert";
    QStringList value;
    value.append("no");
    value.append("yes");
    result.insert(key, value);
    return result;
}

/*!
 * \brief TranslateByLine::exec
 * \param targetPoint
 * \return
 */
bool TranslateByLine::exec(Point &targetPoint){
    if(this->isValid()){
        Line *line = this->getLine();
        ScalarEntityDistance *seDistance = this->getDistance();
        if(line != NULL && seDistance != NULL){
            OiVec v(3);
            v.setAt(0, line->ijk.getAt(0));
            v.setAt(1, line->ijk.getAt(1));
            v.setAt(2, line->ijk.getAt(2));
            OiVec v0 = v.normalize();
            v0.add(1.0);
            line->ijk = v0;
            this->shiftPoint(targetPoint, line, seDistance);
            return true;
        }else{
            this->writeToConsole("Not enough valid geometries available for calculation");
            return false;
        }
    }else{
        this->writeToConsole("Not enough valid geometries available for calculation");
        return false;
    }
}

/*!
 * \brief TranslateByLine::exec
 * \param targetLine
 * \return
 */
bool TranslateByLine::exec(Line &targetLine){
    if(this->isValid()){
        Line *line = this->getLine();
        ScalarEntityDistance *seDistance = this->getDistance();
        if(line != NULL && seDistance != NULL){
            OiVec v(3);
            v.setAt(0, line->ijk.getAt(0));
            v.setAt(1, line->ijk.getAt(1));
            v.setAt(2, line->ijk.getAt(2));
            OiVec v0 = v.normalize();
            v0.add(1.0);
            line->ijk = v0;
            this->shiftLine(targetLine, line, seDistance);
            return true;
        }else{
            this->writeToConsole("Not enough valid geometries available for calculation");
            return false;
        }
    }else{
        this->writeToConsole("Not enough valid geometries available for calculation");
        return false;
    }
}

/*!
 * \brief TranslateByLine::exec
 * \param targetPlane
 * \return
 */
bool TranslateByLine::exec(Plane &targetPlane){
    if(this->isValid()){
        Line *line = this->getLine();
        ScalarEntityDistance *seDistance = this->getDistance();
        if(line != NULL && seDistance != NULL){
            OiVec v(3);
            v.setAt(0, line->ijk.getAt(0));
            v.setAt(1, line->ijk.getAt(1));
            v.setAt(2, line->ijk.getAt(2));
            OiVec v0 = v.normalize();
            v0.add(1.0);
            line->ijk = v0;
            this->shiftPlane(targetPlane, line, seDistance);
            return true;
        }else{
            this->writeToConsole("Not enough valid geometries available for calculation");
            return false;
        }
    }else{
        this->writeToConsole("Not enough valid geometries available for calculation");
        return false;
    }
}

/*!
 * \brief TranslateByLine::exec
 * \param targetSphere
 * \return
 */
bool TranslateByLine::exec(Sphere &targetSphere){
    if(this->isValid()){
        Line *line = this->getLine();
        ScalarEntityDistance *seDistance = this->getDistance();
        if(line != NULL && seDistance != NULL){
            OiVec v(3);
            v.setAt(0, line->ijk.getAt(0));
            v.setAt(1, line->ijk.getAt(1));
            v.setAt(2, line->ijk.getAt(2));
            OiVec v0 = v.normalize();
            v0.add(1.0);
            line->ijk = v0;
            this->shiftSphere(targetSphere, line, seDistance);
            return true;
        }else{
            this->writeToConsole("Not enough valid geometries available for calculation");
            return false;
        }
    }else{
        this->writeToConsole("Not enough valid geometries available for calculation");
        return false;
    }
}

/*!
 * \brief TranslateByLine::getLine
 * \return
 */
Line* TranslateByLine::getLine(){
    Line *result = NULL;
    foreach(Line *l, this->lines){
        if(result == NULL && l->getIsSolved()){
            result = l;
            this->setUseState(result->getId(), true);
        }else{
            this->setUseState(l->getId(), false);
        }
    }
    return result;
}

/*!
 * \brief TranslateByLine::getDistance
 * \return
 */
ScalarEntityDistance* TranslateByLine::getDistance(){
    ScalarEntityDistance *result = NULL;
    foreach(ScalarEntityDistance *sed, this->scalarEntityDistances){
        if(result == NULL && sed->getIsSolved()){
            result = sed;
            this->setUseState(result->getId(), true);
        }else{
            this->setUseState(sed->getId(), false);
        }
    }
    return result;
}

/*!
 * \brief TranslateByLine::shiftPoint
 * \param targetPoint
 * \param line
 * \param distance
 */
void TranslateByLine::shiftPoint(Point &targetPoint, Line *line, ScalarEntityDistance *distance){
    if(targetPoint.getIsSolved() && line->getIsSolved() && distance->getIsSolved()){ //if all elements are solved
        if(targetPoint.getIsUpdated() && line->getIsUpdated() && distance->getIsUpdated()){ //if all elements are in the same coordinate system
            FunctionConfiguration myConfig = this->getFunctionConfiguration();
            QMap<QString, QString> stringParameter = myConfig.stringParameter;
            bool invert = false;
            if(stringParameter.contains("invert")){
                QString value = static_cast<QString>(stringParameter.find("invert").value());
                if(value.compare("yes", Qt::CaseInsensitive) == 0){ //if yes was selected
                    invert = true;
                }
            }
            if(invert){
                targetPoint.xyz = targetPoint.xyz - distance->getDistance() * line->ijk;
                this->variancePropagationPoint(targetPoint, line, distance, true);
            }else{
                targetPoint.xyz = targetPoint.xyz + distance->getDistance() * line->ijk;
                this->variancePropagationPoint(targetPoint, line, distance, false);
            }
            targetPoint.xyz.setAt(3, 1.0);
        }
    }
}

/*!
 * \brief TranslateByLine::variancePropagationPoint
 * Calculate variance propagation for type point
 * \param targetPoint
 * \param line
 * \param distance
 */
void TranslateByLine::variancePropagationPoint(Point &targetPoint, Line *line, ScalarEntityDistance *distance, bool invert){
    OiMat sll_apost_point = targetPoint.getStatistic().qxx * targetPoint.getStatistic().s0_aposteriori * targetPoint.getStatistic().s0_aposteriori;
    OiMat sll_apost_line = line->getStatistic().qxx * (line->getStatistic().s0_aposteriori * line->getStatistic().s0_aposteriori);
    OiMat sll_apriori_point = targetPoint.getStatistic().qxx * (targetPoint.getStatistic().s0_apriori * targetPoint.getStatistic().s0_apriori);
    OiMat sll_apriori_line = line->getStatistic().qxx * (line->getStatistic().s0_apriori * line->getStatistic().s0_apriori);
    double dist = distance->getDistance();
    OiMat f(3, 6);
    f.setAt(0, 0, 1.0);
    f.setAt(1, 1, 1.0);
    f.setAt(2, 2, 1.0);
    if(invert){
        f.setAt(0, 3, -dist);
        f.setAt(1, 4, -dist);
        f.setAt(2, 5, -dist);
    }else{
        f.setAt(0, 3, dist);
        f.setAt(1, 4, dist);
        f.setAt(2, 5, dist);
    }
    OiMat sll_apost_all(6, 6);
    OiMat sll_apriori_all(6, 6);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            sll_apriori_all.setAt(i, j, sll_apriori_point.getAt(i, j));
            sll_apriori_all.setAt(i+3, j+3, sll_apriori_line.getAt(i, j));
            sll_apost_all.setAt(i, j, sll_apost_point.getAt(i, j));
            sll_apost_all.setAt(i+3, j+3, sll_apost_line.getAt(i, j));
        }
    }
    OiMat sll_apost_f = f * sll_apost_all * f.t();
    OiMat sll_apriori_f = f * sll_apriori_all * f.t();
    Statistic myStats;
    myStats.qxx = sll_apriori_f / (targetPoint.getStatistic().s0_aposteriori * targetPoint.getStatistic().s0_aposteriori);
    targetPoint.setStatistic(myStats);
    this->myStatistic.qxx = targetPoint.getStatistic().qxx;
    this->myStatistic.isValid = true;
}

/*!
 * \brief TranslateByLine::shiftLine
 * \param targetLine
 * \param line
 * \param distance
 */
void TranslateByLine::shiftLine(Line &targetLine, Line *line, ScalarEntityDistance *distance){
    if(targetLine.getIsSolved() && line->getIsSolved() && distance->getIsSolved()){ //if all elements are solved
        if(targetLine.getIsUpdated() && line->getIsUpdated() && distance->getIsUpdated()){ //if all elements are in the same coordinate system
            FunctionConfiguration myConfig = this->getFunctionConfiguration();
            QMap<QString, QString> stringParameter = myConfig.stringParameter;
            bool invert = false;
            if(stringParameter.contains("invert")){
                QString value = static_cast<QString>(stringParameter.find("invert").value());
                if(value.compare("yes", Qt::CaseInsensitive) == 0){ //if yes was selected
                    invert = true;
                }
            }
            if(invert){
                targetLine.xyz = targetLine.xyz - distance->getDistance() * line->ijk;
            }else{
                targetLine.xyz = targetLine.xyz + distance->getDistance() * line->ijk;
            }
            targetLine.xyz.setAt(3, 1.0);
        }
    }
}

/*!
 * \brief TranslateByLine::shiftPlane
 * \param targetPlane
 * \param line
 * \param distance
 */
void TranslateByLine::shiftPlane(Plane &targetPlane, Line *line, ScalarEntityDistance *distance){
    if(targetPlane.getIsSolved() && line->getIsSolved() && distance->getIsSolved()){ //if all elements are solved
        if(targetPlane.getIsUpdated() && line->getIsUpdated() && distance->getIsUpdated()){ //if all elements are in the same coordinate system
            FunctionConfiguration myConfig = this->getFunctionConfiguration();
            QMap<QString, QString> stringParameter = myConfig.stringParameter;
            bool invert = false;
            if(stringParameter.contains("invert")){
                QString value = static_cast<QString>(stringParameter.find("invert").value());
                if(value.compare("yes", Qt::CaseInsensitive) == 0){ //if yes was selected
                    invert = true;
                }
            }
            if(invert){
                targetPlane.xyz = targetPlane.xyz - distance->getDistance() * line->ijk;
            }else{
                targetPlane.xyz = targetPlane.xyz + distance->getDistance() * line->ijk;
            }
            targetPlane.xyz.setAt(3, 1.0);
        }
    }
}

/*!
 * \brief TranslateByLine::shiftSphere
 * \param targetSphere
 * \param line
 * \param distance
 */
void TranslateByLine::shiftSphere(Sphere &targetSphere, Line *line, ScalarEntityDistance *distance){
    if(targetSphere.getIsSolved() && line->getIsSolved() && distance->getIsSolved()){ //if all elements are solved
        if(targetSphere.getIsUpdated() && line->getIsUpdated() && distance->getIsUpdated()){ //if all elements are in the same coordinate system
            FunctionConfiguration myConfig = this->getFunctionConfiguration();
            QMap<QString, QString> stringParameter = myConfig.stringParameter;
            bool invert = false;
            if(stringParameter.contains("invert")){
                QString value = static_cast<QString>(stringParameter.find("invert").value());
                if(value.compare("yes", Qt::CaseInsensitive) == 0){ //if yes was selected
                    invert = true;
                }
            }
            if(invert){
                targetSphere.xyz = targetSphere.xyz - distance->getDistance() * line->ijk;
            }else{
                targetSphere.xyz = targetSphere.xyz + distance->getDistance() * line->ijk;
            }
            targetSphere.xyz.setAt(3, 1.0);
        }
    }
}
