#include "p_translatebyvalue.h"

/*!
 * \brief TranslateByValue::init
 */
void TranslateByValue::init(){

    //set plugin meta data
    this->metaData.name = "TranslateByValue";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "bra";
    this->metaData.description = QString("%1")
            .arg("This function shifts the target geometry along the input vector.");
    this->metaData.iid = "de.openIndy.plugin.function.objectTransformation.v001";

    //set needed elements
    NeededElement param1;
    param1.description = "Select a geometry that has a vector.";
    param1.infinite = false;
    param1.typeOfElement = eDirectionElement;
    this->neededElements.append(param1);

    //set spplicable for
    this->applicableFor.append(ePointFeature);
    this->applicableFor.append(eLineFeature);
    this->applicableFor.append(ePlaneFeature);
    this->applicableFor.append(eSphereFeature);

    //set double parameter
    this->doubleParameters.insert("offset", 0.0);

}

/*!
 * \brief TranslateByValue::exec
 * \param point
 * \return
 */
bool TranslateByValue::exec(Point &point){
    return this->setUpResult(point);
}

/*!
 * \brief TranslateByValue::exec
 * \param line
 * \return
 */
bool TranslateByValue::exec(Line &line){
    return this->setUpResult(line);
}

/*!
 * \brief TranslateByValue::exec
 * \param plane
 * \return
 */
bool TranslateByValue::exec(Plane &plane){
    return this->setUpResult(plane);
}

/*!
 * \brief TranslateByValue::exec
 * \param sphere
 * \return
 */
bool TranslateByValue::exec(Sphere &sphere){
    return this->setUpResult(sphere);
}

/*!
 * \brief TranslateByValue::setUpResult
 * \param point
 * \return
 */
bool TranslateByValue::setUpResult(Point &point){

    //get and check vector element
    if(!this->inputElements.contains(0) || this->inputElements[0].size() != 1){
        return false;
    }
    QPointer<Geometry> geometry = this->inputElements[0].at(0).geometry;
    if(geometry.isNull() || !geometry->getIsSolved() || !geometry->hasDirection()){
        return false;
    }

    //get and check offset
    double offset = 0.0;
    if(this->scalarInputParams.doubleParameter.contains("offset")){
        offset = this->scalarInputParams.doubleParameter.value("offset");
    }else{
        offset = this->doubleParameters.value("offset");
    }

    //get normalized vector
    OiVec vec = geometry->getDirection().getVector();
    vec.normalize();

    //shift point position
    OiVec x_point = point.getPosition().getVector();
    x_point = x_point + offset * vec;

    //set result
    Position position;
    position.setVector(x_point);
    point.setPoint(position);

}

/*!
 * \brief TranslateByValue::setUpResult
 * \param line
 * \return
 */
bool TranslateByValue::setUpResult(Line &line){

    //get and check vector element
    if(!this->inputElements.contains(0) || this->inputElements[0].size() != 1){
        return false;
    }
    QPointer<Geometry> geometry = this->inputElements[0].at(0).geometry;
    if(geometry.isNull() || !geometry->getIsSolved() || !geometry->hasDirection()){
        return false;
    }

    //get and check offset
    double offset = 0.0;
    if(this->scalarInputParams.doubleParameter.contains("offset")){
        offset = this->scalarInputParams.doubleParameter.value("offset");
    }else{
        offset = this->doubleParameters.value("offset");
    }

    //get normalized vector
    OiVec vec = geometry->getDirection().getVector();
    vec.normalize();

    //shift line position
    OiVec x_line = line.getPosition().getVector();
    x_line = x_line + offset * vec;

    //set result
    Position position;
    position.setVector(x_line);
    line.setLine(position, line.getDirection());

}

/*!
 * \brief TranslateByValue::setUpResult
 * \param plane
 * \return
 */
bool TranslateByValue::setUpResult(Plane &plane){

    //get and check vector element
    if(!this->inputElements.contains(0) || this->inputElements[0].size() != 1){
        return false;
    }
    QPointer<Geometry> geometry = this->inputElements[0].at(0).geometry;
    if(geometry.isNull() || !geometry->getIsSolved() || !geometry->hasDirection()){
        return false;
    }

    //get and check offset
    double offset = 0.0;
    if(this->scalarInputParams.doubleParameter.contains("offset")){
        offset = this->scalarInputParams.doubleParameter.value("offset");
    }else{
        offset = this->doubleParameters.value("offset");
    }

    //get normalized vector
    OiVec vec = geometry->getDirection().getVector();
    vec.normalize();

    //shift plane position
    OiVec x_plane = plane.getPosition().getVector();
    x_plane = x_plane + offset * vec;

    //set result
    Position position;
    position.setVector(x_plane);
    plane.setPlane(position, plane.getDirection());

}

/*!
 * \brief TranslateByValue::setUpResult
 * \param sphere
 * \return
 */
bool TranslateByValue::setUpResult(Sphere &sphere){

    //get and check vector element
    if(!this->inputElements.contains(0) || this->inputElements[0].size() != 1){
        return false;
    }
    QPointer<Geometry> geometry = this->inputElements[0].at(0).geometry;
    if(geometry.isNull() || !geometry->getIsSolved() || !geometry->hasDirection()){
        return false;
    }

    //get and check offset
    double offset = 0.0;
    if(this->scalarInputParams.doubleParameter.contains("offset")){
        offset = this->scalarInputParams.doubleParameter.value("offset");
    }else{
        offset = this->doubleParameters.value("offset");
    }

    //get normalized vector
    OiVec vec = geometry->getDirection().getVector();
    vec.normalize();

    //shift sphere position
    OiVec x_sphere = sphere.getPosition().getVector();
    x_sphere = x_sphere + offset * vec;

    //set result
    Position position;
    position.setVector(x_sphere);
    sphere.setSphere(position, sphere.getRadius());

}
