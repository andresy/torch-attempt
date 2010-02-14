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

static void THTensor_(resize)(THTensor *self, int nDimension, long *size)
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
    for(d = 0; d < nDimension; d++)
    {
      self->size[d] = size[d];
      self->stride[d] = stride;
      stride *= size[d];
    }

    stride += self->storageOffset;
    if(stride > THStorageAPI.size(self->storage))
      THStorageAPI.resize(self->storage, stride);
  }
}

static THTensor* THTensor_(init)(THStorage *storage, long storageOffset, int nDimension, long *size)
{
  THTensor* self = THAlloc(sizeof(THTensor));

  if(storage)
    self->storage = storage;
  else
    self->storage = THStorageAPI.alloc();
  
  self->storageOffset = (storageOffset > 0 ? storageOffset : 0);
  
  self->nDimension = 0;
  self->size = NULL;
  self->stride = NULL;
  self->refcount = 1;

  if(nDimension > 0)
    THTensor_(resize)(self, nDimension, size);

  return self;
}

/* Empty init */
static THTensor *THTensor_(alloc)()
{
  return THTensor_(init)(NULL, 0, 0, NULL);
}

/* Pointer-copy init */
static THTensor* THTensor_(allocWithTensor)(THTensor *tensor)
{
  return THTensor_(init)(tensor->storage, tensor->storageOffset, tensor->nDimension, tensor->size);
}

static THTensor* THTensor_(allocWithTensorNarrow)(THTensor *tensor, long firstIndex, long size)
{
  THTensor *self;
  int dimension = tensor->nDimension-1;

  THArgCheck( (firstIndex >= 0) && (firstIndex < tensor->size[dimension]), 2, "out of range");
  THArgCheck( (size > 0) && (firstIndex+size <= tensor->size[dimension]), 3, "out of range");

  self = THTensor_(allocWithTensor)(tensor);
  self->storageOffset += firstIndex*self->stride[dimension];
  self->size[dimension] = size;

  return self;
}

static THTensor* THTensor_(allocWithTensorSelect)(THTensor *tensor, long sliceIndex)
{
  THTensor *self;
  int dimension = tensor->nDimension-1;
  int d;

  THArgCheck((dimension >= 0) && (dimension < tensor->nDimension), 1, "out of range");
  THArgCheck((sliceIndex >= 0) && (sliceIndex < tensor->size[dimension]), 2, "out of range");

  self = THTensor_(allocWithTensor)(tensor);
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
static THTensor *THTensor_(allocWithStorage)(THStorage *storage, long storageOffset, int nDimension, long *size)
{
  return THTensor_(init)(storage, storageOffset, nDimension, size);
}

static THTensor *THTensor_(allocWithStorage4d)(THStorage *storage, long storageOffset,
                                             long size0, long size1, long size2, long size3)
{
  long size[4] = {size0, size1, size2, size3};
  return THTensor_(allocWithStorage)(storage, storageOffset, 4, size);
}

static THTensor *THTensor_(allocWithStorage1d)(THStorage *storage, long storageOffset,
                                             long size0)
{
  return THTensor_(allocWithStorage4d)(storage, storageOffset, size0, 0, 0, 0);
}

static THTensor *THTensor_(allocWithStorage2d)(THStorage *storage, long storageOffset,
                                             long size0, long size1)
{
  return THTensor_(allocWithStorage4d)(storage, storageOffset, size0, size1, 0, 0);
}

static THTensor *THTensor_(allocWithStorage3d)(THStorage *storage, long storageOffset,
                                             long size0, long size1, long size2)
{
  return THTensor_(allocWithStorage4d)(storage, storageOffset, size0, size1, size2, 0);
}


/* Normal init */
static THTensor *THTensor_(allocWithSize)(int nDimension, long *size)
{
  return THTensor_(init)(NULL, 0, nDimension, size);
}

static THTensor *THTensor_(allocWithSize4d)(long size0, long size1, long size2, long size3)
{
  long size[4] = {size0, size1, size2, size3};
  return THTensor_(allocWithSize)(4, size);
}

static THTensor *THTensor_(allocWithSize1d)(long size0)
{
  return THTensor_(allocWithSize4d)(size0, 0, 0, 0);
}

static THTensor *THTensor_(allocWithSize2d)(long size0, long size1)
{
  return THTensor_(allocWithSize4d)(size0, size1, 0, 0);
}

static THTensor *THTensor_(allocWithSize3d)(long size0, long size1, long size2)
{
  return THTensor_(allocWithSize4d)(size0, size1, size2, 0);
}

static THStorage* THTensor_(storage)(THTensor *self)
{
  return self->storage;
}

static long THTensor_(storageOffset)(THTensor *self)
{
  return self->storageOffset;
}

static int THTensor_(nDimension)(THTensor *self)
{
  return self->nDimension;
}

static long THTensor_(size)(THTensor *self, int dim)
{
  THArgCheck( (dim >= 0) && (dim < self->nDimension), 2, "invalid dimension");
  return self->size[dim];
}

static long THTensor_(stride)(THTensor *self, int dim)
{
  THArgCheck( (dim >= 0) && (dim < self->nDimension), 2, "invalid dimension");
  return self->stride[dim];
}

/* Resize */
static void THTensor_(resizeAs)(THTensor *self, THTensor *tensor)
{
  THTensor_(resize)(self, tensor->nDimension, tensor->size);
}

static void THTensor_(resize4d)(THTensor *self, long size0, long size1, long size2, long size3)
{
  long size[4] = {size0, size1, size2, size3};
  THTensor_(resize)(self, 4, size);
}

static void THTensor_(resize1d)(THTensor *self, long size0)
{
  THTensor_(resize4d)(self, size0, 0, 0, 0);
}

static void THTensor_(resize2d)(THTensor *self, long size0, long size1)
{
  THTensor_(resize4d)(self, size0, size1, 0, 0);
}

static void THTensor_(resize3d)(THTensor *self, long size0, long size1, long size2)
{
  THTensor_(resize4d)(self, size0, size1, size2, 0);
}

static void THTensor_(set1d)(THTensor *self, long x0, real value)
{
  THArgCheck(self->nDimension == 1, 1, "tensor must have one dimension");
  THArgCheck( (x0 >= 0) && (x0 < self->size[0]), 2, "out of range");
  (THStorageAPI.data(self->storage)+self->storageOffset)[x0] = value;
}

static real THTensor_(get1d)(THTensor *self, long x0)
{
  THArgCheck(self->nDimension == 1, 1, "tensor must have one dimension");
  THArgCheck( (x0 >= 0) && (x0 < self->size[0]), 2, "out of range");
  return (THStorageAPI.data(self->storage)+self->storageOffset)[x0];
}

static void THTensor_(set2d)(THTensor *self, long x0, long x1, real value)
{
  THArgCheck(self->nDimension == 2, 1, "tensor must have two dimensions");
  THArgCheck((x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]), 2, "out of range");
  (THStorageAPI.data(self->storage)+self->storageOffset)[x0+x1*self->stride[1]] = value;
}

static real THTensor_(get2d)(THTensor *self, long x0, long x1)
{
  THArgCheck(self->nDimension == 2, 1, "tensor must have two dimensions");
  THArgCheck((x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]), 2, "out of range");
  return (THStorageAPI.data(self->storage)+self->storageOffset)[x0+x1*self->stride[1]];
}

static void THTensor_(set3d)(THTensor *self, long x0, long x1, long x2, real value)
{
  THArgCheck(self->nDimension == 3, 1, "tensor must have three dimensions");
  THArgCheck( (x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]) && (x2 >= 0) && (x2 < self->size[2]), 2, "out of range");
  (THStorageAPI.data(self->storage)+self->storageOffset)[x0+x1*self->stride[1]+x2*self->stride[2]] = value;
}

static real THTensor_(get3d)(THTensor *self, long x0, long x1, long x2)
{
  THArgCheck(self->nDimension == 3, 1, "tensor must have three dimensions");
  THArgCheck( (x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]) && (x2 >= 0) && (x2 < self->size[2]), 2, "out of range");
  return (THStorageAPI.data(self->storage)+self->storageOffset)[x0+x1*self->stride[1]+x2*self->stride[2]];
}

static void THTensor_(set4d)(THTensor *self, long x0, long x1, long x2, long x3, real value)
{
  THArgCheck(self->nDimension == 4, 1, "tensor must have four dimensions");
  THArgCheck((x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]) && (x2 >= 0) && (x2 < self->size[2]) && (x3 >= 0) && (x3 < self->size[3]), 2, "out of range");
  (THStorageAPI.data(self->storage)+self->storageOffset)[x0+x1*self->stride[1]+x2*self->stride[2]+x3*self->stride[3]] = value;
}

static real THTensor_(get4d)(THTensor *self, long x0, long x1, long x2, long x3)
{
  THArgCheck(self->nDimension == 4, 1, "tensor must have four dimensions");
  THArgCheck((x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]) && (x2 >= 0) && (x2 < self->size[2]) && (x3 >= 0) && (x3 < self->size[3]), 2, "out of range");
  return (THStorageAPI.data(self->storage)+self->storageOffset)[x0+x1*self->stride[1]+x2*self->stride[2]+x3*self->stride[3]];
}

static real* THTensor_(data)(THTensor *self)
{
  return THStorageAPI.data(self->storage)+self->storageOffset;
}
    
static real* THTensor_(data1d)(THTensor *self, long i0)
{
  return THStorageAPI.data(self->storage)+self->storageOffset+i0*self->stride[0];
}

static real* THTensor_(data2d)(THTensor *self, long i0, long i1)
{
  return THStorageAPI.data(self->storage)+self->storageOffset+i0*self->stride[0]+i1*self->stride[1];
}

static real* THTensor_(data3d)(THTensor *self, long i0, long i1, long i2)
{
  return THStorageAPI.data(self->storage)+self->storageOffset+i0*self->stride[0]+i1*self->stride[1]+i2*self->stride[2];
}

static real* THTensor_(data4d)(THTensor *self, long i0, long i1, long i2, long i3)
{
  return THStorageAPI.data(self->storage)+self->storageOffset+i0*self->stride[0]+i1*self->stride[1]+i2*self->stride[2]+i3*self->stride[3];
}

static long THTensor_(nElement)(THTensor *self)
{
  int nDimension = self->nDimension;
  if(nDimension == 0)
    return 0;
  else
    return self->stride[self->nDimension-1]*self->size[nDimension-1];
}

static void THTensor_(copy)(THTensor *self, THTensor *tensor)
{
  if(self == tensor)
    return;

  THArgCheck(THTensor_(nElement)(self) == THTensor_(nElement)(tensor), 2, "tensor sizes do not match");
  
  if(self->storage == tensor->storage)
    memmove(THTensor_(data)(self), THTensor_(data)(tensor), sizeof(real)*THTensor_(nElement)(self));
  else
    memcpy(THTensor_(data)(self), THTensor_(data)(tensor), sizeof(real)*THTensor_(nElement)(self));
}

static void THTensor_(retain)(THTensor *self)
{
  if(self)
    ++self->refcount;
}

static void THTensor_(free)(THTensor *self)
{
  if(--self->refcount == 0)
  {
    THFree(self->size);
    THFree(self->stride);
    THStorageAPI.free(self->storage);
    THFree(self);
  }
}

/*******************************************************************************/

const struct THTensorAPI THTensorAPI = {
  THTensor_(alloc),
  THTensor_(allocWithTensor),
  THTensor_(allocWithTensorNarrow),
  THTensor_(allocWithTensorSelect),
  THTensor_(allocWithStorage),
  THTensor_(allocWithStorage1d),
  THTensor_(allocWithStorage2d),
  THTensor_(allocWithStorage3d),
  THTensor_(allocWithStorage4d),
  THTensor_(allocWithSize),
  THTensor_(allocWithSize1d),
  THTensor_(allocWithSize2d),
  THTensor_(allocWithSize3d),
  THTensor_(allocWithSize4d),
  THTensor_(storage),
  THTensor_(storageOffset),
  THTensor_(nDimension),
  THTensor_(size),
  THTensor_(stride),
  THTensor_(resize),
  THTensor_(resizeAs),
  THTensor_(resize1d),
  THTensor_(resize2d),
  THTensor_(resize3d),
  THTensor_(resize4d),
  THTensor_(set1d),
  THTensor_(set2d),
  THTensor_(set3d),
  THTensor_(set4d),
  THTensor_(get1d),
  THTensor_(get2d),
  THTensor_(get3d),
  THTensor_(get4d),
  THTensor_(data),
  THTensor_(data1d),
  THTensor_(data2d),
  THTensor_(data3d),
  THTensor_(data4d),
  THTensor_(nElement),
  THTensor_(copy),
  THTensor_(retain),
  THTensor_(free)
};
