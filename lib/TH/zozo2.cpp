#include "zozo2.h"
#include <iostream>

/* Empty init */
template <class T> THTensor<T>* THTensor_new(long size)
{
  THTensor<T> *tensor = (THTensor<T>*)malloc(sizeof(THTensor<T>));
  tensor->size = size;
  tensor->stuff = (T*)malloc(sizeof(T)*tensor->size);

  for(int i = 0; i < size; i++)
    tensor->stuff[i] = i+42;

  return tensor;
}

template <class T> void THTensor_print(THTensor<T> *tensor)
{
  for(int i = 0; i < tensor->size; i++)
    std::cout << tensor->stuff[i] << " ";
  std::cout << "\n";
}

template THTensor<char>* THTensor_new(long size);
template void THTensor_print(THTensor<char> *tensor);

template THTensor<int>* THTensor_new(long size);
template void THTensor_print(THTensor<int> *tensor);
