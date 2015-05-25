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
            .arg("This function changes the orientation of a plane according to the specified point.");
    this->metaData.iid = "de.openIndy.plugin.function.objectTransformation.v001";

    //set needed elements
    NeededElement param1;
    param1.description = "Select a point used for the orientation of the plane.";
    param1.infinite = false;
    param1.typeOfElement = ePointElement;
    this->neededElements.append(param1);

    //set spplicable for
    this->applicableFor.append(ePlaneFeature);

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
 * \brief RectifyToPoint::setUpResult
 * \param plane
 * \return
 */
bool RectifyToPoint::setUpResult(Plane &plane){

    //get and check point
    if(!this->inputElements.contains(0) || this->inputElements[0].size() != 1){
        return false;
    }
    QPointer<Point> point = this->inputElements[0].at(0).point;
    if(point.isNull() || !point->getIsSolved()){
        return false;
    }

    //get the sense (positive or negative)
    float sense = 1.0;
    if(this->scalarInputParams.stringParameter.contains("sense")){
        if(this->scalarInputParams.stringParameter.value("sense").compare("negative") == 0){
            sense = -1.0;
        }
    }

    //get the position of the point and the plane and the normal vector
    OiVec n_plane = plane.getDirection().getVector();
    n_plane.normalize();
    OiVec x_plane = plane.getPosition().getVector();
    OiVec x_point = point->getPosition().getVector();

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
    Position position = plane.getPosition();
    Direction direction = plane.getDirection();
    position.setVector(x_point);
    direction.setVector(n_plane);
    plane.setPlane(position, direction);

    return true;

}
