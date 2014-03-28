#include "p_bestfitcircle.h"

BestFitCircle::BestFitCircle()
{
}

PluginMetaData *BestFitCircle::getMetaData()
{
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "BestFitCircle";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "mloesler";
    metaData->description = QString("%1 %2")
            .arg("This function calculates an adjusted circle.")
            .arg("You can input as many observations as you want which are then used to find the best fit sphere.");
    metaData->iid = "de.openIndy.Plugin.Function.FitFunction.v001";
    //...
    return metaData;

}

bool BestFitCircle::exec(Circle &)
{

    this->writeToConsole("Hallo Kreis!");

}

QList<InputParams> BestFitCircle::getNeededElements()
{
    QList<InputParams> result;
//    InputParams param;
//    param.index = 0;
//    param.description = "Select at least four observations to calculate the best fit sphere.";
//    param.infinite = true;
//    param.typeOfElement = Configuration::eObservationElement;
//    result.append(param);

    InputParams param;
    param.index = 0;
    param.description = "Select at least four observations to calculate the best fit sphere.";
    param.infinite = true;
    param.typeOfElement = Configuration::ePointElement;
    result.append(param);

    return result;

}

QList<Configuration::FeatureTypes> BestFitCircle::applicableFor()
{
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::eCircleFeature);
    return result;
}
