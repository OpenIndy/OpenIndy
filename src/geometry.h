#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "feature.h"
#include "measurementconfig.h"
#include "statistic.h"
#include "simulationdata.h"

class Observation;
class CoordinateSystem;

/*!
 * \brief The Geometry class
 *
 */
class Geometry : public Feature
{
    Q_OBJECT
public:
    explicit Geometry(bool isNominal, QObject *parent = 0);
    virtual ~Geometry();

    bool getIsCommon() const;
    void setCommonState(bool isCommon);

    bool getIsNominal() const;

    QList<Geometry *> getMyNominals() const;
    bool addNominal(Geometry *myNominal);
    bool removeNominal(Geometry *myNominal);

    Geometry *getMyActual() const;
    bool setMyActual(Geometry *myActual);

	QString getDisplayObs() const;
    QList<Observation *> getObservations() const;
    bool addObservation(Observation *obs);
    bool removeObservation(Observation *obs);

    CoordinateSystem *getNominalSystem() const;
    bool setNominalSystem(CoordinateSystem *nomSys);

    virtual OiVec getXYZ() const;
    virtual OiVec getIJK() const;
    virtual double getRadius() const;
    virtual double getScalar() const;
	
    QMap<Configuration::ReadingTypes, QString> getUsedReadingTypes() const;
    void removeReadingType(Configuration::ReadingTypes rType);

    MeasurementConfig getMeasurementConfig() const;
    void setMeasurementConfig(MeasurementConfig myConfig);

    Statistic getStatistic() const;
    void setStatistic(Statistic myStatistic);

    SimulationData& getSimulationData();
    void setSimulationData(SimulationData s);

    virtual QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    virtual bool fromOpenIndyXML(QDomElement &xmlElem);

    virtual bool saveSimulationData() = 0;
    void resetSimulationData();

private:
    void insertReadingType(Configuration::ReadingTypes readingType, QString displayName);

signals:
    void geomIsCommonChanged(int featureId);
    void geomMyNominalsChanged(int featureId);
    void geomMyActualChanged(int featureId);
    void geomMyObservationsChanged(int featureId);
    void geomMyNominalSystemChanged(int featureId);
    void geomMyStatisticChanged(int featureId);
    void geomMyMeasurementConfigChanged(int featureId);
    void geomUsedReadingTypesChanged(int featureId);

protected:
    bool isCommon;
    bool isNominal;
    QList<Geometry*> nominals;
    Geometry *myActual;
    QList<Observation*> myObservations;
    CoordinateSystem* myNominalCoordSys;

    Statistic myStatistic;
    SimulationData mySimulationData;
    QMap<Configuration::ReadingTypes, QString> usedReadingTypes;

    //current measurement config that is used to create readings until the user selects another one
    MeasurementConfig activeMeasurementConfig; //only for this geometry instance

};

#endif // GEOMETRY_H
