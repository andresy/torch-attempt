#define TH_TENSOR_APPLY(self, code)                 \
  {                                                 \
    real *self##_data = THTensorAPI.data(self);     \
    long self##_size = THTensorAPI.nElement(self);  \
    int i;                                          \
                                                    \
    for(i = 0; i < self##_size; i++)                \
    {                                               \
      code                                          \
    }                                               \
  }

#define TH_TENSOR_APPLY2(self, tensor, code)                            \
  {                                                                     \
    real *self##_data = THTensorAPI.data(self);                         \
    long self##_size = THTensorAPI.nElement(self);                      \
    real *tensor##_data = THTensorAPI.data(tensor);                     \
    long tensor##_size = THTensorAPI.nElement(tensor);                  \
    int i;                                                              \
                                                                        \
    THArgCheck(self##_size == tensor##_size, 1, "inconsistent tensor sizes"); \
    for(i = 0; i < self##_size; i++)                                    \
    {                                                                   \
      code                                                              \
    }                                                                   \
  }                                                                     \

#define TENSOR_IMPLEMENT_BASIC_FUNCTION(NAME, CFUNC)           \
  static void THTensor_##NAME(THTensor *self)                  \
  {                                                            \
    TH_TENSOR_APPLY(self, self_data[i] = CFUNC(self_data[i]);) \
  }

#define TENSOR_IMPLEMENT_BASIC_FUNCTION_VALUE(NAME, CFUNC)              \
  static void THTensor_##NAME(THTensor *self, real value)               \
  {                                                                     \
    TH_TENSOR_APPLY(self, self_data[i] = CFUNC(self_data[i], value););  \
  }                                                                     \

TENSOR_IMPLEMENT_BASIC_FUNCTION(log, log)
TENSOR_IMPLEMENT_BASIC_FUNCTION(log1p, log1p)
TENSOR_IMPLEMENT_BASIC_FUNCTION(exp, exp)
TENSOR_IMPLEMENT_BASIC_FUNCTION(cos, cos)
TENSOR_IMPLEMENT_BASIC_FUNCTION(acos, acos)
TENSOR_IMPLEMENT_BASIC_FUNCTION(cosh, cosh)
TENSOR_IMPLEMENT_BASIC_FUNCTION(sin, sin)
TENSOR_IMPLEMENT_BASIC_FUNCTION(asin, asin)
TENSOR_IMPLEMENT_BASIC_FUNCTION(sinh, sinh)
TENSOR_IMPLEMENT_BASIC_FUNCTION(tan, tan)
TENSOR_IMPLEMENT_BASIC_FUNCTION(atan, atan)
TENSOR_IMPLEMENT_BASIC_FUNCTION(tanh, tanh)
TENSOR_IMPLEMENT_BASIC_FUNCTION_VALUE(pow, pow)
TENSOR_IMPLEMENT_BASIC_FUNCTION(sqrt, sqrt)
TENSOR_IMPLEMENT_BASIC_FUNCTION(ceil, ceil)
TENSOR_IMPLEMENT_BASIC_FUNCTION(floor, floor)
TENSOR_IMPLEMENT_BASIC_FUNCTION(abs, fabs)

static void THTensor_zero(THTensor *self)
{
  TH_TENSOR_APPLY(self, self_data[i] = 0;);
}

static void THTensor_add(THTensor *self, double value)
{
  TH_TENSOR_APPLY(self, self_data[i] += value;)
}

static void THTensor_mul(THTensor *self, double value)
{
#if TH_IS_FLOAT
  THBlas_sscal(THTensorAPI.nElement(self), value, THTensorAPI.data(self), 1);
#elif TH_IS_DOUBLE
  THBlas_dscal(THTensorAPI.nElement(self), value, THTensorAPI.data(self), 1);
#else
  TH_TENSOR_APPLY(self, self_data[i] *= value;);
#endif
}

static void THTensor_div(THTensor *self, double value)
{
  THArgCheck(value != 0, 2, "division by 0");
  THTensor_mul(self, 1/value);
}

static void THTensor_addTensor(THTensor *self, double value, THTensor *tensor)
{
#if TH_IS_FLOAT
  THBlas_saxpy(THTensorAPI.nElement(self), value, THTensorAPI.data(tensor), 1, THTensorAPI.data(self), 1);
#elif TH_IS_DOUBLE
  THBlas_daxpy(THTensorAPI.nElement(self), value, THTensorAPI.data(tensor), 1, THTensorAPI.data(self), 1);
#else
  TH_TENSOR_APPLY2(self, tensor, self_data[i] += value*tensor_data[i];);
#endif
}

static void THTensor_cmul(THTensor *self, THTensor *tensor)
{
  TH_TENSOR_APPLY2(self, tensor, self_data[i] *= tensor_data[i];);
}

/* void THTensor_addcmul(THTensor *tensor, double value, THTensor *src1, THTensor *src2) */
/* { */
/*   TH_TENSOR_APPLY3(double, tensor, double, src1, double, src2, *tensor_p += value * *src1_p * *src2_p;); */
/* } */

static void THTensor_cdiv(THTensor *self, THTensor *tensor)
{
  TH_TENSOR_APPLY2(self, tensor, self_data[i] /= tensor_data[i];);
}

/* void THTensor_addcdiv(THTensor *tensor, double value, THTensor *src1, THTensor *src2) */
/* { */
/*   TH_TENSOR_APPLY3(double, tensor, double, src1, double, src2, *tensor_p += value * *src1_p / *src2_p;); */
/* } */

static real THTensor_dot(THTensor *self, THTensor *tensor)
{
#if TH_IS_FLOAT
  return THBlas_sdot(THTensorAPI.nElement(self), THTensorAPI.data(self), 1, THTensorAPI.data(tensor));
#elif TH_IS_DOUBLE
  return THBlas_ddot(THTensorAPI.nElement(self), THTensorAPI.data(self), 1, THTensorAPI.data(tensor));
#else
  real sum;
  TH_TENSOR_APPLY2(self, tensor, sum += self_data[i]*tensor_data[i];);
  return sum;
#endif
}

/* basic statistics */
static real THTensor_min(THTensor *self)
{
  real theMin = THTensorAPI.get1d(self, 0);
  TH_TENSOR_APPLY(self, if(self_data[i] < theMin) theMin = self_data[i];);
  return theMin;
}

static real THTensor_max(THTensor *self)
{
  real theMax = THTensorAPI.get1d(self, 0);
  TH_TENSOR_APPLY(self, if(self_data[i] > theMax) theMax = self_data[i];);
  return theMax;
}

static real THTensor_sum(THTensor *self)
{
  real sum = 0;
  TH_TENSOR_APPLY(self, sum += self_data[i];);
  return sum;
}

static real THTensor_mean(THTensor *self)
{
  THArgCheck(THTensorAPI.nElement(self) > 0, 1, "cannot average an empty tensor");
  return THTensor_sum(self)/THTensorAPI.nElement(self);
}

static real THTensor_var(THTensor *self)
{
  real mean = THTensor_mean(self);
  real sum = 0;
  TH_TENSOR_APPLY(self, sum += (self_data[i] - mean)*(self_data[i] - mean););
  sum /= (THTensorAPI.nElement(self)-1);
  return sum;
}

static real THTensor_std(THTensor *self)
{
  return sqrt(THTensor_var(self));
}
 
static real THTensor_norm(THTensor *self, real value)
{
  real sum = 0;
  if(value == 1)
  {
    TH_TENSOR_APPLY(self, sum += fabs(self_data[i]););
    return sum;
  }
  else if(value == 2)
  {
    TH_TENSOR_APPLY(self, sum += self_data[i]*self_data[i];);
    return sqrt(sum);
  }
  else
  {
    TH_TENSOR_APPLY(self, sum += pow(fabs(self_data[i]), value););
    return pow(sum, 1.0/value);
  }
}

static real THTensor_dist(THTensor *self, THTensor *tensor, double value)
{
  real sum = 0;
  if(value == 1)
  {
    TH_TENSOR_APPLY2(self, tensor, sum += fabs(self_data[i]-tensor_data[i]););
    return sum;
  }
  else if(value == 2)
  {
    TH_TENSOR_APPLY2(self, tensor,
                    real z = self_data[i]-tensor_data[i];
                    sum += z*z;);
    return sqrt(sum);
  }
  else
  {
    TH_TENSOR_APPLY2(self, tensor, sum += pow(fabs(self_data[i]-tensor_data[i]), value););
    return pow(sum, 1.0/value);
  }
}

/* void THTensor_addT2dotT1(THTensor *tensor, double alpha, THTensor *mat, THTensor *vec)  */
/* { */
/*   if( (mat->nDimension != 2) || (vec->nDimension != 1) ) */
/*     THError("matrix and vector expected"); */
 
/*   if( mat->size[1] != vec->size[0] ) */
/*     THError("size mismatch"); */

/*   if(tensor->nDimension != 1) */
/*     THError("size mismatch"); */
    
/*   if( tensor->size[0] != mat->size[0] ) */
/*     THError("size mismatch"); */

/*   if(mat->stride[0] == 1) */
/*     THBlas_matVec(0, mat->size[0], mat->size[1], alpha, THTensor_dataPtr(mat), mat->stride[1], THTensor_dataPtr(vec), vec->stride[0], 1, THTensor_dataPtr(tensor), tensor->stride[0]); */
/*   else if(mat->stride[1] == 1) */
/*     THBlas_matVec(1,  mat->size[1], mat->size[0], alpha, THTensor_dataPtr(mat), mat->stride[0], THTensor_dataPtr(vec), vec->stride[0], 1, THTensor_dataPtr(tensor), tensor->stride[0]); */
/*   else */
/*   { */
/*     long l; */

/*     for(l = 0; l < mat->size[0]; l++) */
/*       *(THTensor_dataPtr1d(tensor, l)) += alpha*THBlas_dot(mat->size[1], THTensor_dataPtr1d(mat, l), mat->stride[1], THTensor_dataPtr(vec), vec->stride[0]); */
/*   } */
/* } */

/* void THTensor_addT4dotT2(THTensor *tensor, double alpha, THTensor *t4, THTensor *t2) */
/* { */
/*   long i, j, k, l; */

/*   if( (t4->nDimension != 4) || (t2->nDimension != 2) ) */
/*     THError("4D tensor and matrix expected"); */

/*   if( (t4->size[2] != t2->size[0]) || (t4->size[3] != t2->size[1]) ) */
/*     THError("size mismatch"); */

/*   if(tensor->nDimension != 2) */
/*     THError("size mismatch"); */

/*   if( (tensor->size[0] != t4->size[0]) || (tensor->size[1] != t4->size[1]) ) */
/*     THError("size mismatch"); */

/*   if(t4->stride[2] == 1) */
/*   { */
/*     long t4sz2 = t4->size[2]; */
/*     long t4sz3 = t4->size[3]; */
/*     long t4st3 = t4->stride[3]; */
/*     long t2st0 = t2->stride[0]; */
/*     long t2st1 = t2->stride[1]; */
    
/*     if(tensor->stride[0] == 1) */
/*     { */
/*       double *pt = THTensor_dataPtr(tensor); */
/*       for(j = 0; j < tensor->size[1]; j++) */
/*       { */
/*         for(i = 0; i < tensor->size[0]; i++) */
/*         { */
/*           double sum = 0; */
/*           double *pt4 = THTensor_dataPtr2d(t4, i, j); */
/*           double *pt2 = THTensor_dataPtr(t2); */
/*           for(l = 0; l < t4sz3; l++) */
/*           { */
/*             for(k = 0; k < t4sz2; k++) */
/*               sum += pt4[k]*pt2[k*t2st0]; */
/*             pt4 += t4st3; */
/*             pt2 += t2st1; */
/*           } */
/*           pt[i*tensor->stride[0]] += alpha*sum; */
/*         } */
/*         pt += tensor->stride[1]; */
/*       } */
/*     } */
/*     else */
/*     { */
/*       double *pt = THTensor_dataPtr(tensor); */
/*       for(i = 0; i < tensor->size[0]; i++) */
/*       { */
/*         for(j = 0; j < tensor->size[1]; j++) */
/*         { */
/*           double sum = 0; */
/*           double *pt4 = THTensor_dataPtr2d(t4, i, j); */
/*           double *pt2 = THTensor_dataPtr(t2); */
/*           for(l = 0; l < t4sz3; l++) */
/*           { */
/*             for(k = 0; k < t4sz2; k++) */
/*               sum += pt4[k]*pt2[k*t2st0]; */
/*             pt4 += t4st3; */
/*             pt2 += t2st1; */
/*           } */
/*           pt[j*tensor->stride[1]] += alpha*sum; */
/*         } */
/*         pt += tensor->stride[0]; */
/*       } */
/*     } */
/*   } */
/*   else if(t4->stride[3] == 1) */
/*   { */
/*     long t4sz2 = t4->size[2]; */
/*     long t4sz3 = t4->size[3]; */
/*     long t4st2 = t4->stride[2]; */
/*     long t2st0 = t2->stride[0]; */
/*     long t2st1 = t2->stride[1]; */
    
/*     if(tensor->stride[0] == 1) */
/*     { */
/*       double *pt = THTensor_dataPtr(tensor); */
/*       for(j = 0; j < tensor->size[1]; j++) */
/*       { */
/*         for(i = 0; i < tensor->size[0]; i++) */
/*         { */
/*           double sum = 0; */
/*           double *pt4 = THTensor_dataPtr2d(t4, i, j); */
/*           double *pt2 = THTensor_dataPtr(t2); */
/*           for(k = 0; k < t4sz2; k++) */
/*           { */
/*             for(l = 0; l < t4sz3; l++) */
/*               sum += pt4[l]*pt2[l*t2st1]; */
/*             pt4 += t4st2; */
/*             pt2 += t2st0; */
/*           } */
/*           pt[i*tensor->stride[0]] += alpha*sum; */
/*         } */
/*         pt += tensor->stride[1]; */
/*       } */
/*     } */
/*     else */
/*     { */
/*       double *pt = THTensor_dataPtr(tensor); */
/*       for(i = 0; i < tensor->size[0]; i++) */
/*       { */
/*         for(j = 0; j < tensor->size[1]; j++) */
/*         { */
/*           double sum = 0; */
/*           double *pt4 = THTensor_dataPtr2d(t4, i, j); */
/*           double *pt2 = THTensor_dataPtr(t2); */
/*           for(k = 0; k < t4sz2; k++) */
/*           { */
/*             for(l = 0; l < t4sz3; l++) */
/*               sum += pt4[l]*pt2[l*t2st1]; */
/*             pt4 += t4st2; */
/*             pt2 += t2st0; */
/*           } */
/*           pt[j*tensor->stride[1]] += alpha*sum; */
/*         } */
/*         pt += tensor->stride[0]; */
/*       } */
/*     } */
/*   } */
/*   else if(t4->stride[0] == 1) */
/*   { */
/*     long sz0 = tensor->size[0]; */
/*     long sz1 = tensor->size[1]; */
/*     long st0 = tensor->stride[0]; */
/*     long st1 = tensor->stride[1]; */
/*     long t4st1 = t4->stride[1]; */
    
/*     if(t2->stride[0] == 1) */
/*     { */
/*       double *pt2 = THTensor_dataPtr(t2); */
/*       for(l = 0; l < t4->size[3]; l++) */
/*       { */
/*         for(k = 0; k < t4->size[2]; k++) */
/*         { */
/*           double z = alpha * pt2[k*t2->stride[0]]; */
/*           double *pt4 = THTensor_dataPtr4d(t4, 0, 0, k, l); */
/*           double *pt = THTensor_dataPtr(tensor); */
/*           for(j = 0; j < sz1; j++) */
/*           { */
/*             for(i = 0; i < sz0; i++) */
/*               pt[i*st0] += pt4[i]*z; */
/*             pt += st1; */
/*             pt4 += t4st1; */
/*           } */
/*         } */
/*         pt2 += t2->stride[1]; */
/*       } */
/*     } */
/*     else */
/*     { */
/*       double *pt2 = THTensor_dataPtr(t2); */
/*       for(k = 0; k < t4->size[2]; k++) */
/*       { */
/*         for(l = 0; l < t4->size[3]; l++) */
/*         { */
/*           double z = alpha * pt2[l*t2->stride[1]]; */
/*           double *pt4 = THTensor_dataPtr4d(t4, 0, 0, k, l); */
/*           double *pt = THTensor_dataPtr(tensor); */
/*           for(j = 0; j < sz1; j++) */
/*           { */
/*             for(i = 0; i < sz0; i++) */
/*               pt[i*st0] += pt4[i]*z; */
/*             pt += st1; */
/*             pt4 += t4st1; */
/*           } */
/*         } */
/*         pt2 += t2->stride[0]; */
/*       } */
/*     } */
/*   } */
/*   else if(t4->stride[1] == 1) */
/*   { */
/*     long sz0 = tensor->size[0]; */
/*     long sz1 = tensor->size[1]; */
/*     long st0 = tensor->stride[0]; */
/*     long st1 = tensor->stride[1]; */
/*     long t4st0 = t4->stride[0]; */
    
/*     if(t2->stride[0] == 1) */
/*     { */
/*       for(l = 0; l < t4->size[3]; l++) */
/*       { */
/*         for(k = 0; k < t4->size[2]; k++) */
/*         { */
/*           double z = alpha * *(THTensor_dataPtr2d(t2, k, l)); */
/*           double *pt4 = THTensor_dataPtr4d(t4, 0, 0, k, l); */
/*           double *pt = THTensor_dataPtr(tensor); */
/*           for(i = 0; i < sz0; i++) */
/*           { */
/*             for(j = 0; j < sz1; j++) */
/*               pt[j*st1] += pt4[j]*z; */
/*             pt += st0; */
/*             pt4 += t4st0; */
/*           } */
/*         } */
/*       } */
/*     } */
/*     else */
/*     { */
/*       for(k = 0; k < t4->size[2]; k++) */
/*       { */
/*         for(l = 0; l < t4->size[3]; l++) */
/*         { */
/*           double z = alpha * *(THTensor_dataPtr2d(t2, k, l)); */
/*           double *pt4 = THTensor_dataPtr4d(t4, 0, 0, k, l); */
/*           double *pt = THTensor_dataPtr(tensor); */
/*           for(i = 0; i < sz0; i++) */
/*           { */
/*             for(j = 0; j < sz1; j++) */
/*               pt[j*st1] += pt4[j]*z; */
/*             pt += st0; */
/*             pt4 += t4st0; */
/*           } */
/*         } */
/*       } */
/*     } */
/*   } */
/*   else */
/*   { */
/*     double *pt = THTensor_dataPtr(tensor); */
/*     long t2sz0 = t2->size[0]; */
/*     long t2sz1 = t2->size[1]; */
/*     long t2st0 = t2->stride[0]; */
/*     long t2st1 = t2->stride[1]; */
/*     long t4st2 = t4->stride[2]; */
/*     long t4st3 = t4->stride[3]; */
/*     for(j = 0; j < tensor->size[1]; j++) */
/*     { */
/*       for(i = 0; i < tensor->size[0]; i++) */
/*       { */
/*         double sum = 0; */
/*         double *pt4 = THTensor_dataPtr4d(t4, i, j, 0, 0); */
/*         double *pt2 = THTensor_dataPtr(t2); */
/*         for(l = 0; l < t2sz1; l++) */
/*         { */
/*           for(k = 0; k < t2sz0; k++) */
/*             sum += pt4[k*t4st2]*pt2[k*t2st0]; */
/*           pt2 += t2st1; */
/*           pt4 += t4st3; */
/*         } */
/*         pt[i*tensor->stride[0]] += alpha*sum; */
/*       } */
/*       pt += tensor->stride[1]; */
/*     } */
/*   } */
/* } */

/* void THTensor_addT1outT1(THTensor *tensor, double alpha, THTensor *vec1, THTensor *vec2) */
/* { */
/*   if( (vec1->nDimension != 1) || (vec2->nDimension != 1) ) */
/*     THError("vector and vector expected"); */

/*   if(tensor->nDimension != 2) */
/*     THError("size mismatch."); */
    
/*   if( (tensor->size[0] != vec1->size[0]) || (tensor->size[1] != vec2->size[0]) ) */
/*     THError("size mismatch."); */

/*   if(tensor->stride[0] == 1) */
/*   { */
/*     THBlas_outerProduct(vec1->size[0], vec2->size[0], alpha, THTensor_dataPtr(vec1), vec1->stride[0], */
/*                         THTensor_dataPtr(vec2), vec2->stride[0], */
/*                         THTensor_dataPtr(tensor), tensor->stride[1]); */
/*   } */
/*   else if(tensor->stride[1] == 1) */
/*   { */
/*     THBlas_outerProduct(vec2->size[0], vec1->size[0], alpha, THTensor_dataPtr(vec2), vec2->stride[0], */
/*                         THTensor_dataPtr(vec1), vec1->stride[0], */
/*                         THTensor_dataPtr(tensor), tensor->stride[0]); */
/*   } */
/*   else */
/*   { */
/*     double *pvec1 = THTensor_dataPtr(vec1); */
/*     double *pvec2 = THTensor_dataPtr(vec2); */
/*     double *pt = THTensor_dataPtr(tensor); */
/*     long vec1st0 = vec1->stride[0]; */
/*     long vec1sz0 = vec1->size[0]; */
/*     long vec2st0 = vec2->stride[0]; */
/*     long vec2sz0 = vec2->size[0]; */
/*     long st0 = tensor->stride[0]; */
/*     long st1 = tensor->stride[1]; */
/*     long l, c; */
/*     for(c = 0; c < vec2sz0; c++) */
/*     { */
/*       double z = alpha*pvec2[c*vec2st0]; */
/*       for(l = 0; l < vec1sz0; l++) */
/*         pt[l*st0] += pvec1[l*vec1st0]*z; */
/*       pt += st1; */
/*     } */
/*   } */
/* } */

/* void THTensor_addT2outT2(THTensor *tensor, double alpha, THTensor *m1, THTensor *m2) */
/* { */
/*   long i, j, k, l; */

/*   if( (m1->nDimension != 2) || (m2->nDimension != 2) )  */
/*     THError("matrix and matrix expected");  */

/*   if(tensor->nDimension != 4) */
/*     THError("size mismatch"); */

/*   if( (tensor->size[0] != m1->size[0]) || (tensor->size[1] != m1->size[1]) || (tensor->size[2] != m2->size[0]) || (tensor->size[3] != m2->size[1]) )  */
/*     THError("size mismatch");  */

/*   if(tensor->stride[0] == 1) */
/*   { */
/*     long st1 = tensor->stride[1]; */
/*     long m1st0 = m1->stride[0]; */
/*     long m1st1 = m1->stride[1]; */
/*     long m1sz0 = m1->size[0]; */
/*     long m1sz1 = m1->size[1]; */

/*     if(m2->stride[0] == 1) */
/*     { */
/*       for(l = 0; l < m2->size[1]; l++) */
/*       { */
/*         double *pm2 = THTensor_dataPtr2d(m2, 0, l); */
/*         for(k = 0; k < m2->size[0]; k++) */
/*         { */
/*           double *pt = THTensor_dataPtr4d(tensor, 0,0,k,l); */
/*           double *pm1 = THTensor_dataPtr(m1); */
/*           double z = alpha*pm2[k*m2->stride[0]]; */
/*           for(j = 0; j < m1sz1; j++) */
/*           { */
/*             for(i = 0; i < m1sz0; i++) */
/*               pt[i] += z * pm1[i*m1st0]; */
/*             pt += st1; */
/*             pm1 += m1st1; */
/*           } */
/*         } */
/*         pm2 += m2->stride[1]; */
/*       } */
/*     } */
/*     else */
/*     { */
/*       for(k = 0; k < m2->size[0]; k++) */
/*       { */
/*         double *pm2 = THTensor_dataPtr1d(m2, k); */
/*         for(l = 0; l < m2->size[1]; l++) */
/*         { */
/*           double *pt = THTensor_dataPtr4d(tensor, 0,0,k,l); */
/*           double *pm1 = THTensor_dataPtr(m1); */
/*           double z = alpha*pm2[l*m2->stride[1]]; */
/*           for(j = 0; j < m1sz1; j++) */
/*           { */
/*             for(i = 0; i < m1sz0; i++) */
/*               pt[i] += z * pm1[i*m1st0]; */
/*             pt += st1; */
/*             pm1 += m1st1; */
/*           } */
/*         } */
/*         pm2 += m2->stride[0]; */
/*       } */
/*     } */
/*   } */
/*   else if(tensor->stride[1] == 1) */
/*   { */
/*     long st0 = tensor->stride[0]; */
/*     long m1st0 = m1->stride[0]; */
/*     long m1st1 = m1->stride[1]; */
/*     long m1sz0 = m1->size[0]; */
/*     long m1sz1 = m1->size[1]; */

/*     if(m2->stride[0] == 1) */
/*     { */
/*       for(l = 0; l < m2->size[1]; l++) */
/*       { */
/*         double *pm2 = THTensor_dataPtr2d(m2, 0, l); */
/*         for(k = 0; k < m2->size[0]; k++) */
/*         { */
/*           double *pt = THTensor_dataPtr4d(tensor, 0,0,k,l); */
/*           double *pm1 = THTensor_dataPtr(m1); */
/*           double z = alpha*pm2[k*m2->stride[0]]; */
/*           for(i = 0; i < m1sz0; i++) */
/*           { */
/*             for(j = 0; j < m1sz1; j++) */
/*               pt[j] += z * pm1[j*m1st1]; */
/*             pt += st0; */
/*             pm1 += m1st0; */
/*           } */
/*         } */
/*         pm2 += m2->stride[1]; */
/*       } */
/*     } */
/*     else */
/*     { */
/*       for(k = 0; k < m2->size[0]; k++) */
/*       { */
/*         double *pm2 = THTensor_dataPtr1d(m2, k); */
/*         for(l = 0; l < m2->size[1]; l++) */
/*         { */
/*           double *pt = THTensor_dataPtr4d(tensor, 0,0,k,l); */
/*           double *pm1 = THTensor_dataPtr(m1); */
/*           double z = alpha*pm2[l*m2->stride[1]]; */
/*           for(i = 0; i < m1sz0; i++) */
/*           { */
/*             for(j = 0; j < m1sz1; j++) */
/*               pt[j] += z * pm1[j*m1st1]; */
/*             pt += st0; */
/*             pm1 += m1st0; */
/*           } */
/*         } */
/*         pm2 += m2->stride[0]; */
/*       } */
/*     } */
/*   } */
/*   else if(tensor->stride[2] == 1) */
/*   { */
/*     long st3 = tensor->stride[3]; */
/*     long m2st0 = m2->stride[0]; */
/*     long m2st1 = m2->stride[1]; */
/*     long m2sz0 = m2->size[0]; */
/*     long m2sz1 = m2->size[1]; */

/*     if(m1->stride[0] == 1) */
/*     { */
/*       for(l = 0; l < m1->size[1]; l++) */
/*       { */
/*         double *pm1 = THTensor_dataPtr2d(m1, 0, l); */
/*         for(k = 0; k < m1->size[0]; k++) */
/*         { */
/*           double *pt = THTensor_dataPtr2d(tensor, k,l); */
/*           double *pm2 = THTensor_dataPtr(m2); */
/*           double z = alpha*pm1[k*m1->stride[0]]; */
/*           for(j = 0; j < m2sz1; j++) */
/*           { */
/*             for(i = 0; i < m2sz0; i++) */
/*               pt[i] += z * pm2[i*m2st0]; */
/*             pt += st3; */
/*             pm2 += m2st1; */
/*           } */
/*         } */
/*         pm1 += m1->stride[1]; */
/*       } */
/*     } */
/*     else */
/*     { */
/*       for(k = 0; k < m1->size[0]; k++) */
/*       { */
/*         double *pm1 = THTensor_dataPtr1d(m1, k); */
/*         for(l = 0; l < m1->size[1]; l++) */
/*         { */
/*           double *pt = THTensor_dataPtr2d(tensor, k,l); */
/*           double *pm2 = THTensor_dataPtr(m2); */
/*           double z = alpha*pm1[l*m1->stride[1]]; */
/*           for(j = 0; j < m2sz1; j++) */
/*           { */
/*             for(i = 0; i < m2sz0; i++) */
/*               pt[i] += z * pm2[i*m2st0]; */
/*             pt += st3; */
/*             pm2 += m2st1; */
/*           } */
/*         } */
/*         pm1 += m1->stride[0]; */
/*       } */
/*     } */
/*   } */
/*   else if(tensor->stride[3] == 1) */
/*   { */
/*     long st2 = tensor->stride[2]; */
/*     long m2st0 = m2->stride[0]; */
/*     long m2st1 = m2->stride[1]; */
/*     long m2sz0 = m2->size[0]; */
/*     long m2sz1 = m2->size[1]; */

/*     if(m1->stride[0] == 1) */
/*     { */
/*       for(l = 0; l < m1->size[1]; l++) */
/*       { */
/*         double *pm1 = THTensor_dataPtr2d(m1, 0, l); */
/*         for(k = 0; k < m1->size[0]; k++) */
/*         { */
/*           double *pt = THTensor_dataPtr2d(tensor, k,l); */
/*           double *pm2 = THTensor_dataPtr(m2); */
/*           double z = alpha*pm1[k*m1->stride[0]]; */
/*           for(i = 0; i < m2sz0; i++) */
/*           { */
/*             for(j = 0; j < m2sz1; j++) */
/*               pt[j] += z * pm2[j*m2st1]; */
/*             pt += st2; */
/*             pm2 += m2st0; */
/*           } */
/*         } */
/*         pm1 += m1->stride[1]; */
/*       } */
/*     } */
/*     else */
/*     { */
/*       for(k = 0; k < m1->size[0]; k++) */
/*       { */
/*         double *pm1 = THTensor_dataPtr1d(m1, k); */
/*         for(l = 0; l < m1->size[1]; l++) */
/*         { */
/*           double *pt = THTensor_dataPtr2d(tensor, k,l); */
/*           double *pm2 = THTensor_dataPtr(m2); */
/*           double z = alpha*pm1[l*m1->stride[1]]; */
/*           for(i = 0; i < m2sz0; i++) */
/*           { */
/*             for(j = 0; j < m2sz1; j++) */
/*               pt[j] += z * pm2[j*m2st1]; */
/*             pt += st2; */
/*             pm2 += m2st0; */
/*           } */
/*         } */
/*         pm1 += m1->stride[0]; */
/*       } */
/*     } */
/*   } */
/*   else */
/*   { */
/*     long st0 = tensor->stride[0]; */
/*     long st1 = tensor->stride[1]; */
/*     long m1st0 = m1->stride[0]; */
/*     long m1st1 = m1->stride[1]; */
/*     long m1sz0 = m1->size[0]; */
/*     long m1sz1 = m1->size[1]; */
/*     for(l = 0; l < m2->size[1]; l++) */
/*     { */
/*       double *pm2 = THTensor_dataPtr2d(m2, 0, l); */
/*       for(k = 0; k < m2->size[0]; k++) */
/*       { */
/*         double *pt = THTensor_dataPtr4d(tensor, 0,0,k,l); */
/*         double *pm1 = THTensor_dataPtr(m1); */
/*         double z = alpha*pm2[k*m2->stride[0]]; */
/*         for(j = 0; j < m1sz1; j++) */
/*         { */
/*           for(i = 0; i < m1sz0; i++) */
/*             pt[i*st0] += z * pm1[i*m1st0]; */
/*           pt += st1; */
/*           pm1 += m1st1; */
/*         } */
/*       } */
/*       pm2 += m2->stride[1]; */
/*     } */
/*   } */
/* } */

/* void THTensor_addT2dotT2(THTensor *tensor, double alpha, THTensor *m1, THTensor *m2)  */
/* {  */
/*   long r, c; */

/*   if( (m1->nDimension != 2) || (m2->nDimension != 2) )  */
/*     THError("matrix and matrix expected");  */
 
/*   if(tensor->nDimension != 2) */
/*     THError("size mismatch");  */

/*   if( (tensor->size[0] != m1->size[0]) || (tensor->size[1] != m2->size[1]) || (m1->size[1] != m2->size[0]) )  */
/*     THError("size mismatch");  */

/*   if(tensor->stride[0] == 1) */
/*   { */
/*     if(m1->stride[0] == 1) */
/*     { */
/*       if(m2->stride[0] == 1) */
/*       { */
/*         THBlas_matMat(0, 0, tensor->size[0], tensor->size[1], m1->size[1], alpha, */
/*                      THTensor_dataPtr(m1), m1->stride[1], */
/*                      THTensor_dataPtr(m2), m2->stride[1], */
/*                      1, THTensor_dataPtr(tensor), tensor->stride[1]); */
/*       } */
/*       else if(m2->stride[1] == 1) */
/*       { */
/*         THBlas_matMat(0, 1, tensor->size[0], tensor->size[1], m1->size[1], alpha, */
/*                      THTensor_dataPtr(m1), m1->stride[1], */
/*                      THTensor_dataPtr(m2), m2->stride[0], */
/*                      1, THTensor_dataPtr(tensor), tensor->stride[1]); */
/*       } */
/*       else */
/*       { */
/*         for(c = 0; c < tensor->size[1]; c++) */
/*         { */
/*           THBlas_matVec(0, m1->size[0], m1->size[1], alpha,  */
/*                         THTensor_dataPtr(m1), m1->stride[1], */
/*                         THTensor_selectPtr(m2, 1, c), m2->stride[0], */
/*                         1, THTensor_selectPtr(tensor, 1, c), tensor->stride[0]); */
/*         } */
/*       } */
/*     } */
/*     else if(m1->stride[1] == 1) */
/*     { */
/*       if(m2->stride[0] == 1) */
/*       { */
/*         THBlas_matMat(1, 0, tensor->size[0], tensor->size[1], m1->size[1], alpha, */
/*                      THTensor_dataPtr(m1), m1->stride[0], */
/*                      THTensor_dataPtr(m2), m2->stride[1], */
/*                      1, THTensor_dataPtr(tensor), tensor->stride[1]); */
/*       } */
/*       else if(m2->stride[1] == 1) */
/*       { */
/*         THBlas_matMat(1, 1, tensor->size[0], tensor->size[1], m1->size[1], alpha, */
/*                      THTensor_dataPtr(m1), m1->stride[0], */
/*                      THTensor_dataPtr(m2), m2->stride[0], */
/*                      1, THTensor_dataPtr(tensor), tensor->stride[1]); */
/*       } */
/*       else */
/*       { */
/*         for(c = 0; c < tensor->size[1]; c++) */
/*         { */
/*           THBlas_matVec(1, m1->size[1], m1->size[0], alpha,  */
/*                         THTensor_dataPtr(m1), m1->stride[0], */
/*                         THTensor_selectPtr(m2, 1, c), m2->stride[0], */
/*                         1, THTensor_selectPtr(tensor, 1, c), tensor->stride[0]); */
/*         } */
/*       } */
/*     } */
/*     else */
/*     { */
/*       if(m2->stride[0] == 1) */
/*       { */
/*         for(r = 0; r < tensor->size[0]; r++) */
/*         { */
/*           THBlas_matVec(1, m2->size[0], m2->size[1], alpha,  */
/*                         THTensor_dataPtr(m2), m2->stride[1], */
/*                         THTensor_selectPtr(m1, 0, r), m1->stride[1], */
/*                         1, THTensor_selectPtr(tensor, 0, r), tensor->stride[1]); */
/*         } */
/*       } */
/*       else if(m2->stride[1] == 1) */
/*       { */
/*         for(r = 0; r < tensor->size[0]; r++) */
/*         { */
/*           THBlas_matVec(0, tensor->size[1], m1->size[1], alpha, */
/*                        THTensor_dataPtr(m2), m2->stride[0], */
/*                        THTensor_selectPtr(m1, 0, r), m1->stride[1], */
/*                        1, THTensor_selectPtr(tensor, 0, r), tensor->stride[1]); */
/*         } */
/*       } */
/*       else */
/*       { */
/*         for(c = 0; c < tensor->size[1]; c++) */
/*         { */
/*           double *p = THTensor_selectPtr(tensor, 1, c); */
/*           for(r = 0; r < tensor->size[0]; r++) */
/*             p[r] += alpha*THBlas_dot(m1->size[1], THTensor_selectPtr(m1, 0, r), m1->stride[1], THTensor_selectPtr(m2, 1, c), m2->stride[0]); */
/*         } */
/*       } */
/*     } */
/*   } */
/*   else if(tensor->stride[1] == 1) */
/*   { */
/* /\********\/ */
/*     if(m2->stride[1] == 1) */
/*     { */
/*       if(m1->stride[1] == 1) */
/*       { */
/*         THBlas_matMat(0, 0, tensor->size[1], tensor->size[0], m2->size[0], alpha, */
/*                      THTensor_dataPtr(m2), m2->stride[0], */
/*                      THTensor_dataPtr(m1), m1->stride[0], */
/*                      1, THTensor_dataPtr(tensor), tensor->stride[0]); */
/*       } */
/*       else if(m1->stride[0] == 1) */
/*       { */
/*         THBlas_matMat(0, 1, tensor->size[1], tensor->size[0], m2->size[0], alpha, */
/*                      THTensor_dataPtr(m2), m2->stride[0], */
/*                      THTensor_dataPtr(m1), m1->stride[1], */
/*                      1, THTensor_dataPtr(tensor), tensor->stride[0]); */
/*       } */
/*       else */
/*       { */
/*         for(r = 0; r < tensor->size[0]; r++) */
/*         { */
/*           THBlas_matVec(0, tensor->size[1], m2->size[0], alpha,  */
/*                        THTensor_dataPtr(m2), m2->stride[0], */
/*                        THTensor_selectPtr(m1, 0, r), m1->stride[1], */
/*                        1, THTensor_selectPtr(tensor, 0, r), tensor->stride[1]); */
/*         } */
/*       } */
/*     } */
/*     else if(m2->stride[0] == 1) */
/*     { */
/*       if(m1->stride[1] == 1) */
/*       { */
/*         THBlas_matMat(1, 0, tensor->size[1], tensor->size[0], m2->size[0], alpha, */
/*                      THTensor_dataPtr(m2), m2->stride[1], */
/*                      THTensor_dataPtr(m1), m1->stride[0], */
/*                      1, THTensor_dataPtr(tensor), tensor->stride[0]); */
/*       } */
/*       else if(m1->stride[0] == 1) */
/*       { */
/*         THBlas_matMat(1, 1, tensor->size[1], tensor->size[0], m2->size[0], alpha, */
/*                      THTensor_dataPtr(m2), m2->stride[1], */
/*                      THTensor_dataPtr(m1), m1->stride[1], */
/*                      1, THTensor_dataPtr(tensor), tensor->stride[0]); */
/*       } */
/*       else */
/*       { */
/*         for(r = 0; r < tensor->size[0]; r++) */
/*         { */
/*           THBlas_matVec(1, m2->size[0], m2->size[1], alpha,  */
/*                         THTensor_dataPtr(m2), m2->stride[1], */
/*                         THTensor_selectPtr(m1, 0, r), m1->stride[1], */
/*                         1, THTensor_selectPtr(tensor, 0, r), tensor->stride[1]); */
/*         } */
/*       } */
/*     } */
/*     else */
/*     { */
/*       if(m1->stride[1] == 1) */
/*       { */
/*         for(c = 0; c < tensor->size[1]; c++) */
/*         { */
/*           THBlas_matVec(1, m1->size[1], m1->size[0], alpha,  */
/*                         THTensor_dataPtr(m1), m1->stride[0], */
/*                         THTensor_selectPtr(m2, 1, c), m2->stride[0], */
/*                         1, THTensor_selectPtr(tensor, 1, c), tensor->stride[0]); */
/*         } */
/*       } */
/*       else if(m1->stride[0] == 1) */
/*       { */
/*         for(c = 0; c < tensor->size[1]; c++) */
/*         { */
/*           THBlas_matVec(0, tensor->size[0], m2->size[0], alpha, */
/*                        THTensor_dataPtr(m1), m1->stride[1], */
/*                        THTensor_selectPtr(m2, 1, c), m2->stride[0], */
/*                        1, THTensor_selectPtr(tensor, 1, c), tensor->stride[0]); */
/*         } */
/*       } */
/*       else */
/*       { */
/*         for(r = 0; r < tensor->size[0]; r++) */
/*         { */
/*           double *p = THTensor_selectPtr(tensor, 0, r); */
/*           for(c = 0; c < tensor->size[1]; c++) */
/*             p[c] += alpha*THBlas_dot(m2->size[0], THTensor_selectPtr(m2, 1, c), m2->stride[0], THTensor_selectPtr(m1, 0, r), m1->stride[1]); */
/*         } */
/*       } */
/*     } */
/* /\*******\/ */
/*   } */
/*   else */
/*   { */
/* /\*******\/ */
/*     if(m1->stride[1] == 1) */
/*     { */
/*       for(c = 0; c < tensor->size[1]; c++) */
/*       { */
/*         THBlas_matVec(1, m1->size[1], m1->size[0], alpha,  */
/*                       THTensor_dataPtr(m1), m1->stride[0], */
/*                       THTensor_selectPtr(m2, 1, c), m2->stride[0], */
/*                       1, THTensor_selectPtr(tensor, 1, c), tensor->stride[0]); */
/*       } */
/*     } */
/*     else if(m2->stride[0] == 1) */
/*     { */
/*       for(r = 0; r < tensor->size[0]; r++) */
/*       { */
/*         THBlas_matVec(1, m2->size[0], m2->size[1], alpha,  */
/*                       THTensor_dataPtr(m2), m2->stride[1], */
/*                       THTensor_selectPtr(m1, 0, r), m1->stride[1], */
/*                       1, THTensor_selectPtr(tensor, 0, r), tensor->stride[1]); */
/*       }         */
/*     } */
/*     else */
/*     { */
/*       for(c = 0; c < tensor->size[1]; c++) */
/*       { */
/*         double *p = THTensor_selectPtr(tensor, 1, c); */
/*         for(r = 0; r < tensor->size[0]; r++) */
/*           p[r*tensor->stride[0]] += alpha*THBlas_dot(m1->size[1], THTensor_selectPtr(m1, 0, r), m1->stride[1], THTensor_selectPtr(m2, 1, c), m2->stride[0]); */
/*       } */
/*     } */
/* /\*******\/     */
/*   } */
/* }  */
