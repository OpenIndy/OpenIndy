#include "p_pointcloudsegmentation.h"

/*!
 * \brief PointCloudSegmentation::getMetaData
 * \return
 */
PluginMetaData* PointCloudSegmentation::getMetaData(){
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "PointCloudSegmentation";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "Benedikt Rauls";
    metaData->description = QString("%1 %2")
            .arg("This function segments a pointcloud.")
            .arg("It is able to detect planes, spheres and/or cylinders based on the given input parameters.");
    metaData->iid = "de.openIndy.Plugin.Function.GenerateFeatureFunction.v001";
    return metaData;
}

/*!
 * \brief PointCloudSegmentation::getNeededElements
 * \return
 */
QList<InputParams> PointCloudSegmentation::getNeededElements(){
    QList<InputParams> result;
    return result;
}

/*!
 * \brief PointCloudSegmentation::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> PointCloudSegmentation::applicableFor(){
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::ePointCloudFeature);
    return result;
}

/*!
 * \brief PointCloudSegmentation::getIntegerParameter
 * \return
 */
QMap<QString, int> PointCloudSegmentation::getIntegerParameter(){

    QMap<QString, int> intParams;

    //let the user specify the maximum number of points in a leaf-node of the octree that will be computed
    intParams.insert("leafSize", 100);

    /* number of pointcloud-points that will be used to fit a shape
    (has no influence on the final fit after all shapes were detected) */
    intParams.insert("fitSampleSize", 20);

    //minimum number of pointcloud-points a shape has to contain to be detected (differences possible for each shape-type)
    intParams.insert("minPointsPlane", 1000);
    intParams.insert("minPointsSphere", 500);
    intParams.insert("minPointsCylinder", 1000);

    return intParams;

}

/*!
 * \brief PointCloudSegmentation::getDoubleParameter
 * \return
 */
QMap<QString, double> PointCloudSegmentation::getDoubleParameter(){

    QMap<QString, double> doubleParams;

    /* maximum orthogonal distance of a pointcloud-point from an estimated shape-surface
    so that the point is still associated with that shape (differences possible for each shape-type) */
    doubleParams.insert("maxDistancePlane", 0.005);
    doubleParams.insert("maxDistanceSphere", 0.002);
    doubleParams.insert("maxDistanceCylinder", 0.020);

    //minimum and maximum radius of a sphere respectively a cylinder that shall be detected in the pointcloud
    doubleParams.insert("minRadiusSphere", 0.004);
    doubleParams.insert("maxRadiusSphere", 0.015);
    doubleParams.insert("minRadiusCylinder", 0.004);
    doubleParams.insert("maxRadiusCylinder", 0.300);

    return doubleParams;

}

/*!
 * \brief PointCloudSegmentation::getStringParameter
 * \return
 */
QMap<QString, QStringList> PointCloudSegmentation::getStringParameter(){

    QMap<QString, QStringList> stringParams;

    QStringList myBoolOptions;
    myBoolOptions.append("true");
    myBoolOptions.append("false");

    QStringList myOutlierOptions;
    myOutlierOptions.append("0.1");
    myOutlierOptions.append("0.2");
    myOutlierOptions.append("0.3");
    myOutlierOptions.append("0.4");
    myOutlierOptions.append("0.5");
    myOutlierOptions.append("0.6");
    myOutlierOptions.append("0.7");
    myOutlierOptions.append("0.8");
    myOutlierOptions.append("0.9");

    //parameters to decide which shape-types (plane, sphere, cylinder) shall be detected
    stringParams.insert("detectPlanes", myBoolOptions);
    stringParams.insert("detectSpheres", myBoolOptions);
    stringParams.insert("detectCylinders", myBoolOptions);

    /* if true only voxel of similar size will be considered in region growing process, so that
    the proximity of shape-points is guaranteed. False ignores voxel-sizes */
    stringParams.insert("forceProximity", myBoolOptions);

    /*estimated percentage of outlier points in a leaf-voxel (between 0.1 and 0.9)
    0.1 means that most of the points in one leaf-voxel belong to the same shape and therefor nearly every
    combination of points (in that voxel) leads to the same shape.
    0.9 means that there many outlier points and only less points belong to the same shape, so that many combinations
    have to be checked till one gets a combination that leads to the desired shape */
    stringParams.insert("outlierPercentage", myOutlierOptions);

    return stringParams;

}

/*!
 * \brief PointCloudSegmentation::exec
 * \param p
 * \return
 */
bool PointCloudSegmentation::exec(PointCloud &p){
    if(this->isValid()){

        //check if there are some points so that segmentation does make sense
        if(p.getPointCount() > 10){

            return true;

        }else{
            this->writeToConsole("Segmentation is only applicable for pointclouds with at least 10 points");
            return false;
        }

    }else{
        this->writeToConsole("PointCloud is not valid");
        return false;
    }

}

/*!
 * \brief PointCloudSegmentation::getResultProtocol
 * \return
 */
QStringList PointCloudSegmentation::getResultProtocol(){
    QStringList dummy;
    dummy.append("test protocol");
    return dummy;
}
