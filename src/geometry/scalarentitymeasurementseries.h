#ifndef SCALARENTITYMEASUREMENTSERIES_H
#define SCALARENTITYMEASUREMENTSERIES_H

#include "geometry.h"
#include "statistic.h"

class ScalarEntityMeasurementSeries : public Geometry
{
    Q_OBJECT
public:
    ScalarEntityMeasurementSeries(bool isNominal, QObject *parent = 0);
    ScalarEntityMeasurementSeries(const ScalarEntityMeasurementSeries &copy);

    void recalc();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlDoc);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    bool saveSimulationData();

    double getSeriesValue() const;
    void setSeriesValue(double value);

    double getScalar() const;

    QString getDisplayIsCommon() const;
    QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;
    QString getDisplayScalarMeasurementSeriesValue() const;

private:
    double seriesValue;
};

#endif // SCALARENTITYMEASUREMENTSERIES_H
