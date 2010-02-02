#include <stdio.h>
#include <stdlib.h>

#define TYPEP_FUNC_(NAME,TYPE) NAME##_##TYPE
#define TYPEP_FUNC(NAME,TYPE) TYPEP_FUNC_(NAME, TYPE)
#define FUNC(NAME) TYPEP_FUNC(NAME, real)

#for (THTensor THStorage real) in (THTensor_char THStorage_char char) (THTensor_int THStorage_int int)

typedef struct FUNC(THTensor_)
{
    long *size;
    long *stride;
    int nDimension;
    
    real *storage;
    long storageOffset;
    int ownStorage;
    int refcount;

} THTensor;

/* Empty init */
THTensor *FUNC(THTensor_new)(void)
{
  THTensor *tensor = malloc(sizeof(THTensor));
  tensor->size = NULL;
  tensor->stride = NULL;
  tensor->nDimension = 0;
  tensor->storage = malloc(sizeof(real)*tensor->size[1]);
  tensor->storageOffset = 0;
  tensor->ownStorage = 0;
  tensor->refcount = 1;
  return tensor;
}

#endfor
