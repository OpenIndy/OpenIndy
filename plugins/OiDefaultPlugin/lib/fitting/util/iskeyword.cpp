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

  Modul:    iskeyword.cpp
  Version:  1.0
  Datum:    14.02.2007
  Autor:    F. Kern

  Beschreibung:

  Position innerhalb einer Schlüsselwort-Liste bestimmen

  fkern@xdesy.de
  www.xdesy.de
*/
//---------------------------------------------------------------------------
//#include <vcl.h>
#pragma hdrstop

#include <stdlib.h>
#include <string.h>

#include "iskeyword.h"
#include "mystring.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

static char *keywFalse[]={"0",
                  "false","f",
                  "falsch",
                  "nein","n",
                  "no",
                  NULL
                  };
static char *keywTrue[]={"1",
                  "true","t",
                  "wahr",
                  "ja","j",
                  "yes","y",
                  NULL
                  };


int isKeyword(char *str,char **keys)
{
  int i;

  i = 0;
  while(keys[i]!=NULL && strcmp(str,keys[i])) i++;
  return(keys[i]==NULL ? -1 : i);
}


int isKeywordTrueFalse(char *str)
{
 if (isKeyword(str,keywTrue)!=-1) return(1);
 if (isKeyword(str,keywFalse)!=-1) return(0);
 return(-1);
}


int isKeywordAlternativen(char *str,char **keys)
{
  int    i,n;
  size_t l;
  char *pstr;
  char  mstr[256];

  i = 0;
  pstr = NULL;
  while(keys[i]!=NULL && pstr==NULL)
  {
    strcpy(mstr,keys[i]);
    pstr = mystrtok(mstr,"|");
    while (pstr!=NULL && (n=strncmp(str,pstr,l=strlen(pstr))))
    {
      pstr = mystrtok(NULL,"|");
    }
    if (pstr==NULL) i++;
  }
  return(pstr==NULL ? -1 : i);
}
