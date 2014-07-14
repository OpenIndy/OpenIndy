#ifndef MATERIALS_H
#define MATERIALS_H

#include <QStringList>
#include <unitconverter.h>

/*!
 * \brief The Materials class handles all supported materials and their thermal expansion coefficients
 */
class Materials
{
public:
    Materials();

    enum supportedMaterials{
        eSteel,
        eAluminum,
        ePlumb,
        eIron,
        eGrayCastIron,
        eCopper,
        eBrass,
        eZinc,
        ePlatinum,
        eConcrete,
        eReinforcedConcrete
    };

    static double getExpansionCoefficient(QString material);
    static double getExpansionCoefficient(Materials::supportedMaterials material);
    static QStringList getMaterials();



private:
    //material names
    static QString sSteel;
    static QString sAluminum;
    static QString sPlumb;
    static QString sIron;
    static QString sGrayCastIron;
    static QString sCopper;
    static QString sBrass;
    static QString sZinc;
    static QString sPlatinum;
    static QString sConcrete;
    static QString sReinforcedConcrete;

    //material thermal expansion coefficients
    static double dSteel;
    static double dAluminum;
    static double dPlumb;
    static double dIron;
    static double dGrayCastIron;
    static double dCopper;
    static double dBrass;
    static double dZinc;
    static double dPlatinum;
    static double dConcrete;
    static double dReinforcedConcrete;
};

#endif // MATERIALS_H
