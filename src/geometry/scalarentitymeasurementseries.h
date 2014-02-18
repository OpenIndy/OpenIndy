#ifndef SCALARENTITYMEASUREMENTSERIES_H
#define SCALARENTITYMEASUREMENTSERIES_H

#include "geometry.h"
#include "statistic.h"

class ScalarEntityMeasurementSeries : public Geometry
{
public:
    ScalarEntityMeasurementSeries();
    ScalarEntityMeasurementSeries(const ScalarEntityMeasurementSeries &copy);

    void recalc();

    double getSeriesValue() const;
    void setSeriesValue(double value);

    QString getDisplayIsCommon() const;
    QString getDisplayIsNominal() const;
    QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;
    QString getDisplayScalarMeasurementSeriesValue() const;

private:
    double seriesValue;
};

#endif // SCALARENTITYMEASUREMENTSERIES_H
