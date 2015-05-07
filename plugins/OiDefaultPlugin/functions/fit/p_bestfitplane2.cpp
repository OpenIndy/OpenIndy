#include "p_bestfitplane2.h"

/*!
 * \brief BestFitPlane2::init
 */
void BestFitPlane2::init(){

    //set plugin meta data
    this->metaData.name = "BestFitPlane2";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "kern";
    this->metaData.description = QString("%1 %2")
            .arg("This function calculates an adjusted plane.")
            .arg("You can input as many observations as you want which are then used to find the best fit plane.");
    this->metaData.iid = "de.openIndy.plugin.function.fitFunction.v001";

    //set needed elements
    this->neededElements.clear();
    NeededElement param1;
    param1.description = "Select at least three observations to calculate the best fit plane.";
    param1.infinite = true;
    param1.typeOfElement = eObservationElement;
    this->neededElements.append(param1);

    //set spplicable for
    this->applicableFor.append(ePlaneFeature);

}

/*!
 * \brief BestFitPlane2::exec
 * \param plane
 * \return
 */
bool BestFitPlane2::exec(Plane &plane){

    //get and check input observations
    if(!this->inputElements.contains(0) || this->inputElements[0].size() < 3){
        emit this->sendMessage(QString("Not enough valid observations to fit the plane %1").arg(plane.getFeatureName()));
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
    if(inputObservations.size() < 3){
        emit this->sendMessage(QString("Not enough valid observations to fit the plane %1").arg(plane.getFeatureName()));
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

    //adjust
    double d;
    double n[3];
    double qxx[4*4];
    if( !fitting_plane(x, y, z, inputObservations.size() , n, &d, qxx) ){
        emit this->sendMessage(QString("Unknown error while fitting plane %1").arg(plane.getFeatureName()));
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
    Direction planeNormal;
    planeNormal.setVector(n[0], n[1], n[2]);
    plane.setPlane(planeNormal, d);

    return true;

}
