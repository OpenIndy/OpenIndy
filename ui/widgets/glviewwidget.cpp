#include "glviewwidget.h"

GlViewWidget::GlViewWidget(QWidget *parent) : QWidget(parent)
{
    GlWindow *view = new GlWindow();
    QWidget *container = this->createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addWidget(container, 1);
    this->setLayout(hLayout);


    engine.registerAspect(new Qt3D::QRenderAspect());
    input = new Qt3D::QInputAspect;
    engine.registerAspect(input);
    engine.initialize();
    QVariantMap data;
    data.insert(QStringLiteral("surface"), QVariant::fromValue(static_cast<QSurface *>(view)));
    data.insert(QStringLiteral("eventSource"), QVariant::fromValue(view));
    engine.setData(data);

    // Root entity
    Qt3D::QEntity *rootEntity = new Qt3D::QEntity();

    // Camera
    cameraEntity = new Qt3D::QCamera(rootEntity);

    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraPosition = QVector3D(0, 0, -40.0f);
    cameraEntity->setPosition(cameraPosition);
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));
    input->setCamera(cameraEntity);

    // FrameGraph
    Qt3D::QFrameGraph *frameGraph = new Qt3D::QFrameGraph();
    Qt3D::QForwardRenderer *forwardRenderer = new Qt3D::QForwardRenderer();
    forwardRenderer->setClearColor(QColor::fromRgbF(0.0, 0.5, 1.0, 1.0));
    forwardRenderer->setCamera(cameraEntity);
    frameGraph->setActiveFrameGraph(forwardRenderer);

    scene.buildScene(rootEntity);


    rootEntity->addComponent(frameGraph);

    engine.setRootEntity(rootEntity);

}

QPointer<OiJob> GlViewWidget::getCurrentJob() const
{
    return scene.getCurrentJob();
}

void GlViewWidget::setCurrentJob(const QPointer<OiJob> &value)
{
    scene.setCurrentJob(value);
}

void GlViewWidget::mouseDoubleClickEvent(QMouseEvent *event)
{



}

void GlViewWidget::wheelEvent(QWheelEvent *event)
{


        float z = cameraPosition.z();
        cameraPosition.setZ(z+event->delta()*100);
        cameraEntity->setPosition(cameraPosition);
        qDebug()<<"zoom";


    input->setCamera(cameraEntity);

    event->accept();

}
