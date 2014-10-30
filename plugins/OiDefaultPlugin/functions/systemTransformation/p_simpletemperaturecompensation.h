#ifndef P_SIMPLETEMPERATURECOMPENSATION_H
#define P_SIMPLETEMPERATURECOMPENSATION_H

#include "pi_systemtransformation.h"
#include "configuration.h"
#include "pluginmetadata.h"
#include "materials.h"

/*!
 * \brief The SimpleTemperatureCompensation class is a system transformation for temperature compensation.
 * By giving it a reference temperature, a actual temperature and a material it will calculate the compensation values.
 * They are equal for x, y and z component.
 */
class SimpleTemperatureCompensation : public SystemTransformation
{
public:
    SimpleTemperatureCompensation();

    PluginMetaData* getMetaData() const;
    QList<InputParams> getNeededElements() const;
    QList<Configuration::FeatureTypes> applicableFor() const;

    bool exec(TrafoParam &);

    QMap<QString, QStringList> getStringParameter() const;
    QMap<QString, double> getDoubleParameter() const;

    void calcExpansion(TrafoParam &tp);
    void calcAccuracy(TrafoParam &tp, double tempAccuracy, double expansion);

    QStringList getResultProtocol();

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
