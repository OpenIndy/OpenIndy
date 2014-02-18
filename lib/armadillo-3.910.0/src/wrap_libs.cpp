#include "armadillo_bits/config.hpp"
#include "armadillo_bits/typedef_blas_int.hpp"

#undef ARMA_USE_WRAPPER
#include "armadillo_bits/compiler_setup.hpp"

#include "armadillo_bits/undefine_conflicts.hpp"
#include "armadillo_bits/include_atlas.hpp"


namespace arma
{

#include "armadillo_bits/blas_bones.hpp"
#include "armadillo_bits/lapack_bones.hpp"

// at this stage we have prototypes for the real blas, lapack and atlas functions

// now we make the wrapper functions


extern "C"
  {
  #if defined(ARMA_USE_BLAS)
    
    float arma_fortran_prefix(arma_sdot)(blas_int* n, const float*  x, blas_int* incx, const float*  y, blas_int* incy)
      {
      return arma_fortran_noprefix(arma_sdot)(n, x, incx, y, incy);
      }
    
    double arma_fortran_prefix(arma_ddot)(blas_int* n, const double* x, blas_int* incx, const double* y, blas_int* incy)
      {
      return arma_fortran_noprefix(arma_ddot)(n, x, incx, y, incy);
      }
    
    
    
    void arma_fortran_prefix(arma_sgemv)(const char* transA, const blas_int* m, const blas_int* n, const float*  alpha, const float*  A, const blas_int* ldA, const float*  x, const blas_int* incx, const float*  beta, float*  y, const blas_int* incy)
      {
      arma_fortran_noprefix(arma_sgemv)(transA, m, n, alpha, A, ldA, x, incx, beta, y, incy);
      }
    
    void arma_fortran_prefix(arma_dgemv)(const char* transA, const blas_int* m, const blas_int* n, const double* alpha, const double* A, const blas_int* ldA, const double* x, const blas_int* incx, const double* beta, double* y, const blas_int* incy)
      {
      arma_fortran_noprefix(arma_dgemv)(transA, m, n, alpha, A, ldA, x, incx, beta, y, incy);
      }
    
    void arma_fortran_prefix(arma_cgemv)(const char* transA, const blas_int* m, const blas_int* n, const void*   alpha, const void*   A, const blas_int* ldA, const void*   x, const blas_int* incx, const void*   beta, void*   y, const blas_int* incy)
      {
      arma_fortran_noprefix(arma_cgemv)(transA, m, n, alpha, A, ldA, x, incx, beta, y, incy);
      }
    
    void arma_fortran_prefix(arma_zgemv)(const char* transA, const blas_int* m, const blas_int* n, const void*   alpha, const void*   A, const blas_int* ldA, const void*   x, const blas_int* incx, const void*   beta, void*   y, const blas_int* incy)
      {
      arma_fortran_noprefix(arma_zgemv)(transA, m, n, alpha, A, ldA, x, incx, beta, y, incy);
      }
    
    
    
    void arma_fortran_prefix(arma_sgemm)(const char* transA, const char* transB, const blas_int* m, const blas_int* n, const blas_int* k, const float*  alpha, const float*  A, const blas_int* ldA, const float*  B, const blas_int* ldB, const float*  beta, float*  C, const blas_int* ldC)
      {
      arma_fortran_noprefix(arma_sgemm)(transA, transB, m, n, k, alpha, A, ldA, B, ldB, beta, C, ldC);
      }
    
    void arma_fortran_prefix(arma_dgemm)(const char* transA, const char* transB, const blas_int* m, const blas_int* n, const blas_int* k, const double* alpha, const double* A, const blas_int* ldA, const double* B, const blas_int* ldB, const double* beta, double* C, const blas_int* ldC)
      {
      arma_fortran_noprefix(arma_dgemm)(transA, transB, m, n, k, alpha, A, ldA, B, ldB, beta, C, ldC);
      }
    
    void arma_fortran_prefix(arma_cgemm)(const char* transA, const char* transB, const blas_int* m, const blas_int* n, const blas_int* k, const void*   alpha, const void*   A, const blas_int* ldA, const void*   B, const blas_int* ldB, const void*   beta, void*   C, const blas_int* ldC)
      {
      arma_fortran_noprefix(arma_cgemm)(transA, transB, m, n, k, alpha, A, ldA, B, ldB, beta, C, ldC);
      }
    
    void arma_fortran_prefix(arma_zgemm)(const char* transA, const char* transB, const blas_int* m, const blas_int* n, const blas_int* k, const void*   alpha, const void*   A, const blas_int* ldA, const void*   B, const blas_int* ldB, const void*   beta, void*   C, const blas_int* ldC)
      {
      arma_fortran_noprefix(arma_zgemm)(transA, transB, m, n, k, alpha, A, ldA, B, ldB, beta, C, ldC);
      }
    
    
    
    void arma_fortran_prefix(arma_ssyrk)(const char* uplo, const char* transA, const blas_int* n, const blas_int* k, const  float* alpha, const  float* A, const blas_int* ldA, const  float* beta,  float* C, const blas_int* ldC)
      {
      arma_fortran_noprefix(arma_ssyrk)(uplo, transA, n, k, alpha, A, ldA, beta, C, ldC);
      }
    
    void arma_fortran_prefix(arma_dsyrk)(const char* uplo, const char* transA, const blas_int* n, const blas_int* k, const double* alpha, const double* A, const blas_int* ldA, const double* beta, double* C, const blas_int* ldC)
      {
      arma_fortran_noprefix(arma_dsyrk)(uplo, transA, n, k, alpha, A, ldA, beta, C, ldC);
      }
    
    
    
    void arma_fortran_prefix(arma_cherk)(const char* uplo, const char* transA, const blas_int* n, const blas_int* k, const  float* alpha, const void* A, const blas_int* ldA, const  float* beta, void* C, const blas_int* ldC)
      {
      arma_fortran_noprefix(arma_cherk)(uplo, transA, n, k, alpha, A, ldA, beta, C, ldC);
      }
    
    void arma_fortran_prefix(arma_zherk)(const char* uplo, const char* transA, const blas_int* n, const blas_int* k, const double* alpha, const void* A, const blas_int* ldA, const double* beta, void* C, const blas_int* ldC)
      {
      arma_fortran_noprefix(arma_zherk)(uplo, transA, n, k, alpha, A, ldA, beta, C, ldC);
      }
    
  #endif
  
  
  
  #if defined(ARMA_USE_LAPACK)
    
    void arma_fortran_prefix(arma_sgetrf)(blas_int* m, blas_int* n,  float* a, blas_int* lda, blas_int* ipiv, blas_int* info)
      {
      arma_fortran_noprefix(arma_sgetrf)(m, n, a, lda, ipiv, info);
      }
    
    void arma_fortran_prefix(arma_dgetrf)(blas_int* m, blas_int* n, double* a, blas_int* lda, blas_int* ipiv, blas_int* info)
      {
      arma_fortran_noprefix(arma_dgetrf)(m, n, a, lda, ipiv, info);
      }

    void arma_fortran_prefix(arma_cgetrf)(blas_int* m, blas_int* n,   void* a, blas_int* lda, blas_int* ipiv, blas_int* info)
      {
      arma_fortran_noprefix(arma_cgetrf)(m, n, a, lda, ipiv, info);
      }
    
    void arma_fortran_prefix(arma_zgetrf)(blas_int* m, blas_int* n,   void* a, blas_int* lda, blas_int* ipiv, blas_int* info)
      {
      arma_fortran_noprefix(arma_zgetrf)(m, n, a, lda, ipiv, info);
      }
    
    
    
    void arma_fortran_prefix(arma_sgetri)(blas_int* n,  float* a, blas_int* lda, blas_int* ipiv,  float* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_sgetri)(n, a, lda, ipiv, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_dgetri)(blas_int* n, double* a, blas_int* lda, blas_int* ipiv, double* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_dgetri)(n, a, lda, ipiv, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_cgetri)(blas_int* n,  void*  a, blas_int* lda, blas_int* ipiv,   void* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_cgetri)(n, a, lda, ipiv, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_zgetri)(blas_int* n,  void*  a, blas_int* lda, blas_int* ipiv,   void* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_zgetri)(n, a, lda, ipiv, work, lwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_strtri)(char* uplo, char* diag, blas_int* n,  float* a, blas_int* lda, blas_int* info)
      {
      arma_fortran_noprefix(arma_strtri)(uplo, diag, n, a, lda, info);
      }
    
    void arma_fortran_prefix(arma_dtrtri)(char* uplo, char* diag, blas_int* n, double* a, blas_int* lda, blas_int* info)
      {
      arma_fortran_noprefix(arma_dtrtri)(uplo, diag, n, a, lda, info);
      }
    
    void arma_fortran_prefix(arma_ctrtri)(char* uplo, char* diag, blas_int* n,   void* a, blas_int* lda, blas_int* info)
      {
      arma_fortran_noprefix(arma_ctrtri)(uplo, diag, n, a, lda, info);
      }
    
    void arma_fortran_prefix(arma_ztrtri)(char* uplo, char* diag, blas_int* n,   void* a, blas_int* lda, blas_int* info)
      {
      arma_fortran_noprefix(arma_ztrtri)(uplo, diag, n, a, lda, info);
      }
    
    
    
    void arma_fortran_prefix(arma_ssyev)(char* jobz, char* uplo, blas_int* n,  float* a, blas_int* lda,  float* w,  float* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_ssyev)(jobz, uplo, n, a, lda, w, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_dsyev)(char* jobz, char* uplo, blas_int* n, double* a, blas_int* lda, double* w, double* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_dsyev)(jobz, uplo, n, a, lda, w, work, lwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_cheev)(char* jobz, char* uplo, blas_int* n,   void* a, blas_int* lda,  float* w,   void* work, blas_int* lwork,  float* rwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_cheev)(jobz, uplo, n, a, lda, w, work, lwork, rwork, info);
      }
    
    void arma_fortran_prefix(arma_zheev)(char* jobz, char* uplo, blas_int* n,   void* a, blas_int* lda, double* w,   void* work, blas_int* lwork, double* rwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_zheev)(jobz, uplo, n, a, lda, w, work, lwork, rwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_ssyevd)(char* jobz, char* uplo, blas_int* n,  float* a, blas_int* lda,  float* w,  float* work, blas_int* lwork, blas_int* iwork, blas_int* liwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_ssyevd)(jobz, uplo, n, a, lda, w, work, lwork, iwork, liwork, info);
      }
    
    void arma_fortran_prefix(arma_dsyevd)(char* jobz, char* uplo, blas_int* n, double* a, blas_int* lda, double* w, double* work, blas_int* lwork, blas_int* iwork, blas_int* liwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_dsyevd)(jobz, uplo, n, a, lda, w, work, lwork, iwork, liwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_cheevd)(char* jobz, char* uplo, blas_int* n,   void* a, blas_int* lda,  float* w,   void* work, blas_int* lwork,  float* rwork, blas_int* lrwork, blas_int* iwork, blas_int* liwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_cheevd)(jobz, uplo, n, a, lda, w, work, lwork, rwork, lrwork, iwork, liwork, info);
      }
    
    void arma_fortran_prefix(arma_zheevd)(char* jobz, char* uplo, blas_int* n,   void* a, blas_int* lda, double* w,   void* work, blas_int* lwork, double* rwork, blas_int* lrwork, blas_int* iwork, blas_int* liwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_zheevd)(jobz, uplo, n, a, lda, w, work, lwork, rwork, lrwork, iwork, liwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_sgeev)(char* jobvl, char* jobvr, blas_int* n,  float* a, blas_int* lda,  float* wr,  float* wi,  float* vl, blas_int* ldvl,  float* vr, blas_int* ldvr,  float* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_sgeev)(jobvl, jobvr, n, a, lda, wr, wi, vl, ldvl, vr, ldvr, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_dgeev)(char* jobvl, char* jobvr, blas_int* n, double* a, blas_int* lda, double* wr, double* wi, double* vl, blas_int* ldvl, double* vr, blas_int* ldvr, double* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_dgeev)(jobvl, jobvr, n, a, lda, wr, wi, vl, ldvl, vr, ldvr, work, lwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_cgeev)(char* jobvl, char* jobvr, blas_int* n, void* a, blas_int* lda, void* w, void* vl, blas_int* ldvl, void* vr, blas_int* ldvr, void* work, blas_int* lwork,  float* rwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_cgeev)(jobvl, jobvr, n, a, lda, w, vl, ldvl, vr, ldvr, work, lwork, rwork, info);
      }
    
    void arma_fortran_prefix(arma_zgeev)(char* jobvl, char* jobvr, blas_int* n, void* a, blas_int* lda, void* w, void* vl, blas_int* ldvl, void* vr, blas_int* ldvr, void* work, blas_int* lwork, double* rwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_zgeev)(jobvl, jobvr, n, a, lda, w, vl, ldvl, vr, ldvr, work, lwork, rwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_spotrf)(char* uplo, blas_int* n,  float* a, blas_int* lda, blas_int* info)
      {
      arma_fortran_noprefix(arma_spotrf)(uplo, n, a, lda, info);
      }
    
    void arma_fortran_prefix(arma_dpotrf)(char* uplo, blas_int* n, double* a, blas_int* lda, blas_int* info)
      {
      arma_fortran_noprefix(arma_dpotrf)(uplo, n, a, lda, info);
      }
    
    void arma_fortran_prefix(arma_cpotrf)(char* uplo, blas_int* n,   void* a, blas_int* lda, blas_int* info)
      {
      arma_fortran_noprefix(arma_cpotrf)(uplo, n, a, lda, info);
      }
    
    void arma_fortran_prefix(arma_zpotrf)(char* uplo, blas_int* n,   void* a, blas_int* lda, blas_int* info)
      {
      arma_fortran_noprefix(arma_zpotrf)(uplo, n, a, lda, info);
      }
    
    
    
    void arma_fortran_prefix(arma_spotri)(char* uplo, blas_int* n,  float* a, blas_int* lda, blas_int* info)
      {
      arma_fortran_noprefix(arma_spotri)(uplo, n, a, lda, info);
      }
    
    void arma_fortran_prefix(arma_dpotri)(char* uplo, blas_int* n, double* a, blas_int* lda, blas_int* info)
      {
      arma_fortran_noprefix(arma_dpotri)(uplo, n, a, lda, info);
      }
    
    void arma_fortran_prefix(arma_cpotri)(char* uplo, blas_int* n,   void* a, blas_int* lda, blas_int* info)
      {
      arma_fortran_noprefix(arma_cpotri)(uplo, n, a, lda, info);
      }
    
    void arma_fortran_prefix(arma_zpotri)(char* uplo, blas_int* n,   void* a, blas_int* lda, blas_int* info)
      {
      arma_fortran_noprefix(arma_zpotri)(uplo, n, a, lda, info);
      }
    
    
    
    void arma_fortran_prefix(arma_sgeqrf)(blas_int* m, blas_int* n,  float* a, blas_int* lda,  float* tau,  float* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_sgeqrf)(m, n, a, lda, tau, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_dgeqrf)(blas_int* m, blas_int* n, double* a, blas_int* lda, double* tau, double* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_dgeqrf)(m, n, a, lda, tau, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_cgeqrf)(blas_int* m, blas_int* n,   void* a, blas_int* lda,   void* tau,   void* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_cgeqrf)(m, n, a, lda, tau, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_zgeqrf)(blas_int* m, blas_int* n,   void* a, blas_int* lda,   void* tau,   void* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_zgeqrf)(m, n, a, lda, tau, work, lwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_sorgqr)(blas_int* m, blas_int* n, blas_int* k,  float* a, blas_int* lda,  float* tau,  float* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_sorgqr)(m, n, k, a, lda, tau, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_dorgqr)(blas_int* m, blas_int* n, blas_int* k, double* a, blas_int* lda, double* tau, double* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_dorgqr)(m, n, k, a, lda, tau, work, lwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_cungqr)(blas_int* m, blas_int* n, blas_int* k,   void* a, blas_int* lda,   void* tau,   void* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_cungqr)(m, n, k, a, lda, tau, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_zungqr)(blas_int* m, blas_int* n, blas_int* k,   void* a, blas_int* lda,   void* tau,   void* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_zungqr)(m, n, k, a, lda, tau, work, lwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_sgesvd)(char* jobu, char* jobvt, blas_int* m, blas_int* n, float*  a, blas_int* lda, float*  s, float*  u, blas_int* ldu, float*  vt, blas_int* ldvt, float*  work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_sgesvd)(jobu, jobvt, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_dgesvd)(char* jobu, char* jobvt, blas_int* m, blas_int* n, double* a, blas_int* lda, double* s, double* u, blas_int* ldu, double* vt, blas_int* ldvt, double* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_dgesvd)(jobu, jobvt, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_cgesvd)(char* jobu, char* jobvt, blas_int* m, blas_int* n, void*   a, blas_int* lda, float*  s, void*   u, blas_int* ldu, void*   vt, blas_int* ldvt, void*   work, blas_int* lwork, float*  rwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_cgesvd)(jobu, jobvt, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, rwork, info);
      }
    
    void arma_fortran_prefix(arma_zgesvd)(char* jobu, char* jobvt, blas_int* m, blas_int* n, void*   a, blas_int* lda, double* s, void*   u, blas_int* ldu, void*   vt, blas_int* ldvt, void*   work, blas_int* lwork, double* rwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_zgesvd)(jobu, jobvt, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, rwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_sgesdd)(char* jobz, blas_int* m, blas_int* n, float*  a, blas_int* lda, float*  s, float*  u, blas_int* ldu, float*  vt, blas_int* ldvt, float*  work, blas_int* lwork, blas_int* iwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_sgesdd)(jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, iwork, info);
      }
    
    void arma_fortran_prefix(arma_dgesdd)(char* jobz, blas_int* m, blas_int* n, double* a, blas_int* lda, double* s, double* u, blas_int* ldu, double* vt, blas_int* ldvt, double* work, blas_int* lwork, blas_int* iwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_dgesdd)(jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, iwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_cgesdd)(char* jobz, blas_int* m, blas_int* n, void* a, blas_int* lda, float*  s, void* u, blas_int* ldu, void* vt, blas_int* ldvt, void* work, blas_int* lwork, float*  rwork, blas_int* iwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_cgesdd)(jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, rwork, iwork, info);
      }
    
    void arma_fortran_prefix(arma_zgesdd)(char* jobz, blas_int* m, blas_int* n, void* a, blas_int* lda, double* s, void* u, blas_int* ldu, void* vt, blas_int* ldvt, void* work, blas_int* lwork, double* rwork, blas_int* iwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_zgesdd)(jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, rwork, iwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_sgesv)(blas_int* n, blas_int* nrhs, float*  a, blas_int* lda, blas_int* ipiv, float*  b, blas_int* ldb, blas_int* info)
      {
      arma_fortran_noprefix(arma_sgesv)(n, nrhs, a, lda, ipiv, b, ldb, info);
      }
    
    void arma_fortran_prefix(arma_dgesv)(blas_int* n, blas_int* nrhs, double* a, blas_int* lda, blas_int* ipiv, double* b, blas_int* ldb, blas_int* info)
      {
      arma_fortran_noprefix(arma_dgesv)(n, nrhs, a, lda, ipiv, b, ldb, info);
      }
    
    void arma_fortran_prefix(arma_cgesv)(blas_int* n, blas_int* nrhs, void*   a, blas_int* lda, blas_int* ipiv, void*   b, blas_int* ldb, blas_int* info)
      {
      arma_fortran_noprefix(arma_cgesv)(n, nrhs, a, lda, ipiv, b, ldb, info);
      }
    
    void arma_fortran_prefix(arma_zgesv)(blas_int* n, blas_int* nrhs, void*   a, blas_int* lda, blas_int* ipiv, void*   b, blas_int* ldb, blas_int* info)
      {
      arma_fortran_noprefix(arma_zgesv)(n, nrhs, a, lda, ipiv, b, ldb, info);
      }
    
    
    
    void arma_fortran_prefix(arma_sgels)(char* trans, blas_int* m, blas_int* n, blas_int* nrhs, float*  a, blas_int* lda, float*  b, blas_int* ldb, float*  work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_sgels)(trans, m, n, nrhs, a, lda, b, ldb, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_dgels)(char* trans, blas_int* m, blas_int* n, blas_int* nrhs, double* a, blas_int* lda, double* b, blas_int* ldb, double* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_dgels)(trans, m, n, nrhs, a, lda, b, ldb, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_cgels)(char* trans, blas_int* m, blas_int* n, blas_int* nrhs, void*   a, blas_int* lda, void*   b, blas_int* ldb, void*   work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_cgels)(trans, m, n, nrhs, a, lda, b, ldb, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_zgels)(char* trans, blas_int* m, blas_int* n, blas_int* nrhs, void*   a, blas_int* lda, void*   b, blas_int* ldb, void*   work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_zgels)(trans, m, n, nrhs, a, lda, b, ldb, work, lwork, info);
      }
    
    
    
    
    void arma_fortran_prefix(arma_strtrs)(char* uplo, char* trans, char* diag, blas_int* n, blas_int* nrhs, const float*  a, blas_int* lda, float*  b, blas_int* ldb, blas_int* info)
      {
      arma_fortran_noprefix(arma_strtrs)(uplo, trans, diag, n, nrhs, a, lda, b, ldb, info);
      }
    
    void arma_fortran_prefix(arma_dtrtrs)(char* uplo, char* trans, char* diag, blas_int* n, blas_int* nrhs, const double* a, blas_int* lda, double* b, blas_int* ldb, blas_int* info)
      {
      arma_fortran_noprefix(arma_dtrtrs)(uplo, trans, diag, n, nrhs, a, lda, b, ldb, info);
      }
    
    void arma_fortran_prefix(arma_ctrtrs)(char* uplo, char* trans, char* diag, blas_int* n, blas_int* nrhs, const void*   a, blas_int* lda, void*   b, blas_int* ldb, blas_int* info)
      {
      arma_fortran_noprefix(arma_ctrtrs)(uplo, trans, diag, n, nrhs, a, lda, b, ldb, info);
      }
    
    void arma_fortran_prefix(arma_ztrtrs)(char* uplo, char* trans, char* diag, blas_int* n, blas_int* nrhs, const void*   a, blas_int* lda, void*   b, blas_int* ldb, blas_int* info)
      {
      arma_fortran_noprefix(arma_ztrtrs)(uplo, trans, diag, n, nrhs, a, lda, b, ldb, info);
      }
    
    
    
    
    void arma_fortran_prefix(arma_sgees)(char* jobvs, char* sort, blas_int* select, blas_int* n, float*  a, blas_int* lda, blas_int* sdim, float*  wr, float*  wi, float*  vs, blas_int* ldvs, float*  work, blas_int* lwork, blas_int* bwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_sgees)(jobvs, sort, select, n, a, lda, sdim, wr, wi, vs, ldvs, work, lwork, bwork, info);
      }
      
    void arma_fortran_prefix(arma_dgees)(char* jobvs, char* sort, blas_int* select, blas_int* n, double* a, blas_int* lda, blas_int* sdim, double* wr, double* wi, double* vs, blas_int* ldvs, double* work, blas_int* lwork, blas_int* bwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_dgees)(jobvs, sort, select, n, a, lda, sdim, wr, wi, vs, ldvs, work, lwork, bwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_cgees)(char* jobvs, char* sort, blas_int* select, blas_int* n, void* a, blas_int* lda, blas_int* sdim, void* w, void* vs, blas_int* ldvs, void* work, blas_int* lwork, float*  rwork, blas_int* bwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_cgees)(jobvs, sort, select, n, a, lda, sdim, w, vs, ldvs, work, lwork, rwork, bwork, info);
      }
    
    void arma_fortran_prefix(arma_zgees)(char* jobvs, char* sort, blas_int* select, blas_int* n, void* a, blas_int* lda, blas_int* sdim, void* w, void* vs, blas_int* ldvs, void* work, blas_int* lwork, double* rwork, blas_int* bwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_zgees)(jobvs, sort, select, n, a, lda, sdim, w, vs, ldvs, work, lwork, rwork, bwork, info);
      }
    
    
    
    void arma_fortran_prefix(arma_strsyl)(char* transa, char* transb, blas_int* isgn, blas_int* m, blas_int* n, const float*  a, blas_int* lda, const float*  b, blas_int* ldb, float*  c, blas_int* ldc, float*  scale, blas_int* info)
      {
      arma_fortran_noprefix(arma_strsyl)(transa, transb, isgn, m, n, a, lda, b, ldb, c, ldc, scale, info);
      }
    
    void arma_fortran_prefix(arma_dtrsyl)(char* transa, char* transb, blas_int* isgn, blas_int* m, blas_int* n, const double* a, blas_int* lda, const double* b, blas_int* ldb, double* c, blas_int* ldc, double* scale, blas_int* info)
      {
      arma_fortran_noprefix(arma_dtrsyl)(transa, transb, isgn, m, n, a, lda, b, ldb, c, ldc, scale, info);
      }
    
    void arma_fortran_prefix(arma_ctrsyl)(char* transa, char* transb, blas_int* isgn, blas_int* m, blas_int* n, const void*   a, blas_int* lda, const void*   b, blas_int* ldb, void*   c, blas_int* ldc, float*  scale, blas_int* info)
      {
      arma_fortran_noprefix(arma_ctrsyl)(transa, transb, isgn, m, n, a, lda, b, ldb, c, ldc, scale, info);
      }
    
    void arma_fortran_prefix(arma_ztrsyl)(char* transa, char* transb, blas_int* isgn, blas_int* m, blas_int* n, const void*   a, blas_int* lda, const void*   b, blas_int* ldb, void*   c, blas_int* ldc, double* scale, blas_int* info)
      {
      arma_fortran_noprefix(arma_ztrsyl)(transa, transb, isgn, m, n, a, lda, b, ldb, c, ldc, scale, info);
      }
    
    
    
    
    void arma_fortran_prefix(arma_ssytrf)(char* uplo, blas_int* n, float*  a, blas_int* lda, blas_int* ipiv, float*  work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_ssytrf)(uplo, n, a, lda, ipiv, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_dsytrf)(char* uplo, blas_int* n, double* a, blas_int* lda, blas_int* ipiv, double* work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_dsytrf)(uplo, n, a, lda, ipiv, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_csytrf)(char* uplo, blas_int* n, void*   a, blas_int* lda, blas_int* ipiv, void*   work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_csytrf)(uplo, n, a, lda, ipiv, work, lwork, info);
      }
    
    void arma_fortran_prefix(arma_zsytrf)(char* uplo, blas_int* n, void*   a, blas_int* lda, blas_int* ipiv, void*   work, blas_int* lwork, blas_int* info)
      {
      arma_fortran_noprefix(arma_zsytrf)(uplo, n, a, lda, ipiv, work, lwork, info);
      }
    
    
    
    
    void arma_fortran_prefix(arma_ssytri)(char* uplo, blas_int* n, float*  a, blas_int* lda, blas_int* ipiv, float*  work, blas_int* info)
      {
      arma_fortran_noprefix(arma_ssytri)(uplo, n, a, lda, ipiv, work, info);
      }
    
    void arma_fortran_prefix(arma_dsytri)(char* uplo, blas_int* n, double* a, blas_int* lda, blas_int* ipiv, double* work, blas_int* info)
      {
      arma_fortran_noprefix(arma_dsytri)(uplo, n, a, lda, ipiv, work, info);
      }
    
    void arma_fortran_prefix(arma_csytri)(char* uplo, blas_int* n, void*   a, blas_int* lda, blas_int* ipiv, void*   work, blas_int* info)
      {
      arma_fortran_noprefix(arma_csytri)(uplo, n, a, lda, ipiv, work, info);
      }
    
    void arma_fortran_prefix(arma_zsytri)(char* uplo, blas_int* n, void*   a, blas_int* lda, blas_int* ipiv, void*   work, blas_int* info)
      {
      arma_fortran_noprefix(arma_zsytri)(uplo, n, a, lda, ipiv, work, info);
      }
    
  #endif
  
  
  
  #if defined(ARMA_USE_ATLAS)
    
    float wrapper_cblas_sdot(const int N, const float  *X, const int incX, const float  *Y, const int incY)
      {
      return      cblas_sdot(N, X, incX, Y, incY);
      }
    
    double wrapper_cblas_ddot(const int N, const double *X, const int incX, const double *Y, const int incY)
      {
      return       cblas_ddot(N, X, incX, Y, incY);
      }
    
    void wrapper_cblas_cdotu_sub(const int N, const void *X, const int incX, const void *Y, const int incY, void *dotu)
      {
                 cblas_cdotu_sub(N, X, incX, Y, incY, dotu);
      }
    
    void wrapper_cblas_zdotu_sub(const int N, const void *X, const int incX, const void *Y, const int incY, void *dotu)
      {
                 cblas_zdotu_sub(N, X, incX, Y, incY, dotu);
      }
    
    
    
    void wrapper_cblas_sgemv(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const int M, const int N, const float alpha,
                             const float *A, const int lda, const float *X, const int incX, const float beta, float *Y, const int incY)
      {
                 cblas_sgemv(Order, TransA, M, N, alpha, A, lda, X, incX, beta, Y, incY);
      }
    
    void wrapper_cblas_dgemv(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const int M, const int N, const double alpha,
                             const double *A, const int lda, const double *X, const int incX, const double beta, double *Y, const int incY)
      {
                 cblas_dgemv(Order, TransA, M, N, alpha, A, lda, X, incX, beta, Y, incY);
      }
    
    void wrapper_cblas_cgemv(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const int M, const int N, const void *alpha,
                             const void *A, const int lda, const void *X, const int incX, const void *beta, void *Y, const int incY)
      {
                 cblas_cgemv(Order, TransA, M, N, alpha, A, lda, X, incX, beta, Y, incY);
      }
    
    void wrapper_cblas_zgemv(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const int M, const int N, const void *alpha,
                             const void *A, const int lda, const void *X, const int incX, const void *beta, void *Y, const int incY)
      {
                 cblas_zgemv(Order, TransA, M, N, alpha, A, lda, X, incX, beta, Y, incY);
      }
    
    
    
    void wrapper_cblas_sgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const enum CBLAS_TRANSPOSE TransB,
                             const int M, const int N, const int K, const float alpha,
                             const float *A, const int lda, const float *B, const int ldb, const float beta, float *C, const int ldc)
      {
                 cblas_sgemm(Order, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
      }
    
    void wrapper_cblas_dgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const enum CBLAS_TRANSPOSE TransB,
                             const int M, const int N, const int K, const double alpha,
                             const double *A, const int lda, const double *B, const int ldb, const double beta, double *C, const int ldc)
      {
                 cblas_dgemm(Order, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
      }
    
    void wrapper_cblas_cgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const enum CBLAS_TRANSPOSE TransB,
                             const int M, const int N, const int K, const void *alpha,
                             const void *A, const int lda, const void *B, const int ldb, const void *beta, void *C, const int ldc)
      {
                 cblas_cgemm(Order, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
      }
    
    void wrapper_cblas_zgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const enum CBLAS_TRANSPOSE TransB,
                             const int M, const int N, const int K, const void *alpha,
                             const void *A, const int lda, const void *B, const int ldb, const void *beta, void *C, const int ldc)
      {
                 cblas_zgemm(Order, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
      }
    
    
    
    void wrapper_cblas_ssyrk(const enum CBLAS_ORDER Order, const enum CBLAS_UPLO Uplo, const enum CBLAS_TRANSPOSE Trans,
                             const int N, const int K, const float alpha,
                             const float *A, const int lda, const float beta, float *C, const int ldc)
      {
                 cblas_ssyrk(Order, Uplo, Trans, N, K, alpha, A, lda, beta, C, ldc);
      }
    
    void wrapper_cblas_dsyrk(const enum CBLAS_ORDER Order, const enum CBLAS_UPLO Uplo, const enum CBLAS_TRANSPOSE Trans,
                             const int N, const int K, const double alpha,
                             const double *A, const int lda, const double beta, double *C, const int ldc)
      {
                 cblas_dsyrk(Order, Uplo, Trans, N, K, alpha, A, lda, beta, C, ldc);
      }
    
    
    
    void wrapper_cblas_cherk(const enum CBLAS_ORDER Order, const enum CBLAS_UPLO Uplo, const enum CBLAS_TRANSPOSE Trans,
                             const int N, const int K, const float alpha,
                             const void *A, const int lda, const float beta, void *C, const int ldc)
      {
                 cblas_cherk(Order, Uplo, Trans, N, K, alpha, A, lda, beta, C, ldc);
      }
    
    void wrapper_cblas_zherk(const enum CBLAS_ORDER Order, const enum CBLAS_UPLO Uplo, const enum CBLAS_TRANSPOSE Trans,
                             const int N, const int K, const double alpha,
                             const void *A, const int lda, const double beta, void *C, const int ldc)
      {
                 cblas_zherk(Order, Uplo, Trans, N, K, alpha, A, lda, beta, C, ldc);
      }
    
    
    
    int wrapper_clapack_sgetrf(const enum CBLAS_ORDER Order, const int M, const int N, float  *A, const int lda, int *ipiv)
      {
      return    clapack_sgetrf(Order, M, N, A, lda, ipiv);
      }
    
    int wrapper_clapack_dgetrf(const enum CBLAS_ORDER Order, const int M, const int N, double *A, const int lda, int *ipiv)
      {
      return    clapack_dgetrf(Order, M, N, A, lda, ipiv);
      }
    
    int wrapper_clapack_cgetrf(const enum CBLAS_ORDER Order, const int M, const int N, void   *A, const int lda, int *ipiv)
      {
      return    clapack_cgetrf(Order, M, N, A, lda, ipiv);
      }
    
    int wrapper_clapack_zgetrf(const enum CBLAS_ORDER Order, const int M, const int N, void   *A, const int lda, int *ipiv)
      {
      return    clapack_zgetrf(Order, M, N, A, lda, ipiv);
      }
    
    
    
    int wrapper_clapack_sgetri(const enum CBLAS_ORDER Order, const int N, float  *A, const int lda, const int *ipiv)
      {
      return    clapack_sgetri(Order, N, A, lda, ipiv);
      }
    
    int wrapper_clapack_dgetri(const enum CBLAS_ORDER Order, const int N, double *A, const int lda, const int *ipiv)
      {
      return    clapack_dgetri(Order, N, A, lda, ipiv);
      }
    
    int wrapper_clapack_cgetri(const enum CBLAS_ORDER Order, const int N, void   *A, const int lda, const int *ipiv)
      {
      return    clapack_cgetri(Order, N, A, lda, ipiv);
      }
    
    int wrapper_clapack_zgetri(const enum CBLAS_ORDER Order, const int N, void   *A, const int lda, const int *ipiv)
      {
      return    clapack_zgetri(Order, N, A, lda, ipiv);
      }
    
    
    
    int wrapper_clapack_sgesv(const enum CBLAS_ORDER Order, const int N, const int NRHS, float  *A, const int lda, int *ipiv, float  *B, const int ldb)
      {
      return    clapack_sgesv(Order, N, NRHS, A, lda, ipiv, B, ldb);
      }
    
    int wrapper_clapack_dgesv(const enum CBLAS_ORDER Order, const int N, const int NRHS, double *A, const int lda, int *ipiv, double *B, const int ldb)
      {
      return    clapack_dgesv(Order, N, NRHS, A, lda, ipiv, B, ldb);
      }
    
    int wrapper_clapack_cgesv(const enum CBLAS_ORDER Order, const int N, const int NRHS, void   *A, const int lda, int *ipiv, void   *B, const int ldb)
      {
      return    clapack_cgesv(Order, N, NRHS, A, lda, ipiv, B, ldb);
      }
    
    int wrapper_clapack_zgesv(const enum CBLAS_ORDER Order, const int N, const int NRHS, void   *A, const int lda, int *ipiv, void   *B, const int ldb)
      {
      return    clapack_zgesv(Order, N, NRHS, A, lda, ipiv, B, ldb);
      }
    
  #endif
  }

}
