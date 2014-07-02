#ifndef TRAFOCONTROLLER_H
#define TRAFOCONTROLLER_H

#include <QObject>
#include "observation.h"
#include "function.h"
#include "oifeaturestate.h"

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
    //void applyMovements(Observation *obs);
    void transformNewObservations(Observation *obs);

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
    //QList<TrafoParam*> findMovements(Observation *obs);
    QList<TrafoParam*> sortMovements(QList<TrafoParam*> movements);
    void CheckToApplyMovements(CoordinateSystem *from);

    QList<TrafoParam*> findMovements(CoordinateSystem *from);
    void applyMovements(QList<TrafoParam*> movements, CoordinateSystem *from);
    void applyTranslations(OiVec centroidBefore, OiVec centroidAfter, QList<Observation*>observations);
    
};

#endif // TRAFOCONTROLLER_H
