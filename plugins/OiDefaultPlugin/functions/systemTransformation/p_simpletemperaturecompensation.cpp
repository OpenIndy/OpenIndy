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
        metaData->name = "SimpleTempComp";
        metaData->pluginName = "OpenIndy Default Plugin";
        metaData->author = "jw";
        metaData->description = QString("%1 %2")
                .arg("This functions calculates an equal temperature compensation value for x,y and z component.")
                .arg("You need to put in one scalar entity temperature, that is used to calculate the values.");
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
        InputParams param;
        param.index = 0;
        param.description = "Select scalar entity temperature for calculating the compensation.";
        param.infinite = false;
        param.typeOfElement = Configuration::eScalarEntityTemperatureElement;
        result.append(param);
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

        ScalarEntityTemperature *myScalarEntityTemperature = this->getScalarEntityTemperature();

        if(myScalarEntityTemperature != NULL){
            this->calcExpansion(tp,myScalarEntityTemperature);
        }else{
            this->writeToConsole("No valid scalar entity temperature available.");
            return false;
        }

    }else{
        this->writeToConsole("No valid scalar entity temperature available.");
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
    value.append(Configuration::sSteel);
    value.append(Configuration::sAluminum);
    value.append(Configuration::sPlumb);
    value.append(Configuration::sIron);
    value.append(Configuration::sGrayCastIron);
    value.append(Configuration::sCopper);
    value.append(Configuration::sBrass);
    value.append(Configuration::sZinc);
    value.append(Configuration::sPlatinum);
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
    QString key = "reference Temperature";
    double value = 20.0;
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
void SimpleTemperatureCompensation::calcExpansion(TrafoParam &tp, ScalarEntityTemperature *SET)
{
    double actualTemp = 0.0;

    FunctionConfiguration myConfig = this->getFunctionConfiguration();
    QMap<QString,QString> stringParameter = myConfig.stringParameter;
    QMap<QString,double> doubleParameter = myConfig.doubleParameter;

    if(SET->getIsSolved()){
        actualTemp = SET->getTemperature();

        QString material = "";
        double refTemp = 0.0;
        double tempAccuracy = 0.0;
        double expansionCoefficient = 0.0;
        if(stringParameter.contains("material")){
            material = static_cast<QString>(stringParameter.find("material").value());
            protMaterial = material;
            expansionCoefficient = Configuration::getExpansionCoefficient(material);
            protExpansionCoeff = QString::number(expansionCoefficient,'f',6);
        }
        if(doubleParameter.contains("referenceTemperature")){
            refTemp = static_cast<double>(doubleParameter.find("referenceTemperature").value());
            protRefTemp = QString::number(refTemp,'f',2);
        }
        if(doubleParameter.contains("temperatureAccuracy")){
            tempAccuracy = static_cast<double>(doubleParameter.find("temperatureAccuracy").value());
            protTempAccuracy = QString::number(tempAccuracy,'f',2);
        }

        double expansion = (actualTemp-refTemp)*expansionCoefficient;
        protExpansion = QString::number(expansion,'f',4);
        double scale = (1+ (expansion/1000000));
        tp.setScale(scale,scale,scale);
        tp.setTranslation(0.0,0.0,0.0);
        tp.setRotation(0.0,0.0,0.0);
        tp.generateHomogenMatrix();

        this->calcAccuracy(tp,tempAccuracy,expansion);
    }
}

/*!
 * \brief calcAccuracy estimates the accuracy of the transformation via the accuracy of temperature measurement.
 * \param tp
 * \param tempAccuracy
 * \param expansion
 */
void SimpleTemperatureCompensation::calcAccuracy(TrafoParam &tp, double tempAccuracy, double expansion)
{
    double stddev = tempAccuracy*(expansion/1000000);
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
    protocoll.append(QString("expansion coefficient [µm]: " + this->protExpansionCoeff));
    protocoll.append(QString("expansion [m]/[m]: " + this->protExpansion));
    protocoll.append(QString("accuracy for expansion [m]: " + this->protSTDDEV));
    return protocoll;
}

/*!
 * \brief getScalarEntityTemperature gets the scalar entity temperature from the input feautre list.
 * \return
 */
ScalarEntityTemperature *SimpleTemperatureCompensation::getScalarEntityTemperature()
{
    ScalarEntityTemperature *result = NULL;
    foreach(ScalarEntityTemperature *set, this->scalarEntityTemperatures){
        if(result == NULL && set->getIsSolved()){
            result = set;
            this->setUseState(result->getId(), true);
        }else{
            this->setUseState(set->getId(), false);
        }
    }
    return result;
}
