#ifndef TRAFOCONTROLLER_H
#define TRAFOCONTROLLER_H

#include <QObject>

#include "observation.h"
#include "function.h"

/*!
 * \brief The TrafoController class
 *
 */
class TrafoController : public QObject
{
    Q_OBJECT

public:
    explicit TrafoController(QObject *parent = 0);
    
signals:
    
public slots:
/*
    OiMat getTransformationMatrix(CoordinateSystem *from);

    void addObservation(Observation *obs);
    void transformNewObservations(Observation *obs);

    bool transformObservations(CoordinateSystem *from);
    void setObservationState(CoordinateSystem *cs, bool solved);

    //to calc movement transformations
    void transformObsForMovementCalculation(CoordinateSystem *from, CoordinateSystem *to);

private:

    TrafoParam* findTrafoParam(CoordinateSystem *from, CoordinateSystem *to);
    QList<TrafoParam*> sortMovements(QList<TrafoParam*> movements);
    void CheckToApplyMovements(CoordinateSystem *from);

    QList<TrafoParam*> findMovements(CoordinateSystem *from);
    void applyMovements(QList<TrafoParam*> movements, CoordinateSystem *from);*/
};

#endif // TRAFOCONTROLLER_H
