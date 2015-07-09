#include "scenebuilder.h"

SceneBuilder::SceneBuilder(QObject *parent) : QObject(parent)
{

}

QPointer<OiJob> SceneBuilder::getCurrentJob() const
{
    return currentJob;
}

void SceneBuilder::setCurrentJob(const QPointer<OiJob> &value)
{
    currentJob = value;
}

void SceneBuilder::buildScene(Qt3D::QEntity *rootEntity)
{

    /*if(this->currentJob.isNull()){
        return;
    }

    QList<QPointer<FeatureWrapper> > geoms = this->currentJob->getGeometriesList();

    foreach(const QPointer<FeatureWrapper> &f, geoms){
        if(f.isNull() || f->getGeometry().isNull()){
            continue;
        }

        if(f->getGeometry()->hasPosition()){
            float x = (float)f->getGeometry()->getPosition().getVector().getAt(0);
            float y = (float)f->getGeometry()->getPosition().getVector().getAt(1);
            float z = (float)f->getGeometry()->getPosition().getVector().getAt(2);

            this->addPointToScene(rootEntity,x,y,z);
        }

    }*/

    this->testScene(rootEntity);


}



void SceneBuilder::addPointToScene(Qt3D::QEntity *rootEntity, float x, float y, float z)
{

    // Cuboid shape data
    Qt3D::QCuboidMesh *cuboid = new Qt3D::QCuboidMesh();

    // CuboidMesh Transform
    Qt3D::QScaleTransform *cuboidScale = new Qt3D::QScaleTransform();
    Qt3D::QTranslateTransform *cuboidTranslation = new Qt3D::QTranslateTransform();
    Qt3D::QTransform *cuboidTransforms = new Qt3D::QTransform();

    cuboidScale->setScale3D(QVector3D(4.0f, 4.0f, 4.0f));
    cuboidTranslation->setTranslation(QVector3D(x, y, z));

    cuboidTransforms->addTransform(cuboidTranslation);
    cuboidTransforms->addTransform(cuboidScale);

    Qt3D::QPhongMaterial *cuboidMaterial = new Qt3D::QPhongMaterial();
    cuboidMaterial->setDiffuse(QColor(QRgb(0x665423)));

    //Cuboid
    Qt3D::QEntity *m_cuboidEntity = new Qt3D::QEntity(rootEntity);
    m_cuboidEntity->addComponent(cuboid);
    m_cuboidEntity->addComponent(cuboidMaterial);
    m_cuboidEntity->addComponent(cuboidTransforms);

}


void SceneBuilder::testScene(Qt3D::QEntity *rootEntity)
{
    // Material
    Qt3D::QMaterial *material = new Qt3D::QPhongMaterial(rootEntity);

    // Torus
    Qt3D::QEntity *torusEntity = new Qt3D::QEntity(rootEntity);
    Qt3D::QTorusMesh *torusMesh = new Qt3D::QTorusMesh;
    torusMesh->setRadius(5);
    torusMesh->setMinorRadius(1);
    torusMesh->setRings(100);
    torusMesh->setSlices(20);

    Qt3D::QTransform *torusTransform = new Qt3D::QTransform;
    Qt3D::QScaleTransform *torusScaleTransform = new Qt3D::QScaleTransform;
    torusScaleTransform->setScale3D(QVector3D(1.5, 1, 0.5));

    Qt3D::QRotateTransform *torusRotateTransform = new Qt3D::QRotateTransform;
    torusRotateTransform->setAxis(QVector3D(1, 0, 0));
    torusRotateTransform->setAngleDeg(45);

    torusTransform->addTransform(torusScaleTransform);
    torusTransform->addTransform(torusRotateTransform);

    torusEntity->addComponent(torusMesh);
    torusEntity->addComponent(torusTransform);
    torusEntity->addComponent(material);

    // Sphere
    Qt3D::QEntity *sphereEntity = new Qt3D::QEntity(rootEntity);
    Qt3D::QSphereMesh *sphereMesh = new Qt3D::QSphereMesh;
    sphereMesh->setRadius(3);

    Qt3D::QTransform *sphereTransform = new Qt3D::QTransform;
    Qt3D::QTranslateTransform *sphereTranslateTransform = new Qt3D::QTranslateTransform;
    sphereTranslateTransform->setTranslation(QVector3D(20, 0, 0));

    Qt3D::QRotateTransform *sphereRotateTransform = new Qt3D::QRotateTransform;
    QPropertyAnimation *sphereRotateTransformAnimation = new QPropertyAnimation(sphereRotateTransform);
    sphereRotateTransformAnimation->setTargetObject(sphereRotateTransform);
    sphereRotateTransformAnimation->setPropertyName("angle");
    sphereRotateTransformAnimation->setStartValue(QVariant::fromValue(0));
    sphereRotateTransformAnimation->setEndValue(QVariant::fromValue(360));
    sphereRotateTransformAnimation->setDuration(10000);
    sphereRotateTransformAnimation->setLoopCount(-1);
    sphereRotateTransformAnimation->start();

    sphereRotateTransform->setAxis(QVector3D(0, 1, 0));
    sphereRotateTransform->setAngleDeg(0);

    sphereTransform->addTransform(sphereTranslateTransform);
    sphereTransform->addTransform(sphereRotateTransform);

    sphereEntity->addComponent(sphereMesh);
    sphereEntity->addComponent(sphereTransform);
    sphereEntity->addComponent(material);

    // Cylinder shape data
    Qt3D::QCylinderMesh *cylinder = new Qt3D::QCylinderMesh();
    cylinder->setRadius(1);
    cylinder->setLength(3);
    cylinder->setRings(100);
    cylinder->setSlices(20);

    // CylinderMesh Transform
    Qt3D::QScaleTransform *cylinderScale = new Qt3D::QScaleTransform();
    Qt3D::QRotateTransform *cylinderRotation = new Qt3D::QRotateTransform();
    Qt3D::QTranslateTransform *cylinderTranslation = new Qt3D::QTranslateTransform();
    Qt3D::QTransform *cylinderTransforms = new Qt3D::QTransform();

    cylinderScale->setScale3D(QVector3D(1.5f, 1.5f, 1.5f));
    cylinderTranslation->setTranslation(QVector3D(-2.6f, 2.4f, -1.5));
    cylinderRotation->setAngleDeg(45.0f);
    cylinderRotation->setAxis(QVector3D(1, 0, 0));

    cylinderTransforms->addTransform(cylinderTranslation);
    cylinderTransforms->addTransform(cylinderRotation);
    cylinderTransforms->addTransform(cylinderScale);

    Qt3D::QPhongMaterial *cylinderMaterial = new Qt3D::QPhongMaterial();
    cylinderMaterial->setDiffuse(QColor(QRgb(0x928327)));

    // Cylinder
    Qt3D::QEntity *m_cylinderEntity = new Qt3D::QEntity(rootEntity);
    m_cylinderEntity->addComponent(cylinder);
    m_cylinderEntity->addComponent(cylinderMaterial);
    m_cylinderEntity->addComponent(cylinderTransforms);

    // Cuboid shape data
    Qt3D::QCuboidMesh *cuboid = new Qt3D::QCuboidMesh();

    // CuboidMesh Transform
    Qt3D::QScaleTransform *cuboidScale = new Qt3D::QScaleTransform();
    Qt3D::QTranslateTransform *cuboidTranslation = new Qt3D::QTranslateTransform();
    Qt3D::QTransform *cuboidTransforms = new Qt3D::QTransform();

    cuboidScale->setScale3D(QVector3D(4.0f, 4.0f, 4.0f));
    cuboidTranslation->setTranslation(QVector3D(0.9f, -0.9f, 0.0f));

    cuboidTransforms->addTransform(cuboidTranslation);
    cuboidTransforms->addTransform(cuboidScale);

    Qt3D::QPhongMaterial *cuboidMaterial = new Qt3D::QPhongMaterial();
    cuboidMaterial->setDiffuse(QColor(QRgb(0x665423)));

    //Cuboid
    Qt3D::QEntity *m_cuboidEntity = new Qt3D::QEntity(rootEntity);
    m_cuboidEntity->addComponent(cuboid);
    m_cuboidEntity->addComponent(cuboidMaterial);
    m_cuboidEntity->addComponent(cuboidTransforms);
}


