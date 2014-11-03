#include "oistakeout.h"

QList<OiStakeOut *> OiStakeOut::currentStakeOuts;

OiStakeOut::OiStakeOut(QObject *parent) : QObject(parent){
    this->id = Configuration::generateID();
    this->lastGeometryId = -1;
}

/*!
 * \brief OiStakeOut::startStakeOut
 * \param mode
 * \param allGeoms
 * \param groups
 * \param geometries
 * \return
 */
int OiStakeOut::startStakeOut(OiStakeOut::StakeOutMode mode, bool allGeoms, QStringList groups, QList<int> geometries){

    if(allGeoms == false && geometries.size() == 0){
        return -1;
    }

    //create the stake out
    OiStakeOut *stakeOut;
    stakeOut = new OiStakeOut();
    stakeOut->mode = mode;
    OiStakeOut::currentStakeOuts.append(stakeOut);

    //add the selected geometries to the new stake out
    if(allGeoms){
        const QList<FeatureWrapper *> geoms = OiFeatureState::getGeometries();
        foreach(FeatureWrapper *geom, geoms){
            if(geom != NULL && geom->getGeometry() != NULL && geom->getGeometry()->getIsNominal()){
                stakeOut->geometries.insert(geom->getGeometry()->getId(), false);
            }
        }
    }else{
        foreach(int id, geometries){
            FeatureWrapper *geom = OiFeatureState::getFeature(id);
            if(geom != NULL && geom->getGeometry() != NULL && geom->getGeometry()->getIsNominal()){
                stakeOut->geometries.insert(geom->getGeometry()->getId(), false);
            }
        }
    }

    return stakeOut->id;

}

/*!
 * \brief OiStakeOut::stopStakeOut
 * Stop the stake out with the ID id
 * \param id
 * \return
 */
bool OiStakeOut::stopStakeOut(int id){

    OiStakeOut *stakeOut = OiStakeOut::getStakeOut(id);

    if(stakeOut != NULL){
        OiStakeOut::currentStakeOuts.removeOne(stakeOut);
        return true;
    }

    return false;

}

/*!
 * \brief OiStakeOut::getNextGeometry
 * Get the next unmeasured geometry from the stake out with ID id
 * \param id
 * \return
 */
FeatureWrapper *OiStakeOut::getNextGeometry(int id){

    OiStakeOut *stakeOut = OiStakeOut::getStakeOut(id);

    if(stakeOut == NULL){
        return NULL;
    }

    if(stakeOut->lastGeometryId >= 0){ //if this is not the first geometry to stake out

        switch(stakeOut->mode){
        case OiStakeOut::eSequence:
            return stakeOut->getSequenceGeometry();
        case OiStakeOut::eNearest:
            return stakeOut->getNearestGeometry();
        }

    }else{ //if this is the first geometry to stake out

        if(stakeOut->geometries.size() > 0){
            int fid = stakeOut->geometries.firstKey();
            FeatureWrapper *geom = OiFeatureState::getFeature(fid);
            if(geom != NULL && geom->getGeometry() != NULL){
                stakeOut->geometries.insert(fid, true);
                stakeOut->lastGeometryId = fid;
                return geom;
            }
        }else{
            return NULL;
        }

    }

}

/*!
 * \brief OiStakeOut::getStakeOut
 * \param id
 * \return
 */
OiStakeOut *OiStakeOut::getStakeOut(int id){

    foreach(OiStakeOut *stakeOut, OiStakeOut::currentStakeOuts){
        if(stakeOut != NULL && stakeOut->id == id){
            return stakeOut;
        }
    }

    return NULL;

}

/*!
 * \brief OiStakeOut::getSequenceGeometry
 * \return
 */
FeatureWrapper *OiStakeOut::getSequenceGeometry(){

    return NULL;

}

/*!
 * \brief OiStakeOut::getNearestGeometry
 * \return
 */
FeatureWrapper *OiStakeOut::getNearestGeometry(){

    FeatureWrapper *lastGeom = OiFeatureState::getFeature(this->lastGeometryId);
    OiVec lastGeomXyz = lastGeom->getGeometry()->getXYZ();

    if(lastGeom == NULL || lastGeom->getGeometry() == NULL && lastGeomXyz.getSize() >= 3){
        return NULL;
    }

    double nearestDistance = numeric_limits<double>::max();
    FeatureWrapper *nearestGeom = NULL;

    //iterate through all geometries that shall be staked out
    QMapIterator<int, bool> it(this->geometries);
    while(it.hasNext()){

        it.next();

        //continue with the next geometry if the current one has already been staked out
        if(it.value()){
            continue;
        }

        //get the geometry and check its distance
        FeatureWrapper *geom = OiFeatureState::getFeature(it.key());
        OiVec geomXyz = geom->getGeometry()->getXYZ();
        if(geom != NULL && geom->getGeometry() != NULL && geomXyz.getSize() >= 3){
            double distance = qSqrt( (lastGeomXyz.getAt(0)-geomXyz.getAt(0))*(lastGeomXyz.getAt(0)-geomXyz.getAt(0))
                                     + (lastGeomXyz.getAt(1)-geomXyz.getAt(1))*(lastGeomXyz.getAt(1)-geomXyz.getAt(1))
                                     + (lastGeomXyz.getAt(2)-geomXyz.getAt(2))*(lastGeomXyz.getAt(2)-geomXyz.getAt(2)));
            if(distance < nearestDistance){
                nearestDistance = distance;
                nearestGeom = geom;
            }
        }

    }

    //the nearest geometry shall not be considered next time
    if(nearestGeom == NULL){
        return NULL;
    }

    this->geometries.insert(nearestGeom->getGeometry()->getId(), true);
    this->lastGeometryId = nearestGeom->getGeometry()->getId();

    return nearestGeom;

}
