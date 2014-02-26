//---------------------------------------------------------------------------

#ifndef cfitting_ellipseH
#define cfitting_ellipseH
//---------------------------------------------------------------------------

#include "cfitting_it.h"

TReal     ellipse_equation(TReal *x,TReal *para);
TReal*  d_ellipse_equation(TReal *x,TReal *para,TReal *df);

void      ellipse_polynom2MxMyThetaAB(TReal *polynom,TReal *para);
double* d_ellipse_polynom2MxMyThetaAB(TReal *polynom,TReal *para,TReal *df);

bool fitting_ellipse(double *X,double *Y,int count,double *xm,double *theta,double *A,double *Qff);

#endif
