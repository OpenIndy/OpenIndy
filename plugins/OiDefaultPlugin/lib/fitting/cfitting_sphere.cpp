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

#include "cfitting_sphere.h"

#include  <math.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)

extern int    gDebugLevel;
extern double gConvergence;

/*************************************************/

TReal sphere_equation(TReal *x,TReal *para)
{
  TReal ret;
  TReal xm,ym,zm,r;

  xm       = para[0];
  ym       = para[1];
  zm       = para[2];
  r        = para[3];
         // r = sqrt( (x-xm)^2 + (y-ym)^2 )
  ret = hypot(hypot(x[0]-xm,x[1]-ym),x[2]-zm) - r;

  return(ret);
}

TReal* d_sphere_equation(TReal *x,TReal *para,TReal *df)
{
  TReal xm,ym,zm,r;
  TReal rh;

  xm       = para[0];
  ym       = para[1];
  zm       = para[2];
  r        = para[3];

  rh = hypot(hypot(x[0]-xm,x[1]-ym),x[2]-zm);

  df[0] = - (x[0]-xm)/rh;  // df/dxm
  df[1] = - (x[1]-ym)/rh;  // df/dym
  df[2] = - (x[2]-zm)/rh;  // df/dzm
  df[3] = -1.0;            // df/dr

  return(df);
}

bool fitting_sphere(double *X,double *Y,double *Z,int count,double *xm,double *r,double *Qxx)
{
  Cfitting_it  fitting_it(gDebugLevel>=4,gConvergence);
  TReal   ubk[4];
  TReal **x;  // Parameter
  TReal  *y;  // abhängige Variable
  int     i;
  bool    ret;
  int     iter;
  TReal   convergence;

  fitting_it.coords_as_array(X,Y,Z,count,&x,&y);
  /* approx. values */

  ubk[0] = ubk[1] = ubk[2] = 0.0;
  ubk[3] = 1.0;
  ret = fitting_it.approximation(y,count,x,count,ubk,4,Qxx,
               &iter,&convergence,
               sphere_equation,
               d_sphere_equation
               );

  if (ret==true)
  {
    for (i=0;i<3;i++) xm[i] = ubk[i] ;
    *r = ubk[3];
  }
  fitting_it.free_array(x,y,count);
  return(ret);
}
