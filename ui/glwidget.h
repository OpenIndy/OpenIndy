#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <QMouseEvent>
#include <QPoint>

#include "featurewrapper.h"
#include "oiGraphixFactory/oigraphix.h"
#include "oifeaturestate.h"

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

    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    GLfloat translateX;
    GLfloat translateY;
    GLfloat translateZ;
    QColor faceColors[4];
    QPoint lastPos;
    int xMax;
    int yMax;
    int zMax;

    QColor oiBackgroundColor;

signals:

public slots:
    void focusOnFeature(double x, double y, double z);

};

#endif // GLWIDGET_H
