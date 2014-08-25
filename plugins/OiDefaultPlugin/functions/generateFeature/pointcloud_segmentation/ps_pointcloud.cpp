#include "ps_pointcloud.h"

#include "ps_planesegment.h"
#include "ps_spheresegment.h"
#include "ps_cylindersegment.h"

QList<PS_Node*> PS_PointCloud::mergedNodes;

PS_PointCloud::PS_PointCloud(QObject *parent) : QObject(parent)
{
    this->myPoints = new vector<PS_Point_PC*>;
    this->num_points = 0;

    this->myBoundingBox.min[0] = numeric_limits<float>::max();
    this->myBoundingBox.min[1] = numeric_limits<float>::max();
    this->myBoundingBox.min[2] = numeric_limits<float>::max();
    this->myBoundingBox.max[0] = -numeric_limits<float>::max();
    this->myBoundingBox.max[1] = -numeric_limits<float>::max();
    this->myBoundingBox.max[2] = -numeric_limits<float>::max();

    this->myOctree = NULL;

    this->c1 = clock();
}

PS_PointCloud::PS_PointCloud(const PS_PointCloud &copy){
    this->myPoints = copy.myPoints;
    this->myBoundingBox = copy.myBoundingBox;
    this->myOctree = copy.myOctree;
    this->num_points = copy.num_points;
    this->detectedPlanes = copy.detectedPlanes;
    this->detectedSpheres = copy.detectedSpheres;
    this->detectedCylinders = copy.detectedCylinders;
}

PS_PointCloud &PS_PointCloud::operator=(const PS_PointCloud &copy){
    this->myPoints = copy.myPoints;
    this->myBoundingBox = copy.myBoundingBox;
    this->myOctree = copy.myOctree;
    this->num_points = copy.num_points;
    this->detectedPlanes = copy.detectedPlanes;
    this->detectedSpheres = copy.detectedSpheres;
    this->detectedCylinders = copy.detectedCylinders;
    return *this;
}

/*!
 * \brief PointCloud::loadPointCloud
 * Load a point cloud from PTS
 * \param fileName
 */
bool PS_PointCloud::loadPointCloud(QString fileName){

    qDebug() << "start einlesen";

    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return false;
    }

    try{

        this->filePath = file.fileName();

        QTextStream in(&file);

        while(!in.atEnd()) {

            QString line = in.readLine();

            QStringList fields = line.split(' ');

            if(fields.length() >= 3){

                //create a new point and add it to the point cloud
                PS_Point_PC *myPoint = new PS_Point_PC();
                myPoint->xyz[0] = fields.at(0).toFloat();
                myPoint->xyz[1] = fields.at(1).toFloat();
                myPoint->xyz[2] = fields.at(2).toFloat();
                this->myPoints->push_back(myPoint);
                this->num_points++;

                //update bounding box
                if(myPoint->xyz[0] < this->myBoundingBox.min[0]){
                    this->myBoundingBox.min[0] = myPoint->xyz[0];
                }
                if(myPoint->xyz[0] > this->myBoundingBox.max[0]){
                    this->myBoundingBox.max[0] = myPoint->xyz[0];
                }
                if(myPoint->xyz[1] < this->myBoundingBox.min[1]){
                    this->myBoundingBox.min[1] = myPoint->xyz[1];
                }
                if(myPoint->xyz[1] > this->myBoundingBox.max[1]){
                    this->myBoundingBox.max[1] = myPoint->xyz[1];
                }
                if(myPoint->xyz[2] < this->myBoundingBox.min[2]){
                    this->myBoundingBox.min[2] = myPoint->xyz[2];
                }
                if(myPoint->xyz[2] > this->myBoundingBox.max[2]){
                    this->myBoundingBox.max[2] = myPoint->xyz[2];
                }
            }

        }
    }catch(exception &e){
        file.close();
        qDebug() << e.what();
        return false;
    }

    file.close();

    cout << "einlesen fertig " << (clock() - c1)/(double)CLOCKS_PER_SEC << " seconds." << endl;

    return true;
}

/*!
 * \brief PS_PointCloud::setCloud
 * Manually set a previously loaded point cloud
 * \param myPoints
 * \param bbox
 * \param numPoints
 * \return
 */
bool PS_PointCloud::setCloud(vector<PS_Point_PC *> *myPoints, PS_BoundingBox_PC bbox, unsigned long numPoints){

    this->myPoints = myPoints;
    this->myBoundingBox = bbox;
    this->num_points = numPoints;
    return true;

}

/*!
 * \brief PointCloud::setUpOctree
 * \return
 */
bool PS_PointCloud::setUpOctree(PS_InputParameter param)
{
    qDebug() << "start octree";
    c1 = clock();

    this->myOctree = new PS_Octree();
    this->myOctree->setUp(this->myPoints, &this->myBoundingBox, param.leafSize);

    cout << "octree komplett fertig " << (clock() - c1)/(double)CLOCKS_PER_SEC << " seconds." << endl;
    return true;
}

/*!
 * \brief PointCloud::detectShapes
 * \param param
 * \return
 */
bool PS_PointCloud::detectShapes(PS_InputParameter param){

    if(this->myOctree->getIsValid()){

        emit this->updateStatus("Octree successfully created", 1);

        qDebug() << "start detecting shapes";
        c1 = clock();

        //get leaf nodes from Octree (= nodes that are not subdevided into smaller nodes)
        //the nodes are sorted from containing many points to containing less points
        QList<PS_Node *> *leafs = this->myOctree->getLeafs();

        /*float r = 0.0;
        float g = 0.0;
        float b = 0.0;

        //print all leafs to check octree
        QFile fileOctree( "D:/UNI/Master/Semester_04/Masterarbeit/pointcloudSegmentation/result/octree.pts" );
        QTextStream streama( &fileOctree );
        if ( fileOctree.open(QIODevice::ReadWrite | QIODevice::Truncate) ){



            std::random_device rd;
            int seed = rd();
            std::default_random_engine generator( seed );
            uniform_int_distribution<int> distribution(0, 255);

            for(int i = 0; i < leafs->size(); i++){
                if(leafs->at(i)->points.size() > 0){
                    r = distribution(generator);
                    g = distribution(generator);
                    b = distribution(generator);

                    for(int j = 0; j < leafs->at(i)->points.size(); j++){
                        //if(!leafs->at(i)->points.at(j)->isUsed){
                            streama << QString::number(leafs->at(i)->points.at(j)->xyz[0])
                                    << " " << QString::number(leafs->at(i)->points.at(j)->xyz[1])
                                    << " " << QString::number(leafs->at(i)->points.at(j)->xyz[2])
                                    << " " << r << " " << g << " " << b
                                    << endl;
                            //leafs->at(i)->points.at(j)->isUsed = true;
                        //}
                    }

                }
            }


        }*/

        qDebug() << "number of leafs at all: " << leafs->size();

        int numPointLeafs = 0;
        for(int i = 0; i < leafs->size(); i++){
            if(leafs->at(i)->points.size() > 0){
                numPointLeafs++;
            }
        }

        qDebug() << "number of leafs with points: " << numPointLeafs;

        cout << "Start Geometrieerkennung: " << (clock() - c1)/(double)CLOCKS_PER_SEC << " seconds." << endl;

        bool resetUnmerged = false; //if true all nodes are set as unmerged (eg after a plane detection before spheres)

        unsigned long numUsedPoints = 0; //number of already used points

        clock_t test = clock();

        //list to save all unused points of each node
        QList<PS_Point_PC *> unusedPoints;

        //list to save all unmerged points of each node
        QList<PS_Point_PC *> unmergedPoints;

        //iterate through all leaf nodes starting with the one with the most points
        for(int i = 0; i < leafs->size(); i++){

            /*std::random_device rd;
            int seed = rd();
            std::default_random_engine generator( seed );
            uniform_int_distribution<int> distribution(0, 255);

                if(leafs->at(i)->points.size() > 0){
                    r = distribution(generator);
                    g = distribution(generator);
                    b = distribution(generator);

                    for(int j = 0; j < leafs->at(i)->points.size(); j++){
                        if(!leafs->at(i)->points.at(j)->isUsed){
                            streama << QString::number(leafs->at(i)->points.at(j)->xyz[0])
                                    << " " << QString::number(leafs->at(i)->points.at(j)->xyz[1])
                                    << " " << QString::number(leafs->at(i)->points.at(j)->xyz[2])
                                    << " " << r << " " << g << " " << b
                                    << endl;
                            //leafs->at(i)->points.at(j)->isUsed = true;
                        }
                    }

                }*/

            //qDebug() << i;
            //cout << "start: " << (clock() - test)/(double)CLOCKS_PER_SEC << " seconds." << endl;

            //leafs with 10 or less points are not considered, so break here because the list is ordered
            if(leafs->at(i)->points.size() <= 10){
                //break;
                continue;
            }

            //check wether there are still enough points unused to be able to find a geometry
            if( (!param.planeParams.detectPlanes || this->myPoints->size() - numUsedPoints < param.planeParams.minPoints)
                    && (!param.sphereParams.detectSpheres || this->myPoints->size() - numUsedPoints < param.sphereParams.minPoints)
                    && (!param.cylinderParams.detectCylinders || this->myPoints->size() - numUsedPoints < param.cylinderParams.minPoints)){
                break;
            }

            resetUnmerged = false;

            //get the current node to be considered and set it as considered in merge
            PS_Node *n = leafs->at(i);
            n->setWasConsideredAsSeed(true);
            n->setWasConsideredInMerge(true);

            //add node to list of merged nodes
            PS_PointCloud::mergedNodes.append(n);







            //nodes that contain more than 10 unused points are considered
            if(n->getUnusedPointsCount() > 10){

                /*
                 * foreach node try to detect a plane, a sphere and a cylinder
                 * and chose the one that best fits (sigma-value)
                 */

                //create shape variables here and set them to NULL
                PS_PlaneSegment *p = NULL;
                PS_SphereSegment *s = NULL;
                PS_CylinderSegment *c = NULL;

                //get a list of unsused points of node n
                n->getUnusedPoints(unusedPoints);

                /*
                 * detect plane if requested by user
                 */
                if(param.planeParams.detectPlanes){

                    bool planeValid = false;

                    //cout << "vor min: " << (clock() - test)/(double)CLOCKS_PER_SEC << " seconds." << endl;

                    //try to detect a plane in the leaf-node n
                    p = PS_PlaneSegment::detectPlane(unusedPoints, param);

                    //cout << "minSol: " << (clock() - test)/(double)CLOCKS_PER_SEC << " seconds." << endl;

                    //if a plane was found in the given leaf node
                    if(p->getIsValid()){

                        //fit the plane by sample
                        p->fitBySample(param.fitSampleSize);

                        if(p->getIsValid()){

                            //start region growing by considering the neighbour nodes
                            this->considerNeighbourNodes(n, param, p, unmergedPoints);

                            //cout << "test considered: " << (clock() - test)/(double)CLOCKS_PER_SEC << " seconds." << endl;

                            resetUnmerged = true;

                            //qDebug() << "point count: " << p->getPointCount();

                            PS_PlaneSegment::sortOut(p, param, 3);

                            //if the plane is valid and contains enough points
                            if(p->getIsValid() && p->getPointCount() >= param.planeParams.minPoints){



                                //finally fit the plane to be able to merge planes later
                                p->fitBySample(param.fitSampleSize * 10);

                                if(p->getIsValid()){

                                    //sort out points that do not satisfy the distance criterion
                                    //PS_PlaneSegment::sortOut(p, param);

                                    //if there are still enough points after sorting out
                                    //if(p->getPointCount() >= param.planeParams.minPoints){
                                        planeValid = true;
                                    //}

                                }

                            }

                        }

                    }

                    //cout << "test fertig: " << (clock() - test)/(double)CLOCKS_PER_SEC << " seconds." << endl;

                    //delete plane if it is not valid
                    if(!planeValid){
                        delete p;
                        p = NULL;
                    }

                }

                /*
                 * detect sphere if requested by user
                 */
                if(param.sphereParams.detectSpheres){

                    if(resetUnmerged){

                        //reset all nodes to not have been considered in merge
                        for(int j = 0; j < PS_PointCloud::mergedNodes.size(); j++){
                            PS_PointCloud::mergedNodes.at(j)->setWasConsideredInMerge(false);
                        }
                        PS_PointCloud::mergedNodes.clear();

                        //get the current node to be considered and set it as considered in merge
                        n->setWasConsideredInMerge(true);

                        //add node to list of merged nodes
                        PS_PointCloud::mergedNodes.append(n);

                        resetUnmerged = false;
                    }

                    bool sphereValid = false;

                    //try to detect a sphere in the leaf-node n
                    s = PS_SphereSegment::detectSphere(unusedPoints, param);

                    //print sphere
                    /*QFile fileS( QString("D:/UNI/Master/Semester_04/Masterarbeit/pointcloudSegmentation/result/minSphere%1.pts").arg(i) );
                    if ( fileS.open(QIODevice::ReadWrite | QIODevice::Truncate) ){

                        float r = 0.0;
                        float g = 0.0;
                        float b = 0.0;
                        QTextStream streama( &fileS );

                        std::random_device rd;
                        int seed = rd();
                        std::default_random_engine generator( seed );
                        uniform_int_distribution<int> distribution(0, 255);

                        for(int k = 0; k < s->getPoints().size(); k++){
                            r = distribution(generator);
                            g = distribution(generator);
                            b = distribution(generator);
                            streama << QString::number(s->getPoints().at(k)->xyz[0])
                                    << " " << QString::number(s->getPoints().at(k)->xyz[1])
                                    << " " << QString::number(s->getPoints().at(k)->xyz[2])
                                    << " " << r << " " << g << " " << b
                                    << endl;
                        }
                        fileS.close();

                    }*/

                    //if a sphere was found in the given leaf node
                    if(s->getIsValid() && s->getRadius() <= param.sphereParams.maxRadius
                            && s->getRadius() >= param.sphereParams.minRadius){

                        //fit the sphere by sample
                        s->fitBySample(param.fitSampleSize);

                        if(s->getIsValid()){

                            //start region growing by considering the neighbour nodes
                            this->considerNeighbourNodes(n, param, s, unmergedPoints);

                            resetUnmerged = true;

                            PS_SphereSegment::sortOut(s, param, 3);

                            //if the sphere is valid and contains enough points
                            if(s->getIsValid() && s->getPoints().size() >= param.sphereParams.minPoints
                                    && s->getRadius() <= param.sphereParams.maxRadius
                                    && s->getRadius() >= param.sphereParams.minRadius){

                                //finally fit the sphere to be able to merge spheres later
                                s->fitBySample(param.fitSampleSize * 10);

                                if(s->getIsValid() && s->getRadius() <= param.sphereParams.maxRadius
                                        && s->getRadius() >= param.sphereParams.minRadius){

                                    //sort out points that do not satisfy the distance criterion
                                    //PS_SphereSegment::sortOut(s, param);

                                    //cout << "test considered 3: " << (clock() - test)/(double)CLOCKS_PER_SEC << " seconds." << endl;


                                    //if there are still enough points after sorting out
                                    //if(s->getPoints().size() >= param.sphereParams.minPoints){
                                        sphereValid = true;
                                    //}

                                }

                            }

                        }

                    }

                    //delete sphere if it is not valid
                    if(!sphereValid){
                        delete s;
                        s = NULL;
                    }

                }

                /*
                 * detect cylinder if requested by user
                 */
                if(param.cylinderParams.detectCylinders){

                    if(resetUnmerged){

                        //reset all nodes to not have been considered in merge
                        for(int j = 0; j < PS_PointCloud::mergedNodes.size(); j++){
                            PS_PointCloud::mergedNodes.at(j)->setWasConsideredInMerge(false);
                        }
                        PS_PointCloud::mergedNodes.clear();

                        //get the current node to be considered and set it as considered in merge
                        n->setWasConsideredInMerge(true);

                        //add node to list of merged nodes
                        PS_PointCloud::mergedNodes.append(n);

                        resetUnmerged = false;
                    }

                    bool cylinderValid = false;

                    //try to detect a cylinder in the leaf-node n
                    c = PS_CylinderSegment::detectCylinder(unusedPoints, param);

                    //if a cylinder was found in the given leaf node
                    if(c->getIsValid() && c->getRadius() <= param.cylinderParams.maxRadius
                            && c->getRadius() >= param.cylinderParams.minRadius){

                        //fit the cylinder by sample
                        c->fitBySample(param.fitSampleSize);

                        if(c->getIsValid()){

                            //start region growing by considering the neighbour nodes
                            this->considerNeighbourNodes(n, param, c, unmergedPoints);

                            PS_CylinderSegment::sortOut(c, param, 3);

                            //if the cylinder is valid and contains enough points
                            if(c->getIsValid() && c->getPoints().size() >= param.cylinderParams.minPoints
                                    && c->getRadius() <= param.cylinderParams.maxRadius
                                    && c->getRadius() >= param.cylinderParams.minRadius){

                                //finally fit the cylinder to be able to merge cylinders later
                                c->fitBySample(param.fitSampleSize * 10);

                                if(c->getIsValid() && c->getRadius() <= param.cylinderParams.maxRadius
                                        && c->getRadius() >= param.cylinderParams.minRadius){

                                    //sort out points that do not satisfy the distance criterion
                                    //PS_CylinderSegment::sortOut(c, param);

                                    //if there are still enough points after sorting out
                                    //if(c->getPoints().size() >= param.cylinderParams.minPoints){
                                        cylinderValid = true;
                                    //}

                                }

                            }

                        }

                    }

                    //delete cylinder if it is not valid
                    if(!cylinderValid){
                        delete c;
                        c = NULL;
                    }

                }

                this->acceptShapeCandidates(p, s, c, numUsedPoints, param);

                //reset all nodes to not have been considered in merge
                for(int j = 0; j < PS_PointCloud::mergedNodes.size(); j++){
                    PS_PointCloud::mergedNodes.at(j)->setWasConsideredInMerge(false);
                }
                PS_PointCloud::mergedNodes.clear();

                emit this->updateStatus(QString("%1 of %2 leaf nodes considered").arg(i).arg(this->myOctree->getLeafs()->size()), 1 + 59 * (i / this->myOctree->getLeafs()->size()));

                //cout << "test continue: " << (clock() - test)/(double)CLOCKS_PER_SEC << " seconds." << endl;

            }else{
                //continue with the next node when the current node does not contain more than 10 points
                continue;
            }

        }

        emit this->updateStatus("All leafs considered, starting with review...", 60);

        //fileOctree.close();

        cout << "Ende Geometrieerkennung: " << (clock() - c1)/(double)CLOCKS_PER_SEC << " seconds." << endl;

        //review used nodes for all detected shapes and probably add further points which were overlooked
        PS_PlaneSegment::reviewNodes(this->detectedPlanes, param);
        PS_SphereSegment::reviewNodes(this->detectedSpheres, param);
        PS_CylinderSegment::reviewNodes(this->detectedCylinders, param);

        emit this->updateStatus("Review completed, starting with merge...", 67);

        qDebug() << "spheres " << this->detectedSpheres.size();

        cout << "Ende Review: " << (clock() - c1)/(double)CLOCKS_PER_SEC << " seconds." << endl;

        //merge planes, spheres and cylinders which were detected as 2 different shapes, but are in fact the same
        QList<PS_SphereSegment *> mergedSpheres;
        PS_SphereSegment::mergeSpheres(this->detectedSpheres, mergedSpheres, param);
        this->detectedSpheres = mergedSpheres;

        qDebug() << "vor " << this->detectedCylinders.size();
        QList<PS_CylinderSegment *> mergedCylinders;
        PS_CylinderSegment::mergeCylinders(this->detectedCylinders, mergedCylinders, param);
        this->detectedCylinders = mergedCylinders;
        qDebug() << "nach " << this->detectedCylinders.size();

        QList<PS_PlaneSegment *> mergedPlanes;
        PS_PlaneSegment::mergePlanes(this->detectedPlanes, mergedPlanes, param);
        this->detectedPlanes = mergedPlanes;

        emit this->updateStatus("Merge completed, starting to verify...", 80);

        cout << "Ende Merge: " << (clock() - c1)/(double)CLOCKS_PER_SEC << " seconds." << endl;

        qDebug() << "spheres vor verify " << this->detectedSpheres.size();

        //verify spheres and cylinders (sort out ones whose points almost lie in a plane
        QList<PS_SphereSegment *> verifiedSpheres;
        PS_SphereSegment::verifySpheres(this->detectedSpheres, verifiedSpheres, param);
        this->detectedSpheres = verifiedSpheres;

        QList<PS_CylinderSegment *> verifiedCylinders;
        PS_CylinderSegment::verifyCylinders(this->detectedCylinders, verifiedCylinders, param);
        this->detectedCylinders = verifiedCylinders;

        emit this->updateStatus("Verify completed, starting with final sort out and fit...", 90);

        qDebug() << "spheres nach verify " << this->detectedSpheres.size();

        cout << "Ende Verify: " << (clock() - c1)/(double)CLOCKS_PER_SEC << " seconds." << endl;

        //final sort out
        QList<PS_SphereSegment *> finalSpheres;
        foreach(PS_SphereSegment *s, this->detectedSpheres){
            s->fitBySample(param.fitSampleSize * 10);
            foreach(PS_Point_PC *poi, s->getPoints()){
                poi->isUsed = false;
            }
            PS_SphereSegment::sortOut(s, param, 3);
            if(s->getPointCount() >= param.sphereParams.minPoints){
                finalSpheres.append(s);
            }
        }
        this->detectedSpheres = finalSpheres;

        QList<PS_PlaneSegment *> finalPlanes;
        foreach(PS_PlaneSegment *p, this->detectedPlanes){
            //p->fitBySample(param.fitSampleSize * 10);
            p->fit();
            foreach(PS_Point_PC *poi, p->getPoints()){
                poi->isUsed = false;
            }
            PS_PlaneSegment::sortOut(p, param, 3);
            if(p->getPointCount() >= param.planeParams.minPoints){
                finalPlanes.append(p);
            }
        }
        this->detectedPlanes = finalPlanes;

        QList<PS_CylinderSegment *> finalCylinders;
        foreach(PS_CylinderSegment *c, this->detectedCylinders){
            c->fitBySample(param.fitSampleSize * 10);
            foreach(PS_Point_PC *poi, c->getPoints()){
                poi->isUsed = false;
            }
            PS_CylinderSegment::sortOut(c, param, 3);
            if(c->getPointCount() >= param.cylinderParams.minPoints){
                finalCylinders.append(c);
            }
        }
        this->detectedCylinders = finalCylinders;

        cout << "Ende final sort out: " << (clock() - c1)/(double)CLOCKS_PER_SEC << " seconds." << endl;

        //finally fit the detected shapes using all points that are associated to a shape
        if(param.finalFit){
            foreach(PS_SphereSegment *s, this->detectedSpheres){
                s->fit();
            }
            for(int i = 0; i < this->detectedPlanes.size(); ++i){
                this->detectedPlanes.at(i)->fit();
            }
            for(int i = 0; i < this->detectedCylinders.size(); ++i){
                this->detectedCylinders.at(i)->fit();
            }
        }else{
            for(int i = 0; i < this->detectedSpheres.size(); ++i){
                this->detectedSpheres.at(i)->fitBySample(param.fitSampleSize);
            }
            for(int i = 0; i < this->detectedPlanes.size(); ++i){
                this->detectedPlanes.at(i)->fitBySample(param.fitSampleSize);
            }
            for(int i = 0; i < this->detectedCylinders.size(); ++i){
                this->detectedCylinders.at(i)->fitBySample(param.fitSampleSize);
            }
        }

        QString path = qApp->applicationDirPath().append("/result");
        qDebug() << path;
        QDir dir(path);
        if (!dir.exists()) {
            dir.mkpath(path);
        }

        emit this->updateStatus(QString("Segmentation done, writing output files to %1").arg(path), 99);

        double processingTime = (clock() - c1)/(double)CLOCKS_PER_SEC;

        cout << "Ende All: " << (clock() - c1)/(double)CLOCKS_PER_SEC << " seconds." << endl;

        //print result overview to console
        qDebug() << this->detectedPlanes.size() << " planes found";
        qDebug() << this->detectedSpheres.size() << " spheres found";
        qDebug() << this->detectedCylinders.size() << " cylinders found";

        this->printOutput(path, processingTime, param);

        //TODO in plugin show result modal dialog

        return true;

    }
    return false;
}

const QList<PS_PlaneSegment *> &PS_PointCloud::getDetectedPlanes(){
    return this->detectedPlanes;
}

const QList<PS_SphereSegment *> &PS_PointCloud::getDetectedSpheres(){
    return this->detectedSpheres;
}

const QList<PS_CylinderSegment *> &PS_PointCloud::getDetectedCylinders(){
    return this->detectedCylinders;
}

/*!
 * \brief PS_PointCloud::considerNeighbourNodes
 * Check the 6 neighbour nodes of a node if the points belong to the same plane
 * \param n
 * \param param
 * \param p
 * \param unmergedPoints
 */
void PS_PointCloud::considerNeighbourNodes(PS_Node *n, const PS_InputParameter &param, PS_PlaneSegment *p, QList<PS_Point_PC *> &unmergedPoints){

    if(p->getIsValid()){

        //add node n to the list of used nodes of the plane
        p->addUsedNode(n);

        QList<PS_Node *> nodesToGrow;

        //top node
        if(n->top != NULL && !n->top->getWasConsideredInMerge()){
            if(this->mergeNode(n->top, param, p, unmergedPoints)){
                nodesToGrow.append(n->top);
            }
        }

        //bottom node
        if(n->bottom != NULL && !n->bottom->getWasConsideredInMerge()){
            if(this->mergeNode(n->bottom, param, p, unmergedPoints)){
                nodesToGrow.append(n->bottom);
            }
        }

        //left node
        if(n->left != NULL && !n->left->getWasConsideredInMerge()){
            if(this->mergeNode(n->left, param, p, unmergedPoints)){
                nodesToGrow.append(n->left);
            }
        }

        //right node
        if(n->right != NULL && !n->right->getWasConsideredInMerge()){
            if(this->mergeNode(n->right, param, p, unmergedPoints)){
                nodesToGrow.append(n->right);
            }
        }

        //front node
        if(n->front != NULL && !n->front->getWasConsideredInMerge()){
            if(this->mergeNode(n->front, param, p, unmergedPoints)){
                nodesToGrow.append(n->front);
            }
        }

        //back node
        if(n->back != NULL && !n->back->getWasConsideredInMerge()){
            if(this->mergeNode(n->back, param, p, unmergedPoints)){
                nodesToGrow.append(n->back);
            }
        }

        //if there are nodes to be extended
        if(nodesToGrow.size() > 0){

            //refit the plane before considering further neighbours
            p->fitBySample(param.fitSampleSize);

            if(!p->getIsValid()){
                return;
            }

            //sort out points that do not satisfy the distance criterion
            //PS_PlaneSegment::sortOut(p, param, 1);

            if(p->getPointCount() < 4){
                p->setIsValid(false);
                return;
            }

            //consider the neighbour's neighbours
            foreach(PS_Node *neighbour, nodesToGrow){
                this->considerNeighbourNodes(neighbour, param, p, unmergedPoints);
            }

        }

    }

}

/*!
 * \brief PS_PointCloud::considerNeighbourNodes
 * Check the 6 neighbour nodes of a node if the points belong to the same sphere
 * \param n
 * \param param
 * \param s
 * \param unmergedPoints
 */
void PS_PointCloud::considerNeighbourNodes(PS_Node *n, const PS_InputParameter &param, PS_SphereSegment *s, QList<PS_Point_PC *> &unmergedPoints){

    if(s->getIsValid()){

        //add node n to the list of used nodes of the sphere
        s->addUsedNode(n);

        QList<PS_Node *> nodesToGrow;

        //top node
        if(n->top != NULL && !n->top->getWasConsideredInMerge()){
            if(this->mergeNode(n->top, param, s, unmergedPoints)){
                nodesToGrow.append(n->top);
            }
        }

        //bottom node
        if(n->bottom != NULL && !n->bottom->getWasConsideredInMerge()){
            if(this->mergeNode(n->bottom, param, s, unmergedPoints)){
                nodesToGrow.append(n->bottom);
            }
        }

        //left node
        if(n->left != NULL && !n->left->getWasConsideredInMerge()){
            if(this->mergeNode(n->left, param, s, unmergedPoints)){
                nodesToGrow.append(n->left);
            }
        }

        //right node
        if(n->right != NULL && !n->right->getWasConsideredInMerge()){
            if(this->mergeNode(n->right, param, s, unmergedPoints)){
                nodesToGrow.append(n->right);
            }
        }

        //front node
        if(n->front != NULL && !n->front->getWasConsideredInMerge()){
            if(this->mergeNode(n->front, param, s, unmergedPoints)){
                nodesToGrow.append(n->front);
            }
        }

        //back node
        if(n->back != NULL && !n->back->getWasConsideredInMerge()){
            if(this->mergeNode(n->back, param, s, unmergedPoints)){
                nodesToGrow.append(n->back);
            }
        }

        //if there are nodes to be extended
        if(nodesToGrow.size() > 0){

            //refit the sphere before considering further neighbours
            s->fitBySample(param.fitSampleSize);

            if(!s->getIsValid()){
                return;
            }

            //sort out points that do not satisfy the distance criterion
            //PS_SphereSegment::sortOut(s, param, 3);

            if(s->getPoints().size() < 5){
                s->setIsValid(false);
                return;
            }

            //consider the neighbour's neighbours
            foreach(PS_Node *neighbour, nodesToGrow){
                this->considerNeighbourNodes(neighbour, param, s, unmergedPoints);
            }

        }

    }
}

/*!
 * \brief PS_PointCloud::considerNeighbourNodes
 * Check the 6 neighbour nodes of a node if the points belong to the same cylinder
 * \param n
 * \param param
 * \param c
 * \param unmergedPoints
 */
void PS_PointCloud::considerNeighbourNodes(PS_Node *n, const PS_InputParameter &param, PS_CylinderSegment *c, QList<PS_Point_PC *> &unmergedPoints){

    if(c->getIsValid()){

        //add node n to the list of used nodes of the cylinder
        c->addUsedNode(n);

        QList<PS_Node *> nodesToGrow;

        //top node
        if(n->top != NULL && !n->top->getWasConsideredInMerge()){
            if(this->mergeNode(n->top, param, c, unmergedPoints)){
                nodesToGrow.append(n->top);
            }
        }

        //bottom node
        if(n->bottom != NULL && !n->bottom->getWasConsideredInMerge()){
            if(this->mergeNode(n->bottom, param, c, unmergedPoints)){
                nodesToGrow.append(n->bottom);
            }
        }

        //left node
        if(n->left != NULL && !n->left->getWasConsideredInMerge()){
            if(this->mergeNode(n->left, param, c, unmergedPoints)){
                nodesToGrow.append(n->left);
            }
        }

        //right node
        if(n->right != NULL && !n->right->getWasConsideredInMerge()){
            if(this->mergeNode(n->right, param, c, unmergedPoints)){
                nodesToGrow.append(n->right);
            }
        }

        //front node
        if(n->front != NULL && !n->front->getWasConsideredInMerge()){
            if(this->mergeNode(n->front, param, c, unmergedPoints)){
                nodesToGrow.append(n->front);
            }
        }

        //back node
        if(n->back != NULL && !n->back->getWasConsideredInMerge()){
            if(this->mergeNode(n->back, param, c, unmergedPoints)){
                nodesToGrow.append(n->back);
            }
        }

        //if there are nodes to be extended
        if(nodesToGrow.size() > 0){

            //refit the cylinder before considering further neighbours
            c->fitBySample(param.fitSampleSize);

            if(!c->getIsValid()){
                return;
            }

            //sort out points that do not satisfy the distance criterion
            //PS_CylinderSegment::sortOut(c, param, 1);

            if(c->getPoints().size() < 6){
                c->setIsValid(false);
                return;
            }

            //consider the neighbour's neighbours
            foreach(PS_Node *neighbour, nodesToGrow){
                this->considerNeighbourNodes(neighbour, param, c, unmergedPoints);
            }

        }

    }

}

/*!
 * \brief PS_PointCloud::mergeNode
 * Try to add the points of node n to the plane p
 * \param n
 * \param param
 * \param p
 * \param unmergedPoints
 * \return
 */
bool PS_PointCloud::mergeNode(PS_Node *n, const PS_InputParameter &param, PS_PlaneSegment *p, QList<PS_Point_PC *> &unmergedPoints){

    //number of points the plane currently contains
    unsigned int numPoints = p->getPoints().size();

    //save the current state of the plane to be able to fall back later
    p->saveCurrentState();

    //try to add further points of node n
    n->getUnmergedPoints(unmergedPoints);
    PS_PlaneSegment::checkPointsInPlane(p, unmergedPoints, param, 3);

    //set node as considered in merge and add it to list of merged nodes
    n->setWasConsideredInMerge(true);
    PS_PointCloud::mergedNodes.append(n);

    //if at minimum 4 points have been found in the node consider its neighbours
    if(p->getPointCount() - numPoints >= 4){

        return true;

    }else{

        p->fallBack();
        return false;

    }

}

/*!
 * \brief PS_PointCloud::mergeNode
 * Try to add the points of node n to the sphere s
 * \param n
 * \param param
 * \param s
 * \param unmergedPoints
 * \return
 */
bool PS_PointCloud::mergeNode(PS_Node *n, const PS_InputParameter &param, PS_SphereSegment *s, QList<PS_Point_PC *> &unmergedPoints){

    //number of points the sphere currently contains
    unsigned int numPoints = s->getPoints().size();

    //save the current state of the sphere to be able to fall back later
    s->saveCurrentState();

    //try to add further points of node n
    n->getUnmergedPoints(unmergedPoints);
    PS_SphereSegment::checkPointsInSphere(s, unmergedPoints, param, 3);

    //set node as considered in merge and add it to list of merged nodes
    n->setWasConsideredInMerge(true);
    mergedNodes.append(n);

    //if at minimum 5 points have been found in the node consider its neighbours
    if(s->getPointCount() - numPoints >= 5){

        return true;

    }else{

        s->fallBack();
        return false;

    }

}

/*!
 * \brief PS_PointCloud::mergeNode
 * Try to add the points of node n to the cylinder c
 * \param n
 * \param param
 * \param c
 * \param unmergedPoints
 * \return
 */
bool PS_PointCloud::mergeNode(PS_Node *n, const PS_InputParameter &param, PS_CylinderSegment *c, QList<PS_Point_PC *> &unmergedPoints){

    //number of points the cylinder currently contains
    unsigned int numPoints = c->getPoints().size();

    //save the current state of the cylinder to be able to fall back later
    c->saveCurrentState();

    //try to add further points of node n
    n->getUnmergedPoints(unmergedPoints);
    PS_CylinderSegment::checkPointsInCylinder(c, unmergedPoints, param, 3);

    //set node as considered in merge and add it to list of merged nodes
    n->setWasConsideredInMerge(true);
    mergedNodes.append(n);

    //if at minimum 6 points have been found in the node consider its neighbours
    if(c->getPointCount() - numPoints >= 6){

        return true;

    }else{

        c->fallBack();
        return false;

    }

}

/*!
 * \brief PS_PointCloud::acceptShapeCandidates
 * Accept or reject shape candidates that were detected in one leaf voxel
 * \param p
 * \param s
 * \param c
 */
void PS_PointCloud::acceptShapeCandidates(PS_PlaneSegment *p, PS_SphereSegment *s, PS_CylinderSegment *c, unsigned long &numUsedPoints, const PS_InputParameter &param){

    /*
     * compare the sigma-value of the detected plane, sphere and cylinder to decide which shapes to accept
     */
    if( p != NULL && (s == NULL || s->getSigma() > p->getSigma()) && (c == NULL || c->getSigma() > 3.0*p->getSigma() ) ){

        //accept plane and set its points as used
        this->detectedPlanes.append(p);
        foreach(PS_Point_PC *poi, p->getPoints()){
            poi->isUsed = true;
        }
        numUsedPoints += p->getPoints().size();

        //refit sphere with still unused points or delete
        if(s != NULL){
            s->removeUsedPoints();
            if(s->getPointCount() >= param.sphereParams.minPoints){
                s->fitBySample(param.fitSampleSize);
            }else{
                delete s;
                s = NULL;
            }
        }

        //refit cylinder with still unused points or delete
        if(c != NULL){
            c->removeUsedPoints();
            if(c->getPointCount() >= param.cylinderParams.minPoints){
                c->fitBySample(param.fitSampleSize);
            }else{
                //qDebug() << "cylinder rejected because plane";
                delete c;
                c = NULL;
            }
        }

        if(s != NULL && (c == NULL || c->getSigma() > s->getSigma())){

            //accept sphere and set its points as used
            this->detectedSpheres.append(s);
            foreach(PS_Point_PC *poi, s->getPoints()){
                poi->isUsed = true;
            }
            numUsedPoints += s->getPoints().size();

            //refit cylinder with still unused points or delete
            if(c != NULL){
                c->removeUsedPoints();
                if(c->getPointCount() >= param.cylinderParams.minPoints){

                    c->fitBySample(param.fitSampleSize);

                    //accept cylinder and set its points as used
                    this->detectedCylinders.append(c);
                    foreach(PS_Point_PC *poi, c->getPoints()){
                        poi->isUsed = true;
                    }
                    numUsedPoints += c->getPoints().size();

                }else{
                    delete c;
                    c = NULL;
                }
            }

        }else if(c != NULL && (s == NULL || s->getSigma() >= c->getSigma())){

            //accept cylinder and set its points as used
            this->detectedCylinders.append(c);
            foreach(PS_Point_PC *poi, c->getPoints()){
                poi->isUsed = true;
            }
            numUsedPoints += c->getPoints().size();

            //refit sphere with still unused points or delete
            if(s != NULL){
                s->removeUsedPoints();
                if(s->getPointCount() >= param.sphereParams.minPoints){

                    s->fitBySample(param.fitSampleSize);

                    //accept sphere and set its points as used
                    this->detectedSpheres.append(s);
                    foreach(PS_Point_PC *poi, s->getPoints()){
                        poi->isUsed = true;
                    }
                    numUsedPoints += s->getPoints().size();

                }else{
                    delete s;
                    s = NULL;
                }
            }

        }

    }else if( s != NULL && (p == NULL || p->getSigma() >= s->getSigma()) && (c == NULL || c->getSigma() > s->getSigma() ) ){

        //accept sphere and set its points as used
        this->detectedSpheres.append(s);
        foreach(PS_Point_PC *poi, s->getPoints()){
            poi->isUsed = true;
        }
        numUsedPoints += s->getPoints().size();

        //refit plane with still unused points or delete
        if(p != NULL){
            p->removeUsedPoints();
            if(p->getPointCount() >= param.planeParams.minPoints){
                p->fitBySample(param.fitSampleSize);
            }else{
                delete p;
                p = NULL;
            }
        }

        //refit cylinder with still unused points or delete
        if(c != NULL){
            c->removeUsedPoints();
            if(c->getPointCount() >= param.cylinderParams.minPoints){
                c->fitBySample(param.fitSampleSize);
            }else{
                delete c;
                c = NULL;
            }
        }

        if(p != NULL && (c == NULL || c->getSigma() > p->getSigma())){

            //accept plane and set its points as used
            this->detectedPlanes.append(p);
            foreach(PS_Point_PC *poi, p->getPoints()){
                poi->isUsed = true;
            }
            numUsedPoints += p->getPoints().size();

            //refit cylinder with still unused points or delete
            if(c != NULL){
                c->removeUsedPoints();
                if(c->getPointCount() >= param.cylinderParams.minPoints){

                    c->fitBySample(param.fitSampleSize);

                    //accept cylinder and set its points as used
                    this->detectedCylinders.append(c);
                    foreach(PS_Point_PC *poi, c->getPoints()){
                        poi->isUsed = true;
                    }
                    numUsedPoints += c->getPoints().size();

                }else{
                    delete c;
                    c = NULL;
                }
            }

        }else if(c != NULL && (p == NULL || p->getSigma() >= c->getSigma())){

            //accept cylinder and set its points as used
            this->detectedCylinders.append(c);
            foreach(PS_Point_PC *poi, c->getPoints()){
                poi->isUsed = true;
            }
            numUsedPoints += c->getPoints().size();

            //refit plane with still unused points or delete
            if(p != NULL){
                p->removeUsedPoints();
                if(p->getPointCount() >= param.planeParams.minPoints){

                    p->fitBySample(param.fitSampleSize);

                    //accept plane and set its points as used
                    this->detectedPlanes.append(p);
                    foreach(PS_Point_PC *poi, p->getPoints()){
                        poi->isUsed = true;
                    }
                    numUsedPoints += p->getPoints().size();

                }else{
                    delete p;
                    p = NULL;
                }
            }

        }

    }else if( c != NULL && (p == NULL || p->getSigma() >= c->getSigma()) && (s == NULL || s->getSigma() >= c->getSigma() ) ){

        //accept cylinder and set its points as used
        this->detectedCylinders.append(c);
        foreach(PS_Point_PC *poi, c->getPoints()){
            poi->isUsed = true;
        }
        numUsedPoints += c->getPoints().size();

        //refit plane with still unused points or delete
        if(p != NULL){
            p->removeUsedPoints();
            if(p->getPointCount() >= param.planeParams.minPoints){
                p->fitBySample(param.fitSampleSize);
            }else{
                delete p;
                p = NULL;
            }
        }

        //refit sphere with still unused points or delete
        if(s != NULL){
            s->removeUsedPoints();
            if(s->getPointCount() >= param.sphereParams.minPoints){
                s->fitBySample(param.fitSampleSize);
            }else{
                delete s;
                s = NULL;
            }
        }

        if(p != NULL && (s == NULL || s->getSigma() > p->getSigma())){

            //accept plane and set its points as used
            this->detectedPlanes.append(p);
            foreach(PS_Point_PC *poi, p->getPoints()){
                poi->isUsed = true;
            }
            numUsedPoints += p->getPoints().size();

            //refit sphere with still unused points or delete
            if(s != NULL){
                s->removeUsedPoints();
                if(s->getPointCount() >= param.sphereParams.minPoints){

                    s->fitBySample(param.fitSampleSize);

                    //accept sphere and set its points as used
                    this->detectedSpheres.append(s);
                    foreach(PS_Point_PC *poi, s->getPoints()){
                        poi->isUsed = true;
                    }
                    numUsedPoints += s->getPoints().size();

                }else{
                    delete s;
                    s = NULL;
                }
            }

        }else if(s != NULL && (p == NULL || p->getSigma() >= s->getSigma())){

            //accept sphere and set its points as used
            this->detectedSpheres.append(s);
            foreach(PS_Point_PC *poi, s->getPoints()){
                poi->isUsed = true;
            }
            numUsedPoints += s->getPoints().size();

            //refit plane with still unused points or delete
            if(p != NULL){
                p->removeUsedPoints();
                if(p->getPointCount() >= param.planeParams.minPoints){

                    p->fitBySample(param.fitSampleSize);

                    //accept plane and set its points as used
                    this->detectedPlanes.append(p);
                    foreach(PS_Point_PC *poi, p->getPoints()){
                        poi->isUsed = true;
                    }
                    numUsedPoints += p->getPoints().size();

                }else{
                    delete p;
                    p = NULL;
                }
            }

        }

    }

}

/*!
 * \brief PS_PointCloud::printOutput
 * Print output files with results of the segmentation
 * \param filePath
 */
void PS_PointCloud::printOutput(QString filePath, double processingTime, PS_InputParameter param){

    //print x3d sphapes
    /*for(unsigned int i = 0; i < this->detectedSpheres.size(); i++){
        this->detectedSpheres.at(i)->writeToX3D(
                    QString("%1/sphere_%2.x3d").arg(filePath).arg(QString::number(i+1)));
    }
    for(unsigned int i = 0; i < this->detectedSpheres.size(); i++){
        this->detectedSpheres.at(i)->writeToX3D(
                    QString("%1/sphere_%2.x3d").arg(filePath).arg(QString::number(i+1)));
    }
    for(unsigned int i = 0; i < this->detectedCylinders.size(); i++){
        this->detectedCylinders.at(i)->writeToX3D(
                    QString("%1/cylinder_%2.x3d").arg(filePath).arg(QString::number(i+1)));
    }*/

    //write pts files
    for(unsigned int i = 0; i < this->detectedPlanes.size(); i++){
        this->detectedPlanes.at(i)->writeToPts(
                    QString("%1/plane_%2.pts").arg(filePath).arg(QString::number(i+1)));
    }
    for(unsigned int i = 0; i < this->detectedSpheres.size(); i++){
        this->detectedSpheres.at(i)->writeToPts(
                    QString("%1/sphere_%2.pts").arg(filePath).arg(QString::number(i+1)));
    }
    for(unsigned int i = 0; i < this->detectedCylinders.size(); i++){
        this->detectedCylinders.at(i)->writeToPts(
                    QString("%1/cylinder_%2.pts").arg(filePath).arg(QString::number(i+1)));
    }

    //write result protocol
    QFile file( QString("%1/segmentationProtocol.txt").arg(filePath) );
    if ( file.open(QIODevice::ReadWrite | QIODevice::Truncate) ){

        QTextStream stream( &file );

        //general information
        stream << "SEGMENTATION PROTOCOL" << endl;
        stream << "######################" << endl;
        stream << "" << endl;
        stream << "General information" << endl;
        stream << "----------------------" << endl;
        stream << "Created at:" << "\t" << QDateTime::currentDateTime().toString() << endl;
        stream << "Author:" << "\t" << "Benedikt Rauls" << endl;
        stream << "point cloud:" << "\t" << this->filePath << QString(" (%1 points)").arg(this->myPoints->size()) << endl;
        stream << "Processing time:" << "\t" << QString::number(processingTime, 'f', 4) << " sec." << endl;
        stream << "Number of detected Shapes:" << "\t" << QString::number(this->detectedPlanes.size()
                                                                          + this->detectedSpheres.size()
                                                                          + this->detectedCylinders.size())
                  << QString(" (planes: %1, spheres: %2, cylinders: %3)").arg(this->detectedPlanes.size())
                     .arg(this->detectedSpheres.size()).arg(this->detectedCylinders.size()) << endl;
        stream << "" << endl;
        stream << "Input parameters" << endl;
        stream << "----------------------" << endl;
        stream << "leafSize:" << "\t" << QString::number(param.leafSize) << endl;
        stream << "outlierPercentage:" << "\t" << QString::number(param.outlierPercentage, 'f', 2) << endl;
        stream << "fitSampleSize:" << "\t" << QString::number(param.fitSampleSize) << endl;
        stream << "finalFit:" << "\t" << QString(param.finalFit?"true":"false") << endl;
        stream << "detectPlanes:" << "\t" << QString(param.planeParams.detectPlanes?"true":"false") << endl;
        stream << "detectSpheres:" << "\t" << QString(param.sphereParams.detectSpheres?"true":"false") << endl;
        stream << "detectCylinders:" << "\t" << QString(param.cylinderParams.detectCylinders?"true":"false") << endl;
        if(param.planeParams.detectPlanes){
            stream << "maxDistance (plane):" << "\t" << QString::number(param.planeParams.maxDistance, 'f', 4) << " m" << endl;
            stream << "minPoints (plane):" << "\t" << QString::number(param.planeParams.minPoints) << endl;
        }
        if(param.sphereParams.detectSpheres){
            stream << "maxDistance (sphere):" << "\t" << QString::number(param.sphereParams.maxDistance, 'f', 4) << " m" << endl;
            stream << "maxRadius (sphere):" << "\t" << QString::number(param.sphereParams.maxRadius, 'f', 4) << " m" << endl;
            stream << "minRadius (sphere):" << "\t" << QString::number(param.sphereParams.minRadius, 'f', 4) << " m" << endl;
            stream << "minPoints (sphere):" << "\t" << QString::number(param.sphereParams.minPoints) << endl;
        }
        if(param.cylinderParams.detectCylinders){
            stream << "maxDistance (cylinder):" << "\t" << QString::number(param.cylinderParams.maxDistance, 'f', 4) << " m" << endl;
            stream << "maxRadius (cylinder):" << "\t" << QString::number(param.cylinderParams.maxRadius, 'f', 4) << " m" << endl;
            stream << "minRadius (cylinder):" << "\t" << QString::number(param.cylinderParams.minRadius, 'f', 4) << " m" << endl;
            stream << "minPoints (cylinders):" << "\t" << QString::number(param.cylinderParams.minPoints) << endl;
        }
        stream << "" << endl;
        stream << "Extracted Planes" << endl;
        stream << "----------------------" << endl;
        stream << "name" << "\t" << "i" << "\t" << "j" << "\t" << "k" << "\t" << "d[m]" << "\t" << "numPoints" << "\t" << "stdev[m]" << endl;
        for(int i = 0; i < this->detectedPlanes.size(); ++i){
            float *ijk = this->detectedPlanes.at(i)->getIJK();
            stream << QString("plane_%1").arg(i+1) << "\t"
                    << QString::number(ijk[0], 'f', 6) << "\t"
                    << QString::number(ijk[1], 'f', 6) << "\t"
                    << QString::number(ijk[2], 'f', 6) << "\t"
                    << QString::number(this->detectedPlanes.at(i)->getDistance(), 'f', 4) << "\t"
                    << QString::number(this->detectedPlanes.at(i)->getPointCount()) << "\t"
                    << QString::number(this->detectedPlanes.at(i)->getSigma(), 'f', 4) << endl;
        }
        stream << "" << endl;
        stream << "Extracted Spheres" << endl;
        stream << "----------------------" << endl;
        stream << "name" << "\t" << "xm[m]" << "\t" << "ym[m]" << "\t" << "zm[m]" << "\t" << "r[m]" << "\t" << "numPoints" << "\t" << "stdev[m]" << endl;
        for(int i = 0; i < this->detectedSpheres.size(); ++i){
            float *xyz = this->detectedSpheres.at(i)->getXYZ();
            stream << QString("sphere_%1").arg(i+1) << "\t"
                    << QString::number(xyz[0], 'f', 4) << "\t"
                    << QString::number(xyz[1], 'f', 4) << "\t"
                    << QString::number(xyz[2], 'f', 4) << "\t"
                    << QString::number(this->detectedSpheres.at(i)->getRadius(), 'f', 4) << "\t"
                    << QString::number(this->detectedSpheres.at(i)->getPointCount()) << "\t"
                    << QString::number(this->detectedSpheres.at(i)->getSigma(), 'f', 4) << endl;
        }
        stream << "" << endl;
        stream << "Extracted Cylinders" << endl;
        stream << "----------------------" << endl;
        stream << "name" << "\t" << "i" << "\t" << "j" << "\t" << "k" << "\t" << "x0[m]" << "\t" << "y0[m]" << "\t" << "z0[m]" << "\t" << "r[m]" << "\t" << "numPoints" << "\t" << "stdev[m]" << endl;
        for(int i = 0; i < this->detectedCylinders.size(); ++i){
            float ijk[3], x0[3];
            this->detectedCylinders.at(i)->getIJK(ijk);
            this->detectedCylinders.at(i)->getX0(x0);
            stream << QString("cylinder_%1").arg(i+1) << "\t"
                    << QString::number(ijk[0], 'f', 6) << "\t"
                    << QString::number(ijk[1], 'f', 6) << "\t"
                    << QString::number(ijk[2], 'f', 6) << "\t"
                    << QString::number(x0[0], 'f', 4) << "\t"
                    << QString::number(x0[1], 'f', 4) << "\t"
                    << QString::number(x0[2], 'f', 4) << "\t"
                    << QString::number(this->detectedCylinders.at(i)->getRadius(), 'f', 4) << "\t"
                    << QString::number(this->detectedCylinders.at(i)->getPointCount()) << "\t"
                    << QString::number(this->detectedCylinders.at(i)->getSigma(), 'f', 4) << endl;
        }

        file.close();

    }

}
