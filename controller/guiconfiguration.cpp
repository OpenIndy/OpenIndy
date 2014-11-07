#include "guiconfiguration.h"

GUIConfiguration::GUIConfiguration(QObject *parent) :
    QObject(parent)
{
}

GUIConfiguration::~GUIConfiguration()
{

}

//! main view
QStringList GUIConfiguration::allAttributes;
QList<DisplayAttribute*> GUIConfiguration::featureAttributes;
QList<DisplayAttribute*> GUIConfiguration::trafoParamAttributes;

//! observation model
QStringList GUIConfiguration::allObsAttributes;
QList<DisplayAttribute*> GUIConfiguration::obsAttributes;

//! reading model
QString GUIConfiguration::readingType = "";
QStringList GUIConfiguration::allReadAttributes;
QList<DisplayAttribute*> GUIConfiguration::polarAttributes;
QList<DisplayAttribute*> GUIConfiguration::cartAttributes;
QList<DisplayAttribute*> GUIConfiguration::distAttributes;
QList<DisplayAttribute*> GUIConfiguration::dirAttributes;
QList<DisplayAttribute*> GUIConfiguration::tempAttributes;
QList<DisplayAttribute*> GUIConfiguration::levelAttributes;
QList<DisplayAttribute*> GUIConfiguration::undefAttributes;

/*!
 * \brief generateLists. Calls this function at project start and after all unit changes. It will generate the new lists that are used for the gui.
 */
void GUIConfiguration::generateLists()
{
    //complete lists
    generateAllAttributes();
    generateAllObsAttributes();
    generateAllReadAttr();

    //user defined main and trafo param view
    generateFeatureAttributes();
    generateTrafoParamAttributes();

    //User defined observation view
    generateObsAttributes();

    //user defined reading view
    generateCartAttr();
    generatePolarAttr();
    generateDirAttr();
    generateDistAttr();
    generateLevelAttr();
    generateTempAttr();
    generateUndefAttr();
}

/*!
 * \brief generateAllAttributes fills the QStringList with all existing attribute names.
 * This list is later used for the tableview model.
 * Call this function at project beginning and after every change in unit setting!
 * Append all new attributes at the END of the list !!!!
 */
void GUIConfiguration::generateAllAttributes()
{
    GUIConfiguration::allAttributes.clear();

    GUIConfiguration::allAttributes.append("feature type");
    GUIConfiguration::allAttributes.append("feature name");
    GUIConfiguration::allAttributes.append("group");
    GUIConfiguration::allAttributes.append(QString("x" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allAttributes.append(QString("y" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allAttributes.append(QString("z" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allAttributes.append("observations");
    GUIConfiguration::allAttributes.append("solved");
    GUIConfiguration::allAttributes.append("measurement config");
    GUIConfiguration::allAttributes.append("functions");
    GUIConfiguration::allAttributes.append("common point");
    GUIConfiguration::allAttributes.append("nominal");
    GUIConfiguration::allAttributes.append(QString("stddev" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allAttributes.append("I");
    GUIConfiguration::allAttributes.append("J");
    GUIConfiguration::allAttributes.append("K");
    GUIConfiguration::allAttributes.append(QString("radius" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allAttributes.append("scalar value [m]");
    GUIConfiguration::allAttributes.append("scalar value [dec. degree]");
    GUIConfiguration::allAttributes.append("start");
    GUIConfiguration::allAttributes.append("destination");
    GUIConfiguration::allAttributes.append(QString("translation x" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allAttributes.append(QString("translation y" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allAttributes.append(QString("translation z" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allAttributes.append(QString("rotation x" + UnitConverter::getAngleUnitString()));
    GUIConfiguration::allAttributes.append(QString("rotation y" + UnitConverter::getAngleUnitString()));
    GUIConfiguration::allAttributes.append(QString("rotation z" + UnitConverter::getAngleUnitString()));
    GUIConfiguration::allAttributes.append("scale x");
    GUIConfiguration::allAttributes.append("scale y");
    GUIConfiguration::allAttributes.append("scale z");
    GUIConfiguration::allAttributes.append(QString("temperature" + UnitConverter::getTemperatureUnitString()));
    GUIConfiguration::allAttributes.append("measurement series");
    GUIConfiguration::allAttributes.append("comment");
    GUIConfiguration::allAttributes.append("use");
    GUIConfiguration::allAttributes.append("time");
    GUIConfiguration::allAttributes.append("datumTransformation");
    GUIConfiguration::allAttributes.append(QString("expansion origin x" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allAttributes.append(QString("expansion origin y" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allAttributes.append(QString("expansion origin z" + UnitConverter::getDistanceUnitString()));
}

/*!
 * \brief generateFeatureAttributes fills the QMap with all available attributes for features except trafo parameter.
 * Also includes a boolean that handles if the attributes is displayed or not.
 */
void GUIConfiguration::generateFeatureAttributes()
{
    GUIConfiguration::featureAttributes.clear();

    DisplayAttribute *ftype = new DisplayAttribute("feature type",true);
    GUIConfiguration::featureAttributes.append(ftype);
    DisplayAttribute *fname = new DisplayAttribute("feature name",true);
    GUIConfiguration::featureAttributes.append(fname);
    DisplayAttribute *fgroup = new DisplayAttribute("group",true);
    GUIConfiguration::featureAttributes.append(fgroup);
    DisplayAttribute *fX = new DisplayAttribute(QString("x"+UnitConverter::getDistanceUnitString()),true);
    GUIConfiguration::featureAttributes.append(fX);
    DisplayAttribute *fY = new DisplayAttribute(QString("y"+UnitConverter::getDistanceUnitString()),true);
    GUIConfiguration::featureAttributes.append(fY);
    DisplayAttribute *fZ = new DisplayAttribute(QString("z"+UnitConverter::getDistanceUnitString()),true);
    GUIConfiguration::featureAttributes.append(fZ);
    DisplayAttribute *fobs = new DisplayAttribute("observations",true);
    GUIConfiguration::featureAttributes.append(fobs);
    DisplayAttribute *fSolved = new DisplayAttribute("solved",true);
    GUIConfiguration::featureAttributes.append(fSolved);
    DisplayAttribute *fMConfig = new DisplayAttribute("measurement config",true);
    GUIConfiguration::featureAttributes.append(fMConfig);
    DisplayAttribute *fFunctions = new DisplayAttribute("functions",true);
    GUIConfiguration::featureAttributes.append(fFunctions);
    DisplayAttribute *fcomPoint = new DisplayAttribute("common point",true);
    GUIConfiguration::featureAttributes.append(fcomPoint);
    DisplayAttribute *fNominal = new DisplayAttribute("nominal",true);
    GUIConfiguration::featureAttributes.append(fNominal);
    DisplayAttribute *fStddev = new DisplayAttribute(QString("stddev"+UnitConverter::getDistanceUnitString()),true);
    GUIConfiguration::featureAttributes.append(fStddev);
    DisplayAttribute *fI = new DisplayAttribute("I",true);
    GUIConfiguration::featureAttributes.append(fI);
    DisplayAttribute *fJ = new DisplayAttribute("J",true);
    GUIConfiguration::featureAttributes.append(fJ);
    DisplayAttribute *fK = new DisplayAttribute("K",true);
    GUIConfiguration::featureAttributes.append(fK);
    DisplayAttribute *fRadius = new DisplayAttribute(QString("radius"+UnitConverter::getDistanceUnitString()),true);
    GUIConfiguration::featureAttributes.append(fRadius);
    DisplayAttribute *fscalarD = new DisplayAttribute("scalar value [m]",true);
    GUIConfiguration::featureAttributes.append(fscalarD);
    DisplayAttribute *fscalarA = new DisplayAttribute("scalar value [dec.degree]",true);
    GUIConfiguration::featureAttributes.append(fscalarA);
    DisplayAttribute *fTemperature = new DisplayAttribute(QString("temperature"+UnitConverter::getTemperatureUnitString()),true);
    GUIConfiguration::featureAttributes.append(fTemperature);
    DisplayAttribute *fMeasSeries = new DisplayAttribute("measurement series",true);
    GUIConfiguration::featureAttributes.append(fMeasSeries);
    DisplayAttribute *fcomment = new DisplayAttribute("comment",true);
    GUIConfiguration::featureAttributes.append(fcomment);
    DisplayAttribute *fExpOriX = new DisplayAttribute(QString("expansion origin x" + UnitConverter::getDistanceUnitString()),true);
    GUIConfiguration::featureAttributes.append(fExpOriX);
    DisplayAttribute *fExpOriY = new DisplayAttribute(QString("expansion origin y" + UnitConverter::getDistanceUnitString()),true);
    GUIConfiguration::featureAttributes.append(fExpOriY);
    DisplayAttribute *fExpOriZ = new DisplayAttribute(QString("expansion origin z" + UnitConverter::getDistanceUnitString()),true);
    GUIConfiguration::featureAttributes.append(fExpOriZ);
}

/*!
 * \brief generateTrafoParamAttributes fills the QMap with all available attributes for trafo parameter.
 * Also includes a boolean that handles if the attributes is displayed or not.
 */
void GUIConfiguration::generateTrafoParamAttributes()
{
    GUIConfiguration::trafoParamAttributes.clear();

    DisplayAttribute *ftype = new DisplayAttribute("feature type",true);
    GUIConfiguration::trafoParamAttributes.append(ftype);
    DisplayAttribute *fname = new DisplayAttribute("feature name",true);
    GUIConfiguration::trafoParamAttributes.append(fname);
    DisplayAttribute *fgroup = new DisplayAttribute("group",true);
    GUIConfiguration::trafoParamAttributes.append(fgroup);
    DisplayAttribute *fsolved = new DisplayAttribute("solved",true);
    GUIConfiguration::trafoParamAttributes.append(fsolved);
    DisplayAttribute *fFunctions = new DisplayAttribute("functions",true);
    GUIConfiguration::trafoParamAttributes.append(fFunctions);
    DisplayAttribute *fstart = new DisplayAttribute("start",true);
    GUIConfiguration::trafoParamAttributes.append(fstart);
    DisplayAttribute *fdest = new DisplayAttribute("destination",true);
    GUIConfiguration::trafoParamAttributes.append(fdest);
    DisplayAttribute *fTranslationX = new DisplayAttribute(QString("translation x"+UnitConverter::getDistanceUnitString()),true);
    GUIConfiguration::trafoParamAttributes.append(fTranslationX);
    DisplayAttribute *fTranslationY = new DisplayAttribute(QString("translation y"+UnitConverter::getDistanceUnitString()),true);
    GUIConfiguration::trafoParamAttributes.append(fTranslationY);
    DisplayAttribute *fTranslationZ = new DisplayAttribute(QString("translation z"+UnitConverter::getDistanceUnitString()),true);
    GUIConfiguration::trafoParamAttributes.append(fTranslationZ);
    DisplayAttribute *fRotationX = new DisplayAttribute(QString("rotation x"+UnitConverter::getAngleUnitString()),true);
    GUIConfiguration::trafoParamAttributes.append(fRotationX);
    DisplayAttribute *fRotationY = new DisplayAttribute(QString("rotation y"+UnitConverter::getAngleUnitString()),true);
    GUIConfiguration::trafoParamAttributes.append(fRotationY);
    DisplayAttribute *fRotationZ = new DisplayAttribute(QString("rotation z"+UnitConverter::getAngleUnitString()),true);
    GUIConfiguration::trafoParamAttributes.append(fRotationZ);
    DisplayAttribute *fScaleX = new DisplayAttribute("scale x",true);
    GUIConfiguration::trafoParamAttributes.append(fScaleX);
    DisplayAttribute *fScaleY = new DisplayAttribute("scale y",true);
    GUIConfiguration::trafoParamAttributes.append(fScaleY);
    DisplayAttribute *fScaleZ = new DisplayAttribute("scale z",true);
    GUIConfiguration::trafoParamAttributes.append(fScaleZ);
    DisplayAttribute *fcomment = new DisplayAttribute("comment",true);
    GUIConfiguration::trafoParamAttributes.append(fcomment);
    DisplayAttribute *fuse = new DisplayAttribute("use",true);
    GUIConfiguration::trafoParamAttributes.append(fuse);
    DisplayAttribute *ftime = new DisplayAttribute("time",true);
    GUIConfiguration::trafoParamAttributes.append(ftime);
    DisplayAttribute *fdatumTrafo = new DisplayAttribute("datumTransformation",true);
    GUIConfiguration::trafoParamAttributes.append(fdatumTrafo);
    DisplayAttribute *fSTD = new DisplayAttribute(QString("stddev" + UnitConverter::getDistanceUnitString()),true);
    GUIConfiguration::trafoParamAttributes.append(fSTD);
}

/*!
 * \brief displayAttributes checks which attributes should be displayed. returns the column indices for this attributes.
 * \param attributes
 * \return
 */
QList<int> GUIConfiguration::displayAttributes(QList<DisplayAttribute*> selectedAttributes, QStringList attributes)
{
    QList<int> display;

    for(int i =0; i<selectedAttributes.size();i++){
        if(selectedAttributes.at(i)->displayState == true){
            int res = attributes.indexOf(selectedAttributes.at(i)->attrName);
            if(res != -1){
                display.append(res);
            }
        }
    }
    return display;
}

/*!
 * \brief displayReadingAttributes selects the columns that should be displayed depending on the current reading type.
 * \return
 */
QList<int> GUIConfiguration::displayReadingAttributes()
{
    QList<int> display;

    //display all selected polar reading attributes
    if(readingType.compare(Configuration::sPolar) == 0){

        for(int i =0; i<polarAttributes.size();i++){
            if(polarAttributes.at(i)->displayState == true){
                int res = allReadAttributes.indexOf(polarAttributes.at(i)->attrName);
                if(res != -1){
                    display.append(res);
                }
            }
        }
        return display;

    //display all selected cartesian reading attributes
    }else if(readingType.compare(Configuration::sCartesian) == 0){

        for(int i =0; i<cartAttributes.size();i++){
            if(cartAttributes.at(i)->displayState == true){
                int res = allReadAttributes.indexOf(cartAttributes.at(i)->attrName);
                if(res != -1){
                    display.append(res);
                }
            }
        }
        return display;

    //display all selected distance reading attributes
    }else if(readingType.compare(Configuration::sDistance) == 0){

        for(int i =0; i<distAttributes.size();i++){
            if(distAttributes.at(i)->displayState == true){
                int res = allReadAttributes.indexOf(distAttributes.at(i)->attrName);
                if(res != -1){
                    display.append(res);
                }
            }
        }
        return display;

    //display all selected direction reading attributes
    }else if(readingType.compare(Configuration::sDirection) == 0){

        for(int i =0; i<dirAttributes.size();i++){
            if(dirAttributes.at(i)->displayState == true){
                int res = allReadAttributes.indexOf(dirAttributes.at(i)->attrName);
                if(res != -1){
                    display.append(res);
                }
            }
        }
        return display;

    //display all selected temperature reading attributes
    }else if(readingType.compare(Configuration::sTemperatur) == 0){

        for(int i =0; i<tempAttributes.size();i++){
            if(tempAttributes.at(i)->displayState == true){
                int res = allReadAttributes.indexOf(tempAttributes.at(i)->attrName);
                if(res != -1){
                    display.append(res);
                }
            }
        }
        return display;

    //display all selected level reading attributes
    }else if(readingType.compare(Configuration::sLevel) == 0){

        for(int i =0; i<levelAttributes.size();i++){
            if(levelAttributes.at(i)->displayState == true){
                int res = allReadAttributes.indexOf(levelAttributes.at(i)->attrName);
                if(res != -1){
                    display.append(res);
                }
            }
        }
        return display;

    //display all selected undefined reading attributes
    }else if(readingType.compare("undefined") == 0){

        for(int i =0; i<undefAttributes.size();i++){
            if(undefAttributes.at(i)->displayState == true){
                int res = allReadAttributes.indexOf(undefAttributes.at(i)->attrName);
                if(res != -1){
                    display.append(res);
                }
            }
        }
        return display;

    }
    //return empty list if no reading type is set
    return display;
}

/*!
 * \brief generateAllReadAttr all attributes for readings
 */
void GUIConfiguration::generateAllReadAttr()
{
    GUIConfiguration::allReadAttributes.clear();

    GUIConfiguration::allReadAttributes.append("id");
    GUIConfiguration::allReadAttributes.append("type");
    GUIConfiguration::allReadAttributes.append("time");
    GUIConfiguration::allReadAttributes.append("instrument");
    GUIConfiguration::allReadAttributes.append(QString("azimuth" + UnitConverter::getAngleUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("zenith" + UnitConverter::getAngleUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("distance" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("sigma azimuth" + UnitConverter::getAngleUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("sigma zenith" + UnitConverter::getAngleUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("sigma distance" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("x" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("y" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("z" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("sigma x" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("sigma y" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("sigma z" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allReadAttributes.append("front sight");
    GUIConfiguration::allReadAttributes.append(QString("temperature" + UnitConverter::getTemperatureUnitString()));
    GUIConfiguration::allReadAttributes.append("valid");
    GUIConfiguration::allReadAttributes.append(QString("sigma temperature" + UnitConverter::getTemperatureUnitString()));
    GUIConfiguration::allReadAttributes.append("undefined values");
    GUIConfiguration::allReadAttributes.append("sigma undefined values");
    GUIConfiguration::allReadAttributes.append(QString("RX" + UnitConverter::getAngleUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("RY" + UnitConverter::getAngleUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("RZ" + UnitConverter::getAngleUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("sigma RX" + UnitConverter::getAngleUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("sigma RY" + UnitConverter::getAngleUnitString()));
    GUIConfiguration::allReadAttributes.append(QString("sigma RZ" + UnitConverter::getAngleUnitString()));
}

/*!
 * \brief generatePolarAttr all attributes for polar readings
 */
void GUIConfiguration::generatePolarAttr()
{
    GUIConfiguration::polarAttributes.clear();

    DisplayAttribute *readID = new DisplayAttribute("id",true);
    DisplayAttribute *readType = new DisplayAttribute("type",true);
    DisplayAttribute *readTime = new DisplayAttribute("time",true);
    DisplayAttribute *readInstrument = new DisplayAttribute("instrument",true);
    DisplayAttribute *readAzimuth = new DisplayAttribute(QString("azimuth" + UnitConverter::getAngleUnitString()),true);
    DisplayAttribute *readZenith = new DisplayAttribute(QString("zenith" + UnitConverter::getAngleUnitString()),true);
    DisplayAttribute *readDistance = new DisplayAttribute(QString("distance" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *readSigAz = new DisplayAttribute(QString("sigma azimuth" + UnitConverter::getAngleUnitString()),true);
    DisplayAttribute *readSigZe = new DisplayAttribute(QString("sigma zenith" + UnitConverter::getAngleUnitString()),true);
    DisplayAttribute *readSigDist = new DisplayAttribute(QString("sigma distance" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *readFSBS = new DisplayAttribute("front sight",true);
    DisplayAttribute *readValid = new DisplayAttribute("valid",true);

    GUIConfiguration::polarAttributes.append(readID);
    GUIConfiguration::polarAttributes.append(readType);
    GUIConfiguration::polarAttributes.append(readTime);
    GUIConfiguration::polarAttributes.append(readInstrument);
    GUIConfiguration::polarAttributes.append(readAzimuth);
    GUIConfiguration::polarAttributes.append(readZenith);
    GUIConfiguration::polarAttributes.append(readDistance);
    GUIConfiguration::polarAttributes.append(readSigAz);
    GUIConfiguration::polarAttributes.append(readSigZe);
    GUIConfiguration::polarAttributes.append(readSigDist);
    GUIConfiguration::polarAttributes.append(readFSBS);
    GUIConfiguration::polarAttributes.append(readValid);
}

/*!
 * \brief generateCartAttr all attributes for cartesian reading
 */
void GUIConfiguration::generateCartAttr()
{
    GUIConfiguration::cartAttributes.clear();

    DisplayAttribute *readID = new DisplayAttribute("id",true);
    DisplayAttribute *readType = new DisplayAttribute("type",true);
    DisplayAttribute *readTime = new DisplayAttribute("time",true);
    DisplayAttribute *readInstrument = new DisplayAttribute("instrument",true);
    DisplayAttribute *readX = new DisplayAttribute(QString("x" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *readY = new DisplayAttribute(QString("y" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *readZ = new DisplayAttribute(QString("z" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *readSigX = new DisplayAttribute(QString("sigma x" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *readSigY = new DisplayAttribute(QString("sigma y" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *readSigZ = new DisplayAttribute(QString("sigma z" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *readValid = new DisplayAttribute("valid",true);

    GUIConfiguration::cartAttributes.append(readID);
    GUIConfiguration::cartAttributes.append(readType);
    GUIConfiguration::cartAttributes.append(readTime);
    GUIConfiguration::cartAttributes.append(readInstrument);
    GUIConfiguration::cartAttributes.append(readX);
    GUIConfiguration::cartAttributes.append(readY);
    GUIConfiguration::cartAttributes.append(readZ);
    GUIConfiguration::cartAttributes.append(readSigX);
    GUIConfiguration::cartAttributes.append(readSigY);
    GUIConfiguration::cartAttributes.append(readSigZ);
    GUIConfiguration::cartAttributes.append(readValid);
}

/*!
 * \brief generateDistAttr all attributes for distance reading
 */
void GUIConfiguration::generateDistAttr()
{
    GUIConfiguration::distAttributes.clear();

    DisplayAttribute *readID = new DisplayAttribute("id",true);
    DisplayAttribute *readType = new DisplayAttribute("type",true);
    DisplayAttribute *readTime = new DisplayAttribute("time",true);
    DisplayAttribute *readInstrument = new DisplayAttribute("instrument",true);
    DisplayAttribute *readDist = new DisplayAttribute(QString("distance" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *readSigDist = new DisplayAttribute(QString("sigma distance" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *readValid = new DisplayAttribute("valid",true);

    GUIConfiguration::distAttributes.append(readID);
    GUIConfiguration::distAttributes.append(readType);
    GUIConfiguration::distAttributes.append(readTime);
    GUIConfiguration::distAttributes.append(readInstrument);
    GUIConfiguration::distAttributes.append(readDist);
    GUIConfiguration::distAttributes.append(readSigDist);
    GUIConfiguration::distAttributes.append(readValid);
}

/*!
 * \brief generateDirAttr all attributes for direction reading
 */
void GUIConfiguration::generateDirAttr()
{
    GUIConfiguration::dirAttributes.clear();

    DisplayAttribute *readID = new DisplayAttribute("id",true);
    DisplayAttribute *readType = new DisplayAttribute("type",true);
    DisplayAttribute *readTime = new DisplayAttribute("time",true);
    DisplayAttribute *readInstrument = new DisplayAttribute("instrument",true);
    DisplayAttribute *readAzimuth = new DisplayAttribute(QString("azimuth" + UnitConverter::getAngleUnitString()),true);
    DisplayAttribute *readZenith = new DisplayAttribute(QString("zenith" + UnitConverter::getAngleUnitString()),true);
    DisplayAttribute *readSigAz = new DisplayAttribute(QString("sigma azimuth" + UnitConverter::getAngleUnitString()),true);
    DisplayAttribute *readSigZe = new DisplayAttribute(QString("sigma zenith" + UnitConverter::getAngleUnitString()),true);
    DisplayAttribute *readValid = new DisplayAttribute("valid",true);
    DisplayAttribute *readFsBs = new DisplayAttribute("front sight",true);

    GUIConfiguration::dirAttributes.append(readID);
    GUIConfiguration::dirAttributes.append(readType);
    GUIConfiguration::dirAttributes.append(readTime);
    GUIConfiguration::dirAttributes.append(readInstrument);
    GUIConfiguration::dirAttributes.append(readAzimuth);
    GUIConfiguration::dirAttributes.append(readZenith);
    GUIConfiguration::dirAttributes.append(readSigAz);
    GUIConfiguration::dirAttributes.append(readSigZe);
    GUIConfiguration::dirAttributes.append(readValid);
    GUIConfiguration::dirAttributes.append(readFsBs);
}

/*!
 * \brief generateTempAttr all attributes for temperature reading
 */
void GUIConfiguration::generateTempAttr()
{
    GUIConfiguration::tempAttributes.clear();

    DisplayAttribute *readID = new DisplayAttribute("id",true);
    DisplayAttribute *readType = new DisplayAttribute("type",true);
    DisplayAttribute *readTime = new DisplayAttribute("time",true);
    DisplayAttribute *readInstrument = new DisplayAttribute("instrument",true);
    DisplayAttribute *readTemp = new DisplayAttribute(QString("temperature" + UnitConverter::getTemperatureUnitString()),true);
    DisplayAttribute *readValid = new DisplayAttribute("valid",true);
    DisplayAttribute *readSigTemp = new DisplayAttribute(QString("sigma temperature" + UnitConverter::getTemperatureUnitString()),true);

    GUIConfiguration::tempAttributes.append(readID);
    GUIConfiguration::tempAttributes.append(readType);
    GUIConfiguration::tempAttributes.append(readTime);
    GUIConfiguration::tempAttributes.append(readInstrument);
    GUIConfiguration::tempAttributes.append(readTemp);
    GUIConfiguration::tempAttributes.append(readValid);
    GUIConfiguration::tempAttributes.append(readSigTemp);
}

void GUIConfiguration::generateLevelAttr()
{
    GUIConfiguration::levelAttributes.clear();

    DisplayAttribute *readID = new DisplayAttribute("id",true);
    DisplayAttribute *readType = new DisplayAttribute("type",true);
    DisplayAttribute *readTime = new DisplayAttribute("time",true);
    DisplayAttribute *readInstrument = new DisplayAttribute("instrument",true);
    DisplayAttribute *readValid = new DisplayAttribute("valid",true);
    DisplayAttribute *readRX = new DisplayAttribute(QString("RX" + UnitConverter::getAngleUnitString()),true);
    DisplayAttribute *readRY = new DisplayAttribute(QString("RY" + UnitConverter::getAngleUnitString()),true);
    DisplayAttribute *readRZ = new DisplayAttribute(QString("RZ" + UnitConverter::getAngleUnitString()),true);
    DisplayAttribute *readSigRX = new DisplayAttribute(QString("sigma RX" + UnitConverter::getAngleUnitString()),true);
    DisplayAttribute *readSigRY = new DisplayAttribute(QString("sigma RY" + UnitConverter::getAngleUnitString()),true);
    DisplayAttribute *readSigRZ = new DisplayAttribute(QString("sigma RZ" + UnitConverter::getAngleUnitString()),true);

    GUIConfiguration::levelAttributes.append(readID);
    GUIConfiguration::levelAttributes.append(readType);
    GUIConfiguration::levelAttributes.append(readTime);
    GUIConfiguration::levelAttributes.append(readInstrument);
    GUIConfiguration::levelAttributes.append(readValid);
    GUIConfiguration::levelAttributes.append(readRX);
    GUIConfiguration::levelAttributes.append(readRY);
    GUIConfiguration::levelAttributes.append(readRZ);
    GUIConfiguration::levelAttributes.append(readSigRX);
    GUIConfiguration::levelAttributes.append(readSigRY);
    GUIConfiguration::levelAttributes.append(readSigRZ);
}

void GUIConfiguration::generateUndefAttr()
{
    GUIConfiguration::undefAttributes.clear();

    DisplayAttribute *readID = new DisplayAttribute("id",true);
    DisplayAttribute *readType = new DisplayAttribute("type",true);
    DisplayAttribute *readTime = new DisplayAttribute("time",true);
    DisplayAttribute *readInstrument = new DisplayAttribute("instrument",true);
    DisplayAttribute *readValid = new DisplayAttribute("valid",true);
    DisplayAttribute *readundefVal = new DisplayAttribute("undefined values",true);
    DisplayAttribute *readSigUndefVal = new DisplayAttribute("sigma undefined values",true);

    GUIConfiguration::undefAttributes.append(readID);
    GUIConfiguration::undefAttributes.append(readType);
    GUIConfiguration::undefAttributes.append(readTime);
    GUIConfiguration::undefAttributes.append(readInstrument);
    GUIConfiguration::undefAttributes.append(readValid);
    GUIConfiguration::undefAttributes.append(readundefVal);
    GUIConfiguration::undefAttributes.append(readSigUndefVal);
}

/*!
 * \brief generateAllObsAttributes. Add new attributes at the end of the list.
 */
void GUIConfiguration::generateAllObsAttributes()
{
    GUIConfiguration::allObsAttributes.clear();

    GUIConfiguration::allObsAttributes.append("id");
    GUIConfiguration::allObsAttributes.append("station");
    GUIConfiguration::allObsAttributes.append("target geometry");
    GUIConfiguration::allObsAttributes.append(QString("x" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allObsAttributes.append(QString("y" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allObsAttributes.append(QString("z" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allObsAttributes.append("valid");
    GUIConfiguration::allObsAttributes.append(QString("sigma x" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allObsAttributes.append(QString("sigma y" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allObsAttributes.append(QString("sigma z" + UnitConverter::getDistanceUnitString()));
    GUIConfiguration::allObsAttributes.append("solved");
}

/*!
 * \brief generateObsAttributes adds all observation attributes to the list and sets them to be displayed
 */
void GUIConfiguration::generateObsAttributes()
{
    GUIConfiguration::obsAttributes.clear();

    //generate attributes and set to true for displaying all of them.
    DisplayAttribute *obsID = new DisplayAttribute("id",true);
    DisplayAttribute *obsStation = new DisplayAttribute("station",true);
    DisplayAttribute *obsTarget = new DisplayAttribute("target geometry",true);
    DisplayAttribute *obsX = new DisplayAttribute(QString("x" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *obsY = new DisplayAttribute(QString("y" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *obsZ = new DisplayAttribute(QString("z" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *obsValid = new DisplayAttribute("valid",true);
    DisplayAttribute *obsSX = new DisplayAttribute(QString("sigma x" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *obsSY = new DisplayAttribute(QString("sigma y" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *obsSZ = new DisplayAttribute(QString("sigma z" + UnitConverter::getDistanceUnitString()),true);
    DisplayAttribute *obsSolved = new DisplayAttribute("solved",true);

    //add attributes to list
    GUIConfiguration::obsAttributes.append(obsID);
    GUIConfiguration::obsAttributes.append(obsStation);
    GUIConfiguration::obsAttributes.append(obsTarget);
    GUIConfiguration::obsAttributes.append(obsX);
    GUIConfiguration::obsAttributes.append(obsY);
    GUIConfiguration::obsAttributes.append(obsZ);
    GUIConfiguration::obsAttributes.append(obsValid);
    GUIConfiguration::obsAttributes.append(obsSX);
    GUIConfiguration::obsAttributes.append(obsSY);
    GUIConfiguration::obsAttributes.append(obsSZ);
    GUIConfiguration::obsAttributes.append(obsSolved);
}
