#include "p_spherefrompoints.h"

/*!
 * \brief SphereFromPoints::init
 */
void SphereFromPoints::init(){

    //set plugin meta data
    this->metaData.name = "SphereFromPoints";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "kern";
    this->metaData.description = QString("%1 %2")
            .arg("This function caclulates an adjusted sphere.")
            .arg("You can input as many points as you want which are then used to find the best fit sphere.");
    this->metaData.iid = "de.openIndy.plugin.function.constructFunction.v001";

    //set needed elements
    NeededElement param1;
    param1.description = "Select at least four points to calculate the best fit sphere.";
    param1.infinite = true;
    param1.typeOfElement = ePointElement;
    this->neededElements.append(param1);

    //set spplicable for
    this->applicableFor.append(eSphereFeature);

}

/*!
 * \brief SphereFromPoints::exec
 * \param sphere
 * \return
 */
bool SphereFromPoints::exec(Sphere &sphere){
    return this->setUpResult(sphere);
}

/*!
 * \brief SphereFromPoints::setUpResult
 * \param sphere
 * \return
 */
bool SphereFromPoints::setUpResult(Sphere &sphere){

    //get and check input points
    if(!this->inputElements.contains(0) || this->inputElements[0].size() < 4){
        emit this->sendMessage(QString("Not enough valid points to fit the sphere %1").arg(sphere.getFeatureName()));
        return false;
    }
    QList<QPointer<Point> > inputPoints;
    foreach(const InputElement &element, this->inputElements[0]){
        if(!element.point.isNull() && element.point->getIsSolved()){
            inputPoints.append(element.point);
            this->setUseState(0, element.id, true);
        }
        this->setUseState(0, element.id, false);
    }
    if(inputPoints.size() < 4){
        emit this->sendMessage(QString("Not enough valid points to fit the sphere %1").arg(sphere.getFeatureName()));
        return false;
    }

    //fill x,y,z arrays
    double *x = new double[inputPoints.size()];
    double *y = new double[inputPoints.size()];
    double *z = new double[inputPoints.size()];
    for(int i = 0; i < inputPoints.size(); i++){
        x[i] = inputPoints[i]->getPosition().getVector().getAt(0);
        y[i] = inputPoints[i]->getPosition().getVector().getAt(1);
        z[i] = inputPoints[i]->getPosition().getVector().getAt(2);
    }

    //calculate centroid of given points
    double centroid[3];
    centroid[0] = 0.0;
    centroid[1] = 0.0;
    centroid[2] = 0.0;
    for(int i = 0; i < inputPoints.size(); i++){
        centroid[0] += x[i];
        centroid[1] += y[i];
        centroid[2] += z[i];
    }
    centroid[0] = centroid[0] / (float)inputPoints.size();
    centroid[1] = centroid[1] / (float)inputPoints.size();
    centroid[2] = centroid[2] / (float)inputPoints.size();

    //reduce points by centroid
    for(int i = 0; i < inputPoints.size(); i++){
        x[i] = x[i] - centroid[0];
        y[i] = y[i] - centroid[1];
        z[i] = z[i] - centroid[2];
    }

    //adjust
    double r;
    double xm[3];
    double qxx[4*4];
    if( !fitting_sphere(x, y, z, inputPoints.size() , xm, &r, qxx) ){
        emit this->sendMessage(QString("Unknown error while fitting sphere %1").arg(sphere.getFeatureName()));
        delete[] x;
        delete[] y;
        delete[] z;
        return false;
    }

    //free space
    delete[] x;
    delete[] y;
    delete[] z;

    //set result
    Position position;
    position.setVector(xm[0], xm[1], xm[2]);
    Radius radius;
    radius.setRadius(r);
    sphere.setSphere(position, radius);

    return true;

}
