#include "p_bestfitcircle.h"

BestFitCircle::BestFitCircle(){}

PluginMetaData *BestFitCircle::getMetaData() {
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "BestFitCircle";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "Michael Loesler - derletztekick.com";
    metaData->description = QString("%1 %2")
            .arg("This function estimates a spatial circle usnig a general least squares algorithm.")
            .arg("Insert at least three non-collinear observed points, to get the best fit circle.")
            .arg("The algorithm was taken from the OpenSource FormFittingToolbox (c) M.Loesler (GNU-GPL).");
    metaData->iid = "de.openIndy.Plugin.Function.FitFunction.v001";

    return metaData;
}

bool BestFitCircle::exec(Circle &circle) {

    int obsCount = this->getObservationCount();

    //if enough observations available
    if(this->isValid() && obsCount >= 3){

        //fill x,y,z arrays
        double *x = new double[obsCount];
        double *y = new double[obsCount];
        double *z = new double[obsCount];

        int k = 0;

        /*
        foreach(Point *point, this->points){
            OiVec xyz = point->xyz;
            x[k] = xyz.getAt(0);
            y[k] = xyz.getAt(1);
            z[k] = xyz.getAt(2);
            k++;
        }
        */

        double xc = 0, yc = 0, zc = 0, rc = 0;

        foreach(Observation *obs, this->observations){
            if(obs->isValid){
                x[k] = obs->myXyz.getAt(0);
                y[k] = obs->myXyz.getAt(1);
                z[k] = obs->myXyz.getAt(2);

                xc += x[k];
                yc += y[k];
                zc += z[k];

                k++;
            }
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

        qDebug() << QString::number(xc) + ", " + QString::number(yc) + ", " + QString::number(zc) + ", " + QString::number(rc);
        qDebug() << QString::number(nx) + ", " + QString::number(ny) + ", " + QString::number(nz) + ", " + QString::number(d);

        // Stelle Matrizen fuer GH-Modell auf; in einer spaeteren Implementierung sollte die NGL direkt aufgebaut werden
        // um Speicherplatz zu sparen. Fuer wenige Punkte (und aus Gruenden der Uebersicht) wird im Moment darauf verzichtet.
        // Aus numerischen Gruenden koennte mit Schwerpunktreduzierten Daten gearbeitet werden.
        // u = [x0, y0, z0, r0, nx0, ny0, nz0, d0] - Korrekte Reihenfolge durch ENUM vorgegeben.

        OiVec v(3*obsCount);
        OiMat Qxx(8,8);

        int itr = 0;
        while(true) {
            qDebug() << "DEBUG: Starte Iteation " + QString::number( itr+1 );

            OiMat A(2*obsCount, 8);
            OiMat B(2*obsCount, 3*obsCount);
            OiVec w(2*obsCount);

            for(int i=0; i<obsCount; i++){
                double xP = x[i] - v.getAt(3*i);
                double yP = y[i] - v.getAt(3*i+1);
                double zP = z[i] - v.getAt(3*i+2);

                //qDebug() << "DEBUG: Fuege Punkt P " + QString::number( xP ) + "/" + QString::number( yP ) + "/" + QString::number( zP ) + " hinzu.";

                // Kugelparameter
                A.setAt(i, Circle::unknownX, -2.0*(xP - xc));
                A.setAt(i, Circle::unknownY, -2.0*(yP - yc));
                A.setAt(i, Circle::unknownZ, -2.0*(zP - zc));
                A.setAt(i, Circle::unknownR, -2.0*rc);

                B.setAt(i, i*3,   2.0*(xP - xc));
                B.setAt(i, i*3+1, 2.0*(yP - yc));
                B.setAt(i, i*3+2, 2.0*(zP - zc));

                w.setAt(i, rc*rc - ((xP-xc)*(xP-xc) + (yP-yc)*(yP-yc) + (zP-zc)*(zP-zc)));

                // Ebene in Normalform
                A.setAt(obsCount+i, Circle::unknownI, xP);
                A.setAt(obsCount+i, Circle::unknownJ, yP);
                A.setAt(obsCount+i, Circle::unknownK, zP);
                A.setAt(obsCount+i, Circle::unknownD, -1.0);

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

            NGLR.setAt(NGL.getRowCount(), Circle::unknownX, nx);
            NGLR.setAt(NGL.getRowCount(), Circle::unknownY, ny);
            NGLR.setAt(NGL.getRowCount(), Circle::unknownZ, nz);
            NGLR.setAt(NGL.getRowCount(), Circle::unknownI, xc);
            NGLR.setAt(NGL.getRowCount(), Circle::unknownJ, yc);
            NGLR.setAt(NGL.getRowCount(), Circle::unknownK, zc);
            NGLR.setAt(NGL.getRowCount(), Circle::unknownD, -1.0);
            nglr.setAt(NGL.getRowCount(), d - (nx*xc + ny*yc + nz*zc));

            NGLR.setAt(NGL.getRowCount()+1, Circle::unknownI, 2.0*nx);
            NGLR.setAt(NGL.getRowCount()+1, Circle::unknownJ, 2.0*ny);
            NGLR.setAt(NGL.getRowCount()+1, Circle::unknownK, 2.0*nz);
            nglr.setAt(NGL.getRowCount()+1, 1.0 - (nx*nx + ny*ny + nz*nz));

            NGLR.setAt(Circle::unknownX, NGL.getRowCount(), nx);
            NGLR.setAt(Circle::unknownY, NGL.getRowCount(), ny);
            NGLR.setAt(Circle::unknownZ, NGL.getRowCount(), nz);
            NGLR.setAt(Circle::unknownI, NGL.getRowCount(), xc);
            NGLR.setAt(Circle::unknownJ, NGL.getRowCount(), yc);
            NGLR.setAt(Circle::unknownK, NGL.getRowCount(), zc);
            NGLR.setAt(Circle::unknownD, NGL.getRowCount(), -1.0);

            NGLR.setAt(Circle::unknownI, NGL.getRowCount()+1, 2.0*nx);
            NGLR.setAt(Circle::unknownJ, NGL.getRowCount()+1, 2.0*ny);
            NGLR.setAt(Circle::unknownK, NGL.getRowCount()+1, 2.0*nz);

            /*
            for (int i=0; i<NGLR.getRowCount(); i++) {
                for (int j=0; j<NGLR.getColCount(); j++)
                    qDebug() << "NGLR("+QString::number(i+1)+", "+QString::number(j+1)+") = "+QString::number( NGLR.getAt(i,j)) +";";
            }
            */

            OiMat QxxR = NGLR.inv();
            OiVec duk  = QxxR * nglr;
            OiVec du(A.getColCount());

            xc += duk.getAt(Circle::unknownX);
            yc += duk.getAt(Circle::unknownY);
            zc += duk.getAt(Circle::unknownZ);
            rc += duk.getAt(Circle::unknownR);

            nx += duk.getAt(Circle::unknownI);
            ny += duk.getAt(Circle::unknownJ);
            nz += duk.getAt(Circle::unknownK);
            d  += duk.getAt(Circle::unknownD);

            maxAbsDu = duk.getAt(0);
            for (int i=0; i<du.getSize(); i++) {
                du.setAt(i, duk.getAt(i));
                maxAbsDu = max(maxAbsDu, fabs(duk.getAt(i)));

                // Dies waere nur am Ende der letzten Iteration notwendig
                for (int j=0; j<du.getSize(); j++)
                    Qxx.setAt(i,j,QxxR.getAt(i,j));
            }

            v = B.t()*Pww*(A*du-w);

            qDebug() << "DEBUG: Ende Iteation " + QString::number( itr+1 ) + "  max(|dx|) = " + QString::number( maxAbsDu );

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
        qDebug() << QString::number(xc) + ", " + QString::number(yc) + ", " + QString::number(zc) + ", "  + QString::number(rc);
        qDebug() << QString::number(nx) + ", " + QString::number(ny) + ", " + QString::number(nz) + ", "  + QString::number(d);

        // Statistik
        Statistic myStats = circle.getStatistic();
        myStats.s0_apriori     = 1.0;
        myStats.s0_aposteriori = (obsCount-6) > 0 ? sqrt(OiVec::dot(v,v)/(obsCount-6)) : 1.0;

        circle.xyz.setAt(0, xc);
        circle.xyz.setAt(1, yc);
        circle.xyz.setAt(2, zc);
        circle.radius = rc;
        circle.ijk.setAt(0, nx);
        circle.ijk.setAt(1, ny);
        circle.ijk.setAt(2, nz);
        circle.dist2origin = d;

        myStats.v.replace(v);
        for(int i=0; i<obsCount; i++){
            Residual r;
            r.addValue("vx", v.getAt(i*3),   UnitConverter::eMetric);
            r.addValue("vy", v.getAt(3*i+1), UnitConverter::eMetric);
            r.addValue("vz", v.getAt(3*i+2), UnitConverter::eMetric);
            myStats.displayResiduals.append(r);
        }
        myStats.qxx = Qxx;

        myStats.isValid = true;
        circle.setStatistic(myStats);
        this->myStatistic = circle.getStatistic();

        return true;
    }
    return false;
}

QList<InputParams> BestFitCircle::getNeededElements() {
    QList<InputParams> result;
    InputParams param;
    param.index = 0;
    param.description = "Select at least three non-collinear observed points to calculate the best fit circle.";
    param.infinite = true;
    param.typeOfElement = Configuration::eObservationElement;
    result.append(param);

//    InputParams param;
//    param.index = 0;
//    param.description = "Select at least 3 non-collinear observed points to calculate the best fit circle.";
//    param.infinite = true;
//    param.typeOfElement = Configuration::ePointElement;
//    result.append(param);

    return result;
}

QList<Configuration::FeatureTypes> BestFitCircle::applicableFor() {
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::eCircleFeature);
    return result;
}
/**
 * Returns the number of valid Points
 * @brief BestFitCircle::getObservationCount
 * @return nop
 */
int BestFitCircle::getObservationCount(){
    int count = 0;
    foreach(Observation *obs, this->observations){
        if(obs->isValid){
            this->setUseState(obs->getId(), true);
            count++;
        }
        else
            this->setUseState(obs->getId(), false);
    }
    return count;
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
