//---------------------------------------------------------------------------

#ifndef cfitting_sphereH
#define cfitting_sphereH
//---------------------------------------------------------------------------

#include "cfitting_it.h"

TReal    sphere_equation(TReal *x,TReal *para);
TReal* d_sphere_equation(TReal *x,TReal *para,TReal *df);

bool fitting_sphere(double *X,double *Y,double *Z,int count,double *xm,double *r,double *Qxx);
#endif
