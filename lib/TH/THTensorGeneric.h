#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "THTensorGeneric.h"
#else

typedef struct THTensor THTensor;

THTensor* THTensor_(alloc)();

THTensor* THTensor_(allocWithTensor)(THTensor *tensor);
THTensor* THTensor_(allocWithTensorNarrow)(THTensor *tensor, long firstIndex, long size);
THTensor* THTensor_(allocWithTensorSelect)(THTensor *tensor, long sliceIndex);

THTensor* THTensor_(allocWithStorage)(THStorage *storage, long storageOffset, int nDimension, long *size);
THTensor* THTensor_(allocWithStorage1d)(THStorage *storage, long storageOffset,
                                        long size0);
THTensor* THTensor_(allocWithStorage2d)(THStorage *storage, long storageOffset,
                                        long size0, long size1);
THTensor* THTensor_(allocWithStorage3d)(THStorage *storage, long storageOffset,
                                        long size0, long size1, long size2);
THTensor* THTensor_(allocWithStorage4d)(THStorage *storage, long storageOffset,
                                        long size0, long size1, long size2, long size3);
    
THTensor* THTensor_(allocWithSize)(int nDimension, long *size);
THTensor* THTensor_(allocWithSize1d)(long size0);
THTensor* THTensor_(allocWithSize2d)(long size0, long size1);
THTensor* THTensor_(allocWithSize3d)(long size0, long size1, long size2);
THTensor* THTensor_(allocWithSize4d)(long size0, long size1, long size2, long size3);

THStorage* THTensor_(storage)(THTensor *self);
long THTensor_(storageOffset)(THTensor *self);
int THTensor_(nDimension)(THTensor *self);
long THTensor_(size)(THTensor *self, int dim);
long THTensor_(stride)(THTensor *self, int dim);

void THTensor_(resize)(THTensor *self, int nDimension, long *size);
void THTensor_(resizeAs)(THTensor *self, THTensor *src);
void THTensor_(resize1d)(THTensor *self, long size0);
void THTensor_(resize2d)(THTensor *self, long size0, long size1);
void THTensor_(resize3d)(THTensor *self, long size0, long size1, long size2);
void THTensor_(resize4d)(THTensor *self, long size0, long size1, long size2, long size3);
    
/* Slow access methods [check everything] */
void THTensor_(set1d)(THTensor *self, long x0, real value);
void THTensor_(set2d)(THTensor *self, long x0, long x1, real value);
void THTensor_(set3d)(THTensor *self, long x0, long x1, long x2, real value);
void THTensor_(set4d)(THTensor *self, long x0, long x1, long x2, long x3, real value);
  
real THTensor_(get1d)(THTensor *self, long x0);
real THTensor_(get2d)(THTensor *self, long x0, long x1);
real THTensor_(get3d)(THTensor *self, long x0, long x1, long x2);
real THTensor_(get4d)(THTensor *self, long x0, long x1, long x2, long x3);
  
real* THTensor_(data)(THTensor *self);
real* THTensor_(data1d)(THTensor *self, long i0);
real* THTensor_(data2d)(THTensor *self, long i0, long i1);
real* THTensor_(data3d)(THTensor *self, long i0, long i1, long i2);
real* THTensor_(data4d)(THTensor *self, long i0, long i1, long i2, long i3);
long THTensor_(nElement)(THTensor *self);

void THTensor_(copy)(THTensor *self, THTensor *tensor);

void THTensor_(retain)(THTensor *self);
void THTensor_(free)(THTensor *self);

#endif
