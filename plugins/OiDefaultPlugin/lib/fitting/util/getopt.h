/*
  X d e s y
  ---------

  Modul:    Getopt.h
  Version:  1.0
  Datum:    15.12.2008
  Autor:    F. Kern

  Beschreibung:

  Verarbeitung von Aufrufparametern


  fkern@xdesy.de
  www.xdesy.de
*/

#ifndef __TURBOC__
  #define getopt Xgetopt
#endif
extern void init_getopt(int opti,int opte,char optm);
extern char getopt(int argc,char *argv[],char *optstring,char **optarg);
