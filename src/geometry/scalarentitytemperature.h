#ifndef SCALARENTITYTEMPERATURE_H
#define SCALARENTITYTEMPERATURE_H

#include "geometry.h"
#include "statistic.h"

class ScalarEntityTemperature : public Geometry
{
    Q_OBJECT
public:
    ScalarEntityTemperature(bool isNominal, QObject *parent = 0);
    ScalarEntityTemperature(const ScalarEntityTemperature &copy);

    void recalc();

    bool toOpenIndyXML(QXmlStreamWriter& stream);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    double getTemperature() const;
    void setTemperature(double temperature);

    QString getDisplayIsCommon() const;
    QString getDisplayIsNominal() const;
    QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;
    QString getDisplayScalarTemperatureValue() const;

private:
    double temperature;
};

#endif // SCALARENTITYTEMPERATURE_H
