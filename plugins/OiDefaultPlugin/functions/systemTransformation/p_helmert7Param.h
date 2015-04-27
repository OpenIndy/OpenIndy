#ifndef P_HELMERT7PARAM_H
#define P_HELMERT7PARAM_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QtCore/qmath.h>
#include <vector>
#include "systemtransformation.h"
#include "point.h"
#include "trafoparam.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

using namespace std;

class Helmert7Param : public SystemTransformation
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

    bool exec(TrafoParam &trafoParam);

private:
    bool svdError;
    QList<OiVec> locSystem;
    QList<OiVec> refSystem;

    void initPoints();
    bool calc(TrafoParam &tp);
    vector<OiVec> calcCentroidCoord();
    vector<OiVec> centroidReducedCoord(QList<OiVec> input, OiVec centroid);
    vector<OiMat> modelMatrix(vector<OiVec> locC, vector<OiVec> refC);
    OiMat normalEquationMatrix(vector<OiMat> vecA);
    OiVec quaternion(OiMat n);
    OiMat rotationMatrix(OiVec q);
    void fillTrafoParam(OiMat r, vector<OiVec> locC, vector<OiVec> refC, vector<OiVec> centroidCoords, TrafoParam &tp);
    bool adjust(TrafoParam &tp);
    OiMat fillAMatrix(OiVec x0);
    OiVec fillLVector();
    OiVec fillL0Vector(OiVec x0);

};

#endif // P_HELMERT7PARAM_H
