#include "p_changeradius.h"

/*!
 * \brief ChangeRadius::init
 */
void ChangeRadius::init(){

    //set plugin meta data
    this->metaData.name = "ChangeRadius";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "bra";
    this->metaData.description = QString("%1")
            .arg("This functions adds the specified offset to the radius of the target geometry");
    this->metaData.iid = "de.openIndy.plugin.function.objectTransformation.v001";

    //set spplicable for
    this->applicableFor.append(eSphereFeature);

    //set double parameter
    this->doubleParameters.insert("offset", 0.0);

}

/*!
 * \brief ChangeRadius::exec
 * \param sphere
 * \return
 */
bool ChangeRadius::exec(Sphere &sphere){
    return this->setUpResult(sphere);
}

/*!
 * \brief ChangeRadius::setUpResult
 * \param sphere
 * \return
 */
bool ChangeRadius::setUpResult(Sphere &sphere){

    //get and check offset
    double offset = 0.0;
    if(this->scalarInputParams.doubleParameter.contains("offset")){
        offset = this->scalarInputParams.doubleParameter.value("offset");
    }else{
        offset = this->doubleParameters.value("offset");
    }

    //add offset to radius of the sphere
    Position position = sphere.getPosition();
    Radius radius = sphere.getRadius();
    radius.setRadius(radius.getRadius() + offset);

    //set result
    sphere.setSphere(position, radius);

    return true;

}
