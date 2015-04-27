#ifndef P_BESTFITSPHERERAULS_H
#define P_BESTFITSPHERERAULS_H

#include <QObject>
#include <QPointer>
#include <QtMath>

#include "fitfunction.h"
#include "oivec.h"
#include "oimat.h"

/*!
 * \brief The BestFitSphereRauls class
 */
class BestFitSphereRauls : public FitFunction
{
    Q_OBJECT
protected:

    //##############################
    //function initialization method
    //##############################

    void init();

    //############
    //exec methods
    //############

    bool exec(Sphere &sphere);

private:

    //##############
    //helper methods
    //##############

    bool approximate(Sphere &sphere, const QList<QPointer<Observation> > &inputObservations);
    bool fit(Sphere &sphere, const QList<QPointer<Observation> > &inputObservations);

};

#endif // P_BESTFITSPHERERAULS_H
