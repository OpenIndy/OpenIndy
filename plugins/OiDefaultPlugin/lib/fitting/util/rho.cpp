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
/*
  X d e s y
  ---------

  Modul:    rho.cpp
  Version:  1.0
  Datum:    14.02.2007
  Autor:    F. Kern

  Beschreibung:

  Konstanten zur Umrechnung in versch. Winkeleinheiten

  fkern@xdesy.de
  www.xdesy.de
*/
//---------------------------------------------------------------------------
//#include <vcl.h>
#pragma hdrstop

#include "rho.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

#include <math.h>

double gRho200 =  M_PI / 200.0;
double g200Rho = 200.0 / M_PI;;
double gRho180 =  M_PI / 180.0;
double g180Rho = 180.0 / M_PI;
