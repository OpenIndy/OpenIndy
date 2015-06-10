#include "p_bestfitsphere_kern.h"

/*!
 * \brief BestFitSphere_kern::init
 */
void BestFitSphere_kern::init(){

    //set plugin meta data
    this->metaData.name = "BestFitSphere_kern";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "kern";
    this->metaData.description = QString("%1 %2")
            .arg("This function calculates an adjusted sphere.")
            .arg("You can input as many points as you want which are then used to find the best fit sphere.");
    this->metaData.iid = "de.openIndy.plugin.function.fitFunction.v001";

    //set needed elements
    this->neededElements.clear();
    NeededElement param1;
    param1.description = "Select at least four observations to calculate the best fit sphere.";
    param1.infinite = true;
    param1.typeOfElement = eObservationElement;
    this->neededElements.append(param1);

    //set spplicable for
    this->applicableFor.append(eSphereFeature);

}

/*!
 * \brief BestFitSphere_kern::exec
 * \param sphere
 * \return
 */
bool BestFitSphere_kern::exec(Sphere &sphere){
    return this->setUpResult(sphere);
}

/*!
 * \brief BestFitSphere_kern::setUpResult
 * \param sphere
 * \return
 */
bool BestFitSphere_kern::setUpResult(Sphere &sphere){

    //get and check input observations
    if(!this->inputElements.contains(0) || this->inputElements[0].size() < 4){
        emit this->sendMessage(QString("Not enough valid observations to fit the sphere %1").arg(sphere.getFeatureName()));
        return false;
    }
    QList<QPointer<Observation> > inputObservations;
    foreach(const InputElement &element, this->inputElements[0]){
        if(!element.observation.isNull() && element.observation->getIsSolved() && element.observation->getIsValid()){
            inputObservations.append(element.observation);
            this->setUseState(0, element.id, true);
            continue;
        }
        this->setUseState(0, element.id, false);
    }
    if(inputObservations.size() < 4){
        emit this->sendMessage(QString("Not enough valid observations to fit the sphere %1").arg(sphere.getFeatureName()));
        return false;
    }

    //fill x,y,z arrays
    double *x = new double[inputObservations.size()];
    double *y = new double[inputObservations.size()];
    double *z = new double[inputObservations.size()];
    for(int i = 0; i < inputObservations.size(); i++){
        x[i] = inputObservations[i]->getXYZ().getAt(0);
        y[i] = inputObservations[i]->getXYZ().getAt(1);
        z[i] = inputObservations[i]->getXYZ().getAt(2);
    }

    //calculate centroid of given observations
    double centroid[3];
    centroid[0] = 0.0;
    centroid[1] = 0.0;
    centroid[2] = 0.0;
    for(int i = 0; i < inputObservations.size(); i++){
        centroid[0] += x[i];
        centroid[1] += y[i];
        centroid[2] += z[i];
    }
    centroid[0] = centroid[0] / (float)inputObservations.size();
    centroid[1] = centroid[1] / (float)inputObservations.size();
    centroid[2] = centroid[2] / (float)inputObservations.size();

    //reduce observations by centroid
    for(int i = 0; i < inputObservations.size(); i++){
        x[i] = x[i] - centroid[0];
        y[i] = y[i] - centroid[1];
        z[i] = z[i] - centroid[2];
    }

    //adjust
    double r;
    double xm[3];
    double qxx[4*4];
    if( !fitting_sphere(x, y, z, inputObservations.size() , xm, &r, qxx) ){
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
