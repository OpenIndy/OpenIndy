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

#include "cfitting_circle.h"

#include <stdlib.h>
#include <math.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)

extern int    gDebugLevel;
extern double gConvergence;

TReal circle_equation(TReal *x,TReal *para)
{
  TReal ret;
  TReal xm,ym,r;

  xm       = para[0];
  ym       = para[1];
  r        = para[2];
         // r = sqrt( (x-xm)^2 + (y-ym)^2 )
  ret = hypot(x[0]-xm,x[1]-ym) - r;

  return(ret);
}

TReal* d_circle_equation(TReal *x,TReal *para,TReal *df)
{
  TReal xm,ym,r;

  xm       = para[0];
  ym       = para[1];
  r        = para[2];

  df[0] = - (x[0]-xm)/hypot(x[0]-xm,x[1]-ym);  // df/dxm
  df[1] = - (x[1]-ym)/hypot(x[0]-xm,x[1]-ym);  // df/dym
  df[2] = -1.0;                                // df/dr

  return(df);
}

bool fitting_circle(double *X,double *Y,int count,double *xm,double *r,double *Qxx)
{
  Cfitting_it  fitting_it(gDebugLevel>=4,gConvergence);
  TReal   ubk[3];
//  TReal   Qxx[3*3];
  TReal **x;  // Parameter
  TReal  *y;  // abhängige Variable
  int     i;
  bool    ret;
  int     iter;
  TReal   convergence;

  fitting_it.coords_as_array(X,Y,NULL,count,&x,&y);

  /* approx. values */
  ubk[0] = ubk[1] = 0.0;
  ubk[2] = 1.0;

  ret = fitting_it.approximation(y,count,x,count,ubk,3,Qxx,
               &iter,&convergence,
               circle_equation,
               d_circle_equation
               );

  if (ret==true)
  {
    for (i=0;i<2;i++) xm[i] = ubk[i] ;
    *r = ubk[2];
  }
  fitting_it.free_array(x,y,count);
  return(ret);
}
