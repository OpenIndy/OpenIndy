#ifndef P_SIMPLETEMPERATURECOMPENSATION_H
#define P_SIMPLETEMPERATURECOMPENSATION_H

#include "systemtransformation.h"
#include "pluginmetadata.h"
#include "materials.h"

/*!
 * \brief The SimpleTemperatureCompensation class is a system transformation for temperature compensation.
 * By giving it a reference temperature, a actual temperature and a material it will calculate the compensation values.
 * They are equal for x, y and z component.
 */
class SimpleTemperatureCompensation : public SystemTransformation
{
    Q_OBJECT

public:

    //##############################
    //function initialization method
    //##############################

    void init();

protected:

    //############
    //exec methods
    //############

    bool exec(TrafoParam &trafoParam);

    void calcExpansion(TrafoParam &tp);
    void calcAccuracy(TrafoParam &tp, double tempAccuracy, double expansion);

private:
    QString protActTemp;
    QString protRefTemp;
    QString protMaterial;
    QString protExpansionCoeff;
    QString protExpansion;
    QString protTempAccuracy;
    QString protSTDDEV;
};

#endif // P_SIMPLETEMPERATURECOMPENSATION_H
