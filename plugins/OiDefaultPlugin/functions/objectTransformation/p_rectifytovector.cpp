#include "p_rectifytovector.h"

/*!
 * \brief RectifyToVector::init
 */
void RectifyToVector::init(){

    //set plugin meta data
    this->metaData.name = "RectifyToVector";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "bra";
    this->metaData.description = QString("%1")
            .arg("This function changes the orientation of a geometry according to the specified direction.");
    this->metaData.iid = "de.openIndy.plugin.function.objectTransformation.v001";

    //set needed elements
    NeededElement param1;
    param1.description = "Select a direction used for the orientation of the target geometry.";
    param1.infinite = false;
    param1.typeOfElement = eDirectionElement;
    this->neededElements.append(param1);

    //set spplicable for
    this->applicableFor.append(ePlaneFeature);
    this->applicableFor.append(eCircleFeature);

    //set string parameter
    this->stringParameters.insert("sense", "negative");
    this->stringParameters.insert("sense", "positive");

}

/*!
 * \brief RectifyToVector::exec
 * \param plane
 * \return
 */
bool RectifyToVector::exec(Plane &plane){
    return this->setUpResult(plane);
}

/*!
 * \brief RectifyToVector::exec
 * \param circle
 * \return
 */
bool RectifyToVector::exec(Circle &circle){
    return this->setUpResult(circle);
}

/*!
 * \brief RectifyToVector::setUpResult
 * \param plane
 * \return
 */
bool RectifyToVector::setUpResult(Plane &plane){

    //get and check point
    if(!this->inputElements.contains(0) || this->inputElements[0].size() != 1){
        return false;
    }
    QPointer<Geometry> geometry = this->inputElements[0].at(0).geometry;
    if(geometry.isNull() || !geometry->getIsSolved() || !geometry->hasDirection()){
        return false;
    }

    //get the sense (positive or negative)
    double sense = 1.0;
    if(this->scalarInputParams.stringParameter.contains("sense")){
        if(this->scalarInputParams.stringParameter.value("sense").compare("negative") == 0){
            sense = -1.0;
        }
    }

    //get the direction to compare
    OiVec r_reference = geometry->getDirection().getVector();
    r_reference.normalize();
    OiVec r_plane = plane.getDirection().getVector();
    r_plane.normalize();

    //calculate the angle between both directions
    double angle = 0.0;
    OiVec::dot(angle, r_reference, r_plane);
    angle = qAbs(qAcos(angle));

    //invert the normal vector if the angle is greater than 90°
    if(angle > PI/2.0){
        r_plane = -1.0 * r_plane;
    }

    //invert the normal vector if sense is negative
    r_plane = sense * r_plane;

    //set result
    Direction direction = plane.getDirection();
    direction.setVector(r_plane);
    plane.setPlane(plane.getPosition(), direction);

    return true;

}

/*!
 * \brief RectifyToVector::setUpResult
 * \param circle
 * \return
 */
bool RectifyToVector::setUpResult(Circle &circle){

    //get and check point
    if(!this->inputElements.contains(0) || this->inputElements[0].size() != 1){
        return false;
    }
    QPointer<Geometry> geometry = this->inputElements[0].at(0).geometry;
    if(geometry.isNull() || !geometry->getIsSolved() || !geometry->hasDirection()){
        return false;
    }

    //get the sense (positive or negative)
    double sense = 1.0;
    if(this->scalarInputParams.stringParameter.contains("sense")){
        if(this->scalarInputParams.stringParameter.value("sense").compare("negative") == 0){
            sense = -1.0;
        }
    }

    //get the direction to compare
    OiVec r_reference = geometry->getDirection().getVector();
    r_reference.normalize();
    OiVec r_circle = circle.getDirection().getVector();
    r_circle.normalize();

    //calculate the angle between both directions
    double angle = 0.0;
    OiVec::dot(angle, r_reference, r_circle);
    angle = qAbs(qAcos(angle));

    //invert the normal vector if the angle is greater than 90°
    if(angle > PI/2.0){
        r_circle = -1.0 * r_circle;
    }

    //invert the normal vector if sense is negative
    r_circle = sense * r_circle;

    //set result
    Direction direction = circle.getDirection();
    direction.setVector(r_circle);
    circle.setCircle(circle.getPosition(), direction, circle.getRadius());

    return true;

}
