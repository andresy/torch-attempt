#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define THEDOT ddot_
/* #define SIZE 2147483653L */
/* #define SIZE 19 */
/* #define SIZE 2147483648 */
/* #define SIZE (INT_MAX+1L) */
#define SIZE (INT_MAX)

double l[] = {
  -1, -2, -1, -1
};

double d[] = {
  2, 2, 3, 3, 1
};

double u[] = {
  -1, -1, -1, -2
};

double x[] = {
  1, 2, 3, 2, 1
};

static double ddotxxx(long n, double *x, long incx, double *y, long incy)
{
  extern double THEDOT(const long *n, double *x, const long *incx, double *y, const long *incy);
  double res = 666;
  res = THEDOT(&n, x, &incx, y, &incy);
  return res;
}

static double sdot(long n, float *x, long incx, float *y, long incy)
{
  extern float sdot_(const long *n, float *x, const long *incx, float *y, const long *incy);
  float res = 666;
  res = sdot_(&n, x, &incx, y, &incy);
  return res;
}

static long
dgtsv(long N, long NRHS, double *DL, double *D, double *DU, double *B,
      long LDB)
{
  extern void dgtsv_(const long *Np, const long *NRHSp, double *DL,
                     double *D, double *DU, double *B, const long *LDBp,
                     long *INFOp);
  long info;
  dgtsv_(&N, &NRHS, DL, D, DU, B, &LDB, &info);
  return info;
}

int
main()
{
  int i, info;
  float *z;
  long li;

  info = dgtsv(5, 1, l, d, u, x, 5);
  if (info != 0) fprintf(stderr, "failure with error %d\n", info);

  for (i=0; i<5; ++i) printf("%5.1f\n", x[i]);

  for(i = 0; i < 4; i++)
  {
    l[i] = i;
    u[i] = i;
  }

  printf("dude = %f\n", ddotxxx(4, l, 1, u, 1));
  printf("size = %ld INT_MAX = %ld\n", (long)(SIZE), (long)(INT_MAX));

  z = malloc(sizeof(float)*SIZE);
  for(li = 0; li < SIZE; li++)
    z[li] = 0;
  z[0] = 2;
  z[SIZE-1] = 3;
  printf("ready\n");
  printf("dude extended = %f\n", sdot(SIZE, z, 1, z, 1));

  return 0;
}
