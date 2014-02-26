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

#include "mystring.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#include <string.h>

char *strtrim(char *str)
{
  char *p;
  int   n;

  p = str;
  while (*p!='\x0' && (*p==' ' || *p=='\t'))
  {
    p++;
  }
/*
  n = strlen(p) - 1;
  while((p+n)>=p && (*(p+n)==' ' || *(p+n)=='\n' || *(p+n)=='\t'))
  {
    *(p+n) = '\x0';
    n--;
  }
*/
  n = strlen(p) - 1;
  while(n>=0 && (*(p+n)==' ' || *(p+n)=='\n' || *(p+n)=='\t'))
  {
    *(p+n) = '\x0';
    n--;
  }
  strcpy(str,p);
  return(str);
}

char *mystrtok(char *str,char *muster)
{
  static char *merkestr;
  char *pstr,*sstr;
  char *mstr;

  if (str!=NULL)
  {
    pstr = str;
  }
  else
  {
    pstr = merkestr;
  }
  if (pstr==NULL) return(NULL);
  sstr = pstr;
  while (*pstr!='\x0')
  {
    mstr = muster;
    while(*mstr!='\x0')
    {
      if (*pstr==*mstr) break;
      mstr++;
    }
    if (*mstr!='\x0')
    {
      *pstr = '\x0';
      merkestr = pstr+1;
      return(sstr);
    }
    pstr++;
  }
  if (pstr!=sstr)
  {
    merkestr = NULL;
    return(sstr);
  }
  return(NULL);
}
