#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/THTensor.c"
#else

/* crap: avec un data, si storage change on est de la baise */

struct THTensor
{
    THStorage *storage;
    long storageOffset;
    int nDimension;
    long *size;
    long *stride;
    int refcount;    
};

void THTensor_(resize)(THTensor *self, int nDimension, long *size)
{
  int isSame = 0;
  int d;

  {
    int nDimension_ = 0;
    for(d = 0; d < nDimension; d++)
    {
      if(size[d] > 0)
        nDimension_++;
      else
        break;
    }
    nDimension = nDimension_;
  }

  if(nDimension == self->nDimension)
  {
    isSame = 1;
    for(d = 0; d < self->nDimension; d++)
    {
      if(self->size[d] != size[d])
      {
        isSame = 0;
        break;
      }
    }
  }

  if(!isSame)
  {
    long stride = 1;
    self->size   = THRealloc(self->size,   sizeof(long)*nDimension);
    self->stride = THRealloc(self->stride, sizeof(long)*nDimension);
    self->nDimension = nDimension;
    for(d = nDimension-1; d >= 0; d--)
    {
      self->size[d] = size[d];
      self->stride[d] = stride;
      stride *= size[d];
    }

    stride += self->storageOffset;
    if(stride > THStorage_(size)(self->storage))
      THStorage_(resize)(self->storage, stride);
  }
}

void THTensor_(init)(THTensor *self, THStorage *storage, long storageOffset, int nDimension, long *size)
{
  if(storage)
  {
    self->storage = storage;
    THStorage_(retain)(storage);
  }
  else
    self->storage = THStorage_(new)();
  
  self->storageOffset = (storageOffset > 0 ? storageOffset : 0);
  
  self->nDimension = 0;
  self->size = NULL;
  self->stride = NULL;
  self->refcount = 1;

  if(nDimension > 0)
    THTensor_(resize)(self, nDimension, size);
}

/* Empty init */
THTensor *THTensor_(new)(void)
{
  THTensor* self = THAlloc(sizeof(THTensor));
  THTensor_(init)(self, NULL, 0, 0, NULL);
  return self;
}

/* Pointer-copy init */
THTensor* THTensor_(newWithTensor)(THTensor *tensor)
{
  THTensor* self = THAlloc(sizeof(THTensor));
  THTensor_(init)(self, tensor->storage, tensor->storageOffset, tensor->nDimension, tensor->size);
  return self;
}

THTensor* THTensor_(newWithTensorNarrow)(THTensor *tensor, long firstIndex, long size)
{
  THTensor *self;
  int dimension = 0;

  THArgCheck( (firstIndex >= 0) && (firstIndex < tensor->size[dimension]), 2, "out of range");
  THArgCheck( (size > 0) && (firstIndex+size <= tensor->size[dimension]), 3, "out of range");

  self = THTensor_(newWithTensor)(tensor);
  self->storageOffset += firstIndex*self->stride[dimension];
  self->size[dimension] = size;

  return self;
}

THTensor* THTensor_(newWithTensorSelect)(THTensor *tensor, long sliceIndex)
{
  THTensor *self;
  int dimension = 0;
  int d;

  THArgCheck((dimension >= 0) && (dimension < tensor->nDimension), 1, "out of range");
  THArgCheck((sliceIndex >= 0) && (sliceIndex < tensor->size[dimension]), 2, "out of range");

  self = THTensor_(newWithTensor)(tensor);
  self->storageOffset += sliceIndex*self->stride[dimension];
  for(d = dimension; d < self->nDimension-1; d++)
  {
    self->size[d] = tensor->size[d+1];
    self->stride[d] = tensor->stride[d+1];
  }
  self->nDimension--;

  return self;
}

/* Storage init */
THTensor *THTensor_(newWithStorage)(THStorage *storage, long storageOffset, int nDimension, long *size)
{
  THTensor* self = THAlloc(sizeof(THTensor));
  THTensor_(init)(self, storage, storageOffset, nDimension, size);
  return self;
}

THTensor *THTensor_(newWithStorage4d)(THStorage *storage, long storageOffset,
                                             long size0, long size1, long size2, long size3)
{
  long size[4];
  size[0] = size0;
  size[1] = size1;
  size[2] = size2;
  size[3] = size3;
  return THTensor_(newWithStorage)(storage, storageOffset, 4, size);
}

THTensor *THTensor_(newWithStorage1d)(THStorage *storage, long storageOffset,
                                             long size0)
{
  return THTensor_(newWithStorage4d)(storage, storageOffset, size0, 0, 0, 0);
}

THTensor *THTensor_(newWithStorage2d)(THStorage *storage, long storageOffset,
                                             long size0, long size1)
{
  return THTensor_(newWithStorage4d)(storage, storageOffset, size0, size1, 0, 0);
}

THTensor *THTensor_(newWithStorage3d)(THStorage *storage, long storageOffset,
                                             long size0, long size1, long size2)
{
  return THTensor_(newWithStorage4d)(storage, storageOffset, size0, size1, size2, 0);
}


/* Normal init */
THTensor *THTensor_(newWithSize)(int nDimension, long *size)
{
  THTensor* self = THAlloc(sizeof(THTensor));
  THTensor_(init)(self, NULL, 0, nDimension, size);
  return self;
}

THTensor *THTensor_(newWithSize4d)(long size0, long size1, long size2, long size3)
{
  long size[4];
  size[0] = size0;
  size[1] = size1;
  size[2] = size2;
  size[3] = size3;
  return THTensor_(newWithSize)(4, size);
}

THTensor *THTensor_(newWithSize1d)(long size0)
{
  return THTensor_(newWithSize4d)(size0, 0, 0, 0);
}

THTensor *THTensor_(newWithSize2d)(long size0, long size1)
{
  return THTensor_(newWithSize4d)(size0, size1, 0, 0);
}

THTensor *THTensor_(newWithSize3d)(long size0, long size1, long size2)
{
  return THTensor_(newWithSize4d)(size0, size1, size2, 0);
}

THStorage* THTensor_(storage)(THTensor *self)
{
  return self->storage;
}

long THTensor_(storageOffset)(THTensor *self)
{
  return self->storageOffset;
}

int THTensor_(nDimension)(THTensor *self)
{
  return self->nDimension;
}

long THTensor_(size)(THTensor *self, int dim)
{
  THArgCheck( (dim >= 0) && (dim < self->nDimension), 2, "invalid dimension");
  return self->size[dim];
}

long THTensor_(stride)(THTensor *self, int dim)
{
  THArgCheck( (dim >= 0) && (dim < self->nDimension), 2, "invalid dimension");
  return self->stride[dim];
}

THLongStorage *THTensor_(newSizeOf)(THTensor *self)
{
  THLongStorage *storage = THLongStorage_newWithSize(self->nDimension);
  memcpy(THLongStorage_data(storage), self->size, sizeof(long)*self->nDimension);
  return storage;
}

THLongStorage *THTensor_(newStrideOf)(THTensor *self)
{
  THLongStorage *storage = THLongStorage_newWithSize(self->nDimension);
  memcpy(THLongStorage_data(storage), self->stride, sizeof(long)*self->nDimension);
  return storage;
}

/* Resize */
void THTensor_(resizeAs)(THTensor *self, THTensor *tensor)
{
  THTensor_(resize)(self, tensor->nDimension, tensor->size);
}

void THTensor_(resize4d)(THTensor *self, long size0, long size1, long size2, long size3)
{
  long size[4];
  size[0] = size0;
  size[1] = size1;
  size[2] = size2;
  size[3] = size3;
  THTensor_(resize)(self, 4, size);
}

void THTensor_(resize1d)(THTensor *self, long size0)
{
  THTensor_(resize4d)(self, size0, 0, 0, 0);
}

void THTensor_(resize2d)(THTensor *self, long size0, long size1)
{
  THTensor_(resize4d)(self, size0, size1, 0, 0);
}

void THTensor_(resize3d)(THTensor *self, long size0, long size1, long size2)
{
  THTensor_(resize4d)(self, size0, size1, size2, 0);
}

void THTensor_(set1d)(THTensor *self, long x0, real value)
{
  THArgCheck(self->nDimension == 1, 1, "tensor must have one dimension");
  THArgCheck( (x0 >= 0) && (x0 < self->size[0]), 2, "out of range");
  (THStorage_(data)(self->storage)+self->storageOffset)[x0] = value;
}

real THTensor_(get1d)(THTensor *self, long x0)
{
  THArgCheck(self->nDimension >= 1, 1, "tensor must have (at least) one dimension");
  THArgCheck( (x0 >= 0) && (x0 < self->size[0]), 2, "out of range");
  return (THStorage_(data)(self->storage)+self->storageOffset)[x0];
}

void THTensor_(set2d)(THTensor *self, long x0, long x1, real value)
{
  THArgCheck(self->nDimension >= 2, 1, "tensor must have (at least) two dimensions");
  THArgCheck((x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]), 2, "out of range");
  (THStorage_(data)(self->storage)+self->storageOffset)[x0+x1*self->stride[1]] = value;
}

real THTensor_(get2d)(THTensor *self, long x0, long x1)
{
  THArgCheck(self->nDimension >= 2, 1, "tensor must have (at least) two dimensions");
  THArgCheck((x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]), 2, "out of range");
  return (THStorage_(data)(self->storage)+self->storageOffset)[x0+x1*self->stride[1]];
}

void THTensor_(set3d)(THTensor *self, long x0, long x1, long x2, real value)
{
  THArgCheck(self->nDimension >= 3, 1, "tensor must have (at least) three dimensions");
  THArgCheck( (x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]) && (x2 >= 0) && (x2 < self->size[2]), 2, "out of range");
  (THStorage_(data)(self->storage)+self->storageOffset)[x0+x1*self->stride[1]+x2*self->stride[2]] = value;
}

real THTensor_(get3d)(THTensor *self, long x0, long x1, long x2)
{
  THArgCheck(self->nDimension >= 3, 1, "tensor must have (at least) three dimensions");
  THArgCheck( (x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]) && (x2 >= 0) && (x2 < self->size[2]), 2, "out of range");
  return (THStorage_(data)(self->storage)+self->storageOffset)[x0+x1*self->stride[1]+x2*self->stride[2]];
}

void THTensor_(set4d)(THTensor *self, long x0, long x1, long x2, long x3, real value)
{
  THArgCheck(self->nDimension >= 4, 1, "tensor must have (at least) four dimensions");
  THArgCheck((x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]) && (x2 >= 0) && (x2 < self->size[2]) && (x3 >= 0) && (x3 < self->size[3]), 2, "out of range");
  (THStorage_(data)(self->storage)+self->storageOffset)[x0+x1*self->stride[1]+x2*self->stride[2]+x3*self->stride[3]] = value;
}

real THTensor_(get4d)(THTensor *self, long x0, long x1, long x2, long x3)
{
  THArgCheck(self->nDimension >= 4, 1, "tensor must have (at least) four dimensions");
  THArgCheck((x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]) && (x2 >= 0) && (x2 < self->size[2]) && (x3 >= 0) && (x3 < self->size[3]), 2, "out of range");
  return (THStorage_(data)(self->storage)+self->storageOffset)[x0+x1*self->stride[1]+x2*self->stride[2]+x3*self->stride[3]];
}

real* THTensor_(data)(THTensor *self)
{
  return THStorage_(data)(self->storage)+self->storageOffset;
}
    
real* THTensor_(data1d)(THTensor *self, long i0)
{
  return THStorage_(data)(self->storage)+self->storageOffset+i0*self->stride[0];
}

real* THTensor_(data2d)(THTensor *self, long i0, long i1)
{
  return THStorage_(data)(self->storage)+self->storageOffset+i0*self->stride[0]+i1*self->stride[1];
}

real* THTensor_(data3d)(THTensor *self, long i0, long i1, long i2)
{
  return THStorage_(data)(self->storage)+self->storageOffset+i0*self->stride[0]+i1*self->stride[1]+i2*self->stride[2];
}

real* THTensor_(data4d)(THTensor *self, long i0, long i1, long i2, long i3)
{
  return THStorage_(data)(self->storage)+self->storageOffset+i0*self->stride[0]+i1*self->stride[1]+i2*self->stride[2]+i3*self->stride[3];
}

long THTensor_(nElement)(THTensor *self)
{
  if(self->nDimension == 0)
    return 0;
  else
    return self->stride[0]*self->size[0];
}

void THTensor_(copy)(THTensor *self, THTensor *tensor)
{
  if(self == tensor)
    return;

  THArgCheck(THTensor_(nElement)(self) == THTensor_(nElement)(tensor), 2, "tensor sizes do not match");
  
  if(self->storage == tensor->storage)
    memmove(THTensor_(data)(self), THTensor_(data)(tensor), sizeof(real)*THTensor_(nElement)(self));
  else
    memcpy(THTensor_(data)(self), THTensor_(data)(tensor), sizeof(real)*THTensor_(nElement)(self));
}

void THTensor_(retain)(THTensor *self)
{
  if(self)
    ++self->refcount;
}

void THTensor_(free)(THTensor *self)
{
  if(--self->refcount == 0)
  {
    THFree(self->size);
    THFree(self->stride);
    THStorage_(free)(self->storage);
    THFree(self);
  }
}

#endif
