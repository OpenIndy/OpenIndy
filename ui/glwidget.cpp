#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    zoom = 1.0;

    center = OiVec(4);
    radius = 1.0;

    rotationAxes = OiVec(4);
    rotationAngle = 0.0;


    setFormat(QGLFormat (QGL::DoubleBuffer | QGL::DepthBuffer));
    translateZ = 0.0;
    translateX = 0.0;
    translateY = 0.0;
    rotationX =0.0;
    rotationY =0.0;
    rotationZ =0.0;
    xMax = 0;
    yMax = 0;
    zMax = 0;
    faceColors[0] = Qt::red;
    faceColors[1] = Qt::green;
    faceColors[2] = Qt::blue;
    faceColors[3] = Qt::yellow;

    oiBackgroundColor = QColor::fromCmykF(0.59,0.40,0.10,0.10);


    QObject::connect(OiFeatureState::getInstance(),SIGNAL(activeFeatureChanged()),this,SLOT(activeFeatureChanged()));

    //QTimer::singleShot(20, this, SLOT(update()));
}


void GLWidget::initializeGL(){


    qglClearColor(oiBackgroundColor.lighter());
    //glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glEnable(GL_SMOOTH);



    static GLfloat lightPosition[4] = {0.0,0.0,0.0,1.0};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);

    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void GLWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw();

}

void GLWidget::resizeGL(int w, int h){

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(70, w / h, 0.01, 1000);
#ifdef QT_OPENGL_ES_1
    glOrthof(-3.0, +3.0, -3.0, +3.0, 0.1, 15.0);
#else
    glOrtho(-3.0, +3.0, -3.0, +3.0, 0.1, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);

}


void GLWidget::setCenterAndReadius(){
    radius = 1.0;

    if(OiFeatureState::getFeatures().size() > 0){

        for(int i =0; i< OiFeatureState::getFeatures().size(); i++){
            OiGraphix::drawFeature(OiFeatureState::getFeatures().at(i));

            if(OiFeatureState::getFeatures().at(i)->getGeometry() != NULL){
                Geometry *g = OiFeatureState::getFeatures().at(i)->getGeometry();

                center = center + g->getXYZ();

                double x = g->getXYZ().getAt(0);
                double y = g->getXYZ().getAt(1);
                double z = g->getXYZ().getAt(2);

                double abs = sqrt(x*x + y*y + z*z);

                if (abs > radius){
                    radius = abs;
                }


            }
        }

        center = center/OiFeatureState::getFeatures().size();
    }
}



/*!
 * \brief GLWidget::draw
 * Redraw all features
 */
void GLWidget::draw(){

    setCenterAndReadius();

    glMatrixMode(GL_MODELVIEW);


    glLoadIdentity();


    glRotatef(rotationX, 1.0, 0.0 ,0.0);
    glRotatef(rotationY, 0.0, 1.0 ,0.0);
    glRotatef(rotationZ, 0.0, 0.0 ,1.0);


    glScaled(zoom,zoom,zoom);


    //glTranslatef(center.getAt(0)+translateX, center.getAt(1)+translateY, center.getAt(2)+translateZ);

    glTranslated(translateX, translateY, translateZ);

    qglColor(Qt::red);


   // drawSacle();
    update();


}

int GLWidget::faceAtPosition(const QPoint &pos){
    return 0;
}

void GLWidget::mousePressEvent(QMouseEvent *event){
    lastPos = event->pos();

    oldMouseX = event->x();
    oldMouseY = event->y();

}

void GLWidget::mouseMoveEvent(QMouseEvent *event){


    OiVec u;
    OiVec v;

    mouseToTrackball(oldMouseX, oldMouseY, width(), height(), u);

    mouseToTrackball(event->x(),event->y(),width(),height(),v);

    trackball(u,v);

    oldMouseX = event->x();
    oldMouseY = event->y();



    //ab hier alte Drehung, kann dann weg
/*
    GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();

    if(event->buttons() & Qt::LeftButton) {
        rotationX += 180 *dy;
        rotationY += 180 *dx;
        updateGL();
    } else if(event->buttons() & Qt::RightButton){
        //rotationX += 180 * dy;
        //rotationZ += 180 * dx;
        translateX += 10*dx;
        translateY += 10*dy;
        updateGL();
    }
    lastPos = event->pos();
    */
}

void GLWidget::mouseToTrackball (int x, int y, int W, int H, OiVec &v)
{
    double ox, oy, oz, r;

    if (W>H)
    {
        r = H/2.0;
    }
    else
    {
        r = W/2.0;
    }

    ox = (x - r)/r;
    oy = (y - r)/r;

    oy *= -1;


    if (ox*ox + oy*oy > radius)
    {
        double length = sqrt(ox*ox+oy*oy);
        ox = ox/length;
        oy = oy/length;

        oz = 0.0;
    }
    else
    {
        oz = sqrt(radius - ox*ox - oy*oy);
    }


    v.setAt(0,ox);
    v.setAt(1,oy);
    v.setAt(2, 1.0);


}

void GLWidget::trackball(OiVec u, OiVec v)
{

    OiVec::cross(rotationAxes, v, u);




}


void GLWidget::mouseDoubleClickEvent(QMouseEvent *event){
    //TODO bei doppel klick koordinaten abgreifen
    QString x = QString::number(event->x());
    QString y =QString::number(event->y());
    QMessageBox::information(this,"coordinate", QString("x:" + x + " y:" +y));
}

void GLWidget::wheelEvent(QWheelEvent *event){

    if (event->delta() < 0) zoom *= 1.2; else zoom *= 1/1.2;
   updateGL();
}

void GLWidget::focusOnFeature(Geometry *g){
    //TODO auf actives feature springen
    //gluLookAt

    double x = g->getXYZ().getAt(0);
    double y = g->getXYZ().getAt(1);
    double z = g->getXYZ().getAt(2);

    translateX = x;
    translateY = y;
    translateZ = z;


    updateGL();

}


/*
 *
 * */
void GLWidget::activeFeatureChanged()
{
    if(OiFeatureState::getActiveFeature() != NULL){

        if(OiFeatureState::getActiveFeature()->getGeometry() != NULL){
            Geometry *g = OiFeatureState::getActiveFeature()->getGeometry();

            focusOnFeature(g);
      }
   }
}

 void GLWidget::drawSacle(){

     glMatrixMode(GL_MODELVIEW);

     glPushMatrix();

     glLoadIdentity();
     glPointSize(10.0);
     glColor3f(0.0f, 0.0f, 0.0f);


     //just a test line
     glBegin(GL_LINES);
         glVertex3f(0, -1, -2);
         glVertex3f(1, -1, -2);
     glEnd();


     glPopMatrix();
 }
