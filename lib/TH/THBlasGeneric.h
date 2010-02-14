const struct THBlasAPI
{
    /* Level 1 */
    void (*swap)(long n, real *x, long incx, real *y, long incy);
    void (*scal)(long n, real a, real *x, long incx);
    void (*copy)(long n, real *x, long incx, real *y, long incy);
    void (*axpy)(long n, real a, real *x, long incx, real *y, long incy);
    real (*dot)(long n, real *x, long incx, real *y, long incy);
    
    /* Level 2 */
    void (*gemv)(char trans, long m, long n, real alpha, real *a, long lda, real *x, long incx, real beta, real *y, long incy);
    void (*ger)(long m, long n, real alpha, real *x, long incx, real *y, long incy, real *a, long lda);
    
    /* Level 3 */
    void (*gemm)(char transa, char transb, long m, long n, long k, real alpha, real *a, long lda, real *b, long ldb, real beta, real *c, long ldc);
    
} THBlasAPI;
