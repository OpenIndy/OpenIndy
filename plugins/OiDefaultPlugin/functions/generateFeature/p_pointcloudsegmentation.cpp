#include "p_pointcloudsegmentation.h"

/*!
 * \brief PointCloudSegmentation::getMetaData
 * \return
 */
PluginMetaData* PointCloudSegmentation::getMetaData() const{
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "PointCloudSegmentation";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "Benedikt Rauls";
    metaData->description = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 %16 %17 %18 %19 %20")
    .arg("This function segments a pointcloud.")
    .arg("It is able to detect planes, spheres and/or cylinders based on the given input parameters.")
    .arg("<br><br> <u>parameter description:</u> <br>")
    .arg("<b>leafSize:</b> Maximum number of points in a leaf voxel.")
    .arg("<b>fitSampleSize:</b> Number of points to fit a geometry during region growing.")
    .arg("<b>minPointsPlane:</b> Minimum number of points in a plane.")
    .arg("<b>minPointsSphere:</b> Minimum number of points in a sphere.")
    .arg("<b>minPointsCylinder:</b> Minimum number of points in a cylinder.")
    .arg("<b>maxDistancePlane:</b> Maximum distance of a point from a plane.")
    .arg("<b>maxDistanceSphere:</b> Maximum distance of a point from a sphere.")
    .arg("<b>maxDistanceCylinder:</b> Maximum distance of a point from a cylinder.")
    .arg("<b>minRadiusSphere:</b> Minimum radius of a sphere.")
    .arg("<b>maxRadiusSphere:</b> Maximum radius of a sphere.")
    .arg("<b>minRadiusCylinder:</b> Minimum radius of a cylinder.")
    .arg("<b>maxRadiusCylinder:</b> Maximum radius of a cylinder.")
    .arg("<b>detectPlanes:</b> Defines wether the algorithm shall detect planes.")
    .arg("<b>detectPlanes:</b> Defines wether the algorithm shall detect spheres.")
    .arg("<b>detectPlanes:</b> Defines wether the algorithm shall detect cylinders.")
    .arg("<b>finalFit:</b> Defines wether the extracted geometries shall be fit using all points.")
    .arg("<b>outlierPercentage:</b> Estimated proportion of outliers in a leaf voxel.");
    metaData->iid = "de.openIndy.Plugin.Function.GenerateFeatureFunction.v001";
    return metaData;
}

/*!
 * \brief PointCloudSegmentation::getNeededElements
 * \return
 */
QList<InputParams> PointCloudSegmentation::getNeededElements() const{
    QList<InputParams> result;
    return result;
}

/*!
 * \brief PointCloudSegmentation::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> PointCloudSegmentation::applicableFor() const{
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::ePointCloudFeature);
    return result;
}

/*!
 * \brief PointCloudSegmentation::getIntegerParameter
 * \return
 */
QMap<QString, int> PointCloudSegmentation::getIntegerParameter() const{

    QMap<QString, int> intParams;

    //let the user specify the maximum number of points in a leaf-node of the octree that will be computed
    intParams.insert("leafSize", 100);

    /* number of pointcloud-points that will be used to fit a shape
    (has no influence on the final fit after all shapes were detected) */
    intParams.insert("fitSampleSize", 50);

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
QMap<QString, double> PointCloudSegmentation::getDoubleParameter() const{

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
QMap<QString, QStringList> PointCloudSegmentation::getStringParameter() const{

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

    //finally fit the detected geometries
    stringParams.insert("finalFit", myBoolOptions);

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

            PS_PointCloud myCloud;

            vector<PS_Point_PC*> *myPoints = new vector<PS_Point_PC*>;
            foreach(Point_PC *poi, p.getPointCloudPoints()){
                PS_Point_PC *nPoi = new PS_Point_PC();
                nPoi->isUsed = false;
                nPoi->xyz[0] = poi->xyz[0];
                nPoi->xyz[1] = poi->xyz[1];
                nPoi->xyz[2] = poi->xyz[2];
                myPoints->push_back(nPoi);
            }

            PS_BoundingBox_PC bbox;
            bbox.max[0] = p.getBoundingBox().max[0];
            bbox.max[1] = p.getBoundingBox().max[1];
            bbox.max[2] = p.getBoundingBox().max[2];
            bbox.min[0] = p.getBoundingBox().min[0];
            bbox.min[1] = p.getBoundingBox().min[1];
            bbox.min[2] = p.getBoundingBox().min[2];

            unsigned long numPoints = p.getPointCount();

            bool checkLoad = myCloud.setCloud(myPoints, bbox, numPoints);

            //if successfully retrieved the point cloud data
            if(checkLoad){

                /*
                 * get the user defined parameters
                 */

                FunctionConfiguration userConfig = this->getFunctionConfiguration();
                QMap<QString, QString> stringParams = userConfig.stringParameter;
                QMap<QString, int> intParams = userConfig.intParameter;
                QMap<QString, double> doubleParams = userConfig.doubleParameter;

                //special plane, sphere and cylinder parameter
                PlaneParameter pParam;
                pParam.detectPlanes = stringParams.value("detectPlanes").compare("false");
                pParam.maxDistance = doubleParams.value("maxDistancePlane");
                pParam.minPoints = intParams.value("minPointsPlane");
                SphereParameter sParam;
                sParam.detectSpheres = stringParams.value("detectSpheres").compare("false");
                sParam.maxDistance = doubleParams.value("maxDistanceSphere");
                sParam.maxRadius = doubleParams.value("maxRadiusSphere");
                sParam.minRadius = doubleParams.value("minRadiusSphere");
                sParam.minPoints = intParams.value("minPointsSphere");
                CylinderParameter cParam;
                cParam.detectCylinders = stringParams.value("detectCylinders").compare("false");
                cParam.maxDistance = doubleParams.value("maxDistanceCylinder");
                cParam.maxRadius = doubleParams.value("maxRadiusCylinder");
                cParam.minRadius = doubleParams.value("minRadiusCylinder");
                cParam.minPoints = intParams.value("minPointsCylinder");

                //set parameter for the segmentation algorithm
                PS_InputParameter param;
                param.leafSize = intParams.value("leafSize");
                param.outlierPercentage = stringParams.value("outlierPercentage").toDouble();
                qDebug() << "outlierper " << param.outlierPercentage;
                param.fitSampleSize = 50; intParams.value("fitSampleSize");
                param.finalFit = stringParams.value("finalFit").compare("false");
                param.planeParams = pParam;
                param.sphereParams = sParam;
                param.cylinderParams = cParam;

                this->myHandler = new SegmentationConsumer(p);
                this->myHandler->startSegmentationTask(myCloud, param);

                /*//set up the octree for the pointcloud
                myCloud.setUpOctree(param);

                //call method to detect shapes in the pointcloud
                myCloud.detectShapes(param);*/

            }else{
                this->writeToConsole("Point cloud data cannot be retrieved");
                return false;
            }

            this->writeToConsole("Point cloud segmentation successfully started");
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
QStringList PointCloudSegmentation::getResultProtocol() const{
    QStringList dummy;
    dummy.append("test protocol");
    return dummy;
}
