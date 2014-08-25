#include "p_projectInPlane.h"

/*!
 * \brief ProjectInPlane::getMetaData
 * \return
 */
PluginMetaData* ProjectInPlane::getMetaData(){
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "ProjectInPlane";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1")
            .arg("This function projects the target geometry in the specified plane.");
    metaData->iid = "de.openIndy.Plugin.Function.ObjectTransformation.v001";
    return metaData;
}

/*!
 * \brief ProjectInPlane::getNeededElements
 * \return
 */
QList<InputParams> ProjectInPlane::getNeededElements(){
    QList<InputParams> result;
    InputParams param;
    param.index = 0;
    param.description = "Select a plane in which the target geometry is to be projected.";
    param.infinite = false;
    param.typeOfElement = Configuration::ePlaneElement;
    result.append(param);
    return result;
}

/*!
 * \brief ProjectInPlane::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> ProjectInPlane::applicableFor(){
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::ePointFeature);
    return result;
}

/*!
 * \brief ProjectInPlane::exec
 * \param targetPoint
 * \return
 */
bool ProjectInPlane::exec(Point &targetPoint){
    if(this->isValid()){
        Plane *plane = this->getPlane();
        if(plane != NULL){
            this->projectPoint(targetPoint, plane);
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
 * \brief ProjectInPlane::getPlane
 * \return
 */
Plane* ProjectInPlane::getPlane(){
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
 * \brief ProjectInPlane::projectPoint
 */
void ProjectInPlane::projectPoint(Point &point, Plane *plane){
    OiVec n(3);
    n.setAt(0, plane->ijk.getAt(0));
    n.setAt(1, plane->ijk.getAt(1));
    n.setAt(2, plane->ijk.getAt(2));
    OiVec n0 = n.normalize(); //normal vector of plane
    OiVec x_plane(3); //position vector of plane
    x_plane.setAt(0, plane->xyz.getAt(0));
    x_plane.setAt(1, plane->xyz.getAt(1));
    x_plane.setAt(2, plane->xyz.getAt(2));
    OiVec x_point(3); //position vector of point
    x_point.setAt(0, point.xyz.getAt(0));
    x_point.setAt(1, point.xyz.getAt(1));
    x_point.setAt(2, point.xyz.getAt(2));
    double d;
    OiVec::dot(d, x_plane, n0); //distance plane from origin
    if(d < 0.0){
        n0 = -1.0 * n0;
        d = -d;
    }
    double s;
    OiVec::dot(s, x_point, n0);
    s = s- d; //distance point from plane
    //project point in plane
    x_point = x_point - s * n0;
    x_point.add(1.0);
    point.xyz = x_point;
}
