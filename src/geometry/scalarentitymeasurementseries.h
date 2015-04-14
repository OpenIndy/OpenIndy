#ifndef SCALARENTITYMEASUREMENTSERIES_H
#define SCALARENTITYMEASUREMENTSERIES_H

#include <QObject>
#include <QtXml>

#include "geometry.h"

/*!
 * \brief The ScalarEntityMeasurementSeries class
 */
class ScalarEntityMeasurementSeries : public Geometry
{
    Q_OBJECT

public:
    ScalarEntityMeasurementSeries(const bool &isNominal, QObject *parent = 0);
    ScalarEntityMeasurementSeries(const bool &isNominal, const double &seriesValue, QObject *parent = 0);

    ScalarEntityMeasurementSeries(const ScalarEntityMeasurementSeries &copy, QObject *parent = 0);

    ScalarEntityMeasurementSeries &operator=(const ScalarEntityMeasurementSeries &copy);

    ~ScalarEntityMeasurementSeries();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum MeasurementSeriesUnknowns{
        unknownMeasurementSeries = eUnknownMeasurementSeries
    };

    //########################################
    //get or set measurement series parameters
    //########################################

    const double &getSeriesValue() const;

    void setSeriesValue(const double &seriesValue);

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

private:

    //#############################
    //measurement series attributes
    //#############################

    double seriesValue;

    //QString getDisplayIsCommon() const;
    //QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    //QString getDisplaySolved() const;
    //QString getDisplayMConfig() const;
    //QString getDisplayStdDev() const;
    //QString getDisplayScalarAngleValue() const;

    //static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type


};

#endif // SCALARENTITYMEASUREMENTSERIES_H
