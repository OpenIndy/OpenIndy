#ifndef OICADVIEWER_H
#define OICADVIEWER_H

#include <QWidget>
#include "oicadcontrol.h"

#include <AIS_InteractiveContext.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <OpenGl_View.hxx>

#include <Voxel_Prs.hxx>
#include <Voxel_Selector.hxx>

#include <BRepPrimAPI_MakeTorus.hxx>

#include <AIS_Shape.hxx>
#include <gp_Pnt.hxx>



class OiCadViewer : public QWidget
{
    Q_OBJECT
public:
    explicit OiCadViewer(QWidget *parent = 0);


    Handle(AIS_InteractiveContext) getIC() const { return myIC; }
    Handle(V3d_View) getView() const { return myView; }
    Handle(OpenGl_GraphicDriver) getGraphicDriver() const { return myGraphicDriver; }
    Voxel_Selector& getSelector() { return mySelector; }
    void setPrs(const Handle(Voxel_Prs)& prs) { myPrs = prs; }
    virtual QPaintEngine*          paintEngine() const;

signals:
    void mousePressed(Qt::KeyboardModifiers,int,int);
    void mouseMoved(Qt::KeyboardModifiers,int,int);
    void mouseReleased(Qt::KeyboardModifiers,int,int);
    void mouseDoubleClick(Qt::KeyboardModifiers,int,int);

public slots:


protected:
    virtual void paintEvent(QPaintEvent* pEvent);
    virtual void resizeEvent(QResizeEvent* rsEvent);
    virtual void mousePressEvent(QMouseEvent* mpEvent);
    virtual void mouseMoveEvent(QMouseEvent* mmEvent);
    virtual void mouseReleaseEvent(QMouseEvent* mrEvent);
    virtual void mouseDoubleClickEvent(QMouseEvent* mdcEvent);
    virtual void wheelEvent(QWheelEvent* event);



private:
    Handle(AIS_InteractiveContext) myIC;
    Handle(V3d_View)               myView;
    Handle(OpenGl_GraphicDriver)   myGraphicDriver;

    bool myRotate;
    bool myZoom;
    bool myPan;

    QPoint myStartPnt;

    void setDegenerateMode(const bool );

    Voxel_Selector mySelector;
    Handle(Voxel_Prs) myPrs;

    OiCadControl control;

};

#endif // OICADVIEWER_H
