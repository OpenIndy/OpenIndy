/**************************************************************************************************************hbegin**
* Filename   : 
* Description: 
* Purpose    :
* Reference  :
* Authors    : Martin Lux (lux@sigma3D.de), Jens Wambach (Jens.Wambach@t-online.de), Benedikt Rauls (benediktrauls@web.de),
*              Fredie Kern (xdesy@gmx.de)
*
* Version    :
*
* Info       : wwww.github.com - www.fh-mainz.de - www.xdesy.de

* Copyright 2013-2014 Lux, Rauls, Wambach, Fachhochschule Mainz - Lehreinheit Geoinformatik und Vermessung - Prof. Dr.-Ing. Fredie Kern  
*
* Licence:
*    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
*    License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any 
*    later version.
*
*    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
*    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License along with this program.  If not, 
*    see <http://www.gnu.org/licenses/>.
*****************************************************************************************************************hend*/
//---------------------------------------------------------------------------

#pragma hdrstop

#include "cfitting_ellipse.h"

#include  <stdlib.h>
#include  <math.h>
#include  "rho.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

extern int    gDebugLevel;
extern double gConvergence;

/*************************************************/

TReal ellipse_equation(TReal *x,TReal *para)
{
  TReal ret;
  TReal a1,a2,a3,a4,a5;

  // Otepka_PhDThesis.pdf
  // implizite Darstellung in Form eines Polynoms 2. Grades
  a1       = para[0];
  a2       = para[1];
  a3       = para[2];
  a4       = para[3];
  a5       = para[4];

  ret = a1*x[0]*x[0] + a2*x[0]*x[1] + a3*x[1]*x[1] + a4*x[0] + a5*x[1]  - 1;

  return(ret);
}

TReal* d_ellipse_equation(TReal *x,TReal *para,TReal *df)
{
  df[0] = x[0]*x[0];  // df/da1
  df[1] = x[0]*x[1];  // df/da2
  df[2] = x[1]*x[1];  // df/da3
  df[3] =      x[0];  // df/da4
  df[4] =      x[1];  // df/da5

  return(df);
}


void ellipse_polynom2MxMyThetaAB(TReal *polynom,TReal *para)
{
  TReal a1,a2,a3,a4,a5;
  TReal a,b,c,d,e,f,g;
  TReal xs,ys,as,bs,thet;
  TReal z,n;

  a1       = polynom[0];
  a2       = polynom[1];
  a3       = polynom[2];
  a4       = polynom[3];
  a5       = polynom[4];

/*
    xm[0] = (a2*a5 - 2.0*a3*a4)/(4.0*a1*a3-a2*a2);
    xm[1] = (a2*a4 - 2.0*a1*a5)/(4.0*a1*a3-a2*a2);
    *theta = -0.5*atan2(a2,(a1-a3));

    z = 1.0 - 0.5*a4*xm[0] - 0.5*a5*xm[1];

    n =  a1*cos(*theta)*cos(*theta) - a2*sin(*theta)*cos(*theta) + a3*sin(*theta)*sin(*theta);
    A[0] = sqrt(fabs(z/n));

    n =  a3*cos(*theta)*cos(*theta) + a2*sin(*theta)*cos(*theta) + a1*sin(*theta)*sin(*theta);

    A[1] = sqrt(fabs(z/n));

*/
  g =  1.0;
  a =     -a1*g;
  b = -0.5*a2*g;
  c =     -a3*g;
  d = -0.5*a4*g;
  f = -0.5*a5*g;

/*
    TReal Delta,J,I;
    Delta = a * (c*g-f*f) - b * (b*g-f*d) + d * (b*f-c*d);
    J = a*c-b*b;
    I = a + c;

    if (fabs(Delta)>1e-3 && J>0.0 && Delta/I < 0.0) printf("is ellipse\n"); else printf("is NOT ellipse\n");
*/

  xs = (c*d - b*f)/(b*b-a*c);
  ys = (a*f - b*d)/(b*b-a*c);

  z = 2.0*(a*f*f + c*d*d + g*b*b - 2.0*b*d*f - a*c*g);
  n = ((b*b-a*c)*( sqrt((a-c)*(a-c) +  4.0*b*b) - (a+c) ) );
  as = sqrt(fabs( z / n ));

  n = ((b*b-a*c)*(-sqrt((a-c)*(a-c) +  4.0*b*b) - (a+c) ) );
  bs = sqrt(fabs( z / n ));

  if (fabs(b)<1e-12)
  {
    thet =  a<c ? 0.0 : M_PI/2.0;
  }
  else
  {
    thet = 0.5*atan2((2.0*b),(a-c));
    if (a>=c)
    {
      thet = M_PI/2.0 + thet;
    }
  }
/*
    printf("xm[0] = %lf xm[1] = %lf   xs = %lf ys = %lf\n",xm[0],xm[1],xs,ys);
    printf("A[0] = %lf  A[1] = %lf    as = %lf bs = %lf\n",A[0],A[1],as,bs);
    printf("theta = %lf thet = %lf\n",*theta*200.0/M_PI,thet*200.0/M_PI);
*/
  para[0] = xs;
  para[1] = ys;
  para[2] = thet;
  para[3] = as;
  para[4] = bs;
}

double*  d_ellipse_polynom2MxMyThetaAB(TReal *polynom,TReal *para,TReal *df)
{
  double polynomh[5];
  double paraf[5];
  int f,u;

  f = 5;
  u = 5;

  ellipse_polynom2MxMyThetaAB(polynom,para);
  for (int i=0;i<u;i++)
  {
    for (int j=0;j<u;j++) polynomh[j] = polynom[j];
    polynomh[i] += polynomh[i]<0.0 ? -0.00001 : 0.00001;
    ellipse_polynom2MxMyThetaAB(polynomh,paraf);
    for (int nr=0;nr<f;nr++)
    {
      df[nr*u+i] = (paraf[nr] - para[nr])/(polynomh[i] - polynom[i]);
    }
  }

  return(df);
}

bool fitting_ellipse(double *X,double *Y,int count,double *xm,double *theta,double *A,double *Qff)
{
  Cfitting_it  fitting_it(gDebugLevel>=4,gConvergence);
  TReal   ubk[5];
  TReal   Qxx[5*5];
  TReal **x;  // Parameter
  TReal  *y;  // abhängige Variable
  bool    ret;
  int     iter;
  TReal   convergence;

  fitting_it.coords_as_array(X,Y,NULL,count,&x,&y);
  /* approx. values */
  ubk[0] = ubk[1] = ubk[2] = 0.0;
  ubk[3] = ubk[4] = 1.0;
  ret = fitting_it.approximation(y,count,x,count,ubk,5,Qxx,
               &iter,&convergence,
               ellipse_equation,
               d_ellipse_equation
               );

  if (ret==true)
  {
    TReal para[5];

    ellipse_polynom2MxMyThetaAB(ubk,para);

    xm[0]  = para[0];
    xm[1]  = para[1];
    *theta = para[2];
    A[0]   = para[3];
    A[1]   = para[4];

    if (Qff!=NULL) fitting_it.variancepropagation(ubk,5,para,5,Qxx,Qff,d_ellipse_polynom2MxMyThetaAB);
  }
  fitting_it.free_array(x,y,count);
  return(ret);
}
