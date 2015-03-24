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
    QString getDisplayIsNominal() const;

    const QList< QPointer<Geometry> > &getMyNominals() const;
    bool addNominal(const QPointer<Geometry> &myNominal);
    bool removeNominal(const QPointer<Geometry> &myNominal);

    const QPointer<Geometry> &getMyActual() const;
    bool setMyActual(const QPointer<Geometry> &myActual);

	QString getDisplayObs() const;
    QList<Observation *> getObservations() const;
    bool addObservation(Observation *obs);
    bool removeObservation(Observation *obs);

    const QPointer<CoordinateSystem> &getNominalSystem() const;
    bool setNominalSystem(const QPointer<CoordinateSystem> &nomSys);

    virtual OiVec getXYZ() const;
    virtual OiVec getIJK() const;
    virtual double getRadius() const;
    virtual double getScalar() const;
	
    QMap<ReadingTypes, QString> getUsedReadingTypes() const;
    void removeReadingType(ReadingTypes rType);

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
    void insertReadingType(ReadingTypes readingType, QString displayName);

signals:
    void geomIsCommonChanged(const int &featureId);
    void geomMyNominalsChanged(const int &featureId);
    void geomMyActualChanged(const int &featureId);
    void geomMyObservationsChanged(const int &featureId);
    void geomMyNominalSystemChanged(const int &featureId);
    void geomMyStatisticChanged(const int &featureId);
    void geomMyMeasurementConfigChanged(const int &featureId);
    void geomUsedReadingTypesChanged(const int &featureId);

protected:
    bool isCommon;
    bool isNominal;
    QList< QPointer<Geometry> > nominals;
    QPointer<Geometry> myActual;
    QList<Observation*> myObservations;
    QPointer<CoordinateSystem> myNominalCoordSys;

    Statistic myStatistic;
    SimulationData mySimulationData;
    QMap<ReadingTypes, QString> usedReadingTypes;

    //current measurement config that is used to create readings until the user selects another one
    MeasurementConfig activeMeasurementConfig; //only for this geometry instance

};

#endif // GEOMETRY_H
