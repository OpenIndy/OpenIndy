#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <QMouseEvent>
#include <QPoint>

#include "featurewrapper.h"
#include "oiGraphixFactory/oigraphix.h"
#include "oifeaturestate.h"

#include <QQuaternion>
#include <QMatrix4x4>

#ifdef Q_OS_MAC
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void draw();
    int faceAtPosition(const QPoint &pos);
    void drawScale();
    void updateCenterAndReadius();

    void mouseToTrackball(int x, int y, int W, int H, OiVec &v);
    void trackball(OiVec u, OiVec v, QQuaternion &q);
    void drawCenterSphere();

    QQuaternion q_now;

    OiVec rotationAxes;
    GLfloat rotationAngle;

    GLfloat translateX;
    GLfloat translateY;
    GLfloat translateZ;
    QColor faceColors[4];
    QPoint lastPos;
    int xMax;
    int yMax;
    int zMax;

    OiVec center;
    double radius;

    double zoom;

    int oldMouseX;
    int oldMouseY;

    QColor oiBackgroundColor;

signals:

public slots:
    void focusOnFeature(Geometry *g);
    void activeFeatureChanged();

};

#endif // GLWIDGET_H
