#include "materials.h"

Materials::Materials()
{
}

QString Materials::sSteel = "steel";
QString Materials::sAluminum = "aluminum";
QString Materials::sPlumb = "plumb";
QString Materials::sIron = "iron";
QString Materials::sGrayCastIron = "gray cast iron";
QString Materials::sCopper = "copper";
QString Materials::sBrass = "brass";
QString Materials::sZinc = "zinc";
QString Materials::sPlatinum = "platinum";
QString Materials::sConcrete = "concrete";
QString Materials::sReinforcedConcrete = "reinforced concrete";

/*!
 * \brief expansion coefficients in meter unit.
 */
double Materials::dSteel = 0.000016;
double Materials::dAluminum = 0.000024;
double Materials::dPlumb = 0.000030;
double Materials::dIron = 0.000012;
double Materials::dGrayCastIron = 0.000009;
double Materials::dCopper = 0.000017;
double Materials::dBrass = 0.000018;
double Materials::dZinc = 0.000027;
double Materials::dPlatinum = 0.000009;
double Materials::dConcrete = 0.000012;
double Materials::dReinforcedConcrete = 0.000013; //range from 10 to 15 at 25°C

double Materials::getExpansionCoefficient(QString material)
{
    if(material.compare(Materials::sSteel) == 0){
        return Materials::dSteel;
    }
    if(material.compare(Materials::sAluminum) == 0){
        return Materials::dAluminum;
    }
    if(material.compare(Materials::sPlumb) == 0){
        return Materials::dPlumb;
    }
    if(material.compare(Materials::sIron) == 0){
        return Materials::dIron;
    }
    if(material.compare(Materials::sGrayCastIron) == 0){
        return Materials::dGrayCastIron;
    }
    if(material.compare(Materials::sCopper) == 0){
        return Materials::dCopper;
    }
    if(material.compare(Materials::sBrass) == 0){
        return Materials::dBrass;
    }
    if(material.compare(Materials::sZinc) == 0){
        return Materials::dZinc;
    }
    if(material.compare(Materials::sPlatinum) == 0){
        return Materials::dPlatinum;
    }
    if(material.compare(Materials::sConcrete) == 0){
        return Materials::dConcrete;
    }
    if(material.compare(Materials::sReinforcedConcrete) == 0){
        return Materials::dReinforcedConcrete;
    }
}


double Materials::getExpansionCoefficient(Materials::supportedMaterials material)
{
    switch (material) {
    case Materials::eSteel:
        return Materials::dSteel;
        break;
    case Materials::eAluminum:
        return Materials::dAluminum;
        break;
    case Materials::ePlumb:
        return Materials::dPlumb;
        break;
    case Materials::eIron:
        return Materials::dIron;
        break;
    case Materials::eGrayCastIron:
        return Materials::dGrayCastIron;
        break;
    case Materials::eCopper:
        return Materials::dCopper;
        break;
    case Materials::eBrass:
        return Materials::dBrass;
        break;
    case Materials::eZinc:
        return Materials::dZinc;
        break;
    case Materials::ePlatinum:
        return Materials::dPlatinum;
        break;
    case Materials::eConcrete:
        return Materials::dConcrete;
        break;
    case Materials::eReinforcedConcrete:
        return Materials::dReinforcedConcrete;
        break;
    default:
        break;
    }
}

/*!
 * \brief getMaterials returns a list of all materials
 * \return
 */
QStringList Materials::getMaterials()
{
    QStringList mats;

    mats.append(sSteel);
    mats.append(sAluminum);
    mats.append(sPlumb);
    mats.append(sIron);
    mats.append(sGrayCastIron);
    mats.append(sCopper);
    mats.append(sBrass);
    mats.append(sZinc);
    mats.append(sPlatinum);
    mats.append(sConcrete);
    mats.append(sReinforcedConcrete);

    return mats;
}
