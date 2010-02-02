#include <stdio.h>
#include <stdlib.h>

typedef struct THTensor_$real
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
$THTensor *THTensor_new_$real(void)
{
  $THTensor *tensor = malloc(sizeof($THTensor));
  tensor->size = NULL;
  tensor->stride = NULL;
  tensor->nDimension = 0;
  tensor->storage = malloc(sizeof(real)*tensor->size[1]);
  tensor->storageOffset = 0;
  tensor->ownStorage = 0;
  tensor->refcount = 1;
  return tensor;
}
