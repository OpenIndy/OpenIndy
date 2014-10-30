#include "p_translateByPlane.h"

/*!
 * \brief TranslateByPlane::getMetaData
 * \return
 */
PluginMetaData* TranslateByPlane::getMetaData() const{
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "TranslateByPlane";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1 %2 %3")
            .arg("This function shifts the target geometry along the normal vector of the specified plane.")
            .arg("<br><br> <u>parameter description:</u> <br>")
            .arg("<b>invert:</b> Defines wether the normal vector shall be inverted before the target geometry is moved");
    metaData->iid = "de.openIndy.Plugin.Function.ObjectTransformation.v001";
    return metaData;
}

/*!
 * \brief TranslateByPlane::getNeededElements
 * \return
 */
QList<InputParams> TranslateByPlane::getNeededElements() const{
    QList<InputParams> result;
    InputParams param1;
    param1.index = 0;
    param1.description = "Select a plane whose normal vector is used to shift the target geometry.";
    param1.infinite = false;
    param1.typeOfElement = Configuration::ePlaneElement;
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
 * \brief TranslateByPlane::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> TranslateByPlane::applicableFor() const{
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::ePointFeature);
    result.append(Configuration::eLineFeature);
    result.append(Configuration::ePlaneFeature);
    result.append(Configuration::eSphereFeature);
    return result;
}

/*!
 * \brief TranslateByPlane::getStringParameter
 * \return
 */
QMap<QString, QStringList> TranslateByPlane::getStringParameter() const{
    QMap<QString, QStringList> result;
    QString key = "invert";
    QStringList value;
    value.append("no");
    value.append("yes");
    result.insert(key, value);
    return result;
}

/*!
 * \brief TranslateByPlane::exec
 * \param targetPoint
 * \return
 */
bool TranslateByPlane::exec(Point &targetPoint){
    if(this->isValid()){
        Plane *plane = this->getPlane();
        ScalarEntityDistance *seDistance = this->getDistance();
        if(plane != NULL && seDistance != NULL){
            OiVec n(3);
            n.setAt(0, plane->ijk.getAt(0));
            n.setAt(1, plane->ijk.getAt(1));
            n.setAt(2, plane->ijk.getAt(2));
            OiVec n0 = n.normalize();
            n0.add(1.0);
            plane->ijk = n0;
            this->shiftPoint(targetPoint, plane, seDistance);
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
 * \brief TranslateByPlane::exec
 * \param targetLine
 * \return
 */
bool TranslateByPlane::exec(Line &targetLine){
    if(this->isValid()){
        Plane *plane = this->getPlane();
        ScalarEntityDistance *seDistance = this->getDistance();
        if(plane != NULL && seDistance != NULL){
            OiVec n(3);
            n.setAt(0, plane->ijk.getAt(0));
            n.setAt(1, plane->ijk.getAt(1));
            n.setAt(2, plane->ijk.getAt(2));
            OiVec n0 = n.normalize();
            n0.add(1.0);
            plane->ijk = n0;
            this->shiftLine(targetLine, plane, seDistance);
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
 * \brief TranslateByPlane::exec
 * \param targetPlane
 * \return
 */
bool TranslateByPlane::exec(Plane &targetPlane){
    if(this->isValid()){
        Plane *plane = this->getPlane();
        ScalarEntityDistance *seDistance = this->getDistance();
        if(plane != NULL && seDistance != NULL){
            OiVec n(3);
            n.setAt(0, plane->ijk.getAt(0));
            n.setAt(1, plane->ijk.getAt(1));
            n.setAt(2, plane->ijk.getAt(2));
            OiVec n0 = n.normalize();
            n0.add(1.0);
            plane->ijk = n0;
            this->shiftPlane(targetPlane, plane, seDistance);
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
 * \brief TranslateByPlane::exec
 * \param targetSphere
 * \return
 */
bool TranslateByPlane::exec(Sphere &targetSphere){
    if(this->isValid()){
        Plane *plane = this->getPlane();
        ScalarEntityDistance *seDistance = this->getDistance();
        if(plane != NULL && seDistance != NULL){
            OiVec n(3);
            n.setAt(0, plane->ijk.getAt(0));
            n.setAt(1, plane->ijk.getAt(1));
            n.setAt(2, plane->ijk.getAt(2));
            OiVec n0 = n.normalize();
            n0.add(1.0);
            plane->ijk = n0;
            this->shiftSphere(targetSphere, plane, seDistance);
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
 * \brief TranslateByPlane::getPlane
 * \return
 */
Plane* TranslateByPlane::getPlane(){
    Plane *result = NULL;
    foreach(Plane *p, this->planes){
        if(result == NULL && p->getIsSolved()){
            result = p;
            this->setUseState(result->getId(), true);
        }else{
            this->setUseState(p->getId(), false);
        }
    }
    return result;
}

/*!
 * \brief TranslateByPlane::getDistance
 * \return
 */
ScalarEntityDistance* TranslateByPlane::getDistance(){
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
 * \brief TranslateByPlane::shiftPoint
 * \param targetPoint
 * \param plane
 * \param distance
 */
void TranslateByPlane::shiftPoint(Point &targetPoint, Plane *plane, ScalarEntityDistance *distance){
    if(targetPoint.getIsSolved() && plane->getIsSolved() && distance->getIsSolved()){ //if all elements are solved
        if(targetPoint.getIsUpdated() && plane->getIsUpdated() && distance->getIsUpdated()){ //if all elements are in the same coordinate system
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
                targetPoint.xyz = targetPoint.xyz - distance->getDistance() * plane->ijk;
            }else{
                targetPoint.xyz = targetPoint.xyz + distance->getDistance() * plane->ijk;
            }
            targetPoint.xyz.setAt(3, 1.0);
        }
    }
}

/*!
 * \brief TranslateByPlane::shiftLine
 * \param targetLine
 * \param plane
 * \param distance
 */
void TranslateByPlane::shiftLine(Line &targetLine, Plane *plane, ScalarEntityDistance *distance){
    if(targetLine.getIsSolved() && plane->getIsSolved() && distance->getIsSolved()){ //if all elements are solved
        if(targetLine.getIsUpdated() && plane->getIsUpdated() && distance->getIsUpdated()){ //if all elements are in the same coordinate system
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
                targetLine.xyz = targetLine.xyz - distance->getDistance() * plane->ijk;
            }else{
                targetLine.xyz = targetLine.xyz + distance->getDistance() * plane->ijk;
            }
            targetLine.xyz.setAt(3, 1.0);
        }
    }
}

/*!
 * \brief TranslateByPlane::shiftPlane
 * \param targetPlane
 * \param plane
 * \param distance
 */
void TranslateByPlane::shiftPlane(Plane &targetPlane, Plane *plane, ScalarEntityDistance *distance){
    if(targetPlane.getIsSolved() && plane->getIsSolved() && distance->getIsSolved()){ //if all elements are solved
        if(targetPlane.getIsUpdated() && plane->getIsUpdated() && distance->getIsUpdated()){ //if all elements are in the same coordinate system
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
                targetPlane.xyz = targetPlane.xyz - distance->getDistance() * plane->ijk;
            }else{
                targetPlane.xyz = targetPlane.xyz + distance->getDistance() * plane->ijk;
            }
            targetPlane.xyz.setAt(3, 1.0);
        }
    }
}

/*!
 * \brief TranslateByPlane::shiftSphere
 * \param targetSphere
 * \param plane
 * \param distance
 */
void TranslateByPlane::shiftSphere(Sphere &targetSphere, Plane *plane, ScalarEntityDistance *distance){
    if(targetSphere.getIsSolved() && plane->getIsSolved() && distance->getIsSolved()){ //if all elements are solved
        if(targetSphere.getIsUpdated() && plane->getIsUpdated() && distance->getIsUpdated()){ //if all elements are in the same coordinate system
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
                targetSphere.xyz = targetSphere.xyz - distance->getDistance() * plane->ijk;
            }else{
                targetSphere.xyz = targetSphere.xyz + distance->getDistance() * plane->ijk;
            }
            targetSphere.xyz.setAt(3, 1.0);
        }
    }
}
