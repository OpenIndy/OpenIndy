#include "p_rectifytopoint.h"

/*!
 * \brief RectifyToPoint::init
 */
void RectifyToPoint::init(){

    //set plugin meta data
    this->metaData.name = "RectifyToPoint";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "bra";
    this->metaData.description = QString("%1")
            .arg("This function changes the orientation of a geometry according to the specified position.");
    this->metaData.iid = "de.openIndy.plugin.function.objectTransformation.v001";

    //set needed elements
    NeededElement param1;
    param1.description = "Select a position used for the orientation of the target geometry.";
    param1.infinite = false;
    param1.typeOfElement = ePositionElement;
    this->neededElements.append(param1);

    //set spplicable for
    this->applicableFor.append(ePlaneFeature);
    this->applicableFor.append(eCircleFeature);

    //set string parameter
    this->stringParameters.insert("sense", "negative");
    this->stringParameters.insert("sense", "positive");

}

/*!
 * \brief RectifyToPoint::exec
 * \param plane
 * \return
 */
bool RectifyToPoint::exec(Plane &plane){
    return this->setUpResult(plane);
}

/*!
 * \brief RectifyToPoint::exec
 * \param circle
 * \return
 */
bool RectifyToPoint::exec(Circle &circle){
    return this->setUpResult(circle);
}

/*!
 * \brief RectifyToPoint::setUpResult
 * \param plane
 * \return
 */
bool RectifyToPoint::setUpResult(Plane &plane){

    //get and check position
    if(!this->inputElements.contains(0) || this->inputElements[0].size() != 1){
        return false;
    }
    QPointer<Geometry> geometry = this->inputElements[0].at(0).geometry;
    if(geometry.isNull() || !geometry->getIsSolved() || !geometry->hasPosition()){
        return false;
    }

    //get the sense (positive or negative)
    double sense = 1.0;
    if(this->scalarInputParams.stringParameter.contains("sense")){
        if(this->scalarInputParams.stringParameter.value("sense").compare("negative") == 0){
            sense = -1.0;
        }
    }

    //get the position of the point and the plane and the normal vector
    OiVec n_plane = plane.getDirection().getVector();
    n_plane.normalize();
    OiVec x_plane = plane.getPosition().getVector();
    OiVec x_point = geometry->getPosition().getVector();

    //calculate the distance of the plane from the origin
    double d;
    OiVec::dot(d, x_plane, n_plane);
    if(d < 0.0){
        n_plane = -1.0 * n_plane;
        d = -d;
    }

    //calculate the distance of the point position from the plane
    double s;
    OiVec::dot(s, x_point, n_plane);
    s = s - d;

    //invert the distance if sense is negative
    s = s * sense;

    //invert the normal vector of the plane if it has the wrong orientation
    if(s < 0.0){
        n_plane = -1.0 * n_plane;
    }

    //set result
    Direction direction = plane.getDirection();
    direction.setVector(n_plane);
    plane.setPlane(plane.getPosition(), direction);

    return true;

}

/*!
 * \brief RectifyToPoint::setUpResult
 * \param circle
 * \return
 */
bool RectifyToPoint::setUpResult(Circle &circle){

    //get and check position
    if(!this->inputElements.contains(0) || this->inputElements[0].size() != 1){
        return false;
    }
    QPointer<Geometry> geometry = this->inputElements[0].at(0).geometry;
    if(geometry.isNull() || !geometry->getIsSolved() || !geometry->hasPosition()){
        return false;
    }

    //get the sense (positive or negative)
    double sense = 1.0;
    if(this->scalarInputParams.stringParameter.contains("sense")){
        if(this->scalarInputParams.stringParameter.value("sense").compare("negative") == 0){
            sense = -1.0;
        }
    }

    //get the position of the point and the circle and the normal vector
    OiVec n_circle = circle.getDirection().getVector();
    n_circle.normalize();
    OiVec x_circle = circle.getPosition().getVector();
    OiVec x_point = geometry->getPosition().getVector();

    //calculate the distance of the circle plane from the origin
    double d;
    OiVec::dot(d, x_circle, n_circle);
    if(d < 0.0){
        n_circle = -1.0 * n_circle;
        d = -d;
    }

    //calculate the distance of the point position from the circle plane
    double s;
    OiVec::dot(s, x_point, n_circle);
    s = s - d;

    //invert the distance if sense is negative
    s = s * sense;

    //invert the normal vector of the circle if it has the wrong orientation
    if(s < 0.0){
        n_circle = -1.0 * n_circle;
    }

    //set result
    Direction direction = circle.getDirection();
    direction.setVector(n_circle);
    circle.setCircle(circle.getPosition(), direction, circle.getRadius());

    return true;

}
