//---------------------------------------------------------------------------

#ifndef cfitting_matrixH
#define cfitting_matrixH
//---------------------------------------------------------------------------

#include "cfitting_util.h"

#define MIT_MATRIXOUTPUT
#define MIT_MATRIXINV4

//#undef MIT_MATRIXOUTPUT
//#undef MIT_MATRIXINV4

//#define MIT_MATRIX_A_PLUS_B
//#define MIT_MATRIX_SKALAR_MAL_A
//#define MIT_MATRIX_A_MAL_B_SUM

#define MIT_MATRIX_A_MAL_B
#define MIT_MATRIX_AT_MAL_B
//#define MIT_MATRIX_A_MAL_BT



TReal **matrix(int nr,int nc);
void free_matrix(TReal **m,int nr,int nc);

#ifdef MIT_MATRIXOUTPUT
void MatrixOut(TReal **a,int z,int s);
void VektorOut(TReal *a,int z);
void MatrixOutMatlab(char *name,TReal **a,int z,int s);
#endif

#ifdef MIT_MATRIX_A_PLUS_B
void a_plus_b(TReal **a,TReal **b,TReal **c,int s,int z);
#endif

#ifdef MIT_MATRIX_SKALAR_MAL_A
void skalar_mal_a(TReal **a,TReal skalar,TReal **c,int s,int z);
#endif

#ifdef MIT_MATRIX_A_MAL_B
void a_mal_b(TReal **a,int z1,int s1,TReal **b,int z2,int s2,TReal **c);
#endif

#ifdef MIT_MATRIX_A_MAL_B_SUM
void a_mal_b_sum(TReal **a,int z1,int s1,TReal **b,int z2,int s2,TReal *c);
#endif

#ifdef MIT_MATRIX_AT_MAL_B
void at_mal_b(TReal **a,int z1,int s1,TReal **b,int z2,int s2,TReal **c);
#endif

#ifdef MIT_MATRIX_A_MAL_BT
void a_mal_bt(TReal **a,int z1,int s1,TReal **b,int z2,int s2,TReal **c);
#endif

#ifdef MIT_MATRIXINV4
int a_inv4(TReal **a,int z,int s,TReal **b);
#endif




#endif
