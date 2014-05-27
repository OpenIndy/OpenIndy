#include "guiconfiguration.h"

GUIConfiguration::GUIConfiguration(QObject *parent) :
    QObject(parent)
{
}

GUIConfiguration::~GUIConfiguration()
{

}

QStringList GUIConfiguration::allAttributes;
QList<DisplayAttribute*> GUIConfiguration::featureAttributes;
QList<DisplayAttribute*> GUIConfiguration::trafoParamAttributes;

/*!
 * \brief generateAllAttributes fills the QStringList with all existing attribute names.
 * This list is later used for the tableview model.
 * Call this function at project beginning and after every change in unit setting!
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
}

/*!
 * \brief displayAttributes checks which attributes should be displayed. returns the column indices for this attributes.
 * \param attributes
 * \return
 */
QList<int> GUIConfiguration::displayAttributes(QList<DisplayAttribute*> attributes)
{
    QList<int> display;

    for(int i =0; i<attributes.size();i++){
        if(attributes.at(i)->displayState == true){
            int res = GUIConfiguration::allAttributes.indexOf(attributes.at(i)->attrName);
            if(res != -1){
                display.append(res);
            }
        }
    }
    return display;
}
