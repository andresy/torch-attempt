#include <stdio.h>
#include <stdlib.h>

template <class T> struct THTensor
{
    long size;
    T *stuff;
};

template <class T> THTensor<T>* THTensor_new(long size);
template <class T> void THTensor_print(THTensor<T> *tensor);
