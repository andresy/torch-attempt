#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "THLabGeneric.h"
#else

void THLab_(numel)(THTensor *t, int *n_);
void THLab_(max)(THTensor *t, int dimension, THTensor *values_, THLongTensor *indices_);
void THLab_(min)(THTensor *t, int dimension, THTensor *values_, THLongTensor *indices_);
void THLab_(sum)(THTensor *t, int dimension, THTensor *r_);
void THLab_(prod)(THTensor *t, int dimension, THTensor *r_);
void THLab_(cumsum)(THTensor *t, int dimension, THTensor *r_);
void THLab_(cumprod)(THTensor *t, int dimension, THTensor *r_);
void THLab_(trace)(THTensor *t, real *trace_);
void THLab_(cross)(THTensor *a, THTensor *b, int dimension, THTensor *r_);

void THLab_(zeros)(int nDimension, long *size, THTensor *r_);
void THLab_(ones)(int nDimension, long *size, THTensor *r_);
void THLab_(diag)(THTensor *t, int k, THTensor *r_);
void THLab_(eye)(long n, long m, THTensor *r_);
void THLab_(range)(real xmin, real xmax, real step, THTensor *r_);
void THLab_(randperm)(long n, THTensor *r_);

void THLab_(reshape)(THTensor *t, int nDimension, long *size, THTensor *r_);
void THLab_(sort)(THTensor *t, int dimension, int descendingOrder, THTensor *rt_, THLongTensor *ri_);
void THLab_(tril)(THTensor *t, long k, THTensor *r_);
void THLab_(triu)(THTensor *t, long k, THTensor *r_);

#if defined(TH_REAL_IS_FLOAT) || defined(TH_REAL_IS_DOUBLE)

void THLab_(log)(THTensor *t, THTensor *r_);
void THLab_(log1p)(THTensor *t, THTensor *r_);
void THLab_(exp)(THTensor *t, THTensor *r_);
void THLab_(cos)(THTensor *t, THTensor *r_);
void THLab_(acos)(THTensor *t, THTensor *r_);
void THLab_(cosh)(THTensor *t, THTensor *r_);
void THLab_(sin)(THTensor *t, THTensor *r_);
void THLab_(asin)(THTensor *t, THTensor *r_);
void THLab_(sinh)(THTensor *t, THTensor *r_);
void THLab_(tan)(THTensor *t, THTensor *r_);
void THLab_(atan)(THTensor *t, THTensor *r_);
void THLab_(tanh)(THTensor *t, THTensor *r_);
void THLab_(pow)(THTensor *t, real value, THTensor *r_);
void THLab_(sqrt)(THTensor *t, THTensor *r_);
void THLab_(ceil)(THTensor *t, THTensor *r_);
void THLab_(floor)(THTensor *t, THTensor *r_);
void THLab_(abs)(THTensor *t, THTensor *r_);

void THLab_(mean)(THTensor *t, int dimension, THTensor *r_);
void THLab_(std)(THTensor *t, int dimension, int flag, THTensor *r_);
void THLab_(var)(THTensor *t, int dimension, int flag, THTensor *r_);
void THLab_(norm)(THTensor *t, real value, real *norm_);
void THLab_(dist)(THTensor *a, THTensor *b, real value, real *dist_);

void THLab_(linspace)(real a, real b, long n, THTensor *r_);
void THLab_(logspace)(real a, real b, long n, THTensor *r_);
void THLab_(rand)(int nDimension, long *size, THTensor *r_);
void THLab_(randn)(int nDimension, long *size, THTensor *r_);

#endif

#endif
