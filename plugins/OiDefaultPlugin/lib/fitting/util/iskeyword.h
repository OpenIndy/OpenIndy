/*
  X d e s y
  ---------

  Modul:    iskeyword.h
  Version:  1.0
  Datum:    14.02.2007
  Autor:    F. Kern

  Beschreibung:

  Position innerhalb einer Schlüsselwort-Liste bestimmen

  fkern@xdesy.de
  www.xdesy.de
*/
//---------------------------------------------------------------------------
#ifndef iskeywordH
#define iskeywordH
//---------------------------------------------------------------------------

int isKeyword(char *str,char **keys);
int isKeywordAlternativen(char *str,char **keys);
int isKeywordTrueFalse(char *str);

#endif
