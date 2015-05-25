#include "p_shift.h"

/*!
 * \brief Shift::init
 */
void Shift::init(){

    //set plugin meta data
    this->metaData.name = "Shift";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "bra";
    this->metaData.description = QString("%1")
            .arg("This function shifts the target geometry along its one vector.");
    this->metaData.iid = "de.openIndy.plugin.function.objectTransformation.v001";

    //set spplicable for
    this->applicableFor.append(ePlaneFeature);

    //set double parameter
    this->doubleParameters.insert("offset", 0.0);

}

/*!
 * \brief Shift::exec
 * \param plane
 * \return
 */
bool Shift::exec(Plane &plane){
    return this->setUpResult(plane);
}

/*!
 * \brief Shift::setUpResult
 * \param plane
 * \return
 */
bool Shift::setUpResult(Plane &plane){

    //get and check offset
    double offset = 0.0;
    if(this->scalarInputParams.doubleParameter.contains("offset")){
        offset = this->scalarInputParams.doubleParameter.value("offset");
    }else{
        offset = this->doubleParameters.value("offset");
    }

    //get normalized vector
    OiVec vec = plane.getDirection().getVector();
    vec.normalize();

    //shift plane position
    OiVec x_plane = plane.getPosition().getVector();
    x_plane = x_plane + offset * vec;

    //set result
    Position position;
    position.setVector(x_plane);
    plane.setPlane(position, plane.getDirection());

    return true;

}
