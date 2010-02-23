#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/THStorageCopy.c"

#define TH_IMPLEMENT_STORAGE_COPY(NAME, TYPE, STORAGE_TYPE)             \
  void NAME(THStorage *self, STORAGE_TYPE *storage)                     \
  {                                                                     \
    long size = THStorage_(size)(self);                                 \
    real *self_data = THStorage_(data)(self);                           \
    TYPE *storage_data = STORAGE_TYPE##_data(storage);                  \
    long i;                                                             \
                                                                        \
    THArgCheck(size == STORAGE_TYPE##_size(storage), 2, "inconsistent storage sizes"); \
                                                                        \
    for(i = 0; i < size; i++)                                           \
      self_data[i] = (real)storage_data[i];                             \
  }                                                                     \

#else

TH_IMPLEMENT_STORAGE_COPY(THStorage_(copyByte), unsigned char, THByteStorage)
TH_IMPLEMENT_STORAGE_COPY(THStorage_(copyChar), char, THCharStorage)
TH_IMPLEMENT_STORAGE_COPY(THStorage_(copyShort), short, THShortStorage)
TH_IMPLEMENT_STORAGE_COPY(THStorage_(copyInt), int, THIntStorage)
TH_IMPLEMENT_STORAGE_COPY(THStorage_(copyLong), long, THLongStorage)
TH_IMPLEMENT_STORAGE_COPY(THStorage_(copyFloat), float, THFloatStorage)
TH_IMPLEMENT_STORAGE_COPY(THStorage_(copyDouble), double, THDoubleStorage)

#endif
