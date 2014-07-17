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
	
    QMap<Configuration::ReadingTypes, QString> getUsedReadingTypes() const;
    void insertReadingType(Configuration::ReadingTypes readingType, QString displayName);

    MeasurementConfig getMeasurementConfig() const;
    void setMeasurementConfig(MeasurementConfig myConfig);

    Statistic getStatistic() const;
    void setStatistic(Statistic myStatistic);

    SimulationData& getSimulationData();
    void setSimulationData(SimulationData s);

    virtual bool toOpenIndyXML(QXmlStreamWriter& stream) = 0;
    virtual ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml) = 0;

    virtual bool saveSimulationData() = 0;
    void resetSimulationData();

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

    MeasurementConfig mConfig;
    Statistic myStatistic;
    SimulationData mySimulationData;
    QMap<Configuration::ReadingTypes, QString> usedReadingTypes;

    bool writeGeometryAttributes(QXmlStreamWriter& stream);
    bool readGeometryAttributes(QXmlStreamReader &xml ,ElementDependencies &d);

};

#endif // GEOMETRY_H
