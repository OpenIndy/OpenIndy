#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include <QObject>
#include <QList>
#include <QPointer>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QCamera>
#include <Qt3DCore/QCameraLens>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QLookAtTransform>
#include <Qt3DCore/QScaleTransform>
#include <Qt3DCore/QRotateTransform>
#include <Qt3DCore/QTranslateTransform>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DInput/QInputAspect>

#include <Qt3DRenderer/QRenderAspect>
#include <Qt3DRenderer/QFrameGraph>
#include <Qt3DRenderer/QForwardRenderer>
#include <Qt3DRenderer/QPhongMaterial>

#include <Qt3DRenderer/QCylinderMesh>
#include <Qt3DRenderer/QSphereMesh>
#include <Qt3DRenderer/QTorusMesh>
#include <Qt3DRenderer/QCuboidMesh>

#include <QPropertyAnimation>

#include "oijob.h"

using namespace oi;

class SceneBuilder : public QObject
{
    Q_OBJECT
public:
    explicit SceneBuilder(QObject *parent = 0);

    QPointer<OiJob> getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &value);

signals:

public slots:
    void buildScene(Qt3D::QEntity *rootEntity);

private:
    QList<QPointer<Qt3D::QEntity> > entities;
    QPointer<OiJob> currentJob;

    void addPointToScene(Qt3D::QEntity *rootEntity, float x, float y, float z);
    void testScene(Qt3D::QEntity *rootEntity);
};

#endif // SCENEBUILDER_H
