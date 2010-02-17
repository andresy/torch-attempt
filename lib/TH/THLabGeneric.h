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

#if defined(TH_REAL_IS_FLOAT) || defined(TH_REAL_IS_DOUBLE)
void THLab_(mean)(THTensor *t, int dimension, THTensor *r_);
void THLab_(std)(THTensor *t, int dimension, int flag, THTensor *r_);
void THLab_(var)(THTensor *t, int dimension, int flag, THTensor *r_);
void THLab_(norm)(THTensor *t, real value, real *norm_);
void THLab_(dist)(THTensor *a, THTensor *b, real value, real *dist_);
#endif

#endif
