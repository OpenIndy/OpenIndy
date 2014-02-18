#ifndef OIGRAPHIXGEOMETRY_H
#define OIGRAPHIXGEOMETRY_H

#include <QtOpenGL>
#include <math.h>

#define PI 3.141592653589793

#ifdef Q_OS_MAC
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

class OiGraphixGeometry
{
public:
    virtual ~OiGraphixGeometry(){}

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;

    virtual void draw(GLfloat x, GLfloat y, GLfloat z) = 0;
};

#endif // OIGRAPHIXGEOMETRY_H
