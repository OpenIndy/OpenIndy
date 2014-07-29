#include "ps_pointcloud.h"

#include "ps_planesegment.h"
#include "ps_spheresegment.h"
#include "ps_cylindersegment.h"

QList<PS_Node*> PS_PointCloud::mergedNodes;

PS_PointCloud::PS_PointCloud()
{
    this->myPoints = new vector<Point_PC*>;
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

        QTextStream in(&file);

        while(!in.atEnd()) {

            QString line = in.readLine();

            QStringList fields = line.split(' ');

            if(fields.length() >= 3){

                //create a new point and add it to the point cloud
                Point_PC *myPoint = new Point_PC();
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

        qDebug() << "start detecting shapes";
        c1 = clock();

        //get leaf nodes from Octree (= nodes that are not subdevided into smaller nodes)
        //the nodes are sorted from containing many points to containing less points
        QList<PS_Node *> *leafs = this->myOctree->getLeafs();

        //print all leafs to check octree
        /*QFile fileOctree( "D:/UNI/Master/Semester_04/Masterarbeit/ersteSchritte/testProjekt/result/octree.pts" );
        if ( fileOctree.open(QIODevice::ReadWrite | QIODevice::Truncate) ){

            float r = 0.0;
            float g = 0.0;
            float b = 0.0;
            QTextStream streama( &fileOctree );

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
                            streama << QString::number(leafs->at(i)->points.at(j)->x[0])
                                    << " " << QString::number(leafs->at(i)->points.at(j)->x[1])
                                    << " " << QString::number(leafs->at(i)->points.at(j)->x[2])
                                    << " " << r << " " << g << " " << b
                                    << endl;
                            //leafs->at(i)->points.at(j)->isUsed = true;
                        //}
                    }

                }
            }
            fileOctree.close();

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

        //iterate through all leaf nodes starting with the one with the most points
        for(int i = 0; i < leafs->size(); i++){

            qDebug() << i;

            //reset all nodes to not have been considered in merge
            for(int j = 0; j < PS_PointCloud::mergedNodes.size(); j++){
                PS_PointCloud::mergedNodes.at(j)->setWasConsideredInMerge(false);
            }
            PS_PointCloud::mergedNodes.clear();

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

                /*
                 * detect plane if requested by user
                 */
                if(param.planeParams.detectPlanes){

                    bool planeValid = false;

                    //try to detect a plane in the leaf-node n
                    p = PS_PlaneSegment::detectPlane(n->getUnusedPoints(), param);

                    //if a plane was found in the given leaf node
                    if(p->getIsValid()){

                        //fit the plane by sample
                        p->fitBySample(param.fitSampleSize);

                        if(p->getIsValid()){

                            //start region growing by considering the neighbour nodes
                            this->considerNeighbourNodes(n, param, p);

                            //if the plane is valid and contains enough points
                            if(p->getIsValid() && p->getPoints().size() >= param.planeParams.minPoints){

                                //finally fit the plane to be able to merge planes later
                                p->fitBySample(param.fitSampleSize);

                                if(p->getIsValid()){

                                    //sort out points that do not satisfy the distance criterion
                                    PS_PlaneSegment::sortOut(p, param);

                                    //if there are still enough points after sorting out
                                    if(p->getPoints().size() >= param.planeParams.minPoints){
                                        planeValid = true;
                                    }

                                }

                            }

                        }

                    }

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

                    bool sphereValid = false;

                    //try to detect a sphere in the leaf-node n
                    s = PS_SphereSegment::detectSphere(n->getUnusedPoints(), param);

                    //if a sphere was found in the given leaf node
                    if(s->getIsValid() && s->getRadius() <= param.sphereParams.maxRadius
                            && s->getRadius() >= param.sphereParams.minRadius){

                        //fit the sphere by sample
                        s->fitBySample(param.fitSampleSize);

                        if(s->getIsValid()){

                            //start region growing by considering the neighbour nodes
                            this->considerNeighbourNodes(n, param, s);

                            //if the sphere is valid and contains enough points
                            if(s->getIsValid() && s->getPoints().size() >= param.sphereParams.minPoints
                                    && s->getRadius() <= param.sphereParams.maxRadius
                                    && s->getRadius() >= param.sphereParams.minRadius){

                                //finally fit the sphere to be able to merge spheres later
                                s->fitBySample(param.fitSampleSize);

                                if(s->getIsValid() && s->getRadius() <= param.sphereParams.maxRadius
                                        && s->getRadius() >= param.sphereParams.minRadius){

                                    //sort out points that do not satisfy the distance criterion
                                    PS_SphereSegment::sortOut(s, param);

                                    //if there are still enough points after sorting out
                                    if(s->getPoints().size() >= param.sphereParams.minPoints){
                                        sphereValid = true;
                                    }

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

                    bool cylinderValid = false;

                    //try to detect a cylinder in the leaf-node n
                    c = PS_CylinderSegment::detectCylinder(n->getUnusedPoints(), param);

                    //if a cylinder was found in the given leaf node
                    if(c->getIsValid() && c->getRadius() <= param.cylinderParams.maxRadius
                            && c->getRadius() >= param.cylinderParams.minRadius){

                        //fit the cylinder by sample
                        c->fitBySample(param.fitSampleSize);

                        if(c->getIsValid()){

                            //start region growing by considering the neighbour nodes
                            this->considerNeighbourNodes(n, param, c);

                            //if the cylinder is valid and contains enough points
                            if(c->getIsValid() && c->getPoints().size() >= param.cylinderParams.minPoints
                                    && c->getRadius() <= param.cylinderParams.maxRadius
                                    && c->getRadius() >= param.cylinderParams.minRadius){

                                //finally fit the cylinder to be able to merge cylinders later
                                c->fitBySample(param.fitSampleSize);

                                if(c->getIsValid() && c->getRadius() <= param.cylinderParams.maxRadius
                                        && c->getRadius() >= param.cylinderParams.minRadius){

                                    //sort out points that do not satisfy the distance criterion
                                    PS_CylinderSegment::sortOut(c, param);

                                    //if there are still enough points after sorting out
                                    if(c->getPoints().size() >= param.cylinderParams.minPoints){
                                        cylinderValid = true;
                                    }

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

                /*
                 * compare the sigma-value of the detected plane, sphere and cylinder
                 */
                if( p != NULL && (s == NULL || s->getSigma() > p->getSigma()) && (c == NULL || c->getSigma() > p->getSigma() ) ){

                    this->detectedPlanes.append(p);
                    foreach(Point_PC *poi, p->getPoints()){
                        poi->isUsed = true;
                    }

                }else if( s != NULL && (p == NULL || p->getSigma() > s->getSigma()) && (c == NULL || c->getSigma() > s->getSigma() ) ){

                    this->detectedSpheres.append(s);
                    foreach(Point_PC *poi, s->getPoints()){
                        poi->isUsed = true;
                    }

                }else if( c != NULL && (p == NULL || p->getSigma() > c->getSigma()) && (s == NULL || s->getSigma() > c->getSigma() ) ){

                    this->detectedCylinders.append(c);
                    foreach(Point_PC *poi, c->getPoints()){
                        poi->isUsed = true;
                    }

                }

            }else{
                //continue with the next node when the current node does not contain more than 10 points
                continue;
            }

        }

        cout << "Ende Geometrieerkennung: " << (clock() - c1)/(double)CLOCKS_PER_SEC << " seconds." << endl;

        //merge planes, spheres and cylinders which were detected as 2 different shapes, but are in fact the same
        QList<PS_SphereSegment *> mergedSpheres;
        PS_SphereSegment::mergeSpheres(this->detectedSpheres, mergedSpheres, param);
        this->detectedSpheres = mergedSpheres;

        QList<PS_CylinderSegment *> mergedCylinders;
        PS_CylinderSegment::mergeCylinders(this->detectedCylinders, mergedCylinders, param);
        this->detectedCylinders = mergedCylinders;

        QList<PS_PlaneSegment *> mergedPlanes;
        PS_PlaneSegment::mergePlanes(this->detectedPlanes, mergedPlanes, param);
        this->detectedPlanes = mergedPlanes;

        //verify spheres (sort out spheres whose points almost lie in a plane
        QList<PS_SphereSegment *> verifiedSpheres;
        PS_SphereSegment::verifySpheres(this->detectedSpheres, verifiedSpheres, param);
        this->detectedSpheres = verifiedSpheres;

        cout << "Ende Merging: " << (clock() - c1)/(double)CLOCKS_PER_SEC << " seconds." << endl;

        //print result overview to console
        qDebug() << this->detectedPlanes.size() << " planes found";
        qDebug() << this->detectedSpheres.size() << " spheres found";
        qDebug() << this->detectedCylinders.size() << " cylinders found";

        //print output files
        for(unsigned int i = 0; i < this->detectedPlanes.size(); i++){
            this->detectedPlanes.at(i)->writeToPts(
                        QString("D:/UNI/Master/Semester_04/Masterarbeit/pointcloudSegmentation/result/plane_%1.pts")
                        .arg(QString::number(i+1)));
        }
        for(unsigned int i = 0; i < this->detectedSpheres.size(); i++){
            this->detectedSpheres.at(i)->writeToPts(
                        QString("D:/UNI/Master/Semester_04/Masterarbeit/pointcloudSegmentation/result/sphere_%1.pts")
                        .arg(QString::number(i+1)));
        }
        for(unsigned int i = 0; i < this->detectedCylinders.size(); i++){
            this->detectedCylinders.at(i)->writeToPts(
                        QString("D:/UNI/Master/Semester_04/Masterarbeit/pointcloudSegmentation/result/cylinder_%1.pts")
                        .arg(QString::number(i+1)));
        }

        //TODO in plugin show result modal dialog

        return true;

    }
    return false;
}

/*!
 * \brief PS_PointCloud::considerNeighbourNodes
 * Check the 6 neighbour nodes of a node if the points belong to the same plane
 * \param n
 * \param param
 * \param p
 */
void PS_PointCloud::considerNeighbourNodes(PS_Node *n, PS_InputParameter param, PS_PlaneSegment *p){

    if(p->getIsValid()){

        //top node
        if(n->top != NULL && ( (n->depth - n->top->depth) < 3 || !param.forceProximity ) && !n->top->getWasConsideredInMerge()){
            this->mergeNode(n->top, param, p);
        }

        //bottom node
        if(n->bottom != NULL && ( (n->depth - n->bottom->depth) < 3 || !param.forceProximity ) && !n->bottom->getWasConsideredInMerge()){
            this->mergeNode(n->bottom, param, p);
        }

        //left node
        if(n->left != NULL && ( (n->depth - n->left->depth) < 3 || !param.forceProximity ) && !n->left->getWasConsideredInMerge()){
            this->mergeNode(n->left, param, p);
        }

        //right node
        if(n->right != NULL && ( (n->depth - n->right->depth) < 3 || !param.forceProximity ) && !n->right->getWasConsideredInMerge()){
            this->mergeNode(n->right, param, p);
        }

        //front node
        if(n->front != NULL && ( (n->depth - n->front->depth) < 3 || !param.forceProximity ) && !n->front->getWasConsideredInMerge()){
            this->mergeNode(n->front, param, p);
        }

        //back node
        if(n->back != NULL && ( (n->depth - n->back->depth) < 3 || !param.forceProximity ) && !n->back->getWasConsideredInMerge()){
            this->mergeNode(n->back, param, p);
        }

    }

}

/*!
 * \brief PS_PointCloud::considerNeighbourNodes
 * Check the 6 neighbour nodes of a node if the points belong to the same sphere
 * \param n
 * \param param
 * \param s
 */
void PS_PointCloud::considerNeighbourNodes(PS_Node *n, PS_InputParameter param, PS_SphereSegment *s){

    if(s->getIsValid()){

        //top node
        if(n->top != NULL && ( (n->depth - n->top->depth) < 3 || !param.forceProximity )  && !n->top->getWasConsideredInMerge()){
            this->mergeNode(n->top, param, s);
        }

        //bottom node
        if(n->bottom != NULL && ( (n->depth - n->bottom->depth) < 3 || !param.forceProximity ) && !n->bottom->getWasConsideredInMerge()){
            this->mergeNode(n->bottom, param, s);
        }

        //left node
        if(n->left != NULL && ( (n->depth - n->left->depth) < 3 || !param.forceProximity ) && !n->left->getWasConsideredInMerge()){
            this->mergeNode(n->left, param, s);
        }

        //right node
        if(n->right != NULL && ( (n->depth - n->right->depth) < 3 || !param.forceProximity ) && !n->right->getWasConsideredInMerge()){
            this->mergeNode(n->right, param, s);
        }

        //front node
        if(n->front != NULL && ( (n->depth - n->front->depth) < 3 || !param.forceProximity ) && !n->front->getWasConsideredInMerge()){
            this->mergeNode(n->front, param, s);
        }

        //back node
        if(n->back != NULL && ( (n->depth - n->back->depth) < 3 || !param.forceProximity ) && !n->back->getWasConsideredInMerge()){
            this->mergeNode(n->back, param, s);
        }

    }
}

/*!
 * \brief PS_PointCloud::considerNeighbourNodes
 * Check the 6 neighbour nodes of a node if the points belong to the same cylinder
 * \param n
 * \param param
 * \param c
 */
void PS_PointCloud::considerNeighbourNodes(PS_Node *n, PS_InputParameter param, PS_CylinderSegment *c){

    //top node
    if(n->top != NULL && ( (n->depth - n->top->depth) < 3 || !param.forceProximity )  && !n->top->getWasConsideredInMerge()){
        this->mergeNode(n->top, param, c);
    }

    //bottom node
    if(n->bottom != NULL && ( (n->depth - n->bottom->depth) < 3 || !param.forceProximity )  && !n->bottom->getWasConsideredInMerge()){
        this->mergeNode(n->bottom, param, c);
    }

    //left node
    if(n->left != NULL && ( (n->depth - n->left->depth) < 3 || !param.forceProximity )  && !n->left->getWasConsideredInMerge()){
        this->mergeNode(n->left, param, c);
    }

    //right node
    if(n->right != NULL && ( (n->depth - n->right->depth) < 3 || !param.forceProximity )  && !n->right->getWasConsideredInMerge()){
        this->mergeNode(n->right, param, c);
    }

    //front node
    if(n->front != NULL && ( (n->depth - n->front->depth) < 3 || !param.forceProximity )  && !n->front->getWasConsideredInMerge()){
        this->mergeNode(n->front, param, c);
    }

    //back node
    if(n->back != NULL && ( (n->depth - n->back->depth) < 3 || !param.forceProximity )  && !n->back->getWasConsideredInMerge()){
        this->mergeNode(n->back, param, c);
    }

}

/*!
 * \brief PS_PointCloud::mergeNode
 * Try to add the points of node n to the plane p
 * \param n
 * \param param
 * \param p
 */
void PS_PointCloud::mergeNode(PS_Node *n, PS_InputParameter param, PS_PlaneSegment *p){

    //number of points the plane currently contains
    unsigned int numPoints = p->getPoints().size();

    //save the current state of the plane to be able to fall back later
    p->saveCurrentState();

    //try to add further points of node n
    PS_PlaneSegment::checkPointsInPlane(p, n->points, param, 2);

    //set node as considered in merge and add it to list of merged nodes
    n->setWasConsideredInMerge(true);
    PS_PointCloud::mergedNodes.append(n);

    //if at minimum 4 points have been found in the node consider its neighbours
    if(p->getPoints().size() - numPoints >= 4){

        //refit the plane before considering further neighbours
        p->fitBySample(param.fitSampleSize);

        if(!p->getIsValid()){
            p->fallBack();
        }else{

            //sort out points that do not satisfy the distance criterion
            PS_PlaneSegment::sortOut(p, param);

            //if the plane (after merging, fitting and sorting out) contains more points than before
            if(p->getPoints().size() - numPoints > 0){

                //consider the neighbour's neighbours
                this->considerNeighbourNodes(n, param, p);

            }else{
                p->fallBack();
            }

        }

    }else{
        p->fallBack();
    }

}

/*!
 * \brief PS_PointCloud::mergeNode
 * Try to add the points of node n to the sphere s
 * \param n
 * \param param
 * \param s
 */
void PS_PointCloud::mergeNode(PS_Node *n, PS_InputParameter param, PS_SphereSegment *s){

    //number of points the sphere currently contains
    unsigned int numPoints = s->getPoints().size();

    //save the current state of the sphere to be able to fall back later
    s->saveCurrentState();

    //try to add further points of node n
    PS_SphereSegment::checkPointsInSphere(s, n->points, param, 2);

    //set node as considered in merge and add it to list of merged nodes
    n->setWasConsideredInMerge(true);
    mergedNodes.append(n);

    //if at minimum 5 points have been found in the node consider its neighbours
    if(s->getPoints().size() - numPoints >= 5){

        //refit the sphere before considering further neighbours
        s->fitBySample(param.fitSampleSize);

        if(!s->getIsValid() || s->getRadius() > param.sphereParams.maxRadius || s->getRadius() < param.sphereParams.minRadius){
            s->fallBack();
        }else{

            //sort out points that do not satisfy the distance criterion
            PS_SphereSegment::sortOut(s, param);

            //if the sphere (after merging, fitting and sorting out) contains more points than before
            if(s->getPoints().size() - numPoints > 0){

                //consider the neighbour's neighbours
                this->considerNeighbourNodes(n, param, s);

            }else{
                s->fallBack();
            }

        }

    }else{
        s->fallBack();
    }

}

/*!
 * \brief PS_PointCloud::mergeNode
 * Try to add the points of node n to the cylinder c
 * \param n
 * \param param
 * \param c
 */
void PS_PointCloud::mergeNode(PS_Node *n, PS_InputParameter param, PS_CylinderSegment *c){

    //number of points the cylinder currently contains
    unsigned int numPoints = c->getPoints().size();

    //save the current state of the cylinder to be able to fall back later
    c->saveCurrentState();

    //try to add further points of node n
    PS_CylinderSegment::checkPointsInCylinder(c, n->points, param, 2);

    //set node as considered in merge and add it to list of merged nodes
    n->setWasConsideredInMerge(true);
    mergedNodes.append(n);

    //if at minimum 6 points have been found in the node consider its neighbours
    if(c->getPoints().size() - numPoints >= 6){

        //refit the cylinder before considering further neighbours
        c->fitBySample(param.fitSampleSize);

        if(!c->getIsValid()){
            c->fallBack();
        }else{

            //sort out points that do not satisfy the distance criterion
            PS_CylinderSegment::sortOut(c, param);

            //if the cylinder (after merging, fitting and sorting out) contains more points than before
            if(c->getPoints().size() - numPoints > 0){

                //consider the neighbour's neighbours
                this->considerNeighbourNodes(n, param, c);

            }else{
                c->fallBack();
            }

        }

    }else{
        c->fallBack();
    }

}
