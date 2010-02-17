#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "THLabGeneric.c"
#else

void THLab_(numel)(THTensor *t, int *n_)
{
  *n_ = THTensor_(nElement)(t);
}

void THLab_(max)(THTensor *t, int dimension, THTensor *values_, THLongTensor *indices_)
{
  long *dim;
  long i;

  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 2, "dimension out of range");

  dim = THAlloc(sizeof(long)*THTensor_(nDimension)(t));
  for(i = 0; i < THTensor_(nDimension)(t); i++)
    dim[i] = THTensor_(size)(t, i);
  dim[dimension] = 1;
  THTensor_(resize)(values_, THTensor_(nDimension)(t), dim);
  THLongTensor_resize(indices_, THTensor_(nDimension)(t), dim);
  THFree(dim);

  TH_TENSOR_DIM_APPLY3(real, Real, t, real, Real, values_, long, Long, indices_, dimension,
                       long theIndex = 0;
                       real theMax = t_data[0];
                       for(i = 1; i < t_size; i++)
                       {
                         if(t_data[i*t_stride] > theMax)
                         {
                           theIndex = i;
                           theMax = t_data[i*t_stride];
                         }
                       }
                       *indices__data = theIndex+1;
                       *values__data = theMax;);  

}

void THLab_(min)(THTensor *t, int dimension, THTensor *values_, THLongTensor *indices_)
{
  long *dim;
  long i;

  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 2, "dimension out of range");

  dim = THAlloc(sizeof(long)*THTensor_(nDimension)(t));
  for(i = 0; i < THTensor_(nDimension)(t); i++)
    dim[i] = THTensor_(size)(t, i);
  dim[dimension] = 1;
  THTensor_(resize)(values_, THTensor_(nDimension)(t), dim);
  THLongTensor_resize(indices_, THTensor_(nDimension)(t), dim);
  THFree(dim);

  TH_TENSOR_DIM_APPLY3(real, Real, t, real, Real, values_, long, Long, indices_, dimension,
                       long theIndex = 0;
                       real theMin = t_data[0];
                       for(i = 1; i < t_size; i++)
                       {
                         if(t_data[i*t_stride] < theMin)
                         {
                           theIndex = i;
                           theMin = t_data[i*t_stride];
                         }
                       }
                       *indices__data = theIndex+1;
                       *values__data = theMin;);  

}


void THLab_(sum)(THTensor *t, int dimension, THTensor *r_)
{
  long *dim;
  long i;

  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 2, "dimension out of range");
  
  dim = THAlloc(sizeof(long)*THTensor_(nDimension)(t));
  for(i = 0; i < THTensor_(nDimension)(t); i++)
    dim[i] = THTensor_(size)(t, i);
  dim[dimension] = 1;
  THTensor_(resize)(r_, THTensor_(nDimension)(t), dim);
  THFree(dim);

  TH_GEN_TENSOR_DIM_APPLY2(t, r_, dimension,
                           real sum = 0;
                           long i;
                           for(i = 0; i < t_size; i++)
                             sum += t_data[i*t_stride];
                           *r__data = sum;);
}

void THLab_(prod)(THTensor *t, int dimension, THTensor *r_)
{
  long *dim;
  long i;

  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 2, "dimension out of range");
  
  dim = THAlloc(sizeof(long)*THTensor_(nDimension)(t));
  for(i = 0; i < THTensor_(nDimension)(t); i++)
    dim[i] = THTensor_(size)(t, i);
  dim[dimension] = 1;
  THTensor_(resize)(r_, THTensor_(nDimension)(t), dim);
  THFree(dim);

  TH_GEN_TENSOR_DIM_APPLY2(t, r_, dimension,
                           real prod = 1;
                           long i;
                           for(i = 0; i < t_size; i++)
                             prod *= t_data[i*t_stride];
                           *r__data = prod;);

}

void THLab_(cumsum)(THTensor *t, int dimension, THTensor *r_)
{
  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 2, "dimension out of range");
  
  THTensor_(resizeAs)(r_, t);

  TH_GEN_TENSOR_DIM_APPLY2(t, r_, dimension,
                           real cumsum = 0;
                           long i;
                           for(i = 0; i < t_size; i++)
                           {
                             cumsum += t_data[i*t_stride];
                             r__data[i*r__stride] = cumsum;
                           });
}

void THLab_(cumprod)(THTensor *t, int dimension, THTensor *r_)
{
  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 2, "dimension out of range");
  
  THTensor_(resizeAs)(r_, t);

  TH_GEN_TENSOR_DIM_APPLY2(t, r_, dimension,
                           real cumprod = 1;
                           long i;
                           for(i = 0; i < t_size; i++)
                           {
                             cumprod *= t_data[i*t_stride];
                             r__data[i*r__stride] = cumprod;
                           });
}

void THLab_(trace)(THTensor *t, real *trace_)
{
  real *t_data = THTensor_(data)(t);
  real sum = 0;
  long i = 0;
  long t_stride, t_diag_size;

  THArgCheck(THTensor_(nDimension)(t) == 2, 1, "not a matrix");

  t_stride = THTensor_(stride)(t, 1);
  t_diag_size = THMin(THTensor_(size)(t, 0), THTensor_(size)(t, 1));
  while(i < t_diag_size)
  {
    sum += t_data[i*(t_stride+1)];
    i++;
  }

  *trace_ = sum;
}

/* floating point only now */

#if defined(TH_REAL_IS_FLOAT) || defined(TH_REAL_IS_DOUBLE)

void THLab_(mean)(THTensor *t, int dimension, THTensor *r_)
{
  long *dim;
  long i;

  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 2, "invalid dimension");

  dim = THAlloc(sizeof(long)*THTensor_(nDimension)(t));
  for(i = 0; i < THTensor_(nDimension)(t); i++)
    dim[i] = THTensor_(size)(t, i);
  dim[dimension] = 1;
  THTensor_(resize)(r_, THTensor_(nDimension)(t), dim);
  THFree(dim);

  TH_GEN_TENSOR_DIM_APPLY2(t, r_, dimension,
                           real sum = 0;
                           long i;
                           for(i = 0; i < t_size; i++)
                             sum += t_data[i*t_stride];
                           *r__data = sum/t_size;);
}

void THLab_(std)(THTensor *t, int dimension, int flag, THTensor *r_)
{
  long *dim;
  int i;

  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 3, "invalid dimension");

  dim = THAlloc(sizeof(long)*THTensor_(nDimension)(t));
  for(i = 0; i < THTensor_(nDimension)(t); i++)
    dim[i] = THTensor_(size)(t, i);
  dim[dimension] = 1;
  THTensor_(resize)(r_, THTensor_(nDimension)(t), dim);
  THFree(dim);

  TH_GEN_TENSOR_DIM_APPLY2(t, r_, dimension,
                           real sum = 0;
                           real sum2 = 0;
                           long i;
                           for(i = 0; i < t_size; i++)
                           {
                             real z = t_data[i*t_stride];
                             sum += z;
                             sum2 += z*z;
                           }

                           if(flag)
                           {
                             sum /= t_size;
                             sum2 /= t_size;
                             sum2 -= sum*sum;
                             sum2 = (sum2 < 0 ? 0 : sum2);
                             *r__data = sqrt(sum2);
                           }
                           else
                           {
                             sum /= t_size;
                             sum2 /= t_size-1;
                             sum2 -= ((real)t_size)/((real)(t_size-1))*sum*sum;
                             sum2 = (sum2 < 0 ? 0 : sum2);
                             *r__data = sqrt(sum2);
                           });
}

void THLab_(var)(THTensor *t, int dimension, int flag, THTensor *r_)
{
  long *dim;
  int i;

  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 3, "invalid dimension");

  dim = THAlloc(sizeof(long)*THTensor_(nDimension)(t));
  for(i = 0; i < THTensor_(nDimension)(t); i++)
    dim[i] = THTensor_(size)(t, i);
  dim[dimension] = 1;
  THTensor_(resize)(r_, THTensor_(nDimension)(t), dim);
  THFree(dim);

  TH_GEN_TENSOR_DIM_APPLY2(t, r_, dimension,
                           real sum = 0;
                           real sum2 = 0;
                           long i;
                           for(i = 0; i < t_size; i++)
                           {
                             real z = t_data[i*t_stride];
                             sum += z;
                             sum2 += z*z;
                           }

                           if(flag)
                           {
                             sum /= t_size;
                             sum2 /= t_size;
                             sum2 -= sum*sum;
                             sum2 = (sum2 < 0 ? 0 : sum2);
                             *r__data = sum2;
                           }
                           else
                           {
                             sum /= t_size;
                             sum2 /= t_size-1;
                             sum2 -= ((real)t_size)/((real)(t_size-1))*sum*sum;
                             sum2 = (sum2 < 0 ? 0 : sum2);
                             *r__data = sum2;
                           });
}

void THLab_(norm)(THTensor *t, real value, real *norm_)
{
  *norm_ = THTensor_(norm)(t, value);
}

void THLab_(dist)(THTensor *a, THTensor *b, real value, real *dist_)
{ 
  *dist_ = THTensor_(dist)(a, b, value);
}

#endif /* floating point only part */
#endif
