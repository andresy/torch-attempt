#include <stdio.h>
#include <stdlib.h>

template <class T> struct THTensor
{
    long *size;
    long *stride;
    int nDimension;
    
    T *storage;
    long storageOffset;
    int ownStorage;
    int refcount;

};

/* Empty init */
template <class T> THTensor<T>* THTensor_new(void)
{
  THTensor<T> *tensor = (THTensor<T>*)malloc(sizeof(THTensor<T>));
  tensor->size = NULL;
  tensor->stride = NULL;
  tensor->nDimension = 0;
  tensor->storage = (T*)malloc(sizeof(T)*tensor->size[1]);
  tensor->storageOffset = 0;
  tensor->ownStorage = 0;
  tensor->refcount = 1;
  return tensor;
}


int main()
{
  THTensor<char> *aob;
  aob = THTensor_new<char>();
  return 0;
}
//* THTensor_new<char>(void);
