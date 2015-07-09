/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glwindow.h"

#include <QKeyEvent>
#include <QWheelEvent>
#include <QGuiApplication>
#include <QOpenGLContext>

GlWindow::GlWindow(QScreen *screen)
    : QWindow(screen)

{
    setSurfaceType(QSurface::OpenGLSurface);

    //resize(1024, 768);

    QSurfaceFormat format;
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        format.setVersion(4, 3);
        format.setProfile(QSurfaceFormat::CoreProfile);
    }
    format.setDepthBufferSize( 24 );
    format.setSamples( 4 );
    setFormat(format);
    create();

    engine.registerAspect(new Qt3D::QRenderAspect());
    input = new Qt3D::QInputAspect;
    engine.registerAspect(input);
    engine.initialize();
    QVariantMap data;
    data.insert(QStringLiteral("surface"), QVariant::fromValue(static_cast<QSurface *>(this)));
    data.insert(QStringLiteral("eventSource"), QVariant::fromValue(this));
    engine.setData(data);

    // Root entity
    rootEntity = new Qt3D::QEntity();

    // Camera
    cameraEntity = new Qt3D::QCamera(rootEntity);

    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraPosition = QVector3D(0, 0, -40.0f);
    cameraEntity->setPosition(cameraPosition);
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));
    input->setCamera(cameraEntity);

    // FrameGraph
    frameGraph = new Qt3D::QFrameGraph();
    forwardRenderer = new Qt3D::QForwardRenderer();
    forwardRenderer->setClearColor(QColor::fromRgbF(0.0, 0.5, 1.0, 1.0));
    forwardRenderer->setCamera(cameraEntity);
    frameGraph->setActiveFrameGraph(forwardRenderer);

    scene.buildScene(rootEntity);

    rootEntity->addComponent(frameGraph);

    engine.setRootEntity(rootEntity);
}

GlWindow::~GlWindow()
{
}

QPointer<OiJob> GlWindow::getCurrentJob() const
{
    return this->scene.getCurrentJob();
}

void GlWindow::setCurrentJob(const QPointer<OiJob> &value)
{
    this->scene.setCurrentJob(value);
}

void GlWindow::wheelEvent(QWheelEvent *ev)
{
    cameraPosition = cameraEntity->position();
    cameraPosition.setZ(cameraPosition.z()+ev->delta()/10);
    cameraEntity->setPosition(cameraPosition);
    ev->accept();
}

void GlWindow::mouseDoubleClickEvent(QMouseEvent *event)
{

    //TODO bei doppelklick soll neu gezeichnet werden
    scene.buildScene(rootEntity);
    engine.setRootEntity(rootEntity);
    event->accept();

}

