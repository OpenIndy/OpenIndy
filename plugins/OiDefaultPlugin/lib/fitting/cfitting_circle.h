//---------------------------------------------------------------------------

#ifndef cfitting_circleH
#define cfitting_circleH
//---------------------------------------------------------------------------

#include "cfitting_it.h"

TReal    circle_equation(TReal *x,TReal *para);
TReal* d_circle_equation(TReal *x,TReal *para,TReal *df);

bool fitting_circle(double *X,double *Y,int count,double *xm,double *r,double *Qxx);

#endif
