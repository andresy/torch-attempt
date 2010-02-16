#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "THTensorMathGeneric.c"
#else

#define TENSOR_IMPLEMENT_BASIC_FUNCTION(NAME, CFUNC)           \
  void THTensor_(NAME)(THTensor *self)                  \
  {                                                            \
    TH_TENSOR_APPLY(self, self_data[i] = CFUNC(self_data[i]);) \
  }                                                            \

#define TENSOR_IMPLEMENT_BASIC_FUNCTION_VALUE(NAME, CFUNC)              \
  void THTensor_(NAME)(THTensor *self, real value)               \
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


void THTensor_(zero)(THTensor *self)
{
  TH_TENSOR_APPLY(self, self_data[i] = 0;);
}

void THTensor_(add)(THTensor *self, real value)
{
  TH_TENSOR_APPLY(self, self_data[i] += value;)
}

void THTensor_(mul)(THTensor *self, real value)
{
  THBlas_(scal)(THTensor_(nElement)(self), value, THTensor_(data)(self), 1);
}

void THTensor_(div)(THTensor *self, real value)
{
  THArgCheck(value != 0, 2, "division by 0");
  THTensor_(mul)(self, 1/value);
}

void THTensor_(addTensor)(THTensor *self, real value, THTensor *tensor)
{
  THBlas_(axpy)(THTensor_(nElement)(self), value, THTensor_(data)(tensor), 1, THTensor_(data)(self), 1);
}

void THTensor_(cmul)(THTensor *self, THTensor *tensor)
{
  TH_TENSOR_APPLY2(self, tensor, self_data[i] *= tensor_data[i];);
}

void THTensor_(addcmul)(THTensor *self, real value, THTensor *src1, THTensor *src2)
{
  TH_TENSOR_APPLY3(self, src1, src2, self_data[i] += value * src1_data[i] * src2_data[i];);
}

void THTensor_(cdiv)(THTensor *self, THTensor *tensor)
{
  TH_TENSOR_APPLY2(self, tensor, self_data[i] /= tensor_data[i];);
}

void THTensor_(addcdiv)(THTensor *self, real value, THTensor *src1, THTensor *src2)
{
  TH_TENSOR_APPLY3(self, src1, src2, self_data[i] += value * src1_data[i] / src2_data[i];);
}

real THTensor_(dot)(THTensor *self, THTensor *tensor)
{
  return THBlas_(dot)(THTensor_(nElement)(self), THTensor_(data)(self), 1, THTensor_(data)(tensor), 1);
}

/* basic statistics */
real THTensor_(min)(THTensor *self)
{
  real theMin = THTensor_(get1d)(self, 0);
  TH_TENSOR_APPLY(self, if(self_data[i] < theMin) theMin = self_data[i];);
  return theMin;
}

real THTensor_(max)(THTensor *self)
{
  real theMax = THTensor_(get1d)(self, 0);
  TH_TENSOR_APPLY(self, if(self_data[i] > theMax) theMax = self_data[i];);
  return theMax;
}

real THTensor_(sum)(THTensor *self)
{
  real sum = 0;
  TH_TENSOR_APPLY(self, sum += self_data[i];);
  return sum;
}

real THTensor_(mean)(THTensor *self)
{
  THArgCheck(THTensor_(nElement)(self) > 0, 1, "cannot average an empty tensor");
  return THTensor_(sum)(self)/THTensor_(nElement)(self);
}

real THTensor_(var)(THTensor *self)
{
  real mean = THTensor_(mean)(self);
  real sum = 0;
  TH_TENSOR_APPLY(self, sum += (self_data[i] - mean)*(self_data[i] - mean););
  sum /= (THTensor_(nElement)(self)-1);
  return sum;
}

real THTensor_(std)(THTensor *self)
{
  return sqrt(THTensor_(var)(self));
}
 
real THTensor_(norm)(THTensor *self, real value)
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

real THTensor_(dist)(THTensor *self, THTensor *tensor, real value)
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

void THTensor_(addmv)(THTensor *self, real alpha, int transpose, THTensor *mat, THTensor *vec)
{
  THArgCheck(THTensor_(nDimension)(self) == 1, 1, "vector expected");
  THArgCheck(THTensor_(nDimension)(mat) == 2, 4, "matrix expected");
  THArgCheck(THTensor_(nDimension)(vec) == 1, 5, "vector expected");

  if(transpose)
  {
    THArgCheck(THTensor_(size)(mat, 1) == THTensor_(size)(vec, 1), 2, "inconsistent matrix/vector sizes");
    THArgCheck(THTensor_(size)(mat, 2) == THTensor_(size)(self, 1), 1, "inconsistent matrix/vector sizes");
  }
  else
  {
    THArgCheck(THTensor_(size)(mat, 2) == THTensor_(size)(vec, 1), 2, "inconsistent matrix/vector sizes");
    THArgCheck(THTensor_(size)(mat, 1) == THTensor_(size)(self, 1), 1, "inconsistent matrix/vector sizes");
  }

  /* BLAS is column-ordered */
  THBlas_(gemv)((transpose ? 'n' : 't'),
                THTensor_(size)(mat, 1), THTensor_(size)(mat, 0),
                alpha,
                THTensor_(data)(mat), THTensor_(stride)(mat, 1),
                THTensor_(data)(vec), 1,
                1,
                THTensor_(data)(self), 1);
}

void THTensor_(addr)(THTensor *self, real alpha, THTensor *vec1, THTensor *vec2)
{
  THArgCheck(THTensor_(nDimension)(self) == 2, 1, "matrix expected");
  THArgCheck(THTensor_(nDimension)(vec1) == 1, 3, "vector expected");
  THArgCheck(THTensor_(nDimension)(vec2) == 1, 4, "vector expected");
  THArgCheck(THTensor_(size)(self, 0) == THTensor_(size)(vec1, 1), 1, "inconsistent matrix/vector sizes");
  THArgCheck(THTensor_(size)(self, 1) == THTensor_(size)(vec2, 1), 1, "inconsistent matrix/vector sizes");

  /* BLAS is column-ordered */
  THBlas_(ger)(THTensor_(size)(self, 1), THTensor_(size)(self, 0),
               alpha,
               THTensor_(data)(vec2), 1,
               THTensor_(data)(vec1), 1,
               THTensor_(data)(self), THTensor_(stride)(self, 1));
}

void THTensor_(addmm)(THTensor *self, real alpha, int transpose1, THTensor *mat1, int transpose2, THTensor *mat2)
{
  THArgCheck(THTensor_(nDimension)(self) == 2, 1, "matrix expected");
  THArgCheck(THTensor_(nDimension)(mat1) == 2, 4, "matrix expected");
  THArgCheck(THTensor_(nDimension)(mat2) == 2, 6, "matrix expected");

  if(transpose1)
    THArgCheck(THTensor_(size)(self, 0) == THTensor_(size)(mat1, 1), 1, "inconsistent matrix/matrix sizes");
  else
    THArgCheck(THTensor_(size)(self, 0) == THTensor_(size)(mat1, 0), 1, "inconsistent matrix/matrix sizes");

  if(transpose2)
    THArgCheck(THTensor_(size)(self, 1) == THTensor_(size)(mat2, 0), 1, "inconsistent matrix/matrix sizes");
  else
    THArgCheck(THTensor_(size)(self, 1) == THTensor_(size)(mat2, 1), 1, "inconsistent matrix/matrix sizes");

  THArgCheck( (transpose1 ? THTensor_(size)(mat1, 0) : THTensor_(size)(mat1, 1)) == 
              (transpose2 ? THTensor_(size)(mat2, 1) : THTensor_(size)(mat2, 0)), 4, "inconsistent matrix/matrix sizes");

  /* BLAS is column-ordered */
  THBlas_(gemm)( (transpose2 ? 't' : 'n'),
                 (transpose1 ? 't' : 'n'),
                 THTensor_(size)(self, 1),
                 THTensor_(size)(self, 0),
                 (transpose2 ? THTensor_(size)(mat2, 1) : THTensor_(size)(mat2, 0)),
                 alpha,
                 THTensor_(data)(mat2), THTensor_(stride)(mat2, 1),
                 THTensor_(data)(mat1), THTensor_(stride)(mat1, 1),
                 1,
                 THTensor_(data)(self), THTensor_(stride)(self, 1));
}

#endif
