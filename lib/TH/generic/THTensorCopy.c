#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/THTensorCopy.c"

#define TH_IMPLEMENT_TENSOR_COPY(NAME, TYPE, TENSOR_TYPE)               \
  void NAME(THTensor *self, TENSOR_TYPE *tensor)                        \
  {                                                                     \
    long size = THTensor_(nElement)(self);                              \
    real *self_data = THTensor_(data)(self);                            \
    TYPE *tensor_data = TENSOR_TYPE##_data(tensor);                     \
    long i;                                                             \
                                                                        \
    THArgCheck(size == TENSOR_TYPE##_nElement(tensor), 2, "inconsistent tensor sizes"); \
                                                                        \
    for(i = 0; i < size; i++)                                           \
      self_data[i] = (real)tensor_data[i];                              \
  }                                                                     \

#else

TH_IMPLEMENT_TENSOR_COPY(THTensor_(copyByte), unsigned char, THByteTensor)
TH_IMPLEMENT_TENSOR_COPY(THTensor_(copyChar), char, THCharTensor)
TH_IMPLEMENT_TENSOR_COPY(THTensor_(copyShort), short, THShortTensor)
TH_IMPLEMENT_TENSOR_COPY(THTensor_(copyInt), int, THIntTensor)
TH_IMPLEMENT_TENSOR_COPY(THTensor_(copyLong), long, THLongTensor)
TH_IMPLEMENT_TENSOR_COPY(THTensor_(copyFloat), float, THFloatTensor)
TH_IMPLEMENT_TENSOR_COPY(THTensor_(copyDouble), double, THDoubleTensor)

#endif
