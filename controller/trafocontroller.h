#ifndef TRAFOCONTROLLER_H
#define TRAFOCONTROLLER_H

#include <QObject>
#include "observation.h"
#include "datalisthandler.h"
#include "function.h"

/*!
 * \brief The TrafoController class handles all transformations and corrections caused by movements (temperature compensation)
 */
class TrafoController : public QObject
{
    Q_OBJECT
public:
    explicit TrafoController(QObject *parent = 0);
    
signals:
    
public slots:

    void addObservation(Observation *obs);
    void applyMovements(Observation *obs);
    void transformNewObservations(Observation *obs);
    void initPointers(DataListHandler dlh);

    bool transformObservations(CoordinateSystem *from);
    void setObservationState(CoordinateSystem *cs, bool valid);

    void MovementsChanged();
    void recalcAllMovements();
    void recalcObservations();

    /*
     //TODO
    movement added
    recalc movements
    movement deleted
    recalc movements
    time changed
*/


private:

    TrafoParam* findTrafoParam(CoordinateSystem *from, CoordinateSystem *to);

    static QList<FeatureWrapper*> *features;
    static FeatureWrapper *activeFeature;
    static QList<CoordinateSystem*> *coordSys;
    static QList<Station*> *stations;
    static Station *activeStation;
    static CoordinateSystem *activeCoordinateSystem;
    static QMap<QString, int> *availableGroups;
    
};

#endif // TRAFOCONTROLLER_H
