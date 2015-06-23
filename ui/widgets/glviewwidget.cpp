#include "glviewwidget.h"

GlViewWidget::GlViewWidget(QWidget *parent) : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::Rgba | QGL::DoubleBuffer | QGL::DepthBuffer));

}

void GlViewWidget::initializeGL()
{



}

void GlViewWidget::paintGL()
{



}

void GlViewWidget::resizeGL(int width, int height)
{


}

