#include "p_simpletemperaturecompensation.h"

/*!
 * \brief SimpleTemperatureCompensation constructor
 */
SimpleTemperatureCompensation::SimpleTemperatureCompensation()
{
}

/*!
 * \brief getMetaData of the function
 * \return
 */
PluginMetaData *SimpleTemperatureCompensation::getMetaData()
{
    PluginMetaData* metaData = new PluginMetaData();
        metaData->name = "StandardTempComp";
        metaData->pluginName = "OpenIndy Default Plugin";
        metaData->author = "jw";
        metaData->description = QString("%1 %2")
                .arg("This functions calculates an equal temperature compensation value for x,y and z component.")
                .arg("Type in the actual and reference temperature and chose a material and get the three scales.");
        metaData->iid = "de.openIndy.Plugin.Function.SystemTransformation.v001";
        return metaData;
}

/*!
 * \brief getNeededElements
 * \return
 */
QList<InputParams> SimpleTemperatureCompensation::getNeededElements()
{
    QList<InputParams> result;
    return result;
}

/*!
 * \brief applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> SimpleTemperatureCompensation::applicableFor()
{
    QList<Configuration::FeatureTypes> result;
        result.append(Configuration::eTrafoParamFeature);
        return result;
}

/*!
 * \brief exec
 * \return
 */
bool SimpleTemperatureCompensation::exec(TrafoParam &tp)
{
    if(this->isValid()){
        FunctionConfiguration myConfig = this->getFunctionConfiguration();
        QMap<QString,QString> stringParameter = myConfig.stringParameter;

        this->calcExpansion(tp);

    }else{
        this->writeToConsole("An error occured at calculating the function.");
        return false;
    }
    return true;
}

/*!
 * \brief getStringParameter specifies additional string input parameters for the function.
 * \return
 */
QMap<QString, QStringList> SimpleTemperatureCompensation::getStringParameter()
{
    QMap<QString, QStringList> result;
    QString key ="material";
    QStringList value;
    value = Materials::getMaterials();
    result.insert(key,value);
    return result;
}

/*!
 * \brief getDoubleParameter specifies additional double parameters fpr the function.
 * \return
 */
QMap<QString, double> SimpleTemperatureCompensation::getDoubleParameter()
{
    QMap<QString,double> result;
    QString key = "referenceTemperature";
    double value = 20.0;
    result.insert(key,value);
    key = "actualTemperature";
    value = 20.0;
    result.insert(key,value);
    key = "temperatureAccuracy";
    value = 0.1;
    result.insert(key, value);
    return result;
}

/*!
 * \brief calcExpansion estimates the thermal expansion value for corrections.
 * \param tp
 * \param SET
 */
void SimpleTemperatureCompensation::calcExpansion(TrafoParam &tp)
{
    FunctionConfiguration myConfig = this->getFunctionConfiguration();
    QMap<QString,QString> stringParameter = myConfig.stringParameter;
    QMap<QString,double> doubleParameter = myConfig.doubleParameter;

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

        tp.setHomogenMatrix(eMat, eMat, scale);

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
    Statistic *myStats = new Statistic();

    myStats->s0_apriori = 1.0;
    myStats->s0_aposteriori = stddev;
    myStats->stdev = stddev;
    myStats->isValid = true;

    tp.setStatistic(myStats);

    this->myStatistic.s0_aposteriori = myStats->s0_aposteriori;
    this->myStatistic.s0_apriori = myStats->s0_apriori;
    this->myStatistic.stdev = myStats->stdev;
    this->myStatistic.isValid = myStats->isValid;
}

QStringList SimpleTemperatureCompensation::getResultProtocol()
{
    QStringList protocoll;
    protocoll.append("temperature compensation using the following parameters:");
    protocoll.append(QString("actual temperature [°C]: " + this->protActTemp));
    protocoll.append(QString("reference temperature [°C]: " + this->protRefTemp));
    protocoll.append(QString("accuracy of temperature measurement [°C]: " + this->protTempAccuracy));
    protocoll.append(QString("material: " + this->protMaterial));
    protocoll.append(QString("expansion coefficient [m]: " + this->protExpansionCoeff));
    protocoll.append(QString("expansion [m]/[m]: " + this->protExpansion));
    protocoll.append(QString("accuracy for expansion [m]: " + this->protSTDDEV));
    return protocoll;
}
