#ifndef TRAFOCONTROLLER_H
#define TRAFOCONTROLLER_H

#include <QObject>
#include <QPointer>

#include "observation.h"
#include "function.h"

using namespace oi;

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

    //######################
    //transform observations
    //######################

    void transformObservations(const QPointer<Feature> &feature, const QPointer<CoordinateSystem> &destinationSystem);
    void transformObservations(const QPointer<CoordinateSystem> &startSystem, const QPointer<CoordinateSystem> &destinationSystem);
    void transformCoordSystems(const QPointer<CoordinateSystem> &startSystem, const QPointer<CoordinateSystem> &destinationSystem, bool isStation);

    bool getTransformationMatrix(OiMat &trafoMat, const QPointer<CoordinateSystem> &startSystem, const QPointer<CoordinateSystem> &destinationSystem);

private:

    //##############
    //helper methods
    //##############

    QPointer<TrafoParam> findTransformation(const QPointer<CoordinateSystem> &startSystem, const QPointer<CoordinateSystem> &destinationSystem);

};

#endif // TRAFOCONTROLLER_H
