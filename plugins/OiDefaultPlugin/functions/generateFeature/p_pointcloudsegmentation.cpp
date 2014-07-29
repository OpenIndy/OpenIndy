#include "p_pointcloudsegmentation.h"

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

QList<InputParams> PointCloudSegmentation::getNeededElements(){
    QList<InputParams> result;
    return result;
}

QList<Configuration::FeatureTypes> PointCloudSegmentation::applicableFor(){
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::ePointCloudFeature);
    return result;
}

bool PointCloudSegmentation::exec(PointCloud &p){
    if(this->isValid()){
        return true;
    }else{
        this->writeToConsole("PointCloud is not valid");
        return false;
    }

}

QStringList PointCloudSegmentation::getResultProtocol(){
    QStringList dummy;
    dummy.append("test protocol");
    return dummy;
}
