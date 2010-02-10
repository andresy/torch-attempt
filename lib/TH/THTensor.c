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

static void THTensor_resize(THTensor *self, int nDimension, long *size)
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

static THTensor* THTensor_init(THStorage *storage, long storageOffset, int nDimension, long *size)
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
    THTensor_resize(self, nDimension, size);

  return self;
}

/* Empty init */
static THTensor *THTensor_alloc()
{
  return THTensor_init(NULL, 0, 0, NULL);
}

/* Pointer-copy init */
static THTensor* THTensor_allocWithTensor(THTensor *tensor)
{
  return THTensor_init(tensor->storage, tensor->storageOffset, tensor->nDimension, tensor->size);
}

static THTensor* THTensor_allocWithTensorNarrow(THTensor *tensor, long firstIndex, long size)
{
  THTensor *self;
  int dimension = tensor->nDimension-1;

  THArgCheck( (firstIndex >= 0) && (firstIndex < tensor->size[dimension]), 2, "out of range");
  THArgCheck( (size > 0) && (firstIndex+size <= tensor->size[dimension]), 3, "out of range");

  self = THTensor_allocWithTensor(tensor);
  self->storageOffset += firstIndex*self->stride[dimension];
  self->size[dimension] = size;

  return self;
}

static THTensor* THTensor_allocWithTensorSelect(THTensor *tensor, long sliceIndex)
{
  THTensor *self;
  int dimension = tensor->nDimension-1;
  int d;

  THArgCheck((dimension >= 0) && (dimension < tensor->nDimension), 1, "out of range");
  THArgCheck((sliceIndex >= 0) && (sliceIndex < tensor->size[dimension]), 2, "out of range");

  self = THTensor_allocWithTensor(tensor);
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
static THTensor *THTensor_allocWithStorage(THStorage *storage, long storageOffset, int nDimension, long *size)
{
  return THTensor_init(storage, storageOffset, nDimension, size);
}

static THTensor *THTensor_allocWithStorage4d(THStorage *storage, long storageOffset,
                                             long size0, long size1, long size2, long size3)
{
  long size[4] = {size0, size1, size2, size3};
  return THTensor_allocWithStorage(storage, storageOffset, 4, size);
}

static THTensor *THTensor_allocWithStorage1d(THStorage *storage, long storageOffset,
                                             long size0)
{
  return THTensor_allocWithStorage4d(storage, storageOffset, size0, 0, 0, 0);
}

static THTensor *THTensor_allocWithStorage2d(THStorage *storage, long storageOffset,
                                             long size0, long size1)
{
  return THTensor_allocWithStorage4d(storage, storageOffset, size0, size1, 0, 0);
}

static THTensor *THTensor_allocWithStorage3d(THStorage *storage, long storageOffset,
                                             long size0, long size1, long size2)
{
  return THTensor_allocWithStorage4d(storage, storageOffset, size0, size1, size2, 0);
}


/* Normal init */
static THTensor *THTensor_allocWithSize(int nDimension, long *size)
{
  return THTensor_init(NULL, 0, nDimension, size);
}

static THTensor *THTensor_allocWithSize4d(long size0, long size1, long size2, long size3)
{
  long size[4] = {size0, size1, size2, size3};
  return THTensor_allocWithSize(4, size);
}

static THTensor *THTensor_allocWithSize1d(long size0)
{
  return THTensor_allocWithSize4d(size0, 0, 0, 0);
}

static THTensor *THTensor_allocWithSize2d(long size0, long size1)
{
  return THTensor_allocWithSize4d(size0, size1, 0, 0);
}

static THTensor *THTensor_allocWithSize3d(long size0, long size1, long size2)
{
  return THTensor_allocWithSize4d(size0, size1, size2, 0);
}

static THStorage* THTensor_storage(THTensor *self)
{
  return self->storage;
}

static long THTensor_storageOffset(THTensor *self)
{
  return self->storageOffset;
}

static int THTensor_nDimension(THTensor *self)
{
  return self->nDimension;
}

static long THTensor_size(THTensor *self, int dim)
{
  THArgCheck( (dim >= 0) && (dim < self->nDimension), 2, "invalid dimension");
  return self->size[dim];
}

static long THTensor_stride(THTensor *self, int dim)
{
  THArgCheck( (dim >= 0) && (dim < self->nDimension), 2, "invalid dimension");
  return self->stride[dim];
}

/* Resize */
static void THTensor_resizeAs(THTensor *self, THTensor *tensor)
{
  THTensor_resize(self, tensor->nDimension, tensor->size);
}

static void THTensor_resize4d(THTensor *self, long size0, long size1, long size2, long size3)
{
  long size[4] = {size0, size1, size2, size3};
  THTensor_resize(self, 4, size);
}

static void THTensor_resize1d(THTensor *self, long size0)
{
  THTensor_resize4d(self, size0, 0, 0, 0);
}

static void THTensor_resize2d(THTensor *self, long size0, long size1)
{
  THTensor_resize4d(self, size0, size1, 0, 0);
}

static void THTensor_resize3d(THTensor *self, long size0, long size1, long size2)
{
  THTensor_resize4d(self, size0, size1, size2, 0);
}

static void THTensor_set1d(THTensor *self, long x0, real value)
{
  THArgCheck(self->nDimension == 1, 1, "tensor must have one dimension");
  THArgCheck( (x0 >= 0) && (x0 < self->size[0]), 2, "out of range");
  (THStorageAPI.data(self->storage)+self->storageOffset)[x0] = value;
}

static real THTensor_get1d(THTensor *self, long x0)
{
  THArgCheck(self->nDimension == 1, 1, "tensor must have one dimension");
  THArgCheck( (x0 >= 0) && (x0 < self->size[0]), 2, "out of range");
  return (THStorageAPI.data(self->storage)+self->storageOffset)[x0];
}

static void THTensor_set2d(THTensor *self, long x0, long x1, real value)
{
  THArgCheck(self->nDimension == 2, 1, "tensor must have two dimensions");
  THArgCheck((x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]), 2, "out of range");
  (THStorageAPI.data(self->storage)+self->storageOffset)[x0+x1*self->stride[1]] = value;
}

static real THTensor_get2d(THTensor *self, long x0, long x1)
{
  THArgCheck(self->nDimension == 2, 1, "tensor must have two dimensions");
  THArgCheck((x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]), 2, "out of range");
  return (THStorageAPI.data(self->storage)+self->storageOffset)[x0+x1*self->stride[1]];
}

static void THTensor_set3d(THTensor *self, long x0, long x1, long x2, real value)
{
  THArgCheck(self->nDimension == 3, 1, "tensor must have three dimensions");
  THArgCheck( (x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]) && (x2 >= 0) && (x2 < self->size[2]), 2, "out of range");
  (THStorageAPI.data(self->storage)+self->storageOffset)[x0+x1*self->stride[1]+x2*self->stride[2]] = value;
}

static real THTensor_get3d(THTensor *self, long x0, long x1, long x2)
{
  THArgCheck(self->nDimension == 3, 1, "tensor must have three dimensions");
  THArgCheck( (x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]) && (x2 >= 0) && (x2 < self->size[2]), 2, "out of range");
  return (THStorageAPI.data(self->storage)+self->storageOffset)[x0+x1*self->stride[1]+x2*self->stride[2]];
}

static void THTensor_set4d(THTensor *self, long x0, long x1, long x2, long x3, real value)
{
  THArgCheck(self->nDimension == 4, 1, "tensor must have four dimensions");
  THArgCheck((x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]) && (x2 >= 0) && (x2 < self->size[2]) && (x3 >= 0) && (x3 < self->size[3]), 2, "out of range");
  (THStorageAPI.data(self->storage)+self->storageOffset)[x0+x1*self->stride[1]+x2*self->stride[2]+x3*self->stride[3]] = value;
}

static real THTensor_get4d(THTensor *self, long x0, long x1, long x2, long x3)
{
  THArgCheck(self->nDimension == 4, 1, "tensor must have four dimensions");
  THArgCheck((x0 >= 0) && (x0 < self->size[0]) && (x1 >= 0) && (x1 < self->size[1]) && (x2 >= 0) && (x2 < self->size[2]) && (x3 >= 0) && (x3 < self->size[3]), 2, "out of range");
  return (THStorageAPI.data(self->storage)+self->storageOffset)[x0+x1*self->stride[1]+x2*self->stride[2]+x3*self->stride[3]];
}

static real* THTensor_data(THTensor *self)
{
  return THStorageAPI.data(self->storage)+self->storageOffset;
}
    
static real* THTensor_data1d(THTensor *self, long i0)
{
  return THStorageAPI.data(self->storage)+self->storageOffset+i0*self->stride[0];
}

static real* THTensor_data2d(THTensor *self, long i0, long i1)
{
  return THStorageAPI.data(self->storage)+self->storageOffset+i0*self->stride[0]+i1*self->stride[1];
}

static real* THTensor_data3d(THTensor *self, long i0, long i1, long i2)
{
  return THStorageAPI.data(self->storage)+self->storageOffset+i0*self->stride[0]+i1*self->stride[1]+i2*self->stride[2];
}

static real* THTensor_data4d(THTensor *self, long i0, long i1, long i2, long i3)
{
  return THStorageAPI.data(self->storage)+self->storageOffset+i0*self->stride[0]+i1*self->stride[1]+i2*self->stride[2]+i3*self->stride[3];
}

static long THTensor_nElement(THTensor *self)
{
  int nDimension = self->nDimension;
  if(nDimension == 0)
    return 0;
  else
    return self->stride[self->nDimension-1]*self->size[nDimension-1];
}

static void THTensor_retain(THTensor *self)
{
  if(self)
    ++self->refcount;
}

static void THTensor_free(THTensor *self)
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
  THTensor_alloc,
  THTensor_allocWithTensor,
  THTensor_allocWithTensorNarrow,
  THTensor_allocWithTensorSelect,
  THTensor_allocWithStorage,
  THTensor_allocWithStorage1d,
  THTensor_allocWithStorage2d,
  THTensor_allocWithStorage3d,
  THTensor_allocWithStorage4d,
  THTensor_allocWithSize,
  THTensor_allocWithSize1d,
  THTensor_allocWithSize2d,
  THTensor_allocWithSize3d,
  THTensor_allocWithSize4d,
  THTensor_storage,
  THTensor_storageOffset,
  THTensor_nDimension,
  THTensor_size,
  THTensor_stride,
  THTensor_resize,
  THTensor_resizeAs,
  THTensor_resize1d,
  THTensor_resize2d,
  THTensor_resize3d,
  THTensor_resize4d,
  THTensor_set1d,
  THTensor_set2d,
  THTensor_set3d,
  THTensor_set4d,
  THTensor_get1d,
  THTensor_get2d,
  THTensor_get3d,
  THTensor_get4d,
  THTensor_data,
  THTensor_data1d,
  THTensor_data2d,
  THTensor_data3d,
  THTensor_data4d,
  THTensor_nElement,
  THTensor_retain,
  THTensor_free
};
