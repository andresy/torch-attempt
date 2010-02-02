#include "Storage.h"
#include <string.h>
#include <stdio.h>

template <typename T> Storage<T>::Storage()
{
  data = NULL;
  size = 0;
  refcount = 1;
}

template <typename T> Storage<T>::Storage(long size)
{
  data = new T[size];
  size = size;
  refcount = 1;
}


template <typename T> void Storage<T>::retain()
{
  ++refcount;
}

template <typename T> void Storage<T>::free()
{
  if(refcount > 0)
  {
    if(--refcount == 0)
    {
      delete this;
    }
  }
}

template <typename T> Storage<T>::~Storage()
{
  delete data;
}

template <typename T> void Storage<T>::resize(long size_, bool keepContent_)
{
  if(keepContent_)
  {
    T *data_ = new T[size];
    memcpy(data_, data, sizeof(T)*size);
    delete data;

    data = data_;
    size = size_;
  }
  else
  {
    delete data;
    data = new T[size_];
    size = size_;
  }
}

template <typename T> void Storage<T>::copy(Storage<T> *src)
{
  long i;
//DEBUG:  THArgCheck(storage->size == src->size, 2, "size mismatch");
  for(int i = 0; i < size; i++)
    data[i] = src->data[i];
}

#define IMPLEMENT_STORAGE_COPY(CST, ST)                          \
  template <typename T> void Storage<T>::copy##CST(Storage<ST> *src) \
  {                                                             \
    /* DEBUG: THArgCheck(storage->size == src->size, 2, "size mismatch");*/  \
    for(int i = 0; i < size; i++)                                \
      data[i] = (T)src->data[i];                          \
}

IMPLEMENT_STORAGE_COPY(Char, char)
IMPLEMENT_STORAGE_COPY(Short, short)
IMPLEMENT_STORAGE_COPY(Int, int)
IMPLEMENT_STORAGE_COPY(Long, long)
IMPLEMENT_STORAGE_COPY(Float, float)
IMPLEMENT_STORAGE_COPY(Double, double)

template class Storage<char>;
template class Storage<short>;
template class Storage<int>;
template class Storage<long>;
template class Storage<float>;
template class Storage<double>;
