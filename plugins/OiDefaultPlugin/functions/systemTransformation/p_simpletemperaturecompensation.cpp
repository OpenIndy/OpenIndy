#include "p_simpletemperaturecompensation.h"

/*!
 * \brief SimpleTemperatureCompensation::init
 */
void SimpleTemperatureCompensation::init(){

    //set plugin meta data
    this->metaData.name = "StandardTempComp";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "jwa";
    this->metaData.description = QString("%1 %2")
            .arg("This functions calculates an equal temperature compensation value for x,y and z component.")
            .arg("Type in the actual and reference temperature and chose a material and get the three scales.");
    this->metaData.iid = "de.openIndy.plugin.function.systemTransformation.v001";

    //set spplicable for
    this->applicableFor.append(eTrafoParamFeature);

    //set string parameter
    QStringList materials = Materials::getMaterials();
    for(int i = 0; i < materials.size(); i++){
        this->stringParameters.insert("material", materials.at(i));
    }

    //set double parameter
    this->doubleParameters.insert("referenceTemperature", 20.0);
    this->doubleParameters.insert("actualTemperature", 20.0);
    this->doubleParameters.insert("temperatureAccuracy", 0.1);

}

/*!
 * \brief SimpleTemperatureCompensation::exec
 * \param trafoParam
 * \return
 */
bool SimpleTemperatureCompensation::exec(TrafoParam &trafoParam){
    this->calcExpansion(trafoParam);

    //set protocol
    this->resultProtocol.append("temperature compensation using the following parameters:");
    this->resultProtocol.append(QString("actual temperature [°C]: " + this->protActTemp));
    this->resultProtocol.append(QString("reference temperature [°C]: " + this->protRefTemp));
    this->resultProtocol.append(QString("accuracy of temperature measurement [°C]: " + this->protTempAccuracy));
    this->resultProtocol.append(QString("material: " + this->protMaterial));
    this->resultProtocol.append(QString("expansion coefficient [m]: " + this->protExpansionCoeff));
    this->resultProtocol.append(QString("expansion [m]/[m]: " + this->protExpansion));
    this->resultProtocol.append("accuracy calculated by temperature measurement accuracy * expansion");
    this->resultProtocol.append(QString("accuracy for expansion [m]: " + this->protSTDDEV));

    return true;
}

/*!
 * \brief calcExpansion estimates the thermal expansion value for corrections.
 * \param tp
 * \param SET
 */
void SimpleTemperatureCompensation::calcExpansion(TrafoParam &tp){

    QMap<QString,QString> stringParameter = this->scalarInputParams.stringParameter;
    QMap<QString,double> doubleParameter = this->scalarInputParams.doubleParameter;

    QString material = "";
    double actTemp = 20.0;
    double refTemp = 20.0;
    double tempAccuracy = 0.0;
    double expansionCoefficient = 0.0;
    if(stringParameter.contains("material")){
        material = static_cast<QString>(stringParameter.find("material").value());
        protMaterial = material;
        expansionCoefficient = Materials::getExpansionCoefficient(material);
        protExpansionCoeff = QString::number(expansionCoefficient,'f',6);
	}
	if(doubleParameter.contains("actualTemperature")){
        actTemp = static_cast<double>(doubleParameter.find("actualTemperature").value());
        protActTemp = QString::number(actTemp,'f',2);
    }
    if(doubleParameter.contains("referenceTemperature")){
        refTemp = static_cast<double>(doubleParameter.find("referenceTemperature").value());
        protRefTemp = QString::number(refTemp,'f',2);
    }
    if(doubleParameter.contains("temperatureAccuracy")){
        tempAccuracy = static_cast<double>(doubleParameter.find("temperatureAccuracy").value());
        protTempAccuracy = QString::number(tempAccuracy,'f',2);
    }
	
	double expansion = (actTemp-refTemp)*expansionCoefficient;
    protExpansion = QString::number(expansion,'f',6);
    double m = 1.0/(1+ (expansion));
	
	OiMat eMat(4,4);
        for(int i = 0; i < 4; i++){
            eMat.setAt(i,i, 1.0);
        }

        OiMat scale(4,4);
        scale.setAt(0,0,m);
        scale.setAt(1,1,m);
        scale.setAt(2,2,m);
        scale.setAt(3,3,1.0);

        tp.setTransformationParameters(eMat, eMat, scale);

        this->calcAccuracy(tp,tempAccuracy,expansion);

    this->calcAccuracy(tp,tempAccuracy,expansion);

}

/*!
 * \brief calcAccuracy estimates the accuracy of the transformation via the accuracy of temperature measurement.
 * \param tp
 * \param tempAccuracy
 * \param expansion
 */
void SimpleTemperatureCompensation::calcAccuracy(TrafoParam &tp, double tempAccuracy, double expansion)
{
    double stddev = tempAccuracy*(expansion);
    protSTDDEV = QString::number(stddev,'f',6);
    Statistic myStats;

    myStats.setS0APriori(1.0);
    myStats.setS0APosteriori(stddev);
    myStats.setStdev(stddev);
    myStats.setIsValid(true);

    tp.setStatistic(myStats);
    this->statistic = myStats;

}
