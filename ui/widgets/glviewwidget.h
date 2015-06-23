#ifndef GLVIEWWIDGET_H
#define GLVIEWWIDGET_H

#include <QGLWidget>
#include <QtWidgets>
#include <QtOpenGL>


class GlViewWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GlViewWidget(QWidget *parent = 0);

signals:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

public slots:

private:


};

#endif // GLVIEWWIDGET_H
