#include "zozo3.h"
#include <iostream>

/* Empty init */
template <class T> THTensor<T>::THTensor(long size_)
{
  size = size_;
  stuff = new T[size_];

  for(int i = 0; i < size; i++)
    stuff[i] = i+42;
}

template <class T> void THTensor<T>::print()
{
  for(int i = 0; i < size; i++)
    std::cout << stuff[i] << " ";
  std::cout << "\n";
}

template class THTensor<char>;
template class THTensor<int>;
