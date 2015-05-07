#include "p_bestfitcircle.h"

/*!
 * \brief BestFitCircle::init
 */
void BestFitCircle::init(){

    //set plugin meta data
    this->metaData.name = "BestFitCircle";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "Michael Loesler - derletztekick.com";
    this->metaData.description = QString("%1 %2")
            .arg("This function estimates a spatial circle usnig a general least squares algorithm.")
            .arg("Insert at least three non-collinear observed points, to get the best fit circle.")
            .arg("The algorithm was taken from the OpenSource FormFittingToolbox (c) M.Loesler (GNU-GPL).");
    this->metaData.iid = "de.openIndy.plugin.function.fitFunction.v001";

    //set needed elements
    this->neededElements.clear();
    NeededElement param1;
    param1.description = "Select at least three non-collinear observed points to calculate the best fit circle.";
    param1.infinite = true;
    param1.typeOfElement = eObservationElement;
    this->neededElements.append(param1);

    //set spplicable for
    this->applicableFor.append(eCircleFeature);

}

/*!
 * \brief BestFitCircle::exec
 * \param circle
 * \return
 */
bool BestFitCircle::exec(Circle &circle) {

    //get and check input observations
    if(!this->inputElements.contains(0) || this->inputElements[0].size() < 3){
        emit this->sendMessage(QString("Not enough valid observations to fit the circle %1").arg(circle.getFeatureName()));
        return false;
    }
    QList<QPointer<Observation> > inputObservations;
    foreach(const InputElement &element, this->inputElements[0]){
        if(!element.observation.isNull() && element.observation->getIsSolved() && element.observation->getIsValid()){
            inputObservations.append(element.observation);
            this->setUseState(0, element.id, true);
            continue;
        }
        this->setUseState(0, element.id, false);
    }
    if(inputObservations.size() < 3){
        emit this->sendMessage(QString("Not enough valid points to fit the point %1").arg(circle.getFeatureName()));
        return false;
    }

    int obsCount = inputObservations.size();

    //fill x,y,z arrays
    double *x = new double[obsCount];
    double *y = new double[obsCount];
    double *z = new double[obsCount];

    int k = 0;

    double xc = 0, yc = 0, zc = 0, rc = 0;

    foreach(const QPointer<Observation> &obs, inputObservations){
        x[k] = obs->getXYZ().getAt(0);
        y[k] = obs->getXYZ().getAt(1);
        z[k] = obs->getXYZ().getAt(2);

        xc += x[k];
        yc += y[k];
        zc += z[k];

        k++;
    }

    xc /= obsCount;
    yc /= obsCount;
    zc /= obsCount;

    for(int i=0; i<obsCount; i++)
        rc += sqrt((x[i]-xc)*(x[i]-xc) + (y[i]-yc)*(y[i]-yc) + (z[i]-zc)*(z[i]-zc));
    rc /= obsCount;

    OiVec plane = this->getApproximatedPlane(obsCount, x, y, z, xc, yc, zc);

    int maxItr = 50;
    double maxAbsDu = 1.0E308;
    double nx = plane.getAt(0);
    double ny = plane.getAt(1);
    double nz = plane.getAt(2);
    double d  = plane.getAt(3);

    // Stelle Matrizen fuer GH-Modell auf; in einer spaeteren Implementierung sollte die NGL direkt aufgebaut werden
    // um Speicherplatz zu sparen. Fuer wenige Punkte (und aus Gruenden der Uebersicht) wird im Moment darauf verzichtet.
    // Aus numerischen Gruenden koennte mit Schwerpunktreduzierten Daten gearbeitet werden.
    // u = [x0, y0, z0, r0, nx0, ny0, nz0, d0] - Korrekte Reihenfolge durch ENUM vorgegeben.

    OiVec v(3*obsCount);
    OiMat Qxx(8,8);

    int itr = 0;
    while(true) {

        OiMat A(2*obsCount, 8);
        OiMat B(2*obsCount, 3*obsCount);
        OiVec w(2*obsCount);

        for(int i=0; i<obsCount; i++){
            double xP = x[i] - v.getAt(3*i);
            double yP = y[i] - v.getAt(3*i+1);
            double zP = z[i] - v.getAt(3*i+2);

            // Kugelparameter
            A.setAt(i, Circle::unknownCenterX, -2.0*(xP - xc));
            A.setAt(i, Circle::unknownCenterY, -2.0*(yP - yc));
            A.setAt(i, Circle::unknownCenterZ, -2.0*(zP - zc));
            A.setAt(i, Circle::unknownRadius, -2.0*rc);

            B.setAt(i, i*3,   2.0*(xP - xc));
            B.setAt(i, i*3+1, 2.0*(yP - yc));
            B.setAt(i, i*3+2, 2.0*(zP - zc));

            w.setAt(i, rc*rc - ((xP-xc)*(xP-xc) + (yP-yc)*(yP-yc) + (zP-zc)*(zP-zc)));

            // Ebene in Normalform
            A.setAt(obsCount+i, Circle::unknownNormalI, xP);
            A.setAt(obsCount+i, Circle::unknownNormalJ, yP);
            A.setAt(obsCount+i, Circle::unknownNormalK, zP);
            A.setAt(obsCount+i, 7, -1.0);

            B.setAt(obsCount+i, i*3,   nx);
            B.setAt(obsCount+i, i*3+1, ny);
            B.setAt(obsCount+i, i*3+2, nz);

            w.setAt(obsCount+i, d - (nx*xP + ny*yP + nz*zP));
        }

        w = -1*B*v + w;

        OiMat Qww = B * B.t();
        OiMat Pww = Qww.inv();
        OiMat NGL = A.t() * Pww * A;
        OiVec ngl = A.t() * Pww * w;

        OiMat NGLR(NGL.getRowCount() + 2, NGL.getColCount() + 2);
        OiVec nglr(ngl.getSize() + 2);

        for (int i=0; i<NGL.getRowCount(); i++) {
            nglr.setAt(i, ngl.getAt(i));
            for (int j=0; j<NGL.getColCount(); j++)
                NGLR.setAt(i,j, NGL.getAt(i,j));
        }

        NGLR.setAt(NGL.getRowCount(), Circle::unknownCenterX, nx);
        NGLR.setAt(NGL.getRowCount(), Circle::unknownCenterY, ny);
        NGLR.setAt(NGL.getRowCount(), Circle::unknownCenterZ, nz);
        NGLR.setAt(NGL.getRowCount(), Circle::unknownNormalI, xc);
        NGLR.setAt(NGL.getRowCount(), Circle::unknownNormalJ, yc);
        NGLR.setAt(NGL.getRowCount(), Circle::unknownNormalK, zc);
        NGLR.setAt(NGL.getRowCount(), 7, -1.0);
        nglr.setAt(NGL.getRowCount(), d - (nx*xc + ny*yc + nz*zc));

        NGLR.setAt(NGL.getRowCount()+1, Circle::unknownNormalI, 2.0*nx);
        NGLR.setAt(NGL.getRowCount()+1, Circle::unknownNormalJ, 2.0*ny);
        NGLR.setAt(NGL.getRowCount()+1, Circle::unknownNormalK, 2.0*nz);
        nglr.setAt(NGL.getRowCount()+1, 1.0 - (nx*nx + ny*ny + nz*nz));

        NGLR.setAt(Circle::unknownCenterX, NGL.getRowCount(), nx);
        NGLR.setAt(Circle::unknownCenterY, NGL.getRowCount(), ny);
        NGLR.setAt(Circle::unknownCenterZ, NGL.getRowCount(), nz);
        NGLR.setAt(Circle::unknownNormalI, NGL.getRowCount(), xc);
        NGLR.setAt(Circle::unknownNormalJ, NGL.getRowCount(), yc);
        NGLR.setAt(Circle::unknownNormalK, NGL.getRowCount(), zc);
        NGLR.setAt(7, NGL.getRowCount(), -1.0);

        NGLR.setAt(Circle::unknownNormalI, NGL.getRowCount()+1, 2.0*nx);
        NGLR.setAt(Circle::unknownNormalJ, NGL.getRowCount()+1, 2.0*ny);
        NGLR.setAt(Circle::unknownNormalK, NGL.getRowCount()+1, 2.0*nz);

        OiMat QxxR = NGLR.inv();
        OiVec duk  = QxxR * nglr;
        OiVec du(A.getColCount());

        xc += duk.getAt(Circle::unknownCenterX);
        yc += duk.getAt(Circle::unknownCenterY);
        zc += duk.getAt(Circle::unknownCenterZ);
        rc += duk.getAt(Circle::unknownRadius);

        nx += duk.getAt(Circle::unknownNormalI);
        ny += duk.getAt(Circle::unknownNormalJ);
        nz += duk.getAt(Circle::unknownNormalK);
        d  += duk.getAt(7);

        maxAbsDu = duk.getAt(0);
        for (int i=0; i<du.getSize(); i++) {
            du.setAt(i, duk.getAt(i));
            maxAbsDu = max(maxAbsDu, fabs(duk.getAt(i)));

            // Dies waere nur am Ende der letzten Iteration notwendig
            for (int j=0; j<du.getSize(); j++)
                Qxx.setAt(i,j,QxxR.getAt(i,j));
        }

        v = B.t()*Pww*(A*du-w);

        if (itr<maxItr && maxAbsDu < 1.0E-8) {
            qDebug() << "DEBUG: Found Solution!";
            break;
        }
        else if (itr>maxItr) {
            qDebug() << "DEBUG: Estimation failed!";
            return false;
        }
        itr++;
    }

    // Statistik
    Statistic myStats;
    myStats.setS0APriori(1.0);
    double vv;
    OiVec::dot(vv, v, v);
    myStats.setS0APosteriori((obsCount-6) > 0 ? sqrt(vv/(obsCount-6.0)) : 1.0);

    Position circlePosition;
    OiVec xyz(3);
    xyz.setAt(0, xc);
    xyz.setAt(1, yc);
    xyz.setAt(2, zc);
    circlePosition.setVector(xyz);

    Direction circleDirection;
    OiVec ijk(3);
    ijk.setAt(0, nx);
    ijk.setAt(1, ny);
    ijk.setAt(2, nz);
    circleDirection.setVector(ijk);

    Radius circleRadius;
    circleRadius.setRadius(rc);

    circle.setCircle(circlePosition, circleDirection, circleRadius);

    myStats.setV(v);
    for(int i=0; i<obsCount; i++){
        Residual r;
        r.elementId = inputObservations.at(i)->getId();
        r.corrections.insert("vx", v.getAt(i*3));
        r.corrections.insert("vx", v.getAt(3*i+1));
        r.corrections.insert("vx", v.getAt(3*i+2));
        r.dimension = eMetric;
        myStats.addDisplayResidual(r);
    }
    myStats.setQxx(Qxx);

    myStats.setIsValid(true);
    circle.setStatistic(myStats);
    this->statistic = circle.getStatistic();

    return true;

}

/**
 * Estimate the approx. Plane. SourceCode is taken from
 * com.derletztekick.geodesy.formFittingToolbox.v2.form.form3d.Circle3D
 * @brief BestFitCircle::getApproximatedPlane
 * @param nop Number of Points
 * @param x X-values
 * @param y Y-values
 * @param z Z-values
 * @param cx X-value of centroid
 * @param cy Y-value of centroid
 * @param cz Z-value of centroid
 * @return plane
 */
OiVec BestFitCircle::getApproximatedPlane(int nop, double x[], double y[], double z[], double cx, double cy, double cz) {
    OiVec X(4);
    OiMat H(3,3);
    for (int k=0; k<nop; k++) {
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                double a=0, b=0;
                if (i==0)
                    a = x[k] - cx;
                else if (i==1)
                    a = y[k] - cy;
                else
                    a = z[k] - cz;

                if (j==0)
                    b = x[k] - cx;
                else if (j==1)
                    b = y[k] - cy;
                else
                    b = z[k] - cz;
                H.setAt(i,j, H.getAt(i,j) + a * b);
            }
        }
    }
    OiMat u(3,3);
    OiVec d(3);
    OiMat v(3,3);
    H.svd(u, d, v);
    int indexMinEigVal = 0;
    double minEigVal = d.getAt(indexMinEigVal);
    for (int i=indexMinEigVal+1; i<d.getSize(); i++) {
        if (minEigVal > d.getAt(i)) {
            minEigVal = d.getAt(i);
            indexMinEigVal = i;
        }
    }
    for (int i=0; i<3; i++)
        X.setAt(i, v.getAt(i, indexMinEigVal));
    X.setAt(3, X.getAt(0)*cx + X.getAt(1)*cy + X.getAt(2)*cz);
    return X;
}
