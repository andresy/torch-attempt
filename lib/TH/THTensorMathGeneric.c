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

#define TH_TENSOR_APPLY3(self, tensor1, tensor2, code)                  \
  {                                                                     \
    real *self##_data = THTensorAPI.data(self);                         \
    long self##_size = THTensorAPI.nElement(self);                      \
    real *tensor1##_data = THTensorAPI.data(tensor1);                   \
    long tensor1##_size = THTensorAPI.nElement(tensor1);                \
    real *tensor2##_data = THTensorAPI.data(tensor2);                   \
    long tensor2##_size = THTensorAPI.nElement(tensor2);                \
    int i;                                                              \
                                                                        \
    THArgCheck(self##_size == tensor1##_size, 1, "inconsistent tensor sizes"); \
    THArgCheck(self##_size == tensor2##_size, 1, "inconsistent tensor sizes"); \
    for(i = 0; i < self##_size; i++)                                    \
    {                                                                   \
      code                                                              \
    }                                                                   \
  }                                                                     \

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
  THBlasAPI.scal(THTensorAPI.nElement(self), value, THTensorAPI.data(self), 1);
}

void THTensor_(div)(THTensor *self, real value)
{
  THArgCheck(value != 0, 2, "division by 0");
  THTensor_(mul)(self, 1/value);
}

void THTensor_(addTensor)(THTensor *self, real value, THTensor *tensor)
{
  THBlasAPI.axpy(THTensorAPI.nElement(self), value, THTensorAPI.data(tensor), 1, THTensorAPI.data(self), 1);
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
  return THBlasAPI.dot(THTensorAPI.nElement(self), THTensorAPI.data(self), 1, THTensorAPI.data(tensor), 1);
}

/* basic statistics */
real THTensor_(min)(THTensor *self)
{
  real theMin = THTensorAPI.get1d(self, 0);
  TH_TENSOR_APPLY(self, if(self_data[i] < theMin) theMin = self_data[i];);
  return theMin;
}

real THTensor_(max)(THTensor *self)
{
  real theMax = THTensorAPI.get1d(self, 0);
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
  THArgCheck(THTensorAPI.nElement(self) > 0, 1, "cannot average an empty tensor");
  return THTensor_(sum)(self)/THTensorAPI.nElement(self);
}

real THTensor_(var)(THTensor *self)
{
  real mean = THTensor_(mean)(self);
  real sum = 0;
  TH_TENSOR_APPLY(self, sum += (self_data[i] - mean)*(self_data[i] - mean););
  sum /= (THTensorAPI.nElement(self)-1);
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
  THArgCheck(THTensorAPI.nDimension(self) == 1, 1, "vector expected");
  THArgCheck(THTensorAPI.nDimension(mat) == 2, 4, "matrix expected");
  THArgCheck(THTensorAPI.nDimension(vec) == 1, 5, "vector expected");

  if(transpose)
  {
    THArgCheck(THTensorAPI.size(mat, 1) == THTensorAPI.size(vec, 1), 2, "inconsistent matrix/vector sizes");
    THArgCheck(THTensorAPI.size(mat, 2) == THTensorAPI.size(self, 1), 1, "inconsistent matrix/vector sizes");
  }
  else
  {
    THArgCheck(THTensorAPI.size(mat, 2) == THTensorAPI.size(vec, 1), 2, "inconsistent matrix/vector sizes");
    THArgCheck(THTensorAPI.size(mat, 1) == THTensorAPI.size(self, 1), 1, "inconsistent matrix/vector sizes");
  }

  /* BLAS is column-ordered */
  THBlasAPI.gemv((transpose ? 'n' : 't'),
                 THTensorAPI.size(mat, 1), THTensorAPI.size(mat, 0),
                 alpha,
                 THTensorAPI.data(mat), THTensorAPI.stride(mat, 1),
                 THTensorAPI.data(vec), 1,
                 1,
                 THTensorAPI.data(self), 1);
}

void THTensor_(addr)(THTensor *self, real alpha, THTensor *vec1, THTensor *vec2)
{
  THArgCheck(THTensorAPI.nDimension(self) == 2, 1, "matrix expected");
  THArgCheck(THTensorAPI.nDimension(vec1) == 1, 3, "vector expected");
  THArgCheck(THTensorAPI.nDimension(vec2) == 1, 4, "vector expected");
  THArgCheck(THTensorAPI.size(self, 0) == THTensorAPI.size(vec1, 1), 1, "inconsistent matrix/vector sizes");
  THArgCheck(THTensorAPI.size(self, 1) == THTensorAPI.size(vec2, 1), 1, "inconsistent matrix/vector sizes");

  /* BLAS is column-ordered */
  THBlasAPI.ger(THTensorAPI.size(self, 1), THTensorAPI.size(self, 0),
                alpha,
                THTensorAPI.data(vec2), 1,
                THTensorAPI.data(vec1), 1,
                THTensorAPI.data(self), THTensorAPI.stride(self, 1));
}

void THTensor_(addmm)(THTensor *self, real alpha, int transpose1, THTensor *mat1, int transpose2, THTensor *mat2)
{
  THArgCheck(THTensorAPI.nDimension(self) == 2, 1, "matrix expected");
  THArgCheck(THTensorAPI.nDimension(mat1) == 2, 4, "matrix expected");
  THArgCheck(THTensorAPI.nDimension(mat2) == 2, 6, "matrix expected");

  if(transpose1)
    THArgCheck(THTensorAPI.size(self, 0) == THTensorAPI.size(mat1, 1), 1, "inconsistent matrix/matrix sizes");
  else
    THArgCheck(THTensorAPI.size(self, 0) == THTensorAPI.size(mat1, 0), 1, "inconsistent matrix/matrix sizes");

  if(transpose2)
    THArgCheck(THTensorAPI.size(self, 1) == THTensorAPI.size(mat2, 0), 1, "inconsistent matrix/matrix sizes");
  else
    THArgCheck(THTensorAPI.size(self, 1) == THTensorAPI.size(mat2, 1), 1, "inconsistent matrix/matrix sizes");

  THArgCheck( (transpose1 ? THTensorAPI.size(mat1, 0) : THTensorAPI.size(mat1, 1)) == 
              (transpose2 ? THTensorAPI.size(mat2, 1) : THTensorAPI.size(mat2, 0)), 4, "inconsistent matrix/matrix sizes");

  /* BLAS is column-ordered */
  THBlasAPI.gemm( (transpose2 ? 't' : 'n'),
                  (transpose1 ? 't' : 'n'),
                  THTensorAPI.size(self, 1),
                  THTensorAPI.size(self, 0),
                  (transpose2 ? THTensorAPI.size(mat2, 1) : THTensorAPI.size(mat2, 0)),
                  alpha,
                  THTensorAPI.data(mat2), THTensorAPI.stride(mat2, 1),
                  THTensorAPI.data(mat1), THTensorAPI.stride(mat1, 1),
                  1,
                  THTensorAPI.data(self), THTensorAPI.stride(self, 1));
}
