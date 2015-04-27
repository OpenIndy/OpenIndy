#include "p_register.h"

/*!
 * \brief Register::init
 */
void Register::init(){

    //set plugin meta data
    this->metaData.name = "Register";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "bra";
    this->metaData.description = QString("%1")
            .arg("This function projects the target geometry in the specified plane.");
    this->metaData.iid = "de.openIndy.plugin.function.objectTransformation.v001";

    //set needed elements
    NeededElement param1;
    param1.description = "Select a plane in which the target geometry is to be projected.";
    param1.infinite = false;
    param1.typeOfElement = ePlaneElement;
    this->neededElements.append(param1);

    //set spplicable for
    this->applicableFor.append(ePointFeature);
    this->applicableFor.append(eSphereFeature);

}

/*!
 * \brief Register::exec
 * \param point
 * \return
 */
bool Register::exec(Point &point){
    return this->setUpResult(point);
}

/*!
 * \brief Register::exec
 * \param sphere
 * \return
 */
bool Register::exec(Sphere &sphere){
    return this->setUpResult(sphere);
}

/*!
 * \brief Register::setUpResult
 * \param point
 * \return
 */
bool Register::setUpResult(Point &point){

    //get and check plane
    if(!this->inputElements.contains(0) || this->inputElements[0].size() != 1){
        return false;
    }
    QPointer<Plane> plane = this->inputElements[0].at(0).plane;
    if(plane.isNull() || !plane->getIsSolved()){
        return false;
    }

    //get the position of the point and the plane and the normal vector
    OiVec n_plane = plane->getDirection().getVector();
    n_plane.normalize();
    OiVec x_plane = plane->getPosition().getVector();
    OiVec x_point = point.getPosition().getVector();

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

    //project the point position into the plane
    x_point = x_point - s * n_plane;

    //set result
    Position position = point.getPosition();
    position.setVector(x_point);
    point.setPoint(position);

    return true;

}

/*!
 * \brief Register::setUpResult
 * \param sphere
 * \return
 */
bool Register::setUpResult(Sphere &sphere){

    //get and check plane
    if(!this->inputElements.contains(0) || this->inputElements[0].size() != 1){
        return false;
    }
    QPointer<Plane> plane = this->inputElements[0].at(0).plane;
    if(plane.isNull() || !plane->getIsSolved()){
        return false;
    }

    //get the position of the sphere and the plane and the normal vector
    OiVec n_plane = plane->getDirection().getVector();
    n_plane.normalize();
    OiVec x_plane = plane->getPosition().getVector();
    OiVec x_sphere = sphere.getPosition().getVector();

    //calculate the distance of the plane from the origin
    double d;
    OiVec::dot(d, x_plane, n_plane);
    if(d < 0.0){
        n_plane = -1.0 * n_plane;
        d = -d;
    }

    //calculate the distance of the sphere position from the plane
    double s;
    OiVec::dot(s, x_sphere, n_plane);
    s = s - d;

    //project the sphere position into the plane
    x_sphere = x_sphere - s * n_plane;

    //set result
    Position position = sphere.getPosition();
    position.setVector(x_sphere);
    Radius radius = sphere.getRadius();
    sphere.setSphere(position, radius);

    return true;

}
