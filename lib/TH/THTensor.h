typedef struct THTensor THTensor;

extern const struct THTensorAPI
{
  THTensor* (*alloc)();

  THTensor* (*allocWithTensor)(THTensor *tensor);
  THTensor* (*allocWithTensorNarrow)(THTensor *tensor, long firstIndex, long size);
  THTensor* (*allocWithTensorSelect)(THTensor *tensor, long sliceIndex);

  THTensor* (*allocWithStorage)(THStorage *storage, long storageOffset, int nDimension, long *size);
  THTensor* (*allocWithStorage1d)(THStorage *storage, long storageOffset,
                                 long size0);
  THTensor* (*allocWithStorage2d)(THStorage *storage, long storageOffset,
                                 long size0, long size1);
  THTensor* (*allocWithStorage3d)(THStorage *storage, long storageOffset,
                                 long size0, long size1, long size2);
  THTensor* (*allocWithStorage4d)(THStorage *storage, long storageOffset,
                                 long size0, long size1, long size2, long size3);
    
  THTensor* (*allocWithSize)(int nDimension, long *size);
  THTensor* (*allocWithSize1d)(long size0);
  THTensor* (*allocWithSize2d)(long size0, long size1);
  THTensor* (*allocWithSize3d)(long size0, long size1, long size2);
  THTensor* (*allocWithSize4d)(long size0, long size1, long size2, long size3);

  THStorage* (*storage)(THTensor *self);
  long (*storageOffset)(THTensor *self);
  int (*nDimension)(THTensor *self);
  long (*size)(THTensor *self, int dim);
  long (*stride)(THTensor *self, int dim);

  void (*resize)(THTensor *self, int nDimension, long *size);
  void (*resizeAs)(THTensor *self, THTensor *src);
  void (*resize1d)(THTensor *self, long size0);
  void (*resize2d)(THTensor *self, long size0, long size1);
  void (*resize3d)(THTensor *self, long size0, long size1, long size2);
  void (*resize4d)(THTensor *self, long size0, long size1, long size2, long size3);
    
  /* Slow access methods [check everything] */
  void (*set1d)(THTensor *self, long x0, real value);
  void (*set2d)(THTensor *self, long x0, long x1, real value);
  void (*set3d)(THTensor *self, long x0, long x1, long x2, real value);
  void (*set4d)(THTensor *self, long x0, long x1, long x2, long x3, real value);
  
  real (*get1d)(THTensor *self, long x0);
  real (*get2d)(THTensor *self, long x0, long x1);
  real (*get3d)(THTensor *self, long x0, long x1, long x2);
  real (*get4d)(THTensor *self, long x0, long x1, long x2, long x3);
  
  real* (*data)(THTensor *self);
  real* (*data1d)(THTensor *self, long i0);
  real* (*data2d)(THTensor *self, long i0, long i1);
  real* (*data3d)(THTensor *self, long i0, long i1, long i2);
  real* (*data4d)(THTensor *self, long i0, long i1, long i2, long i3);
  long (*nElement)(THTensor *self);

  void (*retain)(THTensor *self);
  void (*free)(THTensor *self);
    
} THTensorAPI;
