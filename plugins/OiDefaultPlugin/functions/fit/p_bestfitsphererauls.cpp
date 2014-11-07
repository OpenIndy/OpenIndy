#include "p_bestfitsphererauls.h"

PluginMetaData *BestFitSphereRauls::getMetaData() const{
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "BestFitSphereRauls";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1 %2")
            .arg("This function calculates an adjusted sphere.")
            .arg("You can input as many observations as you want which are then used to find the best fit sphere.");
    metaData->iid = "de.openIndy.Plugin.Function.FitFunction.v001";
    return metaData;
}

bool BestFitSphereRauls::exec(Sphere &s){
    if(this->isValid() && this->setUpObservations() >= 4){

        //calculate approximate sphere parameters
        if(this->approximate(s)){

            return this->fit(s);

        }

    }else{
        this->writeToConsole("Not enough observations available for calculation");
    }
    return false;
}

QList<InputParams> BestFitSphereRauls::getNeededElements() const{
    QList<InputParams> result;
    InputParams param;
    param.index = 0;
    param.description = "Select at least four observations to calculate the best fit sphere.";
    param.infinite = true;
    param.typeOfElement = Configuration::eObservationElement;
    result.append(param);
    return result;
}

QList<Configuration::FeatureTypes> BestFitSphereRauls::applicableFor() const{
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::eSphereFeature);
    return result;
}

/*!
 * \brief BestFitSphereRauls::setUpObservations
 * Return number of valid observations and save them into a special list
 * \return
 */
int BestFitSphereRauls::setUpObservations(){
    this->myValidObservations.clear();
    int count = 0;
    foreach(Observation *obs, this->observations){
        if(obs->getUseState()){
            this->myValidObservations.append(obs);
            count++;
        }
    }
    return count;
}

bool BestFitSphereRauls::approximate(Sphere &s){

    int numPoints = this->myValidObservations.size();

    //calculate centroid coordinates
    OiVec centroid(3);
    for(int i = 0; i < numPoints; i++){
        OiVec x(3);
        x.setAt(0, this->myValidObservations.at(i)->myXyz.getAt(0));
        x.setAt(1, this->myValidObservations.at(i)->myXyz.getAt(1));
        x.setAt(2, this->myValidObservations.at(i)->myXyz.getAt(2));
        centroid += x;
    }
    centroid = centroid / numPoints;

    //Drixler
    OiMat N(4, 4);
    OiVec n(4);
    for(int i = 0; i < numPoints; i++){
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;

        x = this->myValidObservations.at(i)->myXyz.getAt(0) - centroid.getAt(0);
        y = this->myValidObservations.at(i)->myXyz.getAt(1) - centroid.getAt(1);
        z = this->myValidObservations.at(i)->myXyz.getAt(2) - centroid.getAt(2);

        double xx = x*x;
        double yy = y*y;
        double zz = z*z;
        double xy = x*y;
        double xz = x*z;
        double yz = y*z;
        double xyz2 = x*x + y*y + z*z;

        N.setAt(0,0, N.getAt(0,0) + xx);
        N.setAt(0,1, N.getAt(0,1) + xy);
        N.setAt(0,2, N.getAt(0,2) + xz);
        N.setAt(0,3, N.getAt(0,3) + x);
        N.setAt(1,0, N.getAt(1,0) + xy);
        N.setAt(1,1, N.getAt(1,1) + yy);
        N.setAt(1,2, N.getAt(1,2) + yz);
        N.setAt(1,3, N.getAt(1,3) + y);
        N.setAt(2,0, N.getAt(2,0) + xz);
        N.setAt(2,1, N.getAt(2,1) + yz);
        N.setAt(2,2, N.getAt(2,2) + zz);
        N.setAt(2,3, N.getAt(2,3) + z);
        N.setAt(3,0, N.getAt(3,0) + x);
        N.setAt(3,1, N.getAt(3,1) + y);
        N.setAt(3,2, N.getAt(3,2) + z);
        N.setAt(3,3, N.getAt(3,3) + 1.0);

        n.setAt(0, n.getAt(0) + x*xyz2);
        n.setAt(1, n.getAt(1) + y*xyz2);
        n.setAt(2, n.getAt(2) + z*xyz2);
        n.setAt(3, n.getAt(3) + xyz2);
    }

    OiMat Q(4,4);
    try{
        Q = N.inv();
    }catch(exception &e){
        this->writeToConsole(e.what());
        return false;
    }

    OiVec a = -1.0 * Q * n;

    double r = 0.0;
    double xm = 0.0;
    double ym = 0.0;
    double zm = 0.0;

    r = qSqrt( qAbs( 0.25 * (a.getAt(0)*a.getAt(0) + a.getAt(1)*a.getAt(1) + a.getAt(2)*a.getAt(2)) - a.getAt(3) ) );
    xm = -0.5 * a.getAt(0) + centroid.getAt(0);
    ym = -0.5 * a.getAt(1) + centroid.getAt(1);
    zm = -0.5 * a.getAt(2) + centroid.getAt(2);

    //set approximate result
    s.radius = r;
    s.xyz.setAt(0, xm);
    s.xyz.setAt(1, ym);
    s.xyz.setAt(2, zm);

    return true;

}

bool BestFitSphereRauls::fit(Sphere &s){

    int numPoints = this->myValidObservations.size();

    int numIterations = 0;

    //set approximation values
    double xm = s.xyz.getAt(0);
    double ym = s.xyz.getAt(1);
    double zm = s.xyz.getAt(2);
    double r = s.radius;

    OiVec xd(4);
    double xdxd = 0.0;
    OiVec verb(numPoints*3);
    do{

        OiMat A(numPoints, 4);
        OiMat B(numPoints, numPoints*3);
        OiVec w(numPoints);
        for(int i = 0; i < numPoints; i++){

            OiVec x(3);
            x.setAt(0, this->myValidObservations.at(i)->myXyz.getAt(0));
            x.setAt(1, this->myValidObservations.at(i)->myXyz.getAt(1));
            x.setAt(2, this->myValidObservations.at(i)->myXyz.getAt(2));

            double r0 = qSqrt( (x.getAt(0) + verb.getAt(i*3) - xm) * (x.getAt(0) + verb.getAt(i*3) - xm)
                               + (x.getAt(1) + verb.getAt(i*3+1) - ym) * (x.getAt(1) + verb.getAt(i*3+1) - ym)
                               + (x.getAt(2) + verb.getAt(i*3+2) - zm) * (x.getAt(2) + verb.getAt(i*3+2) - zm) );

            A.setAt(i, 0, -1.0 * (x.getAt(0) + verb.getAt(i*3) - xm) / r0);
            A.setAt(i, 1, -1.0 * (x.getAt(1) + verb.getAt(i*3+1) - ym) / r0);
            A.setAt(i, 2, -1.0 * (x.getAt(2) + verb.getAt(i*3+2) - zm) / r0);
            A.setAt(i, 3, -1.0);

            B.setAt(i, i*3, (x.getAt(0) + verb.getAt(i*3) - xm) / r0);
            B.setAt(i, i*3+1, (x.getAt(1) + verb.getAt(i*3+1) - ym) / r0);
            B.setAt(i, i*3+2, (x.getAt(2) + verb.getAt(i*3+2) - zm) / r0);

            w.setAt(i, r0 - r);

        }

        OiMat BBT = B * B.t();
        OiMat AT = A.t();

        OiMat N(numPoints+4, numPoints+4);
        for(int i = 0; i < BBT.getRowCount(); i++){
            for(int j = 0; j < BBT.getColCount(); j++){
                N.setAt(i,j, BBT.getAt(i,j));
            }
        }
        for(int i = 0; i < numPoints; i++){
            for(int j = 0; j < 4; j++){
                N.setAt(i, j+numPoints, A.getAt(i,j));
            }
        }
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < numPoints; j++){
                N.setAt(i+numPoints, j, AT.getAt(i,j));
            }
        }

        for(int i = 0; i < 4; i++){
            w.add(0.0);
        }

        OiMat Q(numPoints+4, numPoints+4);
        try{
            Q = N.inv();
        }catch(exception &e){
            this->writeToConsole(e.what());
            return false;
        }

        OiVec res = -1.0 * Q * w;

        OiVec k(numPoints);
        for(int i = 0; i < numPoints; i++){
            k.setAt(i, res.getAt(i));
        }
        for(int i = 0; i < 4; i++){
            xd.setAt(i, res.getAt(i+numPoints));
        }

        OiVec vd = B.t() * k;

        //Unbekannte und Beobachtungen verbessern
        verb = verb + vd;
        xm += xd.getAt(0);
        ym += xd.getAt(1);
        zm += xd.getAt(2);
        r += xd.getAt(3);

        numIterations++;

        OiVec::dot(xdxd, xd,xd);

    }while( (xdxd > 0.000001) && (numIterations < 101) );

    if(numIterations >= 101){
        this->writeToConsole("No solution found during 100 iterations");
        return false;
    }

    //set final result
    s.radius = r;
    s.xyz.setAt(0, xm);
    s.xyz.setAt(1, ym);
    s.xyz.setAt(2, zm);

    double stdv = 0.0;
    if(verb.getSize() > 12){
        double sum_vv = 0.0;
        for(int i = 0; i < verb.getSize(); i++){
            sum_vv += verb.getAt(i) * verb.getAt(i);
        }
        stdv = qSqrt(sum_vv / (verb.getSize() - 12));
    }else{
        stdv = 0.0;
    }
    Statistic myStats;
    myStats.s0_apriori = 1.0;
    myStats.s0_aposteriori = stdv;
    myStats.stdev = stdv;
    myStats.isValid = true;

    s.setStatistic(myStats);
    this->myStatistic = myStats;

    return true;

}
